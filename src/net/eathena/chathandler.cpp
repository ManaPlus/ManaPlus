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

#include "net/eathena/chathandler.h"

#include "being/localplayer.h"

#include "net/serverfeatures.h"

#include "net/ea/chatrecv.h"

#include "net/eathena/chatrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "resources/chatobject.h"

#include "utils/stringutils.h"

#include "debug.h"

extern Net::ChatHandler *chatHandler;
extern int packetVersion;

namespace EAthena
{

ChatHandler::ChatHandler() :
    Ea::ChatHandler()
{
    chatHandler = this;
}

void ChatHandler::talk(const std::string &restrict text,
                       const std::string &restrict channel A_UNUSED) const
{
    if (!localPlayer)
        return;

    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);

    createOutPacket(CMSG_CHAT_MESSAGE);
    if (packetVersion >= 20151001)
    {
        outMsg.writeInt16(CAST_S16(mes.length() + 4), "len");
        outMsg.writeString(mes, CAST_S32(mes.length()), "message");
    }
    else
    {
        // Added + 1 in order to let eAthena parse admin commands correctly
        outMsg.writeInt16(CAST_S16(mes.length() + 4 + 1), "len");
        outMsg.writeString(mes, CAST_S32(mes.length() + 1), "message");
    }
}

void ChatHandler::talkRaw(const std::string &mes) const
{
    createOutPacket(CMSG_CHAT_MESSAGE);
    outMsg.writeInt16(CAST_S16(mes.length() + 4), "len");
    outMsg.writeString(mes, CAST_S32(mes.length()), "message");
}

void ChatHandler::privateMessage(const std::string &restrict recipient,
                                 const std::string &restrict text) const
{
    createOutPacket(CMSG_CHAT_WHISPER);
    if (packetVersion >= 20151001)
    {
        outMsg.writeInt16(CAST_S16(text.length() + 28), "len");
        outMsg.writeString(recipient, 24, "recipient nick");
        outMsg.writeString(text, CAST_S32(text.length()), "message");
    }
    else
    {
        outMsg.writeInt16(CAST_S16(text.length() + 28 + 1), "len");
        outMsg.writeString(recipient, 24, "recipient nick");
        outMsg.writeString(text, CAST_S32(text.length()), "message");
        outMsg.writeInt8(0, "null char");
    }
    Ea::ChatRecv::mSentWhispers.push(recipient);
}

void ChatHandler::channelMessage(const std::string &restrict channel,
                                 const std::string &restrict text) const
{
    privateMessage(channel, text);
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

    if (line.empty())
        return;

    size_t pos = line.find(' ');
    if (pos != std::string::npos)
    {
        str = line.substr(0, pos);

        const int16_t id = CAST_S16(parseNumber(str));
        outMsg = new MessageOut(id);
        outMsg->writeInt16(id, "packet id");
        line = line.substr(pos + 1);
        pos = line.find(' ');
    }
    else
    {
        const int16_t id = CAST_S16(parseNumber(line));
        outMsg = new MessageOut(id);
        outMsg->writeInt16(id, "packet id");
        delete outMsg;
        return;
    }

    while (pos != std::string::npos)
    {
        str = line.substr(0, pos);
        processRaw(*outMsg, str);
        line = line.substr(pos + 1);
        pos = line.find(' ');
    }
    if (!line.empty())
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
            outMsg.writeInt8(CAST_U8(i), "raw");
            break;
        }
        case 'w':
        {
            outMsg.writeInt16(CAST_S16(i), "raw");
            break;
        }
        case 'l':
        {
            outMsg.writeInt32(CAST_S32(i), "raw");
            break;
        }
        default:
            break;
    }
}

void ChatHandler::ignoreAll() const
{
    createOutPacket(CMSG_IGNORE_ALL);
    outMsg.writeInt8(0, "flag");
}

void ChatHandler::unIgnoreAll() const
{
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
    createOutPacket(CMSG_REQUEST_IGNORE_LIST);
}

void ChatHandler::createChatRoom(const std::string &title,
                                 const std::string &password,
                                 const int limit,
                                 const bool isPublic) const
{
    createOutPacket(CMSG_CREAYE_CHAT_ROOM);
    outMsg.writeInt16(CAST_S16(
        7 + 8 + 36), "len");
    outMsg.writeInt16(CAST_S16(limit), "limit");
    outMsg.writeInt8(CAST_S8(isPublic ? 1 : 0), "public");
    outMsg.writeString(password, 8, "password");
    outMsg.writeString(title, 36, "title");
    ChatRecv::mChatRoom = title;
}

void ChatHandler::battleTalk(const std::string &text) const
{
    if (!localPlayer)
        return;

    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);

    createOutPacket(CMSG_BATTLE_CHAT_MESSAGE);
    if (packetVersion >= 20151001)
    {
        outMsg.writeInt16(CAST_S16(mes.length() + 4), "len");
        outMsg.writeString(mes, CAST_S32(mes.length()), "message");
    }
    else
    {
        // Added + 1 in order to let eAthena parse admin commands correctly
        outMsg.writeInt16(CAST_S16(mes.length() + 4 + 1), "len");
        outMsg.writeString(mes, CAST_S32(mes.length() + 1), "message");
    }
}

void ChatHandler::joinChat(const ChatObject *const chat,
                           const std::string &password) const
{
    if (!chat)
        return;

    createOutPacket(CMSG_CHAT_ROOM_JOIN);
    outMsg.writeInt32(chat->chatId, "chat id");
    outMsg.writeString(password, 8, "password");
}

void ChatHandler::joinChannel(const std::string &channel) const
{
    if (serverFeatures->haveJoinChannel())
    {
        createOutPacket(CMSG_CHAT_JOIN_CHANNEL);
        outMsg.writeString(channel, 24, "channel name");
    }
    else
    {
        channelMessage(channel, "\302\202\302");
    }
}

void ChatHandler::partChannel(const std::string &channel) const
{
    if (serverFeatures->haveJoinChannel())
    {
        createOutPacket(CMSG_CHAT_PART_CHANNEL);
        outMsg.writeString(channel, 24, "channel name");
    }
}

void ChatHandler::talkPet(const std::string &restrict text,
                          const std::string &restrict channel A_UNUSED) const
{
    if (text.empty())
        return;
    std::string msg = text;
    if (msg.size() > 500)
        msg = msg.substr(0, 500);
    const size_t sz = msg.size();

    createOutPacket(CMSG_PET_TALK);
    outMsg.writeInt16(CAST_S16(sz + 4 + 1), "len");
    outMsg.writeString(msg, CAST_S32(sz), "message");
    outMsg.writeInt8(0, "zero byte");
}

void ChatHandler::leaveChatRoom() const
{
    createOutPacket(CMSG_LEAVE_CHAT_ROOM);
}

void ChatHandler::setChatRoomOptions(const int limit,
                                     const bool isPublic,
                                     const std::string &password,
                                     const std::string &title) const
{
    createOutPacket(CMSG_SET_CHAT_ROOM_OPTIONS);
    const int sz = CAST_S32(title.size());
    outMsg.writeInt16(CAST_S16(15 + sz), "len");
    outMsg.writeInt16(CAST_S16(limit), "limit");
    outMsg.writeInt8(CAST_S8(isPublic ? 1 : 0), "type");
    outMsg.writeString(password, 8, "password");
    outMsg.writeString(title, sz, "title");
}

void ChatHandler::setChatRoomOwner(const std::string &nick) const
{
    createOutPacket(CMSG_SET_CHAT_ROOM_OWNER);
    outMsg.writeInt32(0, "role (unused)");
    outMsg.writeString(nick, 24, "nick");
}

void ChatHandler::kickFromChatRoom(const std::string &nick) const
{
    createOutPacket(CMSG_KICK_FROM_CHAT_ROOM);
    outMsg.writeString(nick, 24, "nick");
}

}  // namespace EAthena
