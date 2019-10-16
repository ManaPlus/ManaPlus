/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
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

#ifndef USE_SDL2
#include "localconsts.h"
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_endian.h>
PRAGMA48(GCC diagnostic pop)
#if SDL_BYTEORDER == SDL_LIL_ENDIAN

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

#include "utils/delete2.h"
#include "utils/env.h"

#include "render/sdlgraphics.h"

#include "resources/sdlimagehelper.h"

#include "debug.h"

static SDL_Surface *createSoftware32BitSurface(int width,
                                               int height)
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

    return MSDL_CreateRGBSurface(SDL_SWSURFACE,
        width, height, 32, rmask, gmask, bmask, amask);
}

TEST_CASE("sdlimagehelper combineSurface", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper;

    Dirs::initRootDir();
    Dirs::initHomeDir();

    setBrandingDefaults(branding);
    ConfigManager::initConfiguration();

    GraphicsManager::createWindow(640, 480, 0, SDL_ANYFORMAT | SDL_SWSURFACE);

    userPalette = new UserPalette;

    theme = new Theme;
    Theme::selectSkin();

    ActorSprite::load();

    SECTION("empty copy1")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        for (int f = 0; f < 2 * 2; f ++)
            ptr1[f] = 0xff000000;
        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0xff000000;
        ptr2[1] = 0x00ff0000;
        ptr2[2] = 0x0000ff00;
        ptr2[3] = 0x000000ff;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        // src image test
        REQUIRE(ptr2[0] == 0xff000000);
        REQUIRE(ptr2[1] == 0x00ff0000);
        REQUIRE(ptr2[2] == 0x0000ff00);
        REQUIRE(ptr2[3] == 0x000000ff);

        REQUIRE(ptr1[0] == 0xff000000);
        REQUIRE(ptr1[1] == 0xff000000);
        REQUIRE(ptr1[2] == 0xff000000);
        REQUIRE(ptr1[3] == 0xff000000);
        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("empty copy2")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        for (int f = 0; f < 2 * 2; f ++)
            ptr1[f] = 0xff000000;
        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0xff112233;
        ptr2[1] = 0xff003344;
        ptr2[2] = 0xff330055;
        ptr2[3] = 0xff445500;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        REQUIRE(ptr1[0] == 0xff112233);
        REQUIRE(ptr1[1] == 0xff003344);
        REQUIRE(ptr1[2] == 0xff330055);
        REQUIRE(ptr1[3] == 0xff445500);
        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("empty copy3")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        for (int f = 0; f < 2 * 2; f ++)
            ptr1[f] = 0x00000000;
        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0xff112233;
        ptr2[1] = 0xff003344;
        ptr2[2] = 0xff330055;
        ptr2[3] = 0xff445500;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        REQUIRE(ptr1[0] == 0xff112233);
        REQUIRE(ptr1[1] == 0xff003344);
        REQUIRE(ptr1[2] == 0xff330055);
        REQUIRE(ptr1[3] == 0xff445500);
        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("empty copy4")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        for (int f = 0; f < 2 * 2; f ++)
            ptr1[f] = 0xff000000;
        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0x50112233;
        ptr2[1] = 0x50003344;
        ptr2[2] = 0x50330055;
        ptr2[3] = 0x50445500;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        REQUIRE(ptr1[0] == 0xff09121c);
        REQUIRE(ptr1[1] == 0xff001c25);
        REQUIRE(ptr1[2] == 0xff1c002f);
        REQUIRE(ptr1[3] == 0xff252f00);
        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("empty copy5")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        for (int f = 0; f < 2 * 2; f ++)
            ptr1[f] = 0x00000000;
        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0x50112233;
        ptr2[1] = 0x50003344;
        ptr2[2] = 0x50330055;
        ptr2[3] = 0x50445500;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        REQUIRE(ptr1[0] == 0x8e09121c);
        REQUIRE(ptr1[1] == 0x8e001c25);
        REQUIRE(ptr1[2] == 0x8e1c002f);
        REQUIRE(ptr1[3] == 0x8e252f00);
        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("empty copy6")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        ptr1[0] = 0x50112233;
        ptr1[1] = 0x50003344;
        ptr1[2] = 0x50330055;
        ptr1[3] = 0x50445500;
        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        for (int f = 0; f < 2 * 2; f ++)
            ptr2[f] = 0x00000000;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        REQUIRE(ptr1[0] == 0x50112233);
        REQUIRE(ptr1[1] == 0x50003344);
        REQUIRE(ptr1[2] == 0x50330055);
        REQUIRE(ptr1[3] == 0x50445500);
        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("mix 1")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        ptr1[0] = 0x50112233;
        ptr1[1] = 0x50003344;
        ptr1[2] = 0x50330055;
        ptr1[3] = 0x50445500;
        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0x50003344;
        ptr2[1] = 0x50330055;
        ptr2[2] = 0x50445500;
        ptr2[3] = 0x50112233;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        REQUIRE(ptr1[0] == 0xdf082b3c);
        REQUIRE(ptr1[1] == 0xdf1d174d);
        REQUIRE(ptr1[2] == 0xdf3d2f26);
        REQUIRE(ptr1[3] == 0xdf29391c);
        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("mix 2")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        ptr1[0] = 0x10112233;
        ptr1[1] = 0x20003344;
        ptr1[2] = 0x30330055;
        ptr1[3] = 0x40445500;
        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0x50003344;
        ptr2[1] = 0x60330055;
        ptr2[2] = 0x70445500;
        ptr2[3] = 0x80112233;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        REQUIRE(ptr1[0] == 0x9f082b3c);
        REQUIRE(ptr1[1] == 0xbd1f144e);
        REQUIRE(ptr1[2] == 0xb93f391e);
        REQUIRE(ptr1[3] == 0xf5213224);
        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("part mix 1")
    {
// 11 11 00 00
// 11 11 00 00
// 00 00 00 00
// 00 00 00 00

        SDL_Surface *surface1 = createSoftware32BitSurface(4, 4);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);
        ptr1[0] = 0x10112233;
        ptr1[1] = 0x20003344;
        ptr1[2] = 0x10203040;
        ptr1[3] = 0x20304050;
        ptr1[4] = 0x30330055;
        ptr1[5] = 0x40445500;
        ptr1[6] = 0x30405060;
        ptr1[7] = 0x708090a0;

        ptr1[8] = 0x8090a0b0;
        ptr1[9] = 0x90a0b0c0;
        ptr1[10] = 0xa0b0c0d0;
        ptr1[11] = 0xb0c0d0e0;
        ptr1[12] = 0xc0d0e0f0;
        ptr1[13] = 0xd0e0f000;
        ptr1[14] = 0xe0f00010;
        ptr1[15] = 0xf0001020;

        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0x50003344;
        ptr2[1] = 0x60330055;
        ptr2[2] = 0x70445500;
        ptr2[3] = 0x80112233;

        SDLImageHelper::combineSurface(surface2,
            nullptr,
            surface1,
            nullptr);

        REQUIRE(ptr1[0] == 0x9f082b3c);
        REQUIRE(ptr1[1] == 0xbd1f144e);
        REQUIRE(ptr1[2] == 0x10203040);
        REQUIRE(ptr1[3] == 0x20304050);
        REQUIRE(ptr1[4] == 0xb93f391e);
        REQUIRE(ptr1[5] == 0xf5213224);
        REQUIRE(ptr1[6] == 0x30405060);
        REQUIRE(ptr1[7] == 0x708090a0);

        REQUIRE(ptr1[8] == 0x8090a0b0);
        REQUIRE(ptr1[9] == 0x90a0b0c0);
        REQUIRE(ptr1[10] == 0xa0b0c0d0);
        REQUIRE(ptr1[11] == 0xb0c0d0e0);
        REQUIRE(ptr1[12] == 0xc0d0e0f0);
        REQUIRE(ptr1[13] == 0xd0e0f000);
        REQUIRE(ptr1[14] == 0xe0f00010);
        REQUIRE(ptr1[15] == 0xf0001020);

        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("part mix 2")
    {
// 00 00 00 00
// 00 11 11 00
// 00 11 11 00
// 00 00 00 00

        SDL_Surface *surface1 = createSoftware32BitSurface(4, 4);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);

        ptr1[0] = 0x10203040;
        ptr1[1] = 0x20304050;
        ptr1[2] = 0x30405060;
        ptr1[3] = 0x708090a0;
        ptr1[4] = 0x8090a0b0;

        ptr1[5] = 0x10112233;  // +
        ptr1[6] = 0x20003344;  // +

        ptr1[7] = 0x90a0b0c0;
        ptr1[8] = 0xa0b0c0d0;

        ptr1[9] = 0x30330055;  // +
        ptr1[10] = 0x40445500;  // +

        ptr1[11] = 0xb0c0d0e0;
        ptr1[12] = 0xc0d0e0f0;
        ptr1[13] = 0xd0e0f000;
        ptr1[14] = 0xe0f00010;
        ptr1[15] = 0xf0001020;

        SDL_Surface *surface2 = createSoftware32BitSurface(2, 2);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0x50003344;
        ptr2[1] = 0x60330055;
        ptr2[2] = 0x70445500;
        ptr2[3] = 0x80112233;

        SDL_Rect rect1;
        SDL_Rect rect2;
        rect1.x = 1;
        rect1.y = 1;
        rect1.w = 2;
        rect1.h = 2;
        rect2.x = 0;
        rect2.y = 0;
        rect2.w = 2;
        rect2.h = 2;
        SDLImageHelper::combineSurface(surface2,
            &rect2,
            surface1,
            &rect1);

        REQUIRE(ptr1[0] == 0x10203040);
        REQUIRE(ptr1[1] == 0x20304050);
        REQUIRE(ptr1[2] == 0x30405060);
        REQUIRE(ptr1[3] == 0x708090a0);
        REQUIRE(ptr1[4] == 0x8090a0b0);

        REQUIRE(ptr1[5] == 0x9f082b3c);  // +
        REQUIRE(ptr1[6] == 0xbd1f144e);  // +

        REQUIRE(ptr1[7] == 0x90a0b0c0);
        REQUIRE(ptr1[8] == 0xa0b0c0d0);

        REQUIRE(ptr1[9] == 0xb93f391e);  // +
        REQUIRE(ptr1[10] == 0xf5213224);  // +

        REQUIRE(ptr1[11] == 0xb0c0d0e0);
        REQUIRE(ptr1[12] == 0xc0d0e0f0);
        REQUIRE(ptr1[13] == 0xd0e0f000);
        REQUIRE(ptr1[14] == 0xe0f00010);
        REQUIRE(ptr1[15] == 0xf0001020);

        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    SECTION("part mix 3")
    {
        SDL_Surface *surface1 = createSoftware32BitSurface(4, 4);
        uint32_t *ptr1 = static_cast<uint32_t*>(surface1->pixels);

        ptr1[0] = 0x10203040;
        ptr1[1] = 0x20304050;
        ptr1[2] = 0x30405060;
        ptr1[3] = 0x708090a0;
        ptr1[4] = 0x8090a0b0;
        ptr1[5] = 0x10112233;
        ptr1[6] = 0x20003344;
        ptr1[7] = 0x90a0b0c0;
        ptr1[8] = 0xa0b0c0d0;
        ptr1[9] = 0x30330055;
        ptr1[10] = 0x40445500;
        ptr1[11] = 0xb0c0d0e0;
        ptr1[12] = 0xc0d0e0f0;
        ptr1[13] = 0xd0e0f000;
        ptr1[14] = 0xe0f00010;
        ptr1[15] = 0xf0001020;

        SDL_Surface *surface2 = createSoftware32BitSurface(4, 4);
        uint32_t *ptr2 = static_cast<uint32_t*>(surface2->pixels);

        ptr2[0] = 0x50003344;
        ptr2[1] = 0x60330055;
        ptr2[2] = 0x70445500;
        ptr2[3] = 0x80112233;
        ptr2[4] = 0x90111111;
        ptr2[5] = 0x90111111;
        ptr2[6] = 0xff000000;
        ptr2[7] = 0xff000000;
        ptr2[8] = 0xff000000;
        ptr2[9] = 0xff000000;
        ptr2[10] = 0xff000000;
        ptr2[11] = 0xff000000;
        ptr2[12] = 0xff000000;
        ptr2[13] = 0xff000000;
        ptr2[14] = 0xff000000;
        ptr2[15] = 0xff000000;

        SDL_Rect rect1;
        SDL_Rect rect2;
        rect1.x = 1;
        rect1.y = 1;
        rect1.w = 2;
        rect1.h = 2;
        rect2.x = 0;
        rect2.y = 0;
        rect2.w = 2;
        rect2.h = 2;
        SDLImageHelper::combineSurface(surface2,
            &rect2,
            surface1,
            &rect1);

        REQUIRE(ptr1[0] == 0x10203040);
        REQUIRE(ptr1[1] == 0x20304050);
        REQUIRE(ptr1[2] == 0x30405060);
        REQUIRE(ptr1[3] == 0x708090a0);
        REQUIRE(ptr1[4] == 0x8090a0b0);
        REQUIRE(ptr1[5] == 0x9f082b3c);
        REQUIRE(ptr1[6] == 0xbd1f144e);
        REQUIRE(ptr1[7] == 0x90a0b0c0);
        REQUIRE(ptr1[8] == 0xa0b0c0d0);
        REQUIRE(ptr1[9] == 0xbf1b0d23);
        REQUIRE(ptr1[10] == 0xff1f230c);
        REQUIRE(ptr1[11] == 0xb0c0d0e0);
        REQUIRE(ptr1[12] == 0xc0d0e0f0);
        REQUIRE(ptr1[13] == 0xd0e0f000);
        REQUIRE(ptr1[14] == 0xe0f00010);
        REQUIRE(ptr1[15] == 0xf0001020);

        MSDL_FreeSurface(surface1);
        MSDL_FreeSurface(surface2);
    }

    delete2(userPalette)
    delete2(theme)
    delete2(client)
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
}

#endif  // SDL_BYTEORDER == SDL_LIL_ENDIAN
#endif  // USE_SDL2
