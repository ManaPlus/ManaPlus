/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2018  The ManaPlus Developers
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

#include "unittests/unittests.h"

#include "configmanager.h"
#include "dirs.h"

#include "enums/resources/map/blockmask.h"

#include "resources/image/image.h"

#include "resources/map/map.h"
#include "resources/map/maplayer.h"

#include "debug.h"

TEST_CASE("MapLayer updateConditionTiles", "")
{
    Dirs::initRootDir();
    Dirs::initHomeDir();

    ConfigManager::initConfiguration();

    Image *const img1 = new Image(32, 32);
    Map *map = nullptr;
    MapLayer *layer = nullptr;

    SECTION("simple 1")
    {
        map = new Map("map",
            1, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            1, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        map->addLayer(layer);
        layer->setTileCondition(BlockMask::WATER);
        TileInfo *const tiles = layer->getTiles();

        map->addBlockMask(0, 0, BlockType::NONE);
        layer->updateConditionTiles(map->getMetaTiles(),
            1, 1);
        REQUIRE(tiles[0].isEnabled == false);

        map->addBlockMask(0, 0, BlockType::WATER);
        layer->updateConditionTiles(map->getMetaTiles(),
            1, 1);
        REQUIRE(tiles[0].isEnabled == true);
    }

    SECTION("normal 1")
    {
        map = new Map("map",
            100, 200,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            100, 200,
            false,
            0,
            0);
        layer->setTile(10, 10, img1);
        layer->setTile(10, 20, img1);
        layer->setTile(10, 30, img1);
        map->addLayer(layer);
        layer->setTileCondition(BlockMask::WATER);
        TileInfo *const tiles = layer->getTiles();

        map->addBlockMask(10, 10, BlockType::NONE);
        map->addBlockMask(10, 20, BlockType::NONE);
        map->addBlockMask(20, 20, BlockType::NONE);
        layer->updateConditionTiles(map->getMetaTiles(),
            100, 200);
        for (int x = 0; x < 100; x ++)
        {
            for (int y = 0; y < 200; y ++)
            {
                REQUIRE(tiles[y * 100 + x].isEnabled == false);
            }
        }
    }

    SECTION("normal 2")
    {
        map = new Map("map",
            100, 200,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            100, 200,
            false,
            0,
            0);
        layer->setTile(10, 10, img1);
        layer->setTile(10, 20, img1);
        layer->setTile(10, 30, img1);
        map->addLayer(layer);
        layer->setTileCondition(BlockMask::WATER);
        TileInfo *const tiles = layer->getTiles();

        map->addBlockMask(10, 10, BlockType::WATER);
        map->addBlockMask(10, 20, BlockType::WATER);
        map->addBlockMask(20, 20, BlockType::WATER);
        layer->updateConditionTiles(map->getMetaTiles(),
            100, 200);
        for (int x = 0; x < 100; x ++)
        {
            for (int y = 0; y < 200; y ++)
            {
                if ((x == 10 && y == 10) || (x == 10 && y == 20))
                {
                    REQUIRE(tiles[y * 100 + x].isEnabled == true);
                }
                else
                {
                    REQUIRE(tiles[y * 100 + x].isEnabled == false);
                }
            }
        }
    }

    SECTION("normal 3")
    {
        map = new Map("map",
            100, 200,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            100, 200,
            false,
            0,
            0);
        for (int x = 0; x < 100; x ++)
        {
            for (int y = 0; y < 200; y ++)
            {
                layer->setTile(x, y, img1);
            }
        }
        map->addLayer(layer);
        layer->setTileCondition(BlockMask::WATER);
        TileInfo *const tiles = layer->getTiles();

        map->addBlockMask(10, 10, BlockType::WATER);
        map->addBlockMask(10, 20, BlockType::WATER);
        layer->updateConditionTiles(map->getMetaTiles(),
            100, 200);
        for (int x = 0; x < 100; x ++)
        {
            for (int y = 0; y < 200; y ++)
            {
                if ((x == 10 && y == 10) || (x == 10 && y == 20))
                {
                    REQUIRE(tiles[y * 100 + x].isEnabled == true);
                }
                else
                {
                    REQUIRE(tiles[y * 100 + x].isEnabled == false);
                }
            }
        }
    }

    SECTION("normal 4")
    {
        map = new Map("map",
            100, 200,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            100, 200,
            false,
            0,
            0);
        layer->setTile(10, 10, img1);
        layer->setTile(10, 20, img1);
        map->addLayer(layer);
        layer->setTileCondition(BlockMask::WATER);
        TileInfo *const tiles = layer->getTiles();

        for (int x = 0; x < 100; x ++)
        {
            for (int y = 0; y < 200; y ++)
            {
                map->addBlockMask(x, y, BlockType::WATER);
            }
        }

        layer->updateConditionTiles(map->getMetaTiles(),
            100, 200);
        for (int x = 0; x < 100; x ++)
        {
            for (int y = 0; y < 200; y ++)
            {
                if ((x == 10 && y == 10) || (x == 10 && y == 20))
                {
                    REQUIRE(tiles[y * 100 + x].isEnabled == true);
                }
                else
                {
                    REQUIRE(tiles[y * 100 + x].isEnabled == false);
                }
            }
        }
    }

    delete map;
    delete img1;
}
