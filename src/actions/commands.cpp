/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2015  The ManaPlus Developers
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
#ifdef EATHENA_SUPPORT
#include "emoteshortcut.h"
#endif
#include "game.h"
#include "inventory.h"
#include "item.h"
#include "party.h"

#include "actions/actiondef.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/chatconsts.h"
#include "gui/viewport.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/outfitwindow.h"

#include "gui/widgets/tabs/chat/whispertab.h"

#include "net/adminhandler.h"
#include "net/chathandler.h"
#include "net/guildhandler.h"
#ifdef EATHENA_SUPPORT
#include "net/homunculushandler.h"
#endif
#include "net/partyhandler.h"
#include "net/serverfeatures.h"

#include "resources/iteminfo.h"
#include "resources/chatobject.h"

#include "utils/chatutils.h"
#include "utils/gettext.h"
#include "utils/process.h"

#include "debug.h"

namespace Actions
{

static std::string getNick(const InputEvent &event)
{
    std::string args = event.args;
    if (args.empty())
    {
        WhisperTab *const whisper = dynamic_cast<WhisperTab* const>(event.tab);
        if (!whisper || whisper->getNick().empty())
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
                           const PlayerRelation::Relation &rel,
                           const std::string &str1,
                           const std::string &str2)
{
    if (event.tab)
    {
        if (player_relations.getRelation(event.args) == rel)
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
                           const PlayerRelation::Relation relation,
                           const std::string &relationText)
{
    std::string args = getNick(event);
    if (args.empty())
        return;

    if (player_relations.getRelation(args) == relation)
    {
        if (event.tab)
        {
            // TRANSLATORS: change relation
            event.tab->chatLog(strprintf(_("Player already %s!"),
                relationText.c_str()), ChatMsgType::BY_SERVER);
            return;
        }
    }
    else
    {
        player_relations.setRelation(args, relation);
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
    adminHandler->announce(event.args);
    return true;
}

impHandler(chatIgnore)
{
    changeRelation(event, PlayerRelation::IGNORED, "ignored");
    return true;
}

impHandler(chatUnignore)
{
    std::string args = getNick(event);
    if (args.empty())
        return false;

    const PlayerRelation::Relation rel = player_relations.getRelation(args);
    if (rel != PlayerRelation::NEUTRAL && rel != PlayerRelation::FRIEND)
    {
        player_relations.setRelation(args, PlayerRelation::NEUTRAL);
    }
    else
    {
        if (event.tab)
        {
            // TRANSLATORS: unignore command
            event.tab->chatLog(_("Player wasn't ignored!"),
                ChatMsgType::BY_SERVER);
        }
        return true;
    }

    reportRelation(event,
        PlayerRelation::NEUTRAL,
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

    if (player_relations.getRelation(args) == PlayerRelation::ERASED)
    {
        if (event.tab)
        {
            // TRANSLATORS: erase command
            event.tab->chatLog(_("Player already erased!"),
                ChatMsgType::BY_SERVER);
        }
        return true;
    }
    else
    {
        player_relations.setRelation(args, PlayerRelation::ERASED);
    }

    reportRelation(event,
        PlayerRelation::ERASED,
        // TRANSLATORS: erase command
        _("Player no longer erased!"),
        // TRANSLATORS: erase command
        _("Player could not be erased!"));
    return true;
}

impHandler(chatFriend)
{
    // TRANSLATORS: adding friend command
    changeRelation(event, PlayerRelation::FRIEND, _("friend"));
    return true;
}

impHandler(chatDisregard)
{
    // TRANSLATORS: disregard command
    changeRelation(event, PlayerRelation::DISREGARDED, _("disregarded"));
    return true;
}

impHandler(chatNeutral)
{
    // TRANSLATORS: neutral command
    changeRelation(event, PlayerRelation::NEUTRAL, _("neutral"));
    return true;
}

impHandler(chatBlackList)
{
    // TRANSLATORS: blacklist command
    changeRelation(event, PlayerRelation::BLACKLISTED, _("blacklisted"));
    return true;
}

impHandler(chatEnemy)
{
    // TRANSLATORS: enemy command
    changeRelation(event, PlayerRelation::ENEMY2, _("enemy"));
    return true;
}

impHandler(chatNuke)
{
    if (!actorManager)
        return false;

    const std::string nick = getNick(event);
    Being *const being = actorManager->findBeingByName(
        nick, ActorType::Player);
    if (!being)
        return true;

    actorManager->addBlock(static_cast<uint32_t>(being->getId()));
    actorManager->destroy(being);
    return true;
}

impHandler(chatAdd)
{
    if (!chatWindow)
        return false;

    if (event.args.empty())
        return true;

    const int id = atoi(event.args.c_str());
    if (!id)
        return true;

    Inventory *const inv = PlayerInfo::getInventory();
    const Item *item = nullptr;
    if (inv)
    {
        // +++ need add also color here
        item = inv->findItem(id, 1);
    }

    if (item)
    {
        // +++ need add also color here
        chatWindow->addItemText(item->getInfo().getName());
        return true;
    }

    const FloorItem *const floorItem = actorManager->findItem(id);

    if (floorItem)
    {
        // +++ need add also color here
        chatWindow->addItemText(floorItem->getInfo().getName());
    }
    return true;
}

impHandler0(present)
{
    if (chatWindow)
    {
        chatWindow->doPresent();
        return true;
    }
    return false;
}

impHandler0(printAll)
{
    if (actorManager)
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

    if (localPlayer && parse2Int(event.args, x, y))
    {
        localPlayer->setDestination(x, y);
        return true;
    }
    return false;
}

impHandler(setTarget)
{
    if (!actorManager || !localPlayer)
        return false;

    Being *const target = actorManager->findNearestByName(event.args);
    if (target)
        localPlayer->setTarget(target);
    return true;
}

impHandler(commandOutfit)
{
    if (outfitWindow)
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
    if (localPlayer)
    {
        localPlayer->emote(static_cast<uint8_t>(atoi(event.args.c_str())));
        return true;
    }
    return false;
}

impHandler(awayMessage)
{
    if (localPlayer)
    {
        localPlayer->setAway(event.args);
        return true;
    }
    return false;
}

impHandler(pseudoAway)
{
    if (localPlayer)
    {
        localPlayer->setPseudoAway(event.args);
        localPlayer->updateStatus();
        return true;
    }
    return false;
}

impHandler(follow)
{
    if (!localPlayer)
        return false;

    if (!features.getBoolValue("allowFollow"))
        return false;

    if (!event.args.empty())
    {
        localPlayer->setFollow(event.args);
    }
    else if (event.tab && event.tab->getType() == ChatTabType::WHISPER)
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
    if (!localPlayer)
        return false;

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
    if (!localPlayer)
        return false;

    const std::string args = event.args;
    if (args.empty())
        return true;

    Being *const being = actorManager->findBeingByName(args);
    if (being)
    {
        localPlayer->navigateTo(being->getTileX(), being->getTileY());
    }
    else if (localPlayer->isInParty())
    {
        const Party *const party = localPlayer->getParty();
        if (party)
        {
            const PartyMember *const m = party->getMember(args);
            const PartyMember *const o = party->getMember(
                localPlayer->getName());
            if (m && o && m->getMap() == o->getMap())
                localPlayer->navigateTo(m->getX(), m->getY());
        }
    }
    return true;
}

impHandler(moveCamera)
{
    int x = 0;
    int y = 0;

    if (!viewport)
        return false;

    if (parse2Int(event.args, x, y))
        viewport->moveCameraToPosition(x * mapTileSize, y * mapTileSize);
    return true;
}

impHandler(imitation)
{
    if (!localPlayer)
        return false;

    if (!event.args.empty())
    {
        localPlayer->setImitate(event.args);
    }
    else if (event.tab && event.tab->getType() == ChatTabType::WHISPER)
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

impHandler0(sendMail)
{
    return false;
}

impHandler(info)
{
    if (!event.tab || !localPlayer
        || !serverFeatures->haveNativeGuilds())
    {
        return false;
    }

    if (event.tab->getType() == ChatTabType::GUILD)
    {
        const Guild *const guild = localPlayer->getGuild();
        if (guild)
            guildHandler->info();
    }
    return true;
}

impHandler(wait)
{
    if (localPlayer)
    {
        localPlayer->waitFor(event.args);
        return true;
    }
    return false;
}

impHandler(addPriorityAttack)
{
    if (!actorManager
        || actorManager->isInPriorityAttackList(event.args))
    {
        return false;
    }

    actorManager->removeAttackMob(event.args);
    actorManager->addPriorityAttackMob(event.args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(addAttack)
{
    if (!actorManager || actorManager->isInAttackList(event.args))
        return false;

    actorManager->removeAttackMob(event.args);
    actorManager->addAttackMob(event.args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(removeAttack)
{
    if (!actorManager || event.args.empty() || (
        !actorManager->isInIgnoreAttackList(event.args) &&
        !actorManager->isInPriorityAttackList(event.args)))
    {
        return false;
    }

    actorManager->removeAttackMob(event.args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
    return true;
}

impHandler(addIgnoreAttack)
{
    if (!actorManager || actorManager->isInIgnoreAttackList(event.args))
        return false;

    actorManager->removeAttackMob(event.args);
    actorManager->addIgnoreAttackMob(event.args);

    if (socialWindow)
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
    if (event.tab)
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
    const size_t idx = event.args.find(" ");
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
    if (event.tab)
    {
        event.tab->setAllowHighlight(true);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(disableHighlight)
{
    if (event.tab)
    {
        event.tab->setAllowHighlight(false);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(dontRemoveName)
{
    if (event.tab)
    {
        event.tab->setRemoveNames(false);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(removeName)
{
    if (event.tab)
    {
        event.tab->setRemoveNames(true);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(disableAway)
{
    if (event.tab)
    {
        event.tab->setNoAway(true);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(enableAway)
{
    if (event.tab)
    {
        event.tab->setNoAway(false);
        if (chatWindow)
            chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler(testParticle)
{
    if (localPlayer)
    {
        localPlayer->setTestParticle(event.args);
        return true;
    }
    return false;
}

impHandler(talkRaw)
{
    chatHandler->talkRaw(event.args);
    return true;
}

impHandler(gm)
{
    chatHandler->talk("@wgm " + event.args, GENERAL_CHANNEL);
    return true;
}

impHandler(hack)
{
    chatHandler->sendRaw(event.args);
    return true;
}

impHandler(debugSpawn)
{
    int cnt = atoi(event.args.c_str());
    if (cnt < 1)
        cnt = 1;
    const int half = cnt / 2;
    for (int x = -half; x < cnt - half; x ++)
    {
        for (int y =  -half; y < cnt - half; y ++)
            ActorManager::cloneBeing(localPlayer, x, y, cnt);
    }
    return true;
}

impHandler(serverIgnoreWhisper)
{
    std::string args = getNick(event);
    if (args.empty())
        return false;

    chatHandler->ignore(args);
    return true;
}

impHandler(serverUnIgnoreWhisper)
{
    std::string args = getNick(event);
    if (args.empty())
        return false;

    chatHandler->unIgnore(args);
    return true;
}

impHandler(setHomunculusName)
{
#ifdef EATHENA_SUPPORT
    const std::string args = event.args;
    if (args.empty())
        return false;

    homunculusHandler->setName(args);
    return true;
#else
    return false;
#endif
}

impHandler0(fireHomunculus)
{
#ifdef EATHENA_SUPPORT
    homunculusHandler->fire();
    return true;
#else
    return false;
#endif
}

impHandler0(leaveParty)
{
    partyHandler->leave();
    return true;
}

impHandler(warp)
{
    int x = 0;
    int y = 0;

    if (Game::instance() && parse2Int(event.args, x, y))
    {
        adminHandler->warp(Game::instance()->getCurrentMapName(),
            x, y);
    }
    return true;
}

impHandler(homunTalk)
{
#ifdef EATHENA_SUPPORT
    if (!serverFeatures->haveTalkPet())
        return false;

    std::string args = event.args;
    if (findCutFirst(args, "/me "))
        args = textToMe(args);
    homunculusHandler->talk(args);
    return true;
#else
    return false;
#endif
}

impHandler(homunEmote)
{
#ifdef EATHENA_SUPPORT
    if (!serverFeatures->haveTalkPet())
        return false;

    if (event.action >= InputAction::HOMUN_EMOTE_1
        && event.action <= InputAction::HOMUN_EMOTE_48)
    {
        const int emotion = event.action - InputAction::HOMUN_EMOTE_1;
        if (emoteShortcut)
            homunculusHandler->emote(emoteShortcut->getEmote(emotion));
        if (Game::instance())
            Game::instance()->setValidSpeed();
        return true;
    }
#endif

    return false;
}

impHandler(commandHomunEmote)
{
#ifdef EATHENA_SUPPORT
    if (!serverFeatures->haveTalkPet())
        return false;

    homunculusHandler->emote(static_cast<uint8_t>(
        atoi(event.args.c_str())));
    return true;
#else
    return false;
#endif
}

impHandler(createPublicChatRoom)
{
#ifdef EATHENA_SUPPORT
    if (event.args.empty())
        return false;
    chatHandler->createChatRoom(event.args, "", 100, true);
    return true;
#else
    return false;
#endif
}

impHandler(joinChatRoom)
{
#ifdef EATHENA_SUPPORT
    const std::string args = event.args;
    if (args.empty())
        return false;
    ChatObject *const chat = ChatObject::findByName(args);
    if (!chat)
        return false;
    chatHandler->joinChat(chat, "");
    return true;
#else
    return false;
#endif
}

impHandler0(leaveChatRoom)
{
#ifdef EATHENA_SUPPORT
    chatHandler->leaveChatRoom();
    return true;
#else
    return false;
#endif
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

    const std::string str = strprintf(_("Server config value: %s"),
        serverConfig.getStringValue(args).c_str());
    outStringNormal(event.tab, str, str);
    return true;
}

}  // namespace Actions
