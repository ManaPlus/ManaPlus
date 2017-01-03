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
#include "client.h"
#include "graphicsmanager.h"
#include "logger.h"

#include "being/actorsprite.h"

#include "utils/translation/podict.h"
#include "utils/translation/poparser.h"

#include "resources/sdlimagehelper.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/physfstools.h"

#include "debug.h"

TEST_CASE("PoParser tests", "PoParser")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);

    imageHelper = new SDLImageHelper();
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    ActorSprite::load();

    SECTION("PoParser empty")
    {
        PoParser *parser = new PoParser;
        PoDict *dict = parser->load("ru",
            "unknownfilename.po",
            nullptr);

        REQUIRE(dict != nullptr);
        REQUIRE(dict->getMap() != nullptr);
        REQUIRE(dict->getMap()->size() == 0);

        delete parser;
        delete dict;
    }

    SECTION("PoParser normal")
    {
        PoParser *parser = new PoParser;
        PoDict *dict = parser->load("ru",
            "test/test1",
            nullptr);

        REQUIRE(dict != nullptr);
        REQUIRE(dict->getMap() != nullptr);
        REQUIRE(dict->getMap()->size() == 1786);
        REQUIRE(dict->getStr("Unknown skill message.") ==
            "Неизвестная ошибка скилов.");
        REQUIRE(dict->getStr("Full strip failed because of coating.") ==
            "Full strip failed because of coating.");
        REQUIRE(dict->getStr("You picked up %d [@@%d|%s@@].") ==
            "Вы подняли %d [@@%d|%s@@].");

        delete parser;
        delete dict;
    }

    SECTION("PoParser fuzzy")
    {
        PoParser *parser = new PoParser;
        PoDict *dict = parser->load("ru",
            "test/test1",
            nullptr);

        REQUIRE(dict != nullptr);
        REQUIRE(dict->getMap() != nullptr);
        REQUIRE(dict->getMap()->size() == 1786);
        REQUIRE(dict->getStr("Atk +100%.") == "Atk +100%.");

        delete parser;
        delete dict;
    }
    delete2(client);
}
