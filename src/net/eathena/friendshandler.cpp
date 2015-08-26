/*
 *  The ManaPlus Client
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

#include "net/eathena/friendshandler.h"

#include "logger.h"

#include "net/eathena/friendsrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::FriendsHandler *friendsHandler;

namespace EAthena
{

FriendsHandler::FriendsHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_FRIENDS_PLAYER_ONLINE,
        SMSG_FRIENDS_LIST,
        SMSG_FRIENDS_REQUEST_ACK,
        SMSG_FRIENDS_REQUEST,
        SMSG_FRIENDS_DELETE_PLAYER,
        0
    };
    handledMessages = _messages;
    friendsHandler = this;
}

void FriendsHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_FRIENDS_PLAYER_ONLINE:
            FriendsRecv::processPlayerOnline(msg);
            break;

        case SMSG_FRIENDS_LIST:
            FriendsRecv::processFriendsList(msg);
            break;

        case SMSG_FRIENDS_REQUEST_ACK:
            FriendsRecv::processRequestAck(msg);
            break;

        case SMSG_FRIENDS_REQUEST:
            FriendsRecv::processRequest(msg);
            break;

        case SMSG_FRIENDS_DELETE_PLAYER:
            FriendsRecv::processDeletePlayer(msg);
            break;

        default:
            break;
    }
}

void FriendsHandler::invite(const std::string &name) const
{
    createOutPacket(CMSG_FRIENDS_ADD_PLAYER);
    outMsg.writeString(name, 24, "name");
}

void FriendsHandler::inviteResponse(const int accountId,
                                    const int charId,
                                    const bool accept) const
{
    createOutPacket(CMSG_FRIENDS_REQUEST_ACK);
    outMsg.writeInt32(accountId, "account id");
    outMsg.writeInt32(charId, "char id");
    outMsg.writeInt32(accept ? 1 : 0, "result");
}

void FriendsHandler::remove(const int accountId, const int charId) const
{
    createOutPacket(CMSG_FRIENDS_DELETE_PLAYER);
    outMsg.writeInt32(accountId, "account id");
    outMsg.writeInt32(charId, "char id");
}

}  // namespace EAthena
