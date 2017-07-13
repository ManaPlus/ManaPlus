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

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER
PRAGMA48(GCC diagnostic pop)

#ifdef SIMD_SUPPORTED
// avx2
#include <immintrin.h>
#endif  // SIMD_SUPPORTED

#include "debug.h"

void DyePalette::replaceSColorDefault(uint32_t *restrict pixels,
                                      const int bufSize) const restrict2
{
    STD_VECTOR<DyeColor>::const_iterator it_end = mColors.end();
    const size_t sz = mColors.size();
    if (sz == 0u || pixels == nullptr)
        return;
    if ((sz % 2) != 0u)
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

        STD_VECTOR<DyeColor>::const_iterator it = mColors.begin();
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

        STD_VECTOR<DyeColor>::const_iterator it = mColors.begin();
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

#ifdef SIMD_SUPPORTED
/*
static void print256(const char *const text, const __m256i &val);
static void print256(const char *const text, const __m256i &val)
{
    printf("%s 0x%016llx%016llx%016llx%016llx\n", text, val[0], val[1], val[2], val[3]);
}
*/

__attribute__ ((target ("sse2")))
void DyePalette::replaceSColorSse2(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
    STD_VECTOR<DyeColor>::const_iterator it_end = mColors.end();
    const size_t sz = mColors.size();
    if (sz == 0u || pixels == nullptr)
        return;
    if ((sz % 2) != 0u)
        -- it_end;
    const int mod = bufSize % 8;
    const int bufEnd = bufSize - mod;

    for (int ptr = 0; ptr < bufEnd; ptr += 4)
    {
        __m128i mask = _mm_set1_epi32(0xffffff00);
//        __m128i base = _mm_load_si128(reinterpret_cast<__m128i*>(pixels));
        __m128i base = _mm_loadu_si128(reinterpret_cast<__m128i*>(
            &pixels[ptr]));

        STD_VECTOR<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

            __m128i base2 = _mm_and_si128(mask, base);
            __m128i newMask = _mm_set1_epi32(col2.valueS);
            __m128i cmpMask = _mm_set1_epi32(col.valueS);
            __m128i cmpRes = _mm_cmpeq_epi32(base2, cmpMask);
            cmpRes = _mm_and_si128(mask, cmpRes);
            __m128i srcAnd = _mm_andnot_si128(cmpRes, base);
            __m128i dstAnd = _mm_and_si128(cmpRes, newMask);
            base = _mm_or_si128(srcAnd, dstAnd);
            ++ it;
        }
//        _mm_store_si128(reinterpret_cast<__m128i*>(pixels), base);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(&pixels[ptr]), base);
    }

    // complete end without simd
    for (int ptr = bufSize - mod; ptr < bufSize; ptr ++)
    {
        uint8_t *const p = reinterpret_cast<uint8_t *>(&pixels[ptr]);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const unsigned int data = pixels[ptr] & 0x00ffffff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const unsigned int data = pixels[ptr] & 0xffffff00;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        STD_VECTOR<DyeColor>::const_iterator it = mColors.begin();
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
}

__attribute__ ((target ("avx2")))
void DyePalette::replaceSColorAvx2(uint32_t *restrict pixels,
                                   const int bufSize) const restrict2
{
    STD_VECTOR<DyeColor>::const_iterator it_end = mColors.end();
    const size_t sz = mColors.size();
    if (sz == 0u || pixels == nullptr)
        return;
    if ((sz % 2) != 0u)
        -- it_end;
    const int mod = bufSize % 8;
    const int bufEnd = bufSize - mod;

    for (int ptr = 0; ptr < bufEnd; ptr += 8)
    {
        __m256i mask = _mm256_set1_epi32(0xffffff00);
//        __m256i base = _mm256_load_si256(reinterpret_cast<__m256i*>(pixels));
        __m256i base = _mm256_loadu_si256(reinterpret_cast<__m256i*>(
            &pixels[ptr]));

        STD_VECTOR<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

            __m256i base2 = _mm256_and_si256(mask, base);
            __m256i newMask = _mm256_set1_epi32(col2.valueS);
            __m256i cmpMask = _mm256_set1_epi32(col.valueS);
            __m256i cmpRes = _mm256_cmpeq_epi32(base2, cmpMask);
            cmpRes = _mm256_and_si256(mask, cmpRes);
            __m256i srcAnd = _mm256_andnot_si256(cmpRes, base);
            __m256i dstAnd = _mm256_and_si256(cmpRes, newMask);
            base = _mm256_or_si256(srcAnd, dstAnd);
            ++ it;
        }
//        _mm256_store_si256(reinterpret_cast<__m256i*>(pixels), base);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(&pixels[ptr]), base);
    }

    // complete end without simd
    for (int ptr = bufSize - mod; ptr < bufSize; ptr ++)
    {
        uint8_t *const p = reinterpret_cast<uint8_t *>(&pixels[ptr]);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const unsigned int data = pixels[ptr] & 0x00ffffff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const unsigned int data = pixels[ptr] & 0xffffff00;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        STD_VECTOR<DyeColor>::const_iterator it = mColors.begin();
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
}

#endif  // SIMD_SUPPORTED
