/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef NET_TA_INVENTORYHANDLER_H
#define NET_TA_INVENTORYHANDLER_H

#include "equipment.h"
#include "inventory.h"
#include "playerinfo.h"

#include "gui/inventorywindow.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "net/tmwa/messagehandler.h"

#include <list>

#ifdef __GNUC__
#define _UNUSED_  __attribute__ ((unused))
#else
#define _UNUSED_
#endif

namespace TmwAthena
{

class EquipBackend : public Equipment::Backend
{
    public:
        EquipBackend()
        {
            memset(mEquipment, -1, sizeof(mEquipment));
        }

        Item *getEquipment(int index) const
        {
            int invyIndex = mEquipment[index];
            if (invyIndex == -1)
            {
                return NULL;
            }
            return PlayerInfo::getInventory()->getItem(invyIndex);
        }

        void clear()
        {
            for (int i = 0; i < EQUIPMENT_SIZE; i++)
            {
                if (mEquipment[i] != -1)
                {
                    Item* item = PlayerInfo::getInventory()->getItem(i);
                    if (item)
                    {
                        item->setEquipped(false);
                    }
                }

                mEquipment[i] = -1;
            }
        }

        void setEquipment(int index, int inventoryIndex)
        {
            // Unequip existing item
            Item* item = PlayerInfo::getInventory()
                ->getItem(mEquipment[index]);

            if (item)
                item->setEquipped(false);

            mEquipment[index] = inventoryIndex;

            item = PlayerInfo::getInventory()->getItem(inventoryIndex);
            if (item)
                item->setEquipped(true);
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
        unsigned char color;
        int refine;
        bool equip;

        InventoryItem(int slot, int id, int quantity, int refine,
                      unsigned char color, bool equip)
        {
            this->slot = slot;
            this->id = id;
            this->quantity = quantity;
            this->refine = refine;
            this->color = color;
            this->equip = equip;
        }
};

typedef std::list<InventoryItem> InventoryItems;

class InventoryHandler : public MessageHandler, public Net::InventoryHandler
{
    public:
        enum
        {
            GUILD_STORAGE = Inventory::TYPE_END,
            CART
        };

        InventoryHandler();

        ~InventoryHandler();

        void handleMessage(Net::MessageIn &msg);

        void equipItem(const Item *item);

        void unequipItem(const Item *item);

        void useItem(const Item *item);

        void dropItem(const Item *item, int amount);

        bool canSplit(const Item *item);

        void splitItem(const Item *item, int amount);

        void moveItem(int oldIndex, int newIndex);

        void openStorage(int type);

        void closeStorage(int type);

        void moveItem(int source, int slot, int amount,
                      int destination);

        size_t getSize(int type) const;

        int convertFromServerSlot(int serverSlot);

    private:
        EquipBackend mEquips;
        InventoryItems mInventoryItems;
        Inventory *mStorage;
        InventoryWindow *mStorageWindow;
};

} // namespace TmwAthena

int getSlot(int eAthenaSlot);

#endif // NET_TA_INVENTORYHANDLER_H
