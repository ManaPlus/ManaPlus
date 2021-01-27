/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "debug.h"

SoundEffect::~SoundEffect()
{
    Mix_FreeChunk(mChunk);
}

bool SoundEffect::play(const int loops, const int volume,
                       const int channel) const
{
    Mix_VolumeChunk(mChunk, volume);

    return Mix_PlayChannelTimed(channel, mChunk, loops, -1) != -1;
}

int SoundEffect::calcMemoryLocal() const
{
    return static_cast<int>(sizeof(SoundEffect) +
        sizeof(SDL_AudioSpec)) +
        Resource::calcMemoryLocal();
}
