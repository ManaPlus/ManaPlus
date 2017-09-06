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

#ifdef USE_SDL2

#include "utils/sdl2helper.h"

#include "logger.h"

#include "utils/foreach.h"
#include "utils/stringutils.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_syswm.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

bool SDL::getAllVideoModes(StringVect &modeList)
{
    std::set<std::string> modes;
    const int numDisplays = SDL_GetNumVideoDisplays();
    for (int display = 0; display < numDisplays; display ++)
    {
        const int numModes = SDL_GetNumDisplayModes(display);
        if (numModes > 0)
        {
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

void *SDL::createGLContext(SDL_Window *const window,
                           const int major,
                           const int minor,
                           const int profile)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_ClearError();
    void *context = SDL_GL_CreateContext(window);
    if (context == nullptr)
    {
        logger->log("Error to switch to context %d.%d: %s",
            major,
            minor,
            SDL_GetError());
    }
    if (context == nullptr && (major > 3 || (major == 3 && minor > 3)))
    {
        logger->log("Try fallback to OpenGL 3.3 context");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_ClearError();
        context = SDL_GL_CreateContext(window);
        if (context == nullptr)
        {
            logger->log("Error to switch to context 3.3: %s",
                SDL_GetError());
        }
        if (context == nullptr && profile == 0x01)
        {
            logger->log("Try fallback to OpenGL 3.3 compatibility context");
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0x02);
            SDL_ClearError();
            context = SDL_GL_CreateContext(window);
            if (context == nullptr)
            {
                logger->log("Error to switch to compatibility context 3.3: %s",
                    SDL_GetError());
            }
        }
    }
    if (context == nullptr && (major > 3 || (major == 3 && minor > 0)))
    {
        logger->log("Error to switch to core context %d.%d: %s",
            major,
            minor,
            SDL_GetError());
        logger->log("Try fallback to OpenGL 3.0 core context");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);
        context = SDL_GL_CreateContext(window);
        if (context == nullptr)
        {
            logger->log("Error to switch to core context 3.0: %s",
                SDL_GetError());
        }
    }
    if (context == nullptr && (major > 2 || (major == 2 && minor > 1)))
    {
        logger->log("Try fallback to OpenGL 2.1 compatibility context");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0x02);
        context = SDL_GL_CreateContext(window);
        if (context == nullptr)
        {
            logger->log("Error to switch to compatibility context 2.1: %s",
                SDL_GetError());
        }
    }
    if (context == nullptr)
    {
        logger->log("Cant find working context.");
    }
    return context;
}

void SDL::makeCurrentContext(void *const context A_UNUSED)
{
}

#endif  // USE_SDL2
