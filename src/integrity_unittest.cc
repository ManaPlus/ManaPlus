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

#include "fs/files.h"

#include "fs/virtfs/virtfs.h"
#include "fs/virtfs/virtfsrwops.h"

#include "input/inputactionmap.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "resources/sdlimagehelper.h"
#ifdef USE_SDL2
#include "resources/surfaceimagehelper.h"
#endif  // USE_SDL2

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/env.h"
#include "utils/delete2.h"

#include <SDL_image.h>

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

static bool compareBuffers(const unsigned char *const buf2)
{
    bool isCorrect(true);
    int sz = 0;
    const unsigned char *buf1 = reinterpret_cast<const unsigned char*>(
        VirtFs::loadFile("hide.png", sz));
    REQUIRE(buf1 != nullptr);
    REQUIRE(sz == 368);
    for (int f = 0; f < sz; f ++)
    {
        if (buf1[f] != buf2[f])
        {
            isCorrect = false;
            logger->log("Wrong buffer chars: 0x%x vs 0x%x",
                buf1[f],
                buf2[f]);
        }
    }
    delete [] buf1;
    return isCorrect;
}

TEST_CASE("integrity tests", "integrity")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager::init();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

#ifdef USE_SDL2
    imageHelper = new SurfaceImageHelper;

    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    imageHelper = new SDLImageHelper();

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    ActorSprite::load();
    const char *const name1 = "dir/hide.png";
    const int size1 = 368;

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

    SECTION("integrity Loader::getImage test 1")
    {
        Image *const image = Loader::getImage(
            "graphics/images/login_wallpaper.png");
        REQUIRE(image != nullptr);
        REQUIRE(image->getSDLSurface() != nullptr);
        image->decRef();
    }

    SECTION("integrity Loader::getImage test 2")
    {
        VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);
        Image *const image = Loader::getImage(
            "dir/hide.png");
        VirtFs::unmountZip(prefix + "data/test/test.zip");
        REQUIRE(image != nullptr);
        REQUIRE(image->getSDLSurface() != nullptr);
        image->decRef();
    }

    SECTION("integrity Loader::getImage test 3")
    {
        VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);
        Image *const image = Loader::getImage(
            "dir/brimmedhat.png");
        VirtFs::unmountZip(prefix + "data/test/test.zip");
        REQUIRE(image != nullptr);
        REQUIRE(image->getSDLSurface() != nullptr);
        image->decRef();
    }

    SECTION("integrity Loader::getImage test 4")
    {
        VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);

        SDL_RWops *const rw = VirtFs::rwopsOpenRead(name1);
        REQUIRE(rw != nullptr);
        unsigned char buf[size1];
        const size_t sz = SDL_RWread(rw, buf, 1, size1);
        if (sz != size1)
            SDL_RWclose(rw);
        REQUIRE(sz == size1);
        SDL_RWclose(rw);
        VirtFs::unmountZip(prefix + "data/test/test.zip");
        VirtFs::mountDirSilent(prefix + "data/test", Append_true);
        REQUIRE(compareBuffers(buf));
        VirtFs::unmountDirSilent(prefix + "data/test");
    }

    SECTION("integrity Loader::getImage test 7")
    {
        VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);

        SDL_RWops *const rw = VirtFs::rwopsOpenRead(name1);
        REQUIRE(rw != nullptr);
        int64_t seek = SDL_RWseek(rw, 0, RW_SEEK_END);
        if (seek == -1)
            SDL_RWclose(rw);
        REQUIRE(seek != -1);
        const int64_t pos = SDL_RWtell(rw);
        if (pos != size1)
            SDL_RWclose(rw);
        REQUIRE(pos == size1);

        seek = SDL_RWseek(rw, 0, RW_SEEK_SET);
        if (seek == -1)
            SDL_RWclose(rw);
        REQUIRE(seek != -1);

        unsigned char buf[size1];
        const size_t sz = SDL_RWread(rw, buf, 1, pos);
        if (sz != size1)
            SDL_RWclose(rw);
        REQUIRE(sz == size1);

        SDL_RWclose(rw);
        VirtFs::unmountZip(prefix + "data/test/test.zip");
        VirtFs::mountDirSilent("data/test", Append_true);
        VirtFs::mountDirSilent("../data/test", Append_true);
        REQUIRE(compareBuffers(buf));
        VirtFs::unmountDirSilent("data/test");
        VirtFs::unmountDirSilent("../data/test");
    }

    SECTION("integrity Loader::getImage test 8")
    {
        VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);

        SDL_RWops *const rw = VirtFs::rwopsOpenRead(name1);
        REQUIRE(rw != nullptr);
        if (IMG_isPNG(rw) == false)
        {
            SDL_RWclose(rw);
            REQUIRE(false);
        }
        SDL_Surface *const tmpImage = IMG_LoadPNG_RW(rw);
        SDL_RWclose(rw);
        VirtFs::unmountZip(prefix + "data/test/test.zip");
        REQUIRE(tmpImage != nullptr);
        SDL_FreeSurface(tmpImage);
    }

    SECTION("integrity Loader::getImage test 9")
    {
        VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);

        SDL_RWops *const rw = VirtFs::rwopsOpenRead(name1);
        REQUIRE(rw != nullptr);
        Resource *const res = imageHelper->load(rw);
        VirtFs::unmountZip(prefix + "data/test/test.zip");
        REQUIRE(res != nullptr);
        delete res;
    }

    SECTION("integrity Loader::getImage test 10")
    {
        VirtFs::mountZip(prefix + "data/test/test.zip", Append_false);
        Image *const image = Loader::getImage(
            name1);
        VirtFs::unmountZip(prefix + "data/test/test.zip");
        REQUIRE(image != nullptr);
        REQUIRE(image->getSDLSurface() != nullptr);
        image->decRef();
    }

    resourceManager->cleanOrphans(true);

    delete client;
    client = nullptr;

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}
