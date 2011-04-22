/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include <cassert>

#include "graphics.h"

#include "graphicsvertexes.h"
#include "log.h"

#include "resources/image.h"
#include "resources/imageloader.h"

#include "utils/stringutils.h"

//<SDL_gfxBlitFunc.h>
#include "SDL_gfxBlitFunc.h"

Graphics::Graphics():
    mWidth(0),
    mHeight(0),
    mBpp(0),
    mFullscreen(false),
    mHWAccel(false),
    mBlitMode(BLIT_NORMAL),
    mRedraw(false)
{
}

Graphics::~Graphics()
{
    _endDraw();
}

bool Graphics::setVideoMode(int w, int h, int bpp, bool fs, bool hwaccel)
{
    logger->log("Setting video mode %dx%d %s",
            w, h, fs ? "fullscreen" : "windowed");

    logger->log("Bits per pixel: %d", bpp);

    int displayFlags = SDL_ANYFORMAT;

    mWidth = w;
    mHeight = h;
    mBpp = bpp;
    mFullscreen = fs;
    mHWAccel = hwaccel;

    if (fs)
        displayFlags |= SDL_FULLSCREEN;

    if (hwaccel)
        displayFlags |= SDL_HWSURFACE | SDL_DOUBLEBUF;
    else
        displayFlags |= SDL_SWSURFACE;

    setTarget(SDL_SetVideoMode(w, h, bpp, displayFlags));

    if (!mTarget)
        return false;

    char videoDriverName[65];

    if (SDL_VideoDriverName(videoDriverName, 64))
        logger->log("Using video driver: %s", videoDriverName);
    else
        logger->log1("Using video driver: unknown");

    const SDL_VideoInfo *vi = SDL_GetVideoInfo();

    logger->log("Possible to create hardware surfaces: %s",
            ((vi->hw_available) ? "yes" : "no"));
    logger->log("Window manager available: %s",
            ((vi->wm_available) ? "yes" : "no"));
    logger->log("Accelerated hardware to hardware blits: %s",
            ((vi->blit_hw) ? "yes" : "no"));
    logger->log("Accelerated hardware to hardware colorkey blits: %s",
            ((vi->blit_hw_CC) ? "yes" : "no"));
    logger->log("Accelerated hardware to hardware alpha blits: %s",
            ((vi->blit_hw_A) ? "yes" : "no"));
    logger->log("Accelerated software to hardware blits: %s",
            ((vi->blit_sw) ? "yes" : "no"));
    logger->log("Accelerated software to hardware colorkey blits: %s",
            ((vi->blit_sw_CC) ? "yes" : "no"));
    logger->log("Accelerated software to hardware alpha blits: %s",
            ((vi->blit_sw_A) ? "yes" : "no"));
    logger->log("Accelerated color fills: %s",
            ((vi->blit_fill) ? "yes" : "no"));
    logger->log("Available video memory: %d", vi->video_mem);

    return true;
}

bool Graphics::setFullscreen(bool fs)
{
    if (mFullscreen == fs)
        return true;

    return setVideoMode(mWidth, mHeight, mBpp, fs, mHWAccel);
}

int Graphics::getWidth() const
{
    return mWidth;
}

int Graphics::getHeight() const
{
    return mHeight;
}

bool Graphics::drawImage(Image *image, int x, int y)
{
    if (image)
    {
        return drawImage(image, 0, 0, x, y,
            image->mBounds.w, image->mBounds.h);
    }
    else
    {
        return false;
    }
}

bool Graphics::drawRescaledImage(Image *image, int srcX, int srcY,
                                 int dstX, int dstY,
                                 int width, int height,
                                 int desiredWidth, int desiredHeight,
                                 bool useColor _UNUSED_)
{
    // Check that preconditions for blitting are met.
    if (!mTarget || !image)
        return false;
    if (!image->mSDLSurface)
        return false;

    Image *tmpImage = image->SDLgetScaledImage(desiredWidth, desiredHeight);

    bool returnValue = false;

    if (!tmpImage)
        return false;
    if (!tmpImage->mSDLSurface)
        return false;

    dstX += mClipStack.top().xOffset;
    dstY += mClipStack.top().yOffset;

    srcX += image->mBounds.x;
    srcY += image->mBounds.y;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    dstRect.x = static_cast<short>(dstX);
    dstRect.y = static_cast<short>(dstY);
    srcRect.x = static_cast<short>(srcX);
    srcRect.y = static_cast<short>(srcY);
    srcRect.w = static_cast<Uint16>(width);
    srcRect.h = static_cast<Uint16>(height);

    returnValue = !(SDL_BlitSurface(tmpImage->mSDLSurface,
        &srcRect, mTarget, &dstRect) < 0);

    delete tmpImage;

    return returnValue;
}

bool Graphics::drawImage(Image *image, int srcX, int srcY, int dstX, int dstY,
                         int width, int height, bool)
{
    // Check that preconditions for blitting are met.
    if (!mTarget || !image || !image->mSDLSurface)
        return false;

    dstX += mClipStack.top().xOffset;
    dstY += mClipStack.top().yOffset;

    srcX += image->mBounds.x;
    srcY += image->mBounds.y;

    SDL_Rect dstRect;
    SDL_Rect srcRect;
    dstRect.x = static_cast<short>(dstX);
    dstRect.y = static_cast<short>(dstY);
    srcRect.x = static_cast<short>(srcX);
    srcRect.y = static_cast<short>(srcY);
    srcRect.w = static_cast<Uint16>(width);
    srcRect.h = static_cast<Uint16>(height);

    if (mBlitMode == BLIT_NORMAL)
    {
        return !(SDL_BlitSurface(image->mSDLSurface, &srcRect,
                                 mTarget, &dstRect) < 0);
    }
    else
    {
        return !(SDL_gfxBlitRGBA(image->mSDLSurface, &srcRect,
                                 mTarget, &dstRect) < 0);
    }
}

void Graphics::drawImage(gcn::Image const *image, int srcX, int srcY,
                         int dstX, int dstY, int width, int height)
{
    ProxyImage const *srcImage =
        dynamic_cast< ProxyImage const * >(image);
    if (!srcImage)
        return;
    drawImage(srcImage->getImage(), srcX, srcY, dstX, dstY,
              width, height, true);
}

void Graphics::drawImagePattern(Image *image, int x, int y, int w, int h)
{
    // Check that preconditions for blitting are met.
    if (!mTarget || !image)
        return;
    if (!image->mSDLSurface)
        return;

    const int iw = image->getWidth();
    const int ih = image->getHeight();

    if (iw == 0 || ih == 0) return;

    for (int py = 0; py < h; py += ih)     // Y position on pattern plane
    {
        int dh = (py + ih >= h) ? h - py : ih;
        int srcY = image->mBounds.y;
        int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw) // X position on pattern plane
        {
            int dw = (px + iw >= w) ? w - px : iw;
            int srcX = image->mBounds.x;
            int dstX = x + px + mClipStack.top().xOffset;

            SDL_Rect dstRect;
            SDL_Rect srcRect;
            dstRect.x = static_cast<short>(dstX);
            dstRect.y = static_cast<short>(dstY);
            srcRect.x = static_cast<short>(srcX);
            srcRect.y = static_cast<short>(srcY);
            srcRect.w = static_cast<Uint16>(dw);
            srcRect.h = static_cast<Uint16>(dh);

            SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
        }
    }
}

void Graphics::drawRescaledImagePattern(Image *image, int x, int y,
                                        int w, int h, int scaledWidth,
                                        int scaledHeight)
{
    // Check that preconditions for blitting are met.
    if (!mTarget || !image)
        return;
    if (!image->mSDLSurface)
        return;

    if (scaledHeight == 0 || scaledWidth == 0)
        return;

    Image *tmpImage = image->SDLgetScaledImage(scaledWidth, scaledHeight);
    if (!tmpImage)
        return;

    const int iw = tmpImage->getWidth();
    const int ih = tmpImage->getHeight();

    if (iw == 0 || ih == 0)
        return;

    for (int py = 0; py < h; py += ih)     // Y position on pattern plane
    {
        int dh = (py + ih >= h) ? h - py : ih;
        int srcY = tmpImage->mBounds.y;
        int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw) // X position on pattern plane
        {
            int dw = (px + iw >= w) ? w - px : iw;
            int srcX = tmpImage->mBounds.x;
            int dstX = x + px + mClipStack.top().xOffset;

            SDL_Rect dstRect;
            SDL_Rect srcRect;
            dstRect.x = static_cast<short>(dstX);
            dstRect.y = static_cast<short>(dstY);
            srcRect.x = static_cast<short>(srcX);
            srcRect.y = static_cast<short>(srcY);
            srcRect.w = static_cast<Uint16>(dw);
            srcRect.h = static_cast<Uint16>(dh);

            SDL_BlitSurface(tmpImage->mSDLSurface, &srcRect,
                            mTarget, &dstRect);
        }
    }

    delete tmpImage;
}

void Graphics::drawImageRect(int x, int y, int w, int h,
                             Image *topLeft, Image *topRight,
                             Image *bottomLeft, Image *bottomRight,
                             Image *top, Image *right,
                             Image *bottom, Image *left,
                             Image *center)
{
    pushClipArea(gcn::Rectangle(x, y, w, h));

    const bool drawMain = center && topLeft && topRight
        && bottomLeft && bottomRight;

    // Draw the center area
    if (center && drawMain)
    {
        drawImagePattern(center,
            topLeft->getWidth(), topLeft->getHeight(),
            w - topLeft->getWidth() - topRight->getWidth(),
            h - topLeft->getHeight() - bottomLeft->getHeight());
    }

    // Draw the sides
    if (top && left && bottom && right)
    {
        drawImagePattern(top,
            left->getWidth(), 0,
            w - left->getWidth() - right->getWidth(), top->getHeight());
        drawImagePattern(bottom,
            left->getWidth(), h - bottom->getHeight(),
            w - left->getWidth() - right->getWidth(),
            bottom->getHeight());
        drawImagePattern(left,
            0, top->getHeight(),
            left->getWidth(),
            h - top->getHeight() - bottom->getHeight());
        drawImagePattern(right,
            w - right->getWidth(), top->getHeight(),
            right->getWidth(),
            h - top->getHeight() - bottom->getHeight());
    }

    // Draw the corners
    if (drawMain)
    {
        drawImage(topLeft, 0, 0);
        drawImage(topRight, w - topRight->getWidth(), 0);
        drawImage(bottomLeft, 0, h - bottomLeft->getHeight());
        drawImage(bottomRight,
            w - bottomRight->getWidth(),
            h - bottomRight->getHeight());
    }

    popClipArea();
}

void Graphics::drawImageRect(int x, int y, int w, int h,
                             const ImageRect &imgRect)
{
    drawImageRect(x, y, w, h,
            imgRect.grid[0], imgRect.grid[2], imgRect.grid[6], imgRect.grid[8],
            imgRect.grid[1], imgRect.grid[5], imgRect.grid[7], imgRect.grid[3],
            imgRect.grid[4]);
}

void Graphics::drawImageRect2(GraphicsVertexes* vert, const ImageRect &imgRect)
{
    if (!vert)
        return;

    vert->setPtr(0);

    const bool drawMain = imgRect.grid[4] && imgRect.grid[0] && imgRect.grid[2]
        && imgRect.grid[6] && imgRect.grid[8];

    if (drawMain)
    {
        // center
        drawImagePattern2(vert, imgRect.grid[4]);
    }
    vert->incPtr();

    // top
    drawImagePattern2(vert, imgRect.grid[1]);
    vert->incPtr();

    // bottom
    drawImagePattern2(vert, imgRect.grid[7]);
    vert->incPtr();

    // left
    drawImagePattern2(vert, imgRect.grid[3]);
    vert->incPtr();

    // right
    drawImagePattern2(vert, imgRect.grid[5]);
    vert->incPtr();

    // Draw the corners
    if (drawMain)
    {
        drawImage(imgRect.grid[0], 0, 0);
        drawImage(imgRect.grid[2], vert->mW - imgRect.grid[2]->getWidth(), 0);
        drawImage(imgRect.grid[6], 0, vert->mH - imgRect.grid[6]->getHeight());
        drawImage(imgRect.grid[8],
            vert->mW - imgRect.grid[8]->getWidth(),
            vert->mH - imgRect.grid[8]->getHeight());
    }
}

void Graphics::drawImagePattern2(GraphicsVertexes *vert, Image *img)
{
    // here not checking input parameters

    std::list<DoubleRect*> *arr = vert->getRectsSDL();
    std::list<DoubleRect*>::iterator it;

    for (it = arr->begin(); it != arr->end(); ++it)
        SDL_LowerBlit(img->mSDLSurface, &(*it)->src, mTarget, &(*it)->dst);
//        SDL_BlitSurface(img->mSDLSurface, &(*it)->src, mTarget, &(*it)->dst);
}

bool Graphics::calcImageRect(GraphicsVertexes* vert,
                             int x, int y, int w, int h,
                             Image *topLeft, Image *topRight,
                             Image *bottomLeft, Image *bottomRight,
                             Image *top, Image *right,
                             Image *bottom, Image *left,
                             Image *center)
{
    if (!vert)
        return false;

    const bool drawMain = center && topLeft && topRight
        && bottomLeft && bottomRight;

    vert->init(x, y, w, h);
    pushClipArea(gcn::Rectangle(vert->mX, vert->mY, vert->mW, vert->mH));

    // Draw the center area
    if (center && drawMain)
    {
        calcImagePattern(vert, center,
            topLeft->getWidth(), topLeft->getHeight(),
            w - topLeft->getWidth() - topRight->getWidth(),
            h - topLeft->getHeight() - bottomLeft->getHeight());
    }
    else
    {
        vert->incPtr(1);
    }

    // Draw the sides
    if (top && left && bottom && right)
    {
        calcImagePattern(vert, top,
            left->getWidth(), 0,
            w - left->getWidth() - right->getWidth(), top->getHeight());
        calcImagePattern(vert, bottom,
            left->getWidth(), h - bottom->getHeight(),
            w - left->getWidth() - right->getWidth(),
            bottom->getHeight());
        calcImagePattern(vert, left,
            0, top->getHeight(),
            left->getWidth(),
            h - top->getHeight() - bottom->getHeight());
        calcImagePattern(vert, right,
            w - right->getWidth(), top->getHeight(),
            right->getWidth(),
            h - top->getHeight() - bottom->getHeight());
    }
    else
    {
        vert->incPtr(4);
    }

    popClipArea();
    return 0;
}

void Graphics::calcImagePattern(GraphicsVertexes* vert,
                                Image *image, int x, int y, int w, int h)
{
    // Check that preconditions for blitting are met.
    if (!vert || !mTarget || !image || !image->mSDLSurface || !vert->sdl)
    {
        vert->incPtr(1);
        return;
    }
    vert->clearSDL();

    const int iw = image->getWidth();
    const int ih = image->getHeight();

    if (iw == 0 || ih == 0)
    {
        vert->incPtr(1);
        return;
    }

    for (int py = 0; py < h; py += ih)     // Y position on pattern plane
    {
        int dh = (py + ih >= h) ? h - py : ih;
        int srcY = image->mBounds.y;
        int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw) // X position on pattern plane
        {
            int dw = (px + iw >= w) ? w - px : iw;
            int srcX = image->mBounds.x;
            int dstX = x + px + mClipStack.top().xOffset;

            SDL_Rect dstRect;
            SDL_Rect srcRect;
            dstRect.x = static_cast<short>(dstX);
            dstRect.y = static_cast<short>(dstY);
            srcRect.x = static_cast<short>(srcX);
            srcRect.y = static_cast<short>(srcY);
            srcRect.w = static_cast<Uint16>(dw);
            srcRect.h = static_cast<Uint16>(dh);

            if (SDL_FakeUpperBlit(image->mSDLSurface, &srcRect,
                mTarget, &dstRect) == 1)
            {
                vert->pushSDL(srcRect, dstRect);
            }
        }
    }
    vert->incPtr(1);
}

void Graphics::updateScreen()
{
    SDL_Flip(mTarget);
}

SDL_Surface *Graphics::getScreenshot()
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int rmask = 0xff000000;
    int gmask = 0x00ff0000;
    int bmask = 0x0000ff00;
#else
    int rmask = 0x000000ff;
    int gmask = 0x0000ff00;
    int bmask = 0x00ff0000;
#endif
    int amask = 0x00000000;

    SDL_Surface *screenshot = SDL_CreateRGBSurface(SDL_SWSURFACE, mTarget->w,
            mTarget->h, 24, rmask, gmask, bmask, amask);

    if (screenshot)
        SDL_BlitSurface(mTarget, NULL, screenshot, NULL);

    return screenshot;
}

bool Graphics::drawNet(int x1, int y1, int x2, int y2, int width, int height)
{
    for (int y = y1; y < y2; y += height)
        drawLine(x1, y, x2, y);

    for (int x = x1; x < x2; x += width)
        drawLine(x, y1, x, y2);

    return true;
}

bool Graphics::calcWindow(GraphicsVertexes* vert,
                          int x, int y, int w, int h,
                          const ImageRect &imgRect)
{
    return calcImageRect(vert, x, y, w, h,
        imgRect.grid[0], imgRect.grid[2], imgRect.grid[6], imgRect.grid[8],
        imgRect.grid[1], imgRect.grid[5], imgRect.grid[7], imgRect.grid[3],
        imgRect.grid[4]);
}

int Graphics::SDL_FakeUpperBlit (SDL_Surface *src, SDL_Rect *srcrect,
                                 SDL_Surface *dst, SDL_Rect *dstrect)
{
    SDL_Rect fulldst;
    int srcx, srcy, w, h;

    /* Make sure the surfaces aren't locked */
    if (!src || !dst)
        return(-1);

    if (src->locked || dst->locked)
        return(-1);

    /* If the destination rectangle is NULL, use the entire dest surface */
    if (dstrect == NULL)
    {
        fulldst.x = fulldst.y = 0;
        dstrect = &fulldst;
    }

    /* clip the source rectangle to the source surface */
    if (srcrect)
    {
        int maxw, maxh;

        srcx = srcrect->x;
        w = srcrect->w;
        if (srcx < 0)
        {
            w += srcx;
            dstrect->x -= srcx;
            srcx = 0;
        }
        maxw = src->w - srcx;
        if (maxw < w)
            w = maxw;

        srcy = srcrect->y;
        h = srcrect->h;
        if (srcy < 0)
        {
            h += srcy;
            dstrect->y -= srcy;
            srcy = 0;
        }
        maxh = src->h - srcy;
        if (maxh < h)
            h = maxh;
    }
    else
    {
        srcx = 0;
        srcy = 0;
        w = src->w;
        h = src->h;
    }

    /* clip the destination rectangle against the clip rectangle */
    {
        SDL_Rect *clip = &dst->clip_rect;
        int dx, dy;

        dx = clip->x - dstrect->x;
        if (dx > 0)
        {
            w -= dx;
            dstrect->x += dx;
            srcx += dx;
        }
        dx = dstrect->x + w - clip->x - clip->w;
        if (dx > 0)
            w -= dx;

        dy = clip->y - dstrect->y;
        if (dy > 0)
        {
            h -= dy;
            dstrect->y += dy;
            srcy += dy;
        }
        dy = dstrect->y + h - clip->y - clip->h;
        if (dy > 0)
            h -= dy;
    }

    if (w > 0 && h > 0)
    {
        srcrect->x = srcx;
        srcrect->y = srcy;
        srcrect->w = dstrect->w = w;
        srcrect->h = dstrect->h = h;

        return 1;
//        return SDL_LowerBlit(src, &sr, dst, dstrect);
    }
    dstrect->w = dstrect->h = 0;
    return 0;
}
