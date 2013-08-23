/*
 *  The ManaPlus Client
 *  Copyright (C) 2013  The ManaPlus Developers
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

#ifndef UTILS_SDLHELPER_H
#define UTILS_SDLHELPER_H

#ifdef USE_SDL2
#include "utils/sdl2helper.h"

#else
#include "utils/stringvector.h"

#include "localconsts.h"

struct SDL_Surface;

namespace SDL
{
    bool getAllVideoModes(StringVect &modeList);

    void SetWindowTitle(SDL_Surface *const window,
                        const char *const title);

    void SetWindowIcon(SDL_Surface *const window A_UNUSED,
                       SDL_Surface *const icon);

}  // namespace SDL

#endif  // USE_SDL2
#endif  // UTILS_SDLHELPER_H
