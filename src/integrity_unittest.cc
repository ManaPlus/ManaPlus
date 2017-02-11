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

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "resources/sdlimagehelper.h"
#ifdef USE_SDL2
#include "resources/surfaceimagehelper.h"
#endif  // USE_SDL2

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/env.h"
#include "utils/virtfs.h"
#include "utils/virtfsrwops.h"

#include <physfs.h>
#include <SDL_image.h>

#include "debug.h"

#ifdef USE_SDL2
#define PHYSFSINT int64_t
#define PHYSFSSIZE size_t
#else  // USE_SDL2
#define PHYSFSINT int32_t
#define PHYSFSSIZE int
#endif  // USE_SDL2

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
    unsigned char *buf1 = static_cast<unsigned char*>(
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
    free(buf1);
    return isCorrect;
}

TEST_CASE("integrity tests", "integrity")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    dirSeparator = "/";
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data", Append_false);
    resourceManager->addToSearchPath("../data", Append_false);

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
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);
        Image *const image = Loader::getImage(
            "hide.png");
        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
        REQUIRE(image != nullptr);
        REQUIRE(image->getSDLSurface() != nullptr);
        image->decRef();
    }

    SECTION("integrity Loader::getImage test 3")
    {
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);
        Image *const image = Loader::getImage(
            "dir/brimmedhat.png");
        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
        REQUIRE(image != nullptr);
        REQUIRE(image->getSDLSurface() != nullptr);
        image->decRef();
    }

    SECTION("integrity Loader::getImage test 4")
    {
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);

        SDL_RWops *const rw = VirtFs::RWopsOpenRead(name1);
        if (!rw)
            logger->log("Physfs error: %s", VirtFs::getLastError());
        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
        REQUIRE(rw != nullptr);
        unsigned char buf[size1];
        const size_t sz = SDL_RWread(rw, buf, 1, size1);
        if (sz != size1)
            SDL_RWclose(rw);
        REQUIRE(sz == size1);
        SDL_RWclose(rw);
        REQUIRE(compareBuffers(buf) == true);
    }

    SECTION("integrity Loader::getImage test 5")
    {
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);

        PHYSFS_file *handle = PHYSFS_openRead(name1);
        REQUIRE(handle != nullptr);
//        int64_t seek = rw->seek(rw, 0, RW_SEEK_END);
        const PHYSFS_sint64 len = PHYSFS_fileLength(handle);
        REQUIRE(len == size1);
        PHYSFSINT pos = static_cast<PHYSFSINT>(len);
        REQUIRE(static_cast<PHYSFS_sint64>(pos) == len);
        REQUIRE(pos >= 0);
        REQUIRE(PHYSFS_seek(handle, static_cast<PHYSFS_uint64>(pos)) != 0);
        int64_t seek = pos;

        REQUIRE(seek != -1);

//        const int64_t pos1 = rw->seek(rw, 0, RW_SEEK_CUR);
        const PHYSFS_sint64 current = PHYSFS_tell(handle);
        REQUIRE(current != -1);
        REQUIRE(current == size1);
        pos = CAST_S32(current);
        REQUIRE(static_cast<PHYSFS_sint64>(pos) == current);
        REQUIRE(PHYSFS_seek(handle, static_cast<PHYSFS_uint64>(pos)) != 0);
        const int64_t pos1 = pos;

        REQUIRE(pos1 == size1);

//        seek = rw->seek(rw, 0, RW_SEEK_SET);
        REQUIRE(PHYSFS_seek(handle, static_cast<PHYSFS_uint64>(0)) != 0);
        seek = 0;

        REQUIRE(seek != -1);

        unsigned char buf[size1];

//        const size_t sz = rw->read(rw, buf, 1, pos1);
        const PHYSFS_sint64 rc = PHYSFS_read(handle, buf,
            CAST_U32(1),
            CAST_U32(size1));
        if (rc != static_cast<PHYSFS_sint64>(size1))
        {
            logger->log("PHYSFS_read %d bytes", static_cast<int>(rc));
            if (!PHYSFS_eof(handle))
            {
                logger->log("PhysicsFS read error: %s",
                    PHYSFS_getLastError());
            }
//            rw->close(rw);
            REQUIRE(PHYSFS_close(handle) != 0);
        }
        const size_t sz = rc;
        REQUIRE(sz == size1);

//        rw->close(rw);
        REQUIRE(PHYSFS_close(handle) != 0);

        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
    }

    SECTION("integrity Loader::getImage test 6")
    {
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);

        PHYSFS_file *handle = PHYSFS_openRead(name1);
        REQUIRE(handle != nullptr);
//        int64_t seek = rw->seek(rw, 0, RW_SEEK_END);
        const PHYSFS_sint64 len = PHYSFS_fileLength(handle);
        PHYSFSINT pos = static_cast<PHYSFSINT>(len);
        REQUIRE(static_cast<PHYSFS_sint64>(pos) == len);
        REQUIRE(pos >= 0);
        REQUIRE(PHYSFS_seek(handle, static_cast<PHYSFS_uint64>(pos)) != 0);
        int64_t seek = pos;

        if (seek == -1)
        {
//            rw->close(rw);
            REQUIRE(PHYSFS_close(handle) != 0);
        }
        REQUIRE(seek != -1);

//        const int64_t pos1 = rw->seek(rw, 0, RW_SEEK_CUR);
        const PHYSFS_sint64 current = PHYSFS_tell(handle);
        REQUIRE(current != -1);
        pos = CAST_S32(current);
        REQUIRE(static_cast<PHYSFS_sint64>(pos) == current);
        REQUIRE(PHYSFS_seek(handle, static_cast<PHYSFS_uint64>(pos)) != 0);
        const int64_t pos1 = pos;

        if (pos1 != size1)
        {
//            rw->close(rw);
            REQUIRE(PHYSFS_close(handle) != 0);
        }
        REQUIRE(pos1 == size1);

//        seek = rw->seek(rw, 0, RW_SEEK_SET);
        REQUIRE(PHYSFS_seek(handle, static_cast<PHYSFS_uint64>(0)) != 0);
        seek = 0;

        if (seek == -1)
        {
//            rw->close(rw);
            REQUIRE(PHYSFS_close(handle) != 0);
        }
        REQUIRE(seek != -1);

        unsigned char buf[size1];

//        const size_t sz = rw->read(rw, buf, 1, pos1);
        const PHYSFS_sint64 rc = PHYSFS_read(handle, buf,
            CAST_U32(1),
            CAST_U32(pos1));
        if (rc != static_cast<PHYSFS_sint64>(pos1))
        {
            if (!PHYSFS_eof(handle))
            {
                logger->log("PhysicsFS read error1: %s",
                    PHYSFS_getLastError());
            }
        }
        const size_t sz = rc;

        if (sz != size1)
        {
//            rw->close(rw);
            logger->log("PhysicsFS read error2: %s",
                PHYSFS_getLastError());
            REQUIRE(PHYSFS_close(handle) != 0);
        }
        REQUIRE(sz == size1);

//        rw->close(rw);
        REQUIRE(PHYSFS_close(handle) != 0);

        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
        REQUIRE(compareBuffers(buf) == true);
    }

    SECTION("integrity Loader::getImage test 7")
    {
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);

        SDL_RWops *const rw = VirtFs::RWopsOpenRead(name1);
        if (!rw)
            logger->log("Physfs error: %s", VirtFs::getLastError());
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
        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
        REQUIRE(compareBuffers(buf) == true);
    }

    SECTION("integrity Loader::getImage test 8")
    {
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);

        SDL_RWops *const rw = VirtFs::RWopsOpenRead(name1);
        if (!rw)
            logger->log("Physfs error: %s", VirtFs::getLastError());
        REQUIRE(rw != nullptr);
        if (IMG_isPNG(rw) == false)
        {
            SDL_RWclose(rw);
            REQUIRE(false);
        }
        SDL_Surface *const tmpImage = IMG_LoadPNG_RW(rw);
        SDL_RWclose(rw);
        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
        REQUIRE(tmpImage != nullptr);
        SDL_FreeSurface(tmpImage);
    }

    SECTION("integrity Loader::getImage test 9")
    {
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);

        SDL_RWops *const rw = VirtFs::RWopsOpenRead(name1);
        if (!rw)
            logger->log("Physfs error: %s", VirtFs::getLastError());
        REQUIRE(rw != nullptr);
        Resource *const res = imageHelper->load(rw);
        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
        REQUIRE(res != nullptr);
        res->decRef();
    }

    SECTION("integrity Loader::getImage test 10")
    {
        resourceManager->addToSearchPath("data/test/test.zip", Append_false);
        resourceManager->addToSearchPath("../data/test/test.zip", Append_false);
        Image *const image = Loader::getImage(
            name1);
        resourceManager->removeFromSearchPath("data/test/test.zip");
        resourceManager->removeFromSearchPath("../data/test/test.zip");
        REQUIRE(image != nullptr);
        REQUIRE(image->getSDLSurface() != nullptr);
        image->decRef();
    }

    delete client;
    client = nullptr;

//    VirtFs::deinit();
}
