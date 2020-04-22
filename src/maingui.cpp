/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "maingui.h"

#include "client.h"
#include "commandline.h"
#include "settings.h"

#include "fs/virtfs/fs.h"

#include "utils/delete2.h"
#ifdef ANDROID
#include "fs/mkdir.h"
#include "fs/paths.h"
#endif  // ANDROID
#include "utils/process.h"

#ifdef __MINGW32__
#include <windows.h>
#endif  // __MINGW32__

#include <iostream>

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_image.h>
#include <SDL_mixer.h>
PRAGMA48(GCC diagnostic pop)

#ifndef SDL_VERSIONNUM
#error missing <SDL_version.h>
#endif  // SDL_VERSIONNUM

#ifdef UNITTESTS
#include "logger.h"

#include "utils/cpu.h"
#include "utils/sdlhelper.h"
#include "resources/dye/dyepalette.h"
#ifdef UNITTESTS_CATCH
#define CATCH_CONFIG_RUNNER
#ifdef UNITTESTS_EMBED
#include "unittests/catch.hpp"
#else  // UNITTESTS_EMBED
#include <catch.hpp>
#endif  // UNITTESTS_EMBED
#endif  // UNITTESTS_CATCH
#ifdef UNITTESTS_DOCTEST
#define DOCTEST_CONFIG_IMPLEMENT
#ifdef UNITTESTS_EMBED
#include "unittests/doctest.h"
#else  // UNITTESTS_EMBED
#include <doctest/doctest.h>
#endif  // UNITTESTS_EMBED
#endif  // UNITTESTS_DOCTEST
#else  // UNITTESTS
#include "utils/xml.h"
#endif  // UNITTESTS

#define SDL_IMAGE_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_IMAGE_MAJOR_VERSION, \
    SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL)

#define SDL_IMAGE_VERSION_ATLEAST(X, Y, Z) \
    (SDL_IMAGE_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))

#ifndef SDL_MIXER_COMPILEDVERSION
#define SDL_MIXER_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, \
    SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL)
#endif  // SDL_MIXER_COMPILEDVERSION

#ifndef SDL_MIXER_VERSION_ATLEAST
#define SDL_MIXER_VERSION_ATLEAST(X, Y, Z) \
    (SDL_MIXER_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))
#endif  // SDL_MIXER_VERSION_ATLEAST

#include "debug.h"

#ifdef __SWITCH__
extern "C" {
#include <switch/runtime/devices/socket.h>
#include <switch/runtime/nxlink.h>
}
#include <unistd.h>
static int sock = -1;
void initNxLink() {
    socketInitializeDefault();
    sock = nxlinkStdio();
    if (sock < 0) {
        socketExit();
    }
}
void deinitNxLink() {
    if (sock >= 0) {
        close(sock);
        socketExit();
        sock = -1;
    }
}
#endif

char *selfName = nullptr;

#ifndef UNITTESTS
#ifdef ANDROID
int main(int argc, char *argv[])
#else  // ANDROID

int mainGui(int argc, char *argv[])
#endif  // ANDROID
{
#ifdef __SWITCH__
    initNxLink();
#endif
#if defined(__MINGW32__)
    // load mingw crash handler. Won't fail if dll is not present.
    // may load libray from current dir, it may not same as program dir
    LoadLibrary("exchndl.dll");
#endif  // defined(__MINGW32__)

    selfName = argv[0];

    parseOptions(argc, argv);

    std::ios::sync_with_stdio(false);

#ifdef ANDROID
    mkdir_r(getSdStoragePath().c_str());
#endif  // ANDROID

    VirtFs::init(argv[0]);
    XML::initXML();
#if SDL_IMAGE_VERSION_ATLEAST(1, 2, 11)
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
#endif  // SDL_IMAGE_VERSION_ATLEAST(1, 2, 11)
#if SDL_MIXER_VERSION_ATLEAST(1, 2, 11)
    Mix_Init(MIX_INIT_OGG);
#endif  // SDL_MIXER_VERSION_ATLEAST(1, 2, 11)

#ifdef WIN32
    SetCurrentDirectory(VirtFs::getBaseDir());
#endif  // WIN32

    setPriority(true);
    client = new Client;
    int ret = 0;
    if (!settings.options.testMode)
    {
        client->gameInit();
        ret = client->gameExec();
    }
    else
    {
        client->testsInit();
        ret = Client::testsExec();
    }
    delete2(client)
    VirtFs::deinit();

#if SDL_MIXER_VERSION_ATLEAST(1, 2, 11)
    Mix_Quit();
#endif  // SDL_MIXER_VERSION_ATLEAST(1, 2, 11)
#if SDL_IMAGE_VERSION_ATLEAST(1, 2, 11)
    IMG_Quit();
#endif  // SDL_IMAGE_VERSION_ATLEAST(1, 2, 11)

#ifdef __SWITCH__
    deinitNxLink();
#endif

    return ret;
}
#else  // UNITTESTS

int main(int argc, char *argv[])
{
    logger = new Logger;
    SDL::initLogger();
    VirtFs::init(argv[0]);
    Cpu::detect();
    DyePalette::initFunctions();
#ifdef UNITTESTS_CATCH
    return Catch::Session().run(argc, argv);
#elif defined(UNITTESTS_DOCTEST)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
#else  // UNITTESTS_CATCH
    return 1;
#endif  // UNITTESTS_CATCH
}

#endif  // UNITTESTS
