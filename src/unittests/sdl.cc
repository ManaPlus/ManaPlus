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

#include "configuration.h"
#include "client.h"
#include "dirs.h"
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "fs/files.h"

#include "fs/virtfs/fs.h"

#include "gui/gui.h"
#include "gui/userpalette.h"
#include "gui/theme.h"

#include "resources/sdlimagehelper.h"
#ifdef USE_SDL2
#include "resources/surfaceimagehelper.h"
#endif  // USE_SDL2

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/env.h"
#include "utils/delete2.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_image.h>
PRAGMA48(GCC diagnostic pop)

#ifndef UNITTESTS_CATCH
#endif  // UNITTESTS_CATCH

#include "debug.h"

TEST_CASE("sdl leak test1", "")
{
    logger = new Logger();
    REQUIRE(gui == nullptr);
    ResourceManager::cleanOrphans(true);
    ResourceManager::deleteInstance();
    delete2(logger);
}

TEST_CASE("sdl tests", "sdl")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
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

    userPalette = new UserPalette;
    config.setValue("fontSize", 16);
    theme = new Theme;
    Theme::selectSkin();

    Dirs::initRootDir();
    Dirs::initHomeDir();

    ActorSprite::load();

    SECTION("sdl SDL DuplicateSurface1")
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int rmask = 0xff000000;
        const int gmask = 0x00ff0000;
        const int bmask = 0x0000ff00;
        const int amask = 0x000000ff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const int rmask = 0x000000ff;
        const int gmask = 0x0000ff00;
        const int bmask = 0x00ff0000;
        const int amask = 0xff000000;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        SDL_Surface *const surface = SDL_CreateRGBSurface(SDL_HWSURFACE,
            10, 10, 32, rmask, gmask, bmask, amask);
        uint32_t *ptr = reinterpret_cast<uint32_t*>(surface->pixels);
        for (int f = 0; f < 100; f ++)
        {
            ptr[f] = 300 * f;
        }
#ifdef USE_SDL2
        SDL_Surface *surface2 = SurfaceImageHelper::SDLDuplicateSurface(
            surface);
#else  // USE_SDL2

        SDL_Surface *surface2 = SDLImageHelper::SDLDuplicateSurface(
            surface);
#endif  // USE_SDL2

        uint32_t *ptr2 = reinterpret_cast<uint32_t*>(surface2->pixels);
        for (int f = 0; f < 100; f ++)
        {
            REQUIRE(ptr[f] == ptr2[f]);
        }

        SDL_FreeSurface(surface);
        SDL_FreeSurface(surface2);
    }

    SECTION("sdl SDL DuplicateSurface2")
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int rmask = 0x000000ff;
        const int gmask = 0x0000ff00;
        const int bmask = 0x00ff0000;
        const int amask = 0xff000000;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const int rmask = 0xff000000;
        const int gmask = 0x00ff0000;
        const int bmask = 0x0000ff00;
        const int amask = 0x000000ff;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        SDL_Surface *const surface = SDL_CreateRGBSurface(SDL_HWSURFACE,
            10, 10, 32, rmask, gmask, bmask, amask);
        uint32_t *ptr = reinterpret_cast<uint32_t*>(surface->pixels);
        for (int f = 0; f < 100; f ++)
        {
            ptr[f] = 300 * f;
        }
#ifdef USE_SDL2
        SDL_Surface *surface2 = SurfaceImageHelper::SDLDuplicateSurface(
            surface);
#else  // USE_SDL2

        SDL_Surface *surface2 = SDLImageHelper::SDLDuplicateSurface(
            surface);
#endif  // USE_SDL2

        uint32_t *ptr2 = reinterpret_cast<uint32_t*>(surface2->pixels);
        for (int f = 0; f < 100; f ++)
        {
            REQUIRE(ptr[f] == ptr2[f]);
        }

        SDL_FreeSurface(surface);
        SDL_FreeSurface(surface2);
    }

    SECTION("sdl SDL_ConvertSurface1")
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int rmask = 0xff000000;
        const int gmask = 0x00ff0000;
        const int bmask = 0x0000ff00;
        const int amask = 0x000000ff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const int rmask = 0x000000ff;
        const int gmask = 0x0000ff00;
        const int bmask = 0x00ff0000;
        const int amask = 0xff000000;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        SDL_Surface *const surface = SDL_CreateRGBSurface(SDL_HWSURFACE,
            4, 4, 32, rmask, gmask, bmask, amask);
        uint32_t *ptr = reinterpret_cast<uint32_t*>(surface->pixels);
        ptr[0] = 0x11223344;
        ptr[1] = 0x22334455;
        ptr[2] = 0x33445566;
        ptr[3] = 0x44556677;
        ptr[4] = 0x55667788;
        ptr[5] = 0x66778899;
        ptr[6] = 0x778899aa;
        ptr[7] = 0x8899aabb;
        ptr[8] = 0x99aabbcc;
        ptr[9] = 0xaabbccdd;
        ptr[10] = 0xbbccddee;
        ptr[11] = 0xccddeeff;
        ptr[12] = 0xff000000;
        ptr[13] = 0x00ff0000;
        ptr[14] = 0x0000ff00;
        ptr[15] = 0x000000ff;

        SDL_PixelFormat rgba;
        rgba.palette = nullptr;
        rgba.BitsPerPixel = 32;
        rgba.BytesPerPixel = 4;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rgba.Rmask = 0x000000FF;
        rgba.Gmask = 0x0000FF00;
        rgba.Bmask = 0x00FF0000;
        rgba.Amask = 0xFF000000;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        rgba.Rmask = 0xFF000000;
        rgba.Gmask = 0x00FF0000;
        rgba.Bmask = 0x0000FF00;
        rgba.Amask = 0x000000FF;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        SDL_Surface *const surface2 = MSDL_ConvertSurface(
            surface, &rgba, SDL_SWSURFACE);

        uint32_t *ptr2 = reinterpret_cast<uint32_t*>(surface2->pixels);
        REQUIRE(ptr2[0] == 0x44332211);
        REQUIRE(ptr2[1] == 0x55443322);
        REQUIRE(ptr2[2] == 0x66554433);
        REQUIRE(ptr2[3] == 0x77665544);
        REQUIRE(ptr2[4] == 0x88776655);
        REQUIRE(ptr2[5] == 0x99887766);
        REQUIRE(ptr2[6] == 0xaa998877);
        REQUIRE(ptr2[7] == 0xbbaa9988);
        REQUIRE(ptr2[8] == 0xccbbaa99);
        REQUIRE(ptr2[9] == 0xddccbbaa);
        REQUIRE(ptr2[10] == 0xeeddccbb);
        REQUIRE(ptr2[11] == 0xffeeddcc);
        REQUIRE(ptr2[12] == 0x000000ff);
        REQUIRE(ptr2[13] == 0x0000ff00);
        REQUIRE(ptr2[14] == 0x00ff0000);
        REQUIRE(ptr2[15] == 0xff000000);

        SDL_FreeSurface(surface);
        SDL_FreeSurface(surface2);
    }

    SECTION("sdl SDL_ConvertSurface2")
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const int rmask = 0x000000ff;
        const int gmask = 0x0000ff00;
        const int bmask = 0x00ff0000;
        const int amask = 0xff000000;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        const int rmask = 0xff000000;
        const int gmask = 0x00ff0000;
        const int bmask = 0x0000ff00;
        const int amask = 0x000000ff;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        SDL_Surface *const surface = SDL_CreateRGBSurface(SDL_HWSURFACE,
            4, 4, 32, rmask, gmask, bmask, amask);
        uint32_t *ptr = reinterpret_cast<uint32_t*>(surface->pixels);
        ptr[0] = 0x11223344;
        ptr[1] = 0x22334455;
        ptr[2] = 0x33445566;
        ptr[3] = 0x44556677;
        ptr[4] = 0x55667788;
        ptr[5] = 0x66778899;
        ptr[6] = 0x778899aa;
        ptr[7] = 0x8899aabb;
        ptr[8] = 0x99aabbcc;
        ptr[9] = 0xaabbccdd;
        ptr[10] = 0xbbccddee;
        ptr[11] = 0xccddeeff;
        ptr[12] = 0xff000000;
        ptr[13] = 0x00ff0000;
        ptr[14] = 0x0000ff00;
        ptr[15] = 0x000000ff;

        SDL_PixelFormat rgba;
        rgba.palette = nullptr;
        rgba.BitsPerPixel = 32;
        rgba.BytesPerPixel = 4;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rgba.Rmask = 0xFF000000;
        rgba.Gmask = 0x00FF0000;
        rgba.Bmask = 0x0000FF00;
        rgba.Amask = 0x000000FF;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        rgba.Rmask = 0x000000FF;
        rgba.Gmask = 0x0000FF00;
        rgba.Bmask = 0x00FF0000;
        rgba.Amask = 0xFF000000;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

        SDL_Surface *const surface2 = MSDL_ConvertSurface(
            surface, &rgba, SDL_SWSURFACE);

        uint32_t *ptr2 = reinterpret_cast<uint32_t*>(surface2->pixels);
        REQUIRE(ptr2[0] == 0x44332211);
        REQUIRE(ptr2[1] == 0x55443322);
        REQUIRE(ptr2[2] == 0x66554433);
        REQUIRE(ptr2[3] == 0x77665544);
        REQUIRE(ptr2[4] == 0x88776655);
        REQUIRE(ptr2[5] == 0x99887766);
        REQUIRE(ptr2[6] == 0xaa998877);
        REQUIRE(ptr2[7] == 0xbbaa9988);
        REQUIRE(ptr2[8] == 0xccbbaa99);
        REQUIRE(ptr2[9] == 0xddccbbaa);
        REQUIRE(ptr2[10] == 0xeeddccbb);
        REQUIRE(ptr2[11] == 0xffeeddcc);
        REQUIRE(ptr2[12] == 0x000000ff);
        REQUIRE(ptr2[13] == 0x0000ff00);
        REQUIRE(ptr2[14] == 0x00ff0000);
        REQUIRE(ptr2[15] == 0xff000000);

        SDL_FreeSurface(surface);
        SDL_FreeSurface(surface2);
    }

    SECTION("sdl SDL_ConvertSurface3")
    {
        const int rmask = 0x000000ff;
        const int gmask = 0x0000ff00;
        const int bmask = 0x00ff0000;
        const int amask = 0xff000000;

        SDL_Surface *const surface = SDL_CreateRGBSurface(SDL_HWSURFACE,
            4, 4, 32, rmask, gmask, bmask, amask);
        uint32_t *ptr = reinterpret_cast<uint32_t*>(surface->pixels);
        ptr[0] = 0x11223344;
        ptr[1] = 0x22334455;
        ptr[2] = 0x33445566;
        ptr[3] = 0x44556677;
        ptr[4] = 0x55667788;
        ptr[5] = 0x66778899;
        ptr[6] = 0x778899aa;
        ptr[7] = 0x8899aabb;
        ptr[8] = 0x99aabbcc;
        ptr[9] = 0xaabbccdd;
        ptr[10] = 0xbbccddee;
        ptr[11] = 0xccddeeff;
        ptr[12] = 0xff000000;
        ptr[13] = 0x00ff0000;
        ptr[14] = 0x0000ff00;
        ptr[15] = 0x000000ff;

        SDL_PixelFormat rgba;
        rgba.palette = nullptr;
        rgba.BitsPerPixel = 32;
        rgba.BytesPerPixel = 4;
        rgba.Rmask = 0x000000FF;
        rgba.Gmask = 0x0000FF00;
        rgba.Bmask = 0x00FF0000;
        rgba.Amask = 0xFF000000;

        SDL_Surface *const surface2 = MSDL_ConvertSurface(
            surface, &rgba, SDL_SWSURFACE);

        uint32_t *ptr2 = reinterpret_cast<uint32_t*>(surface2->pixels);
        REQUIRE(ptr2[0] == 0x11223344);
        REQUIRE(ptr2[1] == 0x22334455);
        REQUIRE(ptr2[2] == 0x33445566);
        REQUIRE(ptr2[3] == 0x44556677);
        REQUIRE(ptr2[4] == 0x55667788);
        REQUIRE(ptr2[5] == 0x66778899);
        REQUIRE(ptr2[6] == 0x778899aa);
        REQUIRE(ptr2[7] == 0x8899aabb);
        REQUIRE(ptr2[8] == 0x99aabbcc);
        REQUIRE(ptr2[9] == 0xaabbccdd);
        REQUIRE(ptr2[10] == 0xbbccddee);
        REQUIRE(ptr2[11] == 0xccddeeff);
        REQUIRE(ptr2[12] == 0xff000000);
        REQUIRE(ptr2[13] == 0x00ff0000);
        REQUIRE(ptr2[14] == 0x0000ff00);
        REQUIRE(ptr2[15] == 0x000000ff);

        SDL_FreeSurface(surface);
        SDL_FreeSurface(surface2);
    }

    ResourceManager::cleanOrphans(true);
    delete2(userPalette);
    delete2(theme);
    delete2(client);

    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("sdl leak test2", "")
{
    logger = new Logger();
    REQUIRE(gui == nullptr);
    ResourceManager::cleanOrphans(true);
    ResourceManager::deleteInstance();
    delete2(logger);
}
