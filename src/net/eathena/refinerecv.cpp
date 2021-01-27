/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/eathena/refinerecv.h"

#include "logger.h"

#include "net/messagein.h"

#include "debug.h"

extern int itemIdLen;

namespace EAthena
{

void RefineRecv::processRefineOpen(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
}

void RefineRecv::processRefineAddItem(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    int blockSize = 7;
    if (itemIdLen == 4)
        blockSize += 2;

    const int count = (msg.readInt16("len") - 7) / blockSize;
    msg.readInt16("item index");
    msg.readUInt8("blacksmith blessing");
    for (int f = 0; f < count; f ++)
    {
        msg.readItemId("item id");
        msg.readUInt8("chance");
        msg.readInt32("money");
    }
}

void RefineRecv::processRefineAnnounce(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readString(24, "char name");
    msg.readItemId("item id");
    msg.readInt8("refine level");
    msg.readInt8("status");
}

}  // namespace EAthena
