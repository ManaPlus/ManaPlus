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

#ifndef NAVIGATIONMANAGER_H
#define NAVIGATIONMANAGER_H

#include "localconsts.h"

class Map;
class Resource;

struct MetaTile;

class NavigationManager final
{
    public:
        NavigationManager();

        A_DELETE_COPY(NavigationManager)

        ~NavigationManager();

        static Resource *loadWalkLayer(const Map *const map);

    private:
        static bool findWalkableTile(int &x1, int &y1,
                                     const int width, const int height,
                                     const MetaTile *const tiles,
                                     const int *const data);

        static void fillNum(int x, int y,
                            const int width, const int height,
                            const int num, const MetaTile *const tiles,
                            int *const data);
};

#endif  // NAVIGATIONMANAGER_H
