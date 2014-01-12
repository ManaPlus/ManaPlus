/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "commands.h"

#include "auctionmanager.h"
#include "actormanager.h"
#include "client.h"
#include "configuration.h"
#include "game.h"
#include "graphicsmanager.h"
#include "guildmanager.h"
#include "main.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/playerrelations.h"

#include "gui/gui.h"

#include "gui/windows/buydialog.h"
#include "gui/windows/chatwindow.h"
#include "gui/windows/helpwindow.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/shopwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/tradewindow.h"

#include "gui/widgets/tabs/whispertab.h"

#if defined USE_OPENGL
#include "render/normalopenglgraphics.h"
#endif

#if defined USE_OPENGL && defined DEBUG_SDLFONT
#include "render/nullopenglgraphics.h"
#endif

#include "net/adminhandler.h"
#include "net/beinghandler.h"
#include "net/chathandler.h"
#include "net/gamehandler.h"
#include "net/guildhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"
#include "net/pethandler.h"
#include "net/tradehandler.h"

#ifdef DEBUG_DUMP_LEAKS1
#include "resources/image.h"
#include "resources/resource.h"
#include "resources/subimage.h"
#endif

#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "resources/db/itemdb.h"

#include "utils/gettext.h"
#include "utils/process.h"
#include "utils/timer.h"

#include "debug.h"

#define impHandler(name) void name(const std::string &args, ChatTab *const tab)
#define impHandler0(name) void name(const std::string &args A_UNUSED, \
    ChatTab *const tab A_UNUSED)
#define impHandler1(name) void name(const std::string &args, \
    ChatTab *const tab A_UNUSED)
#define impHandler2(name) void name(const std::string &args A_UNUSED, \
    ChatTab *const tab)

extern std::string tradePartnerName;
extern char **environ;

namespace Commands
{

static void outString(ChatTab *const tab, const std::string &str,
                      const std::string &def)
{
    if (!tab)
    {
        Net::getChatHandler()->me(def, GENERAL_CHANNEL);
        return;
    }

    switch (tab->getType())
    {
        case ChatTab::TAB_PARTY:
        {
            Net::getPartyHandler()->chat(str);
            break;
        }
        case ChatTab::TAB_GUILD:
        {
            if (!player_node)
                return;
            const Guild *const guild = player_node->getGuild();
            if (guild)
            {
                if (guild->getServerGuild())
                {
                    if (tmwServerVersion > 0)
                        return;
                    Net::getGuildHandler()->chat(guild->getId(), str);
                }
                else if (guildManager)
                {
                    guildManager->chat(str);
                }
            }
            break;
        }
        default:
            Net::getChatHandler()->me(def, GENERAL_CHANNEL);
            break;
    }
}

static void changeRelation(const std::string &args,
                           const PlayerRelation::Relation relation,
                           const std::string &relationText,
                           ChatTab *const tab)
{
    if (!tab)
        return;

    if (args.empty())
    {
        // TRANSLATORS: change relation
        tab->chatLog(_("Please specify a name."), BY_SERVER);
        return;
    }

    if (player_relations.getRelation(args) == relation)
    {
        // TRANSLATORS: change relation
        tab->chatLog(strprintf(_("Player already %s!"),
                     relationText.c_str()), BY_SERVER);
        return;
    }
    else
    {
        player_relations.setRelation(args, relation);
    }

    if (player_relations.getRelation(args) == relation)
    {
        // TRANSLATORS: change relation
        tab->chatLog(strprintf(_("Player successfully %s!"),
                     relationText.c_str()), BY_SERVER);
    }
    else
    {
        // TRANSLATORS: change relation
        tab->chatLog(strprintf(_("Player could not be %s!"),
                     relationText.c_str()), BY_SERVER);
    }
}

static bool parse2Int(const std::string &args, int &x, int &y)
{
    bool isValid = false;
    const size_t pos = args.find(" ");
    if (pos != std::string::npos)
    {
        if (pos + 1 < args.length())
        {
            x = atoi(args.substr(0, pos).c_str());
            y = atoi(args.substr(pos + 1, args.length()).c_str());
            isValid = true;
        }
    }
    return isValid;
}

static void outStringNormal(ChatTab *const tab,
                            const std::string &str,
                            const std::string &def)
{
    if (!player_node)
        return;

    if (!tab)
    {
        Net::getChatHandler()->talk(str, GENERAL_CHANNEL);
        return;
    }

    switch (tab->getType())
    {
        case ChatTab::TAB_PARTY:
        {
            Net::getPartyHandler()->chat(str);
            break;
        }
        case ChatTab::TAB_GUILD:
        {
            const Guild *const guild = player_node->getGuild();
            if (guild)
            {
                if (guild->getServerGuild())
                {
                    if (tmwServerVersion > 0)
                        return;
                    Net::getGuildHandler()->chat(guild->getId(), str);
                }
                else if (guildManager)
                {
                    guildManager->chat(str);
                }
            }
            break;
        }
        case ChatTab::TAB_WHISPER:
        {
            const WhisperTab *const whisper
                = static_cast<WhisperTab *const>(tab);
            tab->chatLog(player_node->getName(), str);
            Net::getChatHandler()->privateMessage(whisper->getNick(), str);
            break;
        }
        default:
            Net::getChatHandler()->talk(def, GENERAL_CHANNEL);
            break;
    }
}


impHandler1(announce)
{
    Net::getAdminHandler()->announce(args);
}

impHandler2(help)
{
    if (!helpWindow)
        return;

    if (!tab)
    {
        helpWindow->loadHelp("chatcommands");
        helpWindow->requestMoveToTop();
        return;
    }
    switch (tab->getType())
    {
        case ChatTab::TAB_PARTY:
        {
            helpWindow->loadHelp("chatparty");
            break;
        }
        case ChatTab::TAB_GUILD:
        {
            helpWindow->loadHelp("chatguild");
            break;
        }
        default:
            helpWindow->loadHelp("chatcommands");
            break;
    }
    helpWindow->requestMoveToTop();
}

impHandler2(where)
{
    std::ostringstream where;
    where << Game::instance()->getCurrentMapName() << ", coordinates: "
        << ((player_node->getPixelX() - mapTileSize / 2) / mapTileSize)
        << ", " << ((player_node->getPixelY() - mapTileSize) / mapTileSize);
    tab->chatLog(where.str(), BY_SERVER);
}

impHandler0(who)
{
    Net::getChatHandler()->who();
}

impHandler(msg)
{
    std::string recvnick;
    std::string msg;

    if (args.substr(0, 1) == "\"")
    {
        const size_t pos = args.find('"', 1);
        if (pos != std::string::npos)
        {
            recvnick = args.substr(1, pos - 1);
            if (pos + 2 < args.length())
                msg = args.substr(pos + 2, args.length());
        }
    }
    else
    {
        const size_t pos = args.find(" ");
        if (pos != std::string::npos)
        {
            recvnick = args.substr(0, pos);
            if (pos + 1 < args.length())
                msg = args.substr(pos + 1, args.length());
        }
        else
        {
            recvnick = std::string(args);
            msg.clear();
        }
    }

    trim(msg);

    if (msg.length() > 0)
    {
        std::string playerName = player_node->getName();
        std::string tempNick = recvnick;

        toLower(playerName);
        toLower(tempNick);

        if (tempNick.compare(playerName) == 0 || args.empty())
            return;

        chatWindow->addWhisper(recvnick, msg, BY_PLAYER);
    }
    else
    {
        // TRANSLATORS: whisper send
        tab->chatLog(_("Cannot send empty whispers!"), BY_SERVER);
    }
}

impHandler(query)
{
    if (chatWindow)
    {
        if (chatWindow->addWhisperTab(args, true))
        {
            chatWindow->saveState();
            return;
        }
    }

    if (tab)
    {
        // TRANSLATORS: new whisper query
        tab->chatLog(strprintf(_("Cannot create a whisper tab for nick "
            "\"%s\"! It either already exists, or is you."),
            args.c_str()), BY_SERVER);
    }
}

impHandler0(clear)
{
    if (chatWindow)
        chatWindow->clearTab();
}

impHandler0(cleanGraphics)
{
    ResourceManager::getInstance()->clearCache();

    if (debugChatTab)
    {
        // TRANSLATORS: clear graphics command message
        debugChatTab->chatLog(_("Cache cleaned"));
    }
}

impHandler0(cleanFonts)
{
    if (gui)
        gui->clearFonts();
    if (debugChatTab)
    {
        // TRANSLATORS: clear fonts cache message
        debugChatTab->chatLog(_("Cache cleaned"));
    }
}

impHandler(createParty)
{
    if (!tab)
        return;

    if (args.empty())
    {
        // TRANSLATORS: create party message
        tab->chatLog(_("Party name is missing."), BY_SERVER);
    }
    else
    {
        Net::getPartyHandler()->create(args);
    }
}

impHandler(createGuild)
{
    if (!tab || tmwServerVersion > 0)
        return;

    if (args.empty())
    {
        // TRANSLATORS: create guild message
        tab->chatLog(_("Guild name is missing."), BY_SERVER);
    }
    else
    {
        Net::getGuildHandler()->create(args);
    }
}

impHandler(party)
{
    if (!tab)
        return;

    if (!args.empty())
    {
        Net::getPartyHandler()->invite(args);
    }
    else
    {
        // TRANSLATORS: party invite message
        tab->chatLog(_("Please specify a name."), BY_SERVER);
    }
}

impHandler(me)
{
    outString(tab, strprintf("*%s*", args.c_str()), args);
}

impHandler(toggle)
{
    if (args.empty())
    {
        if (chatWindow && tab)
        {
            // TRANSLATORS: message from toggle chat command
            tab->chatLog(chatWindow->getReturnTogglesChat() ?
                _("Return toggles chat.") : _("Message closes chat."));
        }
        return;
    }

    switch (parseBoolean(args))
    {
        case 1:
            if (tab)
            {
                // TRANSLATORS: message from toggle chat command
                tab->chatLog(_("Return now toggles chat."));
            }
            if (chatWindow)
                chatWindow->setReturnTogglesChat(true);
            return;
        case 0:
            if (tab)
            {
                // TRANSLATORS: message from toggle chat command
                tab->chatLog(_("Message now closes chat."));
            }
            if (chatWindow)
                chatWindow->setReturnTogglesChat(false);
            return;
        case -1:
            if (tab)
                tab->chatLog(strprintf(BOOLEAN_OPTIONS, "toggle"));
            return;
        default:
            return;
    }
}

impHandler0(present)
{
    if (chatWindow)
        chatWindow->doPresent();
}

impHandler(ignore)
{
    changeRelation(args, PlayerRelation::IGNORED, "ignored", tab);
}

impHandler(beFriend)
{
    // TRANSLATORS: adding friend command
    changeRelation(args, PlayerRelation::FRIEND, _("friend"), tab);
}

impHandler(disregard)
{
    // TRANSLATORS: disregard command
    changeRelation(args, PlayerRelation::DISREGARDED, _("disregarded"), tab);
}

impHandler(neutral)
{
    // TRANSLATORS: neutral command
    changeRelation(args, PlayerRelation::NEUTRAL, _("neutral"), tab);
}

impHandler(unignore)
{
    if (args.empty())
    {
        if (tab)
        {
            // TRANSLATORS: unignore command
            tab->chatLog(_("Please specify a name."), BY_SERVER);
        }
        return;
    }

    const PlayerRelation::Relation rel = player_relations.getRelation(args);
    if (rel != PlayerRelation::NEUTRAL && rel != PlayerRelation::FRIEND)
    {
        player_relations.setRelation(args, PlayerRelation::NEUTRAL);
    }
    else
    {
        if (tab)
        {
            // TRANSLATORS: unignore command
            tab->chatLog(_("Player wasn't ignored!"), BY_SERVER);
        }
        return;
    }

    if (tab)
    {
        if (player_relations.getRelation(args) == PlayerRelation::NEUTRAL)
        {
            // TRANSLATORS: unignore command
            tab->chatLog(_("Player no longer ignored!"), BY_SERVER);
        }
        else
        {
            // TRANSLATORS: unignore command
            tab->chatLog(_("Player could not be unignored!"), BY_SERVER);
        }
    }
}

impHandler(blackList)
{
    // TRANSLATORS: blacklist command
    changeRelation(args, PlayerRelation::BLACKLISTED, _("blacklisted"), tab);
}

impHandler(enemy)
{
    // TRANSLATORS: enemy command
    changeRelation(args, PlayerRelation::ENEMY2, _("enemy"), tab);
}

impHandler(erase)
{
    if (args.empty())
    {
        if (tab)
        {
            // TRANSLATORS: erase command
            tab->chatLog(_("Please specify a name."), BY_SERVER);
        }
        return;
    }

    if (player_relations.getRelation(args) == PlayerRelation::ERASED)
    {
        if (tab)
        {
            // TRANSLATORS: erase command
            tab->chatLog(_("Player already erased!"), BY_SERVER);
        }
        return;
    }
    else
    {
        player_relations.setRelation(args, PlayerRelation::ERASED);
    }

    if (tab)
    {
        if (player_relations.getRelation(args) == PlayerRelation::ERASED)
        {
            // TRANSLATORS: erase command
            tab->chatLog(_("Player successfully erased!"), BY_SERVER);
        }
        else
        {
            // TRANSLATORS: erase command
            tab->chatLog(_("Player could not be erased!"), BY_SERVER);
        }
    }
}

impHandler0(quit)
{
//    quit();
}

impHandler0(showAll)
{
    if (actorManager)
        actorManager->printAllToChat();
}

impHandler1(move)
{
    int x = 0;
    int y = 0;

    if (player_node && parse2Int(args, x, y))
        player_node->moveTo(x, y);
}

impHandler1(navigate)
{
    if (!player_node)
        return;

    int x = 0;
    int y = 0;

    if (parse2Int(args, x, y))
        player_node->navigateTo(x, y);
    else
        player_node->navigateClean();
}

impHandler1(target)
{
    if (!actorManager || !player_node)
        return;

    Being *const target = actorManager->findNearestByName(args);
    if (target)
        player_node->setTarget(target);
}

impHandler0(attackHuman)
{
    if (!actorManager || !player_node)
        return;

    Being *const target = actorManager->findNearestLivingBeing(
        player_node, 10, ActorSprite::PLAYER);
    if (target)
    {
        if (player_node->checAttackPermissions(target))
        {
            player_node->setTarget(target);
            player_node->attack2(target, true);
        }
    }
}


impHandler0(closeAll)
{
    if (chatWindow)
    {
        chatWindow->removeAllWhispers();
        chatWindow->saveState();
    }
}

impHandler0(ignoreAll)
{
    if (chatWindow)
    {
        chatWindow->ignoreAllWhispers();
        chatWindow->saveState();
    }
}

impHandler1(outfit)
{
    if (outfitWindow)
    {
        if (!args.empty())
        {
            const std::string op = args.substr(0, 1);
            if (op == "n")
                outfitWindow->wearNextOutfit(true);
            else if (op == "p")
                outfitWindow->wearPreviousOutfit(true);
            else
                outfitWindow->wearOutfit(atoi(args.c_str()) - 1, false, true);
        }
        else
        {
            outfitWindow->wearOutfit(atoi(args.c_str()) - 1, false, true);
        }
    }
}

impHandler1(emote)
{
    if (player_node)
        player_node->emote(static_cast<uint8_t>(atoi(args.c_str())));
}

impHandler1(emotePet)
{
    // need use actual pet id
    Net::getPetHandler()->emote(static_cast<uint8_t>(atoi(args.c_str())), 0);
}

impHandler1(away)
{
    if (player_node)
        player_node->setAway(args);
}

impHandler1(pseudoAway)
{
    if (player_node)
    {
        player_node->setPseudoAway(args);
        player_node->updateStatus();
    }
}

impHandler(follow)
{
    if (!player_node)
        return;

    if (!features.getBoolValue("allowFollow"))
        return;

    if (!args.empty())
        player_node->setFollow(args);
    else if (tab && tab->getType() == ChatTab::TAB_WHISPER)
        player_node->setFollow(static_cast<WhisperTab*>(tab)->getNick());
}

impHandler(imitation)
{
    if (!player_node)
        return;

    if (!args.empty())
        player_node->setImitate(args);
    else if (tab && tab->getType() == ChatTab::TAB_WHISPER)
        player_node->setImitate(static_cast<WhisperTab*>(tab)->getNick());
    else
        player_node->setImitate("");
}

impHandler1(heal)
{
    if (!actorManager)
        return;

    if (!args.empty())
    {
        const Being *const being = actorManager->findBeingByName(
            args, Being::PLAYER);
        if (being)
            actorManager->heal(being);
    }
    else
    {
        actorManager->heal(player_node);
    }
}

impHandler1(hack)
{
    Net::getChatHandler()->sendRaw(args);
}

impHandler1(mail)
{
    if (auctionManager && auctionManager->getEnableAuctionBot())
        auctionManager->sendMail(args);
}

impHandler0(priceLoad)
{
    if (shopWindow)
        shopWindow->loadList();
}

impHandler0(priceSave)
{
    if (shopWindow)
        shopWindow->saveList();
}

impHandler0(disconnect)
{
    Net::getGameHandler()->disconnect2();
}

impHandler1(undress)
{
    if (!actorManager)
        return;

    Being *const target = actorManager->findNearestByName(args);
    if (target)
        Net::getBeingHandler()->undress(target);
}

impHandler1(attack)
{
    if (!player_node || !actorManager)
        return;

    Being *const target = actorManager->findNearestByName(args);
    if (target)
        player_node->setTarget(target);
    player_node->attack2(player_node->getTarget(), true);
}

impHandler1(trade)
{
    if (!actorManager)
        return;

    const Being *const being = actorManager->findBeingByName(
        args, Being::PLAYER);
    if (being)
    {
        Net::getTradeHandler()->request(being);
        tradePartnerName = being->getName();
        if (tradeWindow)
            tradeWindow->clear();
    }
}

impHandler0(dirs)
{
    if (!player_node || !debugChatTab)
        return;

    debugChatTab->chatLog("config directory: "
        + client->getConfigDirectory());
    debugChatTab->chatLog("logs directory: "
        + client->getLocalDataDirectory());
    debugChatTab->chatLog("screenshots directory: "
        + client->getScreenshotDirectory());
    debugChatTab->chatLog("temp directory: "
        + client->getTempDirectory());
}

impHandler2(info)
{
    if (!tab || !player_node || tmwServerVersion > 0)
        return;

    switch (tab->getType())
    {
        case ChatTab::TAB_GUILD:
        {
            const Guild *const guild = player_node->getGuild();
            if (guild)
                Net::getGuildHandler()->info(guild->getId());
            break;
        }
        default:
            break;
    }
}

impHandler1(wait)
{
    if (player_node)
        player_node->waitFor(args);
}

impHandler0(uptime)
{
    if (!debugChatTab)
        return;

    if (cur_time < start_time)
    {
        // TRANSLATORS: uptime command
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), "unknown"));
    }
    else
    {
        std::string str;
        int timeDiff = cur_time - start_time;

        const int weeks = timeDiff / 60 / 60 / 24 / 7;
        if (weeks > 0)
        {
            // TRANSLATORS: uptime command
            str = strprintf(ngettext(N_("%d week"), N_("%d weeks"),
                weeks), weeks);
            timeDiff -= weeks * 60 * 60 * 24 * 7;
        }

        const int days = timeDiff / 60 / 60 / 24;
        if (days > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d day"), N_("%d days"),
                days), days));
            timeDiff -= days * 60 * 60 * 24;
        }
        const int hours = timeDiff / 60 / 60;
        if (hours > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d hour"), N_("%d hours"),
                hours), hours));
            timeDiff -= hours * 60 * 60;
        }
        const int min = timeDiff / 60;
        if (min > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d minute"), N_("%d minutes"),
                min), min));
            timeDiff -= min * 60;
        }

        if (timeDiff > 0)
        {
            if (!str.empty())
                str.append(", ");
            // TRANSLATORS: uptime command
            str.append(strprintf(ngettext(N_("%d second"), N_("%d seconds"),
                timeDiff), timeDiff));
        }
        // TRANSLATORS: uptime command
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), str.c_str()));
    }
}

impHandler1(addPriorityAttack)
{
    if (!actorManager
        || actorManager->isInPriorityAttackList(args))
    {
        return;
    }

    actorManager->removeAttackMob(args);
    actorManager->addPriorityAttackMob(args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
}

impHandler1(addAttack)
{
    if (!actorManager || actorManager->isInAttackList(args))
        return;

    actorManager->removeAttackMob(args);
    actorManager->addAttackMob(args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
}

impHandler1(removeAttack)
{
    if (!actorManager || args.empty()
        || !actorManager->isInAttackList(args))
    {
        return;
    }

    actorManager->removeAttackMob(args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
}

impHandler1(addIgnoreAttack)
{
    if (!actorManager || actorManager->isInIgnoreAttackList(args))
        return;

    actorManager->removeAttackMob(args);
    actorManager->addIgnoreAttackMob(args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
}

impHandler0(cacheInfo)
{
    if (!chatWindow || !debugChatTab)
        return;

/*
    SDLFont *const font = dynamic_cast<SDLFont *const>(chatWindow->getFont());
    if (!font)
        return;

    const TextChunkList *const cache = font->getCache();
    if (!cache)
        return;

    unsigned int all = 0;
    debugChatTab->chatLog(_("font cache size"));
    std::string str;
    for (int f = 0; f < 256; f ++)
    {
        if (!cache[f].size)
        {
            const unsigned int sz = static_cast<int>(cache[f].size);
            all += sz;
            str.append(strprintf("%d: %u, ", f, sz));
        }
    }
    debugChatTab->chatLog(str);
    debugChatTab->chatLog(strprintf("%s %d", _("Cache size:"), all));
#ifdef DEBUG_FONT_COUNTERS
    debugChatTab->chatLog("");
    debugChatTab->chatLog(strprintf("%s %d",
        _("Created:"), font->getCreateCounter()));
    debugChatTab->chatLog(strprintf("%s %d",
        _("Deleted:"), font->getDeleteCounter()));
#endif
*/
}

impHandler0(serverIgnoreAll)
{
    Net::getChatHandler()->ignoreAll();
}

impHandler0(serverUnIgnoreAll)
{
    Net::getChatHandler()->unIgnoreAll();
}

impHandler2(dumpGraphics)
{
    std::string str = strprintf("%s,%s,%dX%dX%d,", PACKAGE_OS, SMALL_VERSION,
        mainGraphics->getWidth(), mainGraphics->getHeight(),
        mainGraphics->getBpp());

    if (mainGraphics->getFullScreen())
        str.append("F");
    else
        str.append("W");
    if (mainGraphics->getHWAccel())
        str.append("H");
    else
        str.append("S");

    if (mainGraphics->getDoubleBuffer())
        str.append("D");
    else
        str.append("_");

#if defined USE_OPENGL
    str.append(strprintf(",%d", mainGraphics->getOpenGL()));
#else
    str.append(",0");
#endif

    str.append(strprintf(",%f,", static_cast<double>(client->getGuiAlpha())))
    .append(config.getBoolValue("adjustPerfomance") ? "1" : "0")
    .append(config.getBoolValue("alphaCache") ? "1" : "0")
    .append(config.getBoolValue("enableMapReduce") ? "1" : "0")
    .append(config.getBoolValue("beingopacity") ? "1" : "0")
    .append(",")
    .append(config.getBoolValue("enableAlphaFix") ? "1" : "0")
    .append(config.getBoolValue("disableAdvBeingCaching") ? "1" : "0")
    .append(config.getBoolValue("disableBeingCaching") ? "1" : "0")
    .append(config.getBoolValue("particleeffects") ? "1" : "0")
    .append(strprintf(",%d-%d", fps, config.getIntValue("fpslimit")));
    outStringNormal(tab, str, str);
}

impHandler0(dumpEnvironment)
{
    logger->log1("Start environment variables");
    for (char **env = environ; *env; ++ env)
        logger->log1(*env);
    logger->log1("End environment variables");
    if (debugChatTab)
    {
        // TRANSLATORS: dump environment command
        debugChatTab->chatLog(_("Environment variables dumped"));
    }
}

impHandler2(dumpTests)
{
    const std::string str = config.getStringValue("testInfo");
    outStringNormal(tab, str, str);
}

impHandler1(setDrop)
{
    if (player_node)
        player_node->setQuickDropCounter(atoi(args.c_str()));
}

impHandler0(error)
{
    const int *const ptr = nullptr;
    logger->log("test %d", *ptr);
}

impHandler(url)
{
    if (tab)
    {
        std::string url = args;
        if (!strStartWith(url, "http") && !strStartWith(url, "?"))
            url = "http://" + url;
        std::string str(strprintf("[@@%s |%s@@]", url.c_str(), args.c_str()));
        outStringNormal(tab, str, str);
    }
}

impHandler1(open)
{
    std::string url = args;
    if (!strStartWith(url, "http"))
        url = "http://" + url;
    openBrowser(url);
}

impHandler1(execute)
{
    const size_t idx = args.find(" ");
    std::string name;
    std::string params;
    if (idx == std::string::npos)
    {
        name = args;
    }
    else
    {
        name = args.substr(0, idx);
        params = args.substr(idx + 1);
    }
    execFile(name, name, params, "");
}

impHandler2(enableHighlight)
{
    if (tab)
    {
        tab->setAllowHighlight(true);
        if (chatWindow)
            chatWindow->saveState();
    }
}

impHandler2(disableHighlight)
{
    if (tab)
    {
        tab->setAllowHighlight(false);
        if (chatWindow)
            chatWindow->saveState();
    }
}

impHandler2(dontRemoveName)
{
    tab->setRemoveNames(false);
    if (chatWindow)
        chatWindow->saveState();
}

impHandler2(removeName)
{
    tab->setRemoveNames(true);
    if (chatWindow)
        chatWindow->saveState();
}

impHandler2(disableAway)
{
    tab->setNoAway(true);
    if (chatWindow)
        chatWindow->saveState();
}

impHandler2(enableAway)
{
    tab->setNoAway(false);
    if (chatWindow)
        chatWindow->saveState();
}

impHandler1(testParticle)
{
    if (player_node)
        player_node->setTestParticle(args);
}

impHandler0(createItems)
{
    BuyDialog *const dialog = new BuyDialog();
    const ItemDB::ItemInfos &items = ItemDB::getItemInfos();
    FOR_EACH (ItemDB::ItemInfos::const_iterator, it, items)
    {
        const ItemInfo *const info = (*it).second;
        const int id = info->getId();
        if (id <= 500)
            continue;

        const int colors = info->getColorsSize();
        if (!colors || serverVersion < 1)
        {
            dialog->addItem(id, 1, 100, 0);
        }
        else
        {
            for (int f = 0; f < colors; f ++)
            {
                if (!info->getColor(f).empty())
                    dialog->addItem(id, f, 100, 0);
            }
        }
    }
    dialog->sort();
}

impHandler1(talkRaw)
{
    Net::getChatHandler()->talkRaw(args);
}

impHandler1(talkPet)
{
    // in future probably need add channel detection
    if (!player_node->getPets().empty())
        Net::getChatHandler()->talkPet(args, GENERAL_CHANNEL);
    else
        Net::getChatHandler()->talk(args, GENERAL_CHANNEL);
}

impHandler0(testsdlfont)
{
#if defined USE_OPENGL && defined DEBUG_SDLFONT
    SDLFont *font = new SDLFont("fonts/dejavusans.ttf", 18);
    timespec time1;
    timespec time2;
    NullOpenGLGraphics *nullGraphics = new NullOpenGLGraphics;
    std::vector<std::string> data;
    volatile int width = 0;

    for (int f = 0; f < 300; f ++)
        data.push_back("test " + toString(f) + "string");
    nullGraphics->_beginDraw();

    clock_gettime(CLOCK_MONOTONIC, &time1);
    for (int f = 0; f < 500; f ++)
    {
        FOR_EACH (std::vector<std::string>::const_iterator, it, data)
        {
            width += font->getWidth(*it);
            font->drawString(nullGraphics, *it, 10, 10);
        }
        FOR_EACH (std::vector<std::string>::const_iterator, it, data)
            font->drawString(nullGraphics, *it, 10, 10);

        font->doClean();
    }

    clock_gettime(CLOCK_MONOTONIC, &time2);

    delete nullGraphics;
    delete font;

    int64_t diff = (static_cast<long long int>(
        time2.tv_sec) * 1000000000LL + static_cast<long long int>(
        time2.tv_nsec)) / 100000 - (static_cast<long long int>(
        time1.tv_sec) * 1000000000LL + static_cast<long long int>(
        time1.tv_nsec)) / 100000;
    if (debugChatTab)
        debugChatTab->chatLog("sdlfont time: " + toString(diff));
#endif
}

#ifdef USE_OPENGL
impHandler2(dumpGL)
{
    std::string str = graphicsManager.getGLVersion();
    outStringNormal(tab, str, str);
}
#else
impHandler0(dumpGL)
{
}
#endif

#ifdef DEBUG_DUMP_LEAKS1
void showRes(std::string str, ResourceManager::Resources *res);

void showRes(std::string str, ResourceManager::Resources *res)
{
    if (!res)
        return;

    str.append(toString(res->size()));
    if (debugChatTab)
        debugChatTab->chatLog(str);
    logger->log(str);
    ResourceManager::ResourceIterator iter = res->begin();
    const ResourceManager::ResourceIterator iter_end = res->end();
    while (iter != iter_end)
    {
        if (iter->second && iter->second->getRefCount())
        {
            char type = ' ';
            char isNew = 'N';
            if (iter->second->getDumped())
                isNew = 'O';
            else
                iter->second->setDumped(true);

            SubImage *const subImage = dynamic_cast<SubImage *const>(
                iter->second);
            Image *const image = dynamic_cast<Image *const>(iter->second);
            int id = 0;
            if (subImage)
                type = 'S';
            else if (image)
                type = 'I';
            if (image)
                id = image->getGLImage();
            logger->log("Resource %c%c: %s (%d) id=%d", type,
                isNew, iter->second->getIdPath().c_str(),
                iter->second->getRefCount(), id);
        }
        ++ iter;
    }
}

impHandler1(dump)
{
    if (!debugChatTab)
        return;

    ResourceManager *const resman = ResourceManager::getInstance();

    if (!args.empty())
    {
        ResourceManager::Resources *res = resman->getResources();
        // TRANSLATORS: dump command
        showRes(_("Resource images:"), res);
        res = resman->getOrphanedResources();
        // TRANSLATORS: dump command
        showRes(_("Resource orphaned images:"), res);
    }
    else
    {
        ResourceManager::Resources *res = resman->getResources();
        // TRANSLATORS: dump command
        debugChatTab->chatLog(_("Resource images:") + toString(res->size()));
        res = resman->getOrphanedResources();
        // TRANSLATORS: dump command
        debugChatTab->chatLog(_("Resource orphaned images:")
            + toString(res->size()));
    }
}

#elif defined ENABLE_MEM_DEBUG
impHandler0(dump)
{
    check_leaks();
}
#else
impHandler0(dump)
{
}
#endif


impHandler0(dumpOGL)
{
#if defined USE_OPENGL && !defined ANDROID
    NormalOpenGLGraphics::dumpSettings();
#endif
}

void replaceVars(std::string &str)
{
    if (!player_node || !actorManager)
        return;

    if (str.find("<PLAYER>") != std::string::npos)
    {
        const Being *target = player_node->getTarget();
        if (!target || target->getType() != ActorSprite::PLAYER)
        {
            target = actorManager->findNearestLivingBeing(
                player_node, 20, ActorSprite::PLAYER);
        }
        if (target)
            replaceAll(str, "<PLAYER>", target->getName());
        else
            replaceAll(str, "<PLAYER>", "");
    }
    if (str.find("<MONSTER>") != std::string::npos)
    {
        const Being *target = player_node->getTarget();
        if (!target || target->getType() != ActorSprite::MONSTER)
        {
            target = actorManager->findNearestLivingBeing(
                player_node, 20, ActorSprite::MONSTER);
        }
        if (target)
            replaceAll(str, "<MONSTER>", target->getName());
        else
            replaceAll(str, "<MONSTER>", "");
    }
    if (str.find("<PEOPLE>") != std::string::npos)
    {
        StringVect names;
        std::string newStr;
        actorManager->getPlayerNames(names, false);
        FOR_EACH (StringVectCIter, it, names)
        {
            if (*it != player_node->getName())
                newStr.append(*it).append(",");
        }
        if (newStr[newStr.size() - 1] == ',')
            newStr = newStr.substr(0, newStr.size() - 1);
        if (!newStr.empty())
            replaceAll(str, "<PEOPLE>", newStr);
        else
            replaceAll(str, "<PEOPLE>", "");
    }
    if (str.find("<PARTY>") != std::string::npos)
    {
        StringVect names;
        std::string newStr;
        const Party *party = nullptr;
        if (player_node->isInParty() && (party = player_node->getParty()))
        {
            party->getNames(names);
            FOR_EACH (StringVectCIter, it, names)
            {
                if (*it != player_node->getName())
                    newStr.append(*it).append(",");
            }
            if (newStr[newStr.size() - 1] == ',')
                newStr = newStr.substr(0, newStr.size() - 1);
            if (!newStr.empty())
                replaceAll(str, "<PARTY>", newStr);
            else
                replaceAll(str, "<PARTY>", "");
        }
        else
        {
            replaceAll(str, "<PARTY>", "");
        }
    }
}

}  // namespace Commands
