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

#include "net/eathena/clanrecv.h"

#include "net/messagein.h"

#include "logger.h"

#include "debug.h"

namespace EAthena
{

void ClanRecv::processClanInfo(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("len");
    msg.readInt32("clan id");
    msg.readString(24, "clan name");
    msg.readString(24, "master name");
    msg.readString(16, "map name");
    const int allyCount = msg.readUInt8("ally clans count");
    const int antagonistCount = msg.readUInt8("antagonist clans count");
    for (int f = 0; f < allyCount; f ++)
    {
        msg.readString(24, "ally clan name");
    }
    for (int f = 0; f < antagonistCount; f ++)
    {
        msg.readString(24, "antagonist clan name");
    }
}

void ClanRecv::processClanOnlineCount(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("online members count");
    msg.readInt16("total members count");
}

}  // namespace EAthena
