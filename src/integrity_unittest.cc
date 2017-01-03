/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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
#include "logger.h"
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "input/inputactionmap.h"

#include "resources/sdlimagehelper.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/env.h"
#include "utils/physfstools.h"

#ifndef USE_SDL2
#include <SDL.h>
#endif  // USE_SDL2

#include "debug.h"

namespace
{
    class InputActionSortFunctorTest final
    {
        public:
            bool operator() (const InputActionT key1,
                             const InputActionT key2) const
            {
                REQUIRE(CAST_SIZE(key1) < CAST_SIZE(InputAction::TOTAL));
                REQUIRE(CAST_SIZE(key2) < CAST_SIZE(InputAction::TOTAL));
                REQUIRE(key1 < InputAction::TOTAL);
                REQUIRE(key2 < InputAction::TOTAL);
                return keys[CAST_SIZE(key1)].priority
                    >= keys[CAST_SIZE(key2)].priority;
            }

            const InputActionData *keys A_NONNULLPOINTER;
    };
    InputActionSortFunctorTest inputActionDataSorterTest;
}  // namespace

TEST_CASE("integrity tests", "integrity")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
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

    SECTION("integrity test 1")
    {
        REQUIRE(sizeof(inputActionData) / sizeof(InputActionData) ==
            CAST_SIZE(InputAction::TOTAL));
    }

    SECTION("integrity test 2")
    {
        KeyToActionMap actionMap;
        const size_t sz = CAST_SIZE(InputAction::TOTAL);
        for (size_t i = 0; i < sz; i ++)
        {
            InputActionT val = static_cast<InputActionT>(i);
            REQUIRE(val < InputAction::TOTAL);
            REQUIRE(val > InputAction::NO_VALUE);
            REQUIRE(val > InputAction::UNDEFINED_VALUE);
            REQUIRE(CAST_SIZE(val) < CAST_SIZE(InputAction::TOTAL));
            REQUIRE(CAST_SIZE(val) < CAST_SIZE(InputAction::NO_VALUE));
            REQUIRE(CAST_SIZE(val) < CAST_SIZE(InputAction::UNDEFINED_VALUE));
            REQUIRE(CAST_S32(val) < CAST_S32(InputAction::TOTAL));
            REQUIRE(CAST_S32(val) > CAST_S32(InputAction::NO_VALUE));
            REQUIRE(CAST_S32(val) > CAST_S32(InputAction::UNDEFINED_VALUE));
        }
    }

    SECTION("integrity test 3")
    {
        KeyToActionMap actionMap;
        int cnt = 0;
        const size_t sz = CAST_SIZE(InputAction::TOTAL);
        for (size_t i = 0; i < sz; i ++)
        {
            actionMap[cnt++].push_back(static_cast<InputActionT>(i));
            if (cnt > 3)
                cnt = 0;
        }
        FOR_EACH (KeyToActionMapIter, it, actionMap)
        {
            KeysVector *const keys = &it->second;
            FOR_EACHP (KeysVectorIter, itk, keys)
            {
                const size_t val = CAST_SIZE(*itk);
                REQUIRE(val < sz);
            }
        }
    }

    SECTION("integrity test 4")
    {
        KeyToActionMap actionMap;
        KeyTimeMap keyTimeMap;
        actionMap.clear();
        keyTimeMap.clear();

        for (size_t i = 0; i < CAST_SIZE(InputAction::TOTAL); i ++)
        {
            actionMap[10].push_back(
                static_cast<InputActionT>(i));
        }

        KeysVector *const keys = &actionMap[0];
        inputActionDataSorterTest.keys = &inputActionData[0];
        std::sort(keys->begin(), keys->end(), inputActionDataSorterTest);
    }

    SECTION("integrity test 5")
    {
        KeyToActionMap mKeyToAction;
        KeyToIdMap mKeyToId;
        KeyTimeMap mKeyTimeMap;
        inputManager.updateKeyActionMap(mKeyToAction, mKeyToId,
            mKeyTimeMap, InputType::KEYBOARD);
    }

    delete client;
    client = nullptr;
}
