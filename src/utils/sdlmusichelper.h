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

#ifndef UTILS_SDLMUSICHELPER_H
#define UTILS_SDLMUSICHELPER_H

#ifdef USE_SDL2
#include "utils/sdl2musichelper.h"
UTILS_SDL2MUSICHELPER_H

#else

#include "localconsts.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_mixer.h>
PRAGMA48(GCC diagnostic pop)

namespace SDL
{
    int MixOpenAudio(const int frequency,
                     const uint16_t format,
                     const int nchannels,
                     const int chunksize);

    Mix_Music *LoadMUSOgg_RW(SDL_RWops *const rw);
}  // namespace SDL

#endif  // USE_SDL2
#endif  // UTILS_SDLMUSICHELPER_H
