/*
 *  The ManaPlus Client
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "const/render/graphics.h"

#include "render/graphics.h"

#include "resources/imagehelper.h"

#include "resources/image/image.h"

#include "resources/loaders/rescaledloader.h"

#include "debug.h"

AmbientLayer::AmbientLayer(const std::string &name,
                           Image *const img,
                           const float parallaxX,
                           const float parallaxY,
                           const float posX,
                           const float posY,
                           const float speedX,
                           const float speedY,
                           const bool keepRatio,
                           const int mask) :
    MemoryCounter(),
    mName(name),
    mImage(img),
    mParallaxX(parallaxX),
    mParallaxY(parallaxY),
    mPosX(posX),
    mPosY(posY),
    mSpeedX(speedX),
    mSpeedY(speedY),
    mMask(mask),
    mKeepRatio(keepRatio)
{
    if (mImage == nullptr)
        return;

    if (keepRatio &&
        imageHelper->useOpenGL() == RENDER_SOFTWARE)
    {
        const int width = mainGraphics->mWidth;
        const int height = mainGraphics->mHeight;
        if (width != defaultScreenWidth && height != defaultScreenHeight)
        {
            // Rescale the overlay to keep the ratio as if we were on
            // the default resolution...
            Image *const rescaledOverlay = Loader::getRescaled(
                mImage,
                CAST_S32(mImage->mBounds.w) /
                defaultScreenWidth * width,
                CAST_S32(mImage->mBounds.h) /
                defaultScreenHeight * height);

            if (rescaledOverlay != nullptr)
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
    if (mImage != nullptr)
    {
        mImage->decRef();
        mImage = nullptr;
    }
}

void AmbientLayer::update(const int timePassed, const float dx, const float dy)
{
    if (mImage == nullptr)
        return;

    const float time = static_cast<float>(timePassed) / 10;
    // Self scrolling of the overlay
    mPosX -= mSpeedX * time;
    mPosY -= mSpeedY * time;

    // Parallax scrolling
    mPosX += dx * mParallaxX;
    mPosY += dy * mParallaxY;

    const SDL_Rect &rect = mImage->mBounds;
    const float imgW = static_cast<float>(rect.w);
    const float imgH = static_cast<float>(rect.h);

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
    if (mImage == nullptr)
        return;

    if (imageHelper->useOpenGL() == RENDER_SOFTWARE ||
        !mKeepRatio)
    {
        graphics->drawPattern(mImage, CAST_S32(-mPosX),
            CAST_S32(-mPosY), x + CAST_S32(mPosX),
            y + CAST_S32(mPosY));
    }
    else
    {
        graphics->drawRescaledPattern(mImage, CAST_S32(-mPosX),
                CAST_S32(-mPosY), x + CAST_S32(mPosX),
                y + CAST_S32(mPosY),
                CAST_S32(mImage->mBounds.w)
                / defaultScreenWidth * graphics->mWidth,
                CAST_S32(mImage->mBounds.h) / defaultScreenHeight
                * graphics->mHeight);
    }
}

int AmbientLayer::calcMemoryLocal() const
{
    return static_cast<int>(sizeof(AmbientLayer) +
        mName.capacity());
}
