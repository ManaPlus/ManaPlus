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

#include "unittests/unittests.h"

#include "configmanager.h"
#include "dirs.h"

#include "unittests/render/mockgraphics.h"

#include "resources/image/image.h"

#include "resources/map/map.h"
#include "resources/map/maplayer.h"

#include "debug.h"

TEST_CASE("MapLayer draw", "")
{
    logger = new Logger;

    Dirs::initRootDir();
    Dirs::initHomeDir();

    ConfigManager::initConfiguration();

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
