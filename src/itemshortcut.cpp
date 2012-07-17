/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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
#include "playerinfo.h"
#include "spellmanager.h"

#include "gui/skilldialog.h"

#include "net/inventoryhandler.h"
#include "net/net.h"

#include "utils/stringutils.h"

#include "debug.h"

ItemShortcut *itemShortcut[SHORTCUT_TABS];

ItemShortcut::ItemShortcut(int number):
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

void ItemShortcut::load(bool oldConfig)
{
    std::string name;
    std::string color;
    Configuration *cfg;
    if (oldConfig)
        cfg = &config;
    else
        cfg = &serverConfig;

    if (mNumber)
    {
        name = "shortcut" + toString(mNumber) + "_";
        color = "shortcutColor" + toString(mNumber) + "_";
    }
    else
    {
        name = "shortcut";
        color = "shortcutColor";
    }
    for (int i = 0; i < SHORTCUT_ITEMS; i++)
    {
        int itemId = cfg->getValue(name + toString(i), -1);
        unsigned char itemColor = cfg->getValue(color + toString(i), 1);

        mItems[i] = itemId;
        mItemColors[i] = itemColor;
    }
}

void ItemShortcut::save()
{
    std::string name;
    std::string color;
    if (mNumber)
    {
        name = "shortcut" + toString(mNumber) + "_";
        color = "shortcutColor" + toString(mNumber) + "_";
    }
    else
    {
        name = "shortcut";
        color = "shortcutColor";
    }

    logger->log("save %s", name.c_str());

    for (int i = 0; i < SHORTCUT_ITEMS; i++)
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

void ItemShortcut::useItem(int index)
{
    if (!PlayerInfo::getInventory())
        return;

    int itemId = mItems[index];
    unsigned char itemColor = mItemColors[index];
    if (itemId >= 0)
    {
        if (itemId < SPELL_MIN_ID)
        {
            Item *item = PlayerInfo::getInventory()->findItem(
                itemId, itemColor);
            if (item && item->getQuantity())
            {
                if (item->isEquipment())
                {
                    if (item->isEquipped())
                        Net::getInventoryHandler()->unequipItem(item);
                    else
                        Net::getInventoryHandler()->equipItem(item);
                }
                else
                {
                    Net::getInventoryHandler()->useItem(item);
                }
            }
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

void ItemShortcut::equipItem(int index)
{
    if (!PlayerInfo::getInventory())
        return;

    if (mItems[index])
    {
        Item *item = PlayerInfo::getInventory()->findItem(
            mItems[index], mItemColors[index]);
        if (item && item->getQuantity())
        {
            if (item->isEquipment())
            {
                if (!item->isEquipped())
                    Net::getInventoryHandler()->equipItem(item);
            }
        }
    }
}
void ItemShortcut::unequipItem(int index)
{
    if (!PlayerInfo::getInventory())
        return;

    if (mItems[index])
    {
        Item *item = PlayerInfo::getInventory()->findItem(
            mItems[index], mItemColors[index]);
        if (item && item->getQuantity())
        {
            if (item->isEquipment())
            {
                if (item->isEquipped())
                    Net::getInventoryHandler()->unequipItem(item);
            }
        }
    }
}

void ItemShortcut::setItemSelected(Item *item)
{
    if (item)
    {
//        logger->log("set selected id: %d", item->getId());
//        logger->log("set selected color: %d", item->getColor());
        mItemSelected = item->getId();
        mItemColorSelected = item->getColor();
    }
    else
    {
        mItemSelected = -1;
        mItemColorSelected = 1;
    }
}

void ItemShortcut::setItem(int index)
{
    mItems[index] = mItemSelected;
    mItemColors[index] = mItemColorSelected;
    save();
}
