/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "inventory.h"
#include "item.h"
#include "settings.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "enums/net/packettypes.h"

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
    if (!localPlayer)
        return;

    if (!PacketLimiter::limitPackets(PACKET_DROP))
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
            const int cnt = settings.quickDropCounter;
            if (localPlayer->isServerBuggy())
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
    if (!localPlayer)
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
            if (!PacketLimiter::limitPackets(PACKET_DROP))
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
