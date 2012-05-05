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

#include "graphics.h"

#include "main.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "resources/image.h"
#include "utils/stringutils.h"

#include <guichan/sdl/sdlpixel.hpp>

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

Graphics::Graphics() :
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
    mStartFreeMem(0)
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

void Graphics::setMainFlags(int w, int h, int bpp, bool fs,
                            bool hwaccel, bool resize, bool noFrame)
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

int Graphics::getOpenGLFlags()
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

    graphicsManager.logString("gl vendor: %s", GL_VENDOR);
    graphicsManager.logString("gl renderer: %s", GL_RENDERER);
    graphicsManager.logString("gl version: %s", GL_VERSION);

    // Setup OpenGL
    glViewport(0, 0, mWidth, mHeight);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    int gotDoubleBuffer;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &gotDoubleBuffer);
    logger->log("Using OpenGL %s double buffering.",
                (gotDoubleBuffer ? "with" : "without"));

    char const *glExtensions = reinterpret_cast<char const *>(
        glGetString(GL_EXTENSIONS));

    logger->log1("opengl extensions: ");
    logger->log1(glExtensions);

    graphicsManager.updateExtensions(glExtensions);

    graphicsManager.updateTextureFormat();
    updateMemoryInfo();

    GLint texSize;
    bool rectTex = graphicsManager.supportExtension(
        "GL_ARB_texture_rectangle");
    if (rectTex && Image::getInternalTextureType() == 4
        && config.getBoolValue("rectangulartextures"))
    {
        logger->log1("using GL_ARB_texture_rectangle");
        Image::mTextureType = GL_TEXTURE_RECTANGLE_ARB;
        glEnable(GL_TEXTURE_RECTANGLE_ARB);
        glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, &texSize);
        Image::mTextureSize = texSize;
        logger->log("OpenGL texture size: %d pixels (rectangle textures)",
            Image::mTextureSize);
    }
    else
    {
        Image::mTextureType = GL_TEXTURE_2D;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
        Image::mTextureSize = texSize;
        logger->log("OpenGL texture size: %d pixels", Image::mTextureSize);
    }
    return videoInfo();
#else
    return false;
#endif
}

int Graphics::getSoftwareFlags()
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

int Graphics::getMemoryUsage()
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

bool Graphics::setVideoMode(int w, int h, int bpp, bool fs,
                            bool hwaccel, bool resize, bool noFrame)
{
    setMainFlags(w, h, bpp, fs, hwaccel, resize, noFrame);

    if (!(mTarget = SDL_SetVideoMode(w, h, bpp, getSoftwareFlags())))
        return false;

    mRect.w = mTarget->w;
    mRect.h = mTarget->h;

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

    if (mTarget->format)
    {
        logger->log("Bits per pixel: %d", mTarget->format->BytesPerPixel);
//        bpp = mTarget->format->BytesPerPixel;
    }

    const SDL_VideoInfo *vi = SDL_GetVideoInfo();
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
    logger->log("Available video memory: %d", vi->video_mem);

    return true;
}

bool Graphics::setFullscreen(bool fs)
{
    if (mFullscreen == fs)
        return true;

    return setVideoMode(mWidth, mHeight, mBpp, fs, mHWAccel,
        mEnableResize, mNoFrame);
}

bool Graphics::resizeScreen(int width, int height)
{
    if (mWidth == width && mHeight == height)
        return true;

    const int prevWidth = mWidth;
    const int prevHeight = mHeight;

    _endDraw();

    bool success = setVideoMode(width, height, mBpp,
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
        return drawImage(image, 0, 0, x, y,
            image->mBounds.w, image->mBounds.h, false);
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
                                 bool useColor A_UNUSED)
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

bool Graphics::drawImage(const Image *image, int srcX, int srcY, int dstX, int dstY,
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

void Graphics::drawImagePattern(const Image *image, int x, int y, int w, int h)
{
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

    const int iw = tmpImage->mBounds.w;
    const int ih = tmpImage->mBounds.h;

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
    const bool drawMain = center && topLeft && topRight
        && bottomLeft && bottomRight;

    // Draw the center area
    if (center && drawMain)
    {
        drawImagePattern(center,
            topLeft->getWidth() + x, topLeft->getHeight() + y,
            w - topLeft->getWidth() - topRight->getWidth(),
            h - topLeft->getHeight() - bottomLeft->getHeight());
    }

    // Draw the sides
    if (top && left && bottom && right)
    {
        drawImagePattern(top,
            x + left->getWidth(), y,
            w - left->getWidth() - right->getWidth(), top->getHeight());
        drawImagePattern(bottom,
            x + left->getWidth(), h - bottom->getHeight() + y,
            w - left->getWidth() - right->getWidth(),
            bottom->getHeight());
        drawImagePattern(left,
            x, y + top->getHeight(),
            left->getWidth(),
            h - top->getHeight() - bottom->getHeight());
        drawImagePattern(right,
            x + w - right->getWidth(), top->getHeight() + y,
            right->getWidth(),
            h - top->getHeight() - bottom->getHeight());
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

void Graphics::drawImagePattern2(GraphicsVertexes *vert, const Image *img)
{
    // here not checking input parameters

    std::vector<DoubleRect*> *arr = vert->getRectsSDL();

    for (std::vector<DoubleRect*>::const_iterator it = arr->begin(),
         it_end = arr->end(); it != it_end; ++it)
    {
        SDL_LowerBlit(img->mSDLSurface, &(*it)->src, mTarget, &(*it)->dst);
//        SDL_BlitSurface(img->mSDLSurface, &(*it)->src, mTarget, &(*it)->dst);
    }
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

    const int iw = image->mBounds.w;
    const int ih = image->mBounds.h;

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

void Graphics::calcTile(ImageVertexes *vert, int x, int y)
{
    // Check that preconditions for blitting are met.
    if (!vert || !vert->image || !vert->image->mSDLSurface)
        return;

    const Image *image = vert->image;

    x += mClipStack.top().xOffset;
    y += mClipStack.top().yOffset;

    DoubleRect *rect = new DoubleRect();

    rect->dst.x = static_cast<short>(x);
    rect->dst.y = static_cast<short>(y);
    rect->src.x = static_cast<short>(image->mBounds.x);
    rect->src.y = static_cast<short>(image->mBounds.y);
    rect->src.w = static_cast<Uint16>(image->mBounds.w);
    rect->src.h = static_cast<Uint16>(image->mBounds.h);
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

void Graphics::drawTile(ImageVertexes *vert)
{
    // vert and img must be != 0
    Image *img = vert->image;
    DoubleRects *rects = &vert->sdl;
    DoubleRects::const_iterator it = rects->begin();
    DoubleRects::const_iterator it_end = rects->end();
    while (it != it_end)
    {
        SDL_LowerBlit(img->mSDLSurface, &(*it)->src, mTarget, &(*it)->dst);
        ++ it;
    }
}

void Graphics::updateScreen()
{
    if (mDoubleBuffer)
    {
        SDL_Flip(mTarget);
    }
    else
    {
        SDL_UpdateRects(mTarget, 1, &mRect);
//        SDL_UpdateRect(mTarget, 0, 0, 0, 0);
    }
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
        SDL_BlitSurface(mTarget, nullptr, screenshot, nullptr);

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

int Graphics::SDL_FakeUpperBlit(SDL_Surface *src, SDL_Rect *srcrect,
                                SDL_Surface *dst, SDL_Rect *dstrect)
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

void Graphics::fillRectangle(const gcn::Rectangle& rectangle)
{
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
        int x1 = area.x > top.x ? area.x : top.x;
        int y1 = area.y > top.y ? area.y : top.y;
        int x2 = area.x + area.width < top.x + top.width ?
            area.x + area.width : top.x + top.width;
        int y2 = area.y + area.height < top.y + top.height ?
            area.y + area.height : top.y + top.height;
        int x, y;

        SDL_LockSurface(mTarget);

        const int bpp = mTarget->format->BytesPerPixel;
        Uint32 pixel = SDL_MapRGB(mTarget->format,
            mColor.r, mColor.g, mColor.b);

        switch (bpp)
        {
            case 1:
                for (y = y1; y < y2; y++)
                {
                    Uint8 *p = static_cast<Uint8 *>(mTarget->pixels)
                        + y * mTarget->pitch;
                    for (x = x1; x < x2; x++)
                        *(p + x) = pixel;
                }
                break;
            case 2:
                for (y = y1; y < y2; y++)
                {
                    Uint8 *p0 = static_cast<Uint8 *>(mTarget->pixels)
                        + y * mTarget->pitch;
                    for (x = x1; x < x2; x++)
                    {
                        Uint8 *p = p0 + x * 2;
                        *reinterpret_cast<Uint16 *>(p) = gcn::SDLAlpha16(
                            pixel, *reinterpret_cast<Uint32 *>(p),
                            mColor.a, mTarget->format);
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
                    Uint8 *p0 = static_cast<Uint8 *>(mTarget->pixels)
                        + y * mTarget->pitch;
                    for (x = x1; x < x2; x++)
                    {
                        Uint8 *p = p0 + x * 3;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                        p[2] = (p[2] * ca + cb) >> 8;
                        p[1] = (p[1] * ca + cg) >> 8;
                        p[0] = (p[0] * ca + cr) >> 8;
#else
                        p[0] = (p[0] * ca + cb) >> 8;
                        p[1] = (p[1] * ca + cg) >> 8;
                        p[2] = (p[2] * ca + cr) >> 8;
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
                    Uint8 *p0 = static_cast<Uint8 *>(mTarget->pixels)
                        + y * mTarget->pitch;
                    for (x = x1; x < x2; x++)
                    {
                        Uint8 *p = p0 + x * 4;
                        Uint32 dst = *reinterpret_cast<Uint32 *>(p);
                        const unsigned int b = (pb + (dst & 0xff) * a1) >> 8;
                        const unsigned int g = (pg + (dst & 0xff00) * a1) >> 8;
                        const unsigned int r = (pr
                            + (dst & 0xff0000) * a1) >> 8;

                        *reinterpret_cast<Uint32 *>(p) = ((b & 0xff)
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

                if (pixel != mOldPixel || mColor.a != mOldAlpha)
                {
                    const unsigned pb = (pixel & 0xff) * mColor.a;
                    const unsigned pg = (pixel & 0xff00) * mColor.a;
                    const unsigned pr = (pixel & 0xff0000) * mColor.a;
                    const unsigned a1 = (255 - mColor.a);

                    const unsigned int a2 = a1 * 0xff;
                    const unsigned int a3 = a1 * 0xff00;

                    for (int f = 0; f <= 0xff; f ++)
                    {
                        cB[f] = ((pb + f * a1) >> 8) & 0xff;
                        cG[f] = ((pg + f * a2) >> 8) & 0xff00;
                        cR[f] = ((pr + f * a3) >> 8) & 0xff0000;
                    }

                    mOldPixel = pixel;
                    mOldAlpha = mColor.a;
                }

                for (y = y1; y < y2; y++)
                {
                    Uint32 *p0 = reinterpret_cast<Uint32*>(static_cast<Uint8*>(
                        mTarget->pixels) + y * mTarget->pitch);
                    for (x = x1; x < x2; x++)
                    {
                        Uint32 *p = p0 + x;
                        const Uint32 dst = *p;
                        *p = cB[dst & 0xff] | cG[(dst & 0xff00) >> 8]
                            | cR[(dst & 0xff0000) >> 16];
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
        rect.x = area.x;
        rect.y = area.y;
        rect.w = area.width;
        rect.h = area.height;

        Uint32 color = SDL_MapRGBA(mTarget->format,
            mColor.r, mColor.g, mColor.b, mColor.a);
        SDL_FillRect(mTarget, &rect, color);
    }
}
