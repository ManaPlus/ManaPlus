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
    std::set<std::string> modes;

    for (int display = 0; display < 100; display ++)
    {
        const int numModes = SDL_GetNumDisplayModes(display);
        if (numModes > 0)
        {
            logger->log("Modes for display %d", display);
            for (int f = 0; f < numModes; f ++)
            {
                SDL_DisplayMode mode;
                SDL_GetDisplayMode(display, f, &mode);
                const int w = mode.w;
                const int h = mode.h;
                logger->log("%dx%dx%d", w, h, mode.refresh_rate);
                modes.insert(strprintf("%dx%d", w, h));
            }
        }
    }
    FOR_EACH (std::set<std::string>::const_iterator, it, modes)
        modeList.push_back(*it);
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

SDL_Thread *SDL::createThread(SDL_ThreadFunction fn,
                              const char *restrict const name,
                              void *restrict const data)
{
    return SDL_CreateThread(fn, name, data);
}

#endif  // USE_SDL2
