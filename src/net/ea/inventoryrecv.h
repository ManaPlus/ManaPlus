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

#ifndef NET_EA_INVENTORYRECV_H
#define NET_EA_INVENTORYRECV_H

#include "localconsts.h"

#include "enums/simpletypes/beingid.h"

#include "net/ea/inventoryitem.h"

#include <queue>

namespace Net
{
    class MessageIn;
}  // namespace Net

class Inventory;

namespace Ea
{
    typedef std::queue<BeingId> PickupQueue;

    class EquipBackend;

    namespace InventoryRecv
    {
        extern EquipBackend mEquips;
        extern InventoryItems mStorageItems;
        extern Inventory *mStorage;
        extern PickupQueue mSentPickups;
        extern bool mDebugInventory;

        void processItemUseResponse(Net::MessageIn &msg);
        void processPlayerStorageStatus(Net::MessageIn &msg);
        void processPlayerStorageClose(Net::MessageIn &msg);
        void processPlayerAttackRange(Net::MessageIn &msg);
        void processPlayerArrowEquip(Net::MessageIn &msg);
    }  // namespace InventoryRecv
}  // namespace Ea

#endif  // NET_EA_INVENTORYRECV_H
