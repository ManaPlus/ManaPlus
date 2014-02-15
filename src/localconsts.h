/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include <SDL_version.h>

#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef USE_SDL2
#warning using SDL2 headers but configure set to use SDL1.2
#warning please add configure flag --with-sdl2
#define USE_SDL2
#endif
#else
#ifdef USE_SDL2
#error using SDL1.2 headers but configure set to use SDL2
#error please remove configure flag --with-sdl2
#endif
#endif

#if !defined(__GXX_EXPERIMENTAL_CXX0X__)
#undef nullptr
#define nullptr 0
#define final
#define override
#define constexpr
#define noexcept
#define A_DELETE(func)
#define A_DELETE_COPY(func)
#else
#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 \
    + __GNUC_PATCHLEVEL__)
#if GCC_VERSION < 40700
#define final
#define override
#define constexpr
#define noexcept
// #define A_DELETE
// #define A_DELETE_COPY
#endif
#undef Z_NULL
#define Z_NULL nullptr
#define M_TCPOK
#define A_DELETE(func) func = delete
#define A_DELETE_COPY(name) name(const name &) = delete; \
    name &operator=(const name&) = delete;
#endif

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#define A_WARN_UNUSED __attribute__ ((warn_unused_result))
#define DEPRECATED __attribute__ ((deprecated))
#define restrict __restrict__
#else
#define A_UNUSED
#define A_WARN_UNUSED
#define gnu_printf printf
#define DEPRECATED
#define restrict
#endif
#ifdef __clang__
#define gnu_printf printf
#endif

#define FOR_EACH(type, iter, array) for (type iter = array.begin(), \
    iter##_end = array.end(); iter != iter##_end; ++ iter)

#define FOR_EACHP(type, iter, array) for (type iter = array->begin(), \
    iter##_end = array->end(); iter != iter##_end; ++ iter)

// #define DEBUG_CONFIG 1
// #define DEBUG_BIND_TEXTURE 1
// #define DISABLE_RESOURCE_CACHING 1
#define DUMP_LEAKED_RESOURCES 1
// #define DEBUG_DUMP_LEAKS1 1
// #define DEBUG_SDLFONT 1

// Android logging
// #define ANDROID_LOG 1

// profiler
// #define USE_PROFILER 1

// draw calls
// #define DEBUG_DRAW_CALLS 1

// debug images usage
// #define DEBUG_IMAGES 1

// debug SDL surfaces
// #define DEBUG_SDL_SURFACES 1

// debug RWops usage
// #define DEBUG_PHYSFS 1

// use file access fuzzer
// #define USE_FUZZER 1

#include "utils/perfomance.h"
