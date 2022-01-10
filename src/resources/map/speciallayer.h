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

#ifndef RESOURCES_MAP_SPECIALLAYER_H
#define RESOURCES_MAP_SPECIALLAYER_H

#include "resources/memorycounter.h"

#include "position.h"

#include "localconsts.h"

class Graphics;
class MapItem;

class SpecialLayer final : public MemoryCounter
{
    public:
        friend class Map;
        friend class MapLayer;

        SpecialLayer(const std::string &name,
                     const int width,
                     const int height);

        A_DELETE_COPY(SpecialLayer)

        ~SpecialLayer() override final;

        void draw(Graphics *const graphics,
                  int startX, int startY,
                  int endX, int endY,
                  const int scrollX, const int scrollY) const A_NONNULL(2);

        MapItem* getTile(const int x, const int y) const A_WARN_UNUSED;

        void setTile(const int x, const int y, MapItem *const item);

        void setTile(const int x, const int y, const int type);

        void addRoad(const Path &road);

        void clean();

        int calcMemoryLocal() const override final;

        std::string getCounterName() const override final
        { return mName; }

        void updateCache();

#ifdef UNITTESTS
        const int *getCache() const
        { return mCache; }

        MapItem **getTiles() const
        { return mTiles; }
#endif  // UNITTESTS

    private:
        const std::string mName;
        MapItem **mTiles;
        int *mCache;
        int mWidth;
        int mHeight;
};

#endif  // RESOURCES_MAP_SPECIALLAYER_H
