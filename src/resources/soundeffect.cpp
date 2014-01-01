/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "resources/soundeffect.h"

#include "logger.h"

#include "debug.h"

SoundEffect::~SoundEffect()
{
    Mix_FreeChunk(mChunk);
}

Resource *SoundEffect::load(SDL_RWops *const rw)
{
    if (!rw)
        return nullptr;
    // Load the music data and free the RWops structure
    Mix_Chunk *const tmpSoundEffect = Mix_LoadWAV_RW(rw, 1);

    if (tmpSoundEffect)
    {
        return new SoundEffect(tmpSoundEffect);
    }
    else
    {
        logger->log("Error, failed to load sound effect: %s", Mix_GetError());
        return nullptr;
    }
}

bool SoundEffect::play(const int loops, const int volume,
                       const int channel) const
{
    Mix_VolumeChunk(mChunk, volume);

    return Mix_PlayChannel(channel, mChunk, loops) != -1;
}
