/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef RESOURCES_INVENTORY_COMPLEXINVENTORY_H
#define RESOURCES_INVENTORY_COMPLEXINVENTORY_H

#include "resources/inventory/inventory.h"

#include "localconsts.h"

class ComplexInventory final : public Inventory
{
    public:
        A_DELETE_COPY(ComplexInventory)

        /**
         * Constructor.
         *
         * @param size the number of items that fit in the inventory
         */
        ComplexInventory(const InventoryTypeT type,
                         const int size);

        /**
         * Destructor.
         */
        ~ComplexInventory();

        bool addVirtualItem(const Item *const item,
                            int index,
                            const int amount) override final;

        void setItem(const int index,
                     const int id,
                     const ItemTypeT type,
                     const int quantity,
                     const uint8_t refine,
                     const ItemColor color,
                     const Identified identified,
                     const Damaged damaged,
                     const Favorite favorite,
                     const Equipm equipment,
                     const Equipped equipped) override final;
};

#endif  // RESOURCES_INVENTORY_COMPLEXINVENTORY_H
