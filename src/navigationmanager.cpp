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

#include "navigationmanager.h"

#include "enums/resources/map/blockmask.h"

#include "resources/map/map.h"
#include "resources/map/metatile.h"
#include "resources/map/walklayer.h"

#include "debug.h"

static const int blockWalkMask = (BlockMask::WALL |
    BlockMask::AIR |
    BlockMask::WATER);

#ifndef DYECMD
namespace
{
    struct Cell final
    {
        Cell(const int x0, const int y0) :
            x(x0),
            y(y0)
        {
        }

        A_DEFAULT_COPY(Cell)

        int x;
        int y;
    };
}  // namespace
#endif  // DYECMD

NavigationManager::NavigationManager()
{
}

NavigationManager::~NavigationManager()
{
}

#ifndef DYECMD
Resource *NavigationManager::loadWalkLayer(const Map *const map)
{
    if (map == nullptr)
        return nullptr;

    const int width = map->getWidth();
    const int height = map->getHeight();
    if (width < 2 || height < 2)
        return nullptr;
    WalkLayer *const walkLayer = new WalkLayer(width, height);

    const MetaTile *const tiles = map->getMetaTiles();
    int *const data = walkLayer->getData();
    if ((tiles == nullptr) || (data == nullptr))
        return walkLayer;

    int x = 0;
    int y = 0;
    int num = 1;
    while (findWalkableTile(x, y, width, height, tiles, data))
    {
        fillNum(x, y, width, height, num, tiles, data);
        num ++;
    }

    return walkLayer;
}

void NavigationManager::fillNum(int x, int y,
                                const int width,
                                const int height,
                                const int num,
                                const MetaTile *const tiles,
                                int *const data)
{
    STD_VECTOR<Cell> cells;
    cells.push_back(Cell(x, y));
    while (!cells.empty())
    {
        const Cell cell = cells.back();
        cells.pop_back();
        int ptr;
        x = cell.x;
        y = cell.y;
        data[x + width * y] = num;
        if (x > 0)
        {
            ptr = (x - 1) + width * y;
            if (data[ptr] == 0)
            {
                if ((tiles[ptr].blockmask & blockWalkMask) == 0)
                    cells.push_back(Cell(x - 1, y));
                else
                    data[ptr] = -num;
            }
        }
        if (x < width - 1)
        {
            ptr = (x + 1) + width * y;
            if (data[ptr] == 0)
            {
                if ((tiles[ptr].blockmask & blockWalkMask) == 0)
                    cells.push_back(Cell(x + 1, y));
                else
                    data[ptr] = -num;
            }
        }
        if (y > 0)
        {
            ptr = x + width * (y - 1);
            if (data[ptr] == 0)
            {
                if ((tiles[ptr].blockmask & blockWalkMask) == 0)
                    cells.push_back(Cell(x, y - 1));
                else
                    data[ptr] = -num;
            }
        }
        if (y < height - 1)
        {
            ptr = x + width * (y + 1);
            if (data[ptr] == 0)
            {
                if ((tiles[ptr].blockmask & blockWalkMask) == 0)
                    cells.push_back(Cell(x, y + 1));
                else
                    data[ptr] = -num;
            }
        }
    }
}

bool NavigationManager::findWalkableTile(int &x1, int &y1,
                                         const int width,
                                         const int height,
                                         const MetaTile *const tiles,
                                         const int *const data)
{
    for (int y = 0; y < height; y ++)
    {
        const int y2 = y * width;
        for (int x = 0; x < width; x ++)
        {
            const int ptr = x + y2;
            if (((tiles[ptr].blockmask & blockWalkMask) == 0) &&
                data[ptr] == 0)
            {
                x1 = x;
                y1 = y;
                return true;
            }
        }
    }
    return false;
}
#endif  // DYECMD
