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

#include "resources/item/complexitem.h"

#include "utils/dtor.h"
#include "utils/foreach.h"

#include "debug.h"

ComplexItem::ComplexItem(const int id,
                         const ItemTypeT type,
                         const int quantity,
                         const uint8_t refine,
                         const ItemColor color,
                         const Identified identified,
                         const Damaged damaged,
                         const Favorite favorite,
                         const Equipm equipment,
                         const Equipped equipped) :
    Item(id,
         type,
         quantity,
         refine,
         color,
         identified,
         damaged,
         favorite,
         equipment,
         equipped),
    mChildItems()
{
}

ComplexItem::~ComplexItem()
{
    delete_all(mChildItems);
    mChildItems.clear();
}

void ComplexItem::addChild(const Item *const item,
                           const int amount)
{
    if (item == nullptr)
        return;
    increaseQuantity(amount);
    Item *child = nullptr;
    FOR_EACH (STD_VECTOR<Item*>::iterator, it, mChildItems)
    {
        Item *const item1 = *it;
        if (item1->getId() == item->getId() &&
            item1->getInvIndex() == item->getInvIndex() &&
            item1->getTag() == item->getTag())
        {
            child = item1;
            break;
        }
    }
    if (child != nullptr)
    {
        child->increaseQuantity(amount);
    }
    else
    {
        child = new ComplexItem(item->getId(),
            item->getType(),
            amount,
            item->getRefine(),
            item->getColor(),
            item->getIdentified(),
            item->getDamaged(),
            item->getFavorite(),
            Equipm_false,
            Equipped_false);
        child->setTag(item->getTag());
        child->setInvIndex(item->getInvIndex());
        mChildItems.push_back(child);
    }
}
