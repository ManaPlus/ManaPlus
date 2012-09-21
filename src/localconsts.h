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
#else
#undef Z_NULL
#define Z_NULL nullptr
#endif

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#  ifdef __clang__
#  define gnu_printf printf
#  endif
#else
#define A_UNUSED
#define gnu_printf printf
#endif


//#define DEBUG_CONFIG 1
//#define DEBUG_BIND_TEXTURE 1
//#define DISABLE_RESOURCE_CACHING 1
#define DUMP_LEAKED_RESOURCES 1
