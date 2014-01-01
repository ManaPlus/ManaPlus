/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/ambientlayer.h"

#include "map.h"

#include "render/graphics.h"

#include "resources/image.h"
#include "resources/imagehelper.h"
#include "resources/resourcemanager.h"

#include "debug.h"

AmbientLayer::AmbientLayer(Image *const img,
                           const float parallaxX, const float parallaxY,
                           const float speedX, const float speedY,
                           const bool keepRatio, const int mask) :
    mImage(img),
    mParallaxX(parallaxX),
    mParallaxY(parallaxY),
    mPosX(0),
    mPosY(0),
    mSpeedX(speedX),
    mSpeedY(speedY),
    mMask(mask),
    mKeepRatio(keepRatio)
{
    if (!mImage)
        return;

    if (keepRatio && imageHelper->useOpenGL() == RENDER_SOFTWARE)
    {
        const int width = mainGraphics->mWidth;
        const int height = mainGraphics->mHeight;
        if (width != defaultScreenWidth && height != defaultScreenHeight)
        {
            // Rescale the overlay to keep the ratio as if we were on
            // the default resolution...
            Image *const rescaledOverlay = ResourceManager::getInstance()->
                getRescaled(mImage, static_cast<int>(mImage->mBounds.w)
                / defaultScreenWidth * width,
                static_cast<int>(mImage->mBounds.h)
                / defaultScreenHeight * height);

            if (rescaledOverlay)
                mImage = rescaledOverlay;
            else
                mImage->incRef();
            return;
        }
    }
    mImage->incRef();
}

AmbientLayer::~AmbientLayer()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = nullptr;
    }
}

void AmbientLayer::update(const int timePassed, const float dx, const float dy)
{
    if (!mImage)
        return;

    const float time = static_cast<float>(timePassed) / 10;
    // Self scrolling of the overlay
    mPosX -= mSpeedX * time;
    mPosY -= mSpeedY * time;

    // Parallax scrolling
    mPosX += dx * mParallaxX;
    mPosY += dy * mParallaxY;

    const SDL_Rect &rect = mImage->mBounds;
    const float imgW = rect.w;
    const float imgH = rect.h;

    // Wrap values
    while (mPosX > imgW)
        mPosX -= imgW;
    while (mPosX < 0)
        mPosX += imgW;

    while (mPosY > imgH)
        mPosY -= imgH;
    while (mPosY < 0)
        mPosY += imgH;
}

void AmbientLayer::draw(Graphics *const graphics, const int x,
                        const int y) const
{
    if (!mImage)
        return;

    if (imageHelper->useOpenGL() == RENDER_SOFTWARE || !mKeepRatio)
    {
        graphics->drawPattern(mImage, static_cast<int>(-mPosX),
            static_cast<int>(-mPosY), x + static_cast<int>(mPosX),
            y + static_cast<int>(mPosY));
    }
    else
    {
        graphics->drawRescaledPattern(mImage, static_cast<int>(-mPosX),
                static_cast<int>(-mPosY), x + static_cast<int>(mPosX),
                y + static_cast<int>(mPosY),
                static_cast<int>(mImage->mBounds.w)
                / defaultScreenWidth * graphics->mWidth,
                static_cast<int>(mImage->mBounds.h) / defaultScreenHeight
                * graphics->mHeight);
    }
}
