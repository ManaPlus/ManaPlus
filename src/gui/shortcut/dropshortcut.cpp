/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2022  Andrei Karas
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

#include "gui/shortcut/dropshortcut.h"

#include "settings.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "resources/inventory/inventory.h"

#include "resources/item/item.h"

#include "net/packetlimiter.h"

#include "debug.h"

static const int DROP_SHORTCUT_ITEMS = 16;

DropShortcut *dropShortcut = nullptr;

DropShortcut::DropShortcut() :
    ShortcutBase("drop", "dropColor", DROP_SHORTCUT_ITEMS),
    mLastDropIndex(0)
{
    clear(false);
    load();
}

DropShortcut::~DropShortcut()
{
}

void DropShortcut::dropFirst() const
{
    if (localPlayer == nullptr)
        return;

    if (!PacketLimiter::limitPackets(PacketType::PACKET_DROP))
        return;

    const int itemId = getItem(0);
    const ItemColor itemColor = getItemColor(0);
    if (PlayerInfo::isItemProtected(itemId))
        return;

    if (itemId > 0)
    {
        const Item *const item = PlayerInfo::getInventory()
            ->findItem(itemId, itemColor);
        if ((item != nullptr) && (item->getQuantity() != 0))
        {
            const int cnt = settings.quickDropCounter;
            if (localPlayer->isServerBuggy())
            {
                PlayerInfo::dropItem(item, cnt, Sfx_true);
            }
            else
            {
                for (int i = 0; i < cnt; i++)
                    PlayerInfo::dropItem(item, 1, Sfx_false);
            }
        }
    }
}

void DropShortcut::dropItems(const int cnt)
{
    if (localPlayer == nullptr)
        return;

    if (localPlayer->isServerBuggy())
    {
        dropItem(settings.quickDropCounter);
        return;
    }

    int n = 0;
    const int sz = settings.quickDropCounter;
    for (int f = 0; f < 9; f++)
    {
        for (int i = 0; i < sz; i++)
        {
            if (!PacketLimiter::limitPackets(PacketType::PACKET_DROP))
                return;
            if (dropItem(1))
                n++;
        }
        if (n >= cnt)
            break;
    }
}

bool DropShortcut::dropItem(const int cnt)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv == nullptr)
        return false;

    int itemId = 0;
    ItemColor itemColor = ItemColor_one;
    while (mLastDropIndex < DROP_SHORTCUT_ITEMS &&
           itemId < 1)
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
        if ((item != nullptr) &&
            item->getQuantity() > 0)
        {
            PlayerInfo::dropItem(item, cnt, Sfx_true);
            return true;
        }
    }
    if (mLastDropIndex >= DROP_SHORTCUT_ITEMS)
        mLastDropIndex = 0;

    if (itemId < 1)
    {
        while (mLastDropIndex < DROP_SHORTCUT_ITEMS &&
               itemId < 1)
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
            if ((item != nullptr) && item->getQuantity() > 0)
            {
                PlayerInfo::dropItem(item, cnt, Sfx_true);
                return true;
            }
        }
        if (mLastDropIndex >= DROP_SHORTCUT_ITEMS)
            mLastDropIndex = 0;
    }
    return false;
}
