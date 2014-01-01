/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#include "itemshortcut.h"

#include "configuration.h"
#include "inventory.h"
#include "item.h"
#include "logger.h"
#include "spellmanager.h"

#include "being/playerinfo.h"

#include "gui/windows/skilldialog.h"

#include "debug.h"

ItemShortcut *itemShortcut[SHORTCUT_TABS];

ItemShortcut::ItemShortcut(const int number):
    mItemSelected(-1),
    mItemColorSelected(1),
    mNumber(number)
{
    load();
}

ItemShortcut::~ItemShortcut()
{
    logger->log1("ItemShortcut::~ItemShortcut");
}

void ItemShortcut::load(const bool oldConfig)
{
    std::string name;
    std::string color;
    const Configuration *cfg;
    if (oldConfig)
        cfg = &config;
    else
        cfg = &serverConfig;

    if (mNumber)
    {
        name = std::string("shortcut").append(toString(mNumber)).append("_");
        color = std::string("shortcutColor").append(
            toString(mNumber)).append("_");
    }
    else
    {
        name = "shortcut";
        color = "shortcutColor";
    }
    for (unsigned int i = 0; i < SHORTCUT_ITEMS; i++)
    {
        const int itemId = cfg->getValue(name + toString(i), -1);
        const unsigned char itemColor = static_cast<const unsigned char>(
            cfg->getValue(color + toString(i), 1));

        mItems[i] = itemId;
        mItemColors[i] = itemColor;
    }
}

void ItemShortcut::save() const
{
    std::string name;
    std::string color;
    if (mNumber)
    {
        name = std::string("shortcut").append(toString(mNumber)).append("_");
        color = std::string("shortcutColor").append(
            toString(mNumber)).append("_");
    }
    else
    {
        name = "shortcut";
        color = "shortcutColor";
    }

    logger->log("save %s", name.c_str());

    for (unsigned int i = 0; i < SHORTCUT_ITEMS; i++)
    {
        const int itemId = mItems[i] ? mItems[i] : -1;
        const int itemColor = mItemColors[i] ? mItemColors[i] : 1;
        if (itemId != -1)
        {
            serverConfig.setValue(name + toString(i), itemId);
            serverConfig.setValue(color + toString(i), itemColor);
        }
        else
        {
            serverConfig.deleteKey(name + toString(i));
            serverConfig.deleteKey(color + toString(i));
        }
    }
}

void ItemShortcut::useItem(const int index) const
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    const int itemId = mItems[index];
    const unsigned char itemColor = mItemColors[index];
    if (itemId >= 0)
    {
        if (itemId < SPELL_MIN_ID)
        {
            const Item *const item = inv->findItem(itemId, itemColor);
            if (item && item->getQuantity())
                PlayerInfo::useEquipItem(item, true);
        }
        else if (itemId < SKILL_MIN_ID && spellManager)
        {
            spellManager->useItem(itemId);
        }
        else if (skillDialog)
        {
            skillDialog->useItem(itemId);
        }
    }
}

void ItemShortcut::equipItem(const int index) const
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    const int itemId = mItems[index];
    if (itemId)
    {
        const Item *const item = inv->findItem(itemId, mItemColors[index]);
        if (item && item->getQuantity())
        {
            if (item->isEquipment())
            {
                if (!item->isEquipped())
                    PlayerInfo::equipItem(item, true);
            }
        }
    }
}
void ItemShortcut::unequipItem(const int index) const
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    const int itemId = mItems[index];
    if (itemId)
    {
        const Item *const item = inv->findItem(itemId, mItemColors[index]);
        if (item && item->getQuantity())
        {
            if (item->isEquipment())
            {
                if (item->isEquipped())
                    PlayerInfo::unequipItem(item, true);
            }
        }
    }
}

void ItemShortcut::setItemSelected(const Item *const item)
{
    if (item)
    {
        mItemSelected = item->getId();
        mItemColorSelected = item->getColor();
    }
    else
    {
        mItemSelected = -1;
        mItemColorSelected = 1;
    }
}

void ItemShortcut::setItem(const int index)
{
    mItems[index] = mItemSelected;
    mItemColors[index] = mItemColorSelected;
    save();
}

void ItemShortcut::setItem(const int index, const int item,
                           const unsigned char color)
{
    mItems[index] = item;
    mItemColors[index] = color;
    save();
}

void ItemShortcut::swap(const int index1, const int index2)
{
    if (index1 < 0 || index2 < 0
        || static_cast<unsigned int>(index1) >= SHORTCUT_ITEMS
        || static_cast<unsigned int>(index2) >= SHORTCUT_ITEMS)
    {
        return;
    }

    const int tmpItem = mItems[index1];
    mItems[index1] = mItems[index2];
    mItems[index2] = tmpItem;
    const unsigned char tmpColor = mItemColors[index1];
    mItemColors[index1] = mItemColors[index2];
    mItemColors[index2] = tmpColor;
    save();
}
