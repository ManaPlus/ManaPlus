/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
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

#include "unittests/unittests.h"

#include "configmanager.h"
#include "configuration.h"
#include "dirs.h"
#include "graphicsmanager.h"

#include "enums/resources/map/mapitemtype.h"

#include "fs/virtfs/fs.h"

#include "unittests/render/mockgraphics.h"

#include "utils/env.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sdlimagehelper.h"

#include "resources/map/map.h"
#include "resources/map/maplayer.h"
#include "resources/map/speciallayer.h"

#include "debug.h"

TEST_CASE("MapLayer drawSpecialLayer (specialLayer)", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    imageHelper = new SDLImageHelper;

    Dirs::initRootDir();
    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        GraphicsManager::createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    GraphicsManager::createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
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
        specialLayer = map->getSpecialLayer();

        layer->drawSpecialLayer(mock,
            0,
            0, 1,
            0, 0);
        REQUIRE(mock->mDraws.empty());

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
        REQUIRE(mock->mDraws.empty());

        layer->drawSpecialLayer(mock,
            6,
            0, maxX,
            0, 0);
        REQUIRE(mock->mDraws.empty());

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
    ResourceManager::cleanOrphans(true);
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}
