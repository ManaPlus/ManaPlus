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

#include "being/localplayer.h"

#include "enums/resources/map/mapitemtype.h"

#include "fs/virtfs/fs.h"

#include "gui/theme.h"

#include "unittests/render/mockgraphics.h"

#include "utils/delete2.h"
#include "utils/env.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sdlimagehelper.h"

#include "resources/image/image.h"

#include "resources/map/map.h"
#include "resources/map/maplayer.h"
#include "resources/map/speciallayer.h"

#include "debug.h"

TEST_CASE("MapLayer drawFringe normal", "")
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

    delete2(localPlayer)
    delete map;
    delete img1;
    delete img2;
    delete img3;
    delete mock;
    delete2(theme)
    GraphicsManager::deleteRenderers();
    ResourceManager::cleanOrphans(true);
    ResourceManager::deleteInstance();
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}
