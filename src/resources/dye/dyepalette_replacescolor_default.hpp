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
