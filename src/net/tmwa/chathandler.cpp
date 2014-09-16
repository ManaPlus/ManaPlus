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

#include "net/tmwa/chathandler.h"

#include "actormanager.h"

#include "being/localplayer.h"

#include "gui/widgets/tabs/chattab.h"

#include "gui/windows/chatwindow.h"

#include "net/net.h"
#include "net/serverfeatures.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "utils/stringutils.h"

#include <string>

#include "debug.h"

extern Net::ChatHandler *chatHandler;
extern int serverVersion;
extern unsigned int tmwServerVersion;

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
        SMSG_MVP,  // MVP
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

        case SMSG_MVP:
            processMVP(msg);
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

    if (Net::getServerFeatures()->haveChatChannels() && channel.size() == 3)
    {
        MessageOut outMsg(CMSG_CHAT_MESSAGE2);
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
        MessageOut outMsg(CMSG_CHAT_MESSAGE);
        // Added + 1 in order to let eAthena parse admin commands correctly
        outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4 + 1), "len");
        outMsg.writeString(mes, static_cast<int>(mes.length() + 1), "message");
    }
}

void ChatHandler::talkRaw(const std::string &mes) const
{
    MessageOut outMsg(CMSG_CHAT_MESSAGE);
    outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4), "len");
    outMsg.writeString(mes, static_cast<int>(mes.length()), "message");
}

void ChatHandler::privateMessage(const std::string &restrict recipient,
                                 const std::string &restrict text)
{
    MessageOut outMsg(CMSG_CHAT_WHISPER);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 28), "len");
    outMsg.writeString(recipient, 24, "recipient nick");
    outMsg.writeString(text, static_cast<int>(text.length()), "message");
    mSentWhispers.push(recipient);
}

void ChatHandler::who() const
{
    MessageOut outMsg(CMSG_WHO_REQUEST);
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
    if (!Net::getServerFeatures()->haveServerIgnore())
        return;
    MessageOut outMsg(CMSG_IGNORE_ALL);
    outMsg.writeInt8(0, "flag");
}

void ChatHandler::unIgnoreAll() const
{
    if (!Net::getServerFeatures()->haveServerIgnore())
        return;
    MessageOut outMsg(CMSG_IGNORE_ALL);
    outMsg.writeInt8(1, "flag");
}

void ChatHandler::ignore(const std::string &nick) const
{
    MessageOut outMsg(CMSG_IGNORE_NICK);
    outMsg.writeString(nick, 24, "nick");
    outMsg.writeInt8(0, "flag");
}

void ChatHandler::unIgnore(const std::string &nick) const
{
    MessageOut outMsg(CMSG_IGNORE_NICK);
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
    int chatMsgLength = msg.readInt16("len") - 4;
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
    int chatMsgLength = msg.readInt16("len") - 4 - 3;
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

void ChatHandler::processWhisper(Net::MessageIn &msg) const
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

}  // namespace TmwAthena
