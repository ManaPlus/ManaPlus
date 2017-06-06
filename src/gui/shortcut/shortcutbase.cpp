/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/shortcut/shortcutbase.h"

#include "configuration.h"

#include "resources/item/item.h"

#include "debug.h"

ShortcutBase::ShortcutBase(const std::string &itemName,
                           const std::string &colorName,
                           const int maxSize) :
    mItems(new int[maxSize]),
    mItemColors(new ItemColor[maxSize]),
    mItemName(itemName),
    mColorName(colorName),
    mItemSelected(-1),
    mMaxSize(maxSize),
    mItemColorSelected(ItemColor_one)
{
    clear(false);
    load();
}

ShortcutBase::~ShortcutBase()
{
    delete [] mItems;
    mItems = nullptr;
    delete [] mItemColors;
    mItemColors = nullptr;
}

void ShortcutBase::load()
{
    const Configuration *cfg = &serverConfig;

    for (int i = 0; i < mMaxSize; i++)
    {
        const int itemId = cfg->getValue(mItemName + toString(i), -1);
        const ItemColor itemColor = fromInt(
            cfg->getValue(mColorName + toString(i), -1),
            ItemColor);

        if (itemId != -1)
        {
            mItems[i] = itemId;
            mItemColors[i] = itemColor;
        }
    }
}

void ShortcutBase::save() const
{
    for (int i = 0; i < mMaxSize; i++)
    {
        const int itemId = mItems[i] != 0 ? mItems[i] : -1;
        const int itemColor = (mItemColors[i] != ItemColor_zero)
            ? toInt(mItemColors[i], int) : 1;
        if (itemId != -1)
        {
            serverConfig.setValue(mItemName + toString(i), itemId);
            serverConfig.setValue(mColorName + toString(i), itemColor);
        }
        else
        {
            serverConfig.deleteKey(mItemName + toString(i));
            serverConfig.deleteKey(mColorName + toString(i));
        }
    }
}

void ShortcutBase::setItemSelected(const Item *const item)
{
    if (item != nullptr)
    {
        mItemSelected = item->getId();
        mItemColorSelected = item->getColor();
    }
    else
    {
        mItemSelected = -1;
        mItemColorSelected = ItemColor_one;
    }
}

void ShortcutBase::setItem(const int index)
{
    if (index < 0 || index >= mMaxSize)
        return;

    mItems[index] = mItemSelected;
    mItemColors[index] = mItemColorSelected;
    save();
}

void ShortcutBase::clear(const bool isSave)
{
    for (int i = 0; i < mMaxSize; i++)
    {
        mItems[i] = -1;
        mItemColors[i] = ItemColor_one;
    }
    if (isSave)
        save();
}
