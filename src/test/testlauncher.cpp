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

#include "test/testlauncher.h"

#include "client.h"
#include "configuration.h"
#include "graphics.h"
#include "graphicsmanager.h"
#include "localconsts.h"
#include "logger.h"
#include "sound.h"

#include "gui/theme.h"

#include "utils/gettext.h"
#include "utils/mkdir.h"
#include "utils/stringutils.h"

#include "resources/image.h"
#include "resources/wallpaper.h"

#include <unistd.h>

#ifdef WIN32
#include <windows.h>
#define sleep(seconds) Sleep((seconds) * 1000)
#endif

//#include <unistd.h>

#include "debug.h"

TestLauncher::TestLauncher(std::string test) :
    mTest(test)
{
    file.open((Client::getLocalDataDirectory()
        + std::string("/test.log")).c_str(), std::ios::out);
}

TestLauncher::~TestLauncher()
{
    file.close();
}

int TestLauncher::exec()
{
    if (mTest == "1" || mTest == "2" || mTest == "3")
        return testBackend();
    else if (mTest == "4")
        return testSound();
    else if (mTest == "5" || mTest == "6" || mTest == "7")
        return testRescale();
    else if (mTest == "8" || mTest == "9" || mTest == "10")
        return testFps();
    else if (mTest == "99")
        return testVideoDetection();
    else if (mTest == "100")
        return testInternal();

    return -1;
}

int TestLauncher::testBackend()
{
    Image *img = Theme::getImageFromTheme("graphics/sprites/arrow_up.gif");
    if (!img)
        return 1;
    int cnt = 100;

    for (int f = 0; f < cnt; f ++)
    {
        mainGraphics->drawImage(img, cnt * 7, cnt * 5);
        mainGraphics->updateScreen();
    }

    sleep(1);
    return 0;
}

int TestLauncher::testSound()
{
    sound.playGuiSfx("system/newmessage.ogg");
    sleep(1);
    sound.playSfx("system/newmessage.ogg", 0, 0);
    sound.playMusic("sfx/system/newmessage.ogg");
    sleep(3);
    sound.stopMusic();
    return 0;
}

int TestLauncher::testRescale()
{
    Wallpaper::loadWallpapers();
    const std::string wallpaperName = Wallpaper::getWallpaper(800, 600);
    volatile Image *img = Theme::getImageFromTheme(wallpaperName);
    if (!img)
        return 1;

    sleep(1);
    return 0;
}

int TestLauncher::testFps()
{
    timeval start;
    timeval end;

    Wallpaper::loadWallpapers();
    Wallpaper::getWallpaper(800, 600);
    Image *img[4];

    img[0] = Theme::getImageFromTheme("graphics/sprites/arrow_up.gif");
    img[1] = Theme::getImageFromTheme("graphics/sprites/arrow_down.gif");
    img[2] = Theme::getImageFromTheme("graphics/sprites/arrow_left.gif");
    img[3] = Theme::getImageFromTheme("graphics/sprites/arrow_right.gif");
    int idx = 0;

    int cnt = 500;

    gettimeofday(&start, nullptr);
    for (int k = 0; k < cnt; k ++)
    {
        for (int x = 0; x < 800; x += 20)
        {
            for (int y = 0; y < 600; y += 25)
            {
                mainGraphics->drawImage(img[idx], x, y);
                idx ++;
                if (idx > 3)
                    idx = 0;
            }
        }
        mainGraphics->updateScreen();
    }

    gettimeofday(&end, nullptr);
    int tFps = calcFps(&start, &end, cnt);
    file << mTest << std::endl;
    file << tFps << std::endl;

    sleep(1);
    return 0;
}

int TestLauncher::testInternal()
{
    timeval start;
    timeval end;

    Wallpaper::loadWallpapers();
    Wallpaper::getWallpaper(800, 600);
    Image *img[4];

    img[0] = Theme::getImageFromTheme(
        "graphics/sprites/manaplus_emotions.png");
    img[1] = Theme::getImageFromTheme(
        "graphics/sprites/manaplus_emotions.png");
    img[2] = Theme::getImageFromTheme("graphics/sprites/arrow_left.gif");
    img[3] = Theme::getImageFromTheme("graphics/sprites/arrow_right.gif");
    int idx = 0;
    int mem =  mainGraphics->getMemoryUsage();

//    int cnt = 5;
    int cnt = 5000;

    gettimeofday(&start, nullptr);
    for (int k = 0; k < cnt; k ++)
    {
        for (int x = 0; x < 800; x += 20)
        {
            for (int y = 0; y < 600; y += 25)
            {
                mainGraphics->drawImage(img[idx], x, y);
                mainGraphics->drawImage(img[idx], x + 1, y);
                mainGraphics->drawImage(img[idx], x, y + 5);

                idx ++;
                if (idx > 3)
                    idx = 0;
            }
        }
        mainGraphics->updateScreen();
    }

    gettimeofday(&end, nullptr);
    int tFps = calcFps(&start, &end, cnt);
    file << mTest << std::endl;
    file << tFps << std::endl;
    file << mem << std::endl;

    sleep(1);
    return 0;
}

int TestLauncher::testVideoDetection()
{
    graphicsManager.detectGraphics();
    file << mTest << std::endl;
    file << config.getIntValue("opengl") << std::endl;
}

int TestLauncher::calcFps(timeval *start, timeval *end, int calls)
{
    long mtime;
    long seconds;
    long useconds;

    seconds  = end->tv_sec  - start->tv_sec;
    useconds = end->tv_usec - start->tv_usec;

    mtime = (seconds * 1000 + useconds / 1000.0) + 0.5;
    if (mtime == 0)
        return 100000;

    return static_cast<long>(calls) * 1000 / mtime;
}
