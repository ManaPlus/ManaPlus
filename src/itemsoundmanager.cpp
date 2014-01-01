/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#include "itemsoundmanager.h"

#include "flooritem.h"
#include "item.h"
#include "soundmanager.h"

#include "being/localplayer.h"

#include "resources/iteminfo.h"

#include "debug.h"

void ItemSoundManager::playSfx(const Item *const item,
                               const ItemSoundEvent sound)
{
    if (!item)
        return;
    playSfx(item->getId(), sound);
}

void ItemSoundManager::playSfx(const int itemId,
                               const ItemSoundEvent sound)
{
    const ItemInfo &info = ItemDB::get(itemId);
    std::string sfx = info.getSound(sound).sound;
    if (sfx.empty())
    {
        // fallback to player race sound if no item sound.
        const int id = -100 - player_node->getSubType();
        const ItemInfo &info2 = ItemDB::get(id);
        sfx = info2.getSound(sound).sound;
    }
    soundManager.playGuiSfx(sfx);
}

void ItemSoundManager::playSfx(const FloorItem *const item,
                               const ItemSoundEvent sound)
{
    if (!item)
        return;
    const ItemInfo &info = ItemDB::get(item->getItemId());
    std::string sfx = info.getSound(sound).sound;
    if (sfx.empty())
    {
        // fallback to player race sound if no item sound.
        const int id = -100 - player_node->getSubType();
        const ItemInfo &info2 = ItemDB::get(id);
        sfx = info2.getSound(sound).sound;
    }
    soundManager.playGuiSfx(sfx);
}
