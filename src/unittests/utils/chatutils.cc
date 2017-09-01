/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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
#include "configuration.h"
#include "configmanager.h"
#include "dirs.h"

#include "actormanager.h"
#include "graphicsmanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "fs/virtfs/fs.h"

#include "gui/gui.h"
#include "gui/theme.h"

#include "utils/chatutils.h"
#include "utils/delete2.h"
#include "utils/env.h"

#include "render/sdlgraphics.h"

#include "resources/sdlimagehelper.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifndef USE_SDL2
#include <SDL.h>
#endif  // USE_SDL2
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

TEST_CASE("chatutils replaceVars", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    imageHelper = new SDLImageHelper();
    mainGraphics = new SDLGraphics;

#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    Dirs::initRootDir();
    Dirs::initHomeDir();

    ConfigManager::initConfiguration();
    setConfigDefaults2(config);
    setBrandingDefaults(branding);

    theme = new Theme;
    Theme::selectSkin();

    gui = new Gui();
    gui->postInit(mainGraphics);

    ActorSprite::load();
    localPlayer = new LocalPlayer(static_cast<BeingId>(1),
        BeingTypeId_zero);
    actorManager = new ActorManager;
    std::string str;

    SECTION("empty")
    {
        replaceVars(str);
        REQUIRE(str.empty());

        str = "test line";
        replaceVars(str);
        REQUIRE(str == "test line");

        str = "test <PLAYER>";
        replaceVars(str);
        REQUIRE(str == "test ");

        str = "test <MONSTER>";
        replaceVars(str);
        REQUIRE(str == "test ");

        str = "test <PEOPLE>";
        replaceVars(str);
        REQUIRE(str == "test ");

        str = "test <PARTY>";
        replaceVars(str);
        REQUIRE(str == "test ");

        str = "test <SOMETHING>";
        replaceVars(str);
        REQUIRE(str == "test <SOMETHING>");
    }

    SECTION("player")
    {
        Being *player1 = Being::createBeing(static_cast<BeingId>(2),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player1->setName("player1");
        actorManager->mActors.insert(player1);

        localPlayer->setTarget(player1);
        str = "test <PLAYER>";
        replaceVars(str);
        REQUIRE(str == "test player1");
    }

    SECTION("monster")
    {
        Being *const monster = Being::createBeing(static_cast<BeingId>(3),
            ActorType::Monster,
            BeingTypeId_zero,
            nullptr);
        monster->setName("monster1");
        actorManager->mActors.insert(monster);

        localPlayer->setTarget(monster);
        str = "test <MONSTER>";
        replaceVars(str);
        REQUIRE(str == "test monster1");
    }

    SECTION("people")
    {
        actorManager->mActors.insert(localPlayer);
        str = "test <PEOPLE>";
        replaceVars(str);
        REQUIRE(str == "test ");

        Being *const player1 = Being::createBeing(static_cast<BeingId>(2),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player1->setName("player1");
        actorManager->mActors.insert(player1);

        str = "test <PEOPLE>";
        replaceVars(str);
        REQUIRE(str == "test player1");

        Being *const player2 = Being::createBeing(static_cast<BeingId>(4),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player2->setName("player2");
        actorManager->mActors.insert(player2);

        str = "test <PEOPLE>";
        replaceVars(str);
        const bool correct = str == "test player1,player2" ||
            str == "test player2,player1";
        REQUIRE(correct == true);
    }

    SECTION("party")
    {
        actorManager->mActors.insert(localPlayer);

        Party *const party1 = Party::getParty(1);
        localPlayer->setParty(party1);

        str = "test <PARTY>";
        replaceVars(str);
        REQUIRE(str == "test ");

        Being *const player1 = Being::createBeing(static_cast<BeingId>(2),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player1->setName("player1");
        actorManager->mActors.insert(player1);
        player1->setParty(party1);

        str = "test <PARTY>";
        replaceVars(str);
        REQUIRE(str == "test player1");

        Being *const player2 = Being::createBeing(static_cast<BeingId>(4),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player2->setName("player2");
        actorManager->mActors.insert(player2);
        player2->setParty(party1);

        str = "test <PARTY>";
        replaceVars(str);
        REQUIRE(str == "test player1,player2");

        Party *const party2 = Party::getParty(2);

        Being *const player3 = Being::createBeing(static_cast<BeingId>(5),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player3->setName("player3");
        actorManager->mActors.insert(player3);
        player3->setParty(party2);

        str = "test <PARTY>";
        replaceVars(str);
        REQUIRE(str == "test player1,player2");

        player3->setParty(party1);

        str = "test <PARTY>";
        replaceVars(str);
        REQUIRE(str == "test player1,player2,player3");

        player2->setParty(party2);

        str = "test <PARTY>";
        replaceVars(str);
        REQUIRE(str == "test player1,player3");

        Party::clearParties();
    }

    delete2(actorManager);
    delete2(localPlayer);
    delete2(client);
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("chatutils textToMe", "")
{
    REQUIRE(textToMe("") == "**");
    REQUIRE(textToMe("123") == "*123*");
    REQUIRE(textToMe("*") == "***");
    REQUIRE(textToMe("test line") == "*test line*");
}
