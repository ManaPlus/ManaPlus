/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#ifndef USE_SDL2

#include "utils/sdlmusichelper.h"

#include "debug.h"

int SDL::MixOpenAudio(const int frequency,
                      const uint16_t format,
                      const int nchannels,
                      const int chunksize)
{
    return Mix_OpenAudio(frequency,
        format,
        nchannels,
        chunksize);
}

Mix_Music *SDL::LoadMUSOgg_RW(SDL_RWops *const rw)
{
    return Mix_LoadMUS_RW(rw);
}

#endif  // USE_SDL2
