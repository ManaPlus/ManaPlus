/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2016  The ManaPlus Developers
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

#ifndef RESOURCES_DYE_DYECOLOR_H
#define RESOURCES_DYE_DYECOLOR_H

#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER

#include "localconsts.h"

struct DyeColor final
{
    DyeColor() noexcept2
    {
        value[3] = 255;
    }

    DyeColor(const uint8_t r,
             const uint8_t g,
             const uint8_t b) noexcept2
    {
        value[0] = r;
        value[1] = g;
        value[2] = b;
        value[3] = 255;
//        value2 = buildHex(r, g, b, 255);
    }

    DyeColor(const uint8_t r,
             const uint8_t g,
             const uint8_t b,
             const uint8_t a) noexcept2
    {
        value[0] = r;
        value[1] = g;
        value[2] = b;
        value[3] = a;
//        value2 = buildHex(r, g, b, a);
    }

    union
    {
        uint8_t value[4];
        uint32_t value1;
    };
//    uint32_t value2;
};

#endif  // RESOURCES_DYE_DYECOLOR_H
