/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef NET_INVENTORYHANDLER_H
#define NET_INVENTORYHANDLER_H

#include "enums/simpletypes/beingid.h"

#include "resources/inventory/inventory.h"

#include <vector>

namespace Net
{

class InventoryHandler notfinal
{
    public:
        virtual ~InventoryHandler()
        { }

        virtual void clear() const = 0;

        virtual void equipItem(const Item *const item) const = 0;

        virtual void unequipItem(const Item *const item) const = 0;

        virtual void useItem(const Item *const item) const = 0;

        virtual void dropItem(const Item *const item,
                              const int amount) const = 0;

        virtual bool canSplit(const Item *const item) const = 0;

        virtual void splitItem(const Item *const item,
                               const int amount) const = 0;

        virtual void moveItem(const int oldIndex,
                              const int newIndex) const = 0;

        virtual void closeStorage() const = 0;

        virtual void moveItem2(const InventoryTypeT source,
                               const int slot,
                               const int amount,
                               const InventoryTypeT destination) const = 0;

        virtual size_t getSize(const InventoryTypeT type) const
                               A_WARN_UNUSED = 0;

        virtual Inventory *getStorage() const = 0;

        virtual void destroyStorage() const = 0;

        virtual void forgotStorage() const = 0;

        virtual int convertFromServerSlot(const int eAthenaSlot) const = 0;

        virtual void useCard(const Item *const item) = 0;

        virtual void insertCard(const int cardIndex,
                                const int itemIndex) const = 0;

        virtual void favoriteItem(const Item *const item,
                                  const bool favorite) const = 0;

        virtual void selectEgg(const Item *const item) const = 0;

        virtual int getProjectileSlot() const = 0;

        virtual int getItemIndex() const A_WARN_UNUSED = 0;

        virtual void selectCart(const BeingId accountId,
                                const int type) const = 0;

        virtual void identifyItem(const Item *const item) const = 0;

        virtual void mergeItemsAck(const std::vector<Item*> &items) const = 0;

        virtual void mergetItemsCancel() const = 0;
};

}  // namespace Net

extern Net::InventoryHandler *inventoryHandler;

#endif  // NET_INVENTORYHANDLER_H
