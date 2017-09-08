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

#ifndef UTILS_SDLHELPER_H
#define UTILS_SDLHELPER_H

#ifdef USE_SDL2
#include "utils/sdl2helper.h"
UTILS_SDL2HELPER_H

#else
#include "utils/stringvector.h"

#include "localconsts.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_thread.h>
PRAGMA48(GCC diagnostic pop)

union SDL_Event;

struct SDL_Surface;
struct SDL_SysWMinfo;

namespace SDL
{
    bool getAllVideoModes(StringVect &modeList);

    void SetWindowTitle(const SDL_Surface *const window,
                        const char *const title);

    void SetWindowIcon(const SDL_Surface *const window A_UNUSED,
                       SDL_Surface *const icon);

    void grabInput(const SDL_Surface *const window A_UNUSED, const bool grab);

    void setGamma(const SDL_Surface *const window A_UNUSED, const float gamma);

    void setVsync(const int val);

    bool getWindowWMInfo(const SDL_Surface *const window A_UNUSED,
                         SDL_SysWMinfo *const info);

    SDL_Thread *createThread(int (SDLCALL *fn)(void *),
                             const char *const name A_UNUSED,
                             void *const data);

    void *createGLContext(SDL_Surface *const window A_UNUSED,
                          const int major,
                          const int minor,
                          const int profile);

    void makeCurrentContext(void *const context);

    void initLogger();

    void WaitThread(SDL_Thread *const thread);

    bool PollEvent(SDL_Event *event);
}  // namespace SDL

#endif  // USE_SDL2
#endif  // UTILS_SDLHELPER_H
