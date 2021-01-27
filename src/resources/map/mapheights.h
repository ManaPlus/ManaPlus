/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef RESOURCES_MAP_MAPHEIGHTS_H
#define RESOURCES_MAP_MAPHEIGHTS_H

#include "resources/memorycounter.h"

#include "utils/cast.h"

#include "localconsts.h"

class MapHeights final : public MemoryCounter
{
    public:
        friend class Map;

        MapHeights(const int width, const int height);

        A_DELETE_COPY(MapHeights)

        ~MapHeights() override final;

        void setHeight(const int x, const int y, const uint8_t height);

        uint8_t getHeight(const int x, const int y) const
        {
            return (x < mWidth &&
                y < mHeight) ? mTiles[x + y * mWidth] : CAST_U8(0U);
        }

        int calcMemoryLocal() const override final;

        std::string getCounterName() const override final
        { return "heights layer"; }

    private:
        int mWidth;
        int mHeight;
        uint8_t *mTiles;
};

#endif  // RESOURCES_MAP_MAPHEIGHTS_H
