/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef RESOURCES_RECT_DOUBLERECT_H
#define RESOURCES_RECT_DOUBLERECT_H

#include "localconsts.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_video.h>
PRAGMA48(GCC diagnostic pop)

struct DoubleRect final
{
    A_DEFAULT_COPY(DoubleRect)

    SDL_Rect src;
    SDL_Rect dst;
};

#endif  // RESOURCES_RECT_DOUBLERECT_H
