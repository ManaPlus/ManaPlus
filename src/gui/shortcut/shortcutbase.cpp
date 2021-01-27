/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2021  Andrei Karas
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
    mMaxSize(maxSize),
    mItemSelected(-1),
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

    for (size_t i = 0; i < mMaxSize; i++)
    {
        const std::string num = toString(CAST_S32(i));
        const int itemId = cfg->getValue(mItemName + num, -1);
        const ItemColor itemColor = fromInt(
            cfg->getValue(mColorName + num, -1),
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
    for (size_t i = 0; i < mMaxSize; i++)
    {
        const int itemId = mItems[i] != 0 ? mItems[i] : -1;
        const std::string num = toString(CAST_S32(i));
        if (itemId != -1)
        {
            const int itemColor = (mItemColors[i] != ItemColor_zero)
                ? toInt(mItemColors[i], int) : 1;
            serverConfig.setValue(mItemName + num, itemId);
            serverConfig.setValue(mColorName + num, itemColor);
        }
        else
        {
            serverConfig.deleteKey(mItemName + num);
            serverConfig.deleteKey(mColorName + num);
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

void ShortcutBase::setItem(const size_t index)
{
    if (index >= mMaxSize)
        return;

    mItems[index] = mItemSelected;
    mItemColors[index] = mItemColorSelected;
    save();
}

void ShortcutBase::clear(const bool isSave)
{
    for (size_t i = 0; i < mMaxSize; i++)
    {
        mItems[i] = -1;
        mItemColors[i] = ItemColor_one;
    }
    if (isSave)
        save();
}
