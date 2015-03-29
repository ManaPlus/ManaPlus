/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/tmwa/chathandler.h"

#include "actormanager.h"
#include "configuration.h"
#include "guildmanager.h"

#include "being/localplayer.h"
#include "being/playerrelations.h"

#include "gui/chatconsts.h"

#include "gui/widgets/tabs/chat/gmtab.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/shopwindow.h"

#include "net/serverfeatures.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "utils/stringutils.h"

#include <string>

#include "debug.h"

extern Net::ChatHandler *chatHandler;

namespace TmwAthena
{

ChatHandler::ChatHandler() :
    MessageHandler(),
    Ea::ChatHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_BEING_CHAT,
        SMSG_BEING_CHAT2,
        SMSG_PLAYER_CHAT,
        SMSG_PLAYER_CHAT2,
        SMSG_WHISPER,
        SMSG_WHISPER_RESPONSE,
        SMSG_GM_CHAT,
        SMSG_MVP_EFFECT,
        SMSG_IGNORE_ALL_RESPONSE,
        0
    };
    handledMessages = _messages;
    chatHandler = this;
}

void ChatHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_WHISPER_RESPONSE:
            processWhisperResponse(msg);
            break;

        // Received whisper
        case SMSG_WHISPER:
            processWhisper(msg);
            break;

        // Received speech from being
        case SMSG_BEING_CHAT:
            processBeingChat(msg);
            break;

        // Received speech from being
        case SMSG_BEING_CHAT2:
            processBeingChat(msg);
            break;

        case SMSG_PLAYER_CHAT:
            processChat(msg);
            break;

        case SMSG_PLAYER_CHAT2:
            processChat2(msg);
            break;

        case SMSG_GM_CHAT:
            processGmChat(msg);
            break;

        case SMSG_MVP_EFFECT:
            processMVPEffect(msg);
            break;

        case SMSG_IGNORE_ALL_RESPONSE:
            processIgnoreAllResponse(msg);
            break;

        default:
            break;
    }
    BLOCK_END("ChatHandler::handleMessage")
}

void ChatHandler::talk(const std::string &restrict text,
                       const std::string &restrict channel) const
{
    if (!localPlayer)
        return;

    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);

    if (serverFeatures->haveSpecialChatChannels() && channel.size() == 3)
    {
        createOutPacket(CMSG_CHAT_MESSAGE2);
        // Added + 1 in order to let eAthena parse admin commands correctly
        outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4 + 3 + 1),
            "len");
        outMsg.writeInt8(channel[0], "channel byte 0");
        outMsg.writeInt8(channel[1], "channel byte 1");
        outMsg.writeInt8(channel[2], "channel byte 2");
        outMsg.writeString(mes, static_cast<int>(mes.length() + 1), "message");
    }
    else
    {
        createOutPacket(CMSG_CHAT_MESSAGE);
        // Added + 1 in order to let eAthena parse admin commands correctly
        outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4 + 1), "len");
        outMsg.writeString(mes, static_cast<int>(mes.length() + 1), "message");
    }
}

void ChatHandler::talkRaw(const std::string &mes) const
{
    createOutPacket(CMSG_CHAT_MESSAGE);
    outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4), "len");
    outMsg.writeString(mes, static_cast<int>(mes.length()), "message");
}

void ChatHandler::privateMessage(const std::string &restrict recipient,
                                 const std::string &restrict text)
{
    createOutPacket(CMSG_CHAT_WHISPER);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 28), "len");
    outMsg.writeString(recipient, 24, "recipient nick");
    outMsg.writeString(text, static_cast<int>(text.length()), "message");
    mSentWhispers.push(recipient);
}

void ChatHandler::channelMessage(const std::string &restrict channel,
                                 const std::string &restrict text)
{
    if (channel == TRADE_CHANNEL)
        talk("\302\202" + text, GENERAL_CHANNEL);
    else if (channel == GM_CHANNEL)
        talk("@wgm " + text, GENERAL_CHANNEL);
}

void ChatHandler::who() const
{
    createOutPacket(CMSG_WHO_REQUEST);
}

void ChatHandler::sendRaw(const std::string &args) const
{
    std::string line = args;
    std::string str;
    MessageOut *outMsg = nullptr;

    if (line == "")
        return;

    size_t pos = line.find(" ");
    if (pos != std::string::npos)
    {
        str = line.substr(0, pos);
        outMsg = new MessageOut(static_cast<int16_t>(parseNumber(str)));
        line = line.substr(pos + 1);
        pos = line.find(" ");
    }
    else
    {
        outMsg = new MessageOut(static_cast<int16_t>(parseNumber(line)));
        delete outMsg;
        return;
    }

    while (pos != std::string::npos)
    {
        str = line.substr(0, pos);
        processRaw(*outMsg, str);
        line = line.substr(pos + 1);
        pos = line.find(" ");
    }
    if (line != "")
        processRaw(*outMsg, line);
    delete outMsg;
}

void ChatHandler::processRaw(MessageOut &restrict outMsg,
                             const std::string &restrict line)
{
    if (line.size() < 2)
        return;

    const uint32_t i = parseNumber(line.substr(1));
    switch (tolower(line[0]))
    {
        case 'b':
        {
            outMsg.writeInt8(static_cast<unsigned char>(i), "raw");
            break;
        }
        case 'w':
        {
            outMsg.writeInt16(static_cast<int16_t>(i), "raw");
            break;
        }
        case 'l':
        {
            outMsg.writeInt32(static_cast<int32_t>(i), "raw");
            break;
        }
        default:
            break;
    }
}

void ChatHandler::ignoreAll() const
{
    if (!serverFeatures->haveServerIgnore())
        return;
    createOutPacket(CMSG_IGNORE_ALL);
    outMsg.writeInt8(0, "flag");
}

void ChatHandler::unIgnoreAll() const
{
    if (!serverFeatures->haveServerIgnore())
        return;
    createOutPacket(CMSG_IGNORE_ALL);
    outMsg.writeInt8(1, "flag");
}

void ChatHandler::ignore(const std::string &nick) const
{
    createOutPacket(CMSG_IGNORE_NICK);
    outMsg.writeString(nick, 24, "nick");
    outMsg.writeInt8(0, "flag");
}

void ChatHandler::unIgnore(const std::string &nick) const
{
    createOutPacket(CMSG_IGNORE_NICK);
    outMsg.writeString(nick, 24, "nick");
    outMsg.writeInt8(1, "flag");
}

void ChatHandler::requestIgnoreList() const
{
}

void ChatHandler::createChatRoom(const std::string &title A_UNUSED,
                                 const std::string &password A_UNUSED,
                                 const int limit A_UNUSED,
                                 const bool isPublic A_UNUSED)
{
}

void ChatHandler::battleTalk(const std::string &text A_UNUSED) const
{
}

void ChatHandler::processChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processChat")
    const int chatMsgLength = msg.readInt16("len") - 4;
    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatHandler::processChat")
        return;
    }

    processChatContinue(msg.readRawString(chatMsgLength, "message"), "");
}

void ChatHandler::processChat2(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processChat")
    const int chatMsgLength = msg.readInt16("len") - 4 - 3;
    std::string channel;
    channel = msg.readUInt8("channel byte 0");
    channel += msg.readUInt8("channel byte 1");
    channel += msg.readUInt8("channel byte 2");
    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatHandler::processChat")
        return;
    }

    processChatContinue(msg.readRawString(chatMsgLength, "message"), channel);
}

void ChatHandler::processChatContinue(std::string chatMsg,
                                      const std::string &channel)
{
    const size_t pos = chatMsg.find(" : ", 0);

    bool allow(true);
    if (chatWindow)
    {
        allow = chatWindow->resortChatLog(chatMsg, ChatMsgType::BY_PLAYER,
            channel, false, true);
    }

    if (channel.empty())
    {
        const std::string senseStr("You sense the following: ");
        if (actorManager && !chatMsg.find(senseStr))
        {
            actorManager->parseLevels(
                chatMsg.substr(senseStr.size()));
        }
    }

    if (pos == std::string::npos && !mShowMotd
        && mSkipping && channel.empty())
    {
        // skip motd from "new" tmw server
        if (mMotdTime == -1)
            mMotdTime = cur_time + 1;
        else if (mMotdTime == cur_time || mMotdTime < cur_time)
            mSkipping = false;
        BLOCK_END("ChatHandler::processChat")
        return;
    }

    if (pos != std::string::npos)
        chatMsg.erase(0, pos + 3);

    trim(chatMsg);

    if (localPlayer)
    {
        if ((chatWindow || mShowMotd) && allow)
            localPlayer->setSpeech(chatMsg, channel);
    }
    BLOCK_END("ChatHandler::processChat")
}

void ChatHandler::processGmChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processChat")
    const bool channels = msg.getId() == SMSG_PLAYER_CHAT2;
    const bool normalChat = msg.getId() == SMSG_PLAYER_CHAT
        || msg.getId() == SMSG_PLAYER_CHAT2;
    int chatMsgLength = msg.readInt16("len") - 4;
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
        BLOCK_END("ChatHandler::processChat")
        return;
    }

    std::string chatMsg = msg.readRawString(chatMsgLength, "message");
    const size_t pos = chatMsg.find(" : ", 0);

    if (normalChat)
    {
        bool allow(true);
        if (chatWindow)
        {
            allow = chatWindow->resortChatLog(chatMsg, ChatMsgType::BY_PLAYER,
                channel, false, true);
        }

        if (channel.empty())
        {
            const std::string senseStr("You sense the following: ");
            if (actorManager && !chatMsg.find(senseStr))
            {
                actorManager->parseLevels(
                    chatMsg.substr(senseStr.size()));
            }
        }

        if (pos == std::string::npos && !mShowMotd
            && mSkipping && channel.empty())
        {
            // skip motd from "new" tmw server
            if (mMotdTime == -1)
                mMotdTime = cur_time + 1;
            else if (mMotdTime == cur_time || mMotdTime < cur_time)
                mSkipping = false;
            BLOCK_END("ChatHandler::processChat")
            return;
        }

        if (pos != std::string::npos)
            chatMsg.erase(0, pos + 3);

        trim(chatMsg);

        if (localPlayer)
        {
            if ((chatWindow || mShowMotd) && allow)
                localPlayer->setSpeech(chatMsg, channel);
        }
    }
    else if (localChatTab)
    {
        if (chatWindow)
            chatWindow->addGlobalMessage(chatMsg);
    }
    BLOCK_END("ChatHandler::processChat")
}

void ChatHandler::processWhisper(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processWhisper")
    const int chatMsgLength = msg.readInt16("len") - 28;
    std::string nick = msg.readString(24, "nick");

    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatHandler::processWhisper")
        return;
    }

    processWhisperContinue(nick, msg.readString(chatMsgLength, "message"));
}

void ChatHandler::processWhisperResponse(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processWhisperResponse")

    const uint8_t type = msg.readUInt8("response");
    processWhisperResponseContinue(type);
}

void ChatHandler::joinChat(const ChatObject *const chat A_UNUSED,
                           const std::string &password A_UNUSED) const
{
}

void ChatHandler::joinChannel(const std::string &channel A_UNUSED)
{
}

void ChatHandler::partChannel(const std::string &channel A_UNUSED)
{
}

void ChatHandler::processWhisperContinue(const std::string &nick,
                                         std::string chatMsg)
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

void ChatHandler::processBeingChat(Net::MessageIn &msg)
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

void ChatHandler::talkPet(const std::string &restrict text,
                          const std::string &restrict channel) const
{
    // here need string duplication
    std::string action = strprintf("\302\202\303 %s", text.c_str());
    talk(action, channel);
}

void ChatHandler::leaveChatRoom() const
{
}

void ChatHandler::setChatRoomOptions(const int limit A_UNUSED,
                                     const bool isPublict A_UNUSED,
                                     const std::string &passwordt A_UNUSED,
                                     const std::string &titlet A_UNUSED) const
{
}

void ChatHandler::setChatRoomOwner(const std::string &nick A_UNUSED) const
{
}

void ChatHandler::kickFromChatRoom(const std::string &nick A_UNUSED) const
{
}

}  // namespace TmwAthena
