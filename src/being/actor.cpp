/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "being/actor.h"

#include "resources/map/map.h"

#include "debug.h"

Actor::Actor() :
    mPixelX(0),
    mPixelY(0),
    mMap(nullptr),
    mPos(),
    mYDiff(0),
    mMapActor()
{
}

Actor::~Actor()
{
    if (mMap != nullptr)
    {
        mMap->removeActor(mMapActor);
        mMap = nullptr;
    }
}

void Actor::setMap(Map *const map)
{
    // Remove Actor from potential previous map
    if (mMap != nullptr)
        mMap->removeActor(mMapActor);

    mMap = map;

    // Add Actor to potential new map
    if (mMap != nullptr)
        mMapActor = mMap->addActor(this);
}

int Actor::getTileX() const
{
    if (mMap == nullptr ||
        mMap->getTileWidth() == 0)
    {
        return 0;
    }

    return getPixelX() / mMap->getTileWidth();
}

int Actor::getTileY() const
{
    if (mMap == nullptr ||
        mMap->getTileHeight() == 0)
    {
        return 0;
    }

    return getPixelY() / mMap->getTileHeight();
}

void Actor::setPixelPositionF(const Vector &restrict pos) restrict2
{
    mPos = pos;
    mPixelX = CAST_S32(mPos.x);
    mPixelY = CAST_S32(mPos.y);
}
