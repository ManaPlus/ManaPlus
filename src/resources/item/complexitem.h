/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef RESOURCES_ITEM_COMPLEXITEM_H
#define RESOURCES_ITEM_COMPLEXITEM_H

#include "resources/item/item.h"

#include "localconsts.h"

/**
 * Represents one or more instances of a certain item type.
 */
class ComplexItem final : public Item
{
    public:
        /**
         * Constructor.
         */
        ComplexItem(const int id,
                    const ItemTypeT type,
                    const int quantity,
                    const uint8_t refine,
                    const ItemColor color,
                    const Identified identified,
                    const Damaged damaged,
                    const Favorite favorite,
                    const Equipm equipment,
                    const Equipped equipped);

        A_DELETE_COPY(ComplexItem)

        /**
         * Destructor.
         */
        virtual ~ComplexItem();

        void addChild(const Item *const item,
                      const int amount);

        const STD_VECTOR<Item*> &getChilds() const noexcept2 A_WARN_UNUSED
        { return mChildItems; }

    protected:
        STD_VECTOR<Item*> mChildItems;
};

#endif  // RESOURCES_ITEM_COMPLEXITEM_H
