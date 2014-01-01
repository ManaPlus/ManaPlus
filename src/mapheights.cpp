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

#include "mapheights.h"

#include "debug.h"

MapHeights::MapHeights(const int width, const int height) :
    mWidth(width),
    mHeight(height),
    mTiles(new uint8_t[mWidth * mHeight])
{
    memset(mTiles, 0, mWidth * mHeight);
}

MapHeights::~MapHeights()
{
    delete [] mTiles;
}

void MapHeights::setHeight(const int x, const int y, const uint8_t height)
{
    mTiles[x + y * mWidth] = height;
}
