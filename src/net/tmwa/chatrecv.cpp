/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/tmwa/chatrecv.h"

#include "actormanager.h"
#include "configuration.h"

#include "being/localplayer.h"
#include "being/playerrelation.h"
#include "being/playerrelations.h"

#include "const/gui/chat.h"

#include "gui/widgets/tabs/chat/gmtab.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/shopwindow.h"

#include "net/serverfeatures.h"

#include "net/ea/chatrecv.h"

#include "net/messagein.h"

#include "net/tmwa/guildmanager.h"

#include "debug.h"

namespace TmwAthena
{

void ChatRecv::processChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processChat")
    const int chatMsgLength = msg.readInt16("len") - 4;
    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatRecv::processChat")
        return;
    }

    processChatContinue(msg.readRawString(chatMsgLength, "message"), "");
}

void ChatRecv::processChatContinue(std::string chatMsg,
                                   const std::string &channel)
{
    const size_t pos = chatMsg.find(" : ", 0);

    bool allow(true);
    if (chatWindow != nullptr)
    {
        allow = chatWindow->resortChatLog(chatMsg,
            ChatMsgType::BY_PLAYER,
            channel,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }

    if (channel.empty())
    {
        const std::string senseStr("You sense the following: ");
        if ((actorManager != nullptr) && (chatMsg.find(senseStr) == 0u))
        {
            actorManager->parseLevels(
                chatMsg.substr(senseStr.size()));
        }
    }

    if (pos == std::string::npos &&
        !Ea::ChatRecv::mShowMotd &&
        Ea::ChatRecv::mSkipping &&
        channel.empty())
    {
        // skip motd from "new" tmw server
        if (Ea::ChatRecv::mMotdTime == 0)
        {
            Ea::ChatRecv::mMotdTime = cur_time + 1;
        }
        else if (Ea::ChatRecv::mMotdTime == cur_time ||
                 Ea::ChatRecv::mMotdTime < cur_time)
        {
            Ea::ChatRecv::mSkipping = false;
        }
        BLOCK_END("ChatRecv::processChat")
        return;
    }

    if (pos != std::string::npos)
        chatMsg.erase(0, pos + 3);

    trim(chatMsg);

    if (localPlayer != nullptr)
    {
        if (((chatWindow != nullptr) || Ea::ChatRecv::mShowMotd) && allow)
            localPlayer->setSpeech(chatMsg, channel);
    }
    BLOCK_END("ChatRecv::processChat")
}

void ChatRecv::processGmChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processChat")
    const int chatMsgLength = msg.readInt16("len") - 4;
    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatRecv::processChat")
        return;
    }

    std::string chatMsg = msg.readRawString(chatMsgLength, "message");

    if (localChatTab != nullptr)
    {
        if (chatWindow != nullptr)
            chatWindow->addGlobalMessage(chatMsg);
    }
    BLOCK_END("ChatRecv::processChat")
}

void ChatRecv::processWhisper(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processWhisper")
    const int chatMsgLength = msg.readInt16("len") - 28;
    std::string nick = msg.readString(24, "nick");

    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatRecv::processWhisper")
        return;
    }

    processWhisperContinue(nick, msg.readString(chatMsgLength, "message"));
}

void ChatRecv::processWhisperResponse(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processWhisperResponse")

    const uint8_t type = msg.readUInt8("response");
    Ea::ChatRecv::processWhisperResponseContinue(msg, type);
}

void ChatRecv::processWhisperContinue(const std::string &nick,
                                      std::string chatMsg)
{
    // ignoring future whisper messages
    if (chatMsg.find("\302\202G") == 0 || chatMsg.find("\302\202A") == 0)
    {
        BLOCK_END("ChatRecv::processWhisper")
        return;
    }
    // remove first unicode space if this is may be whisper command.
    if (chatMsg.find("\302\202!") == 0)
        chatMsg = chatMsg.substr(2);

    if (nick != "Server")
    {
        if ((guildManager != nullptr) && GuildManager::getEnableGuildBot()
            && nick == "guild" && guildManager->processGuildMessage(chatMsg))
        {
            BLOCK_END("ChatRecv::processWhisper")
            return;
        }

        if (player_relations.hasPermission(nick, PlayerRelation::WHISPER))
        {
            const bool tradeBot = config.getBoolValue("tradebot");
            const bool showMsg = !config.getBoolValue("hideShopMessages");
            if (player_relations.hasPermission(nick, PlayerRelation::TRADE))
            {
                if (shopWindow != nullptr)
                {   // commands to shop from player
                    if (chatMsg.find("!selllist ") == 0)
                    {
                        if (tradeBot)
                        {
                            if (showMsg && (chatWindow != nullptr))
                                chatWindow->addWhisper(nick, chatMsg);
                            shopWindow->giveList(nick, ShopWindow::SELL);
                        }
                    }
                    else if (chatMsg.find("!buylist ") == 0)
                    {
                        if (tradeBot)
                        {
                            if (showMsg && (chatWindow != nullptr))
                                chatWindow->addWhisper(nick, chatMsg);
                            shopWindow->giveList(nick, ShopWindow::BUY);
                        }
                    }
                    else if (chatMsg.find("!buyitem ") == 0)
                    {
                        if (showMsg && (chatWindow != nullptr))
                            chatWindow->addWhisper(nick, chatMsg);
                        if (tradeBot)
                        {
                            shopWindow->processRequest(nick, chatMsg,
                                ShopWindow::BUY);
                        }
                    }
                    else if (chatMsg.find("!sellitem ") == 0)
                    {
                        if (showMsg && (chatWindow != nullptr))
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
                        if (showMsg && (chatWindow != nullptr))
                            chatWindow->addWhisper(nick, chatMsg);
                        if (chatMsg.find("B1") == 0 || chatMsg.find("S1") == 0)
                            shopWindow->showList(nick, chatMsg);
                    }
                    else if (chatWindow != nullptr)
                    {
                        chatWindow->addWhisper(nick, chatMsg);
                    }
                }
                else if (chatWindow != nullptr)
                {
                    chatWindow->addWhisper(nick, chatMsg);
                }
            }
            else
            {
                if (chatWindow != nullptr &&
                    (showMsg ||
                    (chatMsg.find("!selllist") != 0 &&
                    chatMsg.find("!buylist") != 0)))
                {
                    chatWindow->addWhisper(nick, chatMsg);
                }
            }
        }
    }
    else if (localChatTab != nullptr)
    {
        if ((gmChatTab != nullptr) && strStartWith(chatMsg, "[GM] "))
        {
            chatMsg = chatMsg.substr(5);
            const size_t pos = chatMsg.find(": ", 0);
            if (pos == std::string::npos)
            {
                gmChatTab->chatLog(chatMsg, ChatMsgType::BY_SERVER);
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
    BLOCK_END("ChatRecv::processWhisper")
}

void ChatRecv::processBeingChat(Net::MessageIn &msg)
{
    if (actorManager == nullptr)
        return;

    BLOCK_START("ChatRecv::processBeingChat")
    const int chatMsgLength = msg.readInt16("len") - 8;
    const BeingId beingId = msg.readBeingId("being id");
    Being *const being = actorManager->findBeing(beingId);

    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatRecv::processBeingChat")
        return;
    }

    std::string chatMsg = msg.readRawString(chatMsgLength, "message");

    if ((being != nullptr) && being->getType() == ActorType::Player)
        being->setTalkTime();

    const size_t pos = chatMsg.find(" : ", 0);
    std::string sender_name = ((pos == std::string::npos)
        ? "" : chatMsg.substr(0, pos));

    if (serverFeatures->haveIncompleteChatMessages())
    {
        // work around for "new" tmw server
        if (being != nullptr)
            sender_name = being->getName();
        if (sender_name.empty())
        {
            sender_name = "?" + toString(CAST_S32(beingId));
            const std::string name = actorManager->getSeenPlayerById(beingId);
            if (!name.empty())
                sender_name.append(" ").append(name);
        }
    }
    else if ((being != nullptr) &&
             sender_name != being->getName() &&
             being->getType() == ActorType::Player)
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
    if ((player_relations.checkPermissionSilently(sender_name,
        PlayerRelation::SPEECH_LOG) != 0u) &&
        (chatWindow != nullptr))
    {
        allow = chatWindow->resortChatLog(
            removeColors(sender_name).append(" : ").append(chatMsg),
            ChatMsgType::BY_OTHER,
            GENERAL_CHANNEL,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }

    if (allow &&
        (being != nullptr) &&
        player_relations.hasPermission(sender_name,
        PlayerRelation::SPEECH_FLOAT))
    {
        being->setSpeech(chatMsg, GENERAL_CHANNEL);
    }
    BLOCK_END("ChatRecv::processBeingChat")
}

void ChatRecv::processScriptMessage(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 5;
    msg.readUInt8("message type");
    const std::string message = msg.readString(sz, "message");
    localChatTab->chatLog(message, ChatMsgType::BY_SERVER);
}

}  // namespace TmwAthena
