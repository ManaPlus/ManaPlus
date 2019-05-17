/*
 *  The ManaPlus Client
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

#include "net/eathena/refinehandler.h"

#include "const/net/inventory.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

namespace EAthena
{

RefineHandler::RefineHandler() :
    Net::RefineHandler()
{
    refineHandler = this;
}

RefineHandler::~RefineHandler()
{
    refineHandler = nullptr;
}

void RefineHandler::addItem(const Item *const item) const
{
    if (item == nullptr)
        return;
    createOutPacket(CMSG_REFINE_ADD_ITEM);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "inv index");
}

void RefineHandler::refineItem(const Item *const item1,
                               const Item *const item2,
                               const int blessing) const
{
    if (item1 == nullptr ||
        item2 == nullptr)
    {
        return;
    }
    createOutPacket(CMSG_REFINE_ITEM);
    outMsg.writeInt16(CAST_S16(
        item1->getInvIndex() + INVENTORY_OFFSET), "inv index");
    outMsg.writeItemId(item2->getId(), "item id");
    outMsg.writeInt8(blessing, "blessing");
}

void RefineHandler::close() const
{
    createOutPacket(CMSG_REFINE_CLOSE);
}

}  // namespace EAthena
