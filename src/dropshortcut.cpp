/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "dropshortcut.h"

#include "client.h"
#include "configuration.h"
#include "inventory.h"
#include "item.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "debug.h"

DropShortcut *dropShortcut = nullptr;

DropShortcut::DropShortcut() :
    mItemSelected(-1),
    mItemColorSelected(1),
    mLastDropIndex(0)
{
    clear(false);
    load();
}

DropShortcut::~DropShortcut()
{
//    save();
}

void DropShortcut::load(const bool oldConfig)
{
    const Configuration *cfg;
    if (oldConfig)
        cfg = &config;
    else
        cfg = &serverConfig;

    for (int i = 0; i < DROP_SHORTCUT_ITEMS; i++)
    {
        const int itemId = cfg->getValue("drop" + toString(i), -1);
        const unsigned char itemColor = static_cast<const unsigned char>(
            cfg->getValue("dropColor" + toString(i), -1));

        if (itemId != -1)
        {
            mItems[i] = itemId;
            mItemColors[i] = itemColor;
        }
    }
}

void DropShortcut::save() const
{
    for (int i = 0; i < DROP_SHORTCUT_ITEMS; i++)
    {
        const int itemId = mItems[i] ? mItems[i] : -1;
        const int itemColor = mItemColors[i] ? mItemColors[i] : 1;
        if (itemId != -1)
        {
            serverConfig.setValue("drop" + toString(i), itemId);
            serverConfig.setValue("dropColor" + toString(i), itemColor);
        }
        else
        {
            serverConfig.deleteKey("drop" + toString(i));
            serverConfig.deleteKey("dropColor" + toString(i));
        }
    }
}

void DropShortcut::dropFirst() const
{
    if (!player_node)
        return;

    if (!client->limitPackets(PACKET_DROP))
        return;

    const int itemId = getItem(0);
    const unsigned char itemColor = getItemColor(0);
    if (PlayerInfo::isItemProtected(itemId))
        return;

    if (itemId > 0)
    {
        const Item *const item = PlayerInfo::getInventory()
            ->findItem(itemId, itemColor);
        if (item && item->getQuantity())
        {
            const int cnt = player_node->getQuickDropCounter();
            if (player_node->isServerBuggy())
            {
                PlayerInfo::dropItem(item, cnt, true);
            }
            else
            {
                for (int i = 0; i < cnt; i++)
                    PlayerInfo::dropItem(item, 1, false);
            }
        }
    }
}

void DropShortcut::dropItems(const int cnt)
{
    if (!player_node)
        return;

    if (player_node->isServerBuggy())
    {
        dropItem(player_node->getQuickDropCounter());
        return;
    }

    int n = 0;
    const int sz = player_node->getQuickDropCounter();
    for (int f = 0; f < 9; f++)
    {
        for (int i = 0; i < sz; i++)
        {
            if (!client->limitPackets(PACKET_DROP))
                return;
            if (dropItem())
                n++;
        }
        if (n >= cnt)
            break;
    }
}

bool DropShortcut::dropItem(const int cnt)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (!inv)
        return false;

    int itemId = 0;
    unsigned char itemColor = 1;
    while (mLastDropIndex < DROP_SHORTCUT_ITEMS && itemId < 1)
    {
        if (!PlayerInfo::isItemProtected(itemId))
        {
            itemId = getItem(mLastDropIndex);
            itemColor = getItemColor(mLastDropIndex);
        }
        mLastDropIndex ++;
    }

    if (itemId > 0)
    {
        const Item *const item = inv->findItem(itemId, itemColor);
        if (item && item->getQuantity() > 0)
        {
            PlayerInfo::dropItem(item, cnt, true);
            return true;
        }
    }
    if (mLastDropIndex >= DROP_SHORTCUT_ITEMS)
        mLastDropIndex = 0;

    if (itemId < 1)
    {
        while (mLastDropIndex < DROP_SHORTCUT_ITEMS && itemId < 1)
        {
            if (!PlayerInfo::isItemProtected(itemId))
            {
                itemId = getItem(mLastDropIndex);
                itemColor = getItemColor(mLastDropIndex);
            }
            mLastDropIndex++;
        }
        if (itemId > 0)
        {
            const Item *const item = inv->findItem(itemId, itemColor);
            if (item && item->getQuantity() > 0)
            {
                PlayerInfo::dropItem(item, cnt, true);
                return true;
            }
        }
        if (mLastDropIndex >= DROP_SHORTCUT_ITEMS)
            mLastDropIndex = 0;
    }
    return false;
}

void DropShortcut::setItemSelected(const Item *const item)
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

void DropShortcut::setItem(const int index)
{
    if (index < 0 || index >= DROP_SHORTCUT_ITEMS)
        return;

    mItems[index] = mItemSelected;
    mItemColors[index] = mItemColorSelected;
    save();
}

void DropShortcut::clear(const bool isSave)
{
    for (int i = 0; i < DROP_SHORTCUT_ITEMS; i++)
    {
        mItems[i] = -1;
        mItemColors[i] = 1;
    }
    if (isSave)
        save();
}
