/*
 *  The ManaPlus Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "particle/imageparticle.h"

#include "render/graphics.h"

#include "resources/image/image.h"

#include "debug.h"

StringIntMap ImageParticle::imageParticleCountByName;

ImageParticle::ImageParticle(Image *restrict const image) :
    Particle()
{
    mType = ParticleType::Image;
    mImage = image;
    if (mImage)
    {
        mImage->incRef();

        const std::string &restrict name = mImage->getIdPath();
        StringIntMapIter it
            = ImageParticle::imageParticleCountByName.find(name);
        if (it == ImageParticle::imageParticleCountByName.end())
            ImageParticle::imageParticleCountByName[name] = 1;
        else
            (*it).second ++;
    }
}

void ImageParticle::draw(Graphics *restrict const graphics,
                         const int offsetX,
                         const int offsetY) const restrict2
{
    FUNC_BLOCK("ImageParticle::draw", 1)
    if (mAlive != AliveStatus::ALIVE || !mImage)
        return;

    const int w = mImage->mBounds.w;
    const int h = mImage->mBounds.h;
    const int screenX = CAST_S32(mPos.x)
        + offsetX - w / 2;
    const int screenY = CAST_S32(mPos.y) - CAST_S32(mPos.z)
        + offsetY - h / 2;

    // Check if on screen
    if (screenX + w < 0 ||
        screenX > graphics->mWidth ||
        screenY + h < 0 ||
        screenY > graphics->mHeight)
    {
        return;
    }

    float alphafactor = mAlpha;

    if (mFadeOut && mLifetimeLeft > -1 && mLifetimeLeft < mFadeOut)
    {
        alphafactor *= static_cast<float>(mLifetimeLeft)
            / static_cast<float>(mFadeOut);
    }

    if (mFadeIn && mLifetimePast < mFadeIn)
    {
        alphafactor *= static_cast<float>(mLifetimePast)
        / static_cast<float>(mFadeIn);
    }

    mImage->setAlpha(alphafactor);
    graphics->drawImage(mImage, screenX, screenY);
    return;
}
