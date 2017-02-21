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

#include "fs/virtfs.h"

#include "resources/sdlimagehelper.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/env.h"
#include "utils/delete2.h"

#include <unistd.h>

#ifndef USE_SDL2
#include <SDL.h>
#endif  // USE_SDL2

#include "debug.h"

namespace
{
    static int testResouceCounter = 0;

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
            if (!v)
            {
                BLOCK_END("TestLoader::load")
                return nullptr;
            }

            const TestLoader *const rl
                = static_cast<const TestLoader *const>(v);

            std::string path1 = rl->path;
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
    dirSeparator = "/";
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager::init();
    ResourceManager *safeResman = resourceManager;
    VirtFs::addDirToSearchPath("data", Append_false);
    VirtFs::addDirToSearchPath("../data", Append_false);

    imageHelper = new SDLImageHelper();
#ifdef USE_SDL2
    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    ActorSprite::load();
    resourceManager = new ResourceManager;
    testResouceCounter = 0;

    SECTION("resourcemanager get 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager get 2")
    {
        TestLoader rl = { "test1" };
        Resource *const res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        res->setSource("source 1");
        REQUIRE(res->getSource() == "source 1");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        Resource *const res2 = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 2);
        REQUIRE(res2->getRefCount() == 2);
        REQUIRE(res->getIdPath() == res2->getIdPath());
        REQUIRE(res2->getSource() == "source 1");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res2);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        res2->decRef();
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager get 3")
    {
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->setSource("source 1");
        res->decRef();
        REQUIRE(resourceManager->mResources.size() == 0);
        REQUIRE(resourceManager->mOrphanedResources["test1"] == res);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager get 4")
    {
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->setSource("source 1");
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
        REQUIRE(resourceManager->mResources.size() == 0);
        REQUIRE(resourceManager->mOrphanedResources["test1"] == res);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        Resource *const res2 = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->getRefCount() == 1);
        REQUIRE(res->getIdPath() == res2->getIdPath());
        REQUIRE(res2->getSource() == "source 1");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res2);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res2->decRef();
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager get 5")
    {
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(resourceManager->isInCache("test1") == true);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->setSource("source 1");
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.size() == 1);
        REQUIRE(resourceManager->mOrphanedResources["test1"] == res);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        sleep(33);
        resourceManager->cleanOrphans();
        REQUIRE(resourceManager->isInCache("test1") == false);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        Resource *const res2 = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(resourceManager->isInCache("test1") == true);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->getRefCount() == 1);
        REQUIRE(res2->getSource() == "");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res2);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res2->decRef();
        REQUIRE(res2->getRefCount() == 0);
    }

    SECTION("resourcemanager get 6")
    {
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        res->setSource("source 1");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        Resource *const res2 = resourceManager->get("test2",
            TestLoader::load, &rl);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 2);
        REQUIRE(res2->getRefCount() == 1);
        REQUIRE(res2->getSource() == "");
        REQUIRE(resourceManager->mResources.size() == 2);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mResources["test2"] == res2);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        res2->decRef();
        REQUIRE(res->getRefCount() == 0);
        REQUIRE(res2->getRefCount() == 0);
    }

    SECTION("resourcemanager getFromCache 1")
    {
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        Resource *const res2 = resourceManager->getFromCache("test1");
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->getRefCount() == 2);
        REQUIRE(res == res2);
        REQUIRE(res2->getSource() == "");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res2);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager getFromCache 2")
    {
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
        Resource *const res2 = resourceManager->getFromCache("test1");
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->getRefCount() == 1);
        REQUIRE(res == res2);
        REQUIRE(res2->getSource() == "");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res2);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager getFromCache 3")
    {
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        res->setSource("source 1");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        Resource *const res2 = resourceManager->get("test2",
            TestLoader::load, &rl);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 2);
        REQUIRE(res2->getRefCount() == 1);
        REQUIRE(res2->getSource() == "");
        REQUIRE(resourceManager->mResources.size() == 2);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mResources["test2"] == res2);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        Resource *const resC = resourceManager->getFromCache("test1");
        Resource *const res2C = resourceManager->getFromCache("test2");
        REQUIRE(resC != nullptr);
        REQUIRE(res2C != nullptr);
        REQUIRE(testResouceCounter == 2);
        REQUIRE(res == resC);
        REQUIRE(res2 == res2C);
        REQUIRE(resC->getRefCount() == 2);
        REQUIRE(res2C->getRefCount() == 2);
        REQUIRE(resourceManager->mResources.size() == 2);
        REQUIRE(resourceManager->mResources["test1"] == resC);
        REQUIRE(resourceManager->mResources["test2"] == res2C);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        res->decRef();
        res->decRef();
        res2->decRef();
        res2->decRef();
        REQUIRE(res->getRefCount() == 0);
        REQUIRE(res2->getRefCount() == 0);
    }

    SECTION("resourcemanager addResource 1")
    {
        REQUIRE(testResouceCounter == 0);
        Resource *res = new TestResource();
        REQUIRE(testResouceCounter == 1);
        resourceManager->addResource("test1", res);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager isInCache 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        REQUIRE(resourceManager->isInCache("test1") == false);
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(resourceManager->isInCache("test1") == true);
        REQUIRE(resourceManager->isInCache("test2") == false);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager getTempResource 1")
    {
        TestLoader rl = { "test1" };
        REQUIRE(resourceManager->getTempResource("test1") == nullptr);
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(res != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        Resource *const res2 = resourceManager->getTempResource("test1");
        REQUIRE(resourceManager->getTempResource("test2") == nullptr);
        REQUIRE(res2 != nullptr);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res2->getRefCount() == 1);
        REQUIRE(res == res2);
        REQUIRE(res2->getSource() == "");
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res2);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
        REQUIRE(res->getRefCount() == 0);
    }

    SECTION("resourcemanager moveToDeleted 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        resourceManager->moveToDeleted(res);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
    }

    SECTION("resourcemanager moveToDeleted 2")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        res->incRef();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 2);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        resourceManager->moveToDeleted(res);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.size() == 1);
        res->decRef();
    }

    SECTION("resourcemanager moveToDeleted 3")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        res->decRef();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 0);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.size() == 1);
        REQUIRE(resourceManager->mOrphanedResources["test1"] == res);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        resourceManager->moveToDeleted(res);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
    }

    SECTION("resourcemanager decRefDelete 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);

        resourceManager->decRefDelete(res);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
    }

    SECTION("resourcemanager cleanUp 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();

        resourceManager->cleanUp(res);
        REQUIRE(testResouceCounter == 0);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.size() == 1);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        resourceManager->mOrphanedResources.clear();
    }

    SECTION("resourcemanager cleanProtected 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->setProtected(true);
        resourceManager->cleanProtected();

        REQUIRE(testResouceCounter == 1);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.size() == 1);
        REQUIRE(resourceManager->mOrphanedResources["test1"] == res);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        res->decRef();
    }

    SECTION("resourcemanager clearDeleted 1")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        res->incRef();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 2);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        resourceManager->moveToDeleted(res);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.size() == 1);

        resourceManager->clearDeleted();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.size() == 1);
        res->decRef();
    }

    SECTION("resourcemanager clearDeleted 2")
    {
        REQUIRE(testResouceCounter == 0);
        TestLoader rl = { "test1" };
        Resource *res = resourceManager->get("test1",
            TestLoader::load, &rl);
        res->incRef();
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res != nullptr);
        REQUIRE(res->getRefCount() == 2);
        REQUIRE(resourceManager->mResources.size() == 1);
        REQUIRE(resourceManager->mResources["test1"] == res);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
        resourceManager->moveToDeleted(res);
        REQUIRE(testResouceCounter == 1);
        REQUIRE(res->getRefCount() == 1);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.size() == 1);

        res->decRef();
        resourceManager->clearDeleted();
        REQUIRE(testResouceCounter == 0);
        REQUIRE(resourceManager->mResources.empty() == true);
        REQUIRE(resourceManager->mOrphanedResources.empty() == true);
        REQUIRE(resourceManager->mDeletedResources.empty() == true);
    }

    delete resourceManager;
    resourceManager = safeResman;
    delete2(client);
    delete2(logger);
//    VirtFs::deinit();
}
