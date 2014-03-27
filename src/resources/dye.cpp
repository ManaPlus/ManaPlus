/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/dye.h"

#include "logger.h"

#include "resources/db/palettedb.h"

#include "utils/delete2.h"

#include <cmath>
#include <sstream>

#include <SDL_endian.h>

#include "debug.h"

DyePalette::DyePalette(const std::string &description,
                       const int8_t blockSize) :
    mColors()
{
    const size_t size = static_cast<int>(description.length());
    if (size == 0)
        return;

    if (description[0] == '#')
    {
        size_t pos = 1;
        for ( ; ; )
        {
            if (pos + blockSize > size)
                break;

            DyeColor color(0, 0, 0, 0);

            for (int i = 0, colorIdx = 0; i < blockSize && colorIdx < 4;
                 i += 2, colorIdx ++)
            {
                color.value[colorIdx] = static_cast<unsigned char>((
                    hexDecode(description[pos + i]) << 4)
                    + hexDecode(description[pos + i + 1]));
            }
            mColors.push_back(color);
            pos += blockSize;

            if (pos == size)
                return;
            if (description[pos] != ',')
                break;

            ++pos;
        }
    }
    else if (description[0] == '@')
    {
        size_t pos = 1;
        for ( ; pos < size ; )
        {
            const size_t idx = description.find(',', pos);
            if (idx == std::string::npos)
                return;
            if (idx == pos)
                break;
            mColors.push_back(PaletteDB::getColor(
                description.substr(pos, idx - pos)));
            pos = idx + 1;
        }
    }

    logger->log("Error, invalid embedded palette: %s", description.c_str());
}

unsigned int DyePalette::hexDecode(const signed char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else
        return 0;
}

void DyePalette::getColor(const int intensity, int color[3]) const
{
    if (intensity == 0)
    {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        return;
    }

    const int last = static_cast<int>(mColors.size());
    if (last == 0)
        return;

    const int intLast = intensity * last;
    const int i = intLast / 255;
    const int t = intLast % 255;

    int j = t != 0 ? i : i - 1;

    if (j >= last)
        j = 0;

    // Get the exact color if any, the next color otherwise.
    const int r2 = mColors[j].value[0];
    const int g2 = mColors[j].value[1];
    const int b2 = mColors[j].value[2];

    if (t == 0)
    {
        // Exact color.
        color[0] = r2;
        color[1] = g2;
        color[2] = b2;
        return;
    }

    // Get the previous color. First color is implicitly black.
    int r1 = 0, g1 = 0, b1 = 0;
    if (i > 0 && i < last + 1)
    {
        r1 = mColors[i - 1].value[0];
        g1 = mColors[i - 1].value[1];
        b1 = mColors[i - 1].value[2];
    }

    // Perform a linear interpolation.
    color[0] = ((255 - t) * r1 + t * r2) / 255;
    color[1] = ((255 - t) * g1 + t * g2) / 255;
    color[2] = ((255 - t) * b1 + t * b2) / 255;
}

void DyePalette::getColor(double intensity, int color[3]) const
{
    // Nothing to do here
    if (mColors.empty())
        return;

    // Force range
    if (intensity > 1.0)
        intensity = 1.0;
    else if (intensity < 0.0)
        intensity = 0.0;

    // Scale up
    intensity *= static_cast<double>(mColors.size() - 1);

    // Color indices
    const int i = static_cast<int>(floor(intensity));
    const int j = static_cast<int>(ceil(intensity));

    if (i == j)
    {
        // Exact color.
        color[0] = mColors[i].value[0];
        color[1] = mColors[i].value[1];
        color[2] = mColors[i].value[2];
        return;
    }

    intensity -= i;
    const double rest = 1 - intensity;

    // Get the colors
    const int r1 = mColors[i].value[0];
    const int g1 = mColors[i].value[1];
    const int b1 = mColors[i].value[2];
    const int r2 = mColors[j].value[0];
    const int g2 = mColors[j].value[1];
    const int b2 = mColors[j].value[2];

    // Perform the interpolation.
    color[0] = static_cast<int>(rest * r1 + intensity * r2);
    color[1] = static_cast<int>(rest * g1 + intensity * g2);
    color[2] = static_cast<int>(rest * b1 + intensity * b2);
}

void DyePalette::replaceSColor(uint32_t *restrict pixels,
                               const int bufSize) const
{
    std::vector<DyeColor>::const_iterator it_end = mColors.end();
    const int sz = mColors.size();
    if (!sz)
        return;
    if (sz % 2)
        -- it_end;

    for (uint32_t *p_end = pixels + bufSize; pixels != p_end; ++pixels)
    {
        uint8_t *const p = reinterpret_cast<uint8_t *>(pixels);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int alpha = *pixels & 0xff000000;
        const unsigned int data = (*pixels) & 0x00ffffff;
#else
        const int alpha = *p & 0xff;
        const unsigned int data = (*pixels) & 0xffffff00;
#endif
//        logger->log("c:%04d   %08x", c, *pixels);
//        logger->log("data:    %08x", data);
        if (!alpha)
        {
//            logger->log("skip:    %08x", *pixels);
            continue;
        }

        std::vector<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            const unsigned int coldata = (col.value[2] << 16)
                | (col.value[1] << 8) | (col.value[0]);
#else
            const unsigned int coldata = (col.value[2] << 8)
                | (col.value[1] << 16) | (col.value[0] << 24);
#endif
//            logger->log("coldata: %08x", coldata);
            if (data == coldata)
            {
//                logger->log("correct");
                p[3] = col2.value[0];
                p[2] = col2.value[1];
                p[1] = col2.value[2];
                break;
            }

            ++ it;
        }
    }
}

void DyePalette::replaceAColor(uint32_t *restrict pixels,
                               const int bufSize) const
{
    std::vector<DyeColor>::const_iterator it_end = mColors.end();
    const int sz = mColors.size();
    if (!sz)
        return;
    if (sz % 2)
        -- it_end;

    for (uint32_t *p_end = pixels + bufSize; pixels != p_end; ++pixels)
    {
        uint8_t *const p = reinterpret_cast<uint8_t *>(pixels);
        const unsigned int data = *pixels;

        std::vector<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            const unsigned int coldata = (col.value[3] << 24)
                | (col.value[2] << 16) | (col.value[1] << 8) | (col.value[0]);
#else
            const unsigned int coldata = (col.value[3]) | (col.value[2] << 8)
                | (col.value[1] << 16) | (col.value[0] << 24);
#endif

            if (data == coldata)
            {
                p[3] = col2.value[0];
                p[2] = col2.value[1];
                p[1] = col2.value[2];
                p[0] = col2.value[3];
                break;
            }

            ++ it;
        }
    }
}

void DyePalette::replaceSOGLColor(uint32_t *restrict pixels,
                                  const int bufSize) const
{
    std::vector<DyeColor>::const_iterator it_end = mColors.end();
    const int sz = mColors.size();
    if (!sz)
        return;
    if (sz % 2)
        -- it_end;

    for (uint32_t *p_end = pixels + bufSize; pixels != p_end; ++pixels)
    {
        uint8_t *const p = reinterpret_cast<uint8_t *>(pixels);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int alpha = *p & 0xff;
        const unsigned int data = (*pixels) & 0xffffff00;
#else
        const int alpha = *pixels & 0xff000000;
        const unsigned int data = (*pixels) & 0x00ffffff;
#endif
        if (!alpha)
            continue;

        std::vector<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            const unsigned int coldata = (col.value[0] << 24)
                | (col.value[1] << 16) | (col.value[2] << 8);
#else
            const unsigned int coldata = (col.value[0])
                | (col.value[1] << 8) | (col.value[2] << 16);
#endif
            if (data == coldata)
            {
                p[0] = col2.value[0];
                p[1] = col2.value[1];
                p[2] = col2.value[2];
                break;
            }

            ++ it;
        }
    }
}

void DyePalette::replaceAOGLColor(uint32_t *restrict pixels,
                                  const int bufSize) const
{
    std::vector<DyeColor>::const_iterator it_end = mColors.end();
    const int sz = mColors.size();
    if (!sz)
        return;
    if (sz % 2)
        -- it_end;

    for (uint32_t *p_end = pixels + bufSize; pixels != p_end; ++pixels)
    {
        uint8_t *const p = reinterpret_cast<uint8_t *>(pixels);
        const unsigned int data = *pixels;

        std::vector<DyeColor>::const_iterator it = mColors.begin();
        while (it != it_end)
        {
            const DyeColor &col = *it;
            ++ it;
            const DyeColor &col2 = *it;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            const unsigned int coldata = (col.value[0] << 24)
                | (col.value[1] << 16) | (col.value[2] << 8) | col.value[3];
#else
            const unsigned int coldata = (col.value[0]) | (col.value[1] << 8)
                | (col.value[2] << 16) | (col.value[3] << 24);
#endif
            if (data == coldata)
            {
                p[0] = col2.value[0];
                p[1] = col2.value[1];
                p[2] = col2.value[2];
                p[3] = col2.value[3];
                break;
            }

            ++ it;
        }
    }
}

Dye::Dye(const std::string &description)
{
    for (int i = 0; i < dyePalateSize; ++i)
        mDyePalettes[i] = nullptr;

    if (description.empty())
        return;

    size_t next_pos = 0;
    const size_t length = description.length();
    do
    {
        const size_t pos = next_pos;
        next_pos = description.find(';', pos);

        if (next_pos == std::string::npos)
            next_pos = length;

        if (next_pos <= pos + 3 || description[pos + 1] != ':')
        {
            logger->log("Error, invalid dye: %s", description.c_str());
            return;
        }

        int i = 0;

        switch (description[pos])
        {
            case 'R': i = 0; break;
            case 'G': i = 1; break;
            case 'Y': i = 2; break;
            case 'B': i = 3; break;
            case 'M': i = 4; break;
            case 'C': i = 5; break;
            case 'W': i = 6; break;
            case 'S': i = 7; break;
            case 'A': i = 8; break;
            default:
                logger->log("Error, invalid dye: %s", description.c_str());
                return;
        }
        mDyePalettes[i] = new DyePalette(description.substr(
            pos + 2, next_pos - pos - 2), i != 8 ? 6 : 8);
        ++next_pos;
    }
    while (next_pos < length);
}

Dye::~Dye()
{
    for (int i = 0; i < dyePalateSize; ++i)
        delete2(mDyePalettes[i])
}

void Dye::instantiate(std::string &restrict target,
                      const std::string &restrict palettes)
{
    size_t next_pos = target.find('|');

    if (next_pos == std::string::npos || palettes.empty())
        return;

    ++next_pos;

    std::ostringstream s;
    s << target.substr(0, next_pos);
    size_t last_pos = target.length(), pal_pos = 0;
    do
    {
        const size_t pos = next_pos;
        next_pos = target.find(';', pos);

        if (next_pos == std::string::npos)
            next_pos = last_pos;

        if (next_pos == pos + 1 && pal_pos != std::string::npos)
        {
            const size_t pal_next_pos = palettes.find(';', pal_pos);
            s << target[pos] << ':';
            if (pal_next_pos == std::string::npos)
            {
                s << palettes.substr(pal_pos);
                s << target.substr(next_pos);
                break;
            }
            s << palettes.substr(pal_pos, pal_next_pos - pal_pos);
            pal_pos = pal_next_pos + 1;
        }
        else if (next_pos > pos + 2)
        {
            s << target.substr(pos, next_pos - pos);
        }
        else
        {
            logger->log("Error, invalid dye placeholder: %s", target.c_str());
            return;
        }
        s << target[next_pos];
        ++next_pos;
    }
    while (next_pos < last_pos);

    target = s.str();
}

int Dye::getType() const
{
    if (mDyePalettes[sPaleteIndex])
        return 1;
    if (mDyePalettes[aPaleteIndex])
        return 2;
    return 0;
}

void Dye::normalDye(uint32_t *restrict pixels, const int bufSize) const
{
    for (uint32_t *p_end = pixels + bufSize; pixels != p_end; ++ pixels)
    {
        const uint32_t p = *pixels;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int alpha = p & 0xff000000;
#else
        const int alpha = p & 0xff;
#endif
        if (!alpha)
            continue;
        int color[3];
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        color[0] = (p) & 255;
        color[1] = (p >> 8) & 255;
        color[2] = (p >> 16) & 255;
#else
        color[0] = (p >> 24) & 255;
        color[1] = (p >> 16) & 255;
        color[2] = (p >> 8) & 255;
#endif

        const int cmax = std::max(color[0], std::max(color[1], color[2]));
        if (cmax == 0)
            continue;

        const int cmin = std::min(color[0], std::min(color[1], color[2]));
        const int intensity = color[0] + color[1] + color[2];

        if (cmin != cmax && (cmin != 0 || (intensity != cmax
            && intensity != 2 * cmax)))
        {
            // not pure
            continue;
        }

        const int i = (color[0] != 0) | ((color[1] != 0) << 1)
            | ((color[2] != 0) << 2);

        if (mDyePalettes[i - 1])
            mDyePalettes[i - 1]->getColor(cmax, color);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        *pixels = (color[0]) | (color[1] << 8)
            | (color[2] << 16) | alpha;
#else
        *pixels = (color[0] << 24) | (color[1] << 16)
            | (color[2] << 8) | alpha;
#endif
    }
}

void Dye::normalOGLDye(uint32_t *restrict pixels, const int bufSize) const
{
    for (uint32_t *p_end = pixels + bufSize; pixels != p_end; ++ pixels)
    {
        const uint32_t p = *pixels;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int alpha = p & 255;
#else
        const int alpha = p & 0xff000000;
#endif
        if (!alpha)
            continue;
        int color[3];
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        color[0] = (p >> 24) & 255;
        color[1] = (p >> 16) & 255;
        color[2] = (p >> 8) & 255;
#else
        color[0] = (p) & 255;
        color[1] = (p >> 8) & 255;
        color[2] = (p >> 16) & 255;
#endif

        const int cmax = std::max(color[0], std::max(color[1], color[2]));
        if (cmax == 0)
            continue;

        const int cmin = std::min(color[0], std::min(color[1], color[2]));
        const int intensity = color[0] + color[1] + color[2];

        if (cmin != cmax && (cmin != 0 || (intensity != cmax
            && intensity != 2 * cmax)))
        {
            // not pure
            continue;
        }

        const int i = (color[0] != 0) | ((color[1] != 0) << 1)
            | ((color[2] != 0) << 2);

        if (mDyePalettes[i - 1])
            mDyePalettes[i - 1]->getColor(cmax, color);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        *pixels = (color[0] << 24) | (color[1] << 16)
            | (color[2] << 8) | alpha;
#else
        *pixels = (color[0]) | (color[1] << 8)
            | (color[2] << 16) | alpha;
#endif
    }
}
