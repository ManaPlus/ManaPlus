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

#include "actorspritemanager.h"
#include "channelmanager.h"
#include "channel.h"
#include "game.h"
#include "localplayer.h"
#include "log.h"
#include "main.h"

#include "gui/chat.h"
#include "gui/gui.h"
#include "gui/outfitwindow.h"
#include "gui/shopwindow.h"
#include "gui/trade.h"
#include "gui/truetypefont.h"

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

#include "utils/gettext.h"
#include "utils/stringutils.h"

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
    {
        handleCloseAll(args, tab);
    }
    else if (type == "ignoreall")
    {
        handleIgnoreAll(args, tab);
    }
    else if (type == "help") // Do help before tabs so they can't override it
    {
        handleHelp(args, tab);
    }
    else if (type == "announce")
    {
        handleAnnounce(args, tab);
    }
    else if (type == "where")
    {
        handleWhere(args, tab);
    }
    else if (type == "who")
    {
        handleWho(args, tab);
    }
    else if (type == "msg" || type == "whisper" || type == "w")
    {
        handleMsg(args, tab);
    }
    else if (type == "query" || type == "q")
    {
        handleQuery(args, tab);
    }
    else if (type == "ignore")
    {
        handleIgnore(args, tab);
    }
    else if (type == "unignore")
    {
        handleUnignore(args, tab);
    }
    else if (type == "friend" || type == "befriend")
    {
        handleFriend(args, tab);
    }
    else if (type == "disregard")
    {
        handleDisregard(args, tab);
    }
    else if (type == "neutral")
    {
        handleNeutral(args, tab);
    }
    else if (type == "erase")
    {
        handleErase(args, tab);
    }
    else if (type == "join")
    {
        handleJoin(args, tab);
    }
    else if (type == "list")
    {
        handleListChannels(args, tab);
    }
    else if (type == "clear")
    {
        handleClear(args, tab);
    }
    else if (type == "createparty")
    {
        handleCreateParty(args, tab);
    }
    else if (type == "createguild")
    {
        handleCreateGuild(args, tab);
    }
    else if (type == "party")
    {
        handleParty(args, tab);
    }
    else if (type == "me")
    {
        handleMe(args, tab);
    }
    else if (type == "toggle")
    {
        handleToggle(args, tab);
    }
    else if (type == "present")
    {
        handlePresent(args, tab);
    }
    else if (type == "quit")
    {
        handleQuit(args, tab);
    }
    else if (type == "all")
    {
        handleShowAll(args, tab);
    }
    else if (type == "move")
    {
        handleMove(args, tab);
    }
    else if (type == "target")
    {
        handleTarget(args, tab);
    }
    else if (type == "outfit")
    {
        handleOutfit(args, tab);
    }
    else if (type == "emote")
    {
        handleEmote(args, tab);
    }
    else if (type == "away")
    {
        handleAway(args, tab);
    }
    else if (type == "follow")
    {
        handleFollow(args, tab);
    }
    else if (type == "heal")
    {
        handleHeal(args, tab);
    }
    else if (type == "navigate")
    {
        handleNavigate(args, tab);
    }
    else if (type == "imitation")
    {
        handleImitation(args, tab);
    }
    else if (type == "mail")
    {
        handleMail(args, tab);
    }
    else if (type == "trade")
    {
        handleTrade(args, tab);
    }
    else if (type == "priceload")
    {
        handlePriceLoad(args, tab);
    }
    else if (type == "pricesave")
    {
        handlePriceSave(args, tab);
    }
    else if (type == "cacheinfo")
    {
        handleCacheInfo(args, tab);
    }
    else if (type == "disconnect")
    {
        handleDisconnect(args, tab);
    }
    else if (type == "undress")
    {
        handleUndress(args, tab);
    }
    else if (type == "attack")
    {
        handleAttack(args, tab);
    }
    else if (type == "dirs")
    {
        handleDirs(args, tab);
    }
    else if (type == "info")
    {
        handleInfo(args, tab);
    }
    else if (type == "wait")
    {
        handleWait(args, tab);
    }
    else if (type == "uptime")
    {
        handleUptime(args, tab);
    }
    else if (tab->handleCommand(type, args))
    {
        // Nothing to do
    }
    else if (type == "hack")
    {
        handleHack(args, tab);
    }
    else
    {
        tab->chatLog(_("Unknown command."));
    }
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
                                    ChatTab *tab _UNUSED_)
{
    Net::getAdminHandler()->announce(args);
}

void CommandHandler::handleHelp(const std::string &args, ChatTab *tab)
{
    if (args == "")
    {
        tab->chatLog(_("-- Help --"));
        tab->chatLog(_("/help > Display this help"));

        tab->chatLog(_("/where > Display map name"));
        tab->chatLog(_("/who > Display number of online users"));
        tab->chatLog(_("/me > Tell something about yourself"));

        tab->chatLog(_("/clear > Clears this window"));

        tab->chatLog(_("/msg > Send a private message to a user"));
        tab->chatLog(_("/whisper > Alias of msg"));
        tab->chatLog(_("/w > Alias of msg"));
        tab->chatLog(_("/query > Makes a tab for private messages "
                       "with another user"));
        tab->chatLog(_("/q > Alias of query"));

        tab->chatLog(_("/away > Tell the other whispering players "
                       "you're away from keyboard."));

        tab->chatLog(_("/ignore > ignore a player"));
        tab->chatLog(_("/unignore > stop ignoring a player"));
        tab->chatLog(_("/ignoreall > Ignore all opened whisper tabs"));
        tab->chatLog(_("/erase > Erase a player"));
        tab->chatLog(_("/befriend > Be friend a player"));
        tab->chatLog(_("/disregard > Disregard a player"));
        tab->chatLog(_("/neutral > Neutral a player"));

        tab->chatLog(_("/list > Display all public channels"));
        tab->chatLog(_("/join > Join or create a channel"));

        tab->chatLog(_("/createparty > Create a new party"));
        tab->chatLog(_("/createguild > Create a new guild"));
        tab->chatLog(_("/party > Invite a user to party"));

        tab->chatLog(_("/toggle > Determine whether <return> "
                       "toggles the chat log"));
        tab->chatLog(_("/present > Get list of players present "
                       "(sent to chat log, if logging)"));

        tab->chatLog(_("/announce > Global announcement (GM only)"));

        tab->chatLog(_("/closeall > Close all opened whisper tabs"));

        tab->chatLog(_("/all > Show all visible beings in debug tab"));

        tab->chatLog(_("/move > Move to given position"));
        tab->chatLog(_("/navigate > Draw path to given position"));
        tab->chatLog(_("/target > Set target to being"));
        tab->chatLog(_("/outfit > Wear outfit by index"));
        tab->chatLog(_("/emote > Show emote by index"));
        tab->chatLog(_("/follow > Follow player"));
        tab->chatLog(_("/imitation > Imitate player"));
        tab->chatLog(_("/heal > Heal player"));
        tab->chatLog(_("/mail > Send offline message to player"));

        tab->showHelp(); // Allow the tab to show it's help

        tab->chatLog(_("For more information, type /help <command>."));
    }
    else if (args == "help") // Do this before tabs so they can't change it
    {
        tab->chatLog(_("Command: /help"));
        tab->chatLog(_("This command displays a list "
                       "of all commands available."));
        tab->chatLog(_("Command: /help <command>"));
        tab->chatLog(_("This command displays help on <command>."));
    }
    else if (tab->handleCommand("help", args))
    {
        // Nothing to do
    }
    else if (args == "announce")
    {
        tab->chatLog(_("Command: /announce <msg>"));
        tab->chatLog(_("*** only available to a GM ***"));
        tab->chatLog(_("This command sends the message <msg> to "
                            "all players currently online."));
    }
    else if (args == "clear")
    {
        tab->chatLog(_("Command: /clear"));
        tab->chatLog(_("This command clears the chat log of previous chat."));
    }
    else if (args == "ignore")
    {
        tab->chatLog(_("Command: /ignore <player>"));
        tab->chatLog(_("This command ignores the given player regardless of "
                       "current relations."));
    }
    else if (args == "join")
    {
        tab->chatLog(_("Command: /join <channel>"));
        tab->chatLog(_("This command makes you enter <channel>."));
        tab->chatLog(_("If <channel> doesn't exist, it's created."));
    }
    else if (args == "list")
    {
        tab->chatLog(_("Command: /list"));
        tab->chatLog(_("This command shows a list of all channels."));
    }
    else if (args == "me")
    {
        tab->chatLog(_("Command: /me <message>"));
        tab->chatLog(_("This command tell others you are (doing) <msg>."));
    }
    else if (args == "msg" || args == "whisper" || args == "w")
    {
        tab->chatLog(_("Command: /msg <nick> <message>"));
        tab->chatLog(_("Command: /whisper <nick> <message>"));
        tab->chatLog(_("Command: /w <nick> <message>"));
        tab->chatLog(_("This command sends the text <message> to <nick>."));
        tab->chatLog(_("If the <nick> has spaces in it, enclose it in "
                            "double quotes (\")."));
    }
    else if (args == "query" || args == "q")
    {
        tab->chatLog(_("Command: /query <nick>"));
        tab->chatLog(_("Command: /q <nick>"));
        tab->chatLog(_("This command tries to make a tab for whispers between"
                       "you and <nick>."));
    }
    else if (args == "away")
    {
        tab->chatLog(_("Command: /away <afk reason>"));
        tab->chatLog(_("This command tells "
                       "you're away from keyboard with the given reason."));
        tab->chatLog(_("Command: /away"));
        tab->chatLog(_("This command clears the away status and message."));
    }
    else if (args == "createparty")
    {
        tab->chatLog(_("Command: /createparty <name>"));
        tab->chatLog(_("This command creates a new party called <name>."));
    }
    else if (args == "createguild")
    {
        tab->chatLog(_("Command: /createguild <name>"));
        tab->chatLog(_("This command creates a new guild called <name>."));
    }
    else if (args == "party")
    {
        tab->chatLog(_("Command: /party <nick>"));
        tab->chatLog(_("This command invites <nick> to party with you."));
        tab->chatLog(_("If the <nick> has spaces in it, enclose it in "
                       "double quotes (\")."));
    }
    else if (args == "present")
    {
        tab->chatLog(_("Command: /present"));
        tab->chatLog(_("This command gets a list of players within hearing "
            "and sends it to chat log."));
    }
    else if (args == "toggle")
    {
        tab->chatLog(_("Command: /toggle <state>"));
        tab->chatLog(_("This command sets whether the return key should "
            "toggle the chat log, or whether the chat log turns off "
            "automatically."));
        tab->chatLog(_("<state> can be one of \"1\", \"yes\", \"true\" to "
            "turn the toggle on, or \"0\", \"no\", \"false\" to turn the "
            "toggle off."));
        tab->chatLog(_("Command: /toggle"));
        tab->chatLog(_("This command displays the return toggle status."));
    }
    else if (args == "unignore")
    {
        tab->chatLog(_("Command: /unignore <player>"));
        tab->chatLog(_("This command stops ignoring the given player if they "
                      "are being ignored"));
    }
    else if (args == "where")
    {
        tab->chatLog(_("Command: /where"));
        tab->chatLog(_("This command displays the name of the current map."));
    }
    else if (args == "who")
    {
        tab->chatLog(_("Command: /who"));
        tab->chatLog(_("This command displays the number of players currently "
                     "online."));
    }
    else
    {
        tab->chatLog(_("Unknown command."));
        tab->chatLog(_("Type /help for a list of commands."));
    }
}

void CommandHandler::handleWhere(const std::string &args _UNUSED_,
                                 ChatTab *tab)
{
    std::ostringstream where;
    where << Game::instance()->getCurrentMapName() << ", coordinates: "
          << ((player_node->getPixelX() - 16) / 32) << ", "
          << ((player_node->getPixelY() - 32) / 32);

    tab->chatLog(where.str(), BY_SERVER);
}

void CommandHandler::handleWho(const std::string &args _UNUSED_,
                               ChatTab *tab _UNUSED_)
{
    Net::getChatHandler()->who();
}

void CommandHandler::handleMsg(const std::string &args, ChatTab *tab)
{
    std::string recvnick = "";
    std::string msg = "";

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

        chatWindow->whisper(recvnick, msg, BY_PLAYER);
    }
    else
        tab->chatLog(_("Cannot send empty whispers!"), BY_SERVER);
}

void CommandHandler::handleQuery(const std::string &args, ChatTab *tab)
{
    if (chatWindow && chatWindow->addWhisperTab(args, true))
        return;

    tab->chatLog(strprintf(_("Cannot create a whisper tab for nick \"%s\"! "
        "It either already exists, or is you."),
        args.c_str()), BY_SERVER);
}

void CommandHandler::handleClear(const std::string &args _UNUSED_,
                                 ChatTab *tab _UNUSED_)
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

void CommandHandler::handleListChannels(const std::string &args _UNUSED_,
                                        ChatTab *tab _UNUSED_)
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
    if (!tab)
    {
        Net::getChatHandler()->me(args);
        return;
    }

    const std::string str = strprintf("*%s*", args.c_str());
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
                Net::getGuildHandler()->chat(guild->getId(), str);
            break;
        }
        default:
            Net::getChatHandler()->me(args);
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

void CommandHandler::handlePresent(const std::string &args _UNUSED_,
                                   ChatTab *tab _UNUSED_)
{
    if (chatWindow)
        chatWindow->doPresent();
}

void CommandHandler::handleIgnore(const std::string &args,
                                  ChatTab *tab _UNUSED_)
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

    if (player_relations.getRelation(args) == PlayerRelation::IGNORED)
    {
        player_relations.removePlayer(args);
    }
    else
    {
        if (tab)
            tab->chatLog(_("Player wasn't ignored!"), BY_SERVER);
        return;
    }

    if (tab)
    {
        if (player_relations.getRelation(args) != PlayerRelation::IGNORED)
            tab->chatLog(_("Player no longer ignored!"), BY_SERVER);
        else
            tab->chatLog(_("Player could not be unignored!"), BY_SERVER);
    }
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

void CommandHandler::handleQuit(const std::string &args _UNUSED_,
                                ChatTab *tab _UNUSED_)
{
//    quit();
}

void CommandHandler::handleShowAll(const std::string &args _UNUSED_,
                                   ChatTab *tab _UNUSED_)
{
    if (actorSpriteManager)
        actorSpriteManager->printAllToChat();
}

void CommandHandler::handleMove(const std::string &args, ChatTab *tab _UNUSED_)
{
    int x = 0;
    int y = 0;

    if (player_node && parse2Int(args, &x, &y))
        player_node->moveTo(x, y);
}

void CommandHandler::handleNavigate(const std::string &args,
                                    ChatTab *tab _UNUSED_)
{
    if (!player_node)
        return;

    int x = 0;
    int y = 0;

    if (parse2Int(args, &x, &y))
        player_node->navigateTo(x, y);
    else
        player_node->naviageClean();
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
                                  ChatTab *tab _UNUSED_)
{
    if (!actorSpriteManager || !player_node)
        return;

    Being* target = actorSpriteManager->findNearestByName(args);
    if (target)
        player_node->setTarget(target);
}

void CommandHandler::handleCloseAll(const std::string &args _UNUSED_,
                                    ChatTab *tab _UNUSED_)
{
    if (chatWindow)
        chatWindow->removeAllWhispers();
}

void CommandHandler::handleIgnoreAll(const std::string &args _UNUSED_,
                                     ChatTab *tab _UNUSED_)
{
    if (chatWindow)
        chatWindow->ignoreAllWhispers();
}

void CommandHandler::handleOutfit(const std::string &args,
                                  ChatTab *tab _UNUSED_)
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
                                 ChatTab *tab _UNUSED_)
{
    if (player_node)
        player_node->emote(static_cast<Uint8>(atoi(args.c_str())));
}

void CommandHandler::handleAway(const std::string &args, ChatTab *tab _UNUSED_)
{
    if (player_node)
        player_node->setAway(args);
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

void CommandHandler::handleHeal(const std::string &args, ChatTab *tab _UNUSED_)
{
    if (!actorSpriteManager)
        return;

    if (!args.empty())
    {
        Being *being = actorSpriteManager->findBeingByName(
            args, Being::PLAYER);
        if (being)
            actorSpriteManager->heal(player_node, being);
    }
    else
    {
        actorSpriteManager->heal(player_node, player_node);
    }
}

void CommandHandler::handleHack(const std::string &args, ChatTab *tab _UNUSED_)
{
    Net::getChatHandler()->sendRaw(args);
}

void CommandHandler::handleMail(const std::string &args, ChatTab *tab _UNUSED_)
{
    Net::getChatHandler()->privateMessage("AuctionBot", "!mail " + args);
}

void CommandHandler::handlePriceLoad(const std::string &args _UNUSED_,
                                     ChatTab *tab _UNUSED_)
{
    if (shopWindow)
        shopWindow->loadList();
}

void CommandHandler::handlePriceSave(const std::string &args _UNUSED_,
                                     ChatTab *tab _UNUSED_)
{
    if (shopWindow)
        shopWindow->saveList();
}

void CommandHandler::handleDisconnect(const std::string &args _UNUSED_,
                                      ChatTab *tab _UNUSED_)
{
    Net::getGameHandler()->disconnect2();
}

void CommandHandler::handleUndress(const std::string &args,
                                   ChatTab *tab _UNUSED_)
{
    if (!actorSpriteManager)
        return;

    Being* target = actorSpriteManager->findNearestByName(args);
    if (target)
        Net::getBeingHandler()->undress(target);
}

void CommandHandler::handleAttack(const std::string &args,
                                  ChatTab *tab _UNUSED_)
{
    if (!player_node || !actorSpriteManager)
        return;

    Being* target = actorSpriteManager->findNearestByName(args);
    if (target)
        player_node->setTarget(target);
    player_node->attack2(player_node->getTarget(), true);
}

void CommandHandler::handleTrade(const std::string &args,
                                 ChatTab *tab _UNUSED_)
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

void CommandHandler::handleDirs(const std::string &args _UNUSED_,
                                ChatTab *tab _UNUSED_)
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

void CommandHandler::handleInfo(const std::string &args _UNUSED_,
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
                                ChatTab *tab _UNUSED_)
{
    if (player_node)
        player_node->waitFor(args);
}

void CommandHandler::handleUptime(const std::string &args _UNUSED_,
                                  ChatTab *tab _UNUSED_)
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

void CommandHandler::handleCacheInfo(const std::string &args _UNUSED_,
                                     ChatTab *tab _UNUSED_)
{
    if (!chatWindow || !debugChatTab)
        return;

    TrueTypeFont *font = dynamic_cast<TrueTypeFont *>(chatWindow->getFont());
    if (!font)
        return;

    std::list<TextChunk> *cache = font->getCache();
    if (!cache)
        return;

    debugChatTab->chatLog("font cache size");
    std::string str;
    for (int f = 0; f < 256; f ++)
    {
        if (!cache[f].empty())
            str += strprintf("%d: %u, ", f, (unsigned int)cache[f].size());
    }
    debugChatTab->chatLog(str);
#ifdef DEBUG_FONT_COUNTERS
    debugChatTab->chatLog("");
    debugChatTab->chatLog("Created: " + toString(font->getCreateCounter()));
    debugChatTab->chatLog("Deleted: " + toString(font->getDeleteCounter()));
#endif
}
