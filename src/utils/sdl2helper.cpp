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

#include <SDL_syswm.h>
#include <SDL_video.h>

#include "debug.h"

bool SDL::getAllVideoModes(StringVect &modeList)
{
    // +++ need use SDL_GetDisplayMode and SDL_GetNumDisplayModes
    modeList.push_back("800x600");
    return true;
}

void SDL::SetWindowTitle(SDL_Window *const window, const char *const title)
{
    SDL_SetWindowTitle(window, title);
}

void SDL::SetWindowIcon(SDL_Window *const window, SDL_Surface *const icon)
{
    SDL_SetWindowIcon(window, icon);
}

void SDL::grabInput(SDL_Window *const window, const bool grab)
{
    SDL_SetWindowGrab(window, grab ? SDL_TRUE : SDL_FALSE);
}

void SDL::setGamma(SDL_Window *const window, const float gamma)
{
    SDL_SetWindowBrightness(window, gamma);
}

void SDL::setVsync(const int val)
{
    SDL_GL_SetSwapInterval(val);
}

bool SDL::getWindowWMInfo(SDL_Window *const window, SDL_SysWMinfo *const info)
{
    return SDL_GetWindowWMInfo(window, info);
}

#endif  // USE_SDL2
