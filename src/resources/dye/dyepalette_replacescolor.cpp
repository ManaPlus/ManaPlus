/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "resources/dye/dyepalette.h"

#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER

#ifdef SIMD_SUPPORTED
// avx2
#include <immintrin.h>
#endif  // SIMD_SUPPORTED

#include "debug.h"

void DyePalette::replaceSColor(uint32_t *restrict pixels,
                               const int bufSize) const restrict2
{
#ifdef SIMD_SUPPORTED
    replaceSColorSimd(pixels, bufSize);
#else  // SIMD_SUPPORTED
#include "resources/dye/dyepalette_replacescolor_default.hpp"
#endif  // SIMD_SUPPORTED
}

void DyePalette::replaceSColorDefault(uint32_t *restrict pixels,
                                      const int bufSize) const restrict2
{
#include "resources/dye/dyepalette_replacescolor_default.hpp"
}

#ifdef SIMD_SUPPORTED
/*
static void print256(const char *const text, const __m256i &val);
static void print256(const char *const text, const __m256i &val)
{
    printf("%s 0x%016llx%016llx%016llx%016llx\n", text, val[0], val[1], val[2], val[3]);
}
*/

__attribute__ ((target ("sse2")))
void DyePalette::replaceSColorSimd(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
#include "resources/dye/dyepalette_replacescolor_sse2.hpp"
}

__attribute__ ((target ("avx2")))
void DyePalette::replaceSColorSimd(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
#include "resources/dye/dyepalette_replacescolor_avx2.hpp"
}

__attribute__ ((target ("sse2")))
void DyePalette::replaceSColorSse2(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
#include "resources/dye/dyepalette_replacescolor_sse2.hpp"
}

__attribute__ ((target ("avx2")))
void DyePalette::replaceSColorAvx2(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
#include "resources/dye/dyepalette_replacescolor_avx2.hpp"
}

#endif  // SIMD_SUPPORTED

FUNCTION_SIMD_DEFAULT
void DyePalette::replaceSColorSimd(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
#include "resources/dye/dyepalette_replacescolor_default.hpp"
}

FUNCTION_SIMD_DEFAULT
void DyePalette::replaceSColorSse2(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
#include "resources/dye/dyepalette_replacescolor_default.hpp"
}

FUNCTION_SIMD_DEFAULT
void DyePalette::replaceSColorAvx2(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
#include "resources/dye/dyepalette_replacescolor_default.hpp"
}
