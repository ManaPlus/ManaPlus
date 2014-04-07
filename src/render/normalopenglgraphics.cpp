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

#include "main.h"
#if defined USE_OPENGL && !defined ANDROID

#include "render/normalopenglgraphics.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "render/mgl.h"

#include "resources/image.h"
#include "resources/openglimagehelper.h"

#include "utils/sdlcheckutils.h"

#include "debug.h"

#define vertFill2D(tVar, vVar, x1, y1, x2, y2, dstX, dstY, w, h) \
    tVar[vp + 0] = x1; \
    tVar[vp + 1] = y1; \
    tVar[vp + 2] = x2; \
    tVar[vp + 3] = y1; \
    tVar[vp + 4] = x2; \
    tVar[vp + 5] = y2; \
    tVar[vp + 6] = x1; \
    tVar[vp + 7] = y2; \
    vVar[vp + 0] = dstX; \
    vVar[vp + 1] = dstY; \
    vVar[vp + 2] = dstX + w; \
    vVar[vp + 3] = dstY; \
    vVar[vp + 4] = dstX + w; \
    vVar[vp + 5] = dstY + h; \
    vVar[vp + 6] = dstX; \
    vVar[vp + 7] = dstY + h;


#define vertFillNv(tVar, vVar, srcX, srcY, dstX, dstY, w, h) \
    tVar[vp + 0] = srcX; \
    tVar[vp + 1] = srcY; \
    tVar[vp + 2] = srcX + w; \
    tVar[vp + 3] = srcY; \
    tVar[vp + 4] = srcX + w; \
    tVar[vp + 5] = srcY + h; \
    tVar[vp + 6] = srcX; \
    tVar[vp + 7] = srcY + h; \
    vVar[vp + 0] = dstX; \
    vVar[vp + 1] = dstY; \
    vVar[vp + 2] = dstX + w; \
    vVar[vp + 3] = dstY; \
    vVar[vp + 4] = dstX + w; \
    vVar[vp + 5] = dstY + h; \
    vVar[vp + 6] = dstX; \
    vVar[vp + 7] = dstY + h;

GLuint NormalOpenGLGraphics::mLastImage = 0;
#ifdef DEBUG_DRAW_CALLS
unsigned int NormalOpenGLGraphics::mDrawCalls = 0;
unsigned int NormalOpenGLGraphics::mLastDrawCalls = 0;
#endif
#ifdef DEBUG_BIND_TEXTURE
unsigned int NormalOpenGLGraphics::mBinds = 0;
unsigned int NormalOpenGLGraphics::mLastBinds = 0;
#endif

NormalOpenGLGraphics::NormalOpenGLGraphics():
    mFloatTexArray(nullptr),
    mIntTexArray(nullptr),
    mIntVertArray(nullptr),
    mFloatTexArrayCached(nullptr),
    mIntTexArrayCached(nullptr),
    mIntVertArrayCached(nullptr),
    mAlphaCached(1.0F),
    mVpCached(0),
    mTexture(false),
    mIsByteColor(false),
    mByteColor(),
    mImageCached(0),
    mFloatColor(1.0F),
    mMaxVertices(500),
    mColorAlpha(false),
#ifdef DEBUG_BIND_TEXTURE
    mOldTexture(),
    mOldTextureId(0),
#endif
    mFbo()
{
    mOpenGL = RENDER_NORMAL_OPENGL;
    mName = "normal OpenGL";
}

NormalOpenGLGraphics::~NormalOpenGLGraphics()
{
    delete [] mFloatTexArray;
    delete [] mIntTexArray;
    delete [] mIntVertArray;
    delete [] mFloatTexArrayCached;
    delete [] mIntTexArrayCached;
    delete [] mIntVertArrayCached;
}

void NormalOpenGLGraphics::initArrays()
{
    mMaxVertices = graphicsManager.getMaxVertices();
    if (mMaxVertices < 500)
        mMaxVertices = 500;
    else if (mMaxVertices > 1024)
        mMaxVertices = 1024;

    // need alocate small size, after if limit reached reallocate to double size
    vertexBufSize = mMaxVertices;
    const int sz = mMaxVertices * 4 + 30;
    if (!mFloatTexArray)
        mFloatTexArray = new GLfloat[sz];
    if (!mIntTexArray)
        mIntTexArray = new GLint[sz];
    if (!mIntVertArray)
        mIntVertArray = new GLint[sz];
    if (!mFloatTexArrayCached)
        mFloatTexArrayCached = new GLfloat[sz];
    if (!mIntTexArrayCached)
        mIntTexArrayCached = new GLint[sz];
    if (!mIntVertArrayCached)
        mIntVertArrayCached = new GLint[sz];
}

bool NormalOpenGLGraphics::setVideoMode(const int w, const int h,
                                        const int scale,
                                        const int bpp,
                                        const bool fs,
                                        const bool hwaccel,
                                        const bool resize,
                                        const bool noFrame)
{
    setMainFlags(w, h, scale, bpp, fs, hwaccel, resize, noFrame);

    return setOpenGLMode();
}

static inline void drawQuad(const Image *const image,
                            const int srcX, const int srcY,
                            const int dstX, const int dstY,
                            const int width, const int height)
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float tw = static_cast<float>(image->mTexWidth);
        const float th = static_cast<float>(image->mTexHeight);
        // Find OpenGL normalized texture coordinates.
        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;
        const float texX2 = static_cast<float>(srcX + width) / tw;
        const float texY2 = static_cast<float>(srcY + height) / th;

        GLfloat tex[] =
        {
            texX1, texY1,
            texX2, texY1,
            texX2, texY2,
            texX1, texY2
        };

        GLint vert[] =
        {
            dstX, dstY,
            dstX + width, dstY,
            dstX + width, dstY + height,
            dstX, dstY + height
        };

        glVertexPointer(2, GL_INT, 0, &vert);
        glTexCoordPointer(2, GL_FLOAT, 0, &tex);

#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif
        glDrawArrays(GL_QUADS, 0, 4);
    }
    else
    {
        GLint tex[] =
        {
            srcX, srcY,
            srcX + width, srcY,
            srcX + width, srcY + height,
            srcX, srcY + height
        };
        GLint vert[] =
        {
            dstX, dstY,
            dstX + width, dstY,
            dstX + width, dstY + height,
            dstX, dstY + height
        };

        glVertexPointer(2, GL_INT, 0, &vert);
        glTexCoordPointer(2, GL_INT, 0, &tex);

#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif
        glDrawArrays(GL_QUADS, 0, 4);
    }
}

static inline void drawRescaledQuad(const Image *const image,
                                    const int srcX, const int srcY,
                                    const int dstX, const int dstY,
                                    const int width, const int height,
                                    const int desiredWidth,
                                    const int desiredHeight)
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float tw = static_cast<float>(image->mTexWidth);
        const float th = static_cast<float>(image->mTexHeight);
        // Find OpenGL normalized texture coordinates.
        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;
        const float texX2 = static_cast<float>(srcX + width) / tw;
        const float texY2 = static_cast<float>(srcY + height) / th;

        GLfloat tex[] =
        {
            texX1, texY1,
            texX2, texY1,
            texX2, texY2,
            texX1, texY2
        };

        GLint vert[] =
        {
            dstX, dstY,
            dstX + desiredWidth, dstY,
            dstX + desiredWidth, dstY + desiredHeight,
            dstX, dstY + desiredHeight
        };

        glVertexPointer(2, GL_INT, 0, &vert);
        glTexCoordPointer(2, GL_FLOAT, 0, &tex);

#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif
        glDrawArrays(GL_QUADS, 0, 4);
    }
    else
    {
        GLint tex[] =
        {
            srcX, srcY,
            srcX + width, srcY,
            srcX + width, srcY + height,
            srcX, srcY + height
        };
        GLint vert[] =
        {
            dstX, dstY,
            dstX + desiredWidth, dstY,
            dstX + desiredWidth, dstY + desiredHeight,
            dstX, dstY + desiredHeight
        };

        glVertexPointer(2, GL_INT, 0, &vert);
        glTexCoordPointer(2, GL_INT, 0, &tex);

#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif
        glDrawArrays(GL_QUADS, 0, 4);
    }
}

bool NormalOpenGLGraphics::drawImage(const Image *const image,
                                     int dstX, int dstY)
{
    return drawImageInline(image, dstX, dstY);
}

bool NormalOpenGLGraphics::drawImageInline(const Image *const image,
                                           int dstX, int dstY)
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (!image)
        return false;


    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    const SDL_Rect &imageRect = image->mBounds;
    drawQuad(image, imageRect.x, imageRect.y,
        dstX, dstY, imageRect.w, imageRect.h);

    return true;
}

void NormalOpenGLGraphics::drawImageCached(const Image *const image,
                                           int x, int y)
{
    if (!image)
        return;

    if (image->mGLImage != mImageCached)
    {
        completeCache();
        mImageCached = image->mGLImage;
        mAlphaCached = image->mAlpha;
    }

    const SDL_Rect &imageRect = image->mBounds;
    const int w = imageRect.w;
    const int h = imageRect.h;

    if (w == 0 || h == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);

    const unsigned int vLimit = mMaxVertices * 4;

    unsigned int vp = mVpCached;

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        const float texX2 = static_cast<float>(srcX + w) / tw;
        const float texY2 = static_cast<float>(srcY + h) / th;

        vertFill2D(mFloatTexArrayCached, mIntVertArrayCached,
            texX1, texY1, texX2, texY2,
            x, y, w, h);

        vp += 8;
        if (vp >= vLimit)
        {
            completeCache();
            vp = 0;
        }
        else
        {
            mVpCached = vp;
        }
    }
    else
    {
        vertFillNv(mIntTexArrayCached, mIntVertArrayCached,
            srcX, srcY, x, y, w, h);

        vp += 8;
        if (vp >= vLimit)
        {
            completeCache();
            vp = 0;
        }
        else
        {
            mVpCached = vp;
        }
    }
}

void NormalOpenGLGraphics::drawPatternCached(const Image *const image,
                                             const int x, const int y,
                                             const int w, const int h)
{
    FUNC_BLOCK("Graphics::drawPatternCached", 1)
    if (!image)
        return;

    if (image->mGLImage != mImageCached)
    {
        completeCache();
        mImageCached = image->mGLImage;
    }

    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int iw = imageRect.w;
    const int ih = imageRect.h;

    if (iw == 0 || ih == 0)
        return;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);

    unsigned int vp = mVpCached;
    const unsigned int vLimit = mMaxVertices * 4;
    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            const float texY2 = static_cast<float>(srcY + height) / th;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;

                const float texX2 = static_cast<float>(srcX + width) / tw;

                vertFill2D(mFloatTexArrayCached, mIntVertArrayCached,
                    texX1, texY1, texX2, texY2,
                    dstX, dstY, width, height);

                vp += 8;
                if (vp >= vLimit)
                {
                    completeCache();
                    vp = 0;
                }
            }
        }
    }
    else
    {
        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;

                vertFillNv(mIntTexArrayCached, mIntVertArrayCached,
                    srcX, srcY, dstX, dstY, width, height);

                vp += 8;
                if (vp >= vLimit)
                {
                    completeCache();
                    vp = 0;
                }
            }
        }
    }
    mVpCached = vp;
}

void NormalOpenGLGraphics::completeCache()
{
    if (!mImageCached)
        return;

    setColorAlpha(mAlphaCached);
#ifdef DEBUG_BIND_TEXTURE
//    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, mImageCached);
    setTexturingAndBlending(true);

    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
        drawQuadArrayfiCached(mVpCached);
    else
        drawQuadArrayiiCached(mVpCached);

    mImageCached = 0;
    mVpCached = 0;
}

bool NormalOpenGLGraphics::drawRescaledImage(const Image *const image,
                                             int dstX, int dstY,
                                             const int desiredWidth,
                                             const int desiredHeight)
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    if (!image)
        return false;

    const SDL_Rect &imageRect = image->mBounds;

    // Just draw the image normally when no resizing is necessary,
    if (imageRect.w == desiredWidth && imageRect.h == desiredHeight)
        return drawImageInline(image, dstX, dstY);

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    // Draw a textured quad.
    drawRescaledQuad(image, imageRect.x, imageRect.y, dstX, dstY,
        imageRect.w, imageRect.h, desiredWidth, desiredHeight);

    return true;
}

void NormalOpenGLGraphics::drawPattern(const Image *const image,
                                       const int x, const int y,
                                       const int w, const int h)
{
    drawPatternInline(image, x, y, w, h);
}

void NormalOpenGLGraphics::drawPatternInline(const Image *const image,
                                             const int x, const int y,
                                             const int w, const int h)
{
    FUNC_BLOCK("Graphics::drawPattern", 1)
    if (!image)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int iw = imageRect.w;
    const int ih = imageRect.h;

    if (iw == 0 || ih == 0)
        return;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);

    setColorAlpha(image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;
    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            const float texY2 = static_cast<float>(srcY + height) / th;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;
                const float texX2 = static_cast<float>(srcX + width) / tw;

                vertFill2D(mFloatTexArray, mIntVertArray,
                    texX1, texY1, texX2, texY2,
                    dstX, dstY, width, height);

                vp += 8;
                if (vp >= vLimit)
                {
                    drawQuadArrayfi(vp);
                    vp = 0;
                }
            }
        }
        if (vp > 0)
            drawQuadArrayfi(vp);
    }
    else
    {
        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;

                vertFillNv(mIntTexArray, mIntVertArray,
                    srcX, srcY, dstX, dstY, width, height);

                vp += 8;
                if (vp >= vLimit)
                {
                    drawQuadArrayii(vp);
                    vp = 0;
                }
            }
        }
        if (vp > 0)
            drawQuadArrayii(vp);
    }
}

void NormalOpenGLGraphics::drawRescaledPattern(const Image *const image,
                                               const int x, const int y,
                                               const int w, const int h,
                                               const int scaledWidth,
                                               const int scaledHeight)
{
    if (!image)
        return;

    if (scaledWidth == 0 || scaledHeight == 0)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    setColorAlpha(image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float tw = static_cast<float>(image->mTexWidth);
        const float th = static_cast<float>(image->mTexHeight);

        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        const float tFractionW = iw / tw;
        const float tFractionH = ih / th;

        for (int py = 0; py < h; py += scaledHeight)
        {
            const int height = (py + scaledHeight >= h)
                ? h - py : scaledHeight;
            const int dstY = y + py;
            const float visibleFractionH = static_cast<float>(height)
                / scaledHeight;
            const float texY2 = texY1 + tFractionH * visibleFractionH;
            for (int px = 0; px < w; px += scaledWidth)
            {
                const int width = (px + scaledWidth >= w)
                    ? w - px : scaledWidth;
                const int dstX = x + px;
                const float visibleFractionW = static_cast<float>(width)
                    / scaledWidth;
                const float texX2 = texX1 + tFractionW * visibleFractionW;

                vertFill2D(mFloatTexArray, mIntVertArray,
                    texX1, texY1, texX2, texY2,
                    dstX, dstY, width, height);

                vp += 8;
                if (vp >= vLimit)
                {
                    drawQuadArrayfi(vp);
                    vp = 0;
                }
            }
        }
        if (vp > 0)
            drawQuadArrayfi(vp);
    }
    else
    {
        const float scaleFactorW = static_cast<float>(scaledWidth) / iw;
        const float scaleFactorH = static_cast<float>(scaledHeight) / ih;

        for (int py = 0; py < h; py += scaledHeight)
        {
            const int height = (py + scaledHeight >= h)
                ? h - py : scaledHeight;
            const int dstY = y + py;
            const int scaledY = srcY + height / scaleFactorH;
            for (int px = 0; px < w; px += scaledWidth)
            {
                const int width = (px + scaledWidth >= w)
                    ? w - px : scaledWidth;
                const int dstX = x + px;
                const int scaledX = srcX + width / scaleFactorW;

                mIntTexArray[vp + 0] = srcX;
                mIntTexArray[vp + 1] = srcY;

                mIntTexArray[vp + 2] = scaledX;
                mIntTexArray[vp + 3] = srcY;

                mIntTexArray[vp + 4] = scaledX;
                mIntTexArray[vp + 5] = scaledY;

                mIntTexArray[vp + 6] = srcX;
                mIntTexArray[vp + 7] = scaledY;

                mIntVertArray[vp + 0] = dstX;
                mIntVertArray[vp + 1] = dstY;

                mIntVertArray[vp + 2] = dstX + width;
                mIntVertArray[vp + 3] = dstY;

                mIntVertArray[vp + 4] = dstX + width;
                mIntVertArray[vp + 5] = dstY + height;

                mIntVertArray[vp + 6] = dstX;
                mIntVertArray[vp + 7] = dstY + height;

                vp += 8;
                if (vp >= vLimit)
                {
                    drawQuadArrayii(vp);
                    vp = 0;
                }
            }
        }
        if (vp > 0)
            drawQuadArrayii(vp);
    }
}

inline void NormalOpenGLGraphics::drawVertexes(const
                                               NormalOpenGLGraphicsVertexes
                                               &ogl)
{
    const std::vector<GLint*> &intVertPool = ogl.mIntVertPool;
    std::vector<GLint*>::const_iterator iv;
    const std::vector<GLint*>::const_iterator iv_end = intVertPool.end();
    const std::vector<int> &vp = ogl.mVp;
    std::vector<int>::const_iterator ivp;
    const std::vector<int>::const_iterator ivp_end = vp.end();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const std::vector<GLfloat*> &floatTexPool = ogl.mFloatTexPool;
        std::vector<GLfloat*>::const_iterator ft;
        const std::vector<GLfloat*>::const_iterator
            ft_end = floatTexPool.end();

        for (iv = intVertPool.begin(), ft = floatTexPool.begin(),
             ivp = vp.begin();
             iv != iv_end && ft != ft_end && ivp != ivp_end;
             ++ iv, ++ ft, ++ ivp)
        {
            drawQuadArrayfi(*iv, *ft, *ivp);
        }
    }
    else
    {
        const std::vector<GLint*> &intTexPool = ogl.mIntTexPool;
        std::vector<GLint*>::const_iterator it;
        const std::vector<GLint*>::const_iterator it_end = intTexPool.end();

        for (iv = intVertPool.begin(), it = intTexPool.begin(),
             ivp = vp.begin();
             iv != iv_end && it != it_end && ivp != ivp_end;
             ++ iv, ++ it, ++ ivp)
        {
            drawQuadArrayii(*iv, *it, *ivp);
        }
    }
}

void NormalOpenGLGraphics::calcPattern(ImageVertexes* const vert,
                                       const Image *const image,
                                       const int x, const int y,
                                       const int w, const int h) const
{
    calcPatternInline(vert, image, x, y, w, h);
}

void NormalOpenGLGraphics::calcPatternInline(ImageVertexes* const vert,
                                             const Image *const image,
                                             const int x, const int y,
                                             const int w, const int h) const
{
    if (!image || !vert)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;

    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);

    const unsigned int vLimit = mMaxVertices * 4;

    NormalOpenGLGraphicsVertexes &ogl = vert->ogl;
    unsigned int vp = ogl.continueVp();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        GLfloat *floatTexArray = ogl.continueFloatTexArray();
        GLint *intVertArray = ogl.continueIntVertArray();

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            const float texY2 = static_cast<float>(srcY + height) / th;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;
                const float texX2 = static_cast<float>(srcX + width) / tw;

                vertFill2D(floatTexArray, intVertArray,
                    texX1, texY1, texX2, texY2,
                    dstX, dstY, width, height);

                vp += 8;
                if (vp >= vLimit)
                {
                    floatTexArray = ogl.switchFloatTexArray();
                    intVertArray = ogl.switchIntVertArray();
                    ogl.switchVp(vp);
                    vp = 0;
                }
            }
        }
    }
    else
    {
        GLint *intTexArray = ogl.continueIntTexArray();
        GLint *intVertArray = ogl.continueIntVertArray();

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            for (int px = 0; px < w; px += iw)
            {
                const int width = (px + iw >= w) ? w - px : iw;
                const int dstX = x + px;

                vertFillNv(intTexArray, intVertArray,
                    srcX, srcY, dstX, dstY, width, height);

                vp += 8;
                if (vp >= vLimit)
                {
                    intTexArray = ogl.switchIntTexArray();
                    intVertArray = ogl.switchIntVertArray();
                    ogl.switchVp(vp);
                    vp = 0;
                }
            }
        }
    }
    ogl.switchVp(vp);
}

void NormalOpenGLGraphics::calcTileCollection(ImageCollection *const vertCol,
                                              const Image *const image,
                                              int x, int y)
{
    if (vertCol->currentGLImage != image->mGLImage)
    {
        ImageVertexes *const vert = new ImageVertexes();
        vertCol->currentGLImage = image->mGLImage;
        vertCol->currentVert = vert;
        vert->image = image;
        vertCol->draws.push_back(vert);
        calcTileVertexesInline(vert, image, x, y);
    }
    else
    {
        calcTileVertexesInline(vertCol->currentVert, image, x, y);
    }
}

void NormalOpenGLGraphics::drawTileCollection(const ImageCollection
                                              *const vertCol)
{
    const ImageVertexesVector &draws = vertCol->draws;
    const ImageCollectionCIter it_end = draws.end();
    for (ImageCollectionCIter it = draws.begin(); it != it_end; ++ it)
    {
        const ImageVertexes *const vert = *it;
        const Image *const image = vert->image;

        setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
        debugBindTexture(image);
#endif
        bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
        setTexturingAndBlending(true);
        drawVertexes(vert->ogl);
    }
}

void NormalOpenGLGraphics::calcPattern(ImageCollection* const vertCol,
                                       const Image *const image,
                                       const int x, const int y,
                                       const int w, const int h) const
{
    ImageVertexes *vert = nullptr;
    if (vertCol->currentGLImage != image->mGLImage)
    {
        vert = new ImageVertexes();
        vertCol->currentGLImage = image->mGLImage;
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

void NormalOpenGLGraphics::calcTileVertexes(ImageVertexes *const vert,
                                            const Image *const image,
                                            int dstX, int dstY) const
{
    calcTileVertexesInline(vert, image, dstX, dstY);
}

void NormalOpenGLGraphics::calcTileVertexesInline(ImageVertexes *const vert,
                                                  const Image *const image,
                                                  int dstX, int dstY) const
{
    if (!vert || !image)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int w = imageRect.w;
    const int h = imageRect.h;

    if (w == 0 || h == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);

    const unsigned int vLimit = mMaxVertices * 4;

    NormalOpenGLGraphicsVertexes &ogl = vert->ogl;

//    std::vector<int> *vps = ogl.getVp();
    unsigned int vp = ogl.continueVp();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        const float texX2 = static_cast<float>(srcX + w) / tw;
        const float texY2 = static_cast<float>(srcY + h) / th;

        GLfloat *const floatTexArray = ogl.continueFloatTexArray();
        GLint *const intVertArray = ogl.continueIntVertArray();

        vertFill2D(floatTexArray, intVertArray,
            texX1, texY1, texX2, texY2,
            dstX, dstY, w, h);

        vp += 8;
        if (vp >= vLimit)
        {
            ogl.switchFloatTexArray();
            ogl.switchIntVertArray();
            ogl.switchVp(vp);
            vp = 0;
        }
    }
    else
    {
        GLint *const intTexArray = ogl.continueIntTexArray();
        GLint *const intVertArray = ogl.continueIntVertArray();

        vertFillNv(intTexArray, intVertArray,
            srcX, srcY, dstX, dstY, w, h);

        vp += 8;
        if (vp >= vLimit)
        {
            ogl.switchIntTexArray();
            ogl.switchIntVertArray();
            ogl.switchVp(vp);
            vp = 0;
        }
    }
    ogl.switchVp(vp);
}

void NormalOpenGLGraphics::drawTileVertexes(const ImageVertexes *const vert)
{
    if (!vert)
        return;
    const Image *const image = vert->image;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);
    drawVertexes(vert->ogl);
}

void NormalOpenGLGraphics::calcWindow(ImageCollection *const vertCol,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const ImageRect &imgRect)
{
    ImageVertexes *vert = nullptr;
    Image *const image = imgRect.grid[4];
    if (!image)
        return;
    if (vertCol->currentGLImage != image->mGLImage)
    {
        vert = new ImageVertexes();
        vertCol->currentGLImage = image->mGLImage;
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

void NormalOpenGLGraphics::updateScreen()
{
    BLOCK_START("Graphics::updateScreen")
//    glFlush();
//    glFinish();
#ifdef DEBUG_DRAW_CALLS
    mLastDrawCalls = mDrawCalls;
    mDrawCalls = 0;
#endif
#ifdef DEBUG_BIND_TEXTURE
    mLastBinds = mBinds;
    mBinds = 0;
#endif
#ifdef USE_SDL2
    SDL_GL_SwapWindow(mWindow);
#else
    SDL_GL_SwapBuffers();
#endif
#ifdef DEBUG_OPENGL
    if (mglFrameTerminator)
        mglFrameTerminator();
#endif
// may be need clear?
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    BLOCK_END("Graphics::updateScreen")
}

void NormalOpenGLGraphics::beginDraw()
{
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const int w = mRect.w;
    const int h = mRect.h;

#ifdef ANDROID
    glOrthof(0.0, static_cast<float>(w),
        static_cast<float>(h),
        0.0, -1.0, 1.0);
#else
    glOrtho(0.0, static_cast<double>(w),
        static_cast<double>(h),
        0.0, -1.0, 1.0);
#endif

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_SCISSOR_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_COLOR_LOGIC_OP);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_STENCIL_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glShadeModel(GL_FLAT);
    glDepthMask(GL_FALSE);

#ifndef ANDROID
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
#ifndef __MINGW32__
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
#endif
#endif

    pushClipArea(Rect(0, 0, w, h));
}

void NormalOpenGLGraphics::endDraw()
{
    popClipArea();
}

void NormalOpenGLGraphics::prepareScreenshot()
{
    if (config.getBoolValue("usefbo"))
        graphicsManager.createFBO(mRect.w, mRect.h, &mFbo);
}

SDL_Surface* NormalOpenGLGraphics::getScreenshot()
{
    const int h = mRect.h;
    const int w = mRect.w - (mRect.w % 4);
    GLint pack = 1;

    SDL_Surface *const screenshot = MSDL_CreateRGBSurface(
            SDL_SWSURFACE,
            w, h, 24,
            0xff0000, 0x00ff00, 0x0000ff, 0x000000);

    if (!screenshot)
        return nullptr;

    if (SDL_MUSTLOCK(screenshot))
        SDL_LockSurface(screenshot);

    // Grap the pixel buffer and write it to the SDL surface
    glGetIntegerv(GL_PACK_ALIGNMENT, &pack);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, screenshot->pixels);

    // Flip the screenshot, as OpenGL has 0,0 in bottom left
    const unsigned int lineSize = 3 * w;
    GLubyte *const buf = new GLubyte[lineSize];

    const int h2 = h / 2;
    for (int i = 0; i < h2; i++)
    {
        GLubyte *const top = static_cast<GLubyte *const>(
            screenshot->pixels) + lineSize * i;
        GLubyte *const bot = static_cast<GLubyte *const>(
            screenshot->pixels) + lineSize * (h - 1 - i);

        memcpy(buf, top, lineSize);
        memcpy(top, bot, lineSize);
        memcpy(bot, buf, lineSize);
    }

    delete [] buf;

    if (config.getBoolValue("usefbo"))
        graphicsManager.deleteFBO(&mFbo);

    glPixelStorei(GL_PACK_ALIGNMENT, pack);

    if (SDL_MUSTLOCK(screenshot))
        SDL_UnlockSurface(screenshot);

    return screenshot;
}

bool NormalOpenGLGraphics::pushClipArea(Rect area)
{
    int transX = 0;
    int transY = 0;

    if (!mClipStack.empty())
    {
        const ClipRect &clipArea = mClipStack.top();
        transX = -clipArea.xOffset;
        transY = -clipArea.yOffset;
    }

    const bool result = Graphics::pushClipArea(area);

    const ClipRect &clipArea = mClipStack.top();
    transX += clipArea.xOffset;
    transY += clipArea.yOffset;

    if (transX || transY)
    {
        glTranslatef(static_cast<GLfloat>(transX),
                     static_cast<GLfloat>(transY), 0);
    }
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);

    return result;
}

void NormalOpenGLGraphics::popClipArea()
{
    if (mClipStack.empty())
        return;

    const ClipRect &clipArea1 = mClipStack.top();
    int transX = -clipArea1.xOffset;
    int transY = -clipArea1.yOffset;

    Graphics::popClipArea();

    if (mClipStack.empty())
        return;

    const ClipRect &clipArea = mClipStack.top();
    transX += clipArea.xOffset;
    transY += clipArea.yOffset;
    if (transX || transY)
    {
        glTranslatef(static_cast<GLfloat>(transX),
                     static_cast<GLfloat>(transY), 0);
    }
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void NormalOpenGLGraphics::drawPoint(int x, int y)
{
    setTexturingAndBlending(false);
    restoreColor();

#ifdef ANDROID
    // TODO need fix
#else
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
#endif
}

void NormalOpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
{
    setTexturingAndBlending(false);
    restoreColor();

    mFloatTexArray[0] = static_cast<float>(x1) + 0.5F;
    mFloatTexArray[1] = static_cast<float>(y1) + 0.5F;
    mFloatTexArray[2] = static_cast<float>(x2) + 0.5F;
    mFloatTexArray[3] = static_cast<float>(y2) + 0.5F;

    drawLineArrayf(4);
}

void NormalOpenGLGraphics::drawRectangle(const Rect& rect)
{
    drawRectangle(rect, false);
}

void NormalOpenGLGraphics::fillRectangle(const Rect& rect)
{
    drawRectangle(rect, true);
}

void NormalOpenGLGraphics::setTexturingAndBlending(const bool enable)
{
    if (enable)
    {
        if (!mTexture)
        {
            glEnable(OpenGLImageHelper::mTextureType);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            mTexture = true;
        }

        if (!mAlpha)
        {
            glEnable(GL_BLEND);
            mAlpha = true;
        }
    }
    else
    {
        mLastImage = 0;
        if (mAlpha && !mColorAlpha)
        {
            glDisable(GL_BLEND);
            mAlpha = false;
        }
        else if (!mAlpha && mColorAlpha)
        {
            glEnable(GL_BLEND);
            mAlpha = true;
        }

        if (mTexture)
        {
            glDisable(OpenGLImageHelper::mTextureType);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            mTexture = false;
        }
    }
}

void NormalOpenGLGraphics::drawRectangle(const Rect& rect,
                                         const bool filled)
{
    BLOCK_START("Graphics::drawRectangle")
    const float offset = filled ? 0 : 0.5F;
    const float x = static_cast<float>(rect.x);
    const float y = static_cast<float>(rect.y);
    const float width = static_cast<float>(rect.width);
    const float height = static_cast<float>(rect.height);

    setTexturingAndBlending(false);
    restoreColor();

    GLfloat vert[] =
    {
        x + offset, y + offset,
        x + width - offset, y + offset,
        x + width - offset, y + height - offset,
        x + offset, y + height - offset
    };

    glVertexPointer(2, GL_FLOAT, 0, &vert);
#ifdef DEBUG_DRAW_CALLS
        mDrawCalls ++;
#endif
    glDrawArrays(filled ? GL_QUADS : GL_LINE_LOOP, 0, 4);
    BLOCK_END("Graphics::drawRectangle")
}

bool NormalOpenGLGraphics::drawNet(const int x1, const int y1,
                                   const int x2, const int y2,
                                   const int width, const int height)
{
    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    setTexturingAndBlending(false);
    restoreColor();

    const float xf1 = static_cast<float>(x1);
    const float xf2 = static_cast<float>(x2);
    const float yf1 = static_cast<float>(y1);
    const float yf2 = static_cast<float>(y2);

    for (int y = y1; y < y2; y += height)
    {
        mFloatTexArray[vp + 0] = xf1;
        mFloatTexArray[vp + 1] = static_cast<float>(y);

        mFloatTexArray[vp + 2] = xf2;
        mFloatTexArray[vp + 3] = static_cast<float>(y);

        vp += 4;
        if (vp >= vLimit)
        {
            drawLineArrayf(vp);
            vp = 0;
        }
    }

    for (int x = x1; x < x2; x += width)
    {
        mFloatTexArray[vp + 0] = static_cast<float>(x);
        mFloatTexArray[vp + 1] = yf1;

        mFloatTexArray[vp + 2] = static_cast<float>(x);
        mFloatTexArray[vp + 3] = yf2;

        vp += 4;
        if (vp >= vLimit)
        {
            drawLineArrayf(vp);
            vp = 0;
        }
    }

    if (vp > 0)
        drawLineArrayf(vp);

    return true;
}

void NormalOpenGLGraphics::bindTexture(const GLenum target,
                                       const GLuint texture)
{
    if (mLastImage != texture)
    {
        mLastImage = texture;
        glBindTexture(target, texture);
#ifdef DEBUG_BIND_TEXTURE
        mBinds ++;
#endif
    }
}

inline void NormalOpenGLGraphics::drawQuadArrayfi(const int size)
{
    glVertexPointer(2, GL_INT, 0, mIntVertArray);
    glTexCoordPointer(2, GL_FLOAT, 0, mFloatTexArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawQuadArrayfiCached(const int size)
{
    glVertexPointer(2, GL_INT, 0, mIntVertArrayCached);
    glTexCoordPointer(2, GL_FLOAT, 0, mFloatTexArrayCached);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawQuadArrayfi(const GLint *const
                                                  intVertArray,
                                                  const GLfloat *const
                                                  floatTexArray,
                                                  const int size)
{
    glVertexPointer(2, GL_INT, 0, intVertArray);
    glTexCoordPointer(2, GL_FLOAT, 0, floatTexArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawQuadArrayii(const int size)
{
    glVertexPointer(2, GL_INT, 0, mIntVertArray);
    glTexCoordPointer(2, GL_INT, 0, mIntTexArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawQuadArrayiiCached(const int size)
{
    glVertexPointer(2, GL_INT, 0, mIntVertArrayCached);
    glTexCoordPointer(2, GL_INT, 0, mIntTexArrayCached);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawQuadArrayii(const GLint *const
                                                  intVertArray,
                                                  const GLint *const
                                                  intTexArray,
                                                  const int size)
{
    glVertexPointer(2, GL_INT, 0, intVertArray);
    glTexCoordPointer(2, GL_INT, 0, intTexArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawLineArrayi(const int size)
{
    glVertexPointer(2, GL_INT, 0, mIntVertArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_LINES, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawLineArrayf(const int size)
{
    glVertexPointer(2, GL_FLOAT, 0, mFloatTexArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_LINES, 0, size / 2);
}

void NormalOpenGLGraphics::dumpSettings()
{
    GLint test[1000];
    logger->log("\n\n");
    logger->log("start opengl dump");
    for (int f = 0; f < 65535; f ++)
    {
        test[0] = 0;
        test[1] = 0;
        test[2] = 0;
        test[3] = 0;
        glGetIntegerv(f, &test[0]);
        if (test[0] || test[1] || test[2] || test[3])
        {
            logger->log("\n%d = %d, %d, %d, %d", f,
                test[0], test[1], test[2], test[3]);
        }
    }
}

void NormalOpenGLGraphics::setColorAlpha(const float alpha)
{
    if (!mIsByteColor && mFloatColor == alpha)
        return;

    glColor4f(1.0F, 1.0F, 1.0F, alpha);
    mIsByteColor = false;
    mFloatColor = alpha;
}

void NormalOpenGLGraphics::restoreColor()
{
    if (mIsByteColor && mByteColor == mColor)
        return;

    glColor4ub(static_cast<GLubyte>(mColor.r),
               static_cast<GLubyte>(mColor.g),
               static_cast<GLubyte>(mColor.b),
               static_cast<GLubyte>(mColor.a));
    mIsByteColor = true;
    mByteColor = mColor;
}

void NormalOpenGLGraphics::drawImageRect(const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &imgRect)
{
    #include "render/graphics_drawImageRect.hpp"
}

void NormalOpenGLGraphics::calcImageRect(ImageVertexes *const vert,
                                         const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &imgRect)
{
    #include "render/graphics_calcImageRect.hpp"
}

#ifdef DEBUG_BIND_TEXTURE
void NormalOpenGLGraphics::debugBindTexture(const Image *const image)
{
    const std::string texture = image->getIdPath();
    if (mOldTexture != texture)
    {
        if ((!mOldTexture.empty() || !texture.empty())
            && mOldTextureId != image->mGLImage)
        {
            logger->log("bind: %s (%d) to %s (%d)", mOldTexture.c_str(),
                mOldTextureId, texture.c_str(), image->mGLImage);
        }
        mOldTextureId = image->mGLImage;
        mOldTexture = texture;
    }
}
#else
void NormalOpenGLGraphics::debugBindTexture(const Image *const image A_UNUSED)
{
}
#endif

#endif  // USE_OPENGL
