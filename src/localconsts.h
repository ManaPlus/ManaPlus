/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#if !defined(__GXX_EXPERIMENTAL_CXX0X__)
#undef nullptr
#define nullptr 0
#define final
#define override
#define constexpr
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
//#define A_DELETE
//#define A_DELETE_COPY
#endif
#undef Z_NULL
#define Z_NULL nullptr
#define A_DELETE(func) func = delete
#define A_DELETE_COPY(name) name(const name &) = delete; \
    name &operator=(const name&) = delete;
#endif

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#define A_WARN_UNUSED __attribute__ ((warn_unused_result))
#else
#define A_UNUSED
#define A_WARN_UNUSED
#define gnu_printf printf
#endif
#ifdef __clang__
#define gnu_printf printf
#endif

//#define DEBUG_CONFIG 1
//#define DEBUG_BIND_TEXTURE 1
//#define DISABLE_RESOURCE_CACHING 1
#define DUMP_LEAKED_RESOURCES 1
//#define DEBUG_DUMP_LEAKS1 1

//Android logging
#define ANDROID_LOG 1

//profiler
//#define USE_PROFILER 1

#include "utils/perfomance.h"
