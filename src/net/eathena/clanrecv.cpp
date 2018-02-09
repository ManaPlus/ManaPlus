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

#include "being/claninfo.h"

#include "net/messagein.h"

#include "logger.h"

#include "debug.h"

namespace EAthena
{

void ClanRecv::processClanInfo(Net::MessageIn &msg)
{
    msg.readInt16("len");
    clanInfo.id = msg.readInt32("clan id");
    clanInfo.name = msg.readString(24, "clan name");
    clanInfo.masterName = msg.readString(24, "master name");
    clanInfo.mapName = msg.readString(16, "map name");
    const int allyCount = msg.readUInt8("ally clans count");
    const int antagonistCount = msg.readUInt8("antagonist clans count");
    for (int f = 0; f < allyCount; f ++)
    {
        clanInfo.allyClans.push_back(
            msg.readString(24, "ally clan name"));
    }
    for (int f = 0; f < antagonistCount; f ++)
    {
        clanInfo.antagonistClans.push_back(
            msg.readString(24, "antagonist clan name"));
    }
}

void ClanRecv::processClanOnlineCount(Net::MessageIn &msg)
{
    clanInfo.onlineMembers = msg.readInt16("online members count");
    clanInfo.totalMembers = msg.readInt16("total members count");
}

void ClanRecv::processClanLeave(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
}

void ClanRecv::processClanChat(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int chatMsgLength = msg.readInt16("len") - 4 - 24;
    if (chatMsgLength <= 0)
        return;
    msg.readInt16("len");
    msg.readString(24, "player name");
    msg.readString(chatMsgLength, "message");
}

}  // namespace EAthena
