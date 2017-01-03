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

#include "net/eathena/chatrecv.h"

#include "actormanager.h"
#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"
#include "being/playerrelation.h"
#include "being/playerrelations.h"

#include "const/gui/chat.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/chatwindow.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "net/messagein.h"

#include "net/ea/chatrecv.h"

#include "net/eathena/mercenaryrecv.h"

#include "resources/chatobject.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace EAthena
{

namespace ChatRecv
{
    std::string mChatRoom;
}

void ChatRecv::processIgnoreNickAck(Net::MessageIn &msg)
{
    const int type = msg.readUInt8("type");
    const int flag = msg.readUInt8("flag");
    switch (type)
    {
        case 0:
            switch (flag)
            {
                case 0:
                    NotifyManager::notify(NotifyTypes::IGNORE_PLAYER_SUCCESS);
                    break;
                case 1:
                    NotifyManager::notify(NotifyTypes::IGNORE_PLAYER_FAILURE);
                    break;
                case 2:
                    NotifyManager::notify(NotifyTypes::IGNORE_PLAYER_TOO_MANY);
                    break;
                default:
                    NotifyManager::notify(NotifyTypes::IGNORE_PLAYER_UNKNOWN);
                    break;
            }
            break;
        case 1:
            switch (flag)
            {
                case 0:
                    NotifyManager::notify(
                        NotifyTypes::UNIGNORE_PLAYER_SUCCESS);
                    break;
                case 1:
                    NotifyManager::notify(
                        NotifyTypes::UNIGNORE_PLAYER_FAILURE);
                    break;
                default:
                    NotifyManager::notify(
                        NotifyTypes::UNIGNORE_PLAYER_UNKNOWN);
                    break;
            }
            break;

        default:
            NotifyManager::notify(NotifyTypes::IGNORE_PLAYER_TYPE_UNKNOWN);
            break;
    }
}

void ChatRecv::processChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processChat")
    const int chatMsgLength = msg.readInt16("len") - 4;
    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatRecv::processChat")
        return;
    }

    processChatContinue(msg.readRawString(chatMsgLength, "message"),
        ChatMsgType::BY_PLAYER);
}

void ChatRecv::processFormatMessage(Net::MessageIn &msg)
{
    const int msgId = msg.readInt16("msg id");
    // +++ here need load message from configuration file
    std::string chatMsg;
    if (msgId >= 1266 && msgId <= 1269)
    {
        MercenaryRecv::handleMercenaryMessage(msgId - 1266);
        return;
    }
    switch (msgId)
    {
        case 1334:
            // TRANSLATORS: error message
            chatMsg = _("Can't cast skill in this area.");
            break;
        case 1335:
            // TRANSLATORS: error message
            chatMsg = _("Can't use item in this area.");
            break;
        case 1773:
            // TRANSLATORS: error message
            chatMsg = _("Can't equip. Wrong level.");
            break;
        case 1774:
            // TRANSLATORS: error message
            chatMsg = _("Can't use. Wrong level.");
            break;
        case 1923:
            // TRANSLATORS: error message
            chatMsg = _("Work in progress.");  // busy with npc
            break;
        default:
            chatMsg = strprintf("Message #%d", msgId);
            break;
    }
    processChatContinue(chatMsg, ChatMsgType::BY_SERVER);
}

void ChatRecv::processFormatMessageNumber(Net::MessageIn &msg)
{
    const int msgId = msg.readInt16("msg id");
    const int value = msg.readInt32("value");
    if (msgId == 1862)
    {
        NotifyManager::notify(NotifyTypes::USE_ITEM_WAIT, value);
        return;
    }
    // +++ here need load message from configuration file
    const std::string chatMsg = strprintf(
        "Message #%d, value: %d", msgId, value);
    processChatContinue(chatMsg, ChatMsgType::BY_SERVER);
}

void ChatRecv::processFormatMessageSkill(Net::MessageIn &msg)
{
    const int skillId = msg.readInt16("skill id");
    const int msgId = msg.readInt32("msg id");
    // +++ here need load message from configuration file
    const std::string chatMsg = strprintf(
        "Message #%d, skill: %d", msgId, skillId);
    processChatContinue(chatMsg, ChatMsgType::BY_SERVER);
}

void ChatRecv::processColorChat(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processChat")
    int chatMsgLength = msg.readInt16("len") - 4;
    msg.readInt32("unused");
    msg.readInt32("chat color");
    chatMsgLength -= 8;
    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatRecv::processChat")
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
        const std::string nick = Ea::ChatRecv::getLastWhisperNick();
        if (nick.size() > 1 && nick[0] == '#')
        {
            if (message == strprintf("[ %s ] %s : \302\202\302",
                nick.c_str(), localPlayer->getName().c_str()))
            {
                Ea::ChatRecv::mSentWhispers.pop();
            }
        }
    }
    processChatContinue(message, ChatMsgType::BY_UNKNOWN);
}

std::string ChatRecv::extractChannelFromMessage(std::string &chatMsg)
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

void ChatRecv::processChatContinue(std::string chatMsg,
                                   const ChatMsgTypeT own)
{
    const std::string channel = extractChannelFromMessage(chatMsg);
    bool allow(true);
    if (chatWindow)
    {
        allow = chatWindow->resortChatLog(chatMsg,
            own,
            channel,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }

    const size_t pos = chatMsg.find(" : ", 0);
    if (pos != std::string::npos)
        chatMsg.erase(0, pos + 3);

    trim(chatMsg);

    if (localPlayer)
    {
        if ((chatWindow || Ea::ChatRecv::mShowMotd) && allow)
            localPlayer->setSpeech(chatMsg, GENERAL_CHANNEL);
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
    // remove non persistend "colors" from server.
    if (!findCutFirst(chatMsg, "ssss"))
        findCutFirst(chatMsg, "eulb");

    if (chatWindow)
        chatWindow->addGlobalMessage(chatMsg);
    BLOCK_END("ChatRecv::processChat")
}

void ChatRecv::processGmChat2(Net::MessageIn &msg)
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

void ChatRecv::processWhisper(Net::MessageIn &msg)
{
    BLOCK_START("ChatRecv::processWhisper")
    int packetLen = 28;
    if (msg.getVersion() >= 20091104)
        packetLen += 4;
    const int chatMsgLength = msg.readInt16("len") - packetLen;
    std::string nick = msg.readString(24, "nick");
    if (msg.getVersion() >= 20091104)
        msg.readInt32("admin flag");

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
    if (msg.getVersion() >= 20131223)
        msg.readInt32("unknown");
    if (type == 1 && chatWindow)
    {
        const std::string nick = Ea::ChatRecv::getLastWhisperNick();
        if (nick.size() > 1 && nick[0] == '#')
        {
            chatWindow->channelChatLog(nick,
                // TRANSLATORS: chat message
                strprintf(_("Message could not be sent, channel "
                "%s is not exists."), nick.c_str()),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_false);
            if (!Ea::ChatRecv::mSentWhispers.empty())
                Ea::ChatRecv::mSentWhispers.pop();
            return;
        }
    }
    Ea::ChatRecv::processWhisperResponseContinue(msg, type);
}

void ChatRecv::processChatIgnoreList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    // +++ need put it in some object or window
    const int count = (msg.readInt16("len") - 4) / 24;
    for (int f = 0; f < count; f ++)
        msg.readString(24, "nick");
}

void ChatRecv::processChatDisplay(Net::MessageIn &msg)
{
    const int len = msg.readInt16("len") - 17;
    ChatObject *const obj = new ChatObject;
    obj->ownerId = msg.readBeingId("owner account id");
    obj->chatId = msg.readInt32("chat id");
    obj->maxUsers = msg.readInt16("max users");
    obj->currentUsers = msg.readInt16("current users");
    obj->type = msg.readUInt8("type");
    obj->title = msg.readString(len, "title");
    obj->update();

    Being *const dstBeing = actorManager->findBeing(obj->ownerId);
    if (dstBeing)
        dstBeing->setChat(obj);
}

void ChatRecv::processChatRoomJoinAck(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 8) / 28;
    const int id = msg.readInt32("chat id");

    // +++ ignore chat members for now
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("role");
        msg.readString(24, "name");
    }

    const ChatObject *const oldChat = ChatObject::findById(id);

    if (oldChat)
        PlayerInfo::setRoomName(oldChat->title);
    else
        PlayerInfo::setRoomName(std::string());
    chatWindow->joinRoom(true);
    ChatObject *const obj = new ChatObject;
    if (oldChat)
    {
        obj->ownerId = oldChat->ownerId;
        obj->chatId = oldChat->chatId;
        obj->maxUsers = oldChat->maxUsers;
        obj->currentUsers = oldChat->currentUsers;
        obj->type = oldChat->type;
        obj->title = oldChat->title;
//        obj->update();
    }
    localPlayer->setChat(obj);
}

void ChatRecv::processChatRoomLeave(Net::MessageIn &msg)
{
    msg.readInt16("users");
    const std::string name = msg.readString(24, "name");
    const int status = msg.readUInt8("flag");  // 0 - left, 1 - kicked
    switch (status)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::ROOM_LEAVE, name);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::ROOM_KICKED, name);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(status);
            break;
    }
    if (localPlayer && name == localPlayer->getName())
    {
        if (chatWindow)
            chatWindow->joinRoom(false);
        PlayerInfo::setRoomName(std::string());
        if (localPlayer)
            localPlayer->setChat(nullptr);
    }
    else
    {
        Being *const being = actorManager->findBeingByName(
            name, ActorType::Player);
        if (being)
            being->setChat(nullptr);
    }
}

void ChatRecv::processJoinChannel(Net::MessageIn &msg)
{
    if (!chatWindow)
        return;

    const std::string channel = msg.readString(24, "channel name");
    const int flag = msg.readUInt8("flag");

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
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_false);
            break;

        case 1:
        case 2:
            chatWindow->addChannelTab(std::string("#").append(
                channel.substr(1)), false);
            break;
    }
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
        if (player_relations.hasPermission(nick, PlayerRelation::WHISPER))
            chatWindow->addWhisper(nick, chatMsg);
    }
    else if (localChatTab)
    {
        localChatTab->chatLog(chatMsg, ChatMsgType::BY_SERVER);
    }
    BLOCK_END("ChatRecv::processWhisper")
}

void ChatRecv::processBeingChat(Net::MessageIn &msg)
{
    if (!actorManager)
        return;

    BLOCK_START("ChatRecv::processBeingChat")
    const int chatMsgLength = msg.readInt16("len") - 8;
    Being *const being = actorManager->findBeing(msg.readBeingId("being id"));

    if (chatMsgLength <= 0)
    {
        BLOCK_END("ChatRecv::processBeingChat")
        return;
    }

    std::string chatMsg = msg.readRawString(chatMsgLength, "message");

    if (being && being->getType() == ActorType::Player)
        being->setTalkTime();

    const size_t pos = chatMsg.find(" : ", 0);
    std::string sender_name = ((pos == std::string::npos)
        ? "" : chatMsg.substr(0, pos));

    if (being && sender_name != being->getName()
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
            ChatMsgType::BY_OTHER,
            GENERAL_CHANNEL,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }

    if (allow && being && player_relations.hasPermission(sender_name,
        PlayerRelation::SPEECH_FLOAT))
    {
        being->setSpeech(chatMsg, GENERAL_CHANNEL);
    }
    BLOCK_END("ChatRecv::processBeingChat")
}

void ChatRecv::processChatRoomCreateAck(Net::MessageIn &msg)
{
    const int result = msg.readUInt8("flag");
    switch (result)
    {
        case 0:
        {
            PlayerInfo::setRoomName(mChatRoom);
            chatWindow->joinRoom(true);
            ChatObject *const obj = new ChatObject;
            obj->ownerId = localPlayer->getId();
            obj->chatId = 0;
            obj->maxUsers = 1000;
            obj->currentUsers = 1;
            obj->type = 1;
            obj->title = mChatRoom;
            obj->update();
            localPlayer->setChat(obj);
            break;
        }
        case 1:
            NotifyManager::notify(NotifyTypes::ROOM_LIMIT_EXCEEDED);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::ROOM_ALREADY_EXISTS);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(result);
            break;
    }
    mChatRoom.clear();
}

void ChatRecv::processChatRoomDestroy(Net::MessageIn &msg)
{
    const int chatId = msg.readInt32("chat id");
    actorManager->removeRoom(chatId);
}

void ChatRecv::processChatRoomJoinFailed(Net::MessageIn &msg)
{
    const int result = msg.readUInt8("flag");
    switch (result)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::ROOM_ERROR_FULL);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::ROOM_ERROR_WRONG_PASSWORD);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::ROOM_ERROR_KICKED);
            break;
        case 3:
            break;
        case 4:
            NotifyManager::notify(NotifyTypes::ROOM_ERROR_ZENY);
            break;
        case 5:
            NotifyManager::notify(NotifyTypes::ROOM_ERROR_LOW_LEVEL);
            break;
        case 6:
            NotifyManager::notify(NotifyTypes::ROOM_ERROR_HIGH_LEVEL);
            break;
        case 7:
            NotifyManager::notify(NotifyTypes::ROOM_ERROR_RACE);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(result);
    }
}

void ChatRecv::processChatRoomAddMember(Net::MessageIn &msg)
{
    msg.readInt16("users");
    const std::string name = msg.readString(24, "name");
    if (!localChatTab)
        return;
    NotifyManager::notify(NotifyTypes::ROOM_JOINED, name);
}

void ChatRecv::processChatRoomSettings(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 17;
    const BeingId ownerId = msg.readBeingId("owner id");
    const int chatId = msg.readInt32("chat id");
    const uint16_t limit = msg.readInt16("limit");
    msg.readInt16("users");
    const uint8_t type = msg.readUInt8("type");
    const std::string &title = msg.readString(sz, "title");
    ChatObject *const chat = localPlayer->getChat();
    if (chat && chat->chatId == chatId)
    {
        chat->ownerId = ownerId;
        chat->maxUsers = limit;
        chat->type = type;
        if (chat->title != title)
        {
            chat->title = title;
            actorManager->updateRoom(chat);
            chatWindow->joinRoom(true);
        }
    }
}

void ChatRecv::processChatRoomRoleChange(Net::MessageIn &msg)
{
    const int role = msg.readInt32("role");
    const std::string name = msg.readString(24, "name");
    switch (role)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::ROOM_ROLE_OWNER, name);
            break;
        case 1:
            // dont show normal role
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(role);
            break;
    }
}

void ChatRecv::processMVPItem(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("item id");
}

void ChatRecv::processMVPExp(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("exo");
}

void ChatRecv::processMVPNoItem(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
}

void ChatRecv::processMannerMessage(Net::MessageIn &msg)
{
    const int result = msg.readInt32("type");
    switch (result)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::MANNER_CHANGED);
            break;
        case 5:
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(result);
            break;
    }
}

void ChatRecv::processChatSilence(Net::MessageIn &msg)
{
    const int result = msg.readUInt8("type");
    const std::string name = msg.readString(24, "gm name");

    switch (result)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::MANNER_POSITIVE_POINTS, name);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::MANNER_NEGATIVE_POINTS, name);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(result);
            break;
    }
}

void ChatRecv::processChatTalkieBox(Net::MessageIn &msg)
{
    msg.readBeingId("being id");
    const std::string message = msg.readString(80, "message");
    localChatTab->chatLog(message, ChatMsgType::BY_SERVER);
}

void ChatRecv::processBattleChatMessage(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int sz = msg.readInt16("len") - 24 - 8;
    msg.readBeingId("account id");
    msg.readString(24, "nick");
    msg.readString(sz, "message");
}

void ChatRecv::processScriptMessage(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 8;
    msg.readBeingId("being id");
    const std::string message = msg.readString(sz, "message");
    localChatTab->chatLog(message, ChatMsgType::BY_SERVER);
}

}  // namespace EAthena
