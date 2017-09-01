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
#include "configuration.h"
#include "configmanager.h"
#include "dirs.h"
#include "graphicsmanager.h"

#include "being/actorsprite.h"

#include "fs/virtfs/fs.h"

#include "gui/gui.h"
#include "gui/theme.h"

#include "render/sdlgraphics.h"

#include "resources/sdlimagehelper.h"

#include "resources/db/palettedb.h"

#include "resources/dye/dyepalette.h"

#include "utils/delete2.h"
#include "utils/env.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifndef USE_SDL2
#include <SDL.h>
#endif  // USE_SDL2
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

TEST_CASE("DyePalette tests", "")
{
    setEnv("SDL_VIDEODRIVER", "dummy");

    client = new Client;
    XML::initXML();
    SDL_Init(SDL_INIT_VIDEO);
    logger = new Logger();
    VirtFs::mountDirSilent("data", Append_false);
    VirtFs::mountDirSilent("../data", Append_false);
    VirtFs::mountDirSilent("data/test", Append_false);
    VirtFs::mountDirSilent("../data/test", Append_false);

    mainGraphics = new SDLGraphics;
    imageHelper = new SDLImageHelper();
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

    ActorSprite::load();
    gui = new Gui();
    gui->postInit(mainGraphics);
    setPathsDefaults(paths);
    PaletteDB::load();

    SECTION("simple test 1")
    {
        DyePalette palette("#12ff34", 6);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x00);
    }

    SECTION("simple test 2")
    {
        DyePalette palette("#12ff3456", 8);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x56);
    }

    SECTION("simple test 3")
    {
        DyePalette palette("#12ff34,002211", 6);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x00);

        REQUIRE(palette.mColors[1].value[0] == 0x00);
        REQUIRE(palette.mColors[1].value[1] == 0x22);
        REQUIRE(palette.mColors[1].value[2] == 0x11);
        REQUIRE(palette.mColors[1].value[3] == 0x00);
    }

    SECTION("simple test 4")
    {
        DyePalette palette("#12ff3412,00221133", 8);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x12);

        REQUIRE(palette.mColors[1].value[0] == 0x00);
        REQUIRE(palette.mColors[1].value[1] == 0x22);
        REQUIRE(palette.mColors[1].value[2] == 0x11);
        REQUIRE(palette.mColors[1].value[3] == 0x33);
    }

    SECTION("simple test 5")
    {
        DyePalette palette("#12ff34,", 6);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x00);
    }

    SECTION("simple test 6")
    {
        DyePalette palette("#12ff3456,", 8);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x56);
    }

    SECTION("simple test 7")
    {
        DyePalette palette("#,,,12ff3412,,00221133", 8);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x12);

        REQUIRE(palette.mColors[1].value[0] == 0x00);
        REQUIRE(palette.mColors[1].value[1] == 0x22);
        REQUIRE(palette.mColors[1].value[2] == 0x11);
        REQUIRE(palette.mColors[1].value[3] == 0x33);
    }

    SECTION("palette test 1")
    {
        DyePalette palette("@Untitled1", 6);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 255);
    }

    SECTION("palette test 2")
    {
        DyePalette palette("@Untitled1,Untitled8", 6);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 255);

        REQUIRE(palette.mColors[1].value[0] == 0);
        REQUIRE(palette.mColors[1].value[1] == 0);
        REQUIRE(palette.mColors[1].value[2] == 255);
        REQUIRE(palette.mColors[1].value[3] == 255);
    }

    SECTION("palette test 3")
    {
        DyePalette palette("@Untitled1,", 6);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 255);
    }

    SECTION("palette test 4")
    {
        DyePalette palette("@,,,Untitled1,,Untitled8", 6);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 255);

        REQUIRE(palette.mColors[1].value[0] == 0);
        REQUIRE(palette.mColors[1].value[1] == 0);
        REQUIRE(palette.mColors[1].value[2] == 255);
        REQUIRE(palette.mColors[1].value[3] == 255);
    }

    SECTION("palette test 5")
    {
        DyePalette palette("@12ff34", 6);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x00);
    }

    SECTION("palette test 6")
    {
        DyePalette palette("@12ff3456", 8);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x56);
    }

    SECTION("palette test 7")
    {
        DyePalette palette("@12ff34,002211", 6);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x00);

        REQUIRE(palette.mColors[1].value[0] == 0x00);
        REQUIRE(palette.mColors[1].value[1] == 0x22);
        REQUIRE(palette.mColors[1].value[2] == 0x11);
        REQUIRE(palette.mColors[1].value[3] == 0x00);
    }

    SECTION("palette test 8")
    {
        DyePalette palette("@12ff3412,00221133", 8);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x12);

        REQUIRE(palette.mColors[1].value[0] == 0x00);
        REQUIRE(palette.mColors[1].value[1] == 0x22);
        REQUIRE(palette.mColors[1].value[2] == 0x11);
        REQUIRE(palette.mColors[1].value[3] == 0x33);
    }

    SECTION("palette test 9")
    {
        DyePalette palette("@12ff34,", 6);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x00);
    }

    SECTION("palette test 10")
    {
        DyePalette palette("@12ff3456,", 8);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x56);
    }

    SECTION("palette test 11")
    {
        DyePalette palette("@,,,12ff3412,,00221133", 8);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 0x12);
        REQUIRE(palette.mColors[0].value[1] == 0xff);
        REQUIRE(palette.mColors[0].value[2] == 0x34);
        REQUIRE(palette.mColors[0].value[3] == 0x12);

        REQUIRE(palette.mColors[1].value[0] == 0x00);
        REQUIRE(palette.mColors[1].value[1] == 0x22);
        REQUIRE(palette.mColors[1].value[2] == 0x11);
        REQUIRE(palette.mColors[1].value[3] == 0x33);
    }

    SECTION("palette test 12")
    {
        DyePalette palette("@Untitled1,334455", 6);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 255);

        REQUIRE(palette.mColors[1].value[0] == 0x33);
        REQUIRE(palette.mColors[1].value[1] == 0x44);
        REQUIRE(palette.mColors[1].value[2] == 0x55);
        REQUIRE(palette.mColors[1].value[3] == 0x00);
    }

    SECTION("palette test 13")
    {
        DyePalette palette("@Untitled1,33445566", 8);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 255);

        REQUIRE(palette.mColors[1].value[0] == 0x33);
        REQUIRE(palette.mColors[1].value[1] == 0x44);
        REQUIRE(palette.mColors[1].value[2] == 0x55);
        REQUIRE(palette.mColors[1].value[3] == 0x66);
    }

    SECTION("palette test 14")
    {
        DyePalette palette("@+77,Untitled1", 8);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 0x77);
    }

    SECTION("palette test 15")
    {
        DyePalette palette("@+87,Untitled1,Untitled8", 8);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 0x87);

        REQUIRE(palette.mColors[1].value[0] == 0);
        REQUIRE(palette.mColors[1].value[1] == 0);
        REQUIRE(palette.mColors[1].value[2] == 255);
        REQUIRE(palette.mColors[1].value[3] == 0x87);
    }

    SECTION("palette test 16")
    {
        DyePalette palette("@+87,Untitled1,+34,Untitled8", 8);
        REQUIRE(palette.mColors.size() == 2);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 0x87);

        REQUIRE(palette.mColors[1].value[0] == 0);
        REQUIRE(palette.mColors[1].value[1] == 0);
        REQUIRE(palette.mColors[1].value[2] == 255);
        REQUIRE(palette.mColors[1].value[3] == 0x34);
    }

    SECTION("palette test 17")
    {
        DyePalette palette("@+12,+23,+77,Untitled1", 8);
        REQUIRE(palette.mColors.size() == 1);
        REQUIRE(palette.mColors[0].value[0] == 47);
        REQUIRE(palette.mColors[0].value[1] == 56);
        REQUIRE(palette.mColors[0].value[2] == 46);
        REQUIRE(palette.mColors[0].value[3] == 0x77);
    }
    delete2(client);
    VirtFs::unmountDirSilent("data");
    VirtFs::unmountDirSilent("../data");
    VirtFs::unmountDirSilent("data/test");
    VirtFs::unmountDirSilent("../data/test");
    delete2(logger);
//    VirtFs::deinit();
}
