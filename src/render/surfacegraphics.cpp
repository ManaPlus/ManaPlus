/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "render/surfacegraphics.h"

#include "main.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "resources/imagehelper.h"
#include "resources/surfaceimagehelper.h"

#include <guichan/sdl/sdlpixel.hpp>

#include "debug.h"

SurfaceGraphics::SurfaceGraphics() :
    Graphics(),
    mBlitMode(BLIT_NORMAL),
    mTarget(nullptr)
{
}

SurfaceGraphics::~SurfaceGraphics()
{
}

bool SurfaceGraphics::drawImage2(const Image *const image, int srcX, int srcY,
                                 int dstX, int dstY,
                                 const int width, const int height,
                                 const bool useColor A_UNUSED)
{
    FUNC_BLOCK("Graphics::drawImage2", 1)
    // Check that preconditions for blitting are met.
    if (!mTarget || !image || !image->mSDLSurface)
        return false;

    srcX += image->mBounds.x;
    srcY += image->mBounds.y;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    dstRect.x = static_cast<int16_t>(dstX);
    dstRect.y = static_cast<int16_t>(dstY);
    srcRect.x = static_cast<int16_t>(srcX);
    srcRect.y = static_cast<int16_t>(srcY);
    srcRect.w = static_cast<uint16_t>(width);
    srcRect.h = static_cast<uint16_t>(height);

#ifdef USE_SDL2
    return !(SDL_BlitSurface(image->mSDLSurface, &srcRect,
                             mTarget, &dstRect) < 0);
#else
    if (mBlitMode == BLIT_NORMAL)
    {
        return !(SDL_BlitSurface(image->mSDLSurface, &srcRect,
                                 mTarget, &dstRect) < 0);
    }
    else
    {
        return !(SurfaceImageHelper::combineSurface(
            image->mSDLSurface, &srcRect, mTarget, &dstRect) < 0);
    }
#endif
}
