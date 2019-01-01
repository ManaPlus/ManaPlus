/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2019  The ManaPlus Developers
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

#ifndef RESOURCES_MAP_TILEINFO_H
#define RESOURCES_MAP_TILEINFO_H

#include "localconsts.h"

class Image;

struct TileInfo final
{
    TileInfo() :
        image(nullptr),
        width(0),
        count(1),
        nextTile(1),
        isEnabled(true)
    {
    }

    A_DELETE_COPY(TileInfo)

    /* tile image */
    Image *image;
    /* repeated tile width in pixels */
    int width;
    /* repeated tiles count - 1 */
    int count;
    /* number of tiles to get next tile */
    int nextTile;
    /* is tile enabled flag. if set to true, also mean image is non null */
    bool isEnabled;
};

#endif  // RESOURCES_MAP_TILEINFO_H
