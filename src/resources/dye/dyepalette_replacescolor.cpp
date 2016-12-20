/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "logger.h"

#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER

#ifdef __x86_64__
// avx2
#include "immintrin.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define buildHex(a, b, c, d) \
    (d) * 16777216U + (c) * 65536U + (b) * 256U + CAST_U32(a)
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
#define buildHex(a, b, c, d) \
    (a) * 16777216U + (b) * 65536U + (c) * 256U + CAST_U32(d)
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN
#endif  // __x86_64__

#include "debug.h"

void DyePalette::replaceSColor(uint32_t *restrict pixels,
                               const int bufSize) const restrict2
{
#ifdef __x86_64__
    if (bufSize % 8 == 0)
        replaceSColor256(pixels, bufSize);
    else
        replaceSColorDefault(pixels, bufSize);
#else  // __x86_64__
    replaceSColorDefault(pixels, bufSize);
#endif  // __x86_64__
}

void DyePalette::replaceSColorDefault(uint32_t *restrict pixels,
                                      const int bufSize) const restrict2
{
    std::vector<DyeColor>::const_iterator it_end = mColors.end();
    const size_t sz = mColors.size();
    if (!sz || !pixels)
        return;
    if (sz % 2)
        -- it_end;

#ifdef ENABLE_CILKPLUS
    cilk_for (int ptr = 0; ptr < bufSize; ptr ++)
    {
        uint8_t *const p = reinterpret_cast<uint8_t *>(&pixels[ptr]);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const unsigned int data = pixels[ptr] & 0x00ffffff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const unsigned int data = pixels[ptr] & 0xffffff00;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        std::vector<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            const unsigned int coldata = (col.value[2] << 16U)
                | (col.value[1] << 8U) | (col.value[0]);
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
            const unsigned int coldata = (col.value[2] << 8U)
                | (col.value[1] << 16U) | (col.value[0] << 24U);
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

            if (data == coldata)
            {
                p[3] = col2.value[0];
                p[2] = col2.value[1];
                p[1] = col2.value[2];
                break;
            }
            ++ it;
        }
    }
#else  // ENABLE_CILKPLUS

    for (const uint32_t *const p_end = pixels + CAST_SIZE(bufSize);
         pixels != p_end;
         ++ pixels)
    {
        uint8_t *const p = reinterpret_cast<uint8_t *>(pixels);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const unsigned int data = (*pixels) & 0x00ffffff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const unsigned int data = (*pixels) & 0xffffff00;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        std::vector<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            const unsigned int coldata = (col.value[2] << 16U)
                | (col.value[1] << 8U) | (col.value[0]);
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

            const unsigned int coldata = (col.value[2] << 8U)
                | (col.value[1] << 16U) | (col.value[0] << 24U);
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

            if (data == coldata)
            {
                p[3] = col2.value[0];
                p[2] = col2.value[1];
                p[1] = col2.value[2];
                break;
            }

            ++ it;
        }
    }
#endif  // ENABLE_CILKPLUS
}

#ifdef __x86_64__
void print256(const char *const text, const __m256i &val);
void print256(const char *const text, const __m256i &val)
{
    printf("%s 0x%016llx%016llx%016llx%016llx\n", text, val[0], val[1], val[2], val[3]);
}

__attribute__ ((target ("avx2")))
void DyePalette::replaceSColor256(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2
{
    std::vector<DyeColor>::const_iterator it_end = mColors.end();
    const size_t sz = mColors.size();
    if (!sz || !pixels)
        return;
    if (sz % 2)
        -- it_end;

    for (const uint32_t *const p_end = pixels + CAST_SIZE(bufSize);
         pixels != p_end;
         pixels += 8)
    {
        __m256i mask = _mm256_set1_epi32(0xffffff00);
        //__m256i base = _mm256_load_si256(reinterpret_cast<__m256i*>(pixels));
        __m256i base = _mm256_loadu_si256(reinterpret_cast<__m256i*>(pixels));
        //print256("mask  ", mask);

        std::vector<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            //print256("base  ", base);
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

            __m256i base2 = _mm256_and_si256(mask, base);
            //print256("base2  ", base2);
            __m256i newMask = _mm256_set1_epi32(buildHex(col2.value[0], col2.value[1], col2.value[2], 0));
            //print256("newMask  ", newMask);
            __m256i cmpMask = _mm256_set1_epi32(buildHex(col.value[0], col.value[1], col.value[2], 0));
            //print256("cmpMask  ", cmpMask);
            __m256i cmpRes = _mm256_cmpeq_epi32(base2, cmpMask);
            //print256("cmpRes  ", cmpRes);
            cmpRes = _mm256_and_si256(mask, cmpRes);
            //print256("cmpRes  ", cmpRes);
            __m256i srcAnd = _mm256_andnot_si256(cmpRes, base);
            //print256("srcAnd  ", srcAnd);
            __m256i dstAnd = _mm256_and_si256(cmpRes, newMask);
            //print256("dstAnd  ", dstAnd);
            base = _mm256_or_si256(srcAnd, dstAnd);
            ++ it;
        }
        //print256("res     ", base);
        //_mm256_store_si256(reinterpret_cast<__m256i*>(pixels), base);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(pixels), base);
    }
}

#endif  // __x86_64__

FUNCTION_SIMD_DEFAULT
void DyePalette::replaceSColor256(uint32_t *restrict pixels,
                                  const int bufSize) const restrict2
{
    replaceSColorDefault(pixels, bufSize);
}
