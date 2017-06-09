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

#include "net/tmwa/chathandler.h"

#include "being/localplayer.h"

#include "const/gui/chat.h"

#include "net/ea/chatrecv.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"

#include "utils/gmfunctions.h"
#include "utils/stringutils.h"

#include "debug.h"

extern unsigned int tmwServerVersion;

namespace TmwAthena
{

ChatHandler::ChatHandler() :
    Ea::ChatHandler()
{
    chatHandler = this;
}

void ChatHandler::talk(const std::string &restrict text,
                       const std::string &restrict channel A_UNUSED) const
{
    if (localPlayer == nullptr)
        return;

    if (tmwServerVersion >= 0x100408)
    {
        createOutPacket(CMSG_CHAT_MESSAGE);
        // Added + 1 in order to let eAthena parse admin commands correctly
        outMsg.writeInt16(CAST_S16(text.length() + 4 + 1), "len");
        outMsg.writeString(text, CAST_S32(text.length() + 1), "message");
    }
    else
    {
        const std::string mes = std::string(localPlayer->getName()).append(
            " : ").append(text);

        createOutPacket(CMSG_CHAT_MESSAGE);
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
    outMsg.writeInt16(CAST_S16(text.length() + 28), "len");
    outMsg.writeString(recipient, 24, "recipient nick");
    outMsg.writeString(text, CAST_S32(text.length()), "message");
    Ea::ChatRecv::mSentWhispers.push(recipient);
}

void ChatHandler::channelMessage(const std::string &restrict channel,
                                 const std::string &restrict text) const
{
    if (channel == TRADE_CHANNEL)
        talk("\302\202" + text, GENERAL_CHANNEL);
    else if (channel == GM_CHANNEL)
        Gm::runCommand("wgm", text);
}

void ChatHandler::who() const
{
    if (tmwServerVersion >= 0x0e0b0b)
        return;

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
}

void ChatHandler::unIgnoreAll() const
{
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
                                 const bool isPublic A_UNUSED) const
{
}

void ChatHandler::battleTalk(const std::string &text A_UNUSED) const
{
}

void ChatHandler::joinChat(const ChatObject *const chat A_UNUSED,
                           const std::string &password A_UNUSED) const
{
}

void ChatHandler::joinChannel(const std::string &channel A_UNUSED) const
{
}

void ChatHandler::partChannel(const std::string &channel A_UNUSED) const
{
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
