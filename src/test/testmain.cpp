/*
 *  The ManaPlus Client
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

#include "test/testmain.h"

#ifdef USE_OPENGL

#include "logger.h"
#include "settings.h"

#include "fs/paths.h"

#include "utils/cast.h"
#include "utils/delete2.h"
#include "utils/process.h"

#include <fstream>

#include "debug.h"

std::string fileName;
extern char *selfName;

TestMain::TestMain() :
    log(new Logger),
    mConfig()
{
    fileName = getSelfName();
    log->setLogFile(pathJoin(settings.localDataDir, "manaplustest.log"));
}

TestMain::~TestMain()
{
    delete2(log)
}

void TestMain::initConfig()
{
    mConfig.init(settings.configDir + "/test.xml",
        UseVirtFs_false,
        SkipError_false);
    mConfig.clear();
//    setConfigDefaults(mConfig);

    mConfig.setValue("hwaccel", false);
    mConfig.setValue("screen", false);
    mConfig.setValue("sound", false);
    mConfig.setValue("guialpha", 0.8F);
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
    int soundTest = -1;
    int rescaleTest[6];
    int softFps = 0;
    int normalOpenGLFps = 0;
    int safeOpenGLFps = 0;
    int modernOpenGLFps = 0;
    int textureSize[6];
    std::string textureSizeStr;

    RenderType openGLMode = RENDER_SOFTWARE;
    int detectMode = 0;
    for (int f = 0; f < 6; f ++)
    {
        rescaleTest[f] = -1;
        textureSize[f] = 1024;
    }
    std::string info;

    const int videoDetectTest = invokeTest("99");
    if (videoDetectTest == 0)
        detectMode = readValue2(99);

    int normalOpenGLTest = invokeNormalOpenGLRenderTest("2");
    int safeOpenGLTest = invokeSafeOpenGLRenderTest("3");
    int modernOpenGLTest = invokeModernOpenGLRenderTest("19");
    if (testAudio)
        soundTest = invokeTest4();
    else
        soundTest = 0;

    info.append(strprintf("%d.%d,%d,%d,%d.", soundTest, softwareTest,
        normalOpenGLTest, safeOpenGLTest, modernOpenGLTest));

    if (softwareTest == 0)
    {
        int softFpsTest = invokeSoftwareRenderTest("8");
        info.append(strprintf("%d", softFpsTest));
        if (softFpsTest == 0)
        {
            softFps = readValue2(8);
            info.append(strprintf(",%d", softFps));
            if (softFps == 0)
            {
                softwareTest = -1;
                softFpsTest = -1;
            }
            else
            {
                rescaleTest[0] = invokeSoftwareRenderTest("5");
                info.append(strprintf(",%d", rescaleTest[0]));
            }
        }
        else
        {
            softwareTest = -1;
        }
    }
    info.append(".");
    if (modernOpenGLTest == 0)
    {
        int modernOpenGLFpsTest = invokeModernOpenGLRenderTest("17");
        info.append(strprintf("%d", modernOpenGLFpsTest));
        if (modernOpenGLFpsTest == 0)
        {
            modernOpenGLFps = readValue2(17);
            info.append(strprintf(",%d", modernOpenGLFps));
            if (modernOpenGLFps == 0)
            {
                modernOpenGLTest = -1;
                modernOpenGLFpsTest = -1;
            }
            else
            {
                rescaleTest[3] = invokeModernOpenGLRenderTest("18");
                info.append(strprintf(",%d", rescaleTest[3]));
            }
        }
        else
        {
            modernOpenGLTest = -1;
        }
    }
    info.append(".");
    if (normalOpenGLTest == 0)
    {
        int normalOpenGLFpsTest = invokeNormalOpenGLRenderTest("9");
        info.append(strprintf("%d", normalOpenGLFpsTest));
        if (normalOpenGLFpsTest == 0)
        {
            normalOpenGLFps = readValue2(9);
            info.append(strprintf(",%d", normalOpenGLFps));
            if (normalOpenGLFps == 0)
            {
                normalOpenGLTest = -1;
                normalOpenGLFpsTest = -1;
            }
            else
            {
                rescaleTest[1] = invokeNormalOpenGLRenderTest("6");
                info.append(strprintf(",%d", rescaleTest[1]));
            }
        }
        else
        {
            normalOpenGLTest = -1;
        }
    }
    info.append(".");
    if (safeOpenGLTest == 0)
    {
        int safeOpenGLFpsTest = invokeSafeOpenGLRenderTest("10");
        info.append(strprintf("%d", safeOpenGLFpsTest));
        if (safeOpenGLFpsTest == 0)
        {
            safeOpenGLFps = readValue2(10);
            info.append(strprintf(",%d", safeOpenGLFps));
            if (safeOpenGLFps == 0)
            {
                safeOpenGLTest = -1;
                safeOpenGLFpsTest = -1;
            }
            else
            {
                rescaleTest[2] = invokeSafeOpenGLRenderTest("7");
                info.append(strprintf(",%d", rescaleTest[2]));
            }
        }
        else
        {
            safeOpenGLTest = -1;
        }
    }
    info.append(".");

    int maxFps = softFps;
    if (maxFps < normalOpenGLFps)
    {
        openGLMode = RENDER_NORMAL_OPENGL;
        maxFps = normalOpenGLFps;
    }
    if (maxFps < modernOpenGLFps)
    {
        openGLMode = RENDER_MODERN_OPENGL;
        maxFps = modernOpenGLFps;
    }
    if (maxFps < safeOpenGLFps)
    {
        openGLMode = RENDER_SAFE_OPENGL;
        maxFps = safeOpenGLFps;
    }

    int batchSize = 256;

    if (invokeNormalOpenBatchTest("11") == 0)
        batchSize = readValue2(11);
    if (batchSize < 256)
        batchSize = 256;

    if (invokeNormalOpenBatchTest("14") == 0)
    {
        textureSize[CAST_SIZE(RENDER_NORMAL_OPENGL)]
            = readValue2(14);
    }
    if (invokeModernOpenBatchTest("15") == 0)
    {
        textureSize[CAST_SIZE(RENDER_MODERN_OPENGL)]
            = readValue2(15);
    }
    if (invokeSafeOpenBatchTest("16") == 0)
    {
        textureSize[CAST_SIZE(RENDER_SAFE_OPENGL)]
            = readValue2(16);
    }
    if (invokeMobileOpenBatchTest("20") == 0)
    {
        textureSize[CAST_SIZE(RENDER_GLES_OPENGL)]
            = readValue2(20);
    }
    for (int f = 0; f < 6; f ++)
        info.append(strprintf(",%d", textureSize[f]));
    info.append(",-");

    textureSizeStr = toString(textureSize[0]);
    for (int f = 1; f < 6; f ++)
        textureSizeStr.append(strprintf(",%d", textureSize[f]));

    // if OpenGL implimentation is not good, disable it.
    if ((detectMode & 15) == 0)
        openGLMode = RENDER_SOFTWARE;

    writeConfig(openGLMode, rescaleTest[CAST_SIZE(openGLMode)],
        soundTest, info, batchSize, textureSizeStr, detectMode);
    return 0;
}

void TestMain::writeConfig(const RenderType openGLMode,
                           const int rescale,
                           const int sound,
                           const std::string &info,
                           const int batchSize A_UNUSED,
                           const std::string &textureSize,
                           const int detectMode)
{
    mConfig.init(settings.configDir + "/config.xml",
        UseVirtFs_false,
        SkipError_false);

    log->log("set mode to %d", CAST_S32(openGLMode));

    // searched values
    mConfig.setValue("opengl", CAST_S32(openGLMode));
    mConfig.setValue("showBackground", rescale == 0);
    mConfig.setValue("sound", sound == 0);

    // better performance
    mConfig.setValue("hwaccel", true);
    mConfig.setValue("fpslimit", 60);
    mConfig.setValue("altfpslimit", 2);
    mConfig.setValue("safemode", false);
    mConfig.setValue("enableMapReduce", true);
    mConfig.setValue("textureSize", textureSize);

    // max batch size
//    mConfig.setValue("batchsize", batchSize);

    // additional modes
    mConfig.setValue("useTextureSampler",
        static_cast<bool>(detectMode & 1024));
    mConfig.setValue("compresstextures",
        static_cast<bool>(detectMode & 2048));

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
    file.open(pathJoin(settings.localDataDir, "test.log").c_str(),
        std::ios::in);
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

int TestMain::invokeTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_SOFTWARE));

    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 0);
    return ret;
}

int TestMain::invokeTest4()
{
    mConfig.setValue("sound", true);
    const int ret = invokeTest("4");

    log->log("4: %d", ret);
    return ret;
}

int TestMain::invokeSoftwareRenderTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_SOFTWARE));
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeNormalOpenGLRenderTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_NORMAL_OPENGL));
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeModernOpenGLRenderTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_MODERN_OPENGL));
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeNormalOpenBatchTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_NORMAL_OPENGL));
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
//    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeModernOpenBatchTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_MODERN_OPENGL));
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
//    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeMobileOpenBatchTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_GLES_OPENGL));
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
//    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeSafeOpenBatchTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_SAFE_OPENGL));
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
//    log->log("%s: %d", test.c_str(), ret);
    return ret;
}

int TestMain::invokeSafeOpenGLRenderTest(const std::string &test)
{
    mConfig.setValue("opengl", CAST_S32(RENDER_SAFE_OPENGL));
    mConfig.write();
    const int ret = execFileWait(fileName, fileName, "-t", test, 30);
    log->log("%s: %d", test.c_str(), ret);
    return ret;
}
#endif  // USE_OPENGL
