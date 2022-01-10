/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "net/tmwa/itemrecv.h"

#include "actormanager.h"

#include "net/messagein.h"

#include "debug.h"

namespace TmwAthena
{

void ItemRecv::processItemDropped(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("item object id");
    const int itemId = msg.readInt16("item id");
    const Identified identify = fromInt(msg.readUInt8("identify"), Identified);
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    const int subX = CAST_S32(msg.readInt8("sub x"));
    const int subY = CAST_S32(msg.readInt8("sub y"));
    const int amount = msg.readInt16("amount");

    if (actorManager != nullptr)
    {
        actorManager->createItem(id,
            itemId,
            x, y,
            ItemType::Unknown,
            amount,
            0,
            ItemColor_one,
            identify,
            Damaged_false,
            subX, subY,
            nullptr);
    }
}

void ItemRecv::processItemVisible(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("item object id");
    const int itemId = msg.readInt16("item id");
    const Identified identified = fromInt(
        msg.readUInt8("identify"), Identified);
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    const int amount = msg.readInt16("amount");
    const int subX = CAST_S32(msg.readInt8("sub x"));
    const int subY = CAST_S32(msg.readInt8("sub y"));

    if (actorManager != nullptr)
    {
        actorManager->createItem(id,
            itemId,
            x, y,
            ItemType::Unknown,
            amount,
            0,
            ItemColor_one,
            identified,
            Damaged_false,
            subX, subY,
            nullptr);
    }
}

}  // namespace TmwAthena
