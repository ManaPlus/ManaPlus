/*
 *  The ManaPlus Client
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

#include "net/eathena/friendshandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern Net::FriendsHandler *friendsHandler;

namespace EAthena
{

FriendsHandler::FriendsHandler()
{
    friendsHandler = this;
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
