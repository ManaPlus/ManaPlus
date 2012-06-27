/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "resources/subimage.h"

#ifdef USE_OPENGL
#include "openglgraphics.h"
#include "opengl1graphics.h"
#endif

#include "client.h"
#include "main.h"

#include "utils/stringutils.h"

#include "debug.h"

SubImage::SubImage(Image *parent, SDL_Surface *image,
                   int x, int y, int width, int height):
    Image(image),
    mParent(parent)
{
    if (mParent)
    {
        mParent->incRef();
        mParent->SDLTerminateAlphaCache();
        mHasAlphaChannel = mParent->hasAlphaChannel();
        mIsAlphaVisible = mHasAlphaChannel;
        mAlphaChannel = mParent->SDLgetAlphaChannel();
#ifdef DEBUG_BIND_TEXTURE
        mIdPath = parent->getIdPath();
#endif
    }
    else
    {
        mHasAlphaChannel = false;
        mIsAlphaVisible = false;
        mAlphaChannel = nullptr;
    }

    // Set up the rectangle.
    mBounds.x = static_cast<short>(x);
    mBounds.y = static_cast<short>(y);
    mBounds.w = static_cast<uint16_t>(width);
    mBounds.h = static_cast<uint16_t>(height);
    if (mParent)
    {
        mInternalBounds.x = mParent->mBounds.x;
        mInternalBounds.y = mParent->mBounds.y;
        mInternalBounds.w = mParent->mBounds.w;
        mInternalBounds.h = mParent->mBounds.h;
    }
    else
    {
        mInternalBounds.x = 0;
        mInternalBounds.y = 0;
        mInternalBounds.w = 1;
        mInternalBounds.h = 1;
    }
    mUseAlphaCache = false;
}

#ifdef USE_OPENGL
SubImage::SubImage(Image *parent, GLuint image,
                   int x, int y, int width, int height,
                   int texWidth, int texHeight):
    Image(image, width, height, texWidth, texHeight),
    mParent(parent)
{
    if (mParent)
        mParent->incRef();

    // Set up the rectangle.
    mBounds.x = static_cast<short>(x);
    mBounds.y = static_cast<short>(y);
    mBounds.w = static_cast<uint16_t>(width);
    mBounds.h = static_cast<uint16_t>(height);
    if (mParent)
    {
        mInternalBounds.x = mParent->mBounds.x;
        mInternalBounds.y = mParent->mBounds.y;
        mInternalBounds.w = mParent->mBounds.w;
        mInternalBounds.h = mParent->mBounds.h;
#ifdef DEBUG_BIND_TEXTURE
        mIdPath = parent->getIdPath();
#endif
    }
    else
    {
        mInternalBounds.x = 0;
        mInternalBounds.y = 0;
        mInternalBounds.w = 1;
        mInternalBounds.h = 1;
    }
    mIsAlphaVisible = mHasAlphaChannel;
}
#endif

SubImage::~SubImage()
{
    // Avoid destruction of the image
    mSDLSurface = nullptr;
    // Avoid possible destruction of its alpha channel
    mAlphaChannel = nullptr;
#ifdef USE_OPENGL
    mGLImage = 0;
#endif
    if (mParent)
    {
        mParent->decRef();
        mParent = nullptr;
    }
}

Image *SubImage::getSubImage(int x, int y, int w, int h)
{
    if (mParent)
        return mParent->getSubImage(mBounds.x + x, mBounds.y + y, w, h);
    else
        return nullptr;
}
