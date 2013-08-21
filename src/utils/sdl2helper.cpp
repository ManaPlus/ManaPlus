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

#ifdef USE_SDL2

#include "utils/sdl2helper.h"

#include "logger.h"

#include "utils/stringutils.h"

#include <SDL_video.h>

#include "debug.h"

bool SDL::getAllVideoModes(StringVect &modeList)
{
    // +++ need use SDL_GetDisplayMode and SDL_GetNumDisplayModes
    modeList.push_back("800x600");
    return true;
}

#endif  // USE_SDL2
