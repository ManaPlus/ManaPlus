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

    std::vector<DyeColor>::const_iterator it_end = mColors.end();
    const size_t sz = mColors.size();
    if (!sz || !pixels)
        return;
    if (sz % 2)
        -- it_end;

    for (int ptr = 0; ptr < bufSize; ptr += 8)
    {
//        __m256i base = _mm256_load_si256(reinterpret_cast<__m256i*>(
//            &pixels[ptr]));
        __m256i base = _mm256_loadu_si256(reinterpret_cast<__m256i*>(
            &pixels[ptr]));

        std::vector<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

            __m256i newMask = _mm256_set1_epi32(col2.valueAOgl);
            __m256i cmpMask = _mm256_set1_epi32(col.valueAOgl);
            __m256i cmpRes = _mm256_cmpeq_epi32(base, cmpMask);
            __m256i srcAnd = _mm256_andnot_si256(cmpRes, base);
            __m256i dstAnd = _mm256_and_si256(cmpRes, newMask);
            base = _mm256_or_si256(srcAnd, dstAnd);

            ++ it;
        }
//        _mm256_store_si256(reinterpret_cast<__m256i*>(&pixels[ptr]), base);
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(&pixels[ptr]), base);
    }
