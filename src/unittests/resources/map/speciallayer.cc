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

#include "client.h"
#include "configmanager.h"
#include "configuration.h"
#include "dirs.h"

#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "gui/userpalette.h"
#include "gui/theme.h"

#include "enums/resources/map/mapitemtype.h"

#include "fs/virtfs/fs.h"

#include "utils/delete2.h"
#include "utils/env.h"

#include "resources/sdlimagehelper.h"

#include "resources/map/speciallayer.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

TEST_CASE("SpecialLayer updateCache", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    imageHelper = new SDLImageHelper;

    Dirs::initRootDir();
    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    userPalette = new UserPalette;

    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();

    SpecialLayer *layer = nullptr;

    SECTION("simple 1")
    {
        layer = new SpecialLayer("test",
            1, 1);
        const int *const cache = layer->getCache();
        REQUIRE(cache[0] == 10000);
        layer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->updateCache();
        REQUIRE(cache[0] == 10000);
    }

    SECTION("simple 2")
    {
        layer = new SpecialLayer("test",
            2, 1);
        const int *const cache = layer->getCache();
        REQUIRE(cache[0] == 10000);
        REQUIRE(cache[1] == 10000);
        layer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->updateCache();
        REQUIRE(cache[0] == 10000);
        REQUIRE(cache[1] == 10000);
    }

    SECTION("simple 3")
    {
        layer = new SpecialLayer("test",
            2, 1);
        layer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->setTile(1, 0, MapItemType::ARROW_DOWN);
        const int *const cache = layer->getCache();
        layer->updateCache();
        REQUIRE(cache[0] == 0);
        REQUIRE(cache[1] == 10000);
    }

    SECTION("simple 4")
    {
        layer = new SpecialLayer("test",
            2, 1);
        layer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->setTile(1, 0, MapItemType::ARROW_UP);
        const int *const cache = layer->getCache();
        layer->updateCache();
        REQUIRE(cache[0] == 0);
        REQUIRE(cache[1] == 10000);
    }

    SECTION("simple 4.2")
    {
        layer = new SpecialLayer("test",
            3, 1);
        layer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->setTile(2, 0, MapItemType::ARROW_UP);
        const int *const cache = layer->getCache();
        layer->updateCache();
        REQUIRE(cache[0] == 1);
        REQUIRE(cache[1] == 0);
        REQUIRE(cache[2] == 10000);
    }

    SECTION("simple 5")
    {
        layer = new SpecialLayer("test",
            3, 1);
        layer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->setTile(1, 0, MapItemType::ARROW_UP);
        const int *const cache = layer->getCache();
        layer->updateCache();
        REQUIRE(cache[0] == 0);
        REQUIRE(cache[1] == 10000);
        REQUIRE(cache[2] == 10000);
    }

    SECTION("simple 6")
    {
        layer = new SpecialLayer("test",
            3, 1);
        layer->setTile(0, 0, MapItemType::ARROW_UP);
        layer->setTile(1, 0, MapItemType::ARROW_UP);
        layer->setTile(2, 0, MapItemType::ARROW_DOWN);
        const int *const cache = layer->getCache();
        layer->updateCache();
        REQUIRE(cache[0] == 0);
        REQUIRE(cache[1] == 0);
        REQUIRE(cache[2] == 10000);
    }

    SECTION("simple 7")
    {
        layer = new SpecialLayer("test",
            3, 1);
        const int *const cache = layer->getCache();
        layer->updateCache();
        REQUIRE(cache[0] == 10000);
        REQUIRE(cache[1] == 10000);
        REQUIRE(cache[2] == 10000);
    }

    SECTION("simple 8")
    {
        layer = new SpecialLayer("test",
            3, 1);
        layer->setTile(0, 0, MapItemType::EMPTY);
        layer->setTile(1, 0, MapItemType::EMPTY);
        layer->setTile(2, 0, MapItemType::EMPTY);
        const int *const cache = layer->getCache();
        layer->updateCache();
        REQUIRE(cache[0] == 10000);
        REQUIRE(cache[1] == 10000);
        REQUIRE(cache[2] == 10000);
    }

    SECTION("simple 9")
    {
        layer = new SpecialLayer("test",
            2, 1);
        const int *const cache = layer->getCache();
        REQUIRE(cache[0] == 10000);
        REQUIRE(cache[1] == 10000);
        layer->setTile(1, 0, MapItemType::ARROW_UP);
        layer->updateCache();
        REQUIRE(cache[0] == 0);
        REQUIRE(cache[1] == 10000);
    }

    SECTION("normal 1")
    {
        layer = new SpecialLayer("test",
            100, 100);
        layer->setTile(1, 10, MapItemType::ARROW_UP);
        layer->setTile(2, 10, MapItemType::ARROW_UP);
        layer->setTile(3, 10, MapItemType::ARROW_UP);
        layer->setTile(4, 10, MapItemType::ARROW_DOWN);
        layer->setTile(5, 10, MapItemType::EMPTY);
        layer->setTile(6, 10, MapItemType::ARROW_DOWN);
        layer->setTile(7, 10, MapItemType::EMPTY);
        layer->setTile(8, 10, MapItemType::EMPTY);
        layer->setTile(9, 10, MapItemType::ARROW_DOWN);
        layer->setTile(10, 10, MapItemType::ARROW_DOWN);
        layer->setTile(11, 10, MapItemType::ARROW_LEFT);
        layer->setTile(12, 10, MapItemType::EMPTY);
        layer->setTile(13, 10, MapItemType::EMPTY);
        layer->setTile(14, 10, MapItemType::EMPTY);
        layer->setTile(15, 10, MapItemType::ARROW_UP);
        layer->setTile(16, 10, MapItemType::ARROW_UP);
        layer->setTile(17, 10, MapItemType::ARROW_UP);
        const int *const cache = layer->getCache();
        layer->updateCache();

        REQUIRE(cache[10 * 100 + 0] == 0);
        REQUIRE(cache[10 * 100 + 1] == 0);
        REQUIRE(cache[10 * 100 + 2] == 0);
        REQUIRE(cache[10 * 100 + 3] == 0);
        REQUIRE(cache[10 * 100 + 4] == 1);
        REQUIRE(cache[10 * 100 + 5] == 0);
        REQUIRE(cache[10 * 100 + 6] == 2);
        REQUIRE(cache[10 * 100 + 7] == 1);
        REQUIRE(cache[10 * 100 + 8] == 0);
        REQUIRE(cache[10 * 100 + 9] == 0);
        REQUIRE(cache[10 * 100 + 10] == 0);
        REQUIRE(cache[10 * 100 + 11] == 3);
        REQUIRE(cache[10 * 100 + 12] == 2);
        REQUIRE(cache[10 * 100 + 13] == 1);
        REQUIRE(cache[10 * 100 + 14] == 0);
        REQUIRE(cache[10 * 100 + 15] == 0);
        REQUIRE(cache[10 * 100 + 16] == 0);
        REQUIRE(cache[10 * 100 + 17] == 10000);
    }

    SECTION("normal2")
    {
        const int maxX = 100;
        const int maxY = 100;
        layer = new SpecialLayer("test",
            maxX, maxY);
        const int *const cache = layer->getCache();
        for (int x = 0; x < maxX; x ++)
        {
            for (int y = 0; y < maxY; y ++)
            {
                layer->setTile(x, y, MapItemType::ARROW_UP);
                REQUIRE(layer->getTiles()[x + y * maxX] != nullptr);
            }
        }
        layer->updateCache();

        for (int y = 0; y < maxY; y ++)
        {
            for (int x = 0; x < maxX - 1; x ++)
            {
                REQUIRE(cache[y * maxX + x] == 0);
            }
            REQUIRE(cache[y * maxX + maxX - 1] == 10000);
        }
    }

    delete layer;
    ResourceManager::cleanOrphans(false);
    delete2(userPalette)
    delete2(theme)
    delete2(client)
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}
