/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "shopitem.h"

#include "units.h"

#include "resources/iteminfo.h"

#include "utils/stringutils.h"

#include "net/serverfeatures.h"

#include "debug.h"

ShopItem::ShopItem(const int inventoryIndex,
                   const int id,
                   const int type,
                   const unsigned char color,
                   const int quantity,
                   const int price) :
    Item(id, type, 0, 0, color,
         Identified_true,
         Damaged_false,
         Favorite_false,
         Equipm_false,
         Equipped_false),
    mDisplayName(),
    mDuplicates(),
    mPrice(price),
    mShowQuantity(true),
    mVisible(true)
{
    updateDisplayName(quantity);
    setInvIndex(inventoryIndex);
    addDuplicate(inventoryIndex, quantity);
}

ShopItem::ShopItem(const int id,
                   const int type,
                   const unsigned char color,
                   const int price) :
    Item(id, type, 0, 0, color,
         Identified_true,
         Damaged_false,
         Favorite_false,
         Equipm_false,
         Equipped_false),
    mDisplayName(),
    mDuplicates(),
    mPrice(price),
    mShowQuantity(false),
    mVisible(true)
{
    updateDisplayName(0);
    setInvIndex(-1);
    addDuplicate(-1, 0);
}

ShopItem::~ShopItem()
{
    /** Clear all remaining duplicates on Object destruction. */
    while (!mDuplicates.empty())
    {
        delete mDuplicates.top();
        mDuplicates.pop();
    }
}

void ShopItem::updateDisplayName(const int quantity)
{
    if (serverFeatures->haveItemColors())
        mDisplayName = std::string(getInfo().getName(mColor));
    else
        mDisplayName = std::string(getInfo().getName());
    if (mPrice)
    {
        mDisplayName.append(" (").append(
            Units::formatCurrency(mPrice)).append(") ");
    }
    if (quantity > 1)
        mDisplayName.append("[").append(toString(quantity)).append("]");
}

void ShopItem::update()
{
    if (mShowQuantity)
        updateDisplayName(mQuantity);
}

void ShopItem::addDuplicate(const int inventoryIndex, const int quantity)
{
    DuplicateItem *const di = new DuplicateItem;
    di->inventoryIndex = inventoryIndex;
    di->quantity = quantity;
    mDuplicates.push(di);
    mQuantity += quantity;
}

void ShopItem::addDuplicate()
{
    DuplicateItem *const di = new DuplicateItem;
    di->inventoryIndex = -1;
    di->quantity = 0;
    mDuplicates.push(di);
}

int ShopItem::sellCurrentDuplicate(const int quantity)
{
    DuplicateItem* dupl = mDuplicates.top();
    if (!dupl)
        return 0;

    const int sellCount = quantity <= dupl->quantity
        ? quantity : dupl->quantity;
    dupl->quantity -= sellCount;
    mQuantity -= sellCount;
    if (dupl->quantity == 0)
    {
        delete dupl;
        mDuplicates.pop();
    }
    return sellCount;
}
