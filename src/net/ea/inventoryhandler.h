/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "inventory.h"
#include "localconsts.h"

#include "net/inventoryhandler.h"

#include "net/ea/equipbackend.h"
#include "net/ea/inventoryitem.h"

#include <vector>
#include <queue>

namespace Net
{
    class MessageIn;
}

namespace Ea
{

typedef std::vector<InventoryItem> InventoryItems;
typedef std::queue<int> PickupQueue;

class InventoryHandler notfinal : public Net::InventoryHandler
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

        void pushPickup(const int floorId)
        { mSentPickups.push(floorId); }

        static int getSlot(const int eAthenaSlot) A_WARN_UNUSED;

        static void processPlayerInventoryRemove(Net::MessageIn &msg);

        static void processPlayerInventoryUse(Net::MessageIn &msg);

        static void processItemUseResponse(Net::MessageIn &msg);

        static void processPlayerStorageStatus(Net::MessageIn &msg);

        static void processPlayerStorageClose(Net::MessageIn &msg);

        static void processPlayerAttackRange(Net::MessageIn &msg);

        static void processPlayerArrowEquip(Net::MessageIn &msg);

        void closeStorage() override final;

        void forgotStorage() override final;

        Inventory *getStorage() const
        { return mStorage; }

    protected:
        InventoryHandler();

        static EquipBackend mEquips;
        static InventoryItems mInventoryItems;
        static Inventory *mStorage;
        static PickupQueue mSentPickups;
        static bool mDebugInventory;
};

}  // namespace Ea

#endif  // NET_EA_INVENTORYHANDLER_H
