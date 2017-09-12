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

#include "unittests/unittests.h"

#include "client.h"
#include "configmanager.h"
#include "configuration.h"
#include "dirs.h"
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "fs/virtfs/fs.h"

#include "gui/userpalette.h"
#include "gui/theme.h"

#include "resources/sdlimagehelper.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/env.h"
#include "utils/delete2.h"

#include <unistd.h>

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifndef USE_SDL2
#include <SDL.h>
#endif  // USE_SDL2
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

namespace
{
    int testResouceCounter = 0;

    class TestResource : public Resource
    {
        public:
            TestResource() :
                Resource()
            {
                testResouceCounter ++;
            }

            ~TestResource()
            {
                testResouceCounter --;
            }
    };

    struct TestLoader final
    {
        std::string path;
        static Resource *load(const void *const v)
        {
            BLOCK_START("TestLoader::load")
            if (v == nullptr)
            {
                BLOCK_END("TestLoader::load")
                return nullptr;
            }

            Resource *const res = new TestResource();
            BLOCK_END("TestLoader::load")
            return res;
        }
    };

}  // namespace

TEST_CASE("resourcemanager", "resourcemanager")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    imageHelper = new SDLImageHelper();
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    userPalette = new UserPalette;

    Dirs::initRootDir();
    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();

//    ConfigManager::initConfiguration();
//    setConfigDefaults2(config);

    while (ResourceManager::cleanOrphans(true))
        continue;

    testResouceCounter = 0;
    const size_t resSize = ResourceManager::getResources().size();

    SECTION("resourcemanager get 0")
    {
        REQUIRE(testResouceCounter == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
    }

    SECTION("resourcemanager get 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager get 2")
    {
        TestLoader rl = { "test1" };
        Resource *const res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        res->mSource = "source 1";
        REQUIRE(res->mSource == "source 1");
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        Resource *const res2 = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 2);
        REQUIRE(res2->mRefCount == 2);
        REQUIRE(res->mIdPath == res2->mIdPath);
        REQUIRE(res2->mSource == "source 1");
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res2);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        res2->decRef();
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager get 3")
    {
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->mSource = "source 1";
        res->decRef();
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources()["test1"] == res);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager get 4")
    {
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->mSource = "source 1";
        res->decRef();
        REQUIRE(res->mRefCount == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources()["test1"] == res);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        Resource *const res2 = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->mRefCount == 1);
        REQUIRE(res->mIdPath == res2->mIdPath);
        REQUIRE(res2->mSource == "source 1");
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res2);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res2->decRef();
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager get 5")
    {
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(ResourceManager::isInCache("test1") == true);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->mSource = "source 1";
        res->decRef();
        REQUIRE(res->mRefCount == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().size() == 1);
        REQUIRE(ResourceManager::getOrphanedResources()["test1"] == res);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        sleep(33);
        ResourceManager::cleanOrphans();
        REQUIRE(ResourceManager::isInCache("test1") == false);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        Resource *const res2 = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(ResourceManager::isInCache("test1") == true);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->mRefCount == 1);
        REQUIRE(res2->mSource.empty());
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res2);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res2->decRef();
        REQUIRE(res2->mRefCount == 0);
    }

    SECTION("resourcemanager get 6")
    {
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        res->mSource = "source 1";
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        Resource *const res2 = ResourceManager::get("test2",
            TestLoader::load, &rl);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 2);
        REQUIRE(res2->mRefCount == 1);
        REQUIRE(res2->mSource.empty());
        REQUIRE(ResourceManager::getResources().size() == 2 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getResources()["test2"] == res2);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        res2->decRef();
        REQUIRE(res->mRefCount == 0);
        REQUIRE(res2->mRefCount == 0);
    }

    SECTION("resourcemanager getFromCache 1")
    {
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        Resource *const res2 = ResourceManager::getFromCache("test1");
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->mRefCount == 2);
        REQUIRE(res == res2);
        REQUIRE(res2->mSource.empty());
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res2);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        res->decRef();
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager getFromCache 2")
    {
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        REQUIRE(res->mRefCount == 0);
        Resource *const res2 = ResourceManager::getFromCache("test1");
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->mRefCount == 1);
        REQUIRE(res == res2);
        REQUIRE(res2->mSource.empty());
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res2);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager getFromCache 3")
    {
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        res->mSource = "source 1";
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        Resource *const res2 = ResourceManager::get("test2",
            TestLoader::load, &rl);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 2);
        REQUIRE(res2->mRefCount == 1);
        REQUIRE(res2->mSource.empty());
        REQUIRE(ResourceManager::getResources().size() == 2 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getResources()["test2"] == res2);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        Resource *const resC = ResourceManager::getFromCache("test1");
        Resource *const res2C = ResourceManager::getFromCache("test2");
        REQUIRE(resC != nullptr);
        REQUIRE(res2C != nullptr);
        REQUIRE(testResouceCounter == 2);
        REQUIRE(res == resC);
        REQUIRE(res2 == res2C);
        REQUIRE(resC->mRefCount == 2);
        REQUIRE(res2C->mRefCount == 2);
        REQUIRE(ResourceManager::getResources().size() == 2 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == resC);
        REQUIRE(ResourceManager::getResources()["test2"] == res2C);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        res->decRef();
        res->decRef();
        res2->decRef();
        res2->decRef();
        REQUIRE(res->mRefCount == 0);
        REQUIRE(res2->mRefCount == 0);
    }

    SECTION("resourcemanager addResource 1")
    {
        REQUIRE(testResouceCounter == 0);
        Resource *res = new TestResource();
        REQUIRE(testResouceCounter == 1);
        ResourceManager::addResource("test1", res);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager isInCache 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        REQUIRE(ResourceManager::isInCache("test1") == false);
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(ResourceManager::isInCache("test1") == true);
        REQUIRE(ResourceManager::isInCache("test2") == false);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager getTempResource 1")
    {
        TestLoader rl = { "test1" };
        REQUIRE(ResourceManager::getTempResource("test1") == nullptr);
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        Resource *const res2 = ResourceManager::getTempResource("test1");
        REQUIRE(ResourceManager::getTempResource("test2") == nullptr);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->mRefCount == 1);
        REQUIRE(res == res2);
        REQUIRE(res2->mSource.empty());
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res2);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
        REQUIRE(res->mRefCount == 0);
    }

    SECTION("resourcemanager moveToDeleted 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        ResourceManager::moveToDeleted(res);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
    }

    SECTION("resourcemanager moveToDeleted 2")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        res->incRef();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 2);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        ResourceManager::moveToDeleted(res);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().size() == 1);
        res->decRef();
    }

    SECTION("resourcemanager moveToDeleted 3")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        res->decRef();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().size() == 1);
        REQUIRE(ResourceManager::getOrphanedResources()["test1"] == res);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        ResourceManager::moveToDeleted(res);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
    }

    SECTION("resourcemanager decRefDelete 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);

        ResourceManager::decRefDelete(res);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
    }

    SECTION("resourcemanager cleanUp 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();

        ResourceManager::cleanUp(res);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().size() == 1);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        ResourceManager::getOrphanedResources().clear();
    }

    SECTION("resourcemanager cleanProtected 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->mProtected = true;
        ResourceManager::cleanProtected();

        REQUIRE(testResouceCounter == 1);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().size() == 1);
        REQUIRE(ResourceManager::getOrphanedResources()["test1"] == res);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        res->decRef();
    }

    SECTION("resourcemanager clearDeleted 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        res->incRef();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 2);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        ResourceManager::moveToDeleted(res);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().size() == 1);

        ResourceManager::clearDeleted();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().size() == 1);
        res->decRef();
    }

    SECTION("resourcemanager clearDeleted 2")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = ResourceManager::get("test1",
            TestLoader::load, &rl);
        res->incRef();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->mRefCount == 2);
        REQUIRE(ResourceManager::getResources().size() == 1 + resSize);
        REQUIRE(ResourceManager::getResources()["test1"] == res);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
        ResourceManager::moveToDeleted(res);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->mRefCount == 1);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().size() == 1);

        res->decRef();
        ResourceManager::clearDeleted();
        REQUIRE(testResouceCounter == 0);
        REQUIRE(ResourceManager::getResources().size() == 0 + resSize);
        REQUIRE(ResourceManager::getOrphanedResources().empty() == true);
        REQUIRE(ResourceManager::getDeletedResources().empty() == true);
    }

    delete2(userPalette);
    delete2(theme);
    delete2(client);
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}
