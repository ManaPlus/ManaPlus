/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2016  The ManaPlus Developers
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

#include "resources/sdlimagehelper.h"
#ifdef USE_SDL2
#include "resources/surfaceimagehelper.h"
#endif  // USE_SDL2

#include "resources/dye/dye.h"
#include "resources/dye/dyepalette.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/delete2.h"
#include "utils/env.h"
#include "utils/physfstools.h"

#ifndef USE_SDL2
#include <SDL.h>
#endif  // USE_SDL2

#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER

#include "debug.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define buildHex(a, b, c, d) \
    (d) * 16777216U + (c) * 65536U + (b) * 256U + CAST_U32(a)
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
#define buildHex(a, b, c, d) \
    (a) * 16777216U + (b) * 65536U + (c) * 256U + CAST_U32(d)
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

TEST_CASE("Dye replaceSOGLColor 1")
{
    DyePalette palette("#00ff00,000011", 6);
    uint32_t data[1];
    data[0] = buildHex(0x01, 0x02, 0x03, 0x10);
    palette.replaceSOGLColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x01, 0x02, 0x03, 0x10));
}

TEST_CASE("Dye replaceSOGLColor 2")
{
    DyePalette palette("#01ff02,030411", 6);
    uint32_t data[1];
    data[0] = buildHex(0x20, 0x02, 0xff, 0x01);
    palette.replaceSOGLColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x20, 0x11, 0x04, 0x03));
}

TEST_CASE("Dye replaceSOGLColor 3")
{
    DyePalette palette("#404040,200000,0100ee,102030", 6);
    uint32_t data[1];
    data[0] = buildHex(0x40, 0xee, 0x00, 0x01);
    palette.replaceSOGLColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x40, 0x30, 0x20, 0x10));
}


TEST_CASE("Dye replaceAOGLColor 1")
{
    DyePalette palette("#00ff0010,00001120", 8);
    uint32_t data[1];
    data[0] = buildHex(0x10, 0x03, 0x02, 0x01);
    palette.replaceAOGLColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x10, 0x03, 0x02, 0x01));
}

TEST_CASE("Dye replaceAOGLColor 2")
{
    DyePalette palette("#00ff0120,020311ff", 8);
    uint32_t data[1];
    data[0] = buildHex(0x20, 0x01, 0xff, 0x00);
    palette.replaceAOGLColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0xff, 0x11, 0x03, 0x02));
}

TEST_CASE("Dye replaceAOGLColor 3")
{
    DyePalette palette("#40404040,20000000,0100ee40,102030ff", 8);
    uint32_t data[1];
    data[0] = buildHex(0x40, 0xee, 0x00, 0x01);
    palette.replaceAOGLColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0xff, 0x30, 0x20, 0x10));
}


TEST_CASE("Dye replaceSColor 1")
{
    DyePalette palette("#00ff00,000011", 6);
    uint32_t data[1];
    data[0] = buildHex(0x10, 0x03, 0x02, 0x01);
    palette.replaceSColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x10, 0x03, 0x02, 0x01));
}

TEST_CASE("Dye replaceSColor 2")
{
    DyePalette palette("#403020,706050", 6);
    uint32_t data[1];
    data[0] = buildHex(0x40, 0x30, 0x20, 0x10);
    palette.replaceSColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x70, 0x60, 0x50, 0x10));
}

TEST_CASE("Dye replaceSColor 3")
{
    DyePalette palette("#123456,000000,ff3020,706050", 6);
    uint32_t data[1];
    data[0] = buildHex(0xff, 0x30, 0x20, 0x10);
    palette.replaceSColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x70, 0x60, 0x50, 0x10));
}


TEST_CASE("Dye replaceAColor 1")
{
    DyePalette palette("#00ff0010,00001120", 8);
    uint32_t data[1];
    data[0] = buildHex(0x10, 0x03, 0x02, 0x01);
    palette.replaceAColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x10, 0x03, 0x02, 0x01));
}

TEST_CASE("Dye replaceAColor 2")
{
    DyePalette palette("#02ff0120,040311ff", 8);
    uint32_t data[1];
    data[0] = buildHex(0x02, 0xff, 0x01, 0x20);
    palette.replaceAColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x04, 0x03, 0x11, 0xff));
}

TEST_CASE("Dye replaceAColor 3")
{
    DyePalette palette("#40404040,20000000,0100ee40,102030ff", 8);
    uint32_t data[1];
    data[0] = buildHex(0x01, 0x00, 0xee, 0x40);
    palette.replaceAColor(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x10, 0x20, 0x30, 0xff));
}

TEST_CASE("Dye normalDye 1")
{
    Dye dye("R:#203040,506070");
    uint32_t data[1];
    data[0] = buildHex(0x50, 0x00, 0x00, 0x55);
    dye.normalDye(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x14, 0x1e, 0x28, 0x55));
}

TEST_CASE("Dye normalDye 2")
{
    Dye dye("G:#203040,506070");
    uint32_t data[1];
    data[0] = buildHex(0x00, 0x50, 0x00, 0x60);
    dye.normalDye(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x14, 0x1e, 0x28, 0x60));
}

TEST_CASE("Dye normalOGLDye 1")
{
    Dye dye("R:#203040,506070");
    uint32_t data[1];
    data[0] = buildHex(0x50, 0x00, 0x00, 0x55);
    dye.normalOGLDye(&data[0], 1);
    REQUIRE(data[0] == buildHex(0x50, 0x2a, 0x20, 0x15));
}

static void dyeCheck(const std::string &dyeString,
                     const std::string &dstName)
{
    const std::string srcName = "arrow_up.png";

    Image *const image1 = Loader::getImage(srcName + dyeString);
    Image *const image2 = Loader::getImage(dstName);
    REQUIRE(image1 != nullptr);
    REQUIRE(image2 != nullptr);
    SDL_Surface *const surface1 = image1->getSDLSurface();
    SDL_Surface *const surface2 = image2->getSDLSurface();
    REQUIRE(surface1 != nullptr);
    REQUIRE(surface2 != nullptr);
    REQUIRE(surface1->w == surface2->w);
    REQUIRE(surface1->h == surface2->h);
    REQUIRE(surface1->pixels != nullptr);
    REQUIRE(surface2->pixels != nullptr);
    const uint32_t *const ptr1 = static_cast<const uint32_t *>(
        surface1->pixels);
    const uint32_t *const ptr2 = static_cast<const uint32_t *>(
        surface2->pixels);
    const size_t sz = surface1->w * surface1->h;
    for (size_t idx = 0; idx < sz; idx ++)
    {
        REQUIRE(ptr1[idx] == ptr2[idx]);
    }
    image2->decRef();
    image1->decRef();
}

TEST_CASE("Dye real dye")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    PHYSFS_init("manaplus");
    dirSeparator = "/";
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    ResourceManager::init();
    resourceManager->addToSearchPath("data/test", Append_false);
    resourceManager->addToSearchPath("../data/test", Append_false);

#ifdef USE_SDL2
    imageHelper = new SurfaceImageHelper;

    SDLImageHelper::setRenderer(graphicsManager.createRenderer(
        graphicsManager.createWindow(640, 480, 0,
        SDL_WINDOW_SHOWN | SDL_SWSURFACE), SDL_RENDERER_SOFTWARE));
#else  // USE_SDL2

    imageHelper = new SDLImageHelper;

    graphicsManager.createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);
#endif  // USE_SDL2

    ActorSprite::load();

    SECTION("B dye")
    {
        dyeCheck("|B:#FFC88A", "arrow_up_B.png");
    }

    SECTION("S dye")
    {
        dyeCheck("|S:#0000FF,FF0000", "arrow_up_S.png");
    }

    SECTION("A dye")
    {
        dyeCheck("|A:#0000FFFF,FF000050", "arrow_up_A.png");
    }
    delete2(client);
}
