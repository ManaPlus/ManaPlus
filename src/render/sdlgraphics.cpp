/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef USE_SDL2

#include "render/sdlgraphics.h"

#include "graphicsmanager.h"

#include "utils/sdlcheckutils.h"

#include "utils/sdlpixel.h"

#include "render/vertexes/imagecollection.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
static unsigned int *cR = nullptr;
static unsigned int *cG = nullptr;
static unsigned int *cB = nullptr;
#endif  // SDL_BYTEORDER == SDL_LIL_ENDIAN

SDLGraphics::SDLGraphics() :
    Graphics(),
    mOldPixel(0),
    mOldAlpha(0)
{
    mOpenGL = RENDER_SOFTWARE;
    mName = "Software";
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
    if (mWindow == nullptr ||
        image == nullptr ||
        image->mSDLSurface == nullptr)
    {
        return;
    }

    Image *const tmpImage = image->SDLgetScaledImage(
        desiredWidth, desiredHeight);

    if ((tmpImage == nullptr) || (tmpImage->mSDLSurface == nullptr))
        return;

    const ClipRect &top = mClipStack.top();
    const SDL_Rect &bounds = image->mBounds;

    SDL_Rect srcRect =
    {
        CAST_S16(bounds.x),
        CAST_S16(bounds.y),
        CAST_U16(bounds.w),
        CAST_U16(bounds.h)
    };

    SDL_Rect dstRect =
    {
        CAST_S16(dstX + top.xOffset),
        CAST_S16(dstY + top.yOffset),
        0,
        0
    };

    SDL_BlitSurface(tmpImage->mSDLSurface, &srcRect, mWindow, &dstRect);
    delete tmpImage;
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
    if (mWindow == nullptr ||
        image == nullptr ||
        image->mSDLSurface == nullptr)
    {
        return;
    }

    const ClipRect &top = mClipStack.top();
    const SDL_Rect &bounds = image->mBounds;

    SDL_Surface *const src = image->mSDLSurface;

    int srcX = bounds.x;
    int srcY = bounds.y;
    dstX += top.xOffset;
    dstY += top.yOffset;

    int w = bounds.w;
    int h = bounds.h;
    if (srcX < 0)
    {
        w += srcX;
        dstX -= CAST_S16(srcX);
        srcX = 0;
    }
    const int maxw = src->w - srcX;
    if (maxw < w)
        w = maxw;

    if (srcY < 0)
    {
        h += srcY;
        dstY -= CAST_S16(srcY);
        srcY = 0;
    }
    const int maxh = src->h - srcY;
    if (maxh < h)
        h = maxh;

    const SDL_Rect *const clip = &mWindow->clip_rect;
    const int clipX = clip->x;
    const int clipY = clip->y;
    int dx = clipX - dstX;
    if (dx > 0)
    {
        w -= dx;
        dstX += CAST_S16(dx);
        srcX += dx;
    }
    dx = dstX + w - clipX - clip->w;
    if (dx > 0)
        w -= dx;

    int dy = clipY - dstY;
    if (dy > 0)
    {
        h -= dy;
        dstY += CAST_S16(dy);
        srcY += dy;
    }
    dy = dstY + h - clipY - clip->h;
    if (dy > 0)
        h -= dy;

    if (w > 0 && h > 0)
    {
        SDL_Rect srcRect =
        {
            CAST_S16(srcX),
            CAST_S16(srcY),
            CAST_U16(w),
            CAST_U16(h)
        };

        SDL_Rect dstRect =
        {
            CAST_S16(dstX),
            CAST_S16(dstY),
            CAST_U16(w),
            CAST_U16(h)
        };

        SDL_LowerBlit(src, &srcRect, mWindow, &dstRect);
    }
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
    if (mWindow == nullptr ||
        image == nullptr ||
        image->mSDLSurface == nullptr)
    {
        return;
    }

    const ClipRect &top = mClipStack.top();
    const SDL_Rect &bounds = image->mBounds;

    SDL_Surface *const src = image->mSDLSurface;

    int srcX = bounds.x;
    int srcY = bounds.y;
    x += top.xOffset;
    y += top.yOffset;

    int w = bounds.w;
    int h = bounds.h;
    if (srcX < 0)
    {
        w += srcX;
        x -= CAST_S16(srcX);
        srcX = 0;
    }
    const int maxw = src->w - srcX;
    if (maxw < w)
        w = maxw;

    if (srcY < 0)
    {
        h += srcY;
        y -= CAST_S16(srcY);
        srcY = 0;
    }
    const int maxh = src->h - srcY;
    if (maxh < h)
        h = maxh;

    const SDL_Rect *const clip = &mWindow->clip_rect;
    const int clipX = clip->x;
    const int clipY = clip->y;
    int dx = clipX - x;
    if (dx > 0)
    {
        w -= dx;
        x += CAST_S16(dx);
        srcX += dx;
    }
    dx = x + w - clipX - clip->w;
    if (dx > 0)
        w -= dx;

    int dy = clipY - y;
    if (dy > 0)
    {
        h -= dy;
        y += CAST_S16(dy);
        srcY += dy;
    }
    dy = y + h - clipY - clip->h;
    if (dy > 0)
        h -= dy;

    if (w > 0 && h > 0)
    {
        SDL_Rect srcRect =
        {
            CAST_S16(srcX),
            CAST_S16(srcY),
            CAST_U16(w),
            CAST_U16(h)
        };

        SDL_Rect dstRect =
        {
            CAST_S16(x),
            CAST_S16(y),
            CAST_U16(w),
            CAST_U16(h)
        };

        SDL_LowerBlit(src, &srcRect, mWindow, &dstRect);
    }
}

void SDLGraphics::drawPatternCached(const Image *restrict const image,
                                    const int x, const int y,
                                    const int w, const int h) restrict2
{
    FUNC_BLOCK("Graphics::drawPatternCached", 1)
    // Check that preconditions for blitting are met.
    if ((mWindow == nullptr) || (image == nullptr))
        return;
    if (image->mSDLSurface == nullptr)
        return;

    const SDL_Rect &bounds = image->mBounds;
    const int iw = bounds.w;
    const int ih = bounds.h;
    if (iw == 0 || ih == 0)
        return;

    const ClipRect &top = mClipStack.top();
    const int xOffset = top.xOffset + x;
    const int yOffset = top.yOffset + y;
    const int srcX = bounds.x;
    const int srcY = bounds.y;
    SDL_Surface *const src = image->mSDLSurface;
    const SDL_Rect *const clip = &mWindow->clip_rect;
    const int clipX = clip->x;
    const int clipY = clip->y;

    for (int py = 0; py < h; py += ih)
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        int dstY = py + yOffset;
        int y2 = srcY;
        int h2 = dh;
        if (y2 < 0)
        {
            h2 += y2;
            dstY -= CAST_S16(y2);
            y2 = 0;
        }
        const int maxh = src->h - y2;
        if (maxh < h2)
            h2 = maxh;

        int dy = clipY - dstY;
        if (dy > 0)
        {
            h2 -= dy;
            dstY += CAST_S16(dy);
            y2 += dy;
        }
        dy = dstY + h2 - clipY - clip->h;
        if (dy > 0)
            h2 -= dy;

        if (h2 > 0)
        {
            for (int px = 0; px < w; px += iw)
            {
                const int dw = (px + iw >= w) ? w - px : iw;
                int dstX = px + xOffset;
                int x2 = srcX;
                int w2 = dw;
                if (x2 < 0)
                {
                    w2 += x2;
                    dstX -= CAST_S16(x2);
                    x2 = 0;
                }
                const int maxw = src->w - x2;
                if (maxw < w2)
                    w2 = maxw;

                int dx = clipX - dstX;
                if (dx > 0)
                {
                    w2 -= dx;
                    dstX += CAST_S16(dx);
                    x2 += dx;
                }
                dx = dstX + w2 - clipX - clip->w;
                if (dx > 0)
                    w2 -= dx;

                if (w2 > 0)
                {
                    SDL_Rect srcRect =
                    {
                        CAST_S16(x2),
                        CAST_S16(y2),
                        CAST_U16(w2),
                        CAST_U16(h2)
                    };

                    SDL_Rect dstRect =
                    {
                        CAST_S16(dstX),
                        CAST_S16(dstY),
                        CAST_U16(w2),
                        CAST_U16(h2)
                    };

                    SDL_LowerBlit(src, &srcRect, mWindow, &dstRect);
                }

//            SDL_BlitSurface(image->mSDLSurface, &srcRect, mWindow, &dstRect);
            }
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
    if ((mWindow == nullptr) || (image == nullptr))
        return;
    if (image->mSDLSurface == nullptr)
        return;

    const SDL_Rect &bounds = image->mBounds;
    const int iw = bounds.w;
    const int ih = bounds.h;
    if (iw == 0 || ih == 0)
        return;

    const ClipRect &top = mClipStack.top();
    const int xOffset = top.xOffset + x;
    const int yOffset = top.yOffset + y;
    const int srcX = bounds.x;
    const int srcY = bounds.y;
    SDL_Surface *const src = image->mSDLSurface;
    const SDL_Rect *const clip = &mWindow->clip_rect;
    const int clipX = clip->x;
    const int clipY = clip->y;

    for (int py = 0; py < h; py += ih)
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        int dstY = py + yOffset;
        int y2 = srcY;
        int h2 = dh;
        if (y2 < 0)
        {
            h2 += y2;
            dstY -= CAST_S16(y2);
            y2 = 0;
        }
        const int maxh = src->h - y2;
        if (maxh < h2)
            h2 = maxh;

        int dy = clipY - dstY;
        if (dy > 0)
        {
            h2 -= dy;
            dstY += CAST_S16(dy);
            y2 += dy;
        }
        dy = dstY + h2 - clipY - clip->h;
        if (dy > 0)
            h2 -= dy;

        if (h2 > 0)
        {
            for (int px = 0; px < w; px += iw)
            {
                const int dw = (px + iw >= w) ? w - px : iw;
                int dstX = px + xOffset;
                int x2 = srcX;
                int w2 = dw;
                if (x2 < 0)
                {
                    w2 += x2;
                    dstX -= CAST_S16(x2);
                    x2 = 0;
                }
                const int maxw = src->w - x2;
                if (maxw < w2)
                    w2 = maxw;

                int dx = clipX - dstX;
                if (dx > 0)
                {
                    w2 -= dx;
                    dstX += CAST_S16(dx);
                    x2 += dx;
                }
                dx = dstX + w2 - clipX - clip->w;
                if (dx > 0)
                    w2 -= dx;

                if (w2 > 0)
                {
                    SDL_Rect srcRect =
                    {
                        CAST_S16(x2),
                        CAST_S16(y2),
                        CAST_U16(w2),
                        CAST_U16(h2)
                    };

                    SDL_Rect dstRect =
                    {
                        CAST_S16(dstX),
                        CAST_S16(dstY),
                        CAST_U16(w2),
                        CAST_U16(h2)
                    };

                    SDL_LowerBlit(src, &srcRect, mWindow, &dstRect);
                }

//            SDL_BlitSurface(image->mSDLSurface, &srcRect, mWindow, &dstRect);
            }
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
    if ((mWindow == nullptr) || (image == nullptr))
        return;
    if (image->mSDLSurface == nullptr)
        return;

    if (scaledHeight == 0 || scaledWidth == 0)
        return;

    Image *const tmpImage = image->SDLgetScaledImage(
        scaledWidth, scaledHeight);
    if (tmpImage == nullptr)
        return;

    const SDL_Rect &bounds = tmpImage->mBounds;
    const int iw = bounds.w;
    const int ih = bounds.h;
    if (iw == 0 || ih == 0)
        return;

    const ClipRect &top = mClipStack.top();
    const int xOffset = top.xOffset + x;
    const int yOffset = top.yOffset + y;
    const int srcX = bounds.x;
    const int srcY = bounds.y;

    for (int py = 0; py < h; py += ih)  // Y position on pattern plane
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int dstY = py + yOffset;

        for (int px = 0; px < w; px += iw)  // X position on pattern plane
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int dstX = px + xOffset;

            SDL_Rect srcRect =
            {
                CAST_S16(srcX),
                CAST_S16(srcY),
                CAST_U16(dw),
                CAST_U16(dh)
            };

            SDL_Rect dstRect =
            {
                CAST_S16(dstX),
                CAST_S16(dstY),
                0,
                0
            };

            SDL_BlitSurface(tmpImage->mSDLSurface, &srcRect,
                            mWindow, &dstRect);
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
    if (vert == nullptr ||
        mWindow == nullptr ||
        image == nullptr ||
        image->mSDLSurface == nullptr)
    {
        return;
    }

    const SDL_Rect &bounds = image->mBounds;
    const int iw = bounds.w;
    const int ih = bounds.h;
    if (iw == 0 || ih == 0)
        return;

    const ClipRect &top = mClipStack.top();
    const int xOffset = top.xOffset + x;
    const int yOffset = top.yOffset + y;
    const int srcX = bounds.x;
    const int srcY = bounds.y;

    for (int py = 0; py < h; py += ih)  // Y position on pattern plane
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int dstY = py + yOffset;

        for (int px = 0; px < w; px += iw)  // X position on pattern plane
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int dstX = px + xOffset;

            DoubleRect *const r = new DoubleRect;
            SDL_Rect &srcRect = r->src;
            srcRect.x = CAST_S16(srcX);
            srcRect.y = CAST_S16(srcY);
            srcRect.w = CAST_U16(dw);
            srcRect.h = CAST_U16(dh);
            SDL_Rect &dstRect = r->dst;
            dstRect.x = CAST_S16(dstX);
            dstRect.y = CAST_S16(dstY);

            if (SDL_FakeUpperBlit(image->mSDLSurface, &srcRect,
                mWindow, &dstRect) == 1)
            {
                vert->sdl.push_back(r);
            }
            else
            {
                delete r;
            }
        }
    }
}

void SDLGraphics::calcPattern(ImageCollection *restrict const vertCol,
                              const Image *restrict const image,
                              const int x, const int y,
                              const int w, const int h) const restrict2
{
    if (vertCol == nullptr || image == nullptr)
        return;

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
    if (vert == nullptr ||
        vert->image == nullptr ||
        vert->image->mSDLSurface == nullptr)
    {
        return;
    }

    const Image *const image = vert->image;
    const ClipRect &top = mClipStack.top();
    const SDL_Rect &bounds = image->mBounds;

    DoubleRect *rect = new DoubleRect;
    rect->src.x = CAST_S16(bounds.x);
    rect->src.y = CAST_S16(bounds.y);
    rect->src.w = CAST_U16(bounds.w);
    rect->src.h = CAST_U16(bounds.h);
    rect->dst.x = CAST_S16(x + top.xOffset);
    rect->dst.y = CAST_S16(y + top.yOffset);
    if (SDL_FakeUpperBlit(image->mSDLSurface, &rect->src,
        mWindow, &rect->dst) == 1)
    {
        vert->sdl.push_back(rect);
    }
    else
    {
        delete rect;
    }
}

void SDLGraphics::calcTileCollection(ImageCollection *restrict const vertCol,
                                     const Image *restrict const image,
                                     int x, int y) restrict2
{
    if (vertCol == nullptr)
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
            SDL_LowerBlit(img->mSDLSurface, &(*it2)->src,
                mWindow, &(*it2)->dst);
            ++ it2;
        }
    }
}

void SDLGraphics::drawTileVertexes(const ImageVertexes *
                                   restrict const vert) restrict2
{
    if (vert == nullptr)
        return;
    // vert and img must be != 0
    const Image *const img = vert->image;
    const DoubleRects *const rects = &vert->sdl;
    DoubleRects::const_iterator it = rects->begin();
    const DoubleRects::const_iterator it_end = rects->end();
    while (it != it_end)
    {
        SDL_LowerBlit(img->mSDLSurface, &(*it)->src, mWindow, &(*it)->dst);
        ++ it;
    }
}

void SDLGraphics::updateScreen() restrict2
{
    BLOCK_START("Graphics::updateScreen")
    if (mDoubleBuffer)
    {
        SDL_Flip(mWindow);
    }
    else
    {
        SDL_UpdateRects(mWindow, 1, &mRect);
//        SDL_UpdateRect(mWindow, 0, 0, 0, 0);
    }
    BLOCK_END("Graphics::updateScreen")
}

void SDLGraphics::calcWindow(ImageCollection *restrict const vertCol,
                             const int x, const int y,
                             const int w, const int h,
                             const ImageRect &restrict imgRect) restrict2
{
    ImageVertexes *vert = nullptr;
    Image *const image = imgRect.grid[4];
    if (image == nullptr)
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

int SDLGraphics::SDL_FakeUpperBlit(const SDL_Surface *restrict const src,
                                   SDL_Rect *restrict const srcrect,
                                   const SDL_Surface *restrict const dst,
                                   SDL_Rect *restrict dstrect) const restrict2
{
    int srcx, srcy, w, h;

    // Make sure the surfaces aren't locked
    if ((src == nullptr) || (dst == nullptr))
        return -1;

    if ((srcrect == nullptr) || (dstrect == nullptr))
        return -1;

    srcx = srcrect->x;
    w = srcrect->w;
    if (srcx < 0)
    {
        w += srcx;
        dstrect->x -= CAST_S16(srcx);
        srcx = 0;
    }
    int maxw = src->w - srcx;
    if (maxw < w)
        w = maxw;

    srcy = srcrect->y;
    h = srcrect->h;
    if (srcy < 0)
    {
        h += srcy;
        dstrect->y -= CAST_S16(srcy);
        srcy = 0;
    }
    int maxh = src->h - srcy;
    if (maxh < h)
        h = maxh;

    const SDL_Rect *const clip = &dst->clip_rect;
    const int clipX = clip->x;
    const int clipY = clip->y;
    int dx = clipX - dstrect->x;
    if (dx > 0)
    {
        w -= dx;
        dstrect->x += CAST_S16(dx);
        srcx += dx;
    }
    dx = dstrect->x + w - clipX - clip->w;
    if (dx > 0)
        w -= dx;

    int dy = clipY - dstrect->y;
    if (dy > 0)
    {
        h -= dy;
        dstrect->y += CAST_S16(dy);
        srcy += dy;
    }
    dy = dstrect->y + h - clipY - clip->h;
    if (dy > 0)
        h -= dy;

    if (w > 0 && h > 0)
    {
        srcrect->x = CAST_S16(srcx);
        srcrect->y = CAST_S16(srcy);
        srcrect->w = CAST_S16(w);
        srcrect->h = CAST_S16(h);
        dstrect->w = CAST_S16(w);
        dstrect->h = CAST_S16(h);

        return 1;
//        return SDL_LowerBlit(src, &sr, dst, dstrect);
    }
    dstrect->w = dstrect->h = 0;
    return 0;
}

void SDLGraphics::fillRectangle(const Rect &restrict rectangle) restrict2
{
    FUNC_BLOCK("Graphics::fillRectangle", 1)
    if (mClipStack.empty())
        return;

    const ClipRect &restrict top = mClipStack.top();

    Rect area = rectangle;
    area.x += top.xOffset;
    area.y += top.yOffset;

    if (!area.isIntersecting(top))
        return;

    if (mAlpha)
    {
        const int x1 = area.x > top.x ? area.x : top.x;
        const int y1 = area.y > top.y ? area.y : top.y;
        const int x2 = area.x + area.width < top.x + top.width ?
            area.x + area.width : top.x + top.width;
        const int y2 = area.y + area.height < top.y + top.height ?
            area.y + area.height : top.y + top.height;

        SDL_LockSurface(mWindow);

        const int bpp = mWindow->format->BytesPerPixel;
        const uint32_t pixel = SDL_MapRGB(mWindow->format,
            CAST_U8(mColor.r), CAST_U8(mColor.g),
            CAST_U8(mColor.b));

        switch (bpp)
        {
            case 1:
                cilk_for (int y = y1; y < y2; y++)
                {
                    uint8_t *const p = static_cast<uint8_t *>(mWindow->pixels)
                        + CAST_SIZE(y * mWindow->pitch);
                    for (int x = x1; x < x2; x++)
                    {
                        *(p + CAST_SIZE(x))
                            = CAST_U8(pixel);
                    }
                }
                break;
            case 2:
                cilk_for (int y = y1; y < y2; y++)
                {
                    uint8_t *const p0 = static_cast<uint8_t *>(mWindow->pixels)
                        + CAST_SIZE(y * mWindow->pitch);
                    for (int x = x1; x < x2; x++)
                    {
                        uint8_t *const p = p0 + CAST_SIZE(x * 2);
                        *reinterpret_cast<uint16_t *>(p) = SDLAlpha16(
                            CAST_U16(pixel),
                            *reinterpret_cast<uint16_t *>(p),
                            CAST_U8(mColor.a), mWindow->format);
                    }
                }
                break;
            case 3:
            {
                const int ca = 255 - mColor.a;
                const int cr = mColor.r * mColor.a;
                const int cg = mColor.g * mColor.a;
                const int cb = mColor.b * mColor.a;

                cilk_for (int y = y1; y < y2; y++)
                {
                    uint8_t *const p0 = static_cast<uint8_t *>(mWindow->pixels)
                        + CAST_SIZE(y * mWindow->pitch);
                    for (int x = x1; x < x2; x++)
                    {
                        uint8_t *const p = p0 + CAST_SIZE(x * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                        p[2] = CAST_U8((p[2] * ca + cb) >> 8);
                        p[1] = CAST_U8((p[1] * ca + cg) >> 8);
                        p[0] = CAST_U8((p[0] * ca + cr) >> 8);
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
                        p[0] = CAST_U8((p[0] * ca + cb) >> 8);
                        p[1] = CAST_U8((p[1] * ca + cg) >> 8);
                        p[2] = CAST_U8((p[2] * ca + cr) >> 8);
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN
                    }
                }
                break;
            }
            case 4:
            {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                const unsigned pb = (pixel & 0xff) * mColor.a;
                const unsigned pg = (pixel & 0xff00) * mColor.a;
                const unsigned pr = (pixel & 0xff0000) * mColor.a;
                const unsigned a1 = (255 - mColor.a);

                cilk_for (int y = y1; y < y2; y++)
                {
                    uint8_t *const p0 = static_cast<uint8_t *>(mWindow->pixels)
                        + y * mWindow->pitch;
                    for (int x = x1; x < x2; x++)
                    {
                        uint8_t *p = p0 + x * 4;
                        uint32_t dst = *reinterpret_cast<uint32_t *>(p);
                        const unsigned int b = (pb + (dst & 0xff) * a1) >> 8;
                        const unsigned int g = (pg + (dst & 0xff00) * a1) >> 8;
                        const unsigned int r = (pr
                            + (dst & 0xff0000) * a1) >> 8;

                        *reinterpret_cast<uint32_t *>(p) = ((b & 0xff)
                            | (g & 0xff00) | (r & 0xff0000));
                    }
                }
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
                if (cR == nullptr)
                {
                    cR = new unsigned int[0x100];
                    cG = new unsigned int[0x100];
                    cB = new unsigned int[0x100];
                    mOldPixel = 0;
                    mOldAlpha = mColor.a;
                }

                const SDL_PixelFormat * const format = mWindow->format;
                const unsigned rMask = format->Rmask;
                const unsigned gMask = format->Gmask;
                const unsigned bMask = format->Bmask;
//                const unsigned aMask = format->Amask;
                unsigned rShift = rMask / 0xff;
                unsigned gShift = gMask / 0xff;
                unsigned bShift = bMask / 0xff;
                if (rShift == 0u)
                    rShift = 1;
                if (gShift == 0u)
                    gShift = 1;
                if (bShift == 0u)
                    bShift = 1;
                if (pixel != mOldPixel || mColor.a != mOldAlpha)
                {
                    const unsigned pb = (pixel & bMask) * mColor.a;
                    const unsigned pg = (pixel & gMask) * mColor.a;
                    const unsigned pr = (pixel & rMask) * mColor.a;
                    const unsigned a0 = (255 - mColor.a);

                    const unsigned int a1 = a0 * bShift;
                    const unsigned int a2 = a0 * gShift;
                    const unsigned int a3 = a0 * rShift;

                    for (int f = 0; f <= 0xff; f ++)
                    {
                        cB[f] = ((pb + f * a1) >> 8) & bMask;
                        cG[f] = ((pg + f * a2) >> 8) & gMask;
                        cR[f] = ((pr + f * a3) >> 8) & rMask;
                    }

                    mOldPixel = pixel;
                    mOldAlpha = mColor.a;
                }

                cilk_for (int y = y1; y < y2; y++)
                {
                    uint32_t *const p0 = reinterpret_cast<uint32_t*>(
                        static_cast<uint8_t*>(mWindow->pixels)
                        + CAST_SIZE(y * mWindow->pitch));
                    for (int x = x1; x < x2; x++)
                    {
                        uint32_t *const p = p0 + CAST_SIZE(x);
                        const uint32_t dst = *p;
                        *p = cB[dst & bMask / bShift]
                            | cG[(dst & gMask) / gShift]
                            | cR[(dst & rMask) / rShift];
                    }
                }
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN
                break;
            }
            default:
                break;
        }

        SDL_UnlockSurface(mWindow);
    }
    else
    {
        SDL_Rect rect =
        {
            CAST_S16(area.x),
            CAST_S16(area.y),
            CAST_U16(area.width),
            CAST_U16(area.height)
        };

        const uint32_t color = SDL_MapRGBA(mWindow->format,
            CAST_S8(mColor.r),
            CAST_S8(mColor.g),
            CAST_S8(mColor.b),
            CAST_S8(mColor.a));
        SDL_FillRect(mWindow, &rect, color);
    }
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
    const ClipRect &restrict carea = mClipStack.top();
    const SDL_Rect rect =
    {
        CAST_S16(carea.x),
        CAST_S16(carea.y),
        CAST_U16(carea.width),
        CAST_U16(carea.height)
    };
    SDL_SetClipRect(mWindow, &rect);
}

void SDLGraphics::popClipArea() restrict2
{
    Graphics::popClipArea();

    if (mClipStack.empty())
        return;

    const ClipRect &restrict carea = mClipStack.top();
    const SDL_Rect rect =
    {
        CAST_S16(carea.x),
        CAST_S16(carea.y),
        CAST_U16(carea.width),
        CAST_U16(carea.height)
    };

    SDL_SetClipRect(mWindow, &rect);
}

void SDLGraphics::drawPoint(int x, int y) restrict2
{
    if (mClipStack.empty())
        return;

    const ClipRect& top = mClipStack.top();

    x += top.xOffset;
    y += top.yOffset;

    if (!top.isPointInRect(x, y))
        return;

    if (mAlpha)
        SDLputPixelAlpha(mWindow, x, y, mColor);
    else
        SDLputPixel(mWindow, x, y, mColor);
}

void SDLGraphics::drawHLine(int x1, int y, int x2) restrict2
{
    if (mClipStack.empty())
        return;

    const ClipRect& top = mClipStack.top();

    const int xOffset = top.xOffset;
    x1 += xOffset;
    y += top.yOffset;
    x2 += xOffset;

    const int topY = top.y;
    if (y < topY || y >= topY + top.height)
        return;

    if (x1 > x2)
    {
        x1 ^= x2;
        x2 ^= x1;
        x1 ^= x2;
    }

    const int topX = top.x;
    if (topX > x1)
    {
        if (topX > x2)
            return;

        x1 = topX;
    }

    const int sumX = topX + top.width;
    if (sumX <= x2)
    {
        if (sumX <= x1)
            return;

        x2 = sumX -1;
    }

    const int bpp = mWindow->format->BytesPerPixel;

    SDL_LockSurface(mWindow);

    uint8_t *p = static_cast<uint8_t*>(mWindow->pixels)
        + CAST_SIZE(y * mWindow->pitch + x1 * bpp);

    const uint32_t pixel = SDL_MapRGB(mWindow->format,
        CAST_U8(mColor.r),
        CAST_U8(mColor.g),
        CAST_U8(mColor.b));
    switch (bpp)
    {
        case 1:
            for (; x1 <= x2; ++x1)
                *(p++) = CAST_U8(pixel);
            break;

        case 2:
        {
            uint16_t* q = reinterpret_cast<uint16_t*>(p);
            const uint16_t pixel1 = CAST_U16(pixel);
            for (; x1 <= x2; ++x1)
                *(q++) = pixel1;
            break;
        }

        case 3:
        {
            const uint8_t b0 = CAST_U8((pixel >> 16) & 0xff);
            const uint8_t b1 = CAST_U8((pixel >> 8) & 0xff);
            const uint8_t b2 = CAST_U8(pixel & 0xff);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            for (; x1 <= x2; ++x1)
            {
                p[0] = b0;
                p[1] = b1;
                p[2] = b2;
                p += 3;
            }
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
            for (; x1 <= x2; ++x1)
            {
                p[0] = b2;
                p[1] = b1;
                p[2] = b0;
                p += 3;
            }
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN
            break;
        }

        case 4:
        {
            uint32_t *q = reinterpret_cast<uint32_t*>(p);
            if (mAlpha)
            {
                unsigned char a = CAST_U8(mColor.a);
                unsigned char a1 = CAST_U8(255U - a);
                const int b0 = (pixel & 0xff) * a;
                const int g0 = (pixel & 0xff00) * a;
                const int r0 = (pixel & 0xff0000) * a;
                for (; x1 <= x2; ++x1)
                {
                    const unsigned int b = (b0 + (*q & 0xff) * a1) >> 8;
                    const unsigned int g = (g0 + (*q & 0xff00) * a1) >> 8;
                    const unsigned int r = (r0 + (*q & 0xff0000) * a1) >> 8;
                    *q = (b & 0xff) | (g & 0xff00) | (r & 0xff0000);

                    q++;
                }
            }
            else
            {
                for (; x1 <= x2; ++x1)
                    *(q++) = pixel;
            }
            break;
        }
        default:
            break;
    }  // end switch

    SDL_UnlockSurface(mWindow);
}

void SDLGraphics::drawVLine(int x, int y1, int y2) restrict2
{
    if (mClipStack.empty())
        return;

    const ClipRect &restrict top = mClipStack.top();

    const int yOffset = top.yOffset;
    x += top.xOffset;
    y1 += yOffset;
    y2 += yOffset;

    if (x < top.x || x >= top.x + top.width)
        return;

    if (y1 > y2)
    {
        y1 ^= y2;
        y2 ^= y1;
        y1 ^= y2;
    }

    if (top.y > y1)
    {
        if (top.y > y2)
            return;

        y1 = top.y;
    }

    const int sumY = top.y + top.height;
    if (sumY <= y2)
    {
        if (sumY <= y1)
            return;

        y2 = sumY - 1;
    }

    const int bpp = mWindow->format->BytesPerPixel;

    SDL_LockSurface(mWindow);

    uint8_t *p = static_cast<uint8_t*>(mWindow->pixels)
        + CAST_SIZE(y1 * mWindow->pitch + x * bpp);

    const uint32_t pixel = SDL_MapRGB(mWindow->format,
        CAST_U8(mColor.r),
        CAST_U8(mColor.g),
        CAST_U8(mColor.b));

    const int pitch = mWindow->pitch;
    switch (bpp)
    {
        case 1:
            for (; y1 <= y2; ++y1)
            {
                *p = CAST_U8(pixel);
                p += pitch;
            }
            break;

        case 2:
            for (; y1 <= y2; ++ y1)
            {
                *reinterpret_cast<uint16_t*>(p)
                    = CAST_U16(pixel);
                p += pitch;
            }
            break;

        case 3:
        {
            const uint8_t b0 = CAST_U8((pixel >> 16) & 0xff);
            const uint8_t b1 = CAST_U8((pixel >> 8) & 0xff);
            const uint8_t b2 = CAST_U8(pixel & 0xff);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            for (; y1 <= y2; ++y1)
            {
                p[0] = b0;
                p[1] = b1;
                p[2] = b2;
                p += pitch;
            }
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
            for (; y1 <= y2; ++y1)
            {
                p[0] = b2;
                p[1] = b1;
                p[2] = b0;
                p += pitch;
            }
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN
            break;
        }

        case 4:
        {
            if (mAlpha)
            {
                unsigned char a = CAST_U8(mColor.a);
                unsigned char a1 = CAST_U8(255U - a);
                const int b0 = (pixel & 0xff) * a;
                const int g0 = (pixel & 0xff00) * a;
                const int r0 = (pixel & 0xff0000) * a;
                for (; y1 <= y2; ++y1)
                {
                    const unsigned int dst = *reinterpret_cast<uint32_t*>(p);
                    const unsigned int b = (b0 + (dst & 0xff) * a1) >> 8;
                    const unsigned int g = (g0 + (dst & 0xff00) * a1) >> 8;
                    const unsigned int r = (r0 + (dst & 0xff0000) * a1) >> 8;
                    *reinterpret_cast<uint32_t*>(p) =
                        (b & 0xff) | (g & 0xff00) | (r & 0xff0000);

                    p += pitch;
                }
            }
            else
            {
                for (; y1 <= y2; ++y1)
                {
                    *reinterpret_cast<uint32_t*>(p) = pixel;
                    p += pitch;
                }
            }
            break;
        }

        default:
            break;
    }  // end switch

    SDL_UnlockSurface(mWindow);
}

void SDLGraphics::drawRectangle(const Rect &restrict rectangle) restrict2
{
    const int x1 = rectangle.x;
    const int x2 = x1 + rectangle.width - 1;
    const int y1 = rectangle.y;
    const int y2 = y1 + rectangle.height - 1;

    drawHLine(x1, y1, x2);
    drawHLine(x1, y2, x2);

    drawVLine(x1, y1, y2);
    drawVLine(x2, y1, y2);
}

void SDLGraphics::drawLine(int x1, int y1,
                           int x2, int y2) restrict2
{
    if (x1 == x2)
    {
        drawVLine(x1, y1, y2);
        return;
    }
    if (y1 == y2)
    {
        drawHLine(x1, y1, x2);
        return;
    }

    //  other cases not implemented
}

bool SDLGraphics::setVideoMode(const int w, const int h,
                               const int scale,
                               const int bpp,
                               const bool fs,
                               const bool hwaccel,
                               const bool resize,
                               const bool noFrame) restrict2
{
    setMainFlags(w, h, scale, bpp, fs, hwaccel, resize, noFrame);

    if ((mWindow = graphicsManager.createWindow(w, h, bpp,
        getSoftwareFlags())) == nullptr)
    {
        mRect.w = 0;
        mRect.h = 0;
        return false;
    }

    mRect.w = CAST_U16(mWindow->w);
    mRect.h = CAST_U16(mWindow->h);

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
