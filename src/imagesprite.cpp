/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "imagesprite.h"

#include "render/graphics.h"

#include "debug.h"

ImageSprite::ImageSprite(Image *const image):
        mImage(image)
{
    if (mImage)
    {
        mAlpha = mImage->getAlpha();
        mImage->incRef();
    }
    else
    {
        mAlpha = 1;
    }
}

ImageSprite::~ImageSprite()
{
    if (mImage)
    {
        mImage->decRef();
        mImage = nullptr;
    }
}

void ImageSprite::draw(Graphics *const graphics,
                       const int posX, const int posY) const
{
    FUNC_BLOCK("ImageSprite::draw", 1)
    if (!mImage)
        return;

    mImage->setAlpha(mAlpha);
    DRAW_IMAGE(graphics, mImage, posX, posY);
}
