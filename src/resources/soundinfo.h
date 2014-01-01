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

#ifndef RESOURCES_SOUNDINFO_H
#define RESOURCES_SOUNDINFO_H

#include <map>
#include <string>
#include <vector>

#include "localconsts.h"

struct SoundInfo final
{
    SoundInfo(const std::string &sound0, const int delay0) :
        sound(sound0),
        delay(delay0)
    {
    }

    std::string sound;
    int delay;
};

typedef std::vector<SoundInfo> SoundInfoVect;

enum ItemSoundEvent
{
    SOUND_EVENT_HIT = 0,
    SOUND_EVENT_MISS,
    SOUND_EVENT_HURT,
    SOUND_EVENT_DIE,
    SOUND_EVENT_MOVE,
    SOUND_EVENT_SIT,
    SOUND_EVENT_SITTOP,
    SOUND_EVENT_SPAWN,
    SOUND_EVENT_DROP,
    SOUND_EVENT_PICKUP,
    SOUND_EVENT_TAKE,   // take from container
    SOUND_EVENT_PUT,    // put into container
    SOUND_EVENT_EQUIP,
    SOUND_EVENT_UNEQUIP,
    SOUND_EVENT_USE
};

typedef std::map<ItemSoundEvent, SoundInfoVect*> ItemSoundEvents;

#endif  // RESOURCES_SOUNDINFO_H
