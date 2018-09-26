/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_EATHENA_INVENTORYRECV_H
#define NET_EATHENA_INVENTORYRECV_H

#include "net/ea/inventoryitem.h"

#include "enums/net/netinventorytype.h"

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace InventoryRecv
    {
        extern Ea::InventoryItems mCartItems;

        void processPlayerEquipment(Net::MessageIn &msg);
        void processPlayerInventoryAdd(Net::MessageIn &msg);
        void processPlayerInventory(Net::MessageIn &msg);
        void processPlayerStorage(Net::MessageIn &msg);
        void processPlayerEquip(Net::MessageIn &msg);
        void processPlayerUnEquip(Net::MessageIn &msg);
        void processPlayerInventoryRemove2(Net::MessageIn &msg);
        void processPlayerStorageEquip(Net::MessageIn &msg);
        void processPlayerStorageAdd(Net::MessageIn &msg);
        void processPlayerUseCard(Net::MessageIn &msg);
        void processPlayerInsertCard(Net::MessageIn &msg);
        void processPlayerItemRentalTime(Net::MessageIn &msg);
        void processPlayerItemRentalExpired(Net::MessageIn &msg);
        void processPlayerStorageRemove(Net::MessageIn &msg);
        void processCartInfo(Net::MessageIn &msg);
        void processCartRemove(Net::MessageIn &msg);
        void processPlayerCartAdd(Net::MessageIn &msg);
        void processPlayerCartEquip(Net::MessageIn &msg);
        void processPlayerCartItems(Net::MessageIn &msg);
        void processPlayerCartRemove(Net::MessageIn &msg);
        void processPlayerIdentifyList(Net::MessageIn &msg);
        void processPlayerIdentified(Net::MessageIn &msg);
        void processPlayerRefine(Net::MessageIn &msg);
        void processPlayerRepairList(Net::MessageIn &msg);
        void processPlayerRepairEffect(Net::MessageIn &msg);
        void processPlayerRefineList(Net::MessageIn &msg);
        void processPlayerStoragePassword(Net::MessageIn &msg);
        void processPlayerStoragePasswordResult(Net::MessageIn &msg);
        void processPlayerCookingList(Net::MessageIn &msg);
        void processItemDamaged(Net::MessageIn &msg);
        void processFavoriteItem(Net::MessageIn &msg);
        void processCartAddError(Net::MessageIn &msg);
        void processBindItem(Net::MessageIn &msg);
        void processPlayerInventoryRemove(Net::MessageIn &msg);
        void processSelectCart(Net::MessageIn &msg);
        void processMergeItem(Net::MessageIn &msg);
        void processMergeItemResponse(Net::MessageIn &msg);
        void processPlayerInventoryUse(Net::MessageIn &msg);
        void processItemMoveFailed(Net::MessageIn &msg);
        void processOverWeightPercent(Net::MessageIn &msg);
        void processInventoryStart1(Net::MessageIn &msg);
        void processInventoryStart2(Net::MessageIn &msg);
        void processInventoryStart3(Net::MessageIn &msg);
        void processInventoryEnd1(Net::MessageIn &msg);
        void processInventoryEnd2(Net::MessageIn &msg);
        void processPlayerCombinedInventory1(Net::MessageIn &msg);
        void processPlayerCombinedInventory2(Net::MessageIn &msg);
        void processPlayerCombinedEquipment1(Net::MessageIn &msg);
        void processPlayerCombinedEquipment2(Net::MessageIn &msg);

        int getSlot(const int eAthenaSlot) A_WARN_UNUSED;
        void processInventoryContinue(Net::MessageIn &msg,
                                      const int len,
                                      const NetInventoryTypeT invType);
        void processEquipmentContinue(Net::MessageIn &msg,
                                      const int len,
                                      const NetInventoryTypeT invType
                                      A_UNUSED);
    }  // namespace InventoryRecv
}  // namespace EAthena

#endif  // NET_EATHENA_INVENTORYRECV_H
