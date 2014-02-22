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

#include "gui/models/shopitems.h"

#include "shopitem.h"

#include "utils/dtor.h"

#include "debug.h"

ShopItems::ShopItems(const bool mergeDuplicates) :
    mShopItems(),
    mMergeDuplicates(mergeDuplicates)
{
}

ShopItems::~ShopItems()
{
    clear();
}

std::string ShopItems::getElementAt(int i)
{
    if (i < 0 || static_cast<unsigned>(i) >= mShopItems.size()
        || !mShopItems.at(i))
    {
        return "";
    }

    return mShopItems.at(i)->getDisplayName();
}

void ShopItems::addItem(const int id, const unsigned char color,
                        const int amount, const int price)
{
    mShopItems.push_back(new ShopItem(-1, id, color, amount, price));
}

void ShopItems::addItemNoDup(const int id, const unsigned char color,
                             const int amount, const int price)
{
    const ShopItem *const item = findItem(id, color);
    if (!item)
        mShopItems.push_back(new ShopItem(-1, id, color, amount, price));
}

void ShopItems::addItem2(const int inventoryIndex, const int id,
                         const unsigned char color,
                         const int quantity, const int price)
{
    ShopItem *item = nullptr;
    if (mMergeDuplicates)
        item = findItem(id, color);

    if (item)
    {
        item->addDuplicate(inventoryIndex, quantity);
    }
    else
    {
        item = new ShopItem(inventoryIndex, id, color, quantity, price);
        mShopItems.push_back(item);
    }
}

ShopItem *ShopItems::at(unsigned int i) const
{
    if (i >= mShopItems.size())
        return nullptr;

    return mShopItems.at(i);
}

void ShopItems::erase(const unsigned int i)
{
    if (i >= mShopItems.size())
        return;

    mShopItems.erase(mShopItems.begin() + i);
}

void ShopItems::del(const unsigned int i)
{
    if (i >= mShopItems.size())
        return;

    ShopItem *item = *(mShopItems.begin() + i);
    mShopItems.erase(mShopItems.begin() + i);
    delete item;
}

void ShopItems::clear()
{
    delete_all(mShopItems);
    mShopItems.clear();
}

ShopItem *ShopItems::findItem(const int id, const unsigned char color) const
{
    std::vector<ShopItem*>::const_iterator it = mShopItems.begin();
    const std::vector<ShopItem*>::const_iterator e = mShopItems.end();
    while (it != e)
    {
        ShopItem *const item = *it;
        if (item->getId() == id && item->getColor() == color)
            return item;

        ++it;
    }

    return nullptr;
}
