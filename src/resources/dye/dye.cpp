/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "resources/dye/dye.h"

#include "logger.h"

#include "resources/dye/dyepalette.h"

#include "utils/delete2.h"

#include <sstream>

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

Dye::Dye(const std::string &restrict description)
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
    size_t last_pos = target.length();
    size_t pal_pos = 0;
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

int Dye::getType() const restrict2 noexcept2
{
    if (mDyePalettes[sPaleteIndex] != nullptr)
        return 1;
    if (mDyePalettes[aPaleteIndex] != nullptr)
        return 2;
    return 0;
}

void Dye::normalDye(uint32_t *restrict pixels,
                    const int bufSize) const restrict2
{
    if (pixels == nullptr)
        return;

    for (const uint32_t *const p_end = pixels + CAST_SIZE(bufSize);
         pixels != p_end;
         ++ pixels)
    {
        const uint32_t p = *pixels;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int alpha = p & 0xff000000;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const int alpha = p & 0xff;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        if (alpha == 0)
            continue;
        unsigned int color[3];
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        color[0] = (p) & 255U;
        color[1] = (p >> 8U) & 255U;
        color[2] = (p >> 16U) & 255U;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        color[0] = (p >> 24U) & 255U;
        color[1] = (p >> 16U) & 255U;
        color[2] = (p >> 8U) & 255U;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const unsigned int cmax = std::max(
            color[0], std::max(color[1], color[2]));
        if (cmax == 0)
            continue;

        const unsigned int cmin = std::min(
            color[0], std::min(color[1], color[2]));
        const unsigned int intensity = color[0] + color[1] + color[2];

        if (cmin != cmax && (cmin != 0 || (intensity != cmax
            && intensity != 2 * cmax)))
        {
            // not pure
            continue;
        }

        const unsigned int i = static_cast<int>(color[0] != 0) |
            (static_cast<int>(color[1] != 0) << 1) |
            (static_cast<int>(color[2] != 0) << 2);

        if (mDyePalettes[i - 1] != nullptr)
            mDyePalettes[i - 1]->getColor(cmax, color);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        *pixels = (color[0]) | (color[1] << 8)
            | (color[2] << 16) | alpha;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        *pixels = (color[0] << 24) | (color[1] << 16)
            | (color[2] << 8) | alpha;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN
    }
}

void Dye::normalOGLDye(uint32_t *restrict pixels,
                       const int bufSize) const restrict2
{
    if (pixels == nullptr)
        return;

    for (const uint32_t *const p_end = pixels + CAST_SIZE(bufSize);
         pixels != p_end;
         ++ pixels)
    {
        const uint32_t p = *pixels;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const uint32_t alpha = p & 255U;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const uint32_t alpha = p & 0xff000000U;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        if (alpha == 0)
            continue;
        unsigned int color[3];
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        color[0] = (p >> 24U) & 255U;
        color[1] = (p >> 16U) & 255U;
        color[2] = (p >> 8U) & 255U;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        color[0] = (p) & 255U;
        color[1] = (p >> 8U) & 255U;
        color[2] = (p >> 16U) & 255U;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const unsigned int cmax = std::max(
            color[0], std::max(color[1], color[2]));
        if (cmax == 0)
            continue;

        const unsigned int cmin = std::min(
            color[0], std::min(color[1], color[2]));
        const unsigned int intensity = color[0] + color[1] + color[2];

        if (cmin != cmax && (cmin != 0 || (intensity != cmax
            && intensity != 2 * cmax)))
        {
            // not pure
            continue;
        }

        const unsigned int i = static_cast<int>(color[0] != 0) |
            (static_cast<int>(color[1] != 0) << 1) |
            (static_cast<int>(color[2] != 0) << 2);

        if (mDyePalettes[i - 1] != nullptr)
            mDyePalettes[i - 1]->getColor(cmax, color);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        *pixels = (color[0] << 24) | (color[1] << 16)
            | (color[2] << 8) | alpha;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        *pixels = (color[0]) | (color[1] << 8)
            | (color[2] << 16) | alpha;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN
    }
}
