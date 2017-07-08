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

#include "graphicsmanager.h"

#include "being/localplayer.h"

#include "fs/virtfs/fs.h"

#include "gui/theme.h"

#include "unittests/render/mockgraphics.h"

#include "utils/delete2.h"
#include "utils/env.h"

#include "resources/sdlimagehelper.h"

#include "resources/image/image.h"

#include "resources/map/map.h"
#include "resources/map/maplayer.h"

#include "debug.h"

TEST_CASE("MapLayer drawFringe simple", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    logger = new Logger;
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

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

    delete2(localPlayer);
    delete map;
    delete img1;
    delete img2;
    delete img3;
    delete mock;
    delete2(theme);
    GraphicsManager::deleteRenderers();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    delete2(logger);
}
