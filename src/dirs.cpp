/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "dirs.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"
#include "main.h"
#include "settings.h"

#include "fs/mkdir.h"
#include "fs/paths.h"

#include "fs/virtfs/fs.h"

#include "utils/base64.h"
#if defined(__native_client__) || (defined(ANDROID) && defined(USE_SDL2))
#include "fs/files.h"
#endif  // defined(__native_client__) || (defined(ANDROID) &&
        // defined(USE_SDL2))
#include "utils/gettext.h"

#ifdef ANDROID
#ifdef USE_SDL2
#include "main.h"

#include "render/graphics.h"
#endif  // USE_SDL2
#endif  // ANDROID

#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif  // __APPLE__

#ifdef WIN32
#include <SDL_syswm.h>
#include "fs/specialfolder.h"
#undef ERROR
#endif  // WIN32

#include <sys/stat.h>

#include <sstream>

#include "debug.h"

#if defined __native_client__
#define _nacl_dir std::string("/persistent/manaplus")
#endif  // defined __native_client__

#ifdef ANDROID
#ifdef USE_SDL2

int loadingProgressCounter = 1;

static void updateProgress(int cnt)
{
    const int progress = cnt + loadingProgressCounter;
    const int h = mainGraphics->mHeight;
    mainGraphics->setColor(Color(255, 255, 255));
    const int maxSize = mainGraphics->mWidth - 100;
    const int width = maxSize * progress / 50;
    mainGraphics->fillRectangle(Rect(50, h - 100, width, 50));
    mainGraphics->updateScreen();
}

void Dirs::setProgress()
{
    loadingProgressCounter++;
    updateProgress(loadingProgressCounter);
}

static void resetProgress()
{
    loadingProgressCounter = 0;
    updateProgress(loadingProgressCounter);
}

void extractAssets()
{
    if (!getenv("APPDIR"))
    {
        logger->log("error: APPDIR is not set!");
        return;
    }
    const std::string fileName = pathJoin(getenv("APPDIR"),
        "data.zip");
    logger->log("Extracting asset into: " + fileName);
    uint8_t *buf = new uint8_t[1000000];

    FILE *const file = fopen(fileName.c_str(), "w");
    for (int f = 0; f < 100; f ++)
    {
        std::string part = strprintf("manaplus-data.zip%u%u",
            CAST_U32(f / 10),
            CAST_U32(f % 10));
        logger->log("testing asset: " + part);
        SDL_RWops *const rw = SDL_RWFromFile(part.c_str(), "r");
        if (rw)
        {
            const int size = SDL_RWsize(rw);
            int size2 = SDL_RWread(rw, buf, 1, size);
            logger->log("asset size: %d", size2);
            fwrite(buf, 1, size2, file);
            SDL_RWclose(rw);
            Dirs::setProgress();
        }
        else
        {
            break;
        }
    }
    fclose(file);

    const std::string fileName2 = pathJoin(getenv("APPDIR"),
        "locale.zip");
    FILE *const file2 = fopen(fileName2.c_str(), "w");
    SDL_RWops *const rw = SDL_RWFromFile("manaplus-locale.zip", "r");
    if (rw)
    {
        const int size = SDL_RWsize(rw);
        int size2 = SDL_RWread(rw, buf, 1, size);
        fwrite(buf, 1, size2, file2);
        SDL_RWclose(rw);
        Dirs::setProgress();
    }
    fclose(file2);

    delete [] buf;
}

#endif  // USE_SDL2
#endif  // ANDROID

void Dirs::updateDataPath()
{
    if (settings.options.dataPath.empty()
        && !branding.getStringValue("dataPath").empty())
    {
        if (isRealPath(branding.getStringValue("dataPath")))
        {
            settings.options.dataPath = branding.getStringValue("dataPath");
        }
        else
        {
            settings.options.dataPath = pathJoin(branding.getDirectory(),
                branding.getStringValue("dataPath"));
        }
        settings.options.skipUpdate = true;
    }
}

void Dirs::extractDataDir()
{
#if defined(ANDROID) && defined(USE_SDL2)
    Files::setCopyCallBack(&updateProgress);
    resetProgress();
    extractAssets();

    const std::string zipName = pathJoin(getenv("APPDIR"), "data.zip");
    const std::string dirName = pathJoin(getenv("APPDIR"), "data");
    VirtFs::mountZip2(zipName,
        "data",
        Append_false);
    VirtFs::mountZip2(zipName,
        "data/perserver/default",
        Append_false);
    Files::extractLocale();
#endif  // defined(ANDROID) && defined(USE_SDL2)
}

void Dirs::mountDataDir()
{
    VirtFs::mountDirSilent(PKG_DATADIR "data/perserver/default",
        Append_false);
    VirtFs::mountDirSilent("data/perserver/default",
        Append_false);

#if defined __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL,
        TRUE,
        reinterpret_cast<uint8_t*>(path),
        PATH_MAX))
    {
        fprintf(stderr, "Can't find Resources directory\n");
    }
    CFRelease(resourcesURL);
    std::string path2 = pathJoin(path, "data");
    VirtFs::mountDir(pathJoin(path2, "perserver/default"), Append_false);
    VirtFs::mountDir(path2, Append_false);
// possible this need for support run client from dmg images.
//    mPackageDir = path;
#endif  // defined __APPLE__

    VirtFs::mountDirSilent(PKG_DATADIR "data", Append_false);
    setPackageDir(PKG_DATADIR "data");
    VirtFs::mountDirSilent("data", Append_false);

#ifdef ANDROID
#ifdef USE_SDL2
    if (getenv("APPDIR"))
    {
        const std::string appDir = getenv("APPDIR");
        VirtFs::mountDir(appDir + "/data", Append_false);
        VirtFs::mountDir(appDir + "/data/perserver/default",
            Append_false);
    }
#endif  // USE_SDL2
#endif  // ANDROID

#if defined __native_client__
    VirtFs::mountZip("/http/data.zip", Append_false);
    VirtFs::mountZip2("/http/data.zip",
        "perserver/default",
        Append_false);
#endif  // defined __native_client__

#ifndef WIN32
    // Add branding/data to VirtFS search path
    if (!settings.options.brandingPath.empty())
    {
        std::string path = settings.options.brandingPath;

        // Strip blah.manaplus from the path
        const int loc = CAST_S32(path.find_last_of('/'));

        if (loc > 0)
        {
            VirtFs::mountDir(path.substr(
                0, loc + 1).append("data"),
                Append_false);
        }
    }
#endif  // WIN32
}

void Dirs::initRootDir()
{
    settings.rootDir = VirtFs::getBaseDir();
    const std::string portableName = settings.rootDir + "portable.xml";
    struct stat statbuf;

    if (stat(portableName.c_str(), &statbuf) == 0 &&
        S_ISREG(statbuf.st_mode))
    {
        std::string dir;
        Configuration portable;
        portable.init(portableName);

        if (settings.options.brandingPath.empty())
        {
            branding.init(portableName);
            branding.setDefaultValues(getBrandingDefaults());
        }

        logger->log("Portable file: %s", portableName.c_str());

        if (settings.options.localDataDir.empty())
        {
            dir = portable.getValue("dataDir", "");
            if (!dir.empty())
            {
                settings.options.localDataDir = settings.rootDir + dir;
                logger->log("Portable data dir: %s",
                    settings.options.localDataDir.c_str());
            }
        }

        if (settings.options.configDir.empty())
        {
            dir = portable.getValue("configDir", "");
            if (!dir.empty())
            {
                settings.options.configDir = settings.rootDir + dir;
                logger->log("Portable config dir: %s",
                    settings.options.configDir.c_str());
            }
        }

        if (settings.options.screenshotDir.empty())
        {
            dir = portable.getValue("screenshotDir", "");
            if (!dir.empty())
            {
                settings.options.screenshotDir = settings.rootDir + dir;
                logger->log("Portable screenshot dir: %s",
                    settings.options.screenshotDir.c_str());
            }
        }
    }
}

/**
 * Initializes the home directory. On UNIX and FreeBSD, ~/.mana is used. On
 * Windows and other systems we use the current working directory.
 */
void Dirs::initHomeDir()
{
    initLocalDataDir();
    initTempDir();
    initConfigDir();
}

void Dirs::initLocalDataDir()
{
    settings.localDataDir = settings.options.localDataDir;

    if (settings.localDataDir.empty())
    {
#ifdef __APPLE__
        // Use Application Directory instead of .mana
        settings.localDataDir = pathJoin(VirtFs::getUserDir(),
            "Library/Application Support",
            branding.getValue("appName", "ManaPlus"));
#elif defined __HAIKU__
        settings.localDataDir = pathJoin(VirtFs::getUserDir(),
           "config/data/Mana");
#elif defined WIN32
        settings.localDataDir = getSpecialFolderLocation(CSIDL_LOCAL_APPDATA);
        if (settings.localDataDir.empty())
            settings.localDataDir = VirtFs::getUserDir();
        settings.localDataDir = pathJoin(settings.localDataDir,
            "Mana");
#elif defined __ANDROID__
        settings.localDataDir = pathJoin(getSdStoragePath(),
            branding.getValue("appShort", "ManaPlus"),
            "local");
#elif defined __native_client__
        settings.localDataDir = pathJoin(_nacl_dir, "local");
#else  // __APPLE__

        settings.localDataDir = pathJoin(VirtFs::getUserDir(),
            ".local/share/mana");
#endif  // __APPLE__
    }

    if (mkdir_r(settings.localDataDir.c_str()) != 0)
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), settings.localDataDir.c_str()));
    }
#ifdef USE_PROFILER
    Perfomance::init(pathJoin(settings.localDataDir, "profiler.log"));
#endif  // USE_PROFILER
}

void Dirs::initTempDir()
{
    settings.tempDir = pathJoin(settings.localDataDir, "temp");

    if (mkdir_r(settings.tempDir.c_str()) != 0)
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), settings.tempDir.c_str()));
    }
//    ResourceManager::deleteFilesInDirectory(settings.tempDir);
}

void Dirs::initConfigDir()
{
    settings.configDir = settings.options.configDir;

    if (settings.configDir.empty())
    {
#ifdef __APPLE__
        settings.configDir = pathJoin(settings.localDataDir,
            branding.getValue("appShort", "mana"));
#elif defined __HAIKU__
        settings.configDir = pathJoin(VirtFs::getUserDir(),
           "config/settings/Mana",
           branding.getValue("appName", "ManaPlus"));
#elif defined WIN32
        settings.configDir = getSpecialFolderLocation(CSIDL_APPDATA);
        if (settings.configDir.empty())
        {
            settings.configDir = settings.localDataDir;
        }
        else
        {
            settings.configDir = pathJoin(settings.configDir,
                "mana",
                branding.getValue("appShort", "mana"));
        }
#elif defined __ANDROID__
        settings.configDir = pathJoin(getSdStoragePath(),
            branding.getValue("appShort", "ManaPlus"),
            "config");
#elif defined __native_client__
        settings.configDir = pathJoin(_nacl_dir, "config");
#else  // __APPLE__

        settings.configDir = pathJoin(VirtFs::getUserDir(),
            ".config/mana",
            branding.getValue("appShort", "mana"));
#endif  // __APPLE__

        logger->log("Generating config dir: " + settings.configDir);
    }

    if (mkdir_r(settings.configDir.c_str()) != 0)
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), settings.configDir.c_str()));
    }
}

/**
 * Parse the update host and determine the updates directory
 * Then verify that the directory exists (creating if needed).
 */
void Dirs::initUpdatesDir()
{
    std::stringstream updates;

    // If updatesHost is currently empty, fill it from config file
    if (settings.updateHost.empty())
        settings.updateHost = config.getStringValue("updatehost");
    if (!checkPath(settings.updateHost))
        return;

    // Don't go out of range int he next check
    if (settings.updateHost.length() < 2)
    {
        if (settings.updatesDir.empty())
            settings.updatesDir = pathJoin("updates", settings.serverName);
        return;
    }

    const size_t sz = settings.updateHost.size();
    // Remove any trailing slash at the end of the update host
    if (settings.updateHost.at(sz - 1) == '/')
        settings.updateHost.resize(sz - 1);

    // Parse out any "http://" or "https://", and set the updates directory
    const size_t pos = settings.updateHost.find("://");
    if (pos != settings.updateHost.npos)
    {
        if (pos + 3 < settings.updateHost.length()
            && !settings.updateHost.empty())
        {
            updates << "updates/" << settings.updateHost.substr(pos + 3);
            settings.updatesDir = updates.str();
        }
        else
        {
            logger->log("Error: Invalid update host: %s",
                settings.updateHost.c_str());
            // TRANSLATORS: update server initialisation error
            errorMessage = strprintf(_("Invalid update host: %s."),
                settings.updateHost.c_str());
            client->setState(State::ERROR);
        }
    }
    else
    {
        logger->log1("Warning: no protocol was specified for the update host");
        updates << "updates/" << settings.updateHost;
        settings.updatesDir = updates.str();
    }

#ifdef WIN32
    if (settings.updatesDir.find(":") != std::string::npos)
        replaceAll(settings.updatesDir, ":", "_");
#endif  // WIN32

    const std::string updateDir("/" + settings.updatesDir);

    // Verify that the updates directory exists. Create if necessary.
    if (!VirtFs::isDirectory(updateDir))
    {
        if (!VirtFs::mkdir(updateDir))
        {
#if defined WIN32
            std::string newDir = pathJoin(settings.localDataDir,
                settings.updatesDir);
            if (!CreateDirectory(newDir.c_str(), nullptr) &&
                GetLastError() != ERROR_ALREADY_EXISTS)
            {
                logger->log("Error: %s can't be made, but doesn't exist!",
                            newDir.c_str());
                // TRANSLATORS: update server initialisation error
                errorMessage = _("Error creating updates directory!");
                client->setState(State::ERROR);
            }
#else  // defined WIN32

            logger->log("Error: %s/%s can't be made, but doesn't exist!",
                settings.localDataDir.c_str(),
                settings.updatesDir.c_str());
            // TRANSLATORS: update server initialisation error
            errorMessage = _("Error creating updates directory!");
            client->setState(State::ERROR);
#endif  // defined WIN32
        }
    }
    const std::string updateLocal = pathJoin(updateDir, "local");
    const std::string updateFix = pathJoin(updateDir, "fix");
    if (!VirtFs::isDirectory(updateLocal))
        VirtFs::mkdir(updateLocal);
    if (!VirtFs::isDirectory(updateFix))
        VirtFs::mkdir(updateFix);
}

void Dirs::initScreenshotDir()
{
    if (!settings.options.screenshotDir.empty())
    {
        settings.screenshotDir = settings.options.screenshotDir;
        if (mkdir_r(settings.screenshotDir.c_str()) != 0)
        {
            logger->log(strprintf(
                // TRANSLATORS: directory creation error
                _("Error: %s doesn't exist and can't be created! "
                "Exiting."), settings.screenshotDir.c_str()));
        }
    }
    else if (settings.screenshotDir.empty())
    {
#ifdef __native_client__
        settings.screenshotDir = pathJoin(_nacl_dir, "screenshots/");
#else  // __native_client__
        settings.screenshotDir = decodeBase64String(
            config.getStringValue("screenshotDirectory3"));
        if (settings.screenshotDir.empty())
        {
#ifdef __ANDROID__
            settings.screenshotDir = getSdStoragePath()
                + std::string("/images");

            if (mkdir_r(settings.screenshotDir.c_str()))
            {
                logger->log(strprintf(
                    // TRANSLATORS: directory creation error
                    _("Error: %s doesn't exist and can't be created! "
                    "Exiting."), settings.screenshotDir.c_str()));
            }
#else  // ANDROID
            settings.screenshotDir = getPicturesDir();
#endif  // ANDROID
            if (config.getBoolValue("useScreenshotDirectorySuffix"))
            {
                const std::string configScreenshotSuffix =
                    branding.getValue("screenshots", "ManaPlus");

                if (!configScreenshotSuffix.empty())
                {
                    settings.screenshotDir = pathJoin(settings.screenshotDir,
                        configScreenshotSuffix);
                }
            }
            config.setValue("screenshotDirectory3",
                encodeBase64String(settings.screenshotDir));
        }
#endif  // __native_client__
    }
    logger->log("screenshotDirectory: " + settings.screenshotDir);
}

void Dirs::initUsersDir()
{
    settings.usersDir = settings.serverConfigDir + "/users/";
    if (mkdir_r(settings.usersDir.c_str()) != 0)
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created!"),
            settings.usersDir.c_str()));
    }

    settings.npcsDir = settings.serverConfigDir + "/npcs/";
    if (mkdir_r(settings.npcsDir.c_str()) != 0)
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created!"),
            settings.npcsDir.c_str()));
    }

    settings.usersIdDir = settings.serverConfigDir + "/usersid/";
    if (mkdir_r(settings.usersIdDir.c_str()) != 0)
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created!"),
            settings.usersIdDir.c_str()));
    }
}
