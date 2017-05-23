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

#include "actions/chat.h"

#include "configuration.h"

#include "actions/actiondef.h"

#include "const/gui/chat.h"

#include "being/localplayer.h"

#include "gui/sdlinput.h"

#include "gui/windows/chatwindow.h"

#include "listeners/inputactionreplaylistener.h"

#include "net/charserverhandler.h"
#include "net/chathandler.h"
#include "net/guildhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"

#ifdef TMWA_SUPPORT
#include "net/tmwa/guildmanager.h"
#endif  // TMWA_SUPPORT

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/booleanoptions.h"
#include "utils/chatutils.h"
#include "utils/parameters.h"

#include "utils/translation/podict.h"

#include "debug.h"

const int DEFAULT_CHAT_WINDOW_SCROLL = 7;

namespace Actions
{

static void outString(ChatTab *const tab,
                      const std::string &str,
                      const std::string &def)
{
    if (!tab)
    {
        if (chatHandler)
            chatHandler->talk(def, GENERAL_CHANNEL);
        return;
    }

    switch (tab->getType())
    {
        case ChatTabType::PARTY:
        {
            if (partyHandler)
                partyHandler->chat(str);
            break;
        }
        case ChatTabType::GUILD:
        {
            if (!guildHandler || !localPlayer)
                return;
            const Guild *const guild = localPlayer->getGuild();
            if (guild)
            {
#ifdef TMWA_SUPPORT
                if (guild->getServerGuild())
                {
                    if (Net::getNetworkType() == ServerType::TMWATHENA)
                        return;
                    guildHandler->chat(str);
                }
                else if (guildManager)
                {
                    guildManager->chat(str);
                }
#else  // TMWA_SUPPORT

                if (guild->getServerGuild())
                    guildHandler->chat(str);
#endif  // TMWA_SUPPORT
            }
            break;
        }
        case ChatTabType::CHANNEL:
        case ChatTabType::GM:
        case ChatTabType::TRADE:
            tab->chatInput(str);
            break;
        default:
        case ChatTabType::UNKNOWN:
        case ChatTabType::INPUT:
        case ChatTabType::WHISPER:
        case ChatTabType::DEBUG:
        case ChatTabType::BATTLE:
        case ChatTabType::LANG:
            if (chatHandler)
                chatHandler->talk(str, GENERAL_CHANNEL);
            break;
    }
}

impHandler0(toggleChat)
{
    return chatWindow ? chatWindow->requestChatFocus() : false;
}

impHandler0(prevChatTab)
{
    if (chatWindow)
    {
        chatWindow->prevTab();
        return true;
    }
    return false;
}

impHandler0(nextChatTab)
{
    if (chatWindow)
    {
        chatWindow->nextTab();
        return true;
    }
    return false;
}

impHandler0(closeChatTab)
{
    if (chatWindow)
    {
        chatWindow->closeTab();
        return true;
    }
    return false;
}

impHandler0(closeAllChatTabs)
{
    if (chatWindow)
    {
        chatWindow->removeAllWhispers();
        chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler0(ignoreAllWhispers)
{
    if (chatWindow)
    {
        chatWindow->ignoreAllWhispers();
        chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler0(scrollChatUp)
{
    if (chatWindow && chatWindow->isWindowVisible())
    {
        chatWindow->scroll(-DEFAULT_CHAT_WINDOW_SCROLL);
        return true;
    }
    return false;
}

impHandler0(scrollChatDown)
{
    if (chatWindow && chatWindow->isWindowVisible())
    {
        chatWindow->scroll(DEFAULT_CHAT_WINDOW_SCROLL);
        return true;
    }
    return false;
}

static bool splitWhisper(const std::string &args,
                         std::string &recvnick,
                         std::string &message)
{
    if (args.substr(0, 1) == "\"")
    {
        const size_t pos = args.find('"', 1);
        if (pos != std::string::npos)
        {
            recvnick = args.substr(1, pos - 1);
            if (pos + 2 < args.length())
                message = args.substr(pos + 2, args.length());
        }
    }
    else
    {
        const size_t pos = args.find(' ');
        if (pos != std::string::npos)
        {
            recvnick = args.substr(0, pos);
            if (pos + 1 < args.length())
                message = args.substr(pos + 1, args.length());
        }
        else
        {
            recvnick = std::string(args);
            message.clear();
        }
    }

    trim(message);

    if (message.length() > 0)
    {
        std::string playerName = localPlayer->getName();
        std::string tempNick = recvnick;

        toLower(playerName);
        toLower(tempNick);

        if (tempNick.compare(playerName) == 0 || args.empty())
            return false;

        return true;
    }
    return false;
}

impHandler(msg)
{
    std::string recvnick;
    std::string message;

    if (splitWhisper(event.args, recvnick, message))
    {
        if (!chatWindow)
            return false;
        ChatTab *const tab = chatWindow->addChatTab(recvnick, false, true);
        if (tab)
        {
            chatWindow->saveState();
            tab->chatInput(message);
        }
    }
    else
    {
        if (event.tab)
        {
            event.tab->chatLog(
                // TRANSLATORS: whisper send
                _("Cannot send empty whisper or channel message!"),
                ChatMsgType::BY_SERVER);
        }
    }
    return true;
}

impHandler(msgText)
{
    if (!chatWindow)
        return false;

    if (config.getBoolValue("whispertab"))
    {
        chatWindow->localChatInput("/q " + event.args);
    }
    else
    {
        chatWindow->addInputText(std::string("/w \"").append(
            event.args).append("\" "));
    }
    return true;
}

impHandler(msg2)
{
    std::string recvnick;
    std::string message;

    if (chatHandler && splitWhisper(event.args, recvnick, message))
        chatHandler->privateMessage(recvnick, message);
    return true;
}

impHandler(query)
{
    const std::string &args = event.args;
    if (chatWindow)
    {
        if (chatWindow->addChatTab(args, true, true))
        {
            chatWindow->saveState();
            return true;
        }
    }

    if (event.tab)
    {
        // TRANSLATORS: new whisper or channel query
        event.tab->chatLog(strprintf(_("Cannot create a whisper tab "
            "\"%s\"! It probably already exists."),
            args.c_str()), ChatMsgType::BY_SERVER);
    }
    return true;
}

impHandler0(clearChatTab)
{
    if (chatWindow)
    {
        chatWindow->clearTab();
        return true;
    }
    return false;
}

impHandler(createParty)
{
    if (!partyHandler)
        return false;

    if (event.args.empty())
    {
        // TRANSLATORS: dialog header
        inputActionReplayListener.openDialog(_("Create party"),
            "",
            InputAction::CREATE_PARTY);
    }
    else
    {
        partyHandler->create(event.args);
    }
    return true;
}

impHandler(createGuild)
{
    if (!guildHandler ||
        Net::getNetworkType() == ServerType::TMWATHENA)
    {
        return false;
    }

    if (event.args.empty())
    {
        // TRANSLATORS: dialog header
        inputActionReplayListener.openDialog(_("Create guild"),
            "",
            InputAction::CREATE_GUILD);
    }
    else
    {
        guildHandler->create(event.args);
    }
    return true;
}

impHandler(party)
{
    if (!event.args.empty())
    {
        if (partyHandler)
            partyHandler->invite(event.args);
    }
    else
    {
        if (event.tab)
        {
            // TRANSLATORS: party invite message
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER);
        }
    }
    return true;
}

impHandler(guild)
{
    if (!guildHandler || !localPlayer)
        return false;

    const std::string args = event.args;
    if (!args.empty())
    {
        const Guild *const guild = localPlayer->getGuild();
        if (guild)
        {
#ifdef TMWA_SUPPORT
            if (guild->getServerGuild())
                guildHandler->invite(args);
            else if (guildManager)
                guildManager->invite(args);
#else  // TMWA_SUPPORT

            guildHandler->invite(args);
#endif  // TMWA_SUPPORT
        }
    }
    else
    {
        if (event.tab)
        {
            // TRANSLATORS: guild invite message
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER);
        }
        else if (localChatTab)
        {
            // TRANSLATORS: guild invite message
            localChatTab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER);
        }
    }
    return true;
}

impHandler(me)
{
    outString(event.tab, textToMe(event.args), event.args);
    return true;
}

impHandler(toggle)
{
    if (event.args.empty())
    {
        if (chatWindow && event.tab)
        {
            event.tab->chatLog(chatWindow->getReturnTogglesChat() ?
                // TRANSLATORS: message from toggle chat command
                _("Return toggles chat.") : _("Message closes chat."),
                ChatMsgType::BY_SERVER);
        }
        return true;
    }

    switch (parseBoolean(event.args))
    {
        case 1:
            if (event.tab)
            {
                // TRANSLATORS: message from toggle chat command
                event.tab->chatLog(_("Return now toggles chat."),
                    ChatMsgType::BY_SERVER);
            }
            if (chatWindow)
                chatWindow->setReturnTogglesChat(true);
            return true;
        case 0:
            if (event.tab)
            {
                // TRANSLATORS: message from toggle chat command
                event.tab->chatLog(_("Message now closes chat."),
                    ChatMsgType::BY_SERVER);
            }
            if (chatWindow)
                chatWindow->setReturnTogglesChat(false);
            return true;
        case -1:
            if (event.tab)
            {
                event.tab->chatLog(strprintf(BOOLEAN_OPTIONS, "toggle"),
                    ChatMsgType::BY_SERVER);
            }
            return true;
        default:
            return true;
    }
}

impHandler(kickParty)
{
    if (!event.args.empty())
    {
        if (partyHandler)
            partyHandler->kick(event.args);
    }
    else
    {
        if (event.tab)
        {
            // TRANSLATORS: party kick message
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER);
        }
    }
    return true;
}

impHandler(kickGuild)
{
    if (!event.args.empty())
    {
        if (localPlayer)
        {
            const Guild *const guild = localPlayer->getGuild();
            if (guild)
            {
                if (guild->getServerGuild())
                {
                    if (guildHandler)
                        guildHandler->kick(guild->getMember(event.args), "");
                }
#ifdef TMWA_SUPPORT
                else if (guildManager)
                {
                    guildManager->kick(event.args);
                }
#endif  // TMWA_SUPPORT
            }
        }
    }
    else
    {
        if (event.tab)
        {
            // TRANSLATORS: party kick message
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER);
        }
    }
    return true;
}

impHandler(addText)
{
    if (chatWindow)
        chatWindow->addInputText(event.args);
    return true;
}

impHandler0(clearChat)
{
    if (chatWindow)
        chatWindow->clearTab();
    return true;
}

impHandler0(chatGeneralTab)
{
    if (chatWindow)
        chatWindow->selectTabByType(ChatTabType::INPUT);
    return true;
}

impHandler0(chatDebugTab)
{
    if (chatWindow)
        chatWindow->selectTabByType(ChatTabType::DEBUG);
    return true;
}

impHandler0(chatBattleTab)
{
    if (chatWindow)
        chatWindow->selectTabByType(ChatTabType::BATTLE);
    return true;
}

impHandler0(chatTradeTab)
{
    if (chatWindow)
        chatWindow->selectTabByType(ChatTabType::TRADE);
    return true;
}

impHandler0(chatLangTab)
{
    if (chatWindow)
        chatWindow->selectTabByType(ChatTabType::LANG);
    return true;
}

impHandler0(chatGmTab)
{
    if (chatWindow)
        chatWindow->selectTabByType(ChatTabType::GM);
    return true;
}

impHandler0(chatPartyTab)
{
    if (chatWindow)
        chatWindow->selectTabByType(ChatTabType::PARTY);
    return true;
}

impHandler0(chatGuildTab)
{
    if (chatWindow)
        chatWindow->selectTabByType(ChatTabType::GUILD);
    return true;
}

impHandler(hat)
{
    if (!localPlayer || !charServerHandler)
        return false;

    const int sprite = localPlayer->getSpriteID(
        charServerHandler->hatSprite());
    std::string str;
    if (sprite == 0)
    {
        // TRANSLATORS: equipped hat chat message
        str = strprintf(_("no hat equipped."));
    }
    else
    {
        const ItemInfo &info = ItemDB::get(sprite);
        // TRANSLATORS: equipped hat chat message
        str = strprintf(_("equipped hat %s."),
            info.getName().c_str());
    }
    outString(event.tab, str, str);
    return true;
}

impHandler(chatClipboard)
{
    int x = 0;
    int y = 0;

    if (chatWindow && parse2Int(event.args, x, y))
    {
        chatWindow->copyToClipboard(x, y);
        return true;
    }
    return false;
}

impHandler(guildNotice)
{
    if (!localPlayer)
        return false;
    const std::string args = event.args;
    if (args.empty())
    {
        // TRANSLATORS: dialog header
        inputActionReplayListener.openDialog(_("Guild notice"),
            "",
            InputAction::GUILD_NOTICE);
        return true;
    }

    const std::string str1 = args.substr(0, 60);
    std::string str2("");
    if (args.size() > 60)
        str2 = args.substr(60);
    const Guild *const guild = localPlayer->getGuild();
    if (guild)
        guildHandler->changeNotice(guild->getId(), str1, str2);
    return true;
}

impHandler(translate)
{
    if (reverseDictionary == nullptr ||
        localPlayer == nullptr ||
        event.args.empty())
    {
        return false;
    }

    ChatTab *const tab = event.tab;
    if (tab == nullptr)
        return false;

    std::string srcStr = event.args;
    std::string enStr;
    toLower(srcStr);
    if (localPlayer->getLanguageId() > 0)
    {
        if (reverseDictionary->haveStr(srcStr))
            enStr = reverseDictionary->getStr(srcStr);
        else if (dictionary->haveStr(srcStr))
            enStr = srcStr;
    }
    else
    {
        if (dictionary->haveStr(srcStr))
            enStr = srcStr;
    }

    if (enStr.empty())
    {
        tab->chatLog(
            // TRANSLATORS: translation error message
            strprintf(_("No translation found for string: %s"),
            srcStr.c_str()),
            ChatMsgType::BY_SERVER);
        return true;
    }

    tab->chatInput(enStr);
    return true;
}

impHandler(sendGuiKey)
{
    if (!guiInput)
        return false;

    const std::string args = event.args;
    if (args.empty())
        return false;
    StringVect pars;
    if (!splitParameters(pars, args, " ,", '\"'))
        return false;
    const int sz = CAST_S32(pars.size());
    if (sz < 1)
        return false;

    int keyValue = atoi(pars[0].c_str());
    if (keyValue == 0 &&
        pars[0].size() == 1)
    {
        keyValue = CAST_S32(pars[0][0]);
    }
    if (sz == 2)
    {
        const InputActionT actionId = inputManager.getActionByConfigField(
            pars[1]);
        guiInput->simulateKey(keyValue, actionId);
    }
    else
    {
        guiInput->simulateKey(keyValue, InputAction::NO_VALUE);
    }
    return true;
}

impHandler(sendMouseKey)
{
    if (!guiInput)
        return false;
    const std::string args = event.args;
    if (args.empty())
        return false;
    StringVect pars;
    if (!splitParameters(pars, args, " ,", '\"'))
        return false;
    const int sz = CAST_S32(pars.size());
    if (sz != 3)
        return false;

    const int x = atoi(pars[0].c_str());
    const int y = atoi(pars[1].c_str());
    const int key1 = CAST_S32(MouseButton::LEFT);
    const int key2 = CAST_S32(MouseButton::MIDDLE);
    const int key = atoi(pars[2].c_str());
    if (key < key1 || key > key2)
        return false;
    guiInput->simulateMouseClick(x,
        y,
        static_cast<MouseButtonT>(key));
    return true;
}

}  // namespace Actions
