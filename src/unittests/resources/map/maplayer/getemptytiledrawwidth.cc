/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
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

#include "resources/image/image.h"

#include "resources/map/maplayer.h"

#include "debug.h"

TEST_CASE("MapLayer getEmptyTileDrawWidth", "")
{
    Dirs::initRootDir();
    Dirs::initHomeDir();

    ConfigManager::initConfiguration();

    Image *const img1 = new Image(32, 32);
    Image *const img2 = new Image(32, 32);
    Image *const img3 = new Image(32, 32);
    MapLayer *layer = nullptr;
    int nextTile;

    SECTION("simple 2")
    {
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        TileInfo *const tiles = layer->getTiles();
        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 1,
            1,
            nextTile) == 0);
        REQUIRE(nextTile == 0);
    }

    SECTION("simple 4")
    {
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(2, 0, img1);
        TileInfo *const tiles = layer->getTiles();
        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 1,
            2,
            nextTile) == 0);
        REQUIRE(nextTile == 0);
    }

    SECTION("simple 5")
    {
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        TileInfo *const tiles = layer->getTiles();
        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 2,
            1,
            nextTile) == 0);
        REQUIRE(nextTile == 0);
    }

    SECTION("normal 1")
    {
        layer = new MapLayer("test",
            0, 0,
            100, 100,
            false,
            0,
            0);
        layer->setTile(1, 10, img1);
        layer->setTile(2, 10, img1);
        layer->setTile(3, 10, img1);
        layer->setTile(4, 10, img2);
        layer->setTile(5, 10, nullptr);
        layer->setTile(6, 10, img2);
        layer->setTile(7, 10, nullptr);
        layer->setTile(8, 10, nullptr);
        layer->setTile(9, 10, img2);
        layer->setTile(10, 10, img2);
        layer->setTile(11, 10, img3);
        layer->setTile(12, 10, nullptr);
        layer->setTile(13, 10, nullptr);
        layer->setTile(14, 10, nullptr);
        layer->setTile(15, 10, img1);
        layer->setTile(16, 10, img1);
        layer->setTile(17, 10, img1);
        TileInfo *const tiles = layer->getTiles();

        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 10 * 100 + 0,
            100 - 0,
            nextTile) == 0);
        REQUIRE(nextTile == 0);

        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 10 * 100 + 5,
            100 - 5,
            nextTile) == 0);
        REQUIRE(nextTile == 0);

        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 10 * 100 + 7,
            100 - 7,
            nextTile) == 1);
        REQUIRE(nextTile == 1);

        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 10 * 100 + 8,
            100 - 8,
            nextTile) == 0);
        REQUIRE(nextTile == 0);

        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 10 * 100 + 12,
            100 - 12,
            nextTile) == 2);
        REQUIRE(nextTile == 2);

        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 10 * 100 + 13,
            100 - 13,
            nextTile) == 1);
        REQUIRE(nextTile == 1);

        REQUIRE(layer->getEmptyTileDrawWidth(tiles + 10 * 100 + 14,
            100 - 14,
            nextTile) == 0);
        REQUIRE(nextTile == 0);
    }

    delete layer;
    delete img1;
    delete img2;
    delete img3;
}
