/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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
#include "item.h"
#include "localconsts.h"

#include "being/playerinfo.h"

#include "gui/windows/inventorywindow.h"

#include "net/inventoryhandler.h"
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

        Item *getEquipment(const int index) const override final A_WARN_UNUSED
        {
            int invyIndex = mEquipment[index];
            if (invyIndex == -1)
                return nullptr;

            const Inventory *const inv = PlayerInfo::getInventory();
            if (inv)
                return inv->getItem(invyIndex);
            else
                return nullptr;
        }

        void clear()
        {
            Inventory *const inv = PlayerInfo::getInventory();
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

        void setEquipment(const int index, const int inventoryIndex)
        {
            Inventory *const inv = PlayerInfo::getInventory();
            if (!inv)
                return;

            // Unequip existing item
            Item *item = inv->getItem(mEquipment[index]);

            if (item)
                item->setEquipped(false);

            // not checking index because it must be safe
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

        InventoryItem(const int slot0, const int id0, const int quantity0,
                      const int refine0, const unsigned char color0,
                      const bool equip0) :
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

        A_DELETE_COPY(InventoryHandler)

        virtual ~InventoryHandler();

        void clear();

        bool canSplit(const Item *const item) const
                      override final A_WARN_UNUSED;

        void splitItem(const Item *const item,
                       const int amount) const override final;

        void moveItem(const int oldIndex,
                      const int newIndex) const override final;

        void openStorage(const int type) const override final;

        size_t getSize(const int type) const override final A_WARN_UNUSED;

        int convertFromServerSlot(const int serverSlot)
                                  const override final A_WARN_UNUSED;

        void pushPickup(const int floorId)
        { mSentPickups.push(floorId); }

        int getSlot(const int eAthenaSlot) const A_WARN_UNUSED;

        void processPlayerInventory(Net::MessageIn &msg,
                                    const bool playerInvintory);

        void processPlayerStorageEquip(Net::MessageIn &msg);

        void processPlayerInventoryAdd(Net::MessageIn &msg);

        void processPlayerInventoryRemove(Net::MessageIn &msg) const;

        void processPlayerInventoryUse(Net::MessageIn &msg) const;

        void processItemUseResponse(Net::MessageIn &msg) const;

        void processPlayerStorageStatus(Net::MessageIn &msg);

        void processPlayerStorageAdd(Net::MessageIn &msg);

        void processPlayerStorageRemove(Net::MessageIn &msg);

        void processPlayerStorageClose(Net::MessageIn &msg);

        void processPlayerEquipment(Net::MessageIn &msg);

        void processPlayerEquip(Net::MessageIn &msg);

        void processPlayerUnEquip(Net::MessageIn &msg);

        void processPlayerAttackRange(Net::MessageIn &msg) const;

        void processPlayerArrowEquip(Net::MessageIn &msg);

        void closeStorage() override final;

        Inventory *getStorage() const
        { return mStorage; }

    protected:
        InventoryHandler();

        EquipBackend mEquips;
        InventoryItems mInventoryItems;
        Inventory *mStorage;
        InventoryWindow *mStorageWindow;
        bool mDebugInventory;

        typedef std::queue<int> PickupQueue;
        PickupQueue mSentPickups;
};

}  // namespace Ea

#endif  // NET_EA_INVENTORYHANDLER_H
