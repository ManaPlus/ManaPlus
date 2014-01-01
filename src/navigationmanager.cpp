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

#include "navigationmanager.h"

#include "map.h"
#include "walklayer.h"

static const int walkMask = (Map::BLOCKMASK_WALL | Map::BLOCKMASK_AIR
    | Map::BLOCKMASK_WATER);

namespace
{
    struct Cell
    {
        Cell(const int x0, const int y0) :
            x(x0),
            y(y0)
        {
        }

        int x;
        int y;
    };
}  // namespace

NavigationManager::NavigationManager()
{
}

NavigationManager::~NavigationManager()
{
}

Resource *NavigationManager::loadWalkLayer(const Map *const map)
{
    if (!map)
        return nullptr;

    const int width = map->getWidth();
    const int height = map->getHeight();
    if (width < 2 || height < 2)
        return nullptr;
    WalkLayer *const walkLayer = new WalkLayer(width, height);

    const MetaTile *const tiles = map->getMetaTiles();
    int *const data = walkLayer->getData();

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

bool NavigationManager::findWalkableTile(int &x1, int &y1,
                                         const int width, const int height,
                                         const MetaTile *const tiles,
                                         const int *const data)
{
    for (int y = 0; y < height; y ++)
    {
        const int y2 = y * width;
        for (int x = 0; x < width; x ++)
        {
            const int ptr = x + y2;
            if (!(tiles[ptr].blockmask & walkMask) && !data[ptr])
            {
                x1 = x;
                y1 = y;
                return true;
            }
        }
    }
    return false;
}

void NavigationManager::fillNum(int x, int y,
                                const int width, const int height,
                                const int num, const MetaTile *const tiles,
                                int *const data)
{
    std::vector<Cell> cells;
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
            if (!data[ptr])
            {
                if (!(tiles[ptr].blockmask & walkMask))
                    cells.push_back(Cell(x - 1, y));
                else
                    data[ptr] = -num;
            }
        }
        if (x < width - 1)
        {
            ptr = (x + 1) + width * y;
            if (!data[ptr])
            {
                if (!(tiles[ptr].blockmask & walkMask))
                    cells.push_back(Cell(x + 1, y));
                else
                    data[ptr] = -num;
            }
        }
        if (y > 0)
        {
            ptr = x + width * (y - 1);
            if (!data[ptr])
            {
                if (!(tiles[ptr].blockmask & walkMask))
                    cells.push_back(Cell(x, y - 1));
                else
                    data[ptr] = -num;
            }
        }
        if (y < height - 1)
        {
            ptr = x + width * (y + 1);
            if (!data[ptr])
            {
                if (!(tiles[ptr].blockmask & walkMask))
                    cells.push_back(Cell(x, y + 1));
                else
                    data[ptr] = -num;
            }
        }
    }
}
