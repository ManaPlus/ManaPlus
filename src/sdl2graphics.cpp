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

#ifdef USE_SDL2

#include "sdl2graphics.h"

#include "main.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "resources/imagehelper.h"
#include "resources/sdl2imagehelper.h"

#include <guichan/sdl/sdlpixel.hpp>

#include "utils/sdlcheckutils.h"

#include <SDL.h>

#include "debug.h"

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
static unsigned int *cR = nullptr;
static unsigned int *cG = nullptr;
static unsigned int *cB = nullptr;
#endif

#ifdef DEBUG_SDL_SURFACES

#define MSDL_RenderCopy(render, texture, src, dst) \
    FakeSDL_RenderCopy(render, texture, src, dst)

static int FakeSDL_RenderCopy(SDL_Renderer *const renderer,
                              SDL_Texture *const texture,
                              const SDL_Rect *const srcrect,
                               const SDL_Rect *const dstrect)
{
    int ret = SDL_RenderCopy(renderer, texture, srcrect, dstrect);
    if (ret)
    {
        logger->log("rendering error in texture %p: %s",
            static_cast<void*>(texture), SDL_GetError());
    }
    return ret;
}

#else

#define MSDL_RenderCopy(render, texture, src, dst) \
    SDL_RenderCopy(render, texture, src, dst)

#endif

SDLGraphics::SDLGraphics() :
    Graphics(),
    mOldPixel(0),
    mOldAlpha(0)
{
}

SDLGraphics::~SDLGraphics()
{
}

bool SDLGraphics::drawRescaledImage(const Image *const image, int srcX, int srcY,
                                    int dstX, int dstY,
                                    const int width, const int height,
                                    const int desiredWidth,
                                    const int desiredHeight,
                                    const bool useColor A_UNUSED)
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    // Check that preconditions for blitting are met.
    if (!mWindow || !image)
        return false;
    if (!image->mTexture)
        return false;

    dstX += mClipStack.top().xOffset;
    dstY += mClipStack.top().yOffset;

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
    dstRect.w = static_cast<uint16_t>(desiredWidth);
    dstRect.h = static_cast<uint16_t>(desiredHeight);

    return (MSDL_RenderCopy(mRenderer, image->mTexture,
        &srcRect, &dstRect) < 0);
}

bool SDLGraphics::drawImage2(const Image *const image, int srcX, int srcY,
                             int dstX, int dstY, const int width,
                             const int height, const bool useColor A_UNUSED)
{
    FUNC_BLOCK("Graphics::drawImage2", 1)
    // Check that preconditions for blitting are met.
    if (!mWindow || !image || !image->mTexture)
        return false;

    dstX += mClipStack.top().xOffset;
    dstY += mClipStack.top().yOffset;

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
    dstRect.w = static_cast<uint16_t>(width);
    dstRect.h = static_cast<uint16_t>(height);

    return !MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
}

void SDLGraphics::drawImagePattern(const Image *const image,
                                   const int x, const int y,
                                   const int w, const int h)
{
    FUNC_BLOCK("Graphics::drawImagePattern", 1)
    // Check that preconditions for blitting are met.
    if (!mWindow || !image)
        return;
    if (!image->mTexture)
        return;

    const int iw = image->mBounds.w;
    const int ih = image->mBounds.h;

    if (iw == 0 || ih == 0)
        return;

    for (int py = 0; py < h; py += ih)  // Y position on pattern plane
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int srcY = image->mBounds.y;
        const int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw)  // X position on pattern plane
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int srcX = image->mBounds.x;
            const int dstX = x + px + mClipStack.top().xOffset;

            SDL_Rect dstRect;
            SDL_Rect srcRect;
            dstRect.x = static_cast<int16_t>(dstX);
            dstRect.y = static_cast<int16_t>(dstY);
            srcRect.x = static_cast<int16_t>(srcX);
            srcRect.y = static_cast<int16_t>(srcY);
            srcRect.w = static_cast<uint16_t>(dw);
            srcRect.h = static_cast<uint16_t>(dh);
            dstRect.w = static_cast<uint16_t>(dw);
            dstRect.h = static_cast<uint16_t>(dh);

            MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
        }
    }
}

void SDLGraphics::drawRescaledImagePattern(const Image *const image,
                                           const int x, const int y,
                                           const int w, const int h,
                                           const int scaledWidth,
                                           const int scaledHeight)
{
    // Check that preconditions for blitting are met.
    if (!mWindow || !image)
        return;
    if (!image->mTexture)
        return;

    if (scaledHeight == 0 || scaledWidth == 0)
        return;

    Image *const tmpImage = image->SDLgetScaledImage(
        scaledWidth, scaledHeight);
    if (!tmpImage)
        return;

    const int iw = tmpImage->mBounds.w;
    const int ih = tmpImage->mBounds.h;

    if (iw == 0 || ih == 0)
        return;

    for (int py = 0; py < h; py += ih)  // Y position on pattern plane
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int srcY = tmpImage->mBounds.y;
        const int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw)  // X position on pattern plane
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int srcX = tmpImage->mBounds.x;
            const int dstX = x + px + mClipStack.top().xOffset;

            SDL_Rect dstRect;
            SDL_Rect srcRect;
            dstRect.x = static_cast<int16_t>(dstX);
            dstRect.y = static_cast<int16_t>(dstY);
            srcRect.x = static_cast<int16_t>(srcX);
            srcRect.y = static_cast<int16_t>(srcY);
            srcRect.w = static_cast<uint16_t>(dw);
            srcRect.h = static_cast<uint16_t>(dh);
            dstRect.w = static_cast<uint16_t>(dw);
            dstRect.h = static_cast<uint16_t>(dh);

            MSDL_RenderCopy(mRenderer, image->mTexture, &srcRect, &dstRect);
        }
    }

    delete tmpImage;
}

void SDLGraphics::calcImagePattern(ImageVertexes* const vert,
                                   const Image *const image,
                                   const int x, const int y,
                                   const int w, const int h) const
{
    // Check that preconditions for blitting are met.
    if (!vert || !mWindow || !image || !image->mTexture)
        return;

    const int iw = image->mBounds.w;
    const int ih = image->mBounds.h;

    if (iw == 0 || ih == 0)
        return;

    for (int py = 0; py < h; py += ih)  // Y position on pattern plane
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int srcY = image->mBounds.y;
        const int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw)  // X position on pattern plane
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int srcX = image->mBounds.x;
            const int dstX = x + px + mClipStack.top().xOffset;

            DoubleRect *const r = new DoubleRect();
            SDL_Rect &dstRect = r->dst;
            SDL_Rect &srcRect = r->src;
            dstRect.x = static_cast<int16_t>(dstX);
            dstRect.y = static_cast<int16_t>(dstY);
            srcRect.x = static_cast<int16_t>(srcX);
            srcRect.y = static_cast<int16_t>(srcY);
            srcRect.w = static_cast<uint16_t>(dw);
            srcRect.h = static_cast<uint16_t>(dh);
            dstRect.w = static_cast<uint16_t>(dw);
            dstRect.h = static_cast<uint16_t>(dh);

            vert->sdl.push_back(r);
        }
    }
}

void SDLGraphics::calcImagePattern(ImageCollection* const vertCol,
                                   const Image *const image,
                                   const int x, const int y,
                                   const int w, const int h) const
{
    ImageVertexes *vert = nullptr;
    if (vertCol->currentImage != image)
    {
        vert = new ImageVertexes();
        vertCol->currentImage = image;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
    }
    else
    {
        vert = vertCol->currentVert;
    }

    calcImagePattern(vert, image, x, y, w, h);
}

void SDLGraphics::calcTile(ImageVertexes *const vert,
                           const Image *const image,
                           int x, int y) const
{
    vert->image = image;
    calcTileSDL(vert, x, y);
}

void SDLGraphics::calcTileSDL(ImageVertexes *const vert, int x, int y) const
{
    // Check that preconditions for blitting are met.
    if (!vert || !vert->image || !vert->image->mTexture)
        return;

    const Image *const image = vert->image;

    x += mClipStack.top().xOffset;
    y += mClipStack.top().yOffset;

    DoubleRect *rect = new DoubleRect();

    rect->dst.x = static_cast<int16_t>(x);
    rect->dst.y = static_cast<int16_t>(y);
    rect->src.x = static_cast<int16_t>(image->mBounds.x);
    rect->src.y = static_cast<int16_t>(image->mBounds.y);
    rect->src.w = static_cast<uint16_t>(image->mBounds.w);
    rect->src.h = static_cast<uint16_t>(image->mBounds.h);
    rect->dst.w = static_cast<uint16_t>(image->mBounds.w);
    rect->dst.h = static_cast<uint16_t>(image->mBounds.h);

    vert->sdl.push_back(rect);
}

void SDLGraphics::calcTile(ImageCollection *const vertCol,
                           const Image *const image,
                           int x, int y)
{
    if (vertCol->currentImage != image)
    {
        ImageVertexes *const vert = new ImageVertexes();
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

void SDLGraphics::drawTile(const ImageCollection *const vertCol)
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

void SDLGraphics::drawTile(const ImageVertexes *const vert)
{
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

void SDLGraphics::updateScreen()
{
    BLOCK_START("Graphics::updateScreen")

    SDL_RenderPresent(mRenderer);

//    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
//    SDL_RenderClear(mRenderer);
    BLOCK_END("Graphics::updateScreen")
}

SDL_Surface *SDLGraphics::getScreenshot()
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
#else
    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
#endif
    const int amask = 0x00000000;

    SDL_Surface *const screenshot = MSDL_CreateRGBSurface(SDL_SWSURFACE,
        mRect.w, mRect.h, 24, rmask, gmask, bmask, amask);

//    if (screenshot)
//        SDL_BlitSurface(mWindow, nullptr, screenshot, nullptr);

    return screenshot;
}

bool SDLGraphics::drawNet(const int x1, const int y1,
                          const int x2, const int y2,
                          const int width, const int height)
{
    // +++ need use SDL_RenderDrawLines
    for (int y = y1; y < y2; y += height)
        drawLine(x1, y, x2, y);

    for (int x = x1; x < x2; x += width)
        drawLine(x, y1, x, y2);

    return true;
}

bool SDLGraphics::calcWindow(ImageCollection *const vertCol,
                             const int x, const int y,
                             const int w, const int h,
                             const ImageRect &imgRect)
{
    ImageVertexes *vert = nullptr;
    Image *const image = imgRect.grid[4];
    if (vertCol->currentImage != image)
    {
        vert = new ImageVertexes();
        vertCol->currentImage = image;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
    }
    else
    {
        vert = vertCol->currentVert;
    }

    return calcImageRect(vert, x, y, w, h,
        imgRect.grid[0], imgRect.grid[2], imgRect.grid[6], imgRect.grid[8],
        imgRect.grid[1], imgRect.grid[5], imgRect.grid[7], imgRect.grid[3],
        imgRect.grid[4]);
}

void SDLGraphics::fillRectangle(const gcn::Rectangle &rectangle)
{
    const SDL_Rect rect
    {
        static_cast<int16_t>(rectangle.x) + mClipStack.top().xOffset,
        static_cast<int16_t>(rectangle.y) + mClipStack.top().yOffset,
        static_cast<uint16_t>(rectangle.width),
        static_cast<uint16_t>(rectangle.height)
    };

    SDL_SetRenderDrawColor(mRenderer, mColor.r, mColor.g, mColor.b, mColor.a);
    SDL_RenderFillRect(mRenderer, &rect);
}

void SDLGraphics::_beginDraw()
{
    pushClipArea(gcn::Rectangle(0, 0, mRect.w, mRect.h));
}

void SDLGraphics::_endDraw()
{
    popClipArea();
}

bool SDLGraphics::pushClipArea(gcn::Rectangle area)
{
    SDL_Rect rect;
    const bool result = gcn::Graphics::pushClipArea(area);

    const gcn::ClipRectangle &carea = mClipStack.top();
    rect.x = static_cast<int16_t>(carea.x);
    rect.y = static_cast<int16_t>(carea.y);
    rect.w = static_cast<int16_t>(carea.width);
    rect.h = static_cast<int16_t>(carea.height);
    SDL_RenderSetClipRect(mRenderer, &rect);

    return result;
}

void SDLGraphics::popClipArea()
{
    gcn::Graphics::popClipArea();

    if (mClipStack.empty())
        return;

    const gcn::ClipRectangle &carea = mClipStack.top();
    SDL_Rect rect;
    rect.x = static_cast<int16_t>(carea.x);
    rect.y = static_cast<int16_t>(carea.y);
    rect.w = static_cast<int16_t>(carea.width);
    rect.h = static_cast<int16_t>(carea.height);

    SDL_RenderSetClipRect(mRenderer, &rect);
}

void SDLGraphics::drawPoint(int x, int y)
{
    if (mClipStack.empty())
        return;

    const gcn::ClipRectangle& top = mClipStack.top();

    x += top.xOffset;
    y += top.yOffset;

    if (!top.isPointInRect(x, y))
        return;

    SDL_RenderDrawPoint(mRenderer, x, y);
}


void SDLGraphics::drawRectangle(const gcn::Rectangle &rectangle)
{
    const SDL_Rect rect
    {
        static_cast<int16_t>(rectangle.x) + mClipStack.top().xOffset,
        static_cast<int16_t>(rectangle.y) + mClipStack.top().yOffset,
        static_cast<uint16_t>(rectangle.width),
        static_cast<uint16_t>(rectangle.height)
    };

    SDL_SetRenderDrawColor(mRenderer, mColor.r, mColor.g, mColor.b, mColor.a);
    SDL_RenderDrawRect(mRenderer, &rect);
}

void SDLGraphics::drawLine(int x1, int y1, int x2, int y2)
{
    const gcn::ClipRectangle& top = mClipStack.top();

    const int x0 = top.xOffset;
    const int y0 = top.yOffset;

    SDL_RenderDrawLine(mRenderer, x1 + x0, y1 + x0, x2 + x0, y2 + x0);
}

bool SDLGraphics::setVideoMode(const int w, const int h, const int bpp,
                               const bool fs, const bool hwaccel,
                               const bool resize, const bool noFrame)
{
    setMainFlags(w, h, bpp, fs, hwaccel, resize, noFrame);

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

    mRenderer = graphicsManager.createRenderer(mWindow, 0);
    SDLImageHelper::setRenderer(mRenderer);
    return videoInfo();
}

#endif  // USE_SDL2
