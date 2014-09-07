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

#include "net/eathena/chathandler.h"

#include "being/localplayer.h"

#include "gui/chatconsts.h"

#include "gui/widgets/tabs/chattab.h"

#include "gui/windows/chatwindow.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "utils/stringutils.h"

#include <string>

#include "debug.h"

extern Net::ChatHandler *chatHandler;

namespace EAthena
{

ChatHandler::ChatHandler() :
    MessageHandler(),
    Ea::ChatHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_BEING_CHAT,
        SMSG_PLAYER_CHAT,
        SMSG_WHISPER,
        SMSG_WHISPER_RESPONSE,
        SMSG_GM_CHAT,
        SMSG_MVP,  // MVP
        SMSG_IGNORE_ALL_RESPONSE,
        SMSG_COLOR_MESSAGE,
        0
    };
    handledMessages = _messages;
    chatHandler = this;
}

void ChatHandler::handleMessage(Net::MessageIn &msg)
{
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

        case SMSG_PLAYER_CHAT:
        case SMSG_GM_CHAT:
        case SMSG_COLOR_MESSAGE:
            processChat(msg);
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
}

void ChatHandler::talk(const std::string &restrict text,
                       const std::string &restrict channel A_UNUSED) const
{
    if (!localPlayer)
        return;

    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);

    MessageOut outMsg(CMSG_CHAT_MESSAGE);
    // Added + 1 in order to let eAthena parse admin commands correctly
    outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4 + 1), "len");
    outMsg.writeString(mes, static_cast<int>(mes.length() + 1), "message");
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
        outMsg = new MessageOut(static_cast<int16_t>(atoi(str.c_str())));
        line = line.substr(pos + 1);
        pos = line.find(" ");
    }
    else
    {
        outMsg = new MessageOut(static_cast<int16_t>(atoi(line.c_str())));
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
    size_t pos = line.find(":");
    if (pos == std::string::npos)
    {
        const int i = atoi(line.c_str());
        if (line.length() <= 3)
            outMsg.writeInt8(static_cast<unsigned char>(i), "raw");
        else if (line.length() <= 5)
            outMsg.writeInt16(static_cast<int16_t>(i), "raw");
        else
            outMsg.writeInt32(i, "raw");
    }
    else
    {
        const std::string header = line.substr(0, pos);
        std::string data = line.substr(pos + 1);
        if (header.length() != 1)
            return;

        int i = 0;

        switch (header[0])
        {
            case '1':
            case '2':
            case '4':
                i = atoi(data.c_str());
                break;
            default:
                break;
        }

        switch (header[0])
        {
            case '1':
                outMsg.writeInt8(static_cast<unsigned char>(i), "raw");
                break;
            case '2':
                outMsg.writeInt16(static_cast<int16_t>(i), "raw");
                break;
            case '4':
                outMsg.writeInt32(i, "raw");
                break;
            case 'c':
            {
                pos = line.find(",");
                if (pos != std::string::npos)
                {
                    const uint16_t x = static_cast<uint16_t>(
                        atoi(data.substr(0, pos).c_str()));
                    data = data.substr(pos + 1);
                    pos = line.find(",");
                    if (pos == std::string::npos)
                        break;

                    const uint16_t y = static_cast<uint16_t>(
                        atoi(data.substr(0, pos).c_str()));
                    const int dir = atoi(data.substr(pos + 1).c_str());
                    outMsg.writeCoordinates(x, y,
                        static_cast<unsigned char>(dir), "raw");
                }
                break;
            }
            case 't':
                outMsg.writeString(data,
                    static_cast<int>(data.length()), "raw");
                break;
            default:
                break;
        }
    }
}

void ChatHandler::ignoreAll() const
{
    MessageOut outMsg(CMSG_IGNORE_ALL);
    outMsg.writeInt8(0, "flag");
}

void ChatHandler::unIgnoreAll() const
{
    MessageOut outMsg(CMSG_IGNORE_ALL);
    outMsg.writeInt8(1, "flag");
}

void ChatHandler::processChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processChat")
    const bool normalChat = msg.getId() == SMSG_PLAYER_CHAT;
    const bool coloredChat = msg.getId() == SMSG_COLOR_MESSAGE;
    int chatMsgLength = msg.readInt16("len") - 4;
    if (coloredChat)
    {
        msg.readInt32("unused");
        msg.readInt32("chat color");
        chatMsgLength -= 8;
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
            allow = chatWindow->resortChatLog(chatMsg,
                ChatMsgType::BY_PLAYER,
                GENERAL_CHANNEL,
                false, true);
        }

        if (pos != std::string::npos)
            chatMsg.erase(0, pos + 3);

        trim(chatMsg);

        if (localPlayer)
        {
            if ((chatWindow || mShowMotd) && allow)
                localPlayer->setSpeech(chatMsg, GENERAL_CHANNEL);
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

}  // namespace EAthena
