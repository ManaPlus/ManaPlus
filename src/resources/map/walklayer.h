/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
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

#ifndef RESOURCES_MAP_WALKLAYER_H
#define RESOURCES_MAP_WALKLAYER_H

#include "resources/resource.h"

#include "localconsts.h"

class WalkLayer final : public Resource
{
    public:
        WalkLayer(const int width, const int height);

        A_DELETE_COPY(WalkLayer)

        ~WalkLayer() override final;

        int *getData()
        { return mTiles; }

        int getDataAt(const int x, const int y) const;

        int calcMemoryLocal() const override final;

        std::string getCounterName() const override final
        { return "walk layer"; }

    private:
        int mWidth;
        int mHeight;
        int *mTiles;
};

#endif  // RESOURCES_MAP_WALKLAYER_H
