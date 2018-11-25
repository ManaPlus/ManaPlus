/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "configmanager.h"

#include "client.h"
#include "configuration.h"
#include "settings.h"

#include "being/beingspeech.h"

#include "fs/files.h"
#include "fs/mkdir.h"
#include "fs/paths.h"

#include "utils/cast.h"
#include "utils/checkutils.h"
#include "utils/gettext.h"

#include "render/renderers.h"

#include "debug.h"

static void setDefaultOption(const char *const name,
                             const bool def)
{
    const int val = serverConfig.getValue(name, -1);
    if (val == -1)
        serverConfig.setValue(name, def);
}

/**
 * Initializes the home directory. On UNIX and FreeBSD, ~/.mana is used. On
 * Windows and other systems we use the current working directory.
 */
void ConfigManager::initServerConfig(const std::string &serverName)
{
    settings.serverConfigDir = pathJoin(settings.configDir, serverName);

    if (mkdir_r(settings.serverConfigDir.c_str()) != 0)
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), settings.serverConfigDir.c_str()));
    }
    const std::string configPath = settings.serverConfigDir + "/config.xml";
    FILE *configFile = fopen(configPath.c_str(), "r");
    if (configFile == nullptr)
    {
        configFile = fopen(configPath.c_str(), "wb");
        logger->log("Creating new server config: " + configPath);
        if (configFile != nullptr)
        {
            fputs("<?xml version=\"1.0\"?>\n", configFile);
            fputs("<configuration>\n", configFile);
            fputs("</configuration>\n", configFile);
        }
    }
    if (configFile != nullptr)
    {
        fclose(configFile);
        serverConfig.init(configPath,
            UseVirtFs_false,
            SkipError_false);
        setConfigDefaults(serverConfig);
        logger->log("serverConfigPath: " + configPath);
    }
    else
    {
        reportAlways("Error creating server config: %s",
            configPath.c_str())
    }

    const bool val = Client::isTmw();
    setDefaultOption("enableManaMarketBot", val);
    setDefaultOption("enableRemoteCommands", !val);
}

void ConfigManager::initConfiguration()
{
#ifdef DEBUG_CONFIG
    config.setIsMain(true);
#endif  // DEBUG_CONFIG

    // Fill configuration with defaults
    config.setValue("hwaccel", false);
#ifdef USE_OPENGL
#if (defined __APPLE__)
    config.setValue("opengl", CAST_S32(RENDER_NORMAL_OPENGL));
#elif (defined ANDROID)
    config.setValue("opengl", CAST_S32(RENDER_GLES_OPENGL));
#elif (defined WIN32)
    config.setValue("opengl", CAST_S32(RENDER_SAFE_OPENGL));
#else  // (defined __APPLE__)

    config.setValue("opengl", CAST_S32(RENDER_SOFTWARE));
#endif  // (defined __APPLE__)
#else  // USE_OPENGL

    config.setValue("opengl", CAST_S32(RENDER_SOFTWARE));
#endif  // USE_OPENGL

    config.setValue("screen", false);
    config.setValue("sound", true);
    config.setValue("guialpha", 0.8F);
//    config.setValue("remember", true);
    config.setValue("sfxVolume", 100);
    config.setValue("musicVolume", 60);
    config.setValue("fpslimit", 60);
    std::string defaultUpdateHost = branding.getValue("defaultUpdateHost", "");
    if (!checkPath(defaultUpdateHost))
        defaultUpdateHost.clear();
    config.setValue("updatehost", defaultUpdateHost);
    config.setValue("useScreenshotDirectorySuffix", true);
    config.setValue("ChatLogLength", 128);

    std::string configPath;

#ifndef UNITTESTS
    if (settings.options.test.empty())
        configPath = settings.configDir + "/config.xml";
    else
        configPath = settings.configDir + "/test.xml";
#else  // UNITTESTS

    configPath = settings.configDir + "/unittestconfig.xml";
#endif  // UNITTESTS

    FILE *configFile = fopen(configPath.c_str(), "r");
    if (configFile == nullptr)
    {
        configFile = fopen(configPath.c_str(), "wb");
        logger->log1("Creating new config");
        if (configFile != nullptr)
        {
            fputs("<?xml version=\"1.0\"?>\n", configFile);
            fputs("<configuration>\n", configFile);
            fputs("</configuration>\n", configFile);
        }
    }
    if (configFile == nullptr)
    {
        reportAlways("Can't create %s. Using defaults.",
            configPath.c_str())
    }
    else
    {
        fclose(configFile);
        config.init(configPath,
            UseVirtFs_false,
            SkipError_false);
        logger->log1("init 3");
        setConfigDefaults(config);
        setConfigDefaults(serverConfig);
        logger->log("configuration file: " + configPath);
    }
}

void ConfigManager::backupConfig(const std::string &name)
{
    const std::string fileName3 = pathJoin(settings.configDir, name);
    StringVect arr;
    if (Files::existsLocal(fileName3) == false)
    {
        logger->log("Config %s not exists, backup skipped.",
            name.c_str());
        return;
    }
    if (Files::loadTextFileLocal(fileName3, arr) == true)
    {
        if (arr.empty())
            return;

        arr.clear();
        const std::string tmpName = pathJoin(settings.configDir,
            name).append(".tmp");
        Files::copyFile(fileName3, tmpName);
        if (Files::loadTextFileLocal(tmpName, arr) == false ||
            arr.empty())
        {
            logger->safeError("Error backuping configs. "
                "Probably no free space on disk.");
        }
        arr.clear();
    }

    const std::string confName = pathJoin(settings.configDir,
        name).append(".bak");
    const int maxFileIndex = 5;
    ::remove((confName + toString(maxFileIndex)).c_str());
    for (int f = maxFileIndex; f > 1; f --)
    {
        const std::string fileName1 = confName + toString(f - 1);
        const std::string fileName2 = confName + toString(f);
        Files::renameFile(fileName1, fileName2);
    }
    const std::string fileName4 = confName + toString(1);
    Files::copyFile(fileName3, fileName4);
}

#ifdef __native_client__
void ConfigManager::storeSafeParameters()
{
    RenderType tmpOpengl;

    isSafeMode = config.getBoolValue("safemode");
    if (isSafeMode)
        logger->log1("Run in safe mode");

    tmpOpengl = intToRenderType(config.getIntValue("opengl"));

    config.setValue("opengl", CAST_S32(RENDER_SOFTWARE));

    config.write();

    if (settings.options.safeMode)
    {
        isSafeMode = true;
        return;
    }

    config.setValue("safemode", false);
    config.setValue("opengl", CAST_S32(tmpOpengl));
}
#elif !defined(ANDROID)
void ConfigManager::storeSafeParameters()
{
    bool tmpHwaccel;
    RenderType tmpOpengl;
    int tmpFpslimit;
    int tmpAltFpslimit;
    bool tmpSound;
    int width;
    int height;
    std::string font;
    std::string bFont;
    std::string particleFont;
    std::string helpFont;
    std::string secureFont;
    std::string npcFont;
    std::string japanFont;
    std::string chinaFont;
    bool showBackground;
    bool enableMumble;
    bool enableMapReduce;

    isSafeMode = config.getBoolValue("safemode");
    if (isSafeMode)
        logger->log1("Run in safe mode");

    tmpOpengl = intToRenderType(config.getIntValue("opengl"));

    width = config.getIntValue("screenwidth");
    height = config.getIntValue("screenheight");
    tmpHwaccel = config.getBoolValue("hwaccel");

    tmpFpslimit = config.getIntValue("fpslimit");
    tmpAltFpslimit = config.getIntValue("altfpslimit");
    tmpSound = config.getBoolValue("sound");

    font = config.getStringValue("font");
    bFont = config.getStringValue("boldFont");
    particleFont = config.getStringValue("particleFont");
    helpFont = config.getStringValue("helpFont");
    secureFont = config.getStringValue("secureFont");
    npcFont = config.getStringValue("npcFont");
    japanFont = config.getStringValue("japanFont");
    chinaFont = config.getStringValue("chinaFont");

    showBackground = config.getBoolValue("showBackground");
    enableMumble = config.getBoolValue("enableMumble");
    enableMapReduce = config.getBoolValue("enableMapReduce");

    if (!settings.options.safeMode && tmpOpengl == RENDER_SOFTWARE)
    {
        // if video mode configured reset most settings to safe
        config.setValue("hwaccel", false);
        config.setValue("altfpslimit", 3);
        config.setValue("sound", false);
        config.setValue("safemode", true);
        config.setValue("screenwidth", 640);
        config.setValue("screenheight", 480);
        config.setValue("font", "fonts/dejavusans.ttf");
        config.setValue("boldFont", "fonts/dejavusans-bold.ttf");
        config.setValue("particleFont", "fonts/dejavusans.ttf");
        config.setValue("helpFont", "fonts/dejavusansmono.ttf");
        config.setValue("secureFont", "fonts/dejavusansmono.ttf");
        config.setValue("npcFont", "fonts/dejavusans.ttf");
        config.setValue("japanFont", "fonts/mplus-1p-regular.ttf");
        config.setValue("chinaFont", "fonts/wqy-microhei.ttf");
        config.setValue("showBackground", false);
        config.setValue("enableMumble", false);
        config.setValue("enableMapReduce", false);
    }
    else
    {
        // if video mode not configured reset only video mode to safe
        config.setValue("screenwidth", 640);
        config.setValue("screenheight", 480);
    }
#if defined(__APPLE__)
    config.setValue("opengl", CAST_S32(RENDER_NORMAL_OPENGL));
#else  // defined(__APPLE__)

    config.setValue("opengl", CAST_S32(RENDER_SOFTWARE));
#endif  // defined(__APPLE__)

    config.write();

    if (settings.options.safeMode)
    {
        isSafeMode = true;
        return;
    }

    config.setValue("safemode", false);
    if (tmpOpengl == RENDER_SOFTWARE)
    {
        config.setValue("hwaccel", tmpHwaccel);
        config.setValue("opengl", CAST_S32(tmpOpengl));
        config.setValue("fpslimit", tmpFpslimit);
        config.setValue("altfpslimit", tmpAltFpslimit);
        config.setValue("sound", tmpSound);
        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
        config.setValue("font", font);
        config.setValue("boldFont", bFont);
        config.setValue("particleFont", particleFont);
        config.setValue("helpFont", helpFont);
        config.setValue("secureFont", secureFont);
        config.setValue("npcFont", npcFont);
        config.setValue("japanFont", japanFont);
        config.setValue("chinaFont", chinaFont);
        config.setValue("showBackground", showBackground);
        config.setValue("enableMumble", enableMumble);
        config.setValue("enableMapReduce", enableMapReduce);
    }
    else
    {
        config.setValue("opengl", CAST_S32(tmpOpengl));
        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
    }
}
#endif  // __native_client__

#define unassignKey(key, value) \
    if (config.getStringValue(prefix + (key)) == (value)) \
        config.setValue(key, "-1");

void ConfigManager::checkConfigVersion()
{
    const int version = config.getIntValue("cfgver");
    if (version < 1)
    {
        if (config.getIntValue("fontSize") == 11)
            config.deleteKey("fontSize");
        if (config.getIntValue("npcfontSize") == 13)
            config.deleteKey("npcfontSize");
    }
    if (version < 2)
    {
        if (config.getIntValue("screenButtonsSize") == 1)
            config.deleteKey("screenButtonsSize");
        if (config.getIntValue("screenJoystickSize") == 1)
            config.deleteKey("screenJoystickSize");
    }
    if (version < 3)
    {
        config.setValue("audioFrequency", 44100);
#ifdef ANDROID
        config.setValue("customcursor", false);
#endif  // ANDROID
    }
#ifdef ANDROID
    if (version < 4)
    {
        config.setValue("showDidYouKnow", false);
    }
#endif  // ANDROID

    if (version < 5)
    {
        if (config.getIntValue("speech") == BeingSpeech::TEXT_OVERHEAD)
        {
            config.setValue("speech", CAST_S32(
                BeingSpeech::NO_NAME_IN_BUBBLE));
        }
    }
    if (version < 6)
        config.setValue("blur", false);

    if (version < 7)
        config.setValue("download-music", true);

    if (version < 9)
    {
        config.deleteKey("videodetected");
        config.setValue("moveToTargetType", 10);
    }
    if (version < 10)
        config.setValue("enableLazyScrolling", false);

    if (version < 11)
    {
#ifdef USE_SDL2
        const std::string prefix = std::string("sdl2");
#else  // USE_SDL2

        const std::string prefix = std::string();
#endif  // USE_SDL2

        unassignKey("keyDirectUp", "k108")
        unassignKey("keyDirectDown", "k59")
        unassignKey("keyDirectLeft", "k107")
        unassignKey("keyDirectRight", "k39")
    }
    if (version < 12)
    {
#ifdef USE_SDL2
        const std::string prefix = std::string("sdl2");
#else  // USE_SDL2

        const std::string prefix = std::string();
#endif  // USE_SDL2

        unassignKey("keyAttack", "k120")
    }

    if (version < 13)
        config.setValue("keyWindowBotChecker", -1);

    if (version < 14 && config.getIntValue("syncPlayerMoveDistance") == 2)
        config.setValue("syncPlayerMoveDistance", 5);
    config.setValue("cfgver", 14);
}

#undef unassignKey
