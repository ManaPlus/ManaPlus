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

#ifndef RESOURCES_MAP_TILEANIMATION_H
#define RESOURCES_MAP_TILEANIMATION_H

#include "utils/vector.h"

#include <map>

#include "localconsts.h"

class Animation;
class Image;
class MapLayer;
class SimpleAnimation;

typedef STD_VECTOR<std::pair<MapLayer*, int> > TilePairVector;
typedef TilePairVector::const_iterator TilePairVectorCIter;

/**
 * Animation cycle of a tile image which changes the map accordingly.
 */
class TileAnimation final
{
    public:
        explicit TileAnimation(Animation *const ani);

        ~TileAnimation();

        A_DELETE_COPY(TileAnimation)

        bool update(const int ticks);

        void addAffectedTile(MapLayer *const layer, const int index)
        { mAffected.push_back(std::make_pair(layer, index)); }

    private:
        TilePairVector mAffected;
        SimpleAnimation *mAnimation;
        Image *mLastImage;
};

typedef std::map<int, TileAnimation*> TileAnimationMap;
typedef TileAnimationMap::const_iterator TileAnimationMapCIter;

#endif  // RESOURCES_MAP_TILEANIMATION_H
