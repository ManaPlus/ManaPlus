/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef RESOURCES_MAP_LOCATION_H
#define RESOURCES_MAP_LOCATION_H

#include "resources/map/metatile.h"

#include "localconsts.h"

/**
 * A location on a tile map. Used for pathfinding, open list.
 */
struct Location final
{
    /**
     * Constructor.
     */
    Location(const int px,
             const int py,
             MetaTile *const ptile) :
        x(px),
        y(py),
        tileCost(ptile->Fcost),
        tile(ptile)
    {}

    A_DEFAULT_COPY(Location)

    /**
     * Comparison operator.
     */
    bool operator< (const Location &loc) const
    {
        return tileCost > loc.tileCost;
    }

    int x, y;
    int tileCost;
    MetaTile *tile;
};

#endif  // RESOURCES_MAP_LOCATION_H
