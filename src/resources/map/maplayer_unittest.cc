/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2017  The ManaPlus Developers
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

#include "catch.hpp"
#include "graphicsmanager.h"

#include "being/localplayer.h"

#include "enums/resources/map/blockmask.h"
#include "enums/resources/map/mapitemtype.h"

#include "fs/virtfs.h"

#include "gui/theme.h"

#include "utils/delete2.h"

#include "render/mockgraphics.h"

#include "resources/sdlimagehelper.h"

#include "resources/image/image.h"

#include "resources/map/map.h"
#include "resources/map/maplayer.h"
#include "resources/map/speciallayer.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

TEST_CASE("MapLayer getTileDrawWidth")
{
    Image *const img1 = new Image(32, 32);
    Image *const img2 = new Image(32, 32);
    Image *const img3 = new Image(32, 32);
    MapLayer *layer = nullptr;
    int width;
    int nextTile;

    SECTION("simple 1")
    {
        layer = new MapLayer("test",
            0, 0,
            1, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        TileInfo *const tiles = layer->getTiles();
        REQUIRE(layer->getTileDrawWidth(tiles,
            1,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);
    }

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
        REQUIRE(layer->getTileDrawWidth(tiles,
            2,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 1);
    }

    SECTION("simple 3")
    {
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img2);
        TileInfo *const tiles = layer->getTiles();
        REQUIRE(layer->getTileDrawWidth(tiles,
            1,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);

        REQUIRE(layer->getTileDrawWidth(tiles + 1,
            1,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);
    }

    SECTION("simple 4")
    {
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        TileInfo *const tiles = layer->getTiles();
        REQUIRE(layer->getTileDrawWidth(tiles,
            2,
            width,
            nextTile) == 1);
        REQUIRE(width == 64);
        REQUIRE(nextTile == 1);

        REQUIRE(layer->getTileDrawWidth(tiles + 1,
            1,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);
    }

    SECTION("simple 4.2")
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
        REQUIRE(layer->getTileDrawWidth(tiles,
            3,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 1);

        REQUIRE(layer->getTileDrawWidth(tiles + 2,
            1,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
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
        REQUIRE(layer->getTileDrawWidth(tiles,
            3,
            width,
            nextTile) == 1);
        REQUIRE(width == 64);
        REQUIRE(nextTile == 2);

        REQUIRE(layer->getTileDrawWidth(tiles + 1,
            2,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 1);
    }

    SECTION("simple 6")
    {
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        TileInfo *const tiles = layer->getTiles();
        REQUIRE(layer->getTileDrawWidth(tiles,
            3,
            width,
            nextTile) == 1);
        REQUIRE(width == 64);
        REQUIRE(nextTile == 1);

        REQUIRE(layer->getTileDrawWidth(tiles + 1,
            2,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);

        REQUIRE(layer->getTileDrawWidth(tiles + 2,
            1,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);
    }

    SECTION("simple 7")
    {
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        TileInfo *const tiles = layer->getTiles();
        tiles[1].isEnabled = false;
        REQUIRE(layer->getTileDrawWidth(tiles,
            3,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 1);

//        REQUIRE(layer->getTileDrawWidth(tiles + 1,
//            2,
//            width,
//            nextTile) == 0);
//        REQUIRE(width == 32);
//        REQUIRE(nextTile == 0);

        REQUIRE(layer->getTileDrawWidth(tiles + 2,
            1,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);
    }

    SECTION("simple 8")
    {
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        TileInfo *const tiles = layer->getTiles();
        tiles[0].isEnabled = false;
//        REQUIRE(layer->getTileDrawWidth(tiles,
//            3,
//            width,
//            nextTile) == 1);
//        REQUIRE(width == 0);
//        REQUIRE(nextTile == 1);

        REQUIRE(layer->getTileDrawWidth(tiles + 1,
            2,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);

        REQUIRE(layer->getTileDrawWidth(tiles + 2,
            1,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
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

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 1,
            100 - 1,
            width,
            nextTile) == 2);
        REQUIRE(width == 96);
        REQUIRE(nextTile == 2);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 2,
            100 - 2,
            width,
            nextTile) == 1);
        REQUIRE(width == 64);
        REQUIRE(nextTile == 1);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 3,
            100 - 3,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 4,
            100 - 4,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 1);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 6,
            100 - 6,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 2);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 9,
            100 - 9,
            width,
            nextTile) == 1);
        REQUIRE(width == 64);
        REQUIRE(nextTile == 1);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 10,
            100 - 10,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 0);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 11,
            100 - 11,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 3);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 15,
            100 - 15,
            width,
            nextTile) == 2);
        REQUIRE(width == 96);
        REQUIRE(nextTile == 84);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 16,
            100 - 16,
            width,
            nextTile) == 1);
        REQUIRE(width == 64);
        REQUIRE(nextTile == 83);

        REQUIRE(layer->getTileDrawWidth(tiles + 10 * 100 + 17,
            100 - 17,
            width,
            nextTile) == 0);
        REQUIRE(width == 32);
        REQUIRE(nextTile == 82);
    }

    delete layer;
    delete img1;
    delete img2;
    delete img3;
}


TEST_CASE("MapLayer getEmptyTileDrawWidth")
{
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


TEST_CASE("MapLayer updateCache")
{
    Image *const img1 = new Image(32, 32);
    Image *const img2 = new Image(32, 32);
    Image *const img3 = new Image(32, 32);
    MapLayer *layer = nullptr;

    SECTION("simple 1")
    {
        layer = new MapLayer("test",
            0, 0,
            1, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(1, 1);
        REQUIRE(tiles[0].isEnabled == true);
        REQUIRE(tiles[0].width == 32);
        REQUIRE(tiles[0].count == 0);
        REQUIRE(tiles[0].nextTile == 0);
    }

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
        layer->updateCache(2, 1);
        REQUIRE(tiles[0].isEnabled == true);
        REQUIRE(tiles[0].width == 32);
        REQUIRE(tiles[0].count == 0);
        REQUIRE(tiles[0].nextTile == 1);
        REQUIRE(tiles[1].isEnabled == false);
        REQUIRE(tiles[1].width == 0);
        REQUIRE(tiles[1].count == 0);
        REQUIRE(tiles[1].nextTile == 0);
    }

    SECTION("simple 3")
    {
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img2);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(2, 1);
        REQUIRE(tiles[0].isEnabled == true);
        REQUIRE(tiles[0].width == 32);
        REQUIRE(tiles[0].count == 0);
        REQUIRE(tiles[0].nextTile == 0);
        REQUIRE(tiles[1].isEnabled == true);
        REQUIRE(tiles[1].width == 32);
        REQUIRE(tiles[1].count == 0);
        REQUIRE(tiles[1].nextTile == 0);
    }

    SECTION("simple 4")
    {
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(2, 1);
        REQUIRE(tiles[0].isEnabled == true);
        REQUIRE(tiles[0].width == 64);
        REQUIRE(tiles[0].count == 1);
        REQUIRE(tiles[0].nextTile == 1);
        REQUIRE(tiles[1].isEnabled == true);
        REQUIRE(tiles[1].width == 32);
        REQUIRE(tiles[1].count == 0);
        REQUIRE(tiles[1].nextTile == 0);
    }

    SECTION("simple 4.2")
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
        layer->updateCache(3, 1);
        REQUIRE(tiles[0].isEnabled == true);
        REQUIRE(tiles[0].width == 32);
        REQUIRE(tiles[0].count == 0);
        REQUIRE(tiles[0].nextTile == 1);
        REQUIRE(tiles[1].isEnabled == false);
        REQUIRE(tiles[1].width == 0);
        REQUIRE(tiles[1].count == 0);
        REQUIRE(tiles[1].nextTile == 0);
        REQUIRE(tiles[2].isEnabled == true);
        REQUIRE(tiles[2].width == 32);
        REQUIRE(tiles[2].count == 0);
        REQUIRE(tiles[2].nextTile == 0);
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
        layer->updateCache(3, 1);
        REQUIRE(tiles[0].isEnabled == true);
        REQUIRE(tiles[0].width == 64);
        REQUIRE(tiles[0].count == 1);
        REQUIRE(tiles[0].nextTile == 2);
        REQUIRE(tiles[1].isEnabled == true);
        REQUIRE(tiles[1].width == 32);
        REQUIRE(tiles[1].count == 0);
        REQUIRE(tiles[1].nextTile == 1);
        REQUIRE(tiles[2].isEnabled == false);
        REQUIRE(tiles[2].width == 0);
        REQUIRE(tiles[2].count == 0);
        REQUIRE(tiles[2].nextTile == 0);
    }

    SECTION("simple 6")
    {
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(3, 1);
        REQUIRE(tiles[0].isEnabled == true);
        REQUIRE(tiles[0].width == 64);
        REQUIRE(tiles[0].count == 1);
        REQUIRE(tiles[0].nextTile == 1);
        REQUIRE(tiles[1].isEnabled == true);
        REQUIRE(tiles[1].width == 32);
        REQUIRE(tiles[1].count == 0);
        REQUIRE(tiles[1].nextTile == 0);
        REQUIRE(tiles[2].isEnabled == true);
        REQUIRE(tiles[2].width == 32);
        REQUIRE(tiles[2].count == 0);
        REQUIRE(tiles[2].nextTile == 0);
    }

    SECTION("simple 7")
    {
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        TileInfo *const tiles = layer->getTiles();
        tiles[0].isEnabled = false;
        layer->updateCache(3, 1);
        REQUIRE(tiles[0].isEnabled == false);
        REQUIRE(tiles[0].width == 0);
        REQUIRE(tiles[0].count == 0);
        REQUIRE(tiles[0].nextTile == 0);
        REQUIRE(tiles[1].isEnabled == true);
        REQUIRE(tiles[1].width == 32);
        REQUIRE(tiles[1].count == 0);
        REQUIRE(tiles[1].nextTile == 0);
        REQUIRE(tiles[2].isEnabled == true);
        REQUIRE(tiles[2].width == 32);
        REQUIRE(tiles[2].count == 0);
        REQUIRE(tiles[2].nextTile == 0);
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
        layer->updateCache(100, 100);

        REQUIRE(tiles[0 * 100 + 0].isEnabled == false);
        REQUIRE(tiles[0 * 100 + 0].width == 0);
        REQUIRE(tiles[0 * 100 + 0].count == 99);
        REQUIRE(tiles[0 * 100 + 0].nextTile == 99);

        REQUIRE(tiles[0 * 100 + 1].isEnabled == false);
        REQUIRE(tiles[0 * 100 + 1].width == 0);
        REQUIRE(tiles[0 * 100 + 1].count == 98);
        REQUIRE(tiles[0 * 100 + 1].nextTile == 98);

        REQUIRE(tiles[10 * 100 + 0].isEnabled == false);
        REQUIRE(tiles[10 * 100 + 0].width == 0);
        REQUIRE(tiles[10 * 100 + 0].count == 0);
        REQUIRE(tiles[10 * 100 + 0].nextTile == 0);

        REQUIRE(tiles[10 * 100 + 1].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 1].width == 96);
        REQUIRE(tiles[10 * 100 + 1].count == 2);
        REQUIRE(tiles[10 * 100 + 1].nextTile == 2);

        REQUIRE(tiles[10 * 100 + 2].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 2].width == 64);
        REQUIRE(tiles[10 * 100 + 2].count == 1);
        REQUIRE(tiles[10 * 100 + 2].nextTile == 1);

        REQUIRE(tiles[10 * 100 + 3].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 3].width == 32);
        REQUIRE(tiles[10 * 100 + 3].count == 0);
        REQUIRE(tiles[10 * 100 + 3].nextTile == 0);

        REQUIRE(tiles[10 * 100 + 4].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 4].width == 32);
        REQUIRE(tiles[10 * 100 + 4].count == 0);
        REQUIRE(tiles[10 * 100 + 4].nextTile == 1);

        REQUIRE(tiles[10 * 100 + 5].isEnabled == false);
        REQUIRE(tiles[10 * 100 + 5].width == 0);
        REQUIRE(tiles[10 * 100 + 5].count == 0);
        REQUIRE(tiles[10 * 100 + 5].nextTile == 0);

        REQUIRE(tiles[10 * 100 + 6].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 6].width == 32);
        REQUIRE(tiles[10 * 100 + 6].count == 0);
        REQUIRE(tiles[10 * 100 + 6].nextTile == 2);

        REQUIRE(tiles[10 * 100 + 7].isEnabled == false);
        REQUIRE(tiles[10 * 100 + 7].width == 0);
        REQUIRE(tiles[10 * 100 + 7].count == 1);
        REQUIRE(tiles[10 * 100 + 7].nextTile == 1);

        REQUIRE(tiles[10 * 100 + 8].isEnabled == false);
        REQUIRE(tiles[10 * 100 + 8].width == 0);
        REQUIRE(tiles[10 * 100 + 8].count == 0);
        REQUIRE(tiles[10 * 100 + 8].nextTile == 0);

        REQUIRE(tiles[10 * 100 + 9].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 9].width == 64);
        REQUIRE(tiles[10 * 100 + 9].count == 1);
        REQUIRE(tiles[10 * 100 + 9].nextTile == 1);

        REQUIRE(tiles[10 * 100 + 10].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 10].width == 32);
        REQUIRE(tiles[10 * 100 + 10].count == 0);
        REQUIRE(tiles[10 * 100 + 10].nextTile == 0);

        REQUIRE(tiles[10 * 100 + 11].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 11].width == 32);
        REQUIRE(tiles[10 * 100 + 11].count == 0);
        REQUIRE(tiles[10 * 100 + 11].nextTile == 3);

        REQUIRE(tiles[10 * 100 + 12].isEnabled == false);
        REQUIRE(tiles[10 * 100 + 12].width == 0);
        REQUIRE(tiles[10 * 100 + 12].count == 2);
        REQUIRE(tiles[10 * 100 + 12].nextTile == 2);

        REQUIRE(tiles[10 * 100 + 13].isEnabled == false);
        REQUIRE(tiles[10 * 100 + 13].width == 0);
        REQUIRE(tiles[10 * 100 + 13].count == 1);
        REQUIRE(tiles[10 * 100 + 13].nextTile == 1);

        REQUIRE(tiles[10 * 100 + 14].isEnabled == false);
        REQUIRE(tiles[10 * 100 + 14].width == 0);
        REQUIRE(tiles[10 * 100 + 14].count == 0);
        REQUIRE(tiles[10 * 100 + 14].nextTile == 0);

        REQUIRE(tiles[10 * 100 + 15].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 15].width == 96);
        REQUIRE(tiles[10 * 100 + 15].count == 2);
        REQUIRE(tiles[10 * 100 + 15].nextTile == 84);

        REQUIRE(tiles[10 * 100 + 16].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 16].width == 64);
        REQUIRE(tiles[10 * 100 + 16].count == 1);
        REQUIRE(tiles[10 * 100 + 16].nextTile == 83);

        REQUIRE(tiles[10 * 100 + 17].isEnabled == true);
        REQUIRE(tiles[10 * 100 + 17].width == 32);
        REQUIRE(tiles[10 * 100 + 17].count == 0);
        REQUIRE(tiles[10 * 100 + 17].nextTile == 82);
    }

    SECTION("normal2")
    {
        const int maxX = 100;
        const int maxY = 100;
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
            false,
            0,
            0);
        TileInfo *const tiles = layer->getTiles();
        for (int x = 0; x < maxX; x ++)
        {
            for (int y = 0; y < maxY; y ++)
            {
                layer->setTile(x, y, img1);
                tiles[y * maxX + x].isEnabled = false;
            }
        }
        tiles[10 * maxX + 41].isEnabled = true;
        layer->updateCache(maxX, maxY);

        REQUIRE(tiles[10 * maxX + 0].isEnabled == false);
        REQUIRE(tiles[10 * maxX + 0].width == 0);
        REQUIRE(tiles[10 * maxX + 0].count == 40);
        REQUIRE(tiles[10 * maxX + 0].nextTile == 40);

        REQUIRE(tiles[10 * maxX + 1].isEnabled == false);
        REQUIRE(tiles[10 * maxX + 1].width == 0);
        REQUIRE(tiles[10 * maxX + 1].count == 39);
        REQUIRE(tiles[10 * maxX + 1].nextTile == 39);
    }

    delete layer;
    delete img1;
    delete img2;
    delete img3;
}

TEST_CASE("MapLayer updateConditionTiles")
{
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

TEST_CASE("MapLayer draw")
{
    Image *const img1 = new Image(32, 32);
    Image *const img2 = new Image(32, 32);
    Image *const img3 = new Image(32, 32);
    Map *map = nullptr;
    MapLayer *layer = nullptr;
    MockGraphics *const mock = new MockGraphics;

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
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(1, 1);

        layer->draw(mock,
            0, 0,
            1, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            1, 1,
            10, 5);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == -10);
        REQUIRE(mock->mDraws[0].y == -5);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            1, 1,
            -10, -5);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == 5);
        REQUIRE(mock->mDraws[0].image == img1);
    }

    SECTION("simple 2")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        map->addLayer(layer);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(2, 1);

        layer->draw(mock,
            0, 0,
            2, 1,
            0, 0);

        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            2, 1,
            10, 5);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == -10);
        REQUIRE(mock->mDraws[0].y == -5);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            2, 1,
            -10, -5);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == 5);
        REQUIRE(mock->mDraws[0].image == img1);
    }

    SECTION("simple 3")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img2);
        map->addLayer(layer);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(2, 1);

        layer->draw(mock,
            0, 0,
            2, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[1].image == img2);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            2, 1,
            -10, -20);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == 20);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 42);
        REQUIRE(mock->mDraws[1].y == 20);
        REQUIRE(mock->mDraws[1].image == img2);
    }

    SECTION("simple 4")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        map->addLayer(layer);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(2, 1);

        layer->draw(mock,
            0, 0,
            2, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            2, 1,
            -10, 20);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == -20);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
    }

    SECTION("simple 4.2")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(2, 0, img1);
        map->addLayer(layer);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(3, 1);

        layer->draw(mock,
            0, 0,
            3, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[1].image == img1);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            3, 1,
            10, -20);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == -10);
        REQUIRE(mock->mDraws[0].y == 20);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 54);
        REQUIRE(mock->mDraws[1].y == 20);
        REQUIRE(mock->mDraws[1].image == img1);
    }

    SECTION("simple 5")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        map->addLayer(layer);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(3, 1);

        layer->draw(mock,
            0, 0,
            3, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            3, 1,
            -10, 20);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == -20);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
    }

    SECTION("simple 6")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        map->addLayer(layer);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(3, 1);

        layer->draw(mock,
            0, 0,
            3, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[1].image == img2);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            3, 1,
            -10, 20);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == -20);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 74);
        REQUIRE(mock->mDraws[1].y == -20);
        REQUIRE(mock->mDraws[1].image == img2);
    }

    SECTION("simple 7")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            false,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        map->addLayer(layer);
        TileInfo *const tiles = layer->getTiles();
        tiles[0].isEnabled = false;
        layer->updateCache(3, 1);

        layer->draw(mock,
            0, 0,
            3, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[1].image == img2);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            3, 1,
            -10, 20);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 42);
        REQUIRE(mock->mDraws[0].y == -20);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 74);
        REQUIRE(mock->mDraws[1].y == -20);
        REQUIRE(mock->mDraws[1].image == img2);
    }

    SECTION("normal 1")
    {
        const int maxX = 100;
        const int maxY = 100;
        map = new Map("map",
            maxX, maxY,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
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
        map->addLayer(layer);
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(maxX, maxY);

        layer->draw(mock,
            0, 0,
            maxX, maxY,
            0, 0);
        REQUIRE(mock->mDraws.size() == 6);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 32 * 1);
        REQUIRE(mock->mDraws[0].y == 32 * 10);
        REQUIRE(mock->mDraws[0].width == 96);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32 * 4);
        REQUIRE(mock->mDraws[1].y == 32 * 10);
        REQUIRE(mock->mDraws[1].image == img2);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 32 * 6);
        REQUIRE(mock->mDraws[2].y == 32 * 10);
        REQUIRE(mock->mDraws[2].image == img2);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[3].x == 32 * 9);
        REQUIRE(mock->mDraws[3].y == 32 * 10);
        REQUIRE(mock->mDraws[3].width == 64);
        REQUIRE(mock->mDraws[3].height == 32);
        REQUIRE(mock->mDraws[3].image == img2);
        REQUIRE(mock->mDraws[4].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[4].x == 32 * 11);
        REQUIRE(mock->mDraws[4].y == 32 * 10);
        REQUIRE(mock->mDraws[4].image == img3);
        REQUIRE(mock->mDraws[5].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[5].x == 32 * 15);
        REQUIRE(mock->mDraws[5].y == 32 * 10);
        REQUIRE(mock->mDraws[5].width == 96);
        REQUIRE(mock->mDraws[5].height == 32);
        REQUIRE(mock->mDraws[5].image == img1);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            maxX, maxY,
            -10, 20);
        REQUIRE(mock->mDraws.size() == 6);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 32 * 1 + 10);
        REQUIRE(mock->mDraws[0].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[0].width == 96);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32 * 4 + 10);
        REQUIRE(mock->mDraws[1].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[1].image == img2);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 32 * 6 + 10);
        REQUIRE(mock->mDraws[2].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[2].image == img2);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[3].x == 32 * 9 + 10);
        REQUIRE(mock->mDraws[3].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[3].width == 64);
        REQUIRE(mock->mDraws[3].height == 32);
        REQUIRE(mock->mDraws[3].image == img2);
        REQUIRE(mock->mDraws[4].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[4].x == 32 * 11 + 10);
        REQUIRE(mock->mDraws[4].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[4].image == img3);
        REQUIRE(mock->mDraws[5].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[5].x == 32 * 15 + 10);
        REQUIRE(mock->mDraws[5].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[5].width == 96);
        REQUIRE(mock->mDraws[5].height == 32);
        REQUIRE(mock->mDraws[5].image == img1);
    }

    SECTION("normal2")
    {
        const int maxX = 100;
        const int maxY = 100;
        map = new Map("map",
            maxX, maxY,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
            false,
            0,
            0);
        TileInfo *const tiles = layer->getTiles();
        map->addLayer(layer);
        for (int x = 0; x < maxX; x ++)
        {
            for (int y = 0; y < maxY; y ++)
            {
                layer->setTile(x, y, img1);
                tiles[y * maxX + x].isEnabled = false;
            }
        }
        tiles[10 * maxX + 41].isEnabled = true;
        layer->updateCache(maxX, maxY);

        layer->draw(mock,
            0, 0,
            maxX, maxY,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32 * 41);
        REQUIRE(mock->mDraws[0].y == 32 * 10);

        mock->mDraws.clear();
        layer->draw(mock,
            0, 0,
            maxX, maxY,
            -10, 20);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32 * 41 + 10);
        REQUIRE(mock->mDraws[0].y == 32 * 10 - 20);
    }

    delete map;
    delete img1;
    delete img2;
    delete img3;
    delete mock;
}

TEST_CASE("MapLayer drawSpecialLayer (specialLayer)")
{
    logger = new Logger;
    ResourceManager::init();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    imageHelper = new SDLImageHelper;
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    Map *map = nullptr;
    MapLayer *layer = nullptr;
    SpecialLayer *specialLayer = nullptr;
    MockGraphics *const mock = new MockGraphics;
    const Actors actors;

    SECTION("simple 1")
    {
        map = new Map("map",
            1, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            1, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();

        layer->drawSpecialLayer(mock,
            0,
            0, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 0);

        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->drawSpecialLayer(mock,
            0,
            0, 1,
            0, 0);
//        REQUIRE(mock->mDraws.size() == 1);
    }

    SECTION("simple 2")
    {
        map = new Map("map",
            1, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            1, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
    }

    SECTION("simple 3")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 2,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
    }

    SECTION("simple 4")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();
        specialLayer->setTile(1, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 2,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32);
        REQUIRE(mock->mDraws[0].y == 0);
    }

    SECTION("simple 5")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->setTile(1, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 2,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32);
        REQUIRE(mock->mDraws[1].y == 0);
    }

    SECTION("simple 6")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->setTile(2, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 3,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
    }

    SECTION("simple 7")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();
        specialLayer->setTile(1, 0, MapItemType::ARROW_UP);
        specialLayer->setTile(2, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 3,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
    }

    SECTION("simple 8")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->setTile(1, 0, MapItemType::ARROW_DOWN);
        specialLayer->setTile(2, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 3,
            0, 0);
        REQUIRE(mock->mDraws.size() == 3);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 64);
        REQUIRE(mock->mDraws[2].y == 0);
    }

    SECTION("normal 1")
    {
        const int maxX = 100;
        const int maxY = 50;
        map = new Map("map",
            maxX, maxY,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getSpecialLayer();
        specialLayer->setTile(0, 5, MapItemType::ARROW_UP);
        specialLayer->setTile(1, 5, MapItemType::ARROW_DOWN);
        specialLayer->setTile(2, 5, MapItemType::ARROW_UP);
        specialLayer->setTile(3, 5, MapItemType::EMPTY);
        specialLayer->setTile(4, 5, MapItemType::EMPTY);
        specialLayer->setTile(6, 5, MapItemType::ARROW_LEFT);
        specialLayer->setTile(10, 20, MapItemType::ARROW_LEFT);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            5,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.size() == 4);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0 * 32);
        REQUIRE(mock->mDraws[0].y == 5 * 32);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 1 * 32);
        REQUIRE(mock->mDraws[1].y == 5 * 32);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 2 * 32);
        REQUIRE(mock->mDraws[2].y == 5 * 32);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[3].x == 6 * 32);
        REQUIRE(mock->mDraws[3].y == 5 * 32);

        mock->mDraws.clear();
        layer->drawSpecialLayer(mock,
            4,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.size() == 0);

        layer->drawSpecialLayer(mock,
            6,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.size() == 0);

        layer->drawSpecialLayer(mock,
            20,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 10 * 32);
        REQUIRE(mock->mDraws[0].y == 20 * 32);
    }

    delete map;
    delete mock;
    GraphicsManager::deleteRenderers();
    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("MapLayer drawSpecialLayer (tempLayer)")
{
    logger = new Logger;
    ResourceManager::init();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    imageHelper = new SDLImageHelper;
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    Map *map = nullptr;
    MapLayer *layer = nullptr;
    SpecialLayer *specialLayer = nullptr;
    MockGraphics *const mock = new MockGraphics;
    const Actors actors;

    SECTION("simple 1")
    {
        map = new Map("map",
            1, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            1, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();

        layer->drawSpecialLayer(mock,
            0,
            0, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 0);

        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->drawSpecialLayer(mock,
            0,
            0, 1,
            0, 0);
//        REQUIRE(mock->mDraws.size() == 1);
    }

    SECTION("simple 2")
    {
        map = new Map("map",
            1, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            1, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 1,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
    }

    SECTION("simple 3")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 2,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
    }

    SECTION("simple 4")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();
        const int *const cache = specialLayer->getCache();
        REQUIRE(cache[0] == 10000);
        REQUIRE(cache[1] == 10000);

        specialLayer->setTile(1, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();
        REQUIRE(cache[0] == 0);
        REQUIRE(cache[1] == 10000);

        layer->drawSpecialLayer(mock,
            0,
            0, 2,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32);
        REQUIRE(mock->mDraws[0].y == 0);
    }

    SECTION("simple 5")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->setTile(1, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 2,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32);
        REQUIRE(mock->mDraws[1].y == 0);
    }

    SECTION("simple 6")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->setTile(2, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 3,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
    }

    SECTION("simple 7")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();
        specialLayer->setTile(1, 0, MapItemType::ARROW_UP);
        specialLayer->setTile(2, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 3,
            0, 0);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
    }

    SECTION("simple 8")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();
        specialLayer->setTile(0, 0, MapItemType::ARROW_UP);
        specialLayer->setTile(1, 0, MapItemType::ARROW_DOWN);
        specialLayer->setTile(2, 0, MapItemType::ARROW_UP);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            0,
            0, 3,
            0, 0);
        REQUIRE(mock->mDraws.size() == 3);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 64);
        REQUIRE(mock->mDraws[2].y == 0);
    }

    SECTION("normal 1")
    {
        const int maxX = 100;
        const int maxY = 50;
        map = new Map("map",
            maxX, maxY,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
            true,
            0,
            0);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        specialLayer = map->getTempLayer();
        specialLayer->setTile(0, 5, MapItemType::ARROW_UP);
        specialLayer->setTile(1, 5, MapItemType::ARROW_DOWN);
        specialLayer->setTile(2, 5, MapItemType::ARROW_UP);
        specialLayer->setTile(3, 5, MapItemType::EMPTY);
        specialLayer->setTile(4, 5, MapItemType::EMPTY);
        specialLayer->setTile(6, 5, MapItemType::ARROW_LEFT);
        specialLayer->setTile(10, 20, MapItemType::ARROW_LEFT);
        specialLayer->updateCache();

        layer->drawSpecialLayer(mock,
            5,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.size() == 4);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0 * 32);
        REQUIRE(mock->mDraws[0].y == 5 * 32);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 1 * 32);
        REQUIRE(mock->mDraws[1].y == 5 * 32);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 2 * 32);
        REQUIRE(mock->mDraws[2].y == 5 * 32);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[3].x == 6 * 32);
        REQUIRE(mock->mDraws[3].y == 5 * 32);

        mock->mDraws.clear();
        layer->drawSpecialLayer(mock,
            4,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.size() == 0);

        layer->drawSpecialLayer(mock,
            6,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.size() == 0);

        layer->drawSpecialLayer(mock,
            20,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 10 * 32);
        REQUIRE(mock->mDraws[0].y == 20 * 32);
    }

    delete map;
    delete mock;
    GraphicsManager::deleteRenderers();
    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}

TEST_CASE("MapLayer drawFringe")
{
    logger = new Logger;
    ResourceManager::init();
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    imageHelper = new SDLImageHelper;
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    theme = new Theme;
    Theme::selectSkin();

    localPlayer = new LocalPlayer(static_cast<BeingId>(1),
        BeingTypeId_zero);

    Image *const img1 = new Image(32, 32);
    Image *const img2 = new Image(32, 32);
    Image *const img3 = new Image(32, 32);
    Map *map = nullptr;
    MapLayer *layer = nullptr;
    MockGraphics *const mock = new MockGraphics;
    const Actors actors;

    SECTION("simple 1")
    {
        map = new Map("map",
            1, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            1, 1,
            true,
            0,
            0);
        layer->setTile(0, 0, img1);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(1, 1);

        layer->drawFringe(mock,
            0, 0,
            1, 1,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            1, 1,
            10, 5,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == -10);
        REQUIRE(mock->mDraws[0].y == -5);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            1, 1,
            -10, -5,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == 5);
        REQUIRE(mock->mDraws[0].image == img1);
    }

    SECTION("simple 2")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        layer->setTile(0, 0, img1);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(2, 1);

        layer->drawFringe(mock,
            0, 0,
            2, 1,
            0, 0,
            actors);

        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            2, 1,
            10, 5,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == -10);
        REQUIRE(mock->mDraws[0].y == -5);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            2, 1,
            -10, -5,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == 5);
        REQUIRE(mock->mDraws[0].image == img1);
    }

    SECTION("simple 3")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img2);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(2, 1);

        layer->drawFringe(mock,
            0, 0,
            2, 1,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[1].image == img2);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            2, 1,
            -10, -20,
            actors);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == 20);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 42);
        REQUIRE(mock->mDraws[1].y == 20);
        REQUIRE(mock->mDraws[1].image == img2);
    }

    SECTION("simple 4")
    {
        map = new Map("map",
            2, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            2, 1,
            true,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(2, 1);

        layer->drawFringe(mock,
            0, 0,
            2, 1,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            2, 1,
            -10, 20,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == -20);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
    }

    SECTION("simple 4.2")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(2, 0, img1);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(3, 1);

        layer->drawFringe(mock,
            0, 0,
            3, 1,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[1].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            3, 1,
            10, -20,
            actors);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == -10);
        REQUIRE(mock->mDraws[0].y == 20);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 54);
        REQUIRE(mock->mDraws[1].y == 20);
        REQUIRE(mock->mDraws[1].image == img1);
    }

    SECTION("simple 5")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(3, 1);

        layer->drawFringe(mock,
            0, 0,
            3, 1,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            3, 1,
            -10, 20,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == -20);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
    }

    SECTION("simple 6")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(3, 1);

        layer->drawFringe(mock,
            0, 0,
            3, 1,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 0);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[1].image == img2);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            3, 1,
            -10, 20,
            actors);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 10);
        REQUIRE(mock->mDraws[0].y == -20);
        REQUIRE(mock->mDraws[0].width == 64);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 74);
        REQUIRE(mock->mDraws[1].y == -20);
        REQUIRE(mock->mDraws[1].image == img2);
    }

    SECTION("simple 7")
    {
        map = new Map("map",
            3, 1,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            3, 1,
            true,
            0,
            0);
        layer->setTile(0, 0, img1);
        layer->setTile(1, 0, img1);
        layer->setTile(2, 0, img2);
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        tiles[0].isEnabled = false;
        layer->updateCache(3, 1);

        layer->drawFringe(mock,
            0, 0,
            3, 1,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32);
        REQUIRE(mock->mDraws[0].y == 0);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 64);
        REQUIRE(mock->mDraws[1].y == 0);
        REQUIRE(mock->mDraws[1].image == img2);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            3, 1,
            -10, 20,
            actors);
        REQUIRE(mock->mDraws.size() == 2);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 42);
        REQUIRE(mock->mDraws[0].y == -20);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 74);
        REQUIRE(mock->mDraws[1].y == -20);
        REQUIRE(mock->mDraws[1].image == img2);
    }

    SECTION("normal 1")
    {
        const int maxX = 100;
        const int maxY = 100;
        map = new Map("map",
            maxX, maxY,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
            true,
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
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        TileInfo *const tiles = layer->getTiles();
        layer->updateCache(maxX, maxY);

        layer->drawFringe(mock,
            0, 0,
            maxX, maxY,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 6);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 32 * 1);
        REQUIRE(mock->mDraws[0].y == 32 * 10);
        REQUIRE(mock->mDraws[0].width == 96);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32 * 4);
        REQUIRE(mock->mDraws[1].y == 32 * 10);
        REQUIRE(mock->mDraws[1].image == img2);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 32 * 6);
        REQUIRE(mock->mDraws[2].y == 32 * 10);
        REQUIRE(mock->mDraws[2].image == img2);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[3].x == 32 * 9);
        REQUIRE(mock->mDraws[3].y == 32 * 10);
        REQUIRE(mock->mDraws[3].width == 64);
        REQUIRE(mock->mDraws[3].height == 32);
        REQUIRE(mock->mDraws[3].image == img2);
        REQUIRE(mock->mDraws[4].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[4].x == 32 * 11);
        REQUIRE(mock->mDraws[4].y == 32 * 10);
        REQUIRE(mock->mDraws[4].image == img3);
        REQUIRE(mock->mDraws[5].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[5].x == 32 * 15);
        REQUIRE(mock->mDraws[5].y == 32 * 10);
        REQUIRE(mock->mDraws[5].width == 96);
        REQUIRE(mock->mDraws[5].height == 32);
        REQUIRE(mock->mDraws[5].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            maxX, maxY,
            -10, 20,
            actors);
        REQUIRE(mock->mDraws.size() == 6);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 32 * 1 + 10);
        REQUIRE(mock->mDraws[0].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[0].width == 96);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32 * 4 + 10);
        REQUIRE(mock->mDraws[1].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[1].image == img2);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 32 * 6 + 10);
        REQUIRE(mock->mDraws[2].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[2].image == img2);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[3].x == 32 * 9 + 10);
        REQUIRE(mock->mDraws[3].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[3].width == 64);
        REQUIRE(mock->mDraws[3].height == 32);
        REQUIRE(mock->mDraws[3].image == img2);
        REQUIRE(mock->mDraws[4].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[4].x == 32 * 11 + 10);
        REQUIRE(mock->mDraws[4].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[4].image == img3);
        REQUIRE(mock->mDraws[5].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[5].x == 32 * 15 + 10);
        REQUIRE(mock->mDraws[5].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[5].width == 96);
        REQUIRE(mock->mDraws[5].height == 32);
        REQUIRE(mock->mDraws[5].image == img1);
    }

    SECTION("normal2")
    {
        const int maxX = 100;
        const int maxY = 100;
        map = new Map("map",
            maxX, maxY,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
            true,
            0,
            0);
        TileInfo *const tiles = layer->getTiles();
        map->addLayer(layer);
        layer->setSpecialLayer(map->getSpecialLayer());
        layer->setTempLayer(map->getTempLayer());
        for (int x = 0; x < maxX; x ++)
        {
            for (int y = 0; y < maxY; y ++)
            {
                layer->setTile(x, y, img1);
                tiles[y * maxX + x].isEnabled = false;
            }
        }
        tiles[10 * maxX + 41].isEnabled = true;
        layer->updateCache(maxX, maxY);

        layer->drawFringe(mock,
            0, 0,
            maxX, maxY,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32 * 41);
        REQUIRE(mock->mDraws[0].y == 32 * 10);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            maxX, maxY,
            -10, 20,
            actors);
        REQUIRE(mock->mDraws.size() == 1);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32 * 41 + 10);
        REQUIRE(mock->mDraws[0].y == 32 * 10 - 20);
    }

    SECTION("normal 3")
    {
        const int maxX = 100;
        const int maxY = 100;
        map = new Map("map",
            maxX, maxY,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
            true,
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
        map->addLayer(layer);
        SpecialLayer *const specialLayer = map->getSpecialLayer();
        SpecialLayer *const tempLayer = map->getTempLayer();
        layer->setSpecialLayer(specialLayer);
        layer->setTempLayer(tempLayer);
        TileInfo *const tiles = layer->getTiles();
        specialLayer->setTile(1, 10, MapItemType::ARROW_UP);
        specialLayer->setTile(10, 10, MapItemType::ARROW_DOWN);
        specialLayer->updateCache();
        layer->updateCache(maxX, maxY);

        layer->drawFringe(mock,
            0, 0,
            maxX, maxY,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 8);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 32 * 1);
        REQUIRE(mock->mDraws[0].y == 32 * 10);
        REQUIRE(mock->mDraws[0].width == 96);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32 * 1);
        REQUIRE(mock->mDraws[1].y == 32 * 10);
//        REQUIRE(mock->mDraws[1].image == img2);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 32 * 4);
        REQUIRE(mock->mDraws[2].y == 32 * 10);
        REQUIRE(mock->mDraws[2].image == img2);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[3].x == 32 * 6);
        REQUIRE(mock->mDraws[3].y == 32 * 10);
        REQUIRE(mock->mDraws[3].image == img2);
        REQUIRE(mock->mDraws[4].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[4].x == 32 * 9);
        REQUIRE(mock->mDraws[4].y == 32 * 10);
        REQUIRE(mock->mDraws[4].width == 64);
        REQUIRE(mock->mDraws[4].height == 32);
        REQUIRE(mock->mDraws[4].image == img2);
        REQUIRE(mock->mDraws[5].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[5].x == 32 * 10);
        REQUIRE(mock->mDraws[5].y == 32 * 10);
//        REQUIRE(mock->mDraws[5].image == img2);
        REQUIRE(mock->mDraws[6].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[6].x == 32 * 11);
        REQUIRE(mock->mDraws[6].y == 32 * 10);
        REQUIRE(mock->mDraws[6].image == img3);
        REQUIRE(mock->mDraws[7].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[7].x == 32 * 15);
        REQUIRE(mock->mDraws[7].y == 32 * 10);
        REQUIRE(mock->mDraws[7].width == 96);
        REQUIRE(mock->mDraws[7].height == 32);
        REQUIRE(mock->mDraws[7].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            maxX, maxY,
            -10, 20,
            actors);
        REQUIRE(mock->mDraws.size() == 8);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[0].x == 32 * 1 + 10);
        REQUIRE(mock->mDraws[0].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[0].width == 96);
        REQUIRE(mock->mDraws[0].height == 32);
        REQUIRE(mock->mDraws[0].image == img1);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[1].x == 32 * 1 + 10);
        REQUIRE(mock->mDraws[1].y == 32 * 10 - 20);
//        REQUIRE(mock->mDraws[1].image == img2);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 32 * 4 + 10);
        REQUIRE(mock->mDraws[2].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[2].image == img2);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[3].x == 32 * 6 + 10);
        REQUIRE(mock->mDraws[3].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[3].image == img2);
        REQUIRE(mock->mDraws[4].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[4].x == 32 * 9 + 10);
        REQUIRE(mock->mDraws[4].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[4].width == 64);
        REQUIRE(mock->mDraws[4].height == 32);
        REQUIRE(mock->mDraws[4].image == img2);
        REQUIRE(mock->mDraws[5].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[5].x == 32 * 10 + 10);
        REQUIRE(mock->mDraws[5].y == 32 * 10 - 20);
//        REQUIRE(mock->mDraws[5].image == img2);
        REQUIRE(mock->mDraws[6].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[6].x == 32 * 11 + 10);
        REQUIRE(mock->mDraws[6].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[6].image == img3);
        REQUIRE(mock->mDraws[7].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[7].x == 32 * 15 + 10);
        REQUIRE(mock->mDraws[7].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[7].width == 96);
        REQUIRE(mock->mDraws[7].height == 32);
        REQUIRE(mock->mDraws[7].image == img1);
    }

    SECTION("normal 4")
    {
        const int maxX = 100;
        const int maxY = 100;
        map = new Map("map",
            maxX, maxY,
            32, 32);
        layer = new MapLayer("test",
            0, 0,
            maxX, maxY,
            true,
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
        map->addLayer(layer);
        SpecialLayer *const specialLayer = map->getSpecialLayer();
        SpecialLayer *const tempLayer = map->getTempLayer();
        layer->setSpecialLayer(specialLayer);
        layer->setTempLayer(tempLayer);
        TileInfo *const tiles = layer->getTiles();
        specialLayer->setTile(0, 10, MapItemType::ARROW_UP);
        specialLayer->setTile(10, 10, MapItemType::ARROW_DOWN);
        specialLayer->setTile(90, 10, MapItemType::ARROW_DOWN);
        specialLayer->updateCache();
        layer->updateCache(maxX, maxY);

        layer->drawFringe(mock,
            0, 0,
            maxX - 20, maxY,
            0, 0,
            actors);
        REQUIRE(mock->mDraws.size() == 8);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32 * 0);
        REQUIRE(mock->mDraws[0].y == 32 * 10);
//        REQUIRE(mock->mDraws[0].image == img2);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[1].x == 32 * 1);
        REQUIRE(mock->mDraws[1].y == 32 * 10);
        REQUIRE(mock->mDraws[1].width == 96);
        REQUIRE(mock->mDraws[1].height == 32);
        REQUIRE(mock->mDraws[1].image == img1);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 32 * 4);
        REQUIRE(mock->mDraws[2].y == 32 * 10);
        REQUIRE(mock->mDraws[2].image == img2);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[3].x == 32 * 6);
        REQUIRE(mock->mDraws[3].y == 32 * 10);
        REQUIRE(mock->mDraws[3].image == img2);
        REQUIRE(mock->mDraws[4].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[4].x == 32 * 9);
        REQUIRE(mock->mDraws[4].y == 32 * 10);
        REQUIRE(mock->mDraws[4].width == 64);
        REQUIRE(mock->mDraws[4].height == 32);
        REQUIRE(mock->mDraws[4].image == img2);
        REQUIRE(mock->mDraws[5].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[5].x == 32 * 10);
        REQUIRE(mock->mDraws[5].y == 32 * 10);
//        REQUIRE(mock->mDraws[5].image == img2);
        REQUIRE(mock->mDraws[6].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[6].x == 32 * 11);
        REQUIRE(mock->mDraws[6].y == 32 * 10);
        REQUIRE(mock->mDraws[6].image == img3);
        REQUIRE(mock->mDraws[7].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[7].x == 32 * 15);
        REQUIRE(mock->mDraws[7].y == 32 * 10);
        REQUIRE(mock->mDraws[7].width == 96);
        REQUIRE(mock->mDraws[7].height == 32);
        REQUIRE(mock->mDraws[7].image == img1);

        mock->mDraws.clear();
        layer->drawFringe(mock,
            0, 0,
            maxX - 20, maxY,
            -10, 20,
            actors);
        REQUIRE(mock->mDraws.size() == 8);
        REQUIRE(mock->mDraws[0].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[0].x == 32 * 0 + 10);
        REQUIRE(mock->mDraws[0].y == 32 * 10 - 20);
//        REQUIRE(mock->mDraws[0].image == img2);
        REQUIRE(mock->mDraws[1].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[1].x == 32 * 1 + 10);
        REQUIRE(mock->mDraws[1].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[1].width == 96);
        REQUIRE(mock->mDraws[1].height == 32);
        REQUIRE(mock->mDraws[1].image == img1);
        REQUIRE(mock->mDraws[2].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[2].x == 32 * 4 + 10);
        REQUIRE(mock->mDraws[2].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[2].image == img2);
        REQUIRE(mock->mDraws[3].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[3].x == 32 * 6 + 10);
        REQUIRE(mock->mDraws[3].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[3].image == img2);
        REQUIRE(mock->mDraws[4].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[4].x == 32 * 9 + 10);
        REQUIRE(mock->mDraws[4].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[4].width == 64);
        REQUIRE(mock->mDraws[4].height == 32);
        REQUIRE(mock->mDraws[4].image == img2);
        REQUIRE(mock->mDraws[5].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[5].x == 32 * 10 + 10);
        REQUIRE(mock->mDraws[5].y == 32 * 10 - 20);
//        REQUIRE(mock->mDraws[5].image == img2);
        REQUIRE(mock->mDraws[6].drawType == MockDrawType::DrawImage);
        REQUIRE(mock->mDraws[6].x == 32 * 11 + 10);
        REQUIRE(mock->mDraws[6].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[6].image == img3);
        REQUIRE(mock->mDraws[7].drawType == MockDrawType::DrawPattern);
        REQUIRE(mock->mDraws[7].x == 32 * 15 + 10);
        REQUIRE(mock->mDraws[7].y == 32 * 10 - 20);
        REQUIRE(mock->mDraws[7].width == 96);
        REQUIRE(mock->mDraws[7].height == 32);
        REQUIRE(mock->mDraws[7].image == img1);
    }

    delete2(localPlayer);
    delete map;
    delete img1;
    delete img2;
    delete img3;
    delete mock;
    delete2(theme);
    GraphicsManager::deleteRenderers();
    VirtFs::removeDirFromSearchPath("data");
    VirtFs::removeDirFromSearchPath("../data");
    delete2(logger);
}
