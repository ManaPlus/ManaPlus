/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "main.h"

#include "client.h"
#include "commandline.h"
#include "settings.h"

#include "utils/delete2.h"
#ifdef ANDROID
#include "utils/mkdir.h"
#endif
#include "utils/physfscheckutils.h"
#include "utils/physfsrwops.h"
#include "utils/process.h"
#include "utils/xml.h"

#ifdef __MINGW32__
#include <windows.h>
#endif

#include <getopt.h>
#include <iostream>
#include <unistd.h>

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_version.h>

#define SDL_IMAGE_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_IMAGE_MAJOR_VERSION, \
    SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL)

#define SDL_IMAGE_VERSION_ATLEAST(X, Y, Z) \
    (SDL_IMAGE_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))

#define SDL_MIXER_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_MIXER_MAJOR_VERSION, \
    SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL)

#define SDL_MIXER_VERSION_ATLEAST(X, Y, Z) \
    (SDL_MIXER_COMPILEDVERSION >= SDL_VERSIONNUM(X, Y, Z))

#include "debug.h"

char *selfName = nullptr;

#ifndef UNITTESTS
int mainGui(int argc, char *argv[])
{
#if defined(__MINGW32__)
    // load mingw crash handler. Won't fail if dll is not present.
    // may load libray from current dir, it may not same as program dir
    LoadLibrary("exchndl.dll");
#endif

    selfName = argv[0];

    parseOptions(argc, argv);

    std::ios::sync_with_stdio(false);

#ifdef ANDROID
    mkdir_r(getSdStoragePath().c_str());
#endif

    PhysFs::init(argv[0]);
    XML::initXML();
#if SDL_IMAGE_VERSION_ATLEAST(1, 2, 11)
    IMG_Init(IMG_INIT_PNG);
#endif
#if SDL_MIXER_VERSION_ATLEAST(1, 2, 11)
    Mix_Init(MIX_INIT_OGG);
#endif

#ifdef WIN32
    SetCurrentDirectory(PhysFs::getBaseDir());
#endif
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
        ret = client->testsExec();
    }
    delete2(client);

#if SDL_MIXER_VERSION_ATLEAST(1, 2, 11)
    Mix_Quit();
#endif
#if SDL_IMAGE_VERSION_ATLEAST(1, 2, 11)
    IMG_Quit();
#endif

#ifdef DUMP_LEAKED_RESOURCES
    reportRWops();
#endif
#ifdef DEBUG_PHYSFS
    reportPhysfsLeaks();
#endif
    return ret;
}
#endif
