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

#include "utils/mkdir.h"
#include "utils/paths.h"
#include "utils/process.h"

#include <iostream>

#include "debug.h"

std::string fileName = "";
extern char *selfName;

TestMain::TestMain()
{
    fileName = getSelfName();

    log = new Logger;
//    log->setLogFile(Client::getLocalDataDirectory()
//        + std::string("/test.log"));
    log->setLogFile(Client::getLocalDataDirectory()
        + std::string("/manaplustest.log"));
}

void TestMain::initConfig()
{
    mConfig.init(Client::getConfigDirectory() + "/test.xml");
//    mConfig.setDefaultValues(getConfigDefaults());

    mConfig.setValue("hwaccel", false);
    mConfig.setValue("screen", false);
    mConfig.setValue("sound", false);
    mConfig.setValue("guialpha", 0.8f);
//    mConfig.setValue("remember", true);
    mConfig.setValue("sfxVolume", 50);
    mConfig.setValue("musicVolume", 60);
    mConfig.setValue("fpslimit", 0);
    mConfig.setValue("customcursor", true);
    mConfig.setValue("useScreenshotDirectorySuffix", true);
    mConfig.setValue("ChatLogLength", 128);
    mConfig.setValue("screenwidth", 800);
    mConfig.setValue("screenheight", 600);
}

int TestMain::exec(const bool testAudio)
{
    initConfig();
    int softwareTest = invokeSoftwareRenderTest("1");
    int fastOpenGLTest = -1;
    int safeOpenGLTest = -1;
    int videoDetectTest = -1;
    int soundTest = -1;
    int rescaleTest[3];
    int softFps = 0;
    int fastOpenGLFps = 0;
    int safeOpenGLFps = 0;

    int openGLMode = 0;
    int maxFps = 0;
    int detectMode = 0;
    rescaleTest[0] = -1;
    rescaleTest[1] = -1;
    rescaleTest[2] = -1;
    std::string info;

    videoDetectTest = invokeTest("99");
    if (!videoDetectTest)
        detectMode = readValue2(99);

    fastOpenGLTest = invokeFastOpenGLRenderTest("2");
    safeOpenGLTest = invokeSafeOpenGLRenderTest("3");
    if (testAudio)
        soundTest = invokeTest4();
    else
        soundTest = true;

    info += strprintf("%d.%d,%d,%d.", soundTest, softwareTest,
        fastOpenGLTest, safeOpenGLTest);

    if (!softwareTest)
    {
        int softFpsTest = invokeSoftwareRenderTest("8");
        info += strprintf ("%d", softFpsTest);
        if (!softFpsTest)
        {
            softFps = readValue2(8);
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
            fastOpenGLFps = readValue2(9);
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
            safeOpenGLFps = readValue2(10);
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

    // if OpenGL implimentation is not good, disable it.
    if (!detectMode)
        openGLMode = 0;

    writeConfig(openGLMode, rescaleTest[openGLMode], soundTest, info);
    return 0;
}

void TestMain::writeConfig(const int openGLMode, const int rescale,
                           const int sound, const std::string &info)
{
    mConfig.init(Client::getConfigDirectory() + "/config.xml");

    log->log("set mode to %d", openGLMode);

    // searched values
    mConfig.setValue("opengl", openGLMode);
    mConfig.setValue("showBackground", !rescale);
    mConfig.setValue("sound", !sound);

    // better perfomance
    mConfig.setValue("hwaccel", true);
    mConfig.setValue("fpslimit", 60);
    mConfig.setValue("altfpslimit", 2);
    mConfig.setValue("safemode", false);
    mConfig.setValue("enableMapReduce", true);

    // stats
    mConfig.setValue("testInfo", info);

    mConfig.write();
}

int TestMain::readValue2(const int ver)
{
    const int def = readValue(ver, 0);
    log->log("value for %d = %d", ver, def);
    return def;
}

int TestMain::readValue(const int ver, int def)
{
    std::string tmp;
    int var;
    std::ifstream file;
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
    return def;
}

int TestMain::invokeTest(std::string test)
{
    mConfig.setValue("opengl", 0);

    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test);
    return ret;
}

int TestMain::invokeTest4()
{
    mConfig.setValue("sound", true);
    const int ret = invokeTest("4");

    log->log("4: %d", ret);
    return ret;
}

int TestMain::invokeSoftwareRenderTest(std::string test)
{
    mConfig.setValue("opengl", 0);
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeFastOpenGLRenderTest(std::string test)
{
#if defined USE_OPENGL
    mConfig.setValue("opengl", 1);
    mConfig.write();
    int ret = execFileWait(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
#else
    return -1;
#endif
}

int TestMain::invokeSafeOpenGLRenderTest(std::string test)
{
#if defined USE_OPENGL
    mConfig.setValue("opengl", 2);
    mConfig.write();
    int ret = execFileWait(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
#else
    return -1;
#endif
}
