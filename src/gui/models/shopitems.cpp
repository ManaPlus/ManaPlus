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

#include "gui/models/shopitems.h"

#include "resources/item/shopitem.h"

#include "utils/dtor.h"
#include "utils/foreach.h"

#include "debug.h"

ShopItems::ShopItems(const bool mergeDuplicates,
                     const std::string &currency) :
    ListModel(),
    mAllShopItems(),
    mShopItems(),
    mCurrency(currency),
    mMergeDuplicates(mergeDuplicates)
{
}

ShopItems::~ShopItems()
{
    clear();
}

std::string ShopItems::getElementAt(int i)
{
    if (i < 0 || CAST_U32(i)
        >= CAST_U32(mShopItems.size()) || (mShopItems.at(i) == nullptr))
    {
        return "";
    }

    return mShopItems.at(i)->getDisplayName();
}

ShopItem *ShopItems::addItem(const int id,
                             const ItemTypeT type,
                             const ItemColor color,
                             const int amount,
                             const int price)
{
    ShopItem *const item = new ShopItem(-1,
        id,
        type,
        color,
        amount,
        price,
        mCurrency);
    mShopItems.push_back(item);
    mAllShopItems.push_back(item);
    return item;
}

ShopItem *ShopItems::addItemNoDup(const int id,
                                  const ItemTypeT type,
                                  const ItemColor color,
                                  const int amount,
                                  const int price)
{
    ShopItem *item = findItem(id, color);
    if (item == nullptr)
    {
        item = new ShopItem(-1,
            id,
            type,
            color,
            amount,
            price,
            mCurrency);
        mShopItems.push_back(item);
        mAllShopItems.push_back(item);
    }
    return item;
}

ShopItem *ShopItems::addItem2(const int inventoryIndex,
                              const int id,
                              const ItemTypeT type,
                              const ItemColor color,
                              const int quantity,
                              const int price)
{
    ShopItem *item = nullptr;
    if (mMergeDuplicates)
        item = findItem(id, color);

    if (item != nullptr)
    {
        item->addDuplicate(inventoryIndex, quantity);
    }
    else
    {
        item = new ShopItem(inventoryIndex,
            id,
            type,
            color,
            quantity,
            price,
            mCurrency);
        mShopItems.push_back(item);
        mAllShopItems.push_back(item);
    }
    return item;
}

ShopItem *ShopItems::at(const size_t i) const
{
    if (i >= mShopItems.size())
        return nullptr;

    return mShopItems.at(i);
}

bool ShopItems::findInAllItems(STD_VECTOR<ShopItem*>::iterator &it,
                               const ShopItem *const item)
{
    const STD_VECTOR<ShopItem*>::iterator it_end = mAllShopItems.end();
    for (it = mAllShopItems.begin(); it != it_end; ++ it)
    {
        if (*it == item)
            return true;
    }
    return false;
}

void ShopItems::erase(const unsigned int i)
{
    if (i >= CAST_U32(mShopItems.size()))
        return;

    const ShopItem *const item = *(mShopItems.begin() + i);
    STD_VECTOR<ShopItem*>::iterator it;
    if (findInAllItems(it, item))
        mAllShopItems.erase(it);
    mShopItems.erase(mShopItems.begin() + i);
}

void ShopItems::del(const unsigned int i)
{
    if (i >= CAST_U32(mShopItems.size()))
        return;

    ShopItem *item = *(mShopItems.begin() + i);
    STD_VECTOR<ShopItem*>::iterator it;
    if (findInAllItems(it, item))
        mAllShopItems.erase(it);
    mShopItems.erase(mShopItems.begin() + i);
    delete item;
}

void ShopItems::clear()
{
    delete_all(mAllShopItems);
    mAllShopItems.clear();
    mShopItems.clear();
}

ShopItem *ShopItems::findItem(const int id,
                              const ItemColor color) const
{
    STD_VECTOR<ShopItem*>::const_iterator it = mShopItems.begin();
    const STD_VECTOR<ShopItem*>::const_iterator e = mShopItems.end();
    while (it != e)
    {
        ShopItem *const item = *it;
        if (item->getId() == id && item->getColor() == color)
            return item;

        ++it;
    }

    return nullptr;
}

void ShopItems::updateList()
{
    mShopItems.clear();
    FOR_EACH (STD_VECTOR<ShopItem*>::iterator, it, mAllShopItems)
    {
        ShopItem *const item = *it;
        if ((item != nullptr) && item->isVisible())
            mShopItems.push_back(item);
    }
}
