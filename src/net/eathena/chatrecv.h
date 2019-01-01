/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef NET_EATHENA_CHATRECV_H
#define NET_EATHENA_CHATRECV_H

#include "enums/gui/chatmsgtype.h"

#include <string>

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace ChatRecv
    {
        std::string extractChannelFromMessage(std::string &chatMsg);
        void processChat(Net::MessageIn &msg);
        void processColorChat(Net::MessageIn &msg);
        void processChatContinue(std::string chatMsg,
                                 const ChatMsgTypeT own);
        void processWhisper(Net::MessageIn &msg);
        void processWhisperResponse(Net::MessageIn &msg);
        void processGmChat(Net::MessageIn &msg);
        void processGmChat2(Net::MessageIn &msg);
        void processChatIgnoreList(Net::MessageIn &msg);
        void processFormatMessage(Net::MessageIn &msg);
        void processFormatMessageNumber(Net::MessageIn &msg);
        void processFormatColor(Net::MessageIn &msg);
        void processFormatMessageString(Net::MessageIn &msg);
        void processFormatMessageStringColor(Net::MessageIn &msg);
        void processFormatMessageSkill(Net::MessageIn &msg);
        void processChatDisplay(Net::MessageIn &msg);
        void processChatRoomJoinAck(Net::MessageIn &msg);
        void processChatRoomLeave(Net::MessageIn &msg);
        void processJoinChannel(Net::MessageIn &msg);
        void processWhisperContinue(const std::string &nick,
                                    std::string chatMsg);
        void processBeingChat(Net::MessageIn &msg);
        void processIgnoreNickAck(Net::MessageIn &msg);
        void processChatRoomCreateAck(Net::MessageIn &msg);
        void processChatRoomDestroy(Net::MessageIn &msg);
        void processChatRoomJoinFailed(Net::MessageIn &msg);
        void processChatRoomAddMember(Net::MessageIn &msg);
        void processChatRoomSettings(Net::MessageIn &msg);
        void processChatRoomRoleChange(Net::MessageIn &msg);
        void processMVPItem(Net::MessageIn &msg);
        void processMVPExp(Net::MessageIn &msg);
        void processMVPNoItem(Net::MessageIn &msg);
        void processMannerMessage(Net::MessageIn &msg);
        void processChatSilence(Net::MessageIn &msg);
        void processChatTalkieBox(Net::MessageIn &msg);
        void processBattleChatMessage(Net::MessageIn &msg);
        void processScriptMessage(Net::MessageIn &msg);
        extern std::string mChatRoom;
    }  // namespace ChatRecv
}  // namespace EAthena

#endif  // NET_EATHENA_CHATRECV_H
