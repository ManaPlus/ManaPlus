/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "resources/inventory/complexinventory.h"

#include "logger.h"

#include "being/playerinfo.h"

#include "utils/cast.h"

#include "resources/item/complexitem.h"

#include "debug.h"

ComplexInventory::ComplexInventory(const InventoryTypeT type,
                                   const int size1) :
    Inventory(type, size1)
{
}

ComplexInventory::~ComplexInventory()
{
}

bool ComplexInventory::addVirtualItem(const Item *const item,
                                      int index,
                                      const int amount)
{
    if ((item == nullptr) || PlayerInfo::isItemProtected(item->getId()))
        return false;

    if (index >= 0 && index < CAST_S32(mSize))
    {
        ComplexItem *citem = nullptr;
        if (mItems[index] != nullptr)
        {
            const Item *const item2 = mItems[index];
            if (item->getId() != item2->getId() ||
                item->getColor() != item2->getColor())
            {   // not same id or color
                return false;
            }
            citem = dynamic_cast<ComplexItem*>(mItems[index]);
            if (citem == nullptr)
            {   // if in inventory not complex item, converting it to complex
                citem = new ComplexItem(item2->getId(),
                    item2->getType(),
                    item2->getQuantity(),
                    item2->getRefine(),
                    item2->getColor(),
                    item2->getIdentified(),
                    item2->getDamaged(),
                    item2->getFavorite(),
                    Equipm_false,
                    Equipped_false);
                citem->setTag(item2->getTag());
                delete mItems[index];
                mItems[index] = citem;
            }
        }
        else
        {
            citem = new ComplexItem(item->getId(),
                item->getType(),
                0,
                item->getRefine(),
                item->getColor(),
                item->getIdentified(),
                item->getDamaged(),
                item->getFavorite(),
                Equipm_false,
                Equipped_false);
            mItems[index] = citem;
        }
        citem->addChild(item, amount);
    }
    else
    {
        index = addItem(item->getId(),
            item->getType(),
            1,
            1,
            item->getColor(),
            item->getIdentified(),
            item->getDamaged(),
            item->getFavorite(),
            Equipm_false,
            Equipped_false);
    }
    if (index == -1)
        return false;

    Item *const item2 = getItem(index);
    if (item2 != nullptr)
        item2->setTag(item->getInvIndex());
    return true;
}

void ComplexInventory::setItem(const int index,
                               const int id,
                               const ItemTypeT type,
                               const int quantity,
                               const uint8_t refine,
                               const ItemColor color,
                               const Identified identified,
                               const Damaged damaged,
                               const Favorite favorite,
                               const Equipm equipment,
                               const Equipped equipped)
{
    if (index < 0 || index >= CAST_S32(mSize))
    {
        logger->log("Warning: invalid inventory index: %d", index);
        return;
    }

    const Item *const item1 = mItems[index];
    if ((item1 == nullptr) && id > 0)
    {
        ComplexItem *const item = new ComplexItem(id,
            type,
            quantity,
            refine,
            color,
            identified,
            damaged,
            favorite,
            equipment,
            equipped);
        item->setInvIndex(index);

        Item *const item2 = new Item(id,
            type,
            quantity,
            refine,
            color,
            identified,
            damaged,
            favorite,
            equipment,
            equipped);
        item2->setInvIndex(index);
        item->addChild(item2, quantity);
        delete item2;

        mItems[index] = item;
        mUsed ++;
        distributeSlotsChangedEvent();
    }
}
