/*
 *  The ManaPlus Client
 *  Copyright (C) 2017  The ManaPlus Developers
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

#include "utils/dumplibs.h"

#include "logger.h"

#include "utils/stringutils.h"

#include <png.h>
#include <SDL_image.h>
PRAGMACLANG6(GCC diagnostic push)
PRAGMACLANG6(GCC diagnostic ignored "-Wold-style-cast")
#include <SDL_net.h>
PRAGMACLANG6(GCC diagnostic pop)
#include <SDL_ttf.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#include <physfs.h>
#pragma GCC diagnostic pop
#include <zlib.h>

#include <curl/curl.h>

#include <libxml/xmlversion.h>

#include "debug.h"

#define dumpCompiledSdlVersion(text, prefix) \
    logger->log(" " text ": %d.%d.%d", \
        prefix##_MAJOR_VERSION, \
        prefix##_MINOR_VERSION, \
        prefix##_PATCHLEVEL)

#define sdlVersionJoin(prefix) \
    prefix##_MAJOR_VERSION, \
    prefix##_MINOR_VERSION, \
    prefix##_PATCHLEVEL

static void dumpLinkedSdlVersion(const char *const text,
                                 const SDL_version *const version)
{
    if (version)
    {
        logger->log(" %s: %d.%d.%d",
            text,
            version->major,
            version->minor,
            version->patch);
    }
}

static void compareVersions(const char *const libName,
                            const char *const buildVersion,
                            const char *const linkedVersion)
{
    if (strcmp(buildVersion, linkedVersion) != 0)
    {
        logger->assertLog(
            "%s: compiled and linked versions not same: %s vs %s",
            libName,
            buildVersion,
            linkedVersion);
    }
}

static void compareSDLVersions(const char *const libName,
                               const int major,
                               const int minor,
                               const int patch,
                               const SDL_version *const linkedVersion)
{
    const std::string buildVersionStr = strprintf("%d.%d.%d",
        major,
        minor,
        patch);
    const std::string linkedVersionStr = strprintf("%d.%d.%d",
        linkedVersion->major,
        linkedVersion->minor,
        linkedVersion->patch);
    if (buildVersionStr != linkedVersionStr)
    {
        logger->assertLog(
            "%s: compiled and linked versions not same: %s vs %s",
            libName,
            buildVersionStr.c_str(),
            linkedVersionStr.c_str());
    }
}

void dumpLibs()
{
    logger->log("Compiled with:");
    logger->log(" zLib: %s", ZLIB_VERSION);
    logger->log(" libxml2: %s", LIBXML_DOTTED_VERSION);
    logger->log(" libcurl: %s", LIBCURL_VERSION);
    logger->log(" libpng: %s", PNG_LIBPNG_VER_STRING);
    PHYSFS_Version physfsVersion;
    PHYSFS_VERSION(&physfsVersion);
    const std::string physfsCompiled = strprintf("%d.%d.%d",
        physfsVersion.major,
        physfsVersion.minor,
        physfsVersion.patch);
    logger->log(" libphysfs: %s", physfsCompiled.c_str());
    dumpCompiledSdlVersion("SDL", SDL);
    dumpCompiledSdlVersion("SDL_net", SDL_NET);
    dumpCompiledSdlVersion("SDL_image", SDL_IMAGE);
    dumpCompiledSdlVersion("SDL_ttf", SDL_TTF);

    logger->log("Linked with:");
#if ZLIB_VERNUM >= 0x1020
    logger->log(" zLib: %s", zlibVersion());
#endif  // ZLIB_VERNUM >= 0x1020
    PHYSFS_getLinkedVersion(&physfsVersion);
    const std::string physfsLinked = strprintf("%d.%d.%d",
        physfsVersion.major,
        physfsVersion.minor,
        physfsVersion.patch);
    logger->log(" libphysfs: %s", physfsLinked.c_str());
#ifdef LIBXML_TEST_VERSION
    LIBXML_TEST_VERSION
#endif  // LIBXML_TEST_VERSION
#ifdef USE_SDL2
    SDL_version sdlVersion;
    sdlVersion.major = 0;
    sdlVersion.minor = 0;
    sdlVersion.patch = 0;
    SDL_GetVersion(&sdlVersion);
    dumpLinkedSdlVersion("SDL", &sdlVersion);
#else  // USE_SDL2
    dumpLinkedSdlVersion("SDL", SDL_Linked_Version());
#endif  // USE_SDL2
    dumpLinkedSdlVersion("SDL_net", SDLNet_Linked_Version());
    dumpLinkedSdlVersion("SDL_image", IMG_Linked_Version());
    dumpLinkedSdlVersion("SDL_ttf", TTF_Linked_Version());

    compareVersions("zLib", ZLIB_VERSION, zlibVersion());
    compareVersions("libphysfs", physfsCompiled.c_str(), physfsLinked.c_str());
#ifdef USE_SDL2
    compareSDLVersions("SDL", sdlVersionJoin(SDL), &sdlVersion);
#else  // USE_SDL2
    compareSDLVersions("SDL", sdlVersionJoin(SDL), SDL_Linked_Version());
#endif  // USE_SDL2

    compareSDLVersions("SDL_net",
        sdlVersionJoin(SDL_NET),
        SDLNet_Linked_Version());
    compareSDLVersions("SDL_image",
        sdlVersionJoin(SDL_IMAGE),
        IMG_Linked_Version());
    compareSDLVersions("SDL_ttf",
        sdlVersionJoin(SDL_TTF),
        TTF_Linked_Version());
}
