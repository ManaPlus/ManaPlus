/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "commandhandler.h"

#include "auctionmanager.h"
#include "actorspritemanager.h"
#include "channelmanager.h"
#include "channel.h"
#include "configuration.h"
#include "game.h"
#include "guildmanager.h"
#include "localplayer.h"
#include "logger.h"
#include "main.h"
#include "party.h"

#include "gui/chatwindow.h"
#include "gui/helpwindow.h"
#include "gui/gui.h"
#include "gui/outfitwindow.h"
#include "gui/shopwindow.h"
#include "gui/socialwindow.h"
#include "gui/tradewindow.h"
#include "gui/sdlfont.h"

#include "gui/widgets/channeltab.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/whispertab.h"

#include "net/adminhandler.h"
#include "net/beinghandler.h"
#include "net/chathandler.h"
#include "net/gamehandler.h"
#include "net/guildhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"
#include "net/tradehandler.h"

#ifdef DEBUG_DUMP_LEAKS
#include "resources/image.h"
#include "resources/resource.h"
#include "resources/resourcemanager.h"
#endif

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

extern std::string tradePartnerName;

CommandHandler::CommandHandler()
{
}

void CommandHandler::handleCommands(const std::string &command, ChatTab *tab)
{
    // here need add splitting commands
    handleCommand(command, tab);
}

void CommandHandler::handleCommand(const std::string &command, ChatTab *tab)
{
    std::string::size_type pos = command.find(' ');
    std::string type(command, 0, pos);
    std::string args(command, pos == std::string::npos
                     ? command.size() : pos + 1);

    args = trim(args);

    if (command == "closeall")
        handleCloseAll(args, tab);
    else if (type == "ignoreall")
        handleIgnoreAll(args, tab);
    else if (type == "help") // Do help before tabs so they can't override it
        handleHelp(args, tab);
    else if (type == "announce")
        handleAnnounce(args, tab);
    else if (type == "where")
        handleWhere(args, tab);
    else if (type == "who")
        handleWho(args, tab);
    else if (type == "msg" || type == "whisper" || type == "w")
        handleMsg(args, tab);
    else if (type == "query" || type == "q")
        handleQuery(args, tab);
    else if (type == "ignore")
        handleIgnore(args, tab);
    else if (type == "unignore")
        handleUnignore(args, tab);
    else if (type == "friend" || type == "befriend")
        handleFriend(args, tab);
    else if (type == "disregard")
        handleDisregard(args, tab);
    else if (type == "neutral")
        handleNeutral(args, tab);
    else if (type == "blacklist")
        handleBlackList(args, tab);
    else if (type == "enemy")
        handleEnemy(args, tab);
    else if (type == "erase")
        handleErase(args, tab);
    else if (type == "join")
        handleJoin(args, tab);
    else if (type == "list")
        handleListChannels(args, tab);
    else if (type == "clear")
        handleClear(args, tab);
    else if (type == "createparty")
        handleCreateParty(args, tab);
    else if (type == "createguild")
        handleCreateGuild(args, tab);
    else if (type == "party")
        handleParty(args, tab);
    else if (type == "me")
        handleMe(args, tab);
    else if (type == "toggle")
        handleToggle(args, tab);
    else if (type == "present")
        handlePresent(args, tab);
    else if (type == "quit")
        handleQuit(args, tab);
    else if (type == "all")
        handleShowAll(args, tab);
    else if (type == "move")
        handleMove(args, tab);
    else if (type == "target")
        handleTarget(args, tab);
    else if (type == "outfit")
        handleOutfit(args, tab);
    else if (type == "emote")
        handleEmote(args, tab);
    else if (type == "away")
        handleAway(args, tab);
    else if (type == "pseudoaway")
        handlePseudoAway(args, tab);
    else if (type == "follow")
        handleFollow(args, tab);
    else if (type == "heal")
        handleHeal(args, tab);
    else if (type == "navigate")
        handleNavigate(args, tab);
    else if (type == "imitation")
        handleImitation(args, tab);
    else if (type == "mail")
        handleMail(args, tab);
    else if (type == "trade")
        handleTrade(args, tab);
    else if (type == "priceload")
        handlePriceLoad(args, tab);
    else if (type == "pricesave")
        handlePriceSave(args, tab);
    else if (type == "cacheinfo")
        handleCacheInfo(args, tab);
    else if (type == "disconnect")
        handleDisconnect(args, tab);
    else if (type == "undress")
        handleUndress(args, tab);
    else if (type == "attack")
        handleAttack(args, tab);
    else if (type == "dirs")
        handleDirs(args, tab);
    else if (type == "info")
        handleInfo(args, tab);
    else if (type == "wait")
        handleWait(args, tab);
    else if (type == "uptime")
        handleUptime(args, tab);
    else if (type == "addpriorityattack")
        handleAddPriorityAttack(args, tab);
    else if (type == "addattack")
        handleAddAttack(args, tab);
    else if (type == "removeattack" || type == "removeignoreattack")
        handleRemoveAttack(args, tab);
    else if (type == "addignoreattack")
        handleAddIgnoreAttack(args, tab);
    else if (type == "dump")
        handleDump(args, tab);
    else if (type == "serverignoreall")
        handleServerIgnoreAll(args, tab);
    else if (type == "serverunignoreall")
        handleServerUnIgnoreAll(args, tab);
    else if (type == "dumpg")
        handleDumpGraphics(args, tab);
    else if (tab->handleCommand(type, args))
        ;
    else if (type == "hack")
        handleHack(args, tab);
    else
        tab->chatLog(_("Unknown command."));
}

char CommandHandler::parseBoolean(const std::string &value)
{
    std::string opt = value.substr(0, 1);

    if (opt == "1" ||
        opt == "y" || opt == "Y" ||
        opt == "t" || opt == "T")
        return 1;
    else if (opt == "0" ||
             opt == "n" || opt == "N" ||
             opt == "f" || opt == "F")
        return 0;
    else
        return -1;
}

void CommandHandler::handleAnnounce(const std::string &args,
                                    ChatTab *tab A_UNUSED)
{
    Net::getAdminHandler()->announce(args);
}

void CommandHandler::handleHelp(const std::string &args A_UNUSED,
                                ChatTab *tab A_UNUSED)
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

void CommandHandler::handleWhere(const std::string &args A_UNUSED,
                                 ChatTab *tab)
{
    std::ostringstream where;
    where << Game::instance()->getCurrentMapName() << ", coordinates: "
          << ((player_node->getPixelX() - 16) / 32) << ", "
          << ((player_node->getPixelY() - 32) / 32);

    tab->chatLog(where.str(), BY_SERVER);
}

void CommandHandler::handleWho(const std::string &args A_UNUSED,
                               ChatTab *tab A_UNUSED)
{
    Net::getChatHandler()->who();
}

void CommandHandler::handleMsg(const std::string &args, ChatTab *tab)
{
    std::string recvnick("");
    std::string msg("");

    if (args.substr(0, 1) == "\"")
    {
        const std::string::size_type pos = args.find('"', 1);
        if (pos != std::string::npos)
        {
            recvnick = args.substr(1, pos - 1);
            if (pos + 2 < args.length())
                msg = args.substr(pos + 2, args.length());
        }
    }
    else
    {
        const std::string::size_type pos = args.find(" ");
        if (pos != std::string::npos)
        {
            recvnick = args.substr(0, pos);
            if (pos + 1 < args.length())
                msg = args.substr(pos + 1, args.length());
        }
        else
        {
            recvnick = std::string(args);
            msg = "";
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
        tab->chatLog(_("Cannot send empty whispers!"), BY_SERVER);
}

void CommandHandler::handleQuery(const std::string &args, ChatTab *tab)
{
    if (chatWindow)
    {
        if (chatWindow->addWhisperTab(args, true))
        {
            chatWindow->saveState();
            return;
        }
    }

    tab->chatLog(strprintf(_("Cannot create a whisper tab for nick \"%s\"! "
        "It either already exists, or is you."),
        args.c_str()), BY_SERVER);
}

void CommandHandler::handleClear(const std::string &args A_UNUSED,
                                 ChatTab *tab A_UNUSED)
{
    if (chatWindow)
        chatWindow->clearTab();
}

void CommandHandler::handleJoin(const std::string &args, ChatTab *tab)
{
    if (!tab)
        return;

    std::string::size_type pos = args.find(' ');
    std::string name(args, 0, pos);
    std::string password(args, pos + 1);
    tab->chatLog(strprintf(_("Requesting to join channel %s."), name.c_str()));
    Net::getChatHandler()->enterChannel(name, password);
}

void CommandHandler::handleListChannels(const std::string &args A_UNUSED,
                                        ChatTab *tab A_UNUSED)
{
    Net::getChatHandler()->channelList();
}

void CommandHandler::handleCreateParty(const std::string &args, ChatTab *tab)
{
    if (!tab)
        return;

    if (args.empty())
        tab->chatLog(_("Party name is missing."), BY_SERVER);
    else
        Net::getPartyHandler()->create(args);
}

void CommandHandler::handleCreateGuild(const std::string &args, ChatTab *tab)
{
    if (!tab)
        return;

    if (args.empty())
        tab->chatLog(_("Guild name is missing."), BY_SERVER);
    else
        Net::getGuildHandler()->create(args);
}

void CommandHandler::handleParty(const std::string &args, ChatTab *tab)
{
    if (!tab)
        return;

    if (args != "")
        Net::getPartyHandler()->invite(args);
    else
        tab->chatLog(_("Please specify a name."), BY_SERVER);
}

void CommandHandler::handleMe(const std::string &args, ChatTab *tab)
{
    outString(tab, strprintf("*%s*", args.c_str()), args);
}

void CommandHandler::outString(ChatTab *tab, const std::string &str,
                               const std::string &def)
{
    if (!tab)
    {
        Net::getChatHandler()->me(def);
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
            const Guild *guild = player_node->getGuild();
            if (guild)
            {
                if (guild->getServerGuild())
                    Net::getGuildHandler()->chat(guild->getId(), str);
                else if (guildManager)
                    guildManager->chat(str);
            }
            break;
        }
        default:
            Net::getChatHandler()->me(def);
            break;
    }
}

void CommandHandler::handleToggle(const std::string &args, ChatTab *tab)
{
    if (args.empty())
    {
        if (chatWindow && tab)
        {
            tab->chatLog(chatWindow->getReturnTogglesChat() ?
                _("Return toggles chat.") : _("Message closes chat."));
        }
        return;
    }

    char opt = parseBoolean(args);

    switch (opt)
    {
        case 1:
            if (tab)
                tab->chatLog(_("Return now toggles chat."));
            if (chatWindow)
                chatWindow->setReturnTogglesChat(true);
            return;
        case 0:
            if (tab)
                tab->chatLog(_("Message now closes chat."));
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

void CommandHandler::handlePresent(const std::string &args A_UNUSED,
                                   ChatTab *tab A_UNUSED)
{
    if (chatWindow)
        chatWindow->doPresent();
}

void CommandHandler::handleIgnore(const std::string &args,
                                  ChatTab *tab A_UNUSED)
{
    changeRelation(args, PlayerRelation::IGNORED, "ignored", tab);
}

void CommandHandler::handleFriend(const std::string &args, ChatTab *tab)
{
    changeRelation(args, PlayerRelation::FRIEND, _("friend"), tab);
}

void CommandHandler::handleDisregard(const std::string &args, ChatTab *tab)
{
    changeRelation(args, PlayerRelation::DISREGARDED, _("disregarded"), tab);
}

void CommandHandler::handleNeutral(const std::string &args, ChatTab *tab)
{
    changeRelation(args, PlayerRelation::NEUTRAL, _("neutral"), tab);
}

void CommandHandler::changeRelation(const std::string &args,
                                    PlayerRelation::Relation relation,
                                    const std::string &relationText,
                                    ChatTab *tab)
{
    if (args.empty())
    {
        if (tab)
            tab->chatLog(_("Please specify a name."), BY_SERVER);
        return;
    }

    if (player_relations.getRelation(args) == relation)
    {
        if (tab)
        {
            tab->chatLog(strprintf(_("Player already %s!"),
                         relationText.c_str()), BY_SERVER);
        }
        return;
    }
    else
    {
        player_relations.setRelation(args, relation);
    }

    if (player_relations.getRelation(args) == relation)
    {
        if (tab)
        {
            tab->chatLog(strprintf(_("Player successfully %s!"),
                         relationText.c_str()), BY_SERVER);
        }
    }
    else
    {
        if (tab)
        {
            tab->chatLog(strprintf(_("Player could not be %s!"),
                         relationText.c_str()), BY_SERVER);
        }
    }
}

void CommandHandler::handleUnignore(const std::string &args, ChatTab *tab)
{
    if (args.empty())
    {
        if (tab)
            tab->chatLog(_("Please specify a name."), BY_SERVER);
        return;
    }

    if (player_relations.getRelation(args) != PlayerRelation::NEUTRAL
        && player_relations.getRelation(args) != PlayerRelation::FRIEND)
    {
        player_relations.setRelation(args, PlayerRelation::NEUTRAL);
    }
    else
    {
        if (tab)
            tab->chatLog(_("Player wasn't ignored!"), BY_SERVER);
        return;
    }

    if (tab)
    {
        if (player_relations.getRelation(args) == PlayerRelation::NEUTRAL)
            tab->chatLog(_("Player no longer ignored!"), BY_SERVER);
        else
            tab->chatLog(_("Player could not be unignored!"), BY_SERVER);
    }
}

void CommandHandler::handleBlackList(const std::string &args, ChatTab *tab)
{
    changeRelation(args, PlayerRelation::BLACKLISTED, _("blacklisted"), tab);
}

void CommandHandler::handleEnemy(const std::string &args, ChatTab *tab)
{
    changeRelation(args, PlayerRelation::ENEMY2, _("enemy"), tab);
}

void CommandHandler::handleErase(const std::string &args, ChatTab *tab)
{
    if (args.empty())
    {
        if (tab)
            tab->chatLog(_("Please specify a name."), BY_SERVER);
        return;
    }

    if (player_relations.getRelation(args) == PlayerRelation::ERASED)
    {
        if (tab)
            tab->chatLog(_("Player already erased!"), BY_SERVER);
        return;
    }
    else
    {
        player_relations.setRelation(args, PlayerRelation::ERASED);
    }

    if (tab)
    {
        if (player_relations.getRelation(args) == PlayerRelation::ERASED)
            tab->chatLog(_("Player successfully erased!"), BY_SERVER);
        else
            tab->chatLog(_("Player could not be erased!"), BY_SERVER);
    }
}

void CommandHandler::handleQuit(const std::string &args A_UNUSED,
                                ChatTab *tab A_UNUSED)
{
//    quit();
}

void CommandHandler::handleShowAll(const std::string &args A_UNUSED,
                                   ChatTab *tab A_UNUSED)
{
    if (actorSpriteManager)
        actorSpriteManager->printAllToChat();
}

void CommandHandler::handleMove(const std::string &args, ChatTab *tab A_UNUSED)
{
    int x = 0;
    int y = 0;

    if (player_node && parse2Int(args, &x, &y))
        player_node->moveTo(x, y);
}

void CommandHandler::handleNavigate(const std::string &args,
                                    ChatTab *tab A_UNUSED)
{
    if (!player_node)
        return;

    int x = 0;
    int y = 0;

    if (parse2Int(args, &x, &y))
        player_node->navigateTo(x, y);
    else
        player_node->navigateClean();
}

bool CommandHandler::parse2Int(const std::string &args, int *x, int *y)
{
    bool isValid = false;
    const std::string::size_type pos = args.find(" ");
    if (pos != std::string::npos)
    {
        if (pos + 1 < args.length())
        {
            *x = atoi(args.substr(0, pos).c_str());
            *y = atoi(args.substr(pos + 1, args.length()).c_str());
            isValid = true;
        }
    }
    return isValid;
}

void CommandHandler::handleTarget(const std::string &args,
                                  ChatTab *tab A_UNUSED)
{
    if (!actorSpriteManager || !player_node)
        return;

    Being* target = actorSpriteManager->findNearestByName(args);
    if (target)
        player_node->setTarget(target);
}

void CommandHandler::handleCloseAll(const std::string &args A_UNUSED,
                                    ChatTab *tab A_UNUSED)
{
    if (chatWindow)
    {
        chatWindow->removeAllWhispers();
        chatWindow->saveState();
    }
}

void CommandHandler::handleIgnoreAll(const std::string &args A_UNUSED,
                                     ChatTab *tab A_UNUSED)
{
    if (chatWindow)
    {
        chatWindow->ignoreAllWhispers();
        chatWindow->saveState();
    }
}

void CommandHandler::handleOutfit(const std::string &args,
                                  ChatTab *tab A_UNUSED)
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

void CommandHandler::handleEmote(const std::string &args,
                                 ChatTab *tab A_UNUSED)
{
    if (player_node)
        player_node->emote(static_cast<Uint8>(atoi(args.c_str())));
}

void CommandHandler::handleAway(const std::string &args, ChatTab *tab A_UNUSED)
{
    if (player_node)
        player_node->setAway(args);
}

void CommandHandler::handlePseudoAway(const std::string &args,
                                      ChatTab *tab A_UNUSED)
{
    if (player_node)
        player_node->setPseudoAway(args);
}

void CommandHandler::handleFollow(const std::string &args, ChatTab *tab)
{
    if (!player_node)
        return;

    if (!args.empty())
    {
        player_node->setFollow(args);
    }
    else if (tab)
    {
        if (tab->getType() == ChatTab::TAB_WHISPER)
        {
            WhisperTab *wTab = static_cast<WhisperTab*>(tab);
            if (wTab)
                player_node->setFollow(wTab->getNick());
        }
    }
}

void CommandHandler::handleImitation(const std::string &args, ChatTab *tab)
{
    if (!player_node)
        return;

    if (!args.empty())
    {
        player_node->setImitate(args);
    }
    else if (tab && tab->getType() == ChatTab::TAB_WHISPER)
    {
        WhisperTab *wTab = static_cast<WhisperTab*>(tab);
        if (wTab)
            player_node->setImitate(wTab->getNick());
    }
    else
    {
        player_node->setImitate("");
    }
}

void CommandHandler::handleHeal(const std::string &args, ChatTab *tab A_UNUSED)
{
    if (!actorSpriteManager)
        return;

    if (!args.empty())
    {
        Being *being = actorSpriteManager->findBeingByName(
            args, Being::PLAYER);
        if (being)
            actorSpriteManager->heal(being);
    }
    else
    {
        actorSpriteManager->heal(player_node);
    }
}

void CommandHandler::handleHack(const std::string &args, ChatTab *tab A_UNUSED)
{
    Net::getChatHandler()->sendRaw(args);
}

void CommandHandler::handleMail(const std::string &args, ChatTab *tab A_UNUSED)
{
    if (auctionManager && auctionManager->getEnableAuctionBot())
        auctionManager->sendMail(args);
}

void CommandHandler::handlePriceLoad(const std::string &args A_UNUSED,
                                     ChatTab *tab A_UNUSED)
{
    if (shopWindow)
        shopWindow->loadList();
}

void CommandHandler::handlePriceSave(const std::string &args A_UNUSED,
                                     ChatTab *tab A_UNUSED)
{
    if (shopWindow)
        shopWindow->saveList();
}

void CommandHandler::handleDisconnect(const std::string &args A_UNUSED,
                                      ChatTab *tab A_UNUSED)
{
    Net::getGameHandler()->disconnect2();
}

void CommandHandler::handleUndress(const std::string &args,
                                   ChatTab *tab A_UNUSED)
{
    if (!actorSpriteManager)
        return;

    Being* target = actorSpriteManager->findNearestByName(args);
    if (target)
        Net::getBeingHandler()->undress(target);
}

void CommandHandler::handleAttack(const std::string &args,
                                  ChatTab *tab A_UNUSED)
{
    if (!player_node || !actorSpriteManager)
        return;

    Being* target = actorSpriteManager->findNearestByName(args);
    if (target)
        player_node->setTarget(target);
    player_node->attack2(player_node->getTarget(), true);
}

void CommandHandler::handleTrade(const std::string &args,
                                 ChatTab *tab A_UNUSED)
{
    if (!actorSpriteManager)
        return;

    Being *being = actorSpriteManager->findBeingByName(args, Being::PLAYER);
    if (being)
    {
        Net::getTradeHandler()->request(being);
        tradePartnerName = being->getName();
        if (tradeWindow)
            tradeWindow->clear();
    }
}

void CommandHandler::handleDirs(const std::string &args A_UNUSED,
                                ChatTab *tab A_UNUSED)
{
    if (!player_node || !debugChatTab)
        return;

    debugChatTab->chatLog("config directory: "
        + Client::getConfigDirectory());
    debugChatTab->chatLog("logs directory: "
        + Client::getLocalDataDirectory());
    debugChatTab->chatLog("screenshots directory: "
        + Client::getScreenshotDirectory());
}

void CommandHandler::handleInfo(const std::string &args A_UNUSED,
                                ChatTab *tab)
{
    switch (tab->getType())
    {
        case ChatTab::TAB_GUILD:
        {
            if (!player_node)
                return;
            const Guild *guild = player_node->getGuild();
            if (guild)
                Net::getGuildHandler()->info(guild->getId());
            break;
        }
        default:
            break;
    }
}

void CommandHandler::handleWait(const std::string &args,
                                ChatTab *tab A_UNUSED)
{
    if (player_node)
        player_node->waitFor(args);
}

void CommandHandler::handleUptime(const std::string &args A_UNUSED,
                                  ChatTab *tab A_UNUSED)
{
    if (!debugChatTab)
        return;

    if (cur_time < start_time)
    {
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), "unknown"));
    }
    else
    {
        std::string str;
        int timeDiff = cur_time - start_time;

        const int weeks = timeDiff / 60 / 60 / 24 / 7;
        if (weeks > 0)
        {
            str = strprintf(ngettext("%d week", "%d weeks", weeks), weeks);
            timeDiff -= weeks * 60 * 60 * 24 * 7;
        }

        const int days = timeDiff / 60 / 60 / 24;
        if (days > 0)
        {
            if (!str.empty())
                str += ", ";
            str += strprintf(ngettext("%d day", "%d days", days), days);
            timeDiff -= days * 60 * 60 * 24;
        }
        const int hours = timeDiff / 60 / 60;
        if (hours > 0)
        {
            if (!str.empty())
                str += ", ";
            str += strprintf(ngettext("%d hour", "%d hours", hours), hours);
            timeDiff -= hours * 60 * 60;
        }
        const int min = timeDiff / 60;
        if (min > 0)
        {
            if (!str.empty())
                str += ", ";
            str += strprintf(ngettext("%d minute", "%d minutes", min), min);
            timeDiff -= min * 60;
        }

        if (timeDiff > 0)
        {
            if (!str.empty())
                str += ", ";
            str += strprintf(ngettext("%d second", "%d seconds",
                timeDiff), timeDiff);
        }
        debugChatTab->chatLog(strprintf(_("Client uptime: %s"), str.c_str()));
    }
}

void CommandHandler::handleAddPriorityAttack(const std::string &args,
                                             ChatTab *tab A_UNUSED)
{
    if (!actorSpriteManager
        || actorSpriteManager->isInPriorityAttackList(args))
    {
        return;
    }

    actorSpriteManager->removeAttackMob(args);
    actorSpriteManager->addPriorityAttackMob(args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
}

void CommandHandler::handleAddAttack(const std::string &args,
                                     ChatTab *tab A_UNUSED)
{
    if (!actorSpriteManager || actorSpriteManager->isInAttackList(args))
        return;

    actorSpriteManager->removeAttackMob(args);
    actorSpriteManager->addAttackMob(args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
}

void CommandHandler::handleRemoveAttack(const std::string &args,
                                        ChatTab *tab A_UNUSED)
{
    if (!actorSpriteManager || args.empty()
        || !actorSpriteManager->isInAttackList(args))
    {
        return;
    }

    actorSpriteManager->removeAttackMob(args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
}

void CommandHandler::handleAddIgnoreAttack(const std::string &args,
                                           ChatTab *tab A_UNUSED)
{
    if (!actorSpriteManager || actorSpriteManager->isInIgnoreAttackList(args))
        return;

    actorSpriteManager->removeAttackMob(args);
    actorSpriteManager->addIgnoreAttackMob(args);

    if (socialWindow)
        socialWindow->updateAttackFilter();
}

void CommandHandler::handleCacheInfo(const std::string &args A_UNUSED,
                                     ChatTab *tab A_UNUSED)
{
    if (!chatWindow || !debugChatTab)
        return;

    SDLFont *font = dynamic_cast<SDLFont *>(chatWindow->getFont());
    if (!font)
        return;

    std::list<SDLTextChunk> *cache = font->getCache();
    if (!cache)
        return;

    int all = 0;
    debugChatTab->chatLog(_("font cache size"));
    std::string str;
    for (int f = 0; f < 256; f ++)
    {
        if (!cache[f].empty())
        {
            all += cache[f].size();
            str += strprintf("%d: %u, ", f,
                static_cast<unsigned int>(cache[f].size()));
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
}

void CommandHandler::handleServerIgnoreAll(const std::string &args A_UNUSED,
                                           ChatTab *tab A_UNUSED)
{
    Net::getChatHandler()->ignoreAll();
}

void CommandHandler::handleServerUnIgnoreAll(const std::string &args A_UNUSED,
                                             ChatTab *tab A_UNUSED)
{
    Net::getChatHandler()->unIgnoreAll();
}

void CommandHandler::handleDumpGraphics(const std::string &args A_UNUSED,
                                        ChatTab *tab)
{
    std::string str;
    str = strprintf ("%s,%s,%dX%dX%d,", PACKAGE_OS, SMALL_VERSION,
        mainGraphics->getWidth(), mainGraphics->getHeight(),
        mainGraphics->getBpp());

    if (mainGraphics->getFullScreen())
        str += "F";
    else
        str += "W";
    if (mainGraphics->getHWAccel())
        str += "H";
    else
        str += "S";

    if (mainGraphics->getDoubleBuffer())
        str += "D";
    else
        str += "_";

#if defined USE_OPENGL
    str += strprintf(",%d", mainGraphics->getOpenGL());
#else
    str += ",0";
#endif

    str += strprintf(",%f,", Client::getGuiAlpha());
    str += config.getBoolValue("adjustPerfomance") ? "1" : "0";
    str += config.getBoolValue("alphaCache") ? "1" : "0";
    str += config.getBoolValue("enableMapReduce") ? "1" : "0";
    str += config.getBoolValue("beingopacity") ? "1" : "0";
    str += ",";
    str += config.getBoolValue("enableAlphaFix") ? "1" : "0";
    str += config.getBoolValue("disableAdvBeingCaching") ? "1" : "0";
    str += config.getBoolValue("disableBeingCaching") ? "1" : "0";
    str += config.getBoolValue("particleeffects") ? "1" : "0";

    str += strprintf(",%d-%d", fps, config.getIntValue("fpslimit"));
    outString(tab, str, str);
}

#ifdef DEBUG_DUMP_LEAKS
void showRes(std::string str, ResourceManager::Resources *res);

void showRes(std::string str, ResourceManager::Resources *res)
{
    if (debugChatTab)
        debugChatTab->chatLog(str + toString(res->size()));
    logger->log(str + toString(res->size()));
    ResourceManager::ResourceIterator iter = res->begin();
    while (iter != res->end())
    {
        if (iter->second && iter->second->getRefCount())
        {
            std::string type = " ";
            std::string isNew = "N";
            if (iter->second->getDumped())
                isNew = "O";
            else
                iter->second->setDumped(true);

            if (dynamic_cast<SubImage*>(iter->second))
                type = "S";
            else if (dynamic_cast<Image*>(iter->second))
                type = "I";
            logger->log("Resource %s%s: %s (%d)", type.c_str(),
                isNew.c_str(), iter->second->getIdPath().c_str(),
                iter->second->getRefCount());
        }
        ++ iter;
    }
}

void CommandHandler::handleDump(const std::string &args,
                                ChatTab *tab A_UNUSED)
{
    if (!debugChatTab)
        return;

    ResourceManager *resman = ResourceManager::getInstance();

    if (!args.empty())
    {
        ResourceManager::Resources *res = resman->getResources();
        showRes(_("Resource images:"), res);
        res = resman->getOrphanedResources();
        showRes(_("Resource orphaned images:"), res);
    }
    else
    {
        ResourceManager::Resources *res = resman->getResources();
        debugChatTab->chatLog(_("Resource images:") + toString(res->size()));
        res = resman->getOrphanedResources();
        debugChatTab->chatLog(_("Resource orphaned images:")
            + toString(res->size()));
    }
}

#elif defined ENABLE_MEM_DEBUG
void CommandHandler::handleDump(const std::string &args A_UNUSED,
                                ChatTab *tab A_UNUSED)
{
    check_leaks();
}
#else
void CommandHandler::handleDump(const std::string &args A_UNUSED,
                                ChatTab *tab A_UNUSED)
{
}
#endif

void CommandHandler::replaceVars(std::string &str)
{
    if (!player_node || !actorSpriteManager)
        return;

    if (str.find("<PLAYER>") != std::string::npos)
    {
        Being *target = player_node->getTarget();
        if (!target || target->getType() != ActorSprite::PLAYER)
        {
            target = actorSpriteManager->findNearestLivingBeing(
                player_node, 20, ActorSprite::PLAYER);
        }
        if (target)
            replaceAll(str, "<PLAYER>", target->getName());
        else
            replaceAll(str, "<PLAYER>", "");
    }
    if (str.find("<MONSTER>") != std::string::npos)
    {
        Being *target = player_node->getTarget();
        if (!target || target->getType() != ActorSprite::MONSTER)
        {
            target = actorSpriteManager->findNearestLivingBeing(
                player_node, 20, ActorSprite::MONSTER);
        }
        if (target)
            replaceAll(str, "<MONSTER>", target->getName());
        else
            replaceAll(str, "<MONSTER>", "");
    }
    if (str.find("<PEOPLE>") != std::string::npos)
    {
        std::vector<std::string> names;
        std::string newStr = "";
        actorSpriteManager->getPlayerNames(names, false);
        std::vector<std::string>::const_iterator it = names.begin();
        std::vector<std::string>::const_iterator it_end = names.end();
        for (; it != it_end; ++ it)
        {
            if (*it != player_node->getName())
                newStr += *it + ",";
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
        std::vector<std::string> names;
        std::string newStr = "";
        Party *party = nullptr;
        if (player_node->isInParty() && (party = player_node->getParty()))
        {
            party->getNames(names);
            std::vector<std::string>::const_iterator it = names.begin();
            std::vector<std::string>::const_iterator it_end = names.end();
            for (; it != it_end; ++ it)
            {
                if (*it != player_node->getName())
                    newStr += *it + ",";
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
