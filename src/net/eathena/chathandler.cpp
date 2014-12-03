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

#include "actormanager.h"

#include "being/localplayer.h"

#include "gui/chatconsts.h"

#include "gui/windows/chatwindow.h"

#include "net/mercenaryhandler.h"
#include "net/serverfeatures.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/chatobject.h"

#include "utils/gettext.h"
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
        SMSG_GM_CHAT2,
        SMSG_MVP,  // MVP
        SMSG_IGNORE_ALL_RESPONSE,
        SMSG_COLOR_MESSAGE,
        SMSG_CHAT_IGNORE_LIST,
        SMSG_FORMAT_MESSAGE,
        SMSG_FORMAT_MESSAGE_NUMBER,
        SMSG_FORMAT_MESSAGE_SKILL,
        SMSG_CHAT_DISPLAY,
        SMSG_CHAT_JOIN_ACK,
        SMSG_CHAT_LEAVE,
        SMSG_CHAT_JOIN_CHANNEL,
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
            processChat(msg);
            break;

        case SMSG_FORMAT_MESSAGE:
            processFormatMessage(msg);
            break;

        case SMSG_FORMAT_MESSAGE_NUMBER:
            processFormatMessageNumber(msg);
            break;

        case SMSG_FORMAT_MESSAGE_SKILL:
            processFormatMessageSkill(msg);
            break;

        case SMSG_COLOR_MESSAGE:
            processColorChat(msg);
            break;

        case SMSG_GM_CHAT:
            processGmChat(msg);
            break;

        case SMSG_GM_CHAT2:
            processGmChat2(msg);
            break;

        case SMSG_MVP:
            processMVP(msg);
            break;

        case SMSG_IGNORE_ALL_RESPONSE:
            processIgnoreAllResponse(msg);
            break;

        case SMSG_CHAT_IGNORE_LIST:
            processChatIgnoreList(msg);
            break;

        case SMSG_CHAT_DISPLAY:
            processChatDisplay(msg);
            break;

        case SMSG_CHAT_JOIN_ACK:
            processChatJoinAck(msg);
            break;

        case SMSG_CHAT_LEAVE:
            processChatLeave(msg);
            break;

        case SMSG_CHAT_JOIN_CHANNEL:
            processJoinChannel(msg);
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

    createOutPacket(CMSG_CHAT_MESSAGE);
    // Added + 1 in order to let eAthena parse admin commands correctly
    outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4 + 1), "len");
    outMsg.writeString(mes, static_cast<int>(mes.length() + 1), "message");
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
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 28 + 1), "len");
    outMsg.writeString(recipient, 24, "recipient nick");
    outMsg.writeString(text, static_cast<int>(text.length()), "message");
    outMsg.writeInt8(0, "null char");
    mSentWhispers.push(recipient);
}

void ChatHandler::channelMessage(const std::string &restrict channel,
                                 const std::string &restrict text)
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
                                 const bool isPublic)
{
    createOutPacket(CMSG_CREAYE_CHAT_ROOM);
    outMsg.writeInt16(static_cast<int16_t>(
        password.size() + title.size() + 5), "len");
    outMsg.writeInt16(static_cast<int16_t>(limit), "limit");
    outMsg.writeInt8(static_cast<int8_t>(isPublic ? 1 : 0), "public");
    outMsg.writeString(password, 8, "password");
    outMsg.writeString(title, 36, "title");
}

void ChatHandler::battleTalk(const std::string &text) const
{
    if (!localPlayer)
        return;

    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);

    createOutPacket(CMSG_BATTLE_CHAT_MESSAGE);
    // Added + 1 in order to let eAthena parse admin commands correctly
    outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4 + 1), "len");
    outMsg.writeString(mes, static_cast<int>(mes.length() + 1), "message");
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

    processChatContinue(msg.readRawString(chatMsgLength, "message"),
        ChatMsgType::BY_PLAYER);
}

void ChatHandler::processFormatMessage(Net::MessageIn &msg)
{
    const int msgId = msg.readInt16("msg id");
    // +++ here need load message from configuration file
    std::string chatMsg;
    if (msgId >= 1266 && msgId <= 1269)
    {
        mercenaryHandler->handleMercenaryMessage(msgId - 1266);
        return;
    }
    switch (msgId)
    {
        case 1334:
            chatMsg = _("Can't cast skill in this area.");
            break;
        case 1335:
            chatMsg = _("Can't use item in this area.");
            break;
        case 1773:
            chatMsg = _("Can't equip. Wrong level.");
            break;
        case 1774:
            chatMsg = _("Can't use. Wrong level.");
            break;
        case 1923:
            chatMsg = _("Work in progress.");  // busy with npc
            break;
        default:
            chatMsg = strprintf("Message #%d", msgId);
            break;
    }
    processChatContinue(chatMsg, ChatMsgType::BY_SERVER);
}

void ChatHandler::processFormatMessageNumber(Net::MessageIn &msg)
{
    int msgId = msg.readInt16("msg id");
    int value = msg.readInt32("value");
    // +++ here need load message from configuration file
    const std::string chatMsg = strprintf(
        "Message #%d, value: %d", msgId, value);
    processChatContinue(chatMsg, ChatMsgType::BY_SERVER);
}

void ChatHandler::processFormatMessageSkill(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");
    const int msgId = msg.readInt32("msg id");
    // +++ here need load message from configuration file
    const std::string chatMsg = strprintf(
        "Message #%d, skill: %d", msgId, skillId);
    processChatContinue(chatMsg, ChatMsgType::BY_SERVER);
}

void ChatHandler::processColorChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processChat")
    int chatMsgLength = msg.readInt16("len") - 4;
    msg.readInt32("unused");
    msg.readInt32("chat color");
    chatMsgLength -= 8;
    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatHandler::processChat")
        return;
    }

    std::string message = msg.readRawString(chatMsgLength, "message");
    std::string msg2 = message;
    if (findCutFirst(msg2, "You're now in the '#") && findCutLast(msg2, "'"))
    {
        const size_t idx = msg2.find("' channel for '");
        if (idx != std::string::npos && chatWindow)
        {
            chatWindow->addChannelTab(std::string("#").append(
                msg2.substr(0, idx)), false);
            return;
        }
    }
    else
    {
        const std::string nick = getLastWhisperNick();
        if (nick.size() > 1 && nick[0] == '#')
        {
            if (message == strprintf("[ %s ] %s : \302\202\302",
                nick.c_str(), localPlayer->getName().c_str()))
            {
                mSentWhispers.pop();
            }
        }
    }
    processChatContinue(message, ChatMsgType::BY_UNKNOWN);
}

std::string ChatHandler::extractChannelFromMessage(std::string &chatMsg)
{
    std::string msg = chatMsg;
    std::string channel(GENERAL_CHANNEL);
    if (findCutFirst(msg, "[ #"))
    {   // found channel message
        const size_t idx = msg.find(" ] ");
        if (idx != std::string::npos)
        {
            channel = std::string("#").append(msg.substr(0, idx));
            chatMsg = msg.substr(idx + 3);
        }
    }
    return channel;
}

void ChatHandler::processChatContinue(std::string chatMsg,
                                      ChatMsgType::Type own)
{
    const std::string channel = extractChannelFromMessage(chatMsg);
    bool allow(true);
    if (chatWindow)
    {
        allow = chatWindow->resortChatLog(chatMsg,
            own,
            channel,
            false, true);
    }

    const size_t pos = chatMsg.find(" : ", 0);
    if (pos != std::string::npos)
        chatMsg.erase(0, pos + 3);

    trim(chatMsg);

    if (localPlayer)
    {
        if ((chatWindow || mShowMotd) && allow)
            localPlayer->setSpeech(chatMsg, GENERAL_CHANNEL);
    }
    BLOCK_END("ChatHandler::processChat")
}

void ChatHandler::processGmChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processChat")
    int chatMsgLength = msg.readInt16("len") - 4;
    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatHandler::processChat")
        return;
    }

    std::string chatMsg = msg.readRawString(chatMsgLength, "message");
    // remove non persistend "colors" from server.
    if (!findCutFirst(chatMsg, "ssss"))
        findCutFirst(chatMsg, "eulb");

    if (chatWindow)
        chatWindow->addGlobalMessage(chatMsg);
    BLOCK_END("ChatHandler::processChat")
}

void ChatHandler::processGmChat2(Net::MessageIn &msg)
{
    const int chatMsgLength = msg.readInt16("len") - 16;
    msg.readInt32("font color");
    msg.readInt16("font type");
    msg.readInt16("font size");
    msg.readInt16("font align");
    msg.readInt16("font y");
    const std::string chatMsg = msg.readRawString(chatMsgLength, "message");
    if (chatWindow)
        chatWindow->addGlobalMessage(chatMsg);
}

void ChatHandler::processWhisper(Net::MessageIn &msg)
{
    BLOCK_START("ChatHandler::processWhisper")
    const int chatMsgLength = msg.readInt16("len") - 32;
    std::string nick = msg.readString(24, "nick");
    msg.readInt32("admin flag");

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
    msg.readInt32("unknown");
    if (type == 1 && chatWindow)
    {
        const std::string nick = getLastWhisperNick();
        if (nick.size() > 1 && nick[0] == '#')
        {
            chatWindow->channelChatLog(nick,
                // TRANSLATORS: chat message
                strprintf(_("Message could not be sent, channel "
                "%s is not exists."), nick.c_str()),
                ChatMsgType::BY_SERVER, false, false);
            if (!mSentWhispers.empty())
                mSentWhispers.pop();
            return;
        }
    }
    processWhisperResponseContinue(type);
}

void ChatHandler::processChatIgnoreList(Net::MessageIn &msg)
{
    // +++ need put it in some object or window
    const int count = (msg.readInt16("len") - 4) / 24;
    for (int f = 0; f < count; f ++)
        msg.readString(24, "nick");
}

void ChatHandler::processChatDisplay(Net::MessageIn &msg)
{
    const int len = msg.readInt16("len") - 17;
    ChatObject *const obj = new ChatObject;
    obj->ownerId = msg.readInt32("owner account id");
    obj->chatId = msg.readInt32("chat id");
    obj->maxUsers = msg.readInt16("max users");
    obj->currentUsers = msg.readInt16("current users");
    obj->type = msg.readUInt8("type");
    obj->title = msg.readString(len, "title");

    Being *const dstBeing = actorManager->findBeing(obj->ownerId);
    if (dstBeing)
        dstBeing->setChat(obj);
}

void ChatHandler::joinChat(const ChatObject *const chat,
                           const std::string &password) const
{
    if (!chat)
        return;

    createOutPacket(CMSG_CHAT_JOIN);
    outMsg.writeInt32(chat->chatId, "chat id");
    outMsg.writeString(password, 8, "password");
}

void ChatHandler::processChatJoinAck(Net::MessageIn &msg)
{
    const int count = msg.readInt16("len") - 8;
    msg.readInt32("chat id");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("role");
        msg.readString(24, "name");
    }
}

void ChatHandler::processChatLeave(Net::MessageIn &msg)
{
    msg.readInt16("users");
    msg.readString(24, "name");
    msg.readUInt8("flag");  // 0 - left, 1 - kicked
}

void ChatHandler::joinChannel(const std::string &channel)
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

void ChatHandler::processJoinChannel(Net::MessageIn &msg)
{
    if (!chatWindow)
        return;

    const std::string channel = msg.readString(24, "channel name");
    int flag = msg.readUInt8("flag");

    if (channel.size() < 2)
        return;
    switch (flag)
    {
        case 0:
        default:
            chatWindow->channelChatLog(channel,
                // TRANSLATORS: chat message
                strprintf(_("Can't open channel. Channel "
                "%s is not exists."), channel.c_str()),
                ChatMsgType::BY_SERVER, false, false);
            break;

        case 1:
        case 2:
            chatWindow->addChannelTab(std::string("#").append(
                channel.substr(1)), false);
            break;
    }
}

}  // namespace EAthena
