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

#ifndef USE_SDL2

#include "utils/sdlhelper.h"

#include "logger.h"

#include "utils/stringutils.h"

#include <SDL_video.h>

#include "debug.h"

bool SDL::getAllVideoModes(StringVect &modeList)
{
    /* Get available fullscreen/hardware modes */
    SDL_Rect **const modes = SDL_ListModes(nullptr,
        SDL_FULLSCREEN | SDL_HWSURFACE);

    /* Check which modes are available */
    if (modes == static_cast<SDL_Rect **>(nullptr))
    {
        logger->log1("No modes available");
        return false;
    }
    else if (modes == reinterpret_cast<SDL_Rect **>(-1))
    {
        logger->log1("All resolutions available");
        return true;
    }
    else
    {
        for (int i = 0; modes[i]; ++ i)
        {
            const std::string modeString =
                toString(static_cast<int>(modes[i]->w)).append("x")
                .append(toString(static_cast<int>(modes[i]->h)));
            logger->log("support mode: " + modeString);
            modeList.push_back(modeString);
        }
        return true;
    }
}

void SDL::SetWindowTitle(SDL_Surface *const window A_UNUSED,
                         const char *const title)
{
    SDL_WM_SetCaption(title, nullptr);
}

void SDL::SetWindowIcon(SDL_Surface *const window A_UNUSED,
                        SDL_Surface *const icon)
{
    SDL_WM_SetIcon(icon, nullptr);
}

void SDL::grabInput(SDL_Surface *const window A_UNUSED, const bool grab)
{
    SDL_WM_GrabInput(grab ? SDL_GRAB_ON : SDL_GRAB_OFF);
}

#endif  // USE_SDL2
