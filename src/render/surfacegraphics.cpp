/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "render/surfacegraphics.h"

#ifndef USE_SDL2
#include "resources/surfaceimagehelper.h"
#endif  // USE_SDL2

#include "resources/image/image.h"

#include "debug.h"

SurfaceGraphics::SurfaceGraphics() :
    Graphics(),
    mBlitMode(BlitMode::BLIT_NORMAL),
    mTarget(nullptr)
{
    mOpenGL = RENDER_SOFTWARE;
    mName = "Surface";
}

SurfaceGraphics::~SurfaceGraphics()
{
}

void SurfaceGraphics::drawImage(const Image *restrict const image,
                                int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    // Check that preconditions for blitting are met.
    if (mTarget == nullptr ||
        image == nullptr ||
        image->mSDLSurface == nullptr)
    {
        return;
    }

    const SDL_Rect &imageRect = image->mBounds;
    SDL_Rect dstRect;
    SDL_Rect srcRect;
    dstRect.x = CAST_S16(dstX);
    dstRect.y = CAST_S16(dstY);
    srcRect.x = CAST_S16(imageRect.x);
    srcRect.y = CAST_S16(imageRect.y);
    srcRect.w = CAST_U16(imageRect.w);
    srcRect.h = CAST_U16(imageRect.h);

#ifdef USE_SDL2
    SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
#else  // USE_SDL2

    if (mBlitMode == BlitMode::BLIT_NORMAL)
    {
        SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
    }
    else
    {
        SurfaceImageHelper::combineSurface(image->mSDLSurface,
            &srcRect, mTarget, &dstRect);
    }
#endif  // USE_SDL2
}

void SurfaceGraphics::copyImage(const Image *restrict const image,
                                int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    // Check that preconditions for blitting are met.
    if (mTarget == nullptr ||
        image == nullptr ||
        image->mSDLSurface == nullptr)
    {
        return;
    }

    const SDL_Rect &imageRect = image->mBounds;
    SDL_Rect dstRect;
    SDL_Rect srcRect;
    dstRect.x = CAST_S16(dstX);
    dstRect.y = CAST_S16(dstY);
    srcRect.x = CAST_S16(imageRect.x);
    srcRect.y = CAST_S16(imageRect.y);
    srcRect.w = CAST_U16(imageRect.w);
    srcRect.h = CAST_U16(imageRect.h);

#ifdef USE_SDL2
    // probably need change some flags
    SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
#else  // USE_SDL2

    SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
#endif  // USE_SDL2
}

void SurfaceGraphics::drawImageCached(const Image *restrict const image,
                                      int x, int y) restrict2
{
    FUNC_BLOCK("Graphics::drawImageCached", 1)
    // Check that preconditions for blitting are met.
    if (mTarget == nullptr ||
        image == nullptr ||
        image->mSDLSurface == nullptr)
    {
        return;
    }

    const SDL_Rect &rect = image->mBounds;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    dstRect.x = CAST_S16(x);
    dstRect.y = CAST_S16(y);
    srcRect.x = CAST_S16(rect.x);
    srcRect.y = CAST_S16(rect.y);
    srcRect.w = CAST_U16(rect.w);
    srcRect.h = CAST_U16(rect.h);

#ifdef USE_SDL2
    SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
#else  // USE_SDL2

    if (mBlitMode == BlitMode::BLIT_NORMAL)
    {
        SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
    }
    else
    {
        SurfaceImageHelper::combineSurface(image->mSDLSurface, &srcRect,
            mTarget, &dstRect);
    }
#endif  // USE_SDL2
}

void SurfaceGraphics::completeCache() restrict2
{
}
