/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "imagesprite.h"

#include "graphics.h"

ImageSprite::ImageSprite(Image *image):
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
        mImage->decRef();
}

bool ImageSprite::draw(Graphics* graphics, int posX, int posY) const
{
    if (!mImage)
        return false;

    if (mImage->getAlpha() != mAlpha)
        mImage->setAlpha(mAlpha);

    return graphics->drawImage(mImage, posX, posY);
}
