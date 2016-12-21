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

#include "utils/buildhex.h"

#include "localconsts.h"

struct DyeColor final
{
    DyeColor() noexcept2 :
        valueA(buildHex(0, 0, 0, 255)),
        valueS(buildHex(0, 0, 0, 0)),
        valueSOgl(buildHexOgl(0, 0, 0, 0))
    {
        value[3] = 255;
    }

    DyeColor(const uint8_t r,
             const uint8_t g,
             const uint8_t b) noexcept2 :
        valueA(buildHex(r, g, b, 255)),
        valueS(buildHex(r, g, b, 0)),
        valueSOgl(buildHexOgl(0, b, g, r))
    {
        value[0] = r;
        value[1] = g;
        value[2] = b;
        value[3] = 255;
    }

    DyeColor(const uint8_t r,
             const uint8_t g,
             const uint8_t b,
             const uint8_t a) noexcept2 :
        valueA(buildHex(r, g, b, a)),
        valueS(buildHex(r, g, b, 0)),
        valueSOgl(buildHexOgl(0, b, g, r))
    {
        value[0] = r;
        value[1] = g;
        value[2] = b;
        value[3] = a;
    }

    void update()
    {
        valueA = buildHex(value[0], value[1], value[2], value[3]);
        valueS = buildHex(value[0], value[1], value[2], 0);
        valueSOgl = buildHexOgl(0, value[2], value[1], value[0]);
    }

    union
    {
        uint8_t value[4];
        uint32_t valueAOgl;
    };
    uint32_t valueA;
    uint32_t valueS;
    uint32_t valueSOgl;
};

#endif  // RESOURCES_DYE_DYECOLOR_H
