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

#include "resources/map/speciallayer.h"

#include "const/resources/map/map.h"

#include "enums/resources/map/mapitemtype.h"

#include "resources/map/mapitem.h"

#include "utils/delete2.h"
#include "utils/foreach.h"

#include "debug.h"

SpecialLayer::SpecialLayer(const std::string &name,
                           const int width,
                           const int height) :
    MemoryCounter(),
    mName(name),
    mTiles(new MapItem*[width * height]),
    mCache(new int[width * height]),
    mWidth(width),
    mHeight(height)
{
    std::fill_n(mTiles, mWidth * mHeight, static_cast<MapItem*>(nullptr));
    std::fill_n(mCache, mWidth * mHeight, 10000);
}

SpecialLayer::~SpecialLayer()
{
    for (int f = 0; f < mWidth * mHeight; f ++)
        delete2(mTiles[f])
    delete [] mTiles;
    delete [] mCache;
}

MapItem* SpecialLayer::getTile(const int x, const int y) const
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return nullptr;
    }
    return mTiles[x + y * mWidth];
}

void SpecialLayer::setTile(const int x, const int y, MapItem *const item)
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return;
    }

    const int idx = x + y * mWidth;
    delete mTiles[idx];
    if (item != nullptr)
        item->setPos(x, y);
    mTiles[idx] = item;
}

void SpecialLayer::setTile(const int x, const int y, const int type)
{
    if (x < 0 || x >= mWidth ||
        y < 0 || y >= mHeight)
    {
        return;
    }

    const int idx = x + y * mWidth;
    MapItem *const tile = mTiles[idx];
    if (tile != nullptr)
    {
        tile->setType(type);
        tile->setPos(x, y);
    }
    else
    {
        mTiles[idx] = new MapItem(type);
        mTiles[idx]->setPos(x, y);
    }
}

void SpecialLayer::addRoad(const Path &road)
{
    FOR_EACH (Path::const_iterator, i, road)
    {
        const Position &pos = *i;
        MapItem *const item = getTile(pos.x, pos.y);
        if (item == nullptr)
            setTile(pos.x, pos.y, new MapItem(MapItemType::ROAD));
        else
            item->setType(MapItemType::ROAD);
    }
    updateCache();
}

void SpecialLayer::clean()
{
    if (mTiles == nullptr)
        return;

    for (int f = 0; f < mWidth * mHeight; f ++)
    {
        MapItem *const item = mTiles[f];
        if (item != nullptr)
            item->setType(MapItemType::EMPTY);
    }
    updateCache();
}

void SpecialLayer::draw(Graphics *const graphics, int startX, int startY,
                        int endX, int endY,
                        const int scrollX, const int scrollY) const
{
    BLOCK_START("SpecialLayer::draw")
    if (startX < 0)
        startX = 0;
    if (startY < 0)
        startY = 0;
    if (endX > mWidth)
        endX = mWidth;
    if (endY > mHeight)
        endY = mHeight;

    for (int y = startY; y < endY; y ++)
    {
        const int py = y * mapTileSize - scrollY;
        const int y2 = y * mWidth;
        for (int x = startX; x < endX; x ++)
        {
            const MapItem *const item = mTiles[x + y2];
            if (item != nullptr)
            {
                item->draw(graphics, x * mapTileSize - scrollX, py,
                    mapTileSize, mapTileSize);
            }
        }
    }
    BLOCK_END("SpecialLayer::draw")
}

int SpecialLayer::calcMemoryLocal() const
{
    return static_cast<int>(sizeof(SpecialLayer) +
        sizeof(MapItem) * mWidth * mHeight);
}

void SpecialLayer::updateCache()
{
    for (int y = 0; y < mHeight; y ++)
    {
        const int y2 = y * mWidth;
        for (int x = 0; x < mWidth; x ++)
        {
            int c = 10000;
            for (int f = x + 1; f < mWidth; f ++)
            {
                MapItem *const item = mTiles[f + y2];
                if (item != nullptr &&
                    item->mType != MapItemType::EMPTY)
                {
                    c = f - x - 1;
                    break;
                }
            }
            mCache[x + y2] = c;
        }
    }
}
