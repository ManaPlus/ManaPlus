/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef NET_TMWA_INVENTORYHANDLER_H
#define NET_TMWA_INVENTORYHANDLER_H

#include "net/ea/inventoryhandler.h"

namespace TmwAthena
{

class InventoryHandler final : public Ea::InventoryHandler
{
    public:
        InventoryHandler();

        A_DELETE_COPY(InventoryHandler)

        ~InventoryHandler();

        void equipItem(const Item *const item) const final;

        void unequipItem(const Item *const item) const final;

        void useItem(const Item *const item) const final;

        void dropItem(const Item *const item,
                      const int amount) const final;

        void closeStorage() const final;

        void moveItem2(const InventoryTypeT source,
                       const int slot,
                       const int amount,
                       const InventoryTypeT destination) const final;

        void useCard(const Item *const item) final A_CONST;

        void insertCard(const int cardIndex,
                        const int itemIndex) const final A_CONST;

        void favoriteItem(const Item *const item,
                          const bool favorite) const final A_CONST;

        void selectEgg(const Item *const item) const final A_CONST;

        int convertFromServerSlot(const int serverSlot)
                                  const final A_WARN_UNUSED;

        void selectCart(const BeingId accountId,
                        const int type) const final;

        void identifyItem(const Item *const item) const final;

        void mergeItemsAck(const std::vector<Item*> &items) const
                           final;

        void mergetItemsCancel() const final;

        int getProjectileSlot() const final
        { return 10; }

        int getItemIndex() const final A_WARN_UNUSED
        { return 0; }
};

}  // namespace TmwAthena

#endif  // NET_TMWA_INVENTORYHANDLER_H
