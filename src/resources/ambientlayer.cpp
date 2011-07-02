/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "resources/ambientlayer.h"

#include "graphics.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "debug.h"

AmbientLayer::AmbientLayer(Image *img, float parallax,
                           float speedX, float speedY, bool keepRatio):
    mImage(img), mParallax(parallax),
    mPosX(0), mPosY(0),
    mSpeedX(speedX), mSpeedY(speedY),
    mKeepRatio(keepRatio)
{
    if (!mImage)
        return;

    if (keepRatio && !mImage->useOpenGL()
        /*&& defaultScreenWidth != 0
        && defaultScreenHeight != 0*/
        && graphics->mWidth != defaultScreenWidth
        && graphics->mHeight != defaultScreenHeight)
    {
        // Rescale the overlay to keep the ratio as if we were on
        // the default resolution...
        Image *rescaledOverlay = ResourceManager::getInstance()->
            getRescaled(mImage, static_cast<int>(mImage->mBounds.w)
            / defaultScreenWidth * graphics->mWidth,
            static_cast<int>(mImage->mBounds.h)
            / defaultScreenHeight * graphics->mHeight);

        if (rescaledOverlay)
            mImage = rescaledOverlay;
        else
            mImage->incRef();
    }
    else
    {
        mImage->incRef();
    }
}

AmbientLayer::~AmbientLayer()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = 0;
    }
}

void AmbientLayer::update(int timePassed, float dx, float dy)
{
    if (!mImage)
        return;

    // Self scrolling of the overlay
    mPosX -= mSpeedX * static_cast<float>(timePassed) / 10;
    mPosY -= mSpeedY * static_cast<float>(timePassed) / 10;

    // Parallax scrolling
    mPosX += dx * mParallax;
    mPosY += dy * mParallax;

    int imgW = mImage->mBounds.w;
    int imgH = mImage->mBounds.h;

    // Wrap values
    while (mPosX > imgW)
        mPosX -= static_cast<float>(imgW);
    while (mPosX < 0)
        mPosX += static_cast<float>(imgW);

    while (mPosY > imgH)
        mPosY -= static_cast<float>(imgH);
    while (mPosY < 0)
        mPosY += static_cast<float>(imgH);
}

void AmbientLayer::draw(Graphics *graphics, int x, int y)
{
    if (!mImage)
        return;

    if (!mImage->useOpenGL() || !mKeepRatio)
    {
        graphics->drawImagePattern(mImage, static_cast<int>(-mPosX),
                static_cast<int>(-mPosY), x + static_cast<int>(mPosX),
                y + static_cast<int>(mPosY));
    }
    else
    {
        graphics->drawRescaledImagePattern(mImage, static_cast<int>(-mPosX),
                static_cast<int>(-mPosY), x + static_cast<int>(mPosX),
                y + static_cast<int>(mPosY),
                static_cast<int>(mImage->mBounds.w)
                / defaultScreenWidth * graphics->mWidth,
                static_cast<int>(mImage->mBounds.h) / defaultScreenHeight
                * graphics->mHeight);
    }
}
