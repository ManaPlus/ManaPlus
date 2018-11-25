/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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
    mCloseImage(images != nullptr ? images->grid[0] : nullptr),
    mCloseImageHighlighted(images != nullptr ? images->grid[1] : nullptr),
    mStickyImageUp(images != nullptr ? images->grid[2] : nullptr),
    mStickyImageDown(images != nullptr ? images->grid[3] : nullptr),
    mPadding(padding),
    mTitlePadding(titlePadding),
    mOptions(options)
{
    if (mCloseImageHighlighted == nullptr)
    {
        mCloseImageHighlighted = mCloseImage;
        if (mCloseImageHighlighted != nullptr)
            mCloseImageHighlighted->incRef();
    }
}

Skin::~Skin()
{
    for (int i = 0; i < 9; i++)
    {
        if ((mBorder != nullptr) && (mBorder->grid[i] != nullptr))
        {
            mBorder->grid[i]->decRef();
            mBorder->grid[i] = nullptr;
        }
    }

    if (mCloseImage != nullptr)
    {
        mCloseImage->decRef();
        mCloseImage = nullptr;
    }

    if (mCloseImageHighlighted != nullptr)
    {
        mCloseImageHighlighted->decRef();
        mCloseImageHighlighted = nullptr;
    }

    if (mStickyImageUp != nullptr)
    {
        mStickyImageUp->decRef();
        mStickyImageUp = nullptr;
    }

    if (mStickyImageDown != nullptr)
    {
        mStickyImageDown->decRef();
        mStickyImageDown = nullptr;
    }

    delete2(mOptions)
    delete2(mBorder)
}

void Skin::updateAlpha(const float minimumOpacityAllowed)
{
    const float alpha = static_cast<float>(
        std::max(static_cast<double>(minimumOpacityAllowed),
        static_cast<double>(settings.guiAlpha)));

    if (mBorder != nullptr)
    {
        for (int i = 0; i < 9; i++)
        {
            if (mBorder->grid[i] != nullptr)
                mBorder->grid[i]->setAlpha(alpha);
        }
    }

    if (mCloseImage != nullptr)
        mCloseImage->setAlpha(alpha);
    if (mCloseImageHighlighted != nullptr)
        mCloseImageHighlighted->setAlpha(alpha);
    if (mStickyImageUp != nullptr)
        mStickyImageUp->setAlpha(alpha);
    if (mStickyImageDown != nullptr)
        mStickyImageDown->setAlpha(alpha);
}

int Skin::getMinWidth() const
{
    if ((mBorder == nullptr) ||
        (mBorder->grid[ImagePosition::UPPER_LEFT] == nullptr) ||
        (mBorder->grid[ImagePosition::UPPER_RIGHT] == nullptr))
    {
        return 1;
    }

    return mBorder->grid[ImagePosition::UPPER_LEFT]->getWidth() +
           mBorder->grid[ImagePosition::UPPER_RIGHT]->getWidth();
}

int Skin::getMinHeight() const
{
    if ((mBorder == nullptr) ||
        (mBorder->grid[ImagePosition::UPPER_LEFT] == nullptr) ||
        (mBorder->grid[ImagePosition::LOWER_LEFT] == nullptr))
    {
        return 1;
    }

    return mBorder->grid[ImagePosition::UPPER_LEFT]->getHeight() +
           mBorder->grid[ImagePosition::LOWER_LEFT]->getHeight();
}
