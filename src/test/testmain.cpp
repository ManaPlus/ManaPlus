/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "test/testmain.h"

#include "utils/gettext.h"

#include "client.h"
#include "configuration.h"
#include "localconsts.h"

#include "utils/gettext.h"
#include "utils/mkdir.h"
#include "utils/stringutils.h"
#include "utils/process.h"

#include <iostream>

#include "debug.h"

std::string fileName = "";
extern char *selfName;

TestMain::TestMain()
{
#ifdef WIN32
    fileName = "manaplus.exe";
#else
    fileName = selfName;
#endif

#if defined __linux__ || defined __linux
    chdir(getFileDir(fileName).c_str());
#endif
    log = new Logger;
//    log->setLogFile(Client::getLocalDataDirectory()
//        + std::string("/test.log"));
    log->setLogFile(Client::getLocalDataDirectory()
        + std::string("/manaplustest.log"));
}

void TestMain::initConfig()
{
    config.init(Client::getConfigDirectory() + "/test.xml");
//    config.setDefaultValues(getConfigDefaults());

    config.setValue("hwaccel", false);
    config.setValue("screen", false);
    config.setValue("sound", false);
    config.setValue("guialpha", 0.8f);
    config.setValue("remember", true);
    config.setValue("sfxVolume", 50);
    config.setValue("musicVolume", 60);
    config.setValue("fpslimit", 0);
    config.setValue("customcursor", true);
    config.setValue("useScreenshotDirectorySuffix", true);
    config.setValue("ChatLogLength", 128);
    config.setValue("screenwidth", 800);
    config.setValue("screenheight", 600);
}

int TestMain::exec()
{
    initConfig();
    int softwareTest = invokeSoftwareRenderTest("1");
    int fastOpenGLTest = invokeFastOpenGLRenderTest("2");
    int safeOpenGLTest = invokeSafeOpenGLRenderTest("3");
    int soundTest = invokeTest4();
    int rescaleTest[3];
    int softFps = 0;
    int fastOpenGLFps = 0;
    int safeOpenGLFps = 0;

    int openGLMode = 0;
    int maxFps = 0;
    rescaleTest[0] = -1;
    rescaleTest[1] = -1;
    rescaleTest[2] = -1;
    std::string info;

    info += strprintf("%d.%d,%d,%d.", soundTest, softwareTest,
        fastOpenGLTest, safeOpenGLTest);

    if (!softwareTest)
    {
        int softFpsTest = invokeSoftwareRenderTest("8");
        info += strprintf ("%d", softFpsTest);
        if (!softFpsTest)
        {
            softFps = readValue(8);
            info += strprintf (",%d", softFps);
            if (!softFps)
            {
                softwareTest = -1;
                softFpsTest = -1;
            }
            else
            {
                rescaleTest[0] = invokeSoftwareRenderTest("5");
                info += strprintf (",%d", rescaleTest[0]);
            }
        }
        else
        {
            softwareTest = -1;
        }
    }
    info += ".";
    if (!fastOpenGLTest)
    {
        int fastOpenGLFpsTest = invokeFastOpenGLRenderTest("9");
        info += strprintf ("%d", fastOpenGLFpsTest);
        if (!fastOpenGLFpsTest)
        {
            fastOpenGLFps = readValue(9);
            info += strprintf (",%d", fastOpenGLFps);
            if (!fastOpenGLFps)
            {
                fastOpenGLTest = -1;
                fastOpenGLFpsTest = -1;
            }
            else
            {
                rescaleTest[1] = invokeFastOpenGLRenderTest("6");
                info += strprintf (",%d", rescaleTest[1]);
            }
        }
        else
        {
            fastOpenGLTest = -1;
        }
    }
    info += ".";
    if (!safeOpenGLTest)
    {
        int safeOpenGLFpsTest = invokeSafeOpenGLRenderTest("10");
        info += strprintf ("%d", safeOpenGLFpsTest);
        if (!safeOpenGLFpsTest)
        {
            safeOpenGLFps = readValue(10);
            info += strprintf (",%d", safeOpenGLFps);
            if (!safeOpenGLFps)
            {
                safeOpenGLTest = -1;
                safeOpenGLFpsTest = -1;
            }
            else
            {
                rescaleTest[2] = invokeSafeOpenGLRenderTest("7");
                info += strprintf (",%d", rescaleTest[2]);
            }
        }
        else
        {
            safeOpenGLTest = -1;
        }
    }
    info += ".";

    maxFps = softFps;
    if (maxFps < fastOpenGLFps)
    {
        openGLMode = 1;
        maxFps = fastOpenGLFps;
    }
    if (maxFps < safeOpenGLFps)
    {
        openGLMode = 2;
        maxFps = safeOpenGLFps;
    }

    writeConfig(openGLMode, rescaleTest[openGLMode], soundTest, info);
    return 0;
}

void TestMain::writeConfig(int openGLMode, int rescale,
                           int sound, std::string info)
{
    config.init(Client::getConfigDirectory() + "/config.xml");

    // searched values
    config.setValue("opengl", openGLMode);
    config.setValue("showBackground", !rescale);
    config.setValue("sound", !sound);

    // better perfomance
    config.setValue("hwaccel", true);
    config.setValue("fpslimit", 60);
    config.setValue("altfpslimit", 2);
    config.setValue("safemode", false);
    config.setValue("enableMapReduce", true);

    // stats
    config.setValue("testInfo", info);

    config.write();
}

int TestMain::readValue(int ver, int def)
{
    std::string tmp;
    int var;
    file.open((Client::getLocalDataDirectory()
        + std::string("/test.log")).c_str(), std::ios::in);
    if (!getline(file, tmp))
    {
        file.close();
        return def;
    }
    var = atoi(tmp.c_str());
    if (ver != var || !getline(file, tmp))
    {
        file.close();
        return def;
    }
    def = atoi(tmp.c_str());
    file.close();
    log->log("value for %d = %d", ver, def);
    return def;
}

int TestMain::invokeTest(std::string test)
{
    config.setValue("opengl", 0);

    config.write();
    int ret = execFile(fileName, fileName, "-t", test);
    return ret;
}

int TestMain::invokeTest4()
{
    config.setValue("sound", true);
    int ret = invokeTest("4");

    log->log("4: %d", ret);
    return ret;
}

int TestMain::invokeSoftwareRenderTest(std::string test)
{
    config.setValue("opengl", 0);
    config.write();
    int ret = execFile(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeFastOpenGLRenderTest(std::string test)
{
#if defined USE_OPENGL
    config.setValue("opengl", 1);
#else
    return -1;
#endif
    config.write();
    int ret = execFile(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeSafeOpenGLRenderTest(std::string test)
{
#if defined USE_OPENGL
    config.setValue("opengl", 2);
#else
    return -1;
#endif
    config.write();
    int ret = execFile(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
}
