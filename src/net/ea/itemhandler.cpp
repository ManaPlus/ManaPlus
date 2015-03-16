/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/ea/itemhandler.h"

#include "actormanager.h"

#include "net/messagein.h"

#include "debug.h"

namespace Ea
{

ItemHandler::ItemHandler()
{
}

ItemHandler::~ItemHandler()
{
}

void ItemHandler::processItemVisible(Net::MessageIn &msg)
{
    const int id = msg.readInt32("item object id");
    const int itemId = msg.readInt16("item id");
    const uint8_t identify = msg.readUInt8("identify");
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    const int amount = msg.readInt16("amount");
    const int subX = static_cast<int>(msg.readInt8("sub x"));
    const int subY = static_cast<int>(msg.readInt8("sub y"));

    if (actorManager)
    {
        actorManager->createItem(id, itemId,
            x, y, amount, identify, subX, subY);
    }
}

void ItemHandler::processItemRemove(Net::MessageIn &msg)
{
    if (actorManager)
    {
        if (FloorItem *const item = actorManager
            ->findItem(msg.readInt32("floor item id")))
        {
            actorManager->destroy(item);
        }
    }
}

}  // namespace Ea
