/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifdef USE_OPENGL

#include "client.h"
#include "graphicsmanager.h"
#include "soundmanager.h"

#include "render/sdlgraphics.h"

#include "gui/theme.h"

#include "utils/physfscheckutils.h"
#include "utils/physfsrwops.h"

#include "resources/dye.h"
#include "resources/image.h"
#include "resources/imagehelper.h"
#include "resources/imagewriter.h"
#include "resources/surfaceimagehelper.h"
#include "resources/wallpaper.h"

#include <unistd.h>

#ifdef WIN32
#include <windows.h>
#define sleep(seconds) Sleep((seconds) * 1000)
#endif

#include "debug.h"

TestLauncher::TestLauncher(std::string test) :
    mTest(test),
    file()
{
    file.open((client->getLocalDataDirectory()
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
    else if (mTest == "11")
        return testBatches();
    else if (mTest == "99")
        return testVideoDetection();
    else if (mTest == "100")
        return testInternal();
    else if (mTest == "101")
        return testDye();

    return -1;
}

int TestLauncher::testBackend() const
{
    const Image *const img = Theme::getImageFromTheme(
        "graphics/sprites/arrow_up.png");
    if (!img)
        return 1;
    const int cnt = 100;

    for (int f = 0; f < cnt; f ++)
    {
        mainGraphics->drawImage(img, cnt * 7, cnt * 5);
        mainGraphics->updateScreen();
    }

    sleep(1);
    return 0;
}

int TestLauncher::testSound() const
{
    soundManager.playGuiSfx("system/newmessage.ogg");
    sleep(1);
    soundManager.playSfx("system/newmessage.ogg", 0, 0);
    soundManager.playMusic("sfx/system/newmessage.ogg");
    sleep(3);
    soundManager.stopMusic();
    return 0;
}

int TestLauncher::testRescale() const
{
    Wallpaper::loadWallpapers();
    const std::string wallpaperName = Wallpaper::getWallpaper(800, 600);
    const volatile Image *const img = Theme::getImageFromTheme(wallpaperName);
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
    Image *img[5];
    const int sz = 4;

    img[0] = Theme::getImageFromTheme("graphics/sprites/arrow_up.png");
    img[1] = Theme::getImageFromTheme(
        "graphics/gui/target-cursor-normal-l.png");
    img[2] = Theme::getImageFromTheme("themes/wood/window.png");
    img[3] = Theme::getImageFromTheme("themes/pink/window.png");
    img[4] = Theme::getImageFromTheme("graphics/images/login_wallpaper.png");
    int idx = 0;

    const int cnt = 50;

    gettimeofday(&start, nullptr);
    for (int k = 0; k < cnt; k ++)
    {
        for (int x = 0; x < 800; x += 30)
        {
            for (int y = 0; y < 600; y += 50)
            {
                mainGraphics->drawImage(img[idx], x, y);
                idx ++;
                if (idx > sz)
                    idx = 0;
                mainGraphics->drawImage(img[idx], x, y);
                idx ++;
                if (idx > sz)
                    idx = 0;
            }
        }
        mainGraphics->updateScreen();
    }

    gettimeofday(&end, nullptr);
    const int tFps = calcFps(&start, &end, cnt);
    file << mTest << std::endl;
    file << tFps << std::endl;

    sleep(1);
    return 0;
}

int TestLauncher::testBatches()
{
    int batches = 512;

    file << mTest << std::endl;
    file << batches << std::endl;
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
    img[2] = Theme::getImageFromTheme("graphics/sprites/arrow_left.png");
    img[3] = Theme::getImageFromTheme("graphics/sprites/arrow_right.png");
    int idx = 0;
    const int mem =  mainGraphics->getMemoryUsage();

//    int cnt = 5;
    const int cnt = 5000;

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
    const int tFps = calcFps(&start, &end, cnt);
    file << mTest << std::endl;
    file << tFps << std::endl;
    file << mem << std::endl;

    sleep(1);
    return 0;
}

int TestLauncher::testDye()
{
    SDL_RWops *rw = MPHYSFSRWOPS_openRead(
        "graphics/sprites/arrow_up.png");
    Dye *d = nullptr;

    if (rw)
    {
        Image *image = d ? surfaceImageHelper->load(rw, *d)
            : surfaceImageHelper->load(rw);
        if (image)
        {
            const SDL_Rect &rect = image->mBounds;
            SDL_Surface *surface = surfaceImageHelper->create32BitSurface(
                rect.w, rect.h);
            if (surface)
            {
                SurfaceImageHelper::combineSurface(image->mSDLSurface, nullptr,
                    surface, nullptr);
                ImageWriter::writePNG(image->mSDLSurface,
                    client->getTempDirectory() + "/testimage1.png");
                ImageWriter::writePNG(surface,
                    client->getTempDirectory() + "/testimage2.png");
            }

            rw = MPHYSFSRWOPS_openRead(
                "graphics/sprites/arrow_up.png");
            d = new Dye("S:#0000ff,00ff00,5c5cff,ff0000");
            image = d ? surfaceImageHelper->load(rw, *d)
                : surfaceImageHelper->load(rw);
            if (image)
            {
                surface = surfaceImageHelper->create32BitSurface(
                    rect.w, rect.h);
                if (surface)
                {
                    SurfaceImageHelper::combineSurface(image->mSDLSurface,
                        nullptr, surface, nullptr);
                    ImageWriter::writePNG(image->mSDLSurface,
                        client->getTempDirectory() + "/testimage3.png");
                    ImageWriter::writePNG(surface,
                        client->getTempDirectory() + "/testimage4.png");
                }
            }
        }
    }
    return 0;
}

int TestLauncher::testVideoDetection()
{
    file << mTest << std::endl;
    file << graphicsManager.detectGraphics() << std::endl;
    return 0;
}

int TestLauncher::calcFps(const timeval *const start, const timeval *const end,
                          const int calls) const
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
#endif
