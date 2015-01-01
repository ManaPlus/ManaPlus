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

#include "configmanager.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"
#include "settings.h"

#include "being/beingspeech.h"

#include "utils/files.h"
#include "utils/gettext.h"
#include "utils/mkdir.h"
#include "utils/paths.h"
#include "utils/physfstools.h"

#include "render/renderers.h"

#include "debug.h"

/**
 * Initializes the home directory. On UNIX and FreeBSD, ~/.mana is used. On
 * Windows and other systems we use the current working directory.
 */
void ConfigManager::initServerConfig(const std::string &serverName)
{
    settings.serverConfigDir = settings.configDir + dirSeparator + serverName;

    if (mkdir_r(settings.serverConfigDir.c_str()))
    {
        // TRANSLATORS: directory creation error
        logger->error(strprintf(_("%s doesn't exist and can't be created! "
            "Exiting."), settings.serverConfigDir.c_str()));
    }
    const std::string configPath = settings.serverConfigDir + "/config.xml";
    FILE *configFile = fopen(configPath.c_str(), "r");
    if (!configFile)
    {
        configFile = fopen(configPath.c_str(), "wt");
        logger->log("Creating new server config: " + configPath);
    }
    if (configFile)
    {
        fclose(configFile);
        serverConfig.init(configPath);
        serverConfig.setDefaultValues(getConfigDefaults());
        logger->log("serverConfigPath: " + configPath);
    }
}

void ConfigManager::initConfiguration()
{
#ifdef DEBUG_CONFIG
    config.setIsMain(true);
#endif

    // Fill configuration with defaults
    config.setValue("hwaccel", false);
#ifdef USE_OPENGL
#if (defined __APPLE__)
    config.setValue("opengl", static_cast<int>(RENDER_NORMAL_OPENGL));
#elif (defined ANDROID)
    config.setValue("opengl", static_cast<int>(RENDER_GLES_OPENGL));
#elif (defined WIN32)
    config.setValue("opengl", static_cast<int>(RENDER_SAFE_OPENGL));
#else
    config.setValue("opengl", static_cast<int>(RENDER_SOFTWARE));
#endif
#else
    config.setValue("opengl", static_cast<int>(RENDER_SOFTWARE));
#endif
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

    if (settings.options.test.empty())
        configPath = settings.configDir + "/config.xml";
    else
        configPath = settings.configDir + "/test.xml";

    FILE *configFile = fopen(configPath.c_str(), "r");
    if (!configFile)
    {
        configFile = fopen(configPath.c_str(), "wt");
        logger->log1("Creating new config");
    }
    if (!configFile)
    {
        logger->log("Can't create %s. Using defaults.", configPath.c_str());
    }
    else
    {
        fclose(configFile);
        config.init(configPath);
        logger->log1("init 3");
        config.setDefaultValues(getConfigDefaults());
        logger->log("configPath: " + configPath);
    }
}

void ConfigManager::backupConfig(const std::string &name)
{
    const std::string confName = std::string(settings.configDir).append(
        "/").append(name).append(".bak");
    const int maxFileIndex = 5;
    ::remove((confName + toString(maxFileIndex)).c_str());
    for (int f = maxFileIndex; f > 1; f --)
    {
        const std::string fileName1 = confName + toString(f - 1);
        const std::string fileName2 = confName + toString(f);
        Files::renameFile(fileName1, fileName2);
    }
    const std::string fileName3 = std::string(settings.configDir).append(
        "/").append(name);
    const std::string fileName4 = confName + toString(1);
    Files::copyFile(fileName3, fileName4);
}

#ifndef ANDROID
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

    if (!settings.options.safeMode && !tmpOpengl)
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
    config.setValue("opengl", static_cast<int>(RENDER_NORMAL_OPENGL));
#else
    config.setValue("opengl", static_cast<int>(RENDER_SOFTWARE));
#endif

    config.write();

    if (settings.options.safeMode)
    {
        isSafeMode = true;
        return;
    }

    config.setValue("safemode", false);
    if (!tmpOpengl)
    {
        config.setValue("hwaccel", tmpHwaccel);
        config.setValue("opengl", static_cast<int>(tmpOpengl));
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
        config.setValue("opengl", static_cast<int>(tmpOpengl));
        config.setValue("screenwidth", width);
        config.setValue("screenheight", height);
    }
}
#endif

#define unassignKey(key, value) \
    if (config.getStringValue(prefix + key) == value) \
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
#endif
    }
    if (version < 4)
    {
#ifdef ANDROID
        config.setValue("showDidYouKnow", false);
#endif
    }
    if (version < 5)
    {
        if (config.getIntValue("speech") == BeingSpeech::TEXT_OVERHEAD)
        {
            config.setValue("speech", static_cast<int>(
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
#else
        const std::string prefix = std::string();
#endif
        unassignKey("keyDirectUp", "k108");
        unassignKey("keyDirectDown", "k59");
        unassignKey("keyDirectLeft", "k107");
        unassignKey("keyDirectRight", "k39");
    }
    if (version < 12)
    {
#ifdef USE_SDL2
        const std::string prefix = std::string("sdl2");
#else
        const std::string prefix = std::string();
#endif
        unassignKey("keyAttack", "k120");
    }
    config.setValue("cfgver", 12);
}

#undef unassignKey
