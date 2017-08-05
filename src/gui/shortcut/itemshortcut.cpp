/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#include "gui/shortcut/itemshortcut.h"

#include "configuration.h"
#include "spellmanager.h"

#include "being/playerinfo.h"

#include "const/spells.h"

#include "const/resources/skill.h"

#include "gui/windows/skilldialog.h"

#include "resources/inventory/inventory.h"

#include "resources/item/item.h"

#include "debug.h"

ItemShortcut *itemShortcut[SHORTCUT_TABS];

ItemShortcut::ItemShortcut(const size_t number) :
    mItems(),
    mItemColors(),
    mItemData(),
    mNumber(number),
    mItemSelected(-1),
    mItemColorSelected(ItemColor_one)
{
    load();
}

ItemShortcut::~ItemShortcut()
{
    logger->log1("ItemShortcut::~ItemShortcut");
}

void ItemShortcut::load()
{
    std::string name;
    std::string color;
    std::string data;
    if (mNumber == SHORTCUT_AUTO_TAB)
        return;

    const Configuration *cfg = &serverConfig;
    if (mNumber != 0)
    {
        name = std::string("shortcut").append(
            toString(CAST_S32(mNumber))).append("_");
        color = std::string("shortcutColor").append(
            toString(CAST_U32(mNumber))).append("_");
        data = std::string("shortcutData").append(
            toString(CAST_U32(mNumber))).append("_");
    }
    else
    {
        name = "shortcut";
        color = "shortcutColor";
        data = "shortcutData";
    }
    for (unsigned int i = 0; i < SHORTCUT_ITEMS; i++)
    {
        const int itemId = cfg->getValue(name + toString(i), -1);
        const ItemColor itemColor = fromInt(
            cfg->getValue(color + toString(i), 1),
            ItemColor);

        mItems[i] = itemId;
        mItemColors[i] = itemColor;
        mItemData[i] = cfg->getValue(data + toString(i), std::string());
    }
}

void ItemShortcut::save() const
{
    std::string name;
    std::string color;
    std::string data;
    if (mNumber == SHORTCUT_AUTO_TAB)
        return;
    if (mNumber != 0)
    {
        name = std::string("shortcut").append(
            toString(CAST_S32(mNumber))).append("_");
        color = std::string("shortcutColor").append(
            toString(CAST_U32(mNumber))).append("_");
        data = std::string("shortcutData").append(
            toString(CAST_U32(mNumber))).append("_");
    }
    else
    {
        name = "shortcut";
        color = "shortcutColor";
        data = "shortcutData";
    }

    for (unsigned int i = 0; i < SHORTCUT_ITEMS; i++)
    {
        const int itemId = mItems[i] != 0 ? mItems[i] : -1;
        const int itemColor = toInt(mItemColors[i], int);
        const std::string itemData = mItemData[i];
        if (itemId != -1)
        {
            serverConfig.setValue(name + toString(i), itemId);
            serverConfig.setValue(color + toString(i), itemColor);
            serverConfig.setValue(data + toString(i), itemData);
        }
        else
        {
            serverConfig.deleteKey(name + toString(i));
            serverConfig.deleteKey(color + toString(i));
            serverConfig.deleteKey(data + toString(i));
        }
    }
}

void ItemShortcut::clear()
{
    for (size_t i = 0; i < SHORTCUT_ITEMS; i++)
    {
        mItems[i] = 0;
        mItemColors[i] = ItemColor_zero;
        mItemData[i].clear();
    }
}

void ItemShortcut::useItem(const int index) const
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return;

    const int itemId = mItems[index];
    const ItemColor itemColor = mItemColors[index];
    if (itemId >= 0)
    {
        if (itemId < SPELL_MIN_ID)
        {
            const Item *const item = inv->findItem(itemId, itemColor);
            if ((item != nullptr) && (item->getQuantity() != 0))
                PlayerInfo::useEquipItem(item, Sfx_true);
        }
        else if (itemId < SKILL_MIN_ID && (spellManager != nullptr))
        {
            spellManager->useItem(itemId);
        }
        else if (skillDialog != nullptr)
        {
            skillDialog->useItem(itemId,
                fromBool(config.getBoolValue("skillAutotarget"), AutoTarget),
                toInt(itemColor, int),
                mItemData[index]);
        }
    }
}

void ItemShortcut::equipItem(const int index) const
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return;

    const int itemId = mItems[index];
    if (itemId != 0)
    {
        const Item *const item = inv->findItem(itemId, mItemColors[index]);
        if ((item != nullptr) && (item->getQuantity() != 0))
        {
            if (item->isEquipment() == Equipm_true)
            {
                if (item->isEquipped() == Equipped_false)
                    PlayerInfo::equipItem(item, Sfx_true);
            }
        }
    }
}
void ItemShortcut::unequipItem(const int index) const
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return;

    const int itemId = mItems[index];
    if (itemId != 0)
    {
        const Item *const item = inv->findItem(itemId, mItemColors[index]);
        if ((item != nullptr) && (item->getQuantity() != 0))
        {
            if (item->isEquipment() == Equipm_true)
            {
                if (item->isEquipped() == Equipped_true)
                    PlayerInfo::unequipItem(item, Sfx_true);
            }
        }
    }
}

void ItemShortcut::setItemSelected(const Item *const item)
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

void ItemShortcut::setItem(const int index)
{
    mItems[index] = mItemSelected;
    mItemColors[index] = mItemColorSelected;
    save();
}

void ItemShortcut::setItem(const int index,
                           const int item,
                           const ItemColor color)
{
    mItems[index] = item;
    mItemColors[index] = color;
    save();
}

void ItemShortcut::setItemFast(const size_t index,
                               const int item,
                               const ItemColor color)
{
    mItems[index] = item;
    mItemColors[index] = color;
}

void ItemShortcut::swap(const int index1, const int index2)
{
    if (index1 < 0 || index2 < 0
        || CAST_U32(index1) >= SHORTCUT_ITEMS
        || CAST_U32(index2) >= SHORTCUT_ITEMS)
    {
        return;
    }

    const int tmpItem = mItems[index1];
    mItems[index1] = mItems[index2];
    mItems[index2] = tmpItem;
    const ItemColor tmpColor = mItemColors[index1];
    mItemColors[index1] = mItemColors[index2];
    mItemColors[index2] = tmpColor;

    const std::string tmpData = mItemData[index1];
    mItemData[index1] = mItemData[index2];
    mItemData[index2] = tmpData;
    save();
}
