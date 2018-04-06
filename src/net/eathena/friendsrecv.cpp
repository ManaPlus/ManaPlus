/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "net/eathena/friendsrecv.h"

#include "logger.h"

#include "net/messagein.h"

#include "debug.h"

extern int packetVersionMain;
extern int packetVersionRe;

namespace EAthena
{

void FriendsRecv::processPlayerOnline(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readInt32("char id");
    msg.readUInt8("flag");  // 0 - online, 1 - offline
    if (packetVersionMain >= 20180307 || packetVersionRe >= 20180221)
        msg.readString(24, "player name");
}

void FriendsRecv::processFriendsList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int count = (msg.readInt16("size") - 4) / 32;
    for (int f = 0; f < count; f ++)
    {
        msg.readBeingId("account id");
        msg.readInt32("char id");
        msg.readString(24, "name");
    }
}

void FriendsRecv::processRequestAck(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("type");
    msg.readBeingId("account id");
    msg.readInt32("char id");
    msg.readString(24, "name");
}

void FriendsRecv::processRequest(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readInt32("char id");
    msg.readString(24, "name");
}

void FriendsRecv::processDeletePlayer(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readInt32("char id");
}

}  // namespace EAthena
