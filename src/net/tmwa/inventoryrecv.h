/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_TMWA_INVENTORYRECV_H
#define NET_TMWA_INVENTORYRECV_H

#include "localconsts.h"

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace TmwAthena
{
    namespace InventoryRecv
    {
        void processPlayerEquipment(Net::MessageIn &msg);
        void processPlayerInventoryAdd(Net::MessageIn &msg);
        void processPlayerInventory(Net::MessageIn &msg);
        void processPlayerStorage(Net::MessageIn &msg);
        void processPlayerEquip(Net::MessageIn &msg);
        void processPlayerUnEquip(Net::MessageIn &msg);
        void processPlayerStorageEquip(Net::MessageIn &msg);
        void processPlayerStorageAdd(Net::MessageIn &msg);
        void processPlayerStorageRemove(Net::MessageIn &msg);
        void processPlayerInventoryRemove(Net::MessageIn &msg);
        void processPlayerInventoryUse(Net::MessageIn &msg);

        int getSlot(const int eAthenaSlot) A_WARN_UNUSED;
    }  // namespace InventoryRecv
}  // namespace TmwAthena

#endif  // NET_TMWA_INVENTORYRECV_H
