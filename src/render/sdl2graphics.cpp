/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef USE_SDL2

#include "render/sdl2graphics.h"

#include "graphicsmanager.h"

#include "render/vertexes/imagecollection.h"

#include "resources/imagerect.h"
#include "resources/sdl2imagehelper.h"

#include "resources/image/image.h"

#include "utils/sdlcheckutils.h"

#include "debug.h"

#ifdef DEBUG_SDL_SURFACES

#define MSDL_RenderCopy(render, texture, src, dst) \
    FakeSDL_RenderCopy(render, texture, src, dst)

static int FakeSDL_RenderCopy(SDL_Renderer *restrict const renderer,
                              SDL_Texture *restrict const texture,
                              const SDL_Rect *restrict const srcrect,
                              const SDL_Rect *restrict const dstrect)
{
    int ret = SDL_RenderCopy(renderer, texture, srcrect, dstrect);
    if (ret)
    {
        logger->log("rendering error in texture %p: %s",
            static_cast<void*>(texture), SDL_GetError());
    }
    return ret;
}

#else  // DEBUG_SDL_SURFACES

#define MSDL_RenderCopy(render, texture, src, dst) \
    SDL_RenderCopy(render, texture, src, dst)

#endif  // DEBUG_SDL_SURFACES

#define setRenderDrawColor(mColor) \
    SDL_SetRenderDrawColor(mRenderer, \
        CAST_U8(mColor.r), \
        CAST_U8(mColor.g), \
        CAST_U8(mColor.b), \
        CAST_U8(mColor.a))

#define defRectFromArea(rect, area) \
    const SDL_Rect rect = \
    { \
        CAST_S32(area.x), \
        CAST_S32(area.y), \
        CAST_S32(area.width), \
        CAST_S32(area.height) \
    }

SDLGraphics::SDLGraphics() :
    Graphics(),
    mRendererFlags(SDL_RENDERER_SOFTWARE),
    mOldPixel(0),
    mOldAlpha(0)
{
    mOpenGL = RENDER_SDL2_DEFAULT;
    mName = "SDL2 default";
}

SDLGraphics::~SDLGraphics()
{
}

void SDLGraphics::drawRescaledImage(const Image *restrict const image,
                                    int dstX, int dstY,
                                    const int desiredWidth,
                                    const int desiredHeight) restrict2
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    // Check that preconditions for blitting are met.
    if (!mWindow || !image || !image->mTexture)
        return;

    const ClipRect &top = mClipStack.top();
    const SDL_Rect &bounds = image->mBounds;
    const SDL_Rect srcRect =
    {
        CAST_S32(bounds.x),
        CAST_S32(bounds.y),
        CAST_S32(bounds.w),
        CAST_S32(bounds.h)
    };
    const SDL_Rect dstRect =
    {
        CAST_S32(dstX + top.xOffset),
        CAST_S32(dstY + top.yOffset),
        CAST_S32(desiredWidth),
        CAST_S32(desiredHeight)
    };

    MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
}

void SDLGraphics::drawImage(const Image *restrict const image,
                            int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void SDLGraphics::drawImageInline(const Image *restrict const image,
                                  int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    // Check that preconditions for blitting are met.
    if (!mWindow || !image || !image->mTexture)
        return;

    const ClipRect &top = mClipStack.top();
    if (!top.width || !top.height)
        return;

    const SDL_Rect &bounds = image->mBounds;
    const SDL_Rect srcRect =
    {
        CAST_S32(bounds.x),
        CAST_S32(bounds.y),
        CAST_S32(bounds.w),
        CAST_S32(bounds.h)
    };

    const SDL_Rect dstRect =
    {
        CAST_S32(dstX + top.xOffset),
        CAST_S32(dstY + top.yOffset),
        CAST_S32(bounds.w),
        CAST_S32(bounds.h)
    };

    MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
}

void SDLGraphics::copyImage(const Image *restrict const image,
                            int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void SDLGraphics::drawImageCached(const Image *restrict const image,
                                  int x, int y) restrict2
{
    FUNC_BLOCK("Graphics::drawImageCached", 1)
    // Check that preconditions for blitting are met.
    if (!mWindow || !image || !image->mTexture)
        return;

    const ClipRect &top = mClipStack.top();
    if (!top.width || !top.height)
        return;

    const SDL_Rect &bounds = image->mBounds;
    const SDL_Rect srcRect =
    {
        CAST_S32(bounds.x),
        CAST_S32(bounds.y),
        CAST_S32(bounds.w),
        CAST_S32(bounds.h)
    };

    const SDL_Rect dstRect =
    {
        CAST_S32(x + top.xOffset),
        CAST_S32(y + top.yOffset),
        CAST_S32(bounds.w),
        CAST_S32(bounds.h)
    };

    MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
}

void SDLGraphics::drawPatternCached(const Image *restrict const image,
                                    const int x, const int y,
                                    const int w, const int h) restrict2
{
    FUNC_BLOCK("Graphics::drawPatternCached", 1)
    // Check that preconditions for blitting are met.
    if (!mWindow || !image)
        return;
    if (!image->mTexture)
        return;

    const ClipRect &top = mClipStack.top();
    if (!top.width || !top.height)
        return;

    const SDL_Rect &bounds = image->mBounds;
    const int iw = bounds.w;
    const int ih = bounds.h;
    if (iw == 0 || ih == 0)
        return;

    const int xOffset = top.xOffset + x;
    const int yOffset = top.yOffset + y;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    srcRect.x = CAST_S32(bounds.x);
    srcRect.y = CAST_S32(bounds.y);
    for (int py = 0; py < h; py += ih)
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        dstRect.y = CAST_S32(py + yOffset);
        srcRect.h = CAST_S32(dh);
        dstRect.h = CAST_S32(dh);

        for (int px = 0; px < w; px += iw)
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            dstRect.x = CAST_S32(px + xOffset);
            srcRect.w = CAST_S32(dw);
            dstRect.w = CAST_S32(dw);

            MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
        }
    }
}

void SDLGraphics::completeCache() restrict2
{
}

void SDLGraphics::drawPattern(const Image *restrict const image,
                              const int x, const int y,
                              const int w, const int h) restrict2
{
    drawPatternInline(image, x, y, w, h);
}

void SDLGraphics::drawPatternInline(const Image *restrict const image,
                                    const int x, const int y,
                                    const int w, const int h) restrict2
{
    FUNC_BLOCK("Graphics::drawPattern", 1)
    // Check that preconditions for blitting are met.
    if (!mWindow || !image)
        return;
    if (!image->mTexture)
        return;

    const ClipRect &top = mClipStack.top();
    if (!top.width || !top.height)
        return;

    const SDL_Rect &bounds = image->mBounds;
    const int iw = bounds.w;
    const int ih = bounds.h;
    if (iw == 0 || ih == 0)
        return;

    const int xOffset = top.xOffset + x;
    const int yOffset = top.yOffset + y;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    srcRect.x = CAST_S32(bounds.x);
    srcRect.y = CAST_S32(bounds.y);
    for (int py = 0; py < h; py += ih)
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        dstRect.y = CAST_S32(py + yOffset);
        srcRect.h = CAST_S32(dh);
        dstRect.h = CAST_S32(dh);

        for (int px = 0; px < w; px += iw)
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            dstRect.x = CAST_S32(px + xOffset);
            srcRect.w = CAST_S32(dw);
            dstRect.w = CAST_S32(dw);

            MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
        }
    }
}

void SDLGraphics::drawRescaledPattern(const Image *restrict const image,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const int scaledWidth,
                                      const int scaledHeight) restrict2
{
    // Check that preconditions for blitting are met.
    if (!mWindow || !image)
        return;
    if (!image->mTexture)
        return;

    if (scaledHeight == 0 || scaledWidth == 0)
        return;

    const ClipRect &top = mClipStack.top();
    if (!top.width || !top.height)
        return;

    Image *const tmpImage = image->SDLgetScaledImage(
        scaledWidth, scaledHeight);
    if (!tmpImage)
        return;

    const SDL_Rect &bounds = tmpImage->mBounds;
    const int iw = bounds.w;
    const int ih = bounds.h;
    if (iw == 0 || ih == 0)
        return;

    const int xOffset = top.xOffset + x;
    const int yOffset = top.yOffset + y;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    srcRect.x = CAST_S32(bounds.x);
    srcRect.y = CAST_S32(bounds.y);
    for (int py = 0; py < h; py += ih)
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        dstRect.y = CAST_S32(py + yOffset);
        srcRect.h = CAST_S32(dh);
        dstRect.h = CAST_S32(dh);

        for (int px = 0; px < w; px += iw)
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            dstRect.x = CAST_S32(px + xOffset);
            srcRect.w = CAST_S32(dw);
            dstRect.w = CAST_S32(dw);

            MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
        }
    }

    delete tmpImage;
}

void SDLGraphics::calcPattern(ImageVertexes *restrict const vert,
                              const Image *restrict const image,
                              const int x, const int y,
                              const int w, const int h) const restrict2
{
    calcPatternInline(vert, image, x, y, w, h);
}

void SDLGraphics::calcPatternInline(ImageVertexes *restrict const vert,
                                    const Image *restrict const image,
                                    const int x, const int y,
                                    const int w, const int h) const restrict2
{
    // Check that preconditions for blitting are met.
    if (!vert || !mWindow || !image || !image->mTexture)
        return;

    const ClipRect &top = mClipStack.top();
    if (!top.width || !top.height)
        return;

    const SDL_Rect &bounds = image->mBounds;
    const int iw = bounds.w;
    const int ih = bounds.h;
    if (iw == 0 || ih == 0)
        return;

    const int xOffset = top.xOffset + x;
    const int yOffset = top.yOffset + y;
    const int srcX = bounds.x;
    const int srcY = bounds.y;
    for (int py = 0; py < h; py += ih)
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int dstY = py + yOffset;

        for (int px = 0; px < w; px += iw)
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int dstX = px + xOffset;

            DoubleRect *const r = new DoubleRect;
            SDL_Rect &dstRect = r->dst;
            SDL_Rect &srcRect = r->src;
            srcRect.x = CAST_S32(srcX);
            srcRect.y = CAST_S32(srcY);
            srcRect.w = CAST_S32(dw);
            srcRect.h = CAST_S32(dh);
            dstRect.x = CAST_S32(dstX);
            dstRect.y = CAST_S32(dstY);
            dstRect.w = CAST_S32(dw);
            dstRect.h = CAST_S32(dh);

            vert->sdl.push_back(r);
        }
    }
}

void SDLGraphics::calcPattern(ImageCollection *restrict const vertCol,
                              const Image *restrict const image,
                              const int x, const int y,
                              const int w, const int h) const restrict2
{
    ImageVertexes *vert = nullptr;
    if (vertCol->currentImage != image)
    {
        vert = new ImageVertexes;
        vertCol->currentImage = image;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
    }
    else
    {
        vert = vertCol->currentVert;
    }

    calcPatternInline(vert, image, x, y, w, h);
}

void SDLGraphics::calcTileVertexes(ImageVertexes *restrict const vert,
                                   const Image *restrict const image,
                                   int x, int y) const restrict2
{
    vert->image = image;
    calcTileSDL(vert, x, y);
}

void SDLGraphics::calcTileVertexesInline(ImageVertexes *restrict const vert,
                                         const Image *restrict const image,
                                         int x, int y) const restrict2
{
    vert->image = image;
    calcTileSDL(vert, x, y);
}

void SDLGraphics::calcTileSDL(ImageVertexes *restrict const vert,
                              int x, int y) const restrict2
{
    // Check that preconditions for blitting are met.
    if (!vert || !vert->image || !vert->image->mTexture)
        return;

    const ClipRect &top = mClipStack.top();
    if (!top.width || !top.height)
        return;

    const Image *const image = vert->image;
    const SDL_Rect &bounds = image->mBounds;

    x += top.xOffset;
    y += top.yOffset;

    DoubleRect *rect = new DoubleRect;
    SDL_Rect &dstRect = rect->dst;
    SDL_Rect &srcRect = rect->src;

    srcRect.x = CAST_S32(bounds.x);
    srcRect.y = CAST_S32(bounds.y);
    srcRect.w = CAST_S32(bounds.w);
    srcRect.h = CAST_S32(bounds.h);
    dstRect.x = CAST_S32(x);
    dstRect.y = CAST_S32(y);
    dstRect.w = CAST_S32(bounds.w);
    dstRect.h = CAST_S32(bounds.h);

    vert->sdl.push_back(rect);
}

void SDLGraphics::calcTileCollection(ImageCollection *restrict const vertCol,
                                     const Image *restrict const image,
                                     int x, int y) restrict2
{
    if (!vertCol)
        return;
    if (vertCol->currentImage != image)
    {
        ImageVertexes *const vert = new ImageVertexes;
        vertCol->currentImage = image;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
        calcTileSDL(vert, x, y);
    }
    else
    {
        calcTileSDL(vertCol->currentVert, x, y);
    }
}

void SDLGraphics::drawTileCollection(const ImageCollection
                                     *restrict const vertCol) restrict2
{
    const ImageVertexesVector &draws = vertCol->draws;
    const ImageCollectionCIter it_end = draws.end();
    for (ImageCollectionCIter it = draws.begin(); it != it_end; ++ it)
    {
        const ImageVertexes *const vert = *it;
        const Image *const img = vert->image;
        const DoubleRects *const rects = &vert->sdl;
        DoubleRects::const_iterator it2 = rects->begin();
        const DoubleRects::const_iterator it2_end = rects->end();
        while (it2 != it2_end)
        {
            MSDL_RenderCopy(mRenderer, img->mTexture,
                &(*it2)->src, &(*it2)->dst);
            ++ it2;
        }
    }
}

void SDLGraphics::drawTileVertexes(const ImageVertexes *restrict const vert)
                                   restrict2
{
    if (!vert)
        return;
    // vert and img must be != 0
    const Image *const img = vert->image;
    const DoubleRects *const rects = &vert->sdl;
    DoubleRects::const_iterator it = rects->begin();
    const DoubleRects::const_iterator it_end = rects->end();
    while (it != it_end)
    {
        MSDL_RenderCopy(mRenderer, img->mTexture, &(*it)->src, &(*it)->dst);
        ++ it;
    }
}

void SDLGraphics::updateScreen() restrict2
{
    BLOCK_START("Graphics::updateScreen")
    SDL_RenderPresent(mRenderer);
//    SDL_RenderClear(mRenderer);
    BLOCK_END("Graphics::updateScreen")
}

void SDLGraphics::calcWindow(ImageCollection *restrict const vertCol,
                             const int x, const int y,
                             const int w, const int h,
                             const ImageRect &restrict imgRect) restrict2
{
    ImageVertexes *vert = nullptr;
    Image *const image = imgRect.grid[4];
    if (!image)
        return;
    if (vertCol->currentImage != image)
    {
        vert = new ImageVertexes;
        vertCol->currentImage = image;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
    }
    else
    {
        vert = vertCol->currentVert;
    }
    calcImageRect(vert, x, y, w, h, imgRect);
}

void SDLGraphics::fillRectangle(const Rect &restrict rectangle) restrict2
{
    const ClipRect &top = mClipStack.top();
    const SDL_Rect rect =
    {
        CAST_S32(rectangle.x + top.xOffset),
        CAST_S32(rectangle.y + top.yOffset),
        CAST_S32(rectangle.width),
        CAST_S32(rectangle.height)
    };

    setRenderDrawColor(mColor);
    SDL_RenderFillRects(mRenderer, &rect, 1);
}

void SDLGraphics::beginDraw() restrict2
{
    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void SDLGraphics::endDraw() restrict2
{
    popClipArea();
}

void SDLGraphics::pushClipArea(const Rect &restrict area) restrict2
{
    Graphics::pushClipArea(area);

    const ClipRect &carea = mClipStack.top();
    defRectFromArea(rect, carea);
    SDL_RenderSetClipRect(mRenderer, &rect);
}

void SDLGraphics::popClipArea() restrict2
{
    Graphics::popClipArea();

    if (mClipStack.empty())
        return;

    const ClipRect &carea = mClipStack.top();
    defRectFromArea(rect, carea);
    SDL_RenderSetClipRect(mRenderer, &rect);
}

void SDLGraphics::drawPoint(int x, int y) restrict2
{
    if (mClipStack.empty())
        return;

    const ClipRect &top = mClipStack.top();

    x += top.xOffset;
    y += top.yOffset;

    if (!top.isPointInRect(x, y))
        return;

    setRenderDrawColor(mColor);
    const SDL_Point point =
    {
        x,
        y
    };

    SDL_RenderDrawPoints(mRenderer, &point, 1);
}


void SDLGraphics::drawRectangle(const Rect &restrict rectangle) restrict2
{
    const ClipRect &top = mClipStack.top();
    setRenderDrawColor(mColor);

    const int x1 = rectangle.x + top.xOffset;
    const int y1 = rectangle.y + top.yOffset;
    const int x2 = x1 + rectangle.width - 1;
    const int y2 = y1 + rectangle.height - 1;
    SDL_Point points[] =
    {
        {x1, y1},
        {x2, y1},
        {x2, y2},
        {x1, y2},
        {x1, y1}
    };

    SDL_RenderDrawLines(mRenderer, points, 5);
}

void SDLGraphics::drawLine(int x1, int y1,
                           int x2, int y2) restrict2
{
    const ClipRect &top = mClipStack.top();
    setRenderDrawColor(mColor);

    const int x0 = top.xOffset;
    const int y0 = top.yOffset;

    SDL_Point points[] =
    {
        {x1 + x0, y1 + y0},
        {x2 + x0, y2 + y0}
    };

    SDL_RenderDrawLines(mRenderer, points, 2);
}

bool SDLGraphics::setVideoMode(const int w, const int h,
                               const int scale,
                               const int bpp,
                               const bool fs,
                               const bool hwaccel,
                               const bool resize,
                               const bool noFrame,
                               const bool allowHighDPI) restrict2
{
    setMainFlags(w, h,
        scale,
        bpp,
        fs,
        hwaccel,
        resize,
        noFrame,
        allowHighDPI);

    if (!(mWindow = graphicsManager.createWindow(w, h, bpp,
        getSoftwareFlags())))
    {
        mRect.w = 0;
        mRect.h = 0;
        return false;
    }

    int w1 = 0;
    int h1 = 0;
    SDL_GetWindowSize(mWindow, &w1, &h1);
    mRect.w = w1;
    mRect.h = h1;

    mRenderer = graphicsManager.createRenderer(mWindow, mRendererFlags);
    SDLImageHelper::setRenderer(mRenderer);
    return videoInfo();
}

void SDLGraphics::drawImageRect(const int x, const int y,
                                const int w, const int h,
                                const ImageRect &restrict imgRect) restrict2
{
    #include "render/graphics_drawImageRect.hpp"
}

void SDLGraphics::calcImageRect(ImageVertexes *restrict const vert,
                                const int x, const int y,
                                const int w, const int h,
                                const ImageRect &restrict imgRect) restrict2
{
    #include "render/graphics_calcImageRect.hpp"
}

#endif  // USE_SDL2
