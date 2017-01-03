/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#ifndef GUI_COLORPAIR_H
#define GUI_COLORPAIR_H

#include "localconsts.h"

class Color;

struct ColorPair final
{
    constexpr ColorPair(const Color *const c1,
              const Color *const c2) :
        color1(c1),
        color2(c2)
    {
    }

    const Color *color1;
    const Color *color2;
};

#endif  // GUI_COLORPAIR_H
