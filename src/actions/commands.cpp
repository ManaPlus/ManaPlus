/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "actions/commands.h"

#include "actormanager.h"
#include "configuration.h"
#include "game.h"
#include "party.h"

#include "actions/actiondef.h"

#include "being/flooritem.h"
#include "being/localplayer.h"
#include "being/playerrelations.h"
#include "being/homunculusinfo.h"
#include "being/playerinfo.h"

#include "const/resources/skill.h"

#include "gui/viewport.h"

#include "gui/popups/popupmenu.h"

#include "gui/shortcut/emoteshortcut.h"
#include "gui/shortcut/itemshortcut.h"

#include "gui/windows/mailwindow.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/inventorywindow.h"
#include "gui/windows/npcdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/shortcutwindow.h"
#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"

#include "gui/widgets/tabs/chat/whispertab.h"

#include "input/inputactionoperators.h"

#include "listeners/inputactionreplaylistener.h"

#include "net/adminhandler.h"
#include "net/chathandler.h"
#include "net/guildhandler.h"
#include "net/familyhandler.h"
#include "net/homunculushandler.h"
#include "net/mailhandler.h"
#include "net/net.h"
#include "net/npchandler.h"
#include "net/partyhandler.h"
#include "net/serverfeatures.h"

#include "resources/chatobject.h"

#include "resources/db/itemdb.h"

#include "resources/map/map.h"

#include "resources/skill/skillinfo.h"

#include "utils/booleanoptions.h"
#include "utils/chatutils.h"
#include "utils/copynpaste.h"
#include "utils/gmfunctions.h"
#include "utils/parameters.h"
#include "utils/process.h"

#include "debug.h"

namespace Actions
{

static std::string getNick(const InputEvent &event)
{
    std::string args = event.args;
    if (args.empty())
    {
        if (event.tab == nullptr ||
            event.tab->getType() != ChatTabType::WHISPER)
        {
            return std::string();
        }

        WhisperTab *const whisper = static_cast<WhisperTab *>(event.tab);
        if (whisper->getNick().empty())
        {
            // TRANSLATORS: change relation
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER);
            return std::string();
        }
        args = whisper->getNick();
    }
    return args;
}

static void reportRelation(const InputEvent &event,
                           const RelationT &rel,
                           const std::string &str1,
                           const std::string &str2)
{
    if (event.tab != nullptr)
    {
        if (playerRelations.getRelation(event.args) == rel)
        {
            // TRANSLATORS: unignore command
            event.tab->chatLog(str1, ChatMsgType::BY_SERVER);
        }
        else
        {
            // TRANSLATORS: unignore command
            event.tab->chatLog(str2, ChatMsgType::BY_SERVER);
        }
    }
}

static void changeRelation(const InputEvent &event,
                           const RelationT relation,
                           const std::string &relationText)
{
    std::string args = getNick(event);
    if (args.empty())
        return;

    if (playerRelations.getRelation(args) == relation)
    {
        if (event.tab != nullptr)
        {
            // TRANSLATORS: change relation
            event.tab->chatLog(strprintf(_("Player already %s!"),
                relationText.c_str()), ChatMsgType::BY_SERVER);
            return;
        }
    }
    else
    {
        playerRelations.setRelation(args, relation);
    }

    reportRelation(event,
        relation,
        // TRANSLATORS: change relation
        strprintf(_("Player successfully %s!"), relationText.c_str()),
        // TRANSLATORS: change relation
        strprintf(_("Player could not be %s!"), relationText.c_str()));
}

impHandler(chatAnnounce)
{
    if (adminHandler != nullptr)
    {
        adminHandler->announce(event.args);
        return true;
    }
    return false;
}

impHandler(chatIgnore)
{
    changeRelation(event, Relation::IGNORED, "ignored");
    return true;
}

impHandler(chatUnignore)
{
    std::string args = getNick(event);
    if (args.empty())
        return false;

    const RelationT rel = playerRelations.getRelation(args);
    if (rel != Relation::NEUTRAL && rel != Relation::FRIEND)
    {
        playerRelations.setRelation(args, Relation::NEUTRAL);
    }
    else
    {
        if (event.tab != nullptr)
        {
            // TRANSLATORS: unignore command
            event.tab->chatLog(_("Player wasn't ignored!"),
                ChatMsgType::BY_SERVER);
        }
        return true;
    }

    reportRelation(event,
        Relation::NEUTRAL,
        // TRANSLATORS: unignore command
        _("Player no longer ignored!"),
        // TRANSLATORS: unignore command
        _("Player could not be unignored!"));
    return true;
}

impHandler(chatErase)
{
    std::string args = getNick(event);
    if (args.empty())
        return false;

    if (playerRelations.getRelation(args) == Relation::ERASED)
    {
        if (event.tab != nullptr)
        {
            // TRANSLATORS: erase command
            event.tab->chatLog(_("Player already erased!"),
                ChatMsgType::BY_SERVER);
        }
        return true;
    }
    else
    {
        playerRelations.setRelation(args, Relation::ERASED);
    }

    reportRelation(event,
        Relation::ERASED,
        // TRANSLATORS: erase command
        _("Player no longer erased!"),
        // TRANSLATORS: erase command
        _("Player could not be erased!"));
    return true;
}

impHandler(chatFriend)
{
    // TRANSLATORS: adding friend command
    changeRelation(event, Relation::FRIEND, _("friend"));
    return true;
}

impHandler(chatDisregard)
{
    // TRANSLATORS: disregard command
    changeRelation(event, Relation::DISREGARDED, _("disregarded"));
    return true;
}

impHandler(chatNeutral)
{
    // TRANSLATORS: neutral command
    changeRelation(event, Relation::NEUTRAL, _("neutral"));
    return true;
}

impHandler(chatBlackList)
{
    // TRANSLATORS: blacklist command
    changeRelation(event, Relation::BLACKLISTED, _("blacklisted"));
    return true;
}

impHandler(chatEnemy)
{
    // TRANSLATORS: enemy command
    changeRelation(event, Relation::ENEMY2, _("enemy"));
    return true;
}

impHandler(chatNuke)
{
    if (actorManager == nullptr)
        return false;

    const std::string nick = getNick(event);
    Being *const being = actorManager->findBeingByName(
        nick, ActorType::Player);
    if (being == nullptr)
        return true;

    actorManager->addBlock(being->getId());
    actorManager->destroy(being);
    return true;
}

impHandler(chatAdd)
{
    if (chatWindow == nullptr)
        return false;

    if (event.args.empty())
        return true;

    STD_VECTOR<int> str;
    splitToIntVector(str, event.args, ',');
    if (str.empty())
        return true;

    int id = str[0];
    if (id == 0)
        return true;

    if (ItemDB::exists(id))
    {
        const std::string names = ItemDB::getNamesStr(str);
        if (!names.empty())
            chatWindow->addItemText(names);
        return true;
    }

    const FloorItem *const floorItem = actorManager->findItem(
        fromInt(id, BeingId));

    if (floorItem != nullptr)
    {
        str[0] =  floorItem->getItemId();
        const std::string names = ItemDB::getNamesStr(str);
        chatWindow->addItemText(names);
    }
    return true;
}

impHandler0(present)
{
    if (chatWindow != nullptr)
    {
        chatWindow->doPresent();
        return true;
    }
    return false;
}

impHandler0(printAll)
{
    if (actorManager != nullptr)
    {
        actorManager->printAllToChat();
        return true;
    }
    return false;
}

impHandler(move)
{
    int x = 0;
    int y = 0;

    if ((localPlayer != nullptr) && parse2Int(event.args, x, y))
    {
        localPlayer->setDestination(x, y);
        return true;
    }
    return false;
}

impHandler(setTarget)
{
    if ((actorManager == nullptr) || (localPlayer == nullptr))
        return false;

    Being *const target = actorManager->findNearestByName(event.args);
    if (target != nullptr)
        localPlayer->setTarget(target);
    return true;
}

impHandler(commandOutfit)
{
    if (outfitWindow != nullptr)
    {
        if (!event.args.empty())
        {
            const std::string op = event.args.substr(0, 1);
            if (op == "n")
            {
                outfitWindow->wearNextOutfit(true);
            }
            else if (op == "p")
            {
                outfitWindow->wearPreviousOutfit(true);
            }
            else
            {
                outfitWindow->wearOutfit(atoi(event.args.c_str()) - 1,
                    false, true);
            }
        }
        else
        {
            outfitWindow->wearOutfit(atoi(event.args.c_str()) - 1,
                false, true);
        }
        return true;
    }
    return false;
}

impHandler(commandEmote)
{
    if (localPlayer != nullptr)
    {
        localPlayer->emote(CAST_U8(atoi(event.args.c_str())));
        return true;
    }
    return false;
}

impHandler(awayMessage)
{
    if (localPlayer != nullptr)
    {
        localPlayer->setAway(event.args);
        return true;
    }
    return false;
}

impHandler(pseudoAway)
{
    if (localPlayer != nullptr)
    {
        localPlayer->setPseudoAway(event.args);
        localPlayer->updateStatus();
        return true;
    }
    return false;
}

impHandler(follow)
{
    if (localPlayer == nullptr)
        return false;

    if (!features.getBoolValue("allowFollow"))
        return false;

    if (!event.args.empty())
    {
        localPlayer->setFollow(event.args);
    }
    else if (event.tab != nullptr &&
             event.tab->getType() == ChatTabType::WHISPER)
    {
        localPlayer->setFollow(static_cast<WhisperTab*>(event.tab)->getNick());
    }
    else
    {
        const Being *const being = localPlayer->getTarget();
        if (being != nullptr)
            localPlayer->setFollow(being->getName());
    }
    return true;
}

impHandler(navigate)
{
    if ((localPlayer == nullptr) ||
        !localPlayer->canMove())
    {
        return false;
    }

    int x = 0;
    int y = 0;

    if (parse2Int(event.args, x, y))
        localPlayer->navigateTo(x, y);
    else
        localPlayer->navigateClean();
    return true;
}

impHandler(navigateTo)
{
    if ((localPlayer == nullptr) ||
        !localPlayer->canMove())
    {
        return false;
    }

    const std::string args = event.args;
    if (args.empty())
        return true;

    Being *const being = actorManager->findBeingByName(args);
    if (being != nullptr)
    {
        localPlayer->navigateTo(being->getTileX(), being->getTileY());
    }
    else if (localPlayer->isInParty())
    {
        const Party *const party = localPlayer->getParty();
        if (party != nullptr)
        {
            const PartyMember *const m = party->getMember(args);
            const PartyMember *const o = party->getMember(
                localPlayer->getName());
            if (m != nullptr &&
                o != nullptr &&
                m->getMap() == o->getMap())
            {
                localPlayer->navigateTo(m->getX(), m->getY());
            }
        }
    }
    return true;
}

impHandler(moveCamera)
{
    int x = 0;
    int y = 0;

    if (viewport == nullptr)
        return false;

    if (parse2Int(event.args, x, y))
        viewport->moveCameraToPosition(x * mapTileSize, y * mapTileSize);
    return true;
}

impHandler0(restoreCamera)
{
    if (viewport == nullptr)
        return false;

    viewport->returnCamera();
    return true;
}

impHandler(imitation)
{
    if (localPlayer == nullptr)
        return false;

    if (!event.args.empty())
    {
        localPlayer->setImitate(event.args);
    }
    else if (event.tab != nullptr &&
             event.tab->getType() == ChatTabType::WHISPER)
    {
        localPlayer->setImitate(static_cast<WhisperTab*>(
            event.tab)->getNick());
    }
    else
    {
        localPlayer->setImitate("");
    }
    return true;
}

impHandler(sendMail)
{
    const ServerTypeT type = Net::getNetworkType();
    if (type == ServerType::EATHENA || type == ServerType::EVOL2)
    {
        std::string name;
        std::string text;

        if (parse2Str(event.args, name, text))
        {
            // TRANSLATORS: quick mail message caption
            mailHandler->send(name, _("Quick message"), text);
        }
    }
    else if (serverConfig.getBoolValue("enableManaMarketBot"))
    {
        chatHandler->privateMessage("ManaMarket", "!mail " + event.args);
        return true;
    }
    return false;
}

impHandler(info)
{
    if (event.tab == nullptr ||
        localPlayer == nullptr ||
        Net::getNetworkType() == ServerType::TMWATHENA)
    {
        return false;
    }

    if (guildHandler != nullptr &&
        event.tab->getType() == ChatTabType::GUILD)
    {
        const Guild *const guild = localPlayer->getGuild();
        if (guild != nullptr)
            guildHandler->info();
    }
    return true;
}

impHandler(wait)
{
    if (localPlayer != nullptr)
    {
        localPlayer->waitFor(event.args);
        return true;
    }
    return false;
}

impHandler(addPriorityAttack)
{
    if ((actorManager == nullptr) ||
        actorManager->isInPriorityAttackList(event.args))
    {
        return false;
    }

    actorManager->removeAttackMob(event.args);
    actorManager->addPriorityAttackMob(event.args);

    if (socialWindow != nullptr)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(addAttack)
{
    if (actorManager == nullptr)
        return false;

    actorManager->removeAttackMob(event.args);
    actorManager->addAttackMob(event.args);

    if (socialWindow != nullptr)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(removeAttack)
{
    if (actorManager == nullptr)
        return false;

    if (event.args.empty())
    {
        if (actorManager->isInAttackList(event.args))
        {
            actorManager->removeAttackMob(event.args);
            actorManager->addIgnoreAttackMob(event.args);
        }
        else
        {
            actorManager->removeAttackMob(event.args);
            actorManager->addAttackMob(event.args);
        }
    }
    else
    {
        actorManager->removeAttackMob(event.args);
    }


    if (socialWindow != nullptr)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(addIgnoreAttack)
{
    if (actorManager == nullptr)
        return false;

    actorManager->removeAttackMob(event.args);
    actorManager->addIgnoreAttackMob(event.args);

    if (socialWindow != nullptr)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(setDrop)
{
    GameModifiers::setQuickDropCounter(atoi(event.args.c_str()));
    return true;
}

impHandler(url)
{
    if (event.tab != nullptr)
    {
        std::string url1 = event.args;
        if (!strStartWith(url1, "http") && !strStartWith(url1, "?"))
            url1 = "http://" + url1;
        std::string str(strprintf("[@@%s |%s@@]",
            url1.c_str(), event.args.c_str()));
        outStringNormal(event.tab, str, str);
        return true;
    }
    return false;
}

impHandler(openUrl)
{
    std::string url = event.args;
    if (!strStartWith(url, "http"))
        url = "http://" + url;
    openBrowser(url);
    return true;
}

impHandler(execute)
{
    const size_t idx = event.args.find(' ');
    std::string name;
    std::string params;
    if (idx == std::string::npos)
    {
        name = event.args;
    }
    else
    {
        name = event.args.substr(0, idx);
        params = event.args.substr(idx + 1);
    }
    execFile(name, name, params, "");
    return true;
}

impHandler(enableHighlight)
{
    if (event.tab != nullptr)
    {
        event.tab->setAllowHighlight(true);
        if (chatWindow != nullptr)
        {
            chatWindow->saveState();
            return true;
        }
    }
    return false;
}

impHandler(disableHighlight)
{
    if (event.tab != nullptr)
    {
        event.tab->setAllowHighlight(false);
        if (chatWindow != nullptr)
        {
            chatWindow->saveState();
            return true;
        }
    }
    return false;
}

impHandler(dontRemoveName)
{
    if (event.tab != nullptr)
    {
        event.tab->setRemoveNames(false);
        if (chatWindow != nullptr)
        {
            chatWindow->saveState();
            return true;
        }
    }
    return false;
}

impHandler(removeName)
{
    if (event.tab != nullptr)
    {
        event.tab->setRemoveNames(true);
        if (chatWindow != nullptr)
        {
            chatWindow->saveState();
            return true;
        }
    }
    return false;
}

impHandler(disableAway)
{
    if (event.tab != nullptr)
    {
        event.tab->setNoAway(true);
        if (chatWindow != nullptr)
        {
            chatWindow->saveState();
            return true;
        }
    }
    return false;
}

impHandler(enableAway)
{
    if (event.tab != nullptr)
    {
        event.tab->setNoAway(false);
        if (chatWindow != nullptr)
        {
            chatWindow->saveState();
            return true;
        }
    }
    return false;
}

impHandler(testParticle)
{
    if (localPlayer != nullptr)
    {
        localPlayer->setTestParticle(event.args);
        return true;
    }
    return false;
}

impHandler(talkRaw)
{
    if (chatHandler != nullptr)
    {
        chatHandler->talkRaw(event.args);
        return true;
    }
    return false;
}

impHandler(gm)
{
    if (chatHandler != nullptr)
    {
        Gm::runCommand("wgm", event.args);
        return true;
    }
    return false;
}

impHandler(hack)
{
    if (chatHandler != nullptr)
    {
        chatHandler->sendRaw(event.args);
        return true;
    }
    return false;
}

impHandler(debugSpawn)
{
    if (localPlayer == nullptr)
        return false;
    int cnt = atoi(event.args.c_str());
    if (cnt < 1)
        cnt = 1;
    const int half = cnt / 2;
    const Map *const map = localPlayer->getMap();
    int x1 = -half;
    if (x1 < 0)
        x1 = 0;
    int y1 = x1;
    int x2 = cnt - half;
    if (x2 > map->getWidth())
        x2 = map->getWidth();
    int y2 = x2;

    for (int x = x1; x < x2; x ++)
    {
        for (int y = y1; y < y2; y ++)
            ActorManager::cloneBeing(localPlayer, x, y, cnt);
    }
    return true;
}

impHandler(serverIgnoreWhisper)
{
    std::string args = getNick(event);
    if (args.empty())
        return false;

    if (chatHandler != nullptr)
    {
        chatHandler->ignore(args);
        return true;
    }
    return false;
}

impHandler(serverUnIgnoreWhisper)
{
    std::string args = getNick(event);
    if (args.empty())
        return false;

    if (chatHandler != nullptr)
    {
        chatHandler->unIgnore(args);
        return true;
    }
    return false;
}

impHandler(setHomunculusName)
{
    const std::string args = event.args;
    if (args.empty())
    {
        const HomunculusInfo *const info = PlayerInfo::getHomunculus();
        if (info != nullptr)
        {
            // TRANSLATORS: dialog header
            inputActionReplayListener.openDialog(_("Rename your homun"),
                info->name,
                InputAction::HOMUNCULUS_SET_NAME);
        }
        return false;
    }

    if (homunculusHandler != nullptr)
    {
        homunculusHandler->setName(args);
        return true;
    }
    return false;
}

impHandler0(fireHomunculus)
{
    if (homunculusHandler != nullptr)
    {
        homunculusHandler->fire();
        return true;
    }
    return false;
}

impHandler0(leaveParty)
{
    if (partyHandler != nullptr)
    {
        partyHandler->leave();
        return true;
    }
    return false;
}

impHandler0(leaveGuild)
{
    if ((guildHandler != nullptr) && (localPlayer != nullptr))
    {
        const Guild *const guild = localPlayer->getGuild();
        if (guild != nullptr)
            guildHandler->leave(guild->getId());
        return true;
    }
    return false;
}

impHandler(warp)
{
    int x = 0;
    int y = 0;

    if ((adminHandler != nullptr) &&
        (Game::instance() != nullptr) &&
        parse2Int(event.args, x, y))
    {
        adminHandler->warp(Game::instance()->getCurrentMapName(),
            x, y);
        return true;
    }
    return false;
}

impHandler(homunTalk)
{
    if ((serverFeatures == nullptr) || !serverFeatures->haveTalkPet())
        return false;

    std::string args = event.args;
    if (findCutFirst(args, "/me "))
        args = textToMe(args);
    if (homunculusHandler != nullptr)
    {
        homunculusHandler->talk(args);
        return true;
    }
    return false;
}

impHandler(homunEmote)
{
    if ((serverFeatures == nullptr) || !serverFeatures->haveTalkPet())
        return false;

    if ((homunculusHandler != nullptr) &&
        event.action >= InputAction::HOMUN_EMOTE_1 &&
        event.action <= InputAction::HOMUN_EMOTE_48)
    {
        const int emotion = event.action - InputAction::HOMUN_EMOTE_1;
        if (emoteShortcut != nullptr)
            homunculusHandler->emote(emoteShortcut->getEmote(emotion));
        if (Game::instance() != nullptr)
            Game::instance()->setValidSpeed();
        return true;
    }

    return false;
}

impHandler(commandHomunEmote)
{
    if ((serverFeatures == nullptr) || !serverFeatures->haveTalkPet())
        return false;

    if (homunculusHandler != nullptr)
    {
        homunculusHandler->emote(CAST_U8(
            atoi(event.args.c_str())));
        return true;
    }
    return false;
}

impHandler(createPublicChatRoom)
{
    if ((chatHandler == nullptr) || event.args.empty())
        return false;
    chatHandler->createChatRoom(event.args, "", 100, true);
    return true;
}

impHandler(joinChatRoom)
{
    if (chatHandler == nullptr)
        return false;
    const std::string args = event.args;
    if (args.empty())
        return false;
    ChatObject *const chat = ChatObject::findByName(args);
    if (chat == nullptr)
        return false;
    chatHandler->joinChat(chat, "");
    return true;
}

impHandler0(leaveChatRoom)
{
    if (chatHandler != nullptr)
    {
        chatHandler->leaveChatRoom();
        return true;
    }
    return false;
}

impHandler(confSet)
{
    std::string name;
    std::string val;

    if (parse2Str(event.args, name, val))
    {
        config.setValue(name, val);
        return true;
    }
    return false;
}

impHandler(serverConfSet)
{
    std::string name;
    std::string val;

    if (parse2Str(event.args, name, val))
    {
        serverConfig.setValue(name, val);
        return true;
    }
    return false;
}

impHandler(confGet)
{
    const std::string args = event.args;
    if (args.empty())
        return false;

    // TRANSLATORS: result from command /confget
    const std::string str = strprintf(_("Config value: %s"),
        config.getStringValue(args).c_str());
    outStringNormal(event.tab, str, str);
    return true;
}

impHandler(serverConfGet)
{
    const std::string args = event.args;
    if (args.empty())
        return false;

    // TRANSLATORS: result from command /serverconfget
    const std::string str = strprintf(_("Server config value: %s"),
        serverConfig.getStringValue(args).c_str());
    outStringNormal(event.tab, str, str);
    return true;
}

impHandler(slide)
{
    int x = 0;
    int y = 0;

    if ((adminHandler != nullptr) && parse2Int(event.args, x, y))
    {
        adminHandler->slide(x, y);
        return true;
    }
    return false;
}

impHandler(selectSkillLevel)
{
    int skill = 0;
    int level = 0;

    if ((skillDialog != nullptr) && parse2Int(event.args, skill, level))
    {
        skillDialog->selectSkillLevel(skill, level);
        return true;
    }
    return false;
}

impHandler(skill)
{
    if (skillDialog == nullptr)
        return false;

    StringVect vect;
    splitToStringVector(vect, event.args, ' ');
    const int sz = CAST_S32(vect.size());
    if (sz < 1)
        return true;
    const int skillId = atoi(vect[0].c_str());
    int level = 0;
    std::string text;
    if (sz > 1)
    {
        level = atoi(vect[1].c_str());
        if (sz > 2)
            text = vect[2];
    }
    // +++ add here also cast type and offsets
    if (text.empty())
    {
        skillDialog->useSkill(skillId,
            AutoTarget_true,
            level,
            false,
            "",
            CastType::Default,
            0,
            0);
    }
    else
    {
        skillDialog->useSkill(skillId,
            AutoTarget_true,
            level,
            true,
            text,
            CastType::Default,
            0,
            0);
    }
    return true;
}

impHandler(craft)
{
    const std::string args = event.args;
    if (args.empty() || (inventoryWindow == nullptr))
        return false;

    inventoryWindow->moveItemToCraft(atoi(args.c_str()));
    return true;
}

impHandler(npcClipboard)
{
    if (npcHandler != nullptr)
    {
        int x = 0;
        int y = 0;

        NpcDialog *const dialog = npcHandler->getCurrentNpcDialog();

        if ((dialog != nullptr) && parse2Int(event.args, x, y))
        {
            dialog->copyToClipboard(x, y);
            return true;
        }
    }
    return false;
}

impHandler(clipboardCopy)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    sendBuffer(args);
    return true;
}

impHandler(addPickup)
{
    if (actorManager != nullptr)
    {
        actorManager->removePickupItem(event.args);
        actorManager->addPickupItem(event.args);
        if (socialWindow != nullptr)
            socialWindow->updatePickupFilter();
        return true;
    }
    return false;
}

impHandler(removePickup)
{
    if (actorManager != nullptr)
    {
        if (event.args.empty())
        {   // default pickup manipulation
            if (actorManager->checkDefaultPickup())
            {
                actorManager->removePickupItem(event.args);
                actorManager->addIgnorePickupItem(event.args);
            }
            else
            {
                actorManager->removePickupItem(event.args);
                actorManager->addPickupItem(event.args);
            }
        }
        else
        {   // any other pickups
            actorManager->removePickupItem(event.args);
        }
        if (socialWindow != nullptr)
            socialWindow->updatePickupFilter();
        return true;
    }
    return false;
}

impHandler(ignorePickup)
{
    if (actorManager != nullptr)
    {
        actorManager->removePickupItem(event.args);
        actorManager->addIgnorePickupItem(event.args);
        if (socialWindow != nullptr)
            socialWindow->updatePickupFilter();
        return true;
    }
    return false;
}

impHandler(monsterInfo)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->monsterInfo(args);
    return true;
}

impHandler(itemInfo)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->itemInfo(args);
    return true;
}

impHandler(whoDrops)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->whoDrops(args);
    return true;
}

impHandler(mobSearch)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->mobSearch(args);
    return true;
}

impHandler(mobSpawnSearch)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->mobSpawnSearch(args);
    return true;
}

impHandler(playerGmCommands)
{
    adminHandler->playerGmCommands(event.args);
    return true;
}

impHandler(playerCharGmCommands)
{
    adminHandler->playerCharGmCommands(event.args);
    return true;
}

impHandler(commandShowLevel)
{
    adminHandler->showLevel(event.args);
    return true;
}

impHandler(commandShowStats)
{
    adminHandler->showStats(event.args);
    return true;
}

impHandler(commandShowStorage)
{
    adminHandler->showStorageList(event.args);
    return true;
}

impHandler(commandShowCart)
{
    adminHandler->showCartList(event.args);
    return true;
}

impHandler(commandShowInventory)
{
    adminHandler->showInventoryList(event.args);
    return true;
}

impHandler(locatePlayer)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->locatePlayer(args);
    return true;
}

impHandler(commandShowAccountInfo)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->showAccountInfo(args);
    return true;
}

impHandler(commandSpawn)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->spawn(args);
    return true;
}

impHandler(commandSpawnSlave)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->spawnSlave(args);
    return true;
}

impHandler(commandSpawnClone)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->spawnClone(args);
    return true;
}

impHandler(commandSpawnSlaveClone)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->spawnSlaveClone(args);
    return true;
}

impHandler(commandSpawnEvilClone)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->spawnEvilClone(args);
    return true;
}

impHandler(commandSavePosition)
{
    adminHandler->savePosition(event.args);
    return true;
}

impHandler(commandLoadPosition)
{
    adminHandler->loadPosition(event.args);
    return true;
}

impHandler(commandRandomWarp)
{
    adminHandler->randomWarp(event.args);
    return true;
}

impHandler(commandGotoNpc)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->gotoNpc(args);
    return true;
}

impHandler(commandGotoPc)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->gotoName(args);
    return true;
}

impHandler(commandRecallPc)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->recallName(args);
    return true;
}

impHandler(commandIpCheck)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->ipcheckName(args);
    return true;
}

impHandler(commandKiller)
{
    adminHandler->killer(event.args);
    return true;
}

impHandler(commandKillable)
{
    adminHandler->killable(event.args);
    return true;
}

impHandler(commandHeal)
{
    adminHandler->heal(event.args);
    return true;
}

impHandler(commandAlive)
{
    adminHandler->alive(event.args);
    return true;
}

impHandler(commandDisguise)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->disguise(args);
    return true;
}

impHandler(commandImmortal)
{
    adminHandler->immortal(event.args);
    return true;
}

impHandler(commandHide)
{
    adminHandler->hide(event.args);
    return true;
}

impHandler(commandNuke)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->nuke(args);
    return true;
}

impHandler(commandKill)
{
    adminHandler->kill(event.args);
    return true;
}

impHandler(commandJail)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->jail(args);
    return true;
}

impHandler(commandUnjail)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->unjail(args);
    return true;
}

impHandler(commandNpcMove)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    StringVect pars;
    if (!splitParameters(pars, args, " ,", '\"'))
        return false;

    if (pars.size() != 3)
        return false;

    adminHandler->npcMove(pars[0],
        atoi(pars[1].c_str()),
        atoi(pars[2].c_str()));
    return true;
}

impHandler(commandNpcHide)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->hideNpc(args);
    return true;
}

impHandler(commandNpcShow)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->showNpc(args);
    return true;
}

impHandler(commandChangePartyLeader)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->changePartyLeader(args);
    return true;
}

impHandler(commandPartyRecall)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->partyRecall(args);
    return true;
}

impHandler(commandBreakGuild)
{
    adminHandler->breakGuild(event.args);
    return true;
}

impHandler(commandGuildRecall)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    adminHandler->guildRecall(args);
    return true;
}

impHandler(mailTo)
{
    if (mailWindow == nullptr)
        return false;
    mailWindow->createMail(event.args);
    return true;
}

impHandler(adoptChild)
{
    const std::string nick = getNick(event);
    Being *const being = actorManager->findBeingByName(
        nick, ActorType::Player);
    if (being == nullptr)
        return true;
    familyHandler->askForChild(being);
    return true;
}

impHandler(showSkillLevels)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    const SkillInfo *restrict const skill = skillDialog->getSkill(
        atoi(args.c_str()));
    if (skill == nullptr)
        return false;
    popupMenu->showSkillLevelPopup(skill);
    return true;
}

impHandler(showSkillType)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    const SkillInfo *restrict const skill = skillDialog->getSkill(
        atoi(args.c_str()));
    if (skill == nullptr)
        return false;
    popupMenu->showSkillTypePopup(skill);
    return true;
}

impHandler(selectSkillType)
{
    int skill = 0;
    int type = 0;

    if ((skillDialog != nullptr) && parse2Int(event.args, skill, type))
    {
        skillDialog->selectSkillCastType(skill,
            static_cast<CastTypeT>(type));
        return true;
    }
    return false;
}

impHandler(showSkillOffsetX)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    const SkillInfo *restrict const skill = skillDialog->getSkill(
        atoi(args.c_str()));
    if (skill == nullptr)
        return false;
    popupMenu->showSkillOffsetPopup(skill, true);
    return true;
}

impHandler(showSkillOffsetY)
{
    const std::string args = event.args;
    if (args.empty())
        return false;
    const SkillInfo *restrict const skill = skillDialog->getSkill(
        atoi(args.c_str()));
    if (skill == nullptr)
        return false;
    popupMenu->showSkillOffsetPopup(skill, false);
    return true;
}

impHandler(setSkillOffsetX)
{
    int skill = 0;
    int offset = 0;

    if ((skillDialog != nullptr) && parse2Int(event.args, skill, offset))
    {
        skillDialog->setSkillOffsetX(skill, offset);
        return true;
    }
    return false;
}

impHandler(setSkillOffsetY)
{
    int skill = 0;
    int offset = 0;

    if ((skillDialog != nullptr) && parse2Int(event.args, skill, offset))
    {
        skillDialog->setSkillOffsetY(skill, offset);
        return true;
    }
    return false;
}

impHandler(partyItemShare)
{
    if (localPlayer == nullptr)
        return false;

    if (localPlayer->isInParty() == false)
        return true;

    ChatTab *tab = event.tab;
    if (tab == nullptr)
        tab = localChatTab;
    if (tab == nullptr)
        return true;

    const std::string args = event.args;
    if (args.empty())
    {
        switch (partyHandler->getShareItems())
        {
            case PartyShare::YES:
                // TRANSLATORS: chat message
                tab->chatLog(_("Item sharing enabled."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::NO:
                // TRANSLATORS: chat message
                tab->chatLog(_("Item sharing disabled."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::NOT_POSSIBLE:
                // TRANSLATORS: chat message
                tab->chatLog(_("Item sharing not possible."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::UNKNOWN:
                // TRANSLATORS: chat message
                tab->chatLog(_("Item sharing unknown."),
                    ChatMsgType::BY_SERVER);
                return true;
            default:
                break;
        }
    }

    const signed char opt = parseBoolean(args);

    switch (opt)
    {
        case 1:
            partyHandler->setShareItems(
                PartyShare::YES);
            break;
        case 0:
            partyHandler->setShareItems(
                PartyShare::NO);
            break;
        case -1:
            tab->chatLog(strprintf(BOOLEAN_OPTIONS, "item"),
                ChatMsgType::BY_SERVER);
            break;
        default:
            break;
    }
    return true;
}

impHandler(partyExpShare)
{
    if (localPlayer == nullptr)
        return false;

    if (localPlayer->isInParty() == false)
        return true;

    ChatTab *tab = event.tab;
    if (tab == nullptr)
        tab = localChatTab;
    if (tab == nullptr)
        return true;

    const std::string args = event.args;
    if (args.empty())
    {
        switch (partyHandler->getShareExperience())
        {
            case PartyShare::YES:
                // TRANSLATORS: chat message
                tab->chatLog(_("Experience sharing enabled."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::NO:
                // TRANSLATORS: chat message
                tab->chatLog(_("Experience sharing disabled."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::NOT_POSSIBLE:
                // TRANSLATORS: chat message
                tab->chatLog(_("Experience sharing not possible."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::UNKNOWN:
                // TRANSLATORS: chat message
                tab->chatLog(_("Experience sharing unknown."),
                    ChatMsgType::BY_SERVER);
                return true;
            default:
                break;
        }
    }

    const signed char opt = parseBoolean(args);

    switch (opt)
    {
        case 1:
            partyHandler->setShareExperience(
                PartyShare::YES);
            break;
        case 0:
            partyHandler->setShareExperience(
                PartyShare::NO);
            break;
        case -1:
            tab->chatLog(strprintf(BOOLEAN_OPTIONS, "exp"),
                ChatMsgType::BY_SERVER);
            break;
        default:
            break;
    }
    return true;
}

impHandler(partyAutoItemShare)
{
    if (localPlayer == nullptr)
        return false;

    if (localPlayer->isInParty() == false)
        return true;

    ChatTab *tab = event.tab;
    if (tab == nullptr)
        tab = localChatTab;
    if (tab == nullptr)
        return true;

    const std::string args = event.args;
    if (args.empty())
    {
        switch (partyHandler->getShareAutoItems())
        {
            case PartyShare::YES:
                // TRANSLATORS: chat message
                tab->chatLog(_("Auto item sharing enabled."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::NO:
                // TRANSLATORS: chat message
                tab->chatLog(_("Auto item sharing disabled."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::NOT_POSSIBLE:
                // TRANSLATORS: chat message
                tab->chatLog(_("Auto item sharing not possible."),
                    ChatMsgType::BY_SERVER);
                return true;
            case PartyShare::UNKNOWN:
                // TRANSLATORS: chat message
                tab->chatLog(_("Auto item sharing unknown."),
                    ChatMsgType::BY_SERVER);
                return true;
            default:
                break;
        }
    }

    const signed char opt = parseBoolean(args);

    switch (opt)
    {
        case 1:
            partyHandler->setShareAutoItems(
                PartyShare::YES);
            break;
        case 0:
            partyHandler->setShareAutoItems(
                PartyShare::NO);
            break;
        case -1:
            tab->chatLog(strprintf(BOOLEAN_OPTIONS, "item"),
                ChatMsgType::BY_SERVER);
            break;
        default:
            break;
    }
    return true;
}

impHandler0(outfitToChat)
{
    if ((outfitWindow == nullptr) || (chatWindow == nullptr))
        return false;

    const std::string str = outfitWindow->getOutfitString();
    if (!str.empty())
        chatWindow->addInputText(str);
    return true;
}

impHandler0(outfitClear)
{
    if (outfitWindow == nullptr)
        return false;

    outfitWindow->clearCurrentOutfit();
    return true;
}

impHandler(moveAttackUp)
{
    if (actorManager == nullptr)
        return false;
    const std::string args = event.args;
    const int idx = actorManager->getAttackMobIndex(args);
    if (idx > 0)
    {
        std::list<std::string> mobs
            = actorManager->getAttackMobs();
        std::list<std::string>::iterator it = mobs.begin();
        std::list<std::string>::iterator it2 = it;
        while (it != mobs.end())
        {
            if (*it == args)
            {
                -- it2;
                mobs.splice(it2, mobs, it);
                actorManager->setAttackMobs(mobs);
                actorManager->rebuildAttackMobs();
                break;
            }
            ++ it;
            ++ it2;
        }

        if (socialWindow != nullptr)
            socialWindow->updateAttackFilter();
        return true;
    }
    return false;
}

impHandler(moveAttackDown)
{
    if (actorManager == nullptr)
        return false;
    const std::string args = event.args;
    const int idx = actorManager->getAttackMobIndex(args);
    const int size = actorManager->getAttackMobsSize();
    if (idx + 1 < size)
    {
        std::list<std::string> mobs
            = actorManager->getAttackMobs();
        std::list<std::string>::iterator it = mobs.begin();
        std::list<std::string>::iterator it2 = it;
        while (it != mobs.end())
        {
            if (*it == args)
            {
                ++ it2;
                if (it2 == mobs.end())
                    break;

                mobs.splice(it, mobs, it2);
                actorManager->setAttackMobs(mobs);
                actorManager->rebuildAttackMobs();
                break;
            }
            ++ it;
            ++ it2;
        }

        if (socialWindow != nullptr)
            socialWindow->updateAttackFilter();
        return true;
    }
    return false;
}

impHandler(movePriorityAttackUp)
{
    if (actorManager == nullptr)
        return false;
    const std::string args = event.args;
    const int idx = actorManager->
        getPriorityAttackMobIndex(args);
    if (idx > 0)
    {
        std::list<std::string> mobs
            = actorManager->getPriorityAttackMobs();
        std::list<std::string>::iterator it = mobs.begin();
        std::list<std::string>::iterator it2 = it;
        while (it != mobs.end())
        {
            if (*it == args)
            {
                -- it2;
                mobs.splice(it2, mobs, it);
                actorManager->setPriorityAttackMobs(mobs);
                actorManager->rebuildPriorityAttackMobs();
                break;
            }
            ++ it;
            ++ it2;
        }

        if (socialWindow != nullptr)
            socialWindow->updateAttackFilter();
        return true;
    }
    return false;
}

impHandler(movePriorityAttackDown)
{
    if (actorManager == nullptr)
        return false;
    const std::string args = event.args;
    const int idx = actorManager
        ->getPriorityAttackMobIndex(args);
    const int size = actorManager->getPriorityAttackMobsSize();
    if (idx + 1 < size)
    {
        std::list<std::string> mobs
            = actorManager->getPriorityAttackMobs();
        std::list<std::string>::iterator it = mobs.begin();
        std::list<std::string>::iterator it2 = it;
        while (it != mobs.end())
        {
            if (*it == args)
            {
                ++ it2;
                if (it2 == mobs.end())
                    break;

                mobs.splice(it, mobs, it2);
                actorManager->setPriorityAttackMobs(mobs);
                actorManager->rebuildPriorityAttackMobs();
                break;
            }
            ++ it;
            ++ it2;
        }

        if (socialWindow != nullptr)
            socialWindow->updateAttackFilter();
        return true;
    }
    return false;
}

impHandler(addSkillShortcut)
{
    const std::string args = event.args;
    if (args.empty() ||
        itemShortcutWindow == nullptr)
    {
        return false;
    }
    const SkillInfo *restrict const skill = skillDialog->getSkill(
        atoi(args.c_str()));
    if (skill == nullptr)
        return false;

    const int num = itemShortcutWindow->getTabIndex();
    if (num < 0 ||
        num >= CAST_S32(SHORTCUT_TABS) ||
        num == CAST_S32(SHORTCUT_AUTO_TAB))
    {
        return false;
    }

    ItemShortcut *const selShortcut = itemShortcut[num];
    const int index = selShortcut->getFreeIndex();
    if (index < 0)
        return true;

    selShortcut->setItem(index,
        skill->id + SKILL_MIN_ID,
        fromInt(skill->customSelectedLevel, ItemColor));
    selShortcut->setItemData(index,
        skill->toDataStr());

//    popupMenu->showSkillLevelPopup(skill);
    return true;
}

}  // namespace Actions
