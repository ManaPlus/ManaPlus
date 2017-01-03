/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/skin.h"

#include "settings.h"

#include "enums/resources/imageposition.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "utils/delete2.h"

#include "debug.h"

Skin::Skin(ImageRect *const restrict skin,
           const ImageRect *const restrict images,
           const std::string &filePath, const std::string &name,
           const int padding, const int titlePadding,
           StringIntMap *restrict const options) :
    instances(1),
    mFilePath(filePath),
    mName(name),
    mBorder(skin),
    mCloseImage(images ? images->grid[0] : nullptr),
    mCloseImageHighlighted(images ? images->grid[1] : nullptr),
    mStickyImageUp(images ? images->grid[2] : nullptr),
    mStickyImageDown(images ? images->grid[3] : nullptr),
    mPadding(padding),
    mTitlePadding(titlePadding),
    mOptions(options)
{
    if (!mCloseImageHighlighted)
    {
        mCloseImageHighlighted = mCloseImage;
        if (mCloseImageHighlighted)
            mCloseImageHighlighted->incRef();
    }
}

Skin::~Skin()
{
    for (int i = 0; i < 9; i++)
    {
        if (mBorder && mBorder->grid[i])
        {
            mBorder->grid[i]->decRef();
            mBorder->grid[i] = nullptr;
        }
    }

    if (mCloseImage)
    {
        mCloseImage->decRef();
        mCloseImage = nullptr;
    }

    if (mCloseImageHighlighted)
    {
        mCloseImageHighlighted->decRef();
        mCloseImageHighlighted = nullptr;
    }

    if (mStickyImageUp)
    {
        mStickyImageUp->decRef();
        mStickyImageUp = nullptr;
    }

    if (mStickyImageDown)
    {
        mStickyImageDown->decRef();
        mStickyImageDown = nullptr;
    }

    delete2(mOptions);
    delete2(mBorder);
}

void Skin::updateAlpha(const float minimumOpacityAllowed)
{
    const float alpha = static_cast<float>(
        std::max(static_cast<double>(minimumOpacityAllowed),
        static_cast<double>(settings.guiAlpha)));

    if (mBorder)
    {
        for (int i = 0; i < 9; i++)
        {
            if (mBorder->grid[i])
                mBorder->grid[i]->setAlpha(alpha);
        }
    }

    if (mCloseImage)
        mCloseImage->setAlpha(alpha);
    if (mCloseImageHighlighted)
        mCloseImageHighlighted->setAlpha(alpha);
    if (mStickyImageUp)
        mStickyImageUp->setAlpha(alpha);
    if (mStickyImageDown)
        mStickyImageDown->setAlpha(alpha);
}

int Skin::getMinWidth() const
{
    if (!mBorder ||
        !mBorder->grid[ImagePosition::UPPER_LEFT] ||
        !mBorder->grid[ImagePosition::UPPER_RIGHT])
    {
        return 1;
    }

    return mBorder->grid[ImagePosition::UPPER_LEFT]->getWidth() +
           mBorder->grid[ImagePosition::UPPER_RIGHT]->getWidth();
}

int Skin::getMinHeight() const
{
    if (!mBorder ||
        !mBorder->grid[ImagePosition::UPPER_LEFT] ||
        !mBorder->grid[ImagePosition::LOWER_LEFT])
    {
        return 1;
    }

    return mBorder->grid[ImagePosition::UPPER_LEFT]->getHeight() +
           mBorder->grid[ImagePosition::LOWER_LEFT]->getHeight();
}
