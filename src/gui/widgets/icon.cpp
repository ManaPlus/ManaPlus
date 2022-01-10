/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/widgets/icon.h"

#include "gui/gui.h"

#include "render/graphics.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "debug.h"

Icon::Icon(const Widget2 *const widget,
           const std::string &file,
           const AutoRelease autoRelease) :
    Widget(widget),
    mImage(Loader::getImage(file)),
    mAutoRelease(autoRelease)
{
    if (mImage != nullptr)
    {
        const SDL_Rect &bounds = mImage->mBounds;
        setSize(bounds.w, bounds.h);
    }
    mAllowLogic = false;
}

Icon::Icon(const Widget2 *const widget,
           Image *const image,
           const AutoRelease autoRelease) :
    Widget(widget),
    mImage(image),
    mAutoRelease(autoRelease)
{
    if (mImage != nullptr)
    {
        const SDL_Rect &bounds = mImage->mBounds;
        setSize(bounds.w, bounds.h);
    }
    mAllowLogic = false;
}

Icon::~Icon()
{
    if (gui != nullptr)
        gui->removeDragged(this);
    if ((mImage != nullptr) && mAutoRelease == AutoRelease_true)
        mImage->decRef();
}

void Icon::setImage(Image *const image)
{
    mImage = image;
    if (mImage != nullptr)
    {
        const SDL_Rect &bounds = mImage->mBounds;
        setSize(bounds.w, bounds.h);
    }
}

void Icon::draw(Graphics *const graphics)
{
    BLOCK_START("Icon::draw")
    if (mImage != nullptr)
    {
        graphics->drawImage(mImage,
            (mDimension.width - mImage->mBounds.w) / 2,
            (mDimension.height - mImage->mBounds.h) / 2);
    }
    BLOCK_END("Icon::draw")
}

void Icon::safeDraw(Graphics *const graphics)
{
    BLOCK_START("Icon::draw")
    if (mImage != nullptr)
    {
        graphics->drawImage(mImage,
            (mDimension.width - mImage->mBounds.w) / 2,
            (mDimension.height - mImage->mBounds.h) / 2);
    }
    BLOCK_END("Icon::draw")
}
