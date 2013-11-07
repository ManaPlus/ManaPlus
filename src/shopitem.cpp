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

#include "shopitem.h"

#include "units.h"

#include "resources/iteminfo.h"

#include "utils/stringutils.h"

#include "debug.h"

extern int serverVersion;

ShopItem::ShopItem(const int inventoryIndex, const int id,
                   const unsigned char color,
                   const int quantity, const int price) :
    Item(id, 0, 0, color),
    mDisplayName(),
    mDuplicates(),
    mPrice(price),
    mShowQuantity(true)
{
    if (serverVersion > 0)
    {
        mDisplayName = std::string(getInfo().getName(color)).append(" (")
            .append(Units::formatCurrency(mPrice)).append(") ");
    }
    else
    {
        mDisplayName = std::string(getInfo().getName()).append(" (")
            .append(Units::formatCurrency(mPrice)).append(") ");
    }
    if (quantity > 0)
        mDisplayName.append("[").append(toString(quantity)).append("]");

    setInvIndex(inventoryIndex);
    addDuplicate(inventoryIndex, quantity);
}

ShopItem::ShopItem(const int id, const unsigned char color, const int price) :
    Item(id, 0, 0, color),
    mDisplayName(),
    mDuplicates(),
    mPrice(price),
    mShowQuantity(false)
{
    if (serverVersion > 0)
    {
        mDisplayName = std::string(getInfo().getName(color)).append(" (")
            .append(Units::formatCurrency(mPrice)).append(")");
    }
    else
    {
        mDisplayName = std::string(getInfo().getName()).append(" (")
            .append(Units::formatCurrency(mPrice)).append(")");
    }
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

void ShopItem::update()
{
    if (mShowQuantity)
    {
        if (serverVersion > 0)
        {
            mDisplayName = std::string(getInfo().getName(mColor)).append(" (")
                .append(Units::formatCurrency(mPrice)).append(") ");
        }
        else
        {
            mDisplayName = std::string(getInfo().getName()).append(" (")
                .append(Units::formatCurrency(mPrice)).append(") ");
        }
        if (mQuantity > 0)
            mDisplayName.append("[").append(toString(mQuantity)).append("]");
    }
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
        dupl = nullptr;
        mDuplicates.pop();
    }
    return sellCount;
}
