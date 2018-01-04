/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "resources/map/objectslayer.h"

#include "utils/cast.h"

#include "resources/map/mapobjectlist.h"

#include "debug.h"

ObjectsLayer::ObjectsLayer(const unsigned width,
                           const unsigned height) :
    MemoryCounter(),
    mTiles(new MapObjectList*[width * height]),
    mWidth(width),
    mHeight(height)
{
    std::fill_n(mTiles, width * height, static_cast<MapObjectList*>(nullptr));
}

ObjectsLayer::~ObjectsLayer()
{
    const unsigned size = mWidth * mHeight;
    for (unsigned f = 0; f < size; f ++)
        delete mTiles[f];

    delete [] mTiles;
    mTiles = nullptr;
}

void ObjectsLayer::addObject(const std::string &name, const int type,
                             const unsigned x, const unsigned y,
                             unsigned dx, unsigned dy)
{
    if (mTiles == nullptr)
        return;

    if (x + dx > mWidth)
        dx = mWidth - x;
    if (y + dy > mHeight)
        dy = mHeight - y;

    for (unsigned y1 = y; y1 < y + dy; y1 ++)
    {
        const unsigned idx1 = x + y1 * mWidth;
        const unsigned idx2 = idx1 + dx;

        for (unsigned i = idx1; i < idx2; i ++)
        {
            if (mTiles[i] == nullptr)
                mTiles[i] = new MapObjectList;
            mTiles[i]->objects.push_back(MapObject(type, name));
        }
    }
}

MapObjectList *ObjectsLayer::getAt(const unsigned x, const unsigned y) const
{
    if (x >= mWidth || y >= mHeight)
        return nullptr;
    return mTiles[x + y * mWidth];
}

int ObjectsLayer::calcMemoryLocal() const
{
    return CAST_S32(sizeof(ObjectsLayer) +
        (sizeof(MapObjectList) + sizeof(MapObjectList*)) * mWidth * mHeight);
}
