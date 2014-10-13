/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/ea/chathandler.h"

#include "actormanager.h"
#include "configuration.h"
#include "guildmanager.h"
#include "notifymanager.h"

#include "being/being.h"
#include "being/playerrelations.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/shopwindow.h"

#include "gui/widgets/tabs/chat/gmtab.h"

#include "net/messagein.h"
#include "net/serverfeatures.h"

#include "net/ea/eaprotocol.h"

#include "resources/notifytypes.h"

#include "utils/gettext.h"

#include <string>

#include "debug.h"

namespace Ea
{

ChatHandler::ChatHandler() :
    mSentWhispers(),
    mMotdTime(-1),
    mShowAllLang(serverConfig.getValue("showAllLang", 0)),
    mShowMotd(config.getBoolValue("showmotd")),
    mSkipping(true)
{
}

void ChatHandler::clear()
{
    mShowMotd = config.getBoolValue("showmotd");
    mSkipping = true;
}

void ChatHandler::me(const std::string &restrict text,
                     const std::string &restrict channel) const
{
    // here need string duplication
    std::string action = strprintf("*%s*", text.c_str());
    talk(action, channel);
}

void ChatHandler::talkPet(const std::string &restrict text,
                          const std::string &restrict channel) const
{
    // here need string duplication
    std::string action = strprintf("\302\202\303 %s", text.c_str());
    talk(action, channel);
}

std::string ChatHandler::getPopLastWhisperNick()
{
    std::string nick;
    if (mSentWhispers.empty())
    {
        nick = "user";
    }
    else
    {
        nick = mSentWhispers.front();
        mSentWhispers.pop();
    }
    return nick;
}

std::string ChatHandler::getLastWhisperNick()
{
    std::string nick;
    if (mSentWhispers.empty())
        nick = "user";
    else
        nick = mSentWhispers.front();
    return nick;
}

void ChatHandler::processWhisperResponseContinue(const uint8_t type)
{
    const std::string nick = getPopLastWhisperNick();
    switch (type)
    {
        case 0x00:
            // Success (don't need to report)
            break;
        case 0x01:
            if (chatWindow)
            {
                chatWindow->addWhisper(nick,
                    // TRANSLATORS: chat message
                    strprintf(_("Whisper could not be sent, %s is offline."),
                        nick.c_str()),
                        ChatMsgType::BY_SERVER);
            }
            break;
        case 0x02:
            if (chatWindow)
            {
                chatWindow->addWhisper(nick,
                    // TRANSLATORS: chat message
                    strprintf(_("Whisper could not "
                    "be sent, ignored by %s."), nick.c_str()),
                    ChatMsgType::BY_SERVER);
            }
            break;
        case 0x03:
            if (chatWindow)
            {
                chatWindow->addWhisper(nick,
                    // TRANSLATORS: chat message
                    strprintf(_("Whisper could not "
                    "be sent, you ignored by all players.")),
                    ChatMsgType::BY_SERVER);
            }
            break;
        default:
            if (logger)
            {
                logger->log("QQQ SMSG_WHISPER_RESPONSE:"
                            + toString(type));
            }
            break;
    }
    BLOCK_END("ChatHandler::processWhisperResponse")
}

void ChatHandler::processWhisperContinue(const std::string &nick,
                                         std::string chatMsg) const
{
    // ignoring future whisper messages
    if (chatMsg.find("\302\202G") == 0 || chatMsg.find("\302\202A") == 0)
    {
        BLOCK_END("ChatHandler::processWhisper")
        return;
    }
    // remove first unicode space if this is may be whisper command.
    if (chatMsg.find("\302\202!") == 0)
        chatMsg = chatMsg.substr(2);

    if (nick != "Server")
    {
        if (guildManager && GuildManager::getEnableGuildBot()
            && nick == "guild" && guildManager->processGuildMessage(chatMsg))
        {
            BLOCK_END("ChatHandler::processWhisper")
            return;
        }

        if (player_relations.hasPermission(nick, PlayerRelation::WHISPER))
        {
            const bool tradeBot = config.getBoolValue("tradebot");
            const bool showMsg = !config.getBoolValue("hideShopMessages");
            if (player_relations.hasPermission(nick, PlayerRelation::TRADE))
            {
                if (shopWindow)
                {   // commands to shop from player
                    if (chatMsg.find("!selllist ") == 0)
                    {
                        if (tradeBot)
                        {
                            if (showMsg && chatWindow)
                                chatWindow->addWhisper(nick, chatMsg);
                            shopWindow->giveList(nick, ShopWindow::SELL);
                        }
                    }
                    else if (chatMsg.find("!buylist ") == 0)
                    {
                        if (tradeBot)
                        {
                            if (showMsg && chatWindow)
                                chatWindow->addWhisper(nick, chatMsg);
                            shopWindow->giveList(nick, ShopWindow::BUY);
                        }
                    }
                    else if (chatMsg.find("!buyitem ") == 0)
                    {
                        if (showMsg && chatWindow)
                            chatWindow->addWhisper(nick, chatMsg);
                        if (tradeBot)
                        {
                            shopWindow->processRequest(nick, chatMsg,
                                ShopWindow::BUY);
                        }
                    }
                    else if (chatMsg.find("!sellitem ") == 0)
                    {
                        if (showMsg && chatWindow)
                            chatWindow->addWhisper(nick, chatMsg);
                        if (tradeBot)
                        {
                            shopWindow->processRequest(nick, chatMsg,
                                ShopWindow::SELL);
                        }
                    }
                    else if (chatMsg.length() > 3
                             && chatMsg.find("\302\202") == 0)
                    {
                        chatMsg = chatMsg.erase(0, 2);
                        if (showMsg && chatWindow)
                            chatWindow->addWhisper(nick, chatMsg);
                        if (chatMsg.find("B1") == 0 || chatMsg.find("S1") == 0)
                            shopWindow->showList(nick, chatMsg);
                    }
                    else if (chatWindow)
                    {
                        chatWindow->addWhisper(nick, chatMsg);
                    }
                }
                else if (chatWindow)
                {
                    chatWindow->addWhisper(nick, chatMsg);
                }
            }
            else
            {
                if (chatWindow && (showMsg || (chatMsg.find("!selllist") != 0
                    && chatMsg.find("!buylist") != 0)))
                {
                    chatWindow->addWhisper(nick, chatMsg);
                }
            }
        }
    }
    else if (localChatTab)
    {
        if (gmChatTab && strStartWith(chatMsg, "[GM] "))
        {
            chatMsg = chatMsg.substr(5);
            const size_t pos = chatMsg.find(": ", 0);
            if (pos == std::string::npos)
            {
                gmChatTab->chatLog(chatMsg);
            }
            else
            {
                gmChatTab->chatLog(chatMsg.substr(0, pos),
                    chatMsg.substr(pos + 2));
            }
        }
        else
        {
            localChatTab->chatLog(chatMsg, ChatMsgType::BY_SERVER);
        }
    }
    BLOCK_END("ChatHandler::processWhisper")
}

void ChatHandler::processBeingChat(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    BLOCK_START("ChatHandler::processBeingChat")
    const bool channels = msg.getId() == SMSG_BEING_CHAT2;
    int chatMsgLength = msg.readInt16("len") - 8;
    Being *const being = actorManager->findBeing(msg.readInt32("being id"));
    if (!being)
    {
        BLOCK_END("ChatHandler::processBeingChat")
        return;
    }

    std::string channel;
    if (channels)
    {
        chatMsgLength -= 3;
        channel = msg.readUInt8("channel byte 0");
        channel += msg.readUInt8("channel byte 1");
        channel += msg.readUInt8("channel byte 2");
    }

    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatHandler::processBeingChat")
        return;
    }

    std::string chatMsg = msg.readRawString(chatMsgLength, "message");

    if (being->getType() == ActorType::Player)
        being->setTalkTime();

    const size_t pos = chatMsg.find(" : ", 0);
    std::string sender_name = ((pos == std::string::npos)
        ? "" : chatMsg.substr(0, pos));

    if (serverFeatures->haveIncompleteChatMessages())
    {
        // work around for "new" tmw server
        sender_name = being->getName();
        if (sender_name.empty())
            sender_name = "?";
    }
    else if (sender_name != being->getName()
             && being->getType() == ActorType::Player)
    {
        if (!being->getName().empty())
            sender_name = being->getName();
    }
    else
    {
        chatMsg.erase(0, pos + 3);
    }

    trim(chatMsg);

    bool allow(true);
    // We use getIgnorePlayer instead of ignoringPlayer here
    // because ignorePlayer' side effects are triggered
    // right below for Being::IGNORE_SPEECH_FLOAT.
    if (player_relations.checkPermissionSilently(sender_name,
        PlayerRelation::SPEECH_LOG) && chatWindow)
    {
        allow = chatWindow->resortChatLog(
            removeColors(sender_name).append(" : ").append(chatMsg),
            ChatMsgType::BY_OTHER, channel, false, true);
    }

    if (allow && player_relations.hasPermission(sender_name,
        PlayerRelation::SPEECH_FLOAT))
    {
        being->setSpeech(chatMsg, channel);
    }
    BLOCK_END("ChatHandler::processBeingChat")
}

void ChatHandler::processMVP(Net::MessageIn &msg) const
{
    BLOCK_START("ChatHandler::processMVP")
    // Display MVP player
    const int id = msg.readInt32("being id");
    if (localChatTab && actorManager && config.getBoolValue("showMVP"))
    {
        const Being *const being = actorManager->findBeing(id);
        if (!being)
            NotifyManager::notify(NotifyTypes::MVP_PLAYER, "");
        else
            NotifyManager::notify(NotifyTypes::MVP_PLAYER, being->getName());
    }
    BLOCK_END("ChatHandler::processMVP")
}

void ChatHandler::processIgnoreAllResponse(Net::MessageIn &msg) const
{
    BLOCK_START("ChatHandler::processIgnoreAllResponse")
    const uint8_t action = msg.readUInt8("action");
    const uint8_t fail = msg.readUInt8("result");
    if (!localChatTab)
    {
        BLOCK_END("ChatHandler::processIgnoreAllResponse")
        return;
    }

    switch (action)
    {
        case 0:
        {
            switch (fail)
            {
                case 0:
                    NotifyManager::notify(NotifyTypes::WHISPERS_IGNORED);
                    break;
                default:
                    NotifyManager::notify(NotifyTypes::
                        WHISPERS_IGNORE_FAILED);
                    break;
            }
            break;
        }
        case 1:
        {
            switch (fail)
            {
                case 0:
                    NotifyManager::notify(NotifyTypes::WHISPERS_UNIGNORED);
                    break;
                default:
                    NotifyManager::notify(NotifyTypes::
                        WHISPERS_UNIGNORE_FAILED);
                    break;
            }
            break;
        }
        default:
            // unknown result
            break;
    }
    BLOCK_END("ChatHandler::processIgnoreAllResponse")
}

}  // namespace Ea
