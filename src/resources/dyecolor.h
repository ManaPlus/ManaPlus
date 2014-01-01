/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef RESOURCES_DYECOLOR_H
#define RESOURCES_DYECOLOR_H

#include <SDL_stdinc.h>

#include "localconsts.h"

struct DyeColor
{
    DyeColor()
    {
        value[3] = 255;
    }

    DyeColor(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        value[0] = r;
        value[1] = g;
        value[2] = b;
        value[3] = 255;
    }

    DyeColor(const uint8_t r, const uint8_t g, const uint8_t b,
             const uint8_t a)
    {
        value[0] = r;
        value[1] = g;
        value[2] = b;
        value[3] = a;
    }

    uint8_t value[4];
};

#endif  // RESOURCES_DYECOLOR_H
