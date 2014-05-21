/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "render/surfacegraphics.h"

#include "main.h"

#include "resources/image.h"
#include "resources/surfaceimagehelper.h"

#include "debug.h"

SurfaceGraphics::SurfaceGraphics() :
    Graphics(),
    mBlitMode(BLIT_NORMAL),
    mTarget(nullptr)
{
    mOpenGL = RENDER_SOFTWARE;
    mName = "Surface";
}

SurfaceGraphics::~SurfaceGraphics()
{
}

bool SurfaceGraphics::drawImage(const Image *const image,
                                int dstX, int dstY)
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    // Check that preconditions for blitting are met.
    if (!mTarget || !image || !image->mSDLSurface)
        return false;

    const SDL_Rect &imageRect = image->mBounds;
    SDL_Rect dstRect;
    SDL_Rect srcRect;
    dstRect.x = static_cast<int16_t>(dstX);
    dstRect.y = static_cast<int16_t>(dstY);
    srcRect.x = static_cast<int16_t>(imageRect.x);
    srcRect.y = static_cast<int16_t>(imageRect.y);
    srcRect.w = static_cast<uint16_t>(imageRect.w);
    srcRect.h = static_cast<uint16_t>(imageRect.h);

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

void SurfaceGraphics::drawImageCached(const Image *const image,
                                      int x, int y)
{
    FUNC_BLOCK("Graphics::drawImageCached", 1)
    // Check that preconditions for blitting are met.
    if (!mTarget || !image || !image->mSDLSurface)
        return;

    const SDL_Rect &rect = image->mBounds;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    dstRect.x = static_cast<int16_t>(x);
    dstRect.y = static_cast<int16_t>(y);
    srcRect.x = static_cast<int16_t>(rect.x);
    srcRect.y = static_cast<int16_t>(rect.y);
    srcRect.w = static_cast<uint16_t>(rect.w);
    srcRect.h = static_cast<uint16_t>(rect.h);

#ifdef USE_SDL2
    SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
#else
    if (mBlitMode == BLIT_NORMAL)
    {
        SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
    }
    else
    {
        SurfaceImageHelper::combineSurface(image->mSDLSurface, &srcRect,
            mTarget, &dstRect);
    }
#endif
}

void SurfaceGraphics::completeCache()
{
}
