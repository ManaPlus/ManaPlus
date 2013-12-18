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

#ifndef UTILS_SDL2HELPER_H
#define UTILS_SDL2HELPER_H

#ifdef USE_SDL2
#include "utils/stringvector.h"

#include <SDL_thread.h>

#include "localconsts.h"

struct SDL_Surface;
struct SDL_SysWMinfo;
struct SDL_Window;

namespace SDL
{
    bool getAllVideoModes(StringVect &modeList);

    void SetWindowTitle(SDL_Window *const window, const char *const title);

    void SetWindowIcon(SDL_Window *const window, SDL_Surface *const icon);

    void grabInput(SDL_Window *const window, const bool grab);

    void setGamma(SDL_Window *const window, const float gamma);

    void setVsync(const int val);

    bool getWindowWMInfo(SDL_Window *const window, SDL_SysWMinfo *const info);

    SDL_Thread *createThread(SDL_ThreadFunction fn,
                             const char *restrict const name,
                             void *restrict const data);
}  // namespace SDL

#endif  // USE_SDL2
#endif  // UTILS_SDL2HELPER_H
