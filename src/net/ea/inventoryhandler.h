/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef NET_EA_INVENTORYHANDLER_H
#define NET_EA_INVENTORYHANDLER_H

#include "equipment.h"
#include "inventory.h"
#include "localconsts.h"
#include "playerinfo.h"

#include "gui/inventorywindow.h"

#include "net/messagein.h"

#include <vector>
#include <queue>

namespace Ea
{

class EquipBackend : public Equipment::Backend
{
    public:
        EquipBackend()
        {
            memset(mEquipment, -1, sizeof(mEquipment));
        }

        A_DELETE_COPY(EquipBackend)

        Item *getEquipment(int index) const
        {
            int invyIndex = mEquipment[index];
            if (invyIndex == -1)
                return nullptr;

            if (PlayerInfo::getInventory())
                return PlayerInfo::getInventory()->getItem(invyIndex);
            else
                return nullptr;
        }

        void clear()
        {
            Inventory *inv = PlayerInfo::getInventory();
            if (!inv)
                return;
            for (int i = 0; i < EQUIPMENT_SIZE; i++)
            {
                if (mEquipment[i] != -1)
                {
                    Item* item = inv->getItem(i);
                    if (item)
                        item->setEquipped(false);
                }

                mEquipment[i] = -1;
            }
        }

        void setEquipment(int index, int inventoryIndex)
        {
            Inventory *inv = PlayerInfo::getInventory();
            if (!inv)
                return;

            // Unequip existing item
            Item* item = inv->getItem(mEquipment[index]);

            if (item)
                item->setEquipped(false);

            mEquipment[index] = inventoryIndex;

            item = inv->getItem(inventoryIndex);
            if (item)
                item->setEquipped(true);

            if (inventoryWindow)
                inventoryWindow->updateButtons();
        }

    private:
        int mEquipment[EQUIPMENT_SIZE];
};

/**
 * Used to cache storage data until we get size data for it.
 */
class InventoryItem
{
    public:
        int slot;
        int id;
        int quantity;
        int refine;
        unsigned char color;
        bool equip;

        InventoryItem(int slot0, int id0, int quantity0, int refine0,
                      unsigned char color0, bool equip0) :
            slot(slot0),
            id(id0),
            quantity(quantity0),
            refine(refine0),
            color(color0),
            equip(equip0)
        {
        }
};

typedef std::vector<InventoryItem> InventoryItems;

class InventoryHandler : public Net::InventoryHandler
{
    public:
        enum
        {
            GUILD_STORAGE = Inventory::TYPE_END,
            CART
        };

        InventoryHandler();

        A_DELETE_COPY(InventoryHandler)

        ~InventoryHandler();

        bool canSplit(const Item *item) const;

        void splitItem(const Item *item, int amount);

        void moveItem(int oldIndex, int newIndex);

        void openStorage(int type);

        size_t getSize(int type) const;

        int convertFromServerSlot(int serverSlot) const;

        void pushPickup(int floorId)
        { mSentPickups.push(floorId); }

        int getSlot(int eAthenaSlot);

        void processPlayerInventory(Net::MessageIn &msg, bool playerInvintory);

        void processPlayerStorageEquip(Net::MessageIn &msg);

        void processPlayerInventoryAdd(Net::MessageIn &msg);

        void processPlayerInventoryRemove(Net::MessageIn &msg);

        void processPlayerInventoryUse(Net::MessageIn &msg);

        void processItemUseResponse(Net::MessageIn &msg);

        void processPlayerStorageStatus(Net::MessageIn &msg);

        void processPlayerStorageAdd(Net::MessageIn &msg);

        void processPlayerStorageRemove(Net::MessageIn &msg);

        void processPlayerStorageClose(Net::MessageIn &msg);

        void processPlayerEquipment(Net::MessageIn &msg);

        void processPlayerEquip(Net::MessageIn &msg);

        void processPlayerUnEquip(Net::MessageIn &msg);

        void processPlayerAttackRange(Net::MessageIn &msg);

        void processPlayerArrowEquip(Net::MessageIn &msg);

    protected:
        EquipBackend mEquips;
        InventoryItems mInventoryItems;
        Inventory *mStorage;
        InventoryWindow *mStorageWindow;
        bool mDebugInventory;

        typedef std::queue<int> PickupQueue;
        PickupQueue mSentPickups;
};

} // namespace Ea

#endif // NET_EA_INVENTORYHANDLER_H
