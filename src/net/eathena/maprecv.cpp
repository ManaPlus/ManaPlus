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

#include "net/eathena/maprecv.h"

#include "logger.h"

#include "net/messagein.h"

#include "debug.h"

namespace EAthena
{

void MapRecv::processInstanceStart(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readString(61, "instance name");
    msg.readInt16("flag");
}

void MapRecv::processInstanceCreate(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("flag");
}

void MapRecv::processInstanceInfo(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readString(61, "instance name");
    msg.readInt32("remaining time");
    msg.readInt32("no players close time");
}

void MapRecv::processInstanceDelete(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("flag");
    msg.readInt32("unused");
}

void MapRecv::processAddMapMarker(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readInt16("x");
    msg.readInt16("y");
}

}  // namespace EAthena
