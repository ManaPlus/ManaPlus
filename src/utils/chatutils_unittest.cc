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

#include "catch.hpp"
#include "client.h"

#include "actormanager.h"
#include "graphicsmanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "gui/gui.h"
#include "gui/theme.h"

#include "utils/chatutils.h"
#include "utils/delete2.h"
#include "utils/physfstools.h"

#include "render/sdlgraphics.h"

#include "resources/sdlimagehelper.h"

#include "resources/resourcemanager/resourcemanager.h"

#ifndef USE_SDL2
#include <SDL.h>
#endif  // USE_SDL2

#include "debug.h"

TEST_CASE("chatutils replaceVars")
{
    client = new Client;
    dirSeparator = "/";
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);
    theme = new Theme;
    Theme::selectSkin();
    imageHelper = new SDLImageHelper();
    mainGraphics = new SDLGraphics;

    gui = new Gui();
    gui->postInit(mainGraphics);
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    ActorSprite::load();
    localPlayer = new LocalPlayer(static_cast<BeingId>(1),
        BeingTypeId_zero);
    actorManager = new ActorManager;
    std::string str;

    SECTION("empty")
    {
        replaceVars(str);
        REQUIRE(str == "");

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

        delete2(localPlayer);
    }

    SECTION("player")
    {
        Being *player1 = new Being(static_cast<BeingId>(2),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player1->setName("player1");
        actorManager->mActors.insert(player1);

        localPlayer->setTarget(player1);
        str = "test <PLAYER>";
        replaceVars(str);
        REQUIRE(str == "test player1");

        delete2(localPlayer);
    }

    SECTION("monster")
    {
        Being *const monster = new Being(static_cast<BeingId>(3),
            ActorType::Monster,
            BeingTypeId_zero,
            nullptr);
        monster->setName("monster1");
        actorManager->mActors.insert(monster);

        localPlayer->setTarget(monster);
        str = "test <MONSTER>";
        replaceVars(str);
        REQUIRE(str == "test monster1");

        delete2(localPlayer);
    }

    SECTION("people")
    {
        actorManager->mActors.insert(localPlayer);
        str = "test <PEOPLE>";
        replaceVars(str);
        REQUIRE(str == "test ");

        Being *const player1 = new Being(static_cast<BeingId>(2),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player1->setName("player1");
        actorManager->mActors.insert(player1);

        str = "test <PEOPLE>";
        replaceVars(str);
        REQUIRE(str == "test player1");

        Being *const player2 = new Being(static_cast<BeingId>(4),
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

        Being *const player1 = new Being(static_cast<BeingId>(2),
            ActorType::Player,
            BeingTypeId_zero,
            nullptr);
        player1->setName("player1");
        actorManager->mActors.insert(player1);
        player1->setParty(party1);

        str = "test <PARTY>";
        replaceVars(str);
        REQUIRE(str == "test player1");

        Being *const player2 = new Being(static_cast<BeingId>(4),
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

        Being *const player3 = new Being(static_cast<BeingId>(5),
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
    }

    delete2(actorManager);
    delete2(client);
//    PhysFs::deinit();
}

TEST_CASE("chatutils textToMe")
{
    REQUIRE(textToMe("") == "**");
    REQUIRE(textToMe("123") == "*123*");
    REQUIRE(textToMe("*") == "***");
    REQUIRE(textToMe("test line") == "*test line*");
}
