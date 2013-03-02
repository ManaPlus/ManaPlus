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

#include "graphics.h"

#include "main.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "resources/imagehelper.h"
#include "resources/openglimagehelper.h"

#include <guichan/sdl/sdlpixel.hpp>

#ifdef USE_OPENGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif
#endif

#include <SDL_gfxBlitFunc.h>

#include "debug.h"

#ifdef USE_OPENGL
#ifndef GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX
//#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX 0x9047
//#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX 0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
//#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX 0x904A
//#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX 0x904B
#endif
#endif

static unsigned int *cR = nullptr;
static unsigned int *cG = nullptr;
static unsigned int *cB = nullptr;

int MSDL_gfxBlitRGBA(SDL_Surface *src, SDL_Rect *srcrect,
                     SDL_Surface *dst, SDL_Rect *dstrect)
{
    return SDL_gfxBlitRGBA(src, srcrect, dst, dstrect);
}

Graphics::Graphics() :
    SDLGraphics(),
    mWidth(0),
    mHeight(0),
    mBpp(0),
    mFullscreen(false),
    mHWAccel(false),
    mBlitMode(BLIT_NORMAL),
    mRedraw(false),
    mDoubleBuffer(false),
    mSecure(false),
    mOpenGL(0),
    mEnableResize(false),
    mNoFrame(false),
    mOldPixel(0),
    mOldAlpha(0),
    mName("Software"),
    mStartFreeMem(0),
    mSync(false)
{
    mRect.x = 0;
    mRect.y = 0;
    mRect.w = 0;
    mRect.h = 0;
}

Graphics::~Graphics()
{
    _endDraw();
}

void Graphics::setSync(const bool sync)
{
    mSync = sync;
}

void Graphics::setMainFlags(const int w, const int h, const int bpp,
                            const bool fs, const bool hwaccel,
                            const bool resize, const bool noFrame)
{
    logger->log("graphics backend: %s", getName().c_str());
    logger->log("Setting video mode %dx%d %s",
            w, h, fs ? "fullscreen" : "windowed");

    mWidth = w;
    mHeight = h;
    mBpp = bpp;
    mFullscreen = fs;
    mHWAccel = hwaccel;
    mEnableResize = resize;
    mNoFrame = noFrame;
}

int Graphics::getOpenGLFlags() const
{
#ifdef USE_OPENGL
    int displayFlags = SDL_ANYFORMAT | SDL_OPENGL;

    if (mFullscreen)
    {
        displayFlags |= SDL_FULLSCREEN;
    }
    else
    {
        // Resizing currently not supported on Windows, where it would require
        // reuploading all textures.
#if !defined(_WIN32)
        if (mEnableResize)
            displayFlags |= SDL_RESIZABLE;
#endif
    }

    if (mNoFrame)
        displayFlags |= SDL_NOFRAME;

    return displayFlags;
#else
    return 0;
#endif
}

bool Graphics::setOpenGLMode()
{
#ifdef USE_OPENGL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    if (!(mTarget = SDL_SetVideoMode(mWidth, mHeight, mBpp, getOpenGLFlags())))
        return false;

#ifdef __APPLE__
    if (mSync)
    {
        const GLint VBL = 1;
        CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &VBL);
    }
#endif

    graphicsManager.setGLVersion();
    graphicsManager.logVersion();

    // Setup OpenGL
    glViewport(0, 0, mWidth, mHeight);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    int gotDoubleBuffer = 0;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &gotDoubleBuffer);
    logger->log("Using OpenGL %s double buffering.",
                (gotDoubleBuffer ? "with" : "without"));

    graphicsManager.initOpenGL();
    initArrays();
    graphicsManager.updateTextureFormat();
    updateMemoryInfo();

    GLint texSize;
    bool rectTex = graphicsManager.supportExtension(
        "GL_ARB_texture_rectangle");
    if (rectTex && OpenGLImageHelper::getInternalTextureType() == 4
        && getOpenGL() != 3 && config.getBoolValue("rectangulartextures")
        && !graphicsManager.isUseTextureSampler())
    {
        logger->log1("using GL_ARB_texture_rectangle");
        OpenGLImageHelper::mTextureType = GL_TEXTURE_RECTANGLE_ARB;
        glEnable(GL_TEXTURE_RECTANGLE_ARB);
        glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, &texSize);
        OpenGLImageHelper::mTextureSize = texSize;
        logger->log("OpenGL texture size: %d pixels (rectangle textures)",
            OpenGLImageHelper::mTextureSize);
    }
    else
    {
        OpenGLImageHelper::mTextureType = GL_TEXTURE_2D;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
        OpenGLImageHelper::mTextureSize = texSize;
        logger->log("OpenGL texture size: %d pixels",
            OpenGLImageHelper::mTextureSize);
    }
    return videoInfo();
#else
    return false;
#endif
}

int Graphics::getSoftwareFlags() const
{
    int displayFlags = SDL_ANYFORMAT;

    if (mFullscreen)
        displayFlags |= SDL_FULLSCREEN;
    else if (mEnableResize)
        displayFlags |= SDL_RESIZABLE;

    if (mHWAccel)
        displayFlags |= SDL_HWSURFACE | SDL_DOUBLEBUF;
    else
        displayFlags |= SDL_SWSURFACE;

    if (mNoFrame)
        displayFlags |= SDL_NOFRAME;

    return displayFlags;
}


void Graphics::updateMemoryInfo()
{
#ifdef USE_OPENGL
    if (mStartFreeMem)
        return;

    if (graphicsManager.supportExtension("GL_NVX_gpu_memory_info"))
    {
        glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
            &mStartFreeMem);
        logger->log("free video memory: %d", mStartFreeMem);
    }
#endif
}

int Graphics::getMemoryUsage() const
{
#ifdef USE_OPENGL
    if (!mStartFreeMem)
        return 0;

    if (graphicsManager.supportExtension("GL_NVX_gpu_memory_info"))
    {
        GLint val;
        glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
            &val);
        return mStartFreeMem - val;
    }
#endif
    return 0;
}

bool Graphics::setVideoMode(const int w, const int h, const int bpp,
                            const bool fs, const bool hwaccel,
                            const bool resize, const bool noFrame)
{
    setMainFlags(w, h, bpp, fs, hwaccel, resize, noFrame);

    if (!(mTarget = SDL_SetVideoMode(w, h, bpp, getSoftwareFlags())))
        return false;

    mRect.w = static_cast<uint16_t>(mTarget->w);
    mRect.h = static_cast<uint16_t>(mTarget->h);

    return videoInfo();
}

bool Graphics::videoInfo()
{
    char videoDriverName[65];

    logger->log("SDL video info");
    if (SDL_VideoDriverName(videoDriverName, 64))
        logger->log("Using video driver: %s", videoDriverName);
    else
        logger->log1("Using video driver: unknown");

    mDoubleBuffer = ((mTarget->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF);
    logger->log("Double buffer mode: %s", mDoubleBuffer ? "yes" : "no");

    imageHelper->dumpSurfaceFormat(mTarget);

    const SDL_VideoInfo *const vi = SDL_GetVideoInfo();
    if (!vi)
        return false;

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

    return true;
}

bool Graphics::setFullscreen(const bool fs)
{
    if (mFullscreen == fs)
        return true;

    return setVideoMode(mWidth, mHeight, mBpp, fs, mHWAccel,
        mEnableResize, mNoFrame);
}

bool Graphics::resizeScreen(const int width, const int height)
{
    if (mWidth == width && mHeight == height)
        return true;

    const int prevWidth = mWidth;
    const int prevHeight = mHeight;

    _endDraw();

    const bool success = setVideoMode(width, height, mBpp,
        mFullscreen, mHWAccel, mEnableResize, mNoFrame);

    // If it didn't work, try to restore the previous size. If that didn't
    // work either, bail out (but then we're in deep trouble).
    if (!success)
    {
        if (!setVideoMode(prevWidth, prevHeight, mBpp,
            mFullscreen, mHWAccel, mEnableResize, mNoFrame))
        {
            return false;
        }
    }

    _beginDraw();

    return success;
}

int Graphics::getWidth() const
{
    return mWidth;
}

int Graphics::getHeight() const
{
    return mHeight;
}

bool Graphics::drawImage(const Image *image, int x, int y)
{
    if (image)
    {
        return drawImage2(image, 0, 0, x, y,
            image->mBounds.w, image->mBounds.h, false);
    }
    else
    {
        return false;
    }
}

bool Graphics::drawRescaledImage(const Image *const image, int srcX, int srcY,
                                 int dstX, int dstY,
                                 const int width, const int height,
                                 const int desiredWidth,
                                 const int desiredHeight,
                                 const bool useColor A_UNUSED)
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    // Check that preconditions for blitting are met.
    if (!mTarget || !image)
        return false;
    if (!image->mSDLSurface)
        return false;

    Image *const tmpImage = image->SDLgetScaledImage(
        desiredWidth, desiredHeight);

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
    srcRect.w = static_cast<uint16_t>(width);
    srcRect.h = static_cast<uint16_t>(height);

    const bool returnValue = !(SDL_BlitSurface(tmpImage->mSDLSurface,
        &srcRect, mTarget, &dstRect) < 0);

    delete tmpImage;

    return returnValue;
}

bool Graphics::drawImage2(const Image *const image, int srcX, int srcY,
                          int dstX, int dstY, const int width,
                          const int height, const bool useColor A_UNUSED)
{
    FUNC_BLOCK("Graphics::drawImage2", 1)
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
    srcRect.w = static_cast<uint16_t>(width);
    srcRect.h = static_cast<uint16_t>(height);

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

void Graphics::drawImagePattern(const Image *const image,
                                const int x, const int y,
                                const int w, const int h)
{
    FUNC_BLOCK("Graphics::drawImagePattern", 1)
    // Check that preconditions for blitting are met.
    if (!mTarget || !image)
        return;
    if (!image->mSDLSurface)
        return;

    const int iw = image->mBounds.w;
    const int ih = image->mBounds.h;

    if (iw == 0 || ih == 0)
        return;

    for (int py = 0; py < h; py += ih)     // Y position on pattern plane
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int srcY = image->mBounds.y;
        const int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw) // X position on pattern plane
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int srcX = image->mBounds.x;
            const int dstX = x + px + mClipStack.top().xOffset;

            SDL_Rect dstRect;
            SDL_Rect srcRect;
            dstRect.x = static_cast<short>(dstX);
            dstRect.y = static_cast<short>(dstY);
            srcRect.x = static_cast<short>(srcX);
            srcRect.y = static_cast<short>(srcY);
            srcRect.w = static_cast<uint16_t>(dw);
            srcRect.h = static_cast<uint16_t>(dh);

            SDL_BlitSurface(image->mSDLSurface, &srcRect, mTarget, &dstRect);
        }
    }
}

void Graphics::drawRescaledImagePattern(const Image *const image,
                                        const int x, const int y,
                                        const int w, const int h,
                                        const int scaledWidth,
                                        const int scaledHeight)
{
    // Check that preconditions for blitting are met.
    if (!mTarget || !image)
        return;
    if (!image->mSDLSurface)
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

    for (int py = 0; py < h; py += ih)     // Y position on pattern plane
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int srcY = tmpImage->mBounds.y;
        const int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw) // X position on pattern plane
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int srcX = tmpImage->mBounds.x;
            const int dstX = x + px + mClipStack.top().xOffset;

            SDL_Rect dstRect;
            SDL_Rect srcRect;
            dstRect.x = static_cast<short>(dstX);
            dstRect.y = static_cast<short>(dstY);
            srcRect.x = static_cast<short>(srcX);
            srcRect.y = static_cast<short>(srcY);
            srcRect.w = static_cast<uint16_t>(dw);
            srcRect.h = static_cast<uint16_t>(dh);

            SDL_BlitSurface(tmpImage->mSDLSurface, &srcRect,
                            mTarget, &dstRect);
        }
    }

    delete tmpImage;
}

void Graphics::drawImageRect(const int x, const int y,
                             const int w, const int h,
                             const Image *const topLeft,
                             const Image *const topRight,
                             const Image *const bottomLeft,
                             const Image *const bottomRight,
                             const Image *const top,
                             const Image *const right,
                             const Image *const bottom,
                             const Image *const left,
                             const Image *const center)
{
    BLOCK_START("Graphics::drawImageRect")
    const bool drawMain = center && topLeft && topRight
        && bottomLeft && bottomRight;

    // Draw the center area
    if (center && drawMain)
    {
        const int tlw = topLeft->getWidth();
        const int tlh = topLeft->getHeight();
        drawImagePattern(center, tlw + x, tlh + y,
            w - tlw - topRight->getWidth(),
            h - tlh - bottomLeft->getHeight());
    }

    // Draw the sides
    if (top && left && bottom && right)
    {
        const int lw = left->getWidth();
        const int rw = right->getWidth();
        const int th = top->getHeight();
        const int bh = bottom->getHeight();
        drawImagePattern(top, x + lw, y, w - lw - rw, th);
        drawImagePattern(bottom, x + lw, h - bh + y, w - lw - rw, bh);
        drawImagePattern(left, x, y + th, lw, h - th - bh);
        drawImagePattern(right, x + w - rw, th + y, rw, h - th - bh);
    }
    // Draw the corners
    if (drawMain)
    {
        drawImage(topLeft, x, y);
        drawImage(topRight, x + w - topRight->getWidth(), y);
        drawImage(bottomLeft, x, h - bottomLeft->getHeight() + y);
        drawImage(bottomRight,
            x + w - bottomRight->getWidth(),
            y + h - bottomRight->getHeight());
    }
    BLOCK_END("Graphics::drawImageRect")
}

void Graphics::drawImageRect(int x, int y, int w, int h,
                             const ImageRect &imgRect)
{
    drawImageRect(x, y, w, h,
            imgRect.grid[0], imgRect.grid[2], imgRect.grid[6], imgRect.grid[8],
            imgRect.grid[1], imgRect.grid[5], imgRect.grid[7], imgRect.grid[3],
            imgRect.grid[4]);
}

bool Graphics::calcImageRect(ImageVertexes *const vert,
                             const int x, const int y,
                             const int w, const int h,
                             const Image *const topLeft,
                             const Image *const topRight,
                             const Image *const bottomLeft,
                             const Image *const bottomRight,
                             const Image *const top,
                             const Image *const right,
                             const Image *const bottom,
                             const Image *const left,
                             const Image *const center)
{
    if (!vert)
        return false;

    BLOCK_START("Graphics::calcImageRect")
    const bool drawMain = center && topLeft && topRight
        && bottomLeft && bottomRight;

//    pushClipArea(gcn::Rectangle(x, y, w, h));

    // Draw the center area
    if (center && drawMain)
    {
        const int tlw = topLeft->getWidth();
        const int tlh = topLeft->getHeight();
        calcImagePattern(vert, center, tlw + x, tlh + y,
            w - tlw - topRight->getWidth(),
            h - tlh - bottomLeft->getHeight());
    }
    // Draw the sides
    if (top && left && bottom && right)
    {
        const int lw = left->getWidth();
        const int rw = right->getWidth();
        const int th = top->getHeight();
        const int bh = bottom->getHeight();
        calcImagePattern(vert, top, x + lw, y, w - lw - rw, th);
        calcImagePattern(vert, bottom, x + lw, y + h - bh, w - lw - rw, bh);
        calcImagePattern(vert, left, x, y + th, lw, h - th - bh);
        calcImagePattern(vert, right, x + w - rw, y + th, rw, h - th - bh);
    }

    calcTile(vert, topLeft, x, y);
    if (topRight)
        calcTile(vert, topRight, x + w - topRight->getWidth(), y);
    if (bottomLeft)
        calcTile(vert, bottomLeft, x, y + h - bottomLeft->getHeight());
    if (bottomRight)
    {
        calcTile(vert, bottomRight, x + w - bottomRight->getWidth(),
            y + h - bottomRight->getHeight());
    }

//    popClipArea();
    BLOCK_END("Graphics::calcImageRect")
    return 0;
}

void Graphics::calcImagePattern(ImageVertexes* const vert,
                                const Image *const image,
                                const int x, const int y,
                                const int w, const int h) const
{
    // Check that preconditions for blitting are met.
    if (!vert || !mTarget || !image || !image->mSDLSurface)
        return;

    const int iw = image->mBounds.w;
    const int ih = image->mBounds.h;

    if (iw == 0 || ih == 0)
        return;

    for (int py = 0; py < h; py += ih)     // Y position on pattern plane
    {
        const int dh = (py + ih >= h) ? h - py : ih;
        const int srcY = image->mBounds.y;
        const int dstY = y + py + mClipStack.top().yOffset;

        for (int px = 0; px < w; px += iw) // X position on pattern plane
        {
            const int dw = (px + iw >= w) ? w - px : iw;
            const int srcX = image->mBounds.x;
            const int dstX = x + px + mClipStack.top().xOffset;

            DoubleRect *const r = new DoubleRect();
            SDL_Rect &dstRect = r->dst;
            SDL_Rect &srcRect = r->src;
            dstRect.x = static_cast<short>(dstX);
            dstRect.y = static_cast<short>(dstY);
            srcRect.x = static_cast<short>(srcX);
            srcRect.y = static_cast<short>(srcY);
            srcRect.w = static_cast<uint16_t>(dw);
            srcRect.h = static_cast<uint16_t>(dh);

            if (SDL_FakeUpperBlit(image->mSDLSurface, &srcRect,
                mTarget, &dstRect) == 1)
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

void Graphics::calcImagePattern(ImageCollection* const vertCol,
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

void Graphics::calcTile(ImageVertexes *const vert,
                        const Image *const image,
                        int x, int y) const
{
    vert->image = image;
    calcTile(vert, x, y);
}

void Graphics::calcTile(ImageCollection *const vertCol,
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
        calcTile(vert, x, y);
    }
    else
    {
        calcTile(vertCol->currentVert, x, y);
    }
}

void Graphics::drawTile(const ImageCollection *const vertCol)
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
                mTarget, &(*it2)->dst);
            ++ it2;
        }
    }
}

void Graphics::calcTile(ImageVertexes *const vert, int x, int y) const
{
    // Check that preconditions for blitting are met.
    if (!vert || !vert->image || !vert->image->mSDLSurface)
        return;

    const Image *const image = vert->image;

    x += mClipStack.top().xOffset;
    y += mClipStack.top().yOffset;

    DoubleRect *rect = new DoubleRect();

    rect->dst.x = static_cast<short>(x);
    rect->dst.y = static_cast<short>(y);
    rect->src.x = static_cast<short>(image->mBounds.x);
    rect->src.y = static_cast<short>(image->mBounds.y);
    rect->src.w = static_cast<uint16_t>(image->mBounds.w);
    rect->src.h = static_cast<uint16_t>(image->mBounds.h);
    if (SDL_FakeUpperBlit(image->mSDLSurface, &rect->src,
        mTarget, &rect->dst) == 1)
    {
        vert->sdl.push_back(rect);
    }
    else
    {
        delete rect;
    }
}

void Graphics::drawTile(const ImageVertexes *const vert)
{
    // vert and img must be != 0
    const Image *const img = vert->image;
    const DoubleRects *const rects = &vert->sdl;
    DoubleRects::const_iterator it = rects->begin();
    const DoubleRects::const_iterator it_end = rects->end();
    while (it != it_end)
    {
        SDL_LowerBlit(img->mSDLSurface, &(*it)->src, mTarget, &(*it)->dst);
        ++ it;
    }
}

void Graphics::updateScreen()
{
    BLOCK_START("Graphics::updateScreen")
    if (mDoubleBuffer)
    {
        SDL_Flip(mTarget);
    }
    else
    {
        SDL_UpdateRects(mTarget, 1, &mRect);
//        SDL_UpdateRect(mTarget, 0, 0, 0, 0);
    }
    BLOCK_END("Graphics::updateScreen")
}

SDL_Surface *Graphics::getScreenshot()
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

    SDL_Surface *const screenshot = SDL_CreateRGBSurface(SDL_SWSURFACE,
        mTarget->w, mTarget->h, 24, rmask, gmask, bmask, amask);

    if (screenshot)
        SDL_BlitSurface(mTarget, nullptr, screenshot, nullptr);

    return screenshot;
}

bool Graphics::drawNet(const int x1, const int y1, const int x2, const int y2,
                       const int width, const int height)
{
    for (int y = y1; y < y2; y += height)
        drawLine(x1, y, x2, y);

    for (int x = x1; x < x2; x += width)
        drawLine(x, y1, x, y2);

    return true;
}

bool Graphics::calcWindow(ImageCollection *const vertCol,
                          const int x, const int y, const int w, const int h,
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
//        calcTile(vert, x, y);
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

int Graphics::SDL_FakeUpperBlit(const SDL_Surface *const src,
                                SDL_Rect *const srcrect,
                                const SDL_Surface *const dst,
                                SDL_Rect *dstrect) const
{
    SDL_Rect fulldst;
    int srcx, srcy, w, h;

    /* Make sure the surfaces aren't locked */
    if (!src || !dst)
        return -1;

    if (src->locked || dst->locked)
        return -1;

    /* If the destination rectangle is nullptr, use the entire dest surface */
    if (!dstrect)
    {
        fulldst.x = 0;
        fulldst.y = 0;
        dstrect = &fulldst;
    }

    /* clip the source rectangle to the source surface */
    if (srcrect)
    {
        srcx = srcrect->x;
        w = srcrect->w;
        if (srcx < 0)
        {
            w += srcx;
            dstrect->x -= static_cast<int16_t>(srcx);
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
            dstrect->y -= static_cast<int16_t>(srcy);
            srcy = 0;
        }
        int maxh = src->h - srcy;
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
        const SDL_Rect *const clip = &dst->clip_rect;
        int dx = clip->x - dstrect->x;
        if (dx > 0)
        {
            w -= dx;
            dstrect->x += static_cast<int16_t>(dx);
            srcx += dx;
        }
        dx = dstrect->x + w - clip->x - clip->w;
        if (dx > 0)
            w -= dx;

        int dy = clip->y - dstrect->y;
        if (dy > 0)
        {
            h -= dy;
            dstrect->y += static_cast<int16_t>(dy);
            srcy += dy;
        }
        dy = dstrect->y + h - clip->y - clip->h;
        if (dy > 0)
            h -= dy;
    }

    if (w > 0 && h > 0)
    {
        if (srcrect)
        {
            srcrect->x = static_cast<int16_t>(srcx);
            srcrect->y = static_cast<int16_t>(srcy);
            srcrect->w = static_cast<int16_t>(w);
            srcrect->h = static_cast<int16_t>(h);
        }
        dstrect->w = static_cast<int16_t>(w);
        dstrect->h = static_cast<int16_t>(h);

        return 1;
//        return SDL_LowerBlit(src, &sr, dst, dstrect);
    }
    dstrect->w = dstrect->h = 0;
    return 0;
}

void Graphics::fillRectangle(const gcn::Rectangle& rectangle)
{
    FUNC_BLOCK("Graphics::fillRectangle", 1)
    if (mClipStack.empty())
        return;

    const gcn::ClipRectangle& top = mClipStack.top();

    gcn::Rectangle area = rectangle;
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
        int x, y;

        SDL_LockSurface(mTarget);

        const int bpp = mTarget->format->BytesPerPixel;
        const uint32_t pixel = SDL_MapRGB(mTarget->format,
            static_cast<uint8_t>(mColor.r), static_cast<uint8_t>(mColor.g),
            static_cast<uint8_t>(mColor.b));

        switch (bpp)
        {
            case 1:
                for (y = y1; y < y2; y++)
                {
                    uint8_t *const p = static_cast<uint8_t *>(mTarget->pixels)
                        + y * mTarget->pitch;
                    for (x = x1; x < x2; x++)
                        *(p + x) = static_cast<uint8_t>(pixel);
                }
                break;
            case 2:
                for (y = y1; y < y2; y++)
                {
                    uint8_t *const p0 = static_cast<uint8_t *>(mTarget->pixels)
                        + y * mTarget->pitch;
                    for (x = x1; x < x2; x++)
                    {
                        uint8_t *const p = p0 + x * 2;
                        *reinterpret_cast<uint16_t *>(p) = gcn::SDLAlpha16(
                            static_cast<uint16_t>(pixel),
                            *reinterpret_cast<uint16_t *>(p),
                            static_cast<uint8_t>(mColor.a), mTarget->format);
                    }
                }
                break;
            case 3:
            {
                const int ca = 255 - mColor.a;
                const int cr = mColor.r * mColor.a;
                const int cg = mColor.g * mColor.a;
                const int cb = mColor.b * mColor.a;

                for (y = y1; y < y2; y++)
                {
                    uint8_t *const p0 = static_cast<uint8_t *>(mTarget->pixels)
                        + y * mTarget->pitch;
                    for (x = x1; x < x2; x++)
                    {
                        uint8_t *const p = p0 + x * 3;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                        p[2] = static_cast<uint8_t>((p[2] * ca + cb) >> 8);
                        p[1] = static_cast<uint8_t>((p[1] * ca + cg) >> 8);
                        p[0] = static_cast<uint8_t>((p[0] * ca + cr) >> 8);
#else
                        p[0] = static_cast<uint8_t>((p[0] * ca + cb) >> 8);
                        p[1] = static_cast<uint8_t>((p[1] * ca + cg) >> 8);
                        p[2] = static_cast<uint8_t>((p[2] * ca + cr) >> 8);
#endif
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

                for (y = y1; y < y2; y++)
                {
                    uint8_t *p0 = static_cast<uint8_t *>(mTarget->pixels)
                        + y * mTarget->pitch;
                    for (x = x1; x < x2; x++)
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
#else
                if (!cR)
                {
                    cR = new unsigned int[0x100];
                    cG = new unsigned int[0x100];
                    cB = new unsigned int[0x100];
                    mOldPixel = 0;
                    mOldAlpha = mColor.a;
                }

                const SDL_PixelFormat * const format = mTarget->format;
                const unsigned rMask = format->Rmask;
                const unsigned gMask = format->Gmask;
                const unsigned bMask = format->Bmask;
//                const unsigned aMask = format->Amask;
                unsigned rShift = rMask / 0xff;
                unsigned gShift = gMask / 0xff;
                unsigned bShift = bMask / 0xff;
                if (!rShift)
                    rShift = 1;
                if (!gShift)
                    gShift = 1;
                if (!bShift)
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

                for (y = y1; y < y2; y++)
                {
                    uint32_t *const p0 = reinterpret_cast<uint32_t*>(
                        static_cast<uint8_t*>(mTarget->pixels)
                        + y * mTarget->pitch);
                    for (x = x1; x < x2; x++)
                    {
                        uint32_t *const p = p0 + x;
                        const uint32_t dst = *p;
                        *p = cB[dst & bMask / bShift]
                            | cG[(dst & gMask) / gShift]
                            | cR[(dst & rMask) / rShift];
                    }
                }
#endif
                break;
            }
            default:
                break;
        }

        SDL_UnlockSurface(mTarget);
    }
    else
    {
        SDL_Rect rect;
        rect.x = static_cast<int16_t>(area.x);
        rect.y = static_cast<int16_t>(area.y);
        rect.w = static_cast<uint16_t>(area.width);
        rect.h = static_cast<uint16_t>(area.height);

        const uint32_t color = SDL_MapRGBA(mTarget->format,
            static_cast<int8_t>(mColor.r),
            static_cast<int8_t>(mColor.g),
            static_cast<int8_t>(mColor.b),
            static_cast<int8_t>(mColor.a));
        SDL_FillRect(mTarget, &rect, color);
    }
}
