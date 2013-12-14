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

#include "main.h"
#if defined USE_OPENGL && !defined ANDROID

#include "render/normalopenglgraphics.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "resources/image.h"
#include "resources/openglimagehelper.h"

#include "utils/sdlcheckutils.h"

#include "debug.h"

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
    mTexture(false),
    mIsByteColor(false),
    mByteColor(),
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
    mFloatTexArray = new GLfloat[sz];
    mIntTexArray = new GLint[sz];
    mIntVertArray = new GLint[sz];
}

bool NormalOpenGLGraphics::setVideoMode(const int w, const int h,
                                        const int bpp, const bool fs,
                                        const bool hwaccel, const bool resize,
                                        const bool noFrame)
{
    setMainFlags(w, h, bpp, fs, hwaccel, resize, noFrame);

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


bool NormalOpenGLGraphics::drawImage2(const Image *const image,
                                      int srcX, int srcY,
                                      int dstX, int dstY,
                                      const int width, const int height,
                                      const bool useColor)
{
    FUNC_BLOCK("Graphics::drawImage2", 1)
    if (!image)
        return false;

    const SDL_Rect &imageRect = image->mBounds;

    if (!useColor)
        setColorAlpha(image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    drawQuad(image, srcX + imageRect.x, srcY + imageRect.y,
        dstX, dstY, width, height);

    return true;
}

bool NormalOpenGLGraphics::drawRescaledImage(const Image *const image,
                                             int srcX, int srcY,
                                             int dstX, int dstY,
                                             const int width, const int height,
                                             const int desiredWidth,
                                             const int desiredHeight,
                                             const bool useColor)
{
    return drawRescaledImage(image, srcX, srcY,
                             dstX, dstY,
                             width, height,
                             desiredWidth, desiredHeight,
                             useColor, true);
}

bool NormalOpenGLGraphics::drawRescaledImage(const Image *const image,
                                             int srcX, int srcY,
                                             int dstX, int dstY,
                                             const int width, const int height,
                                             const int desiredWidth,
                                             const int desiredHeight,
                                             const bool useColor,
                                             bool smooth)
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    if (!image)
        return false;

    // Just draw the image normally when no resizing is necessary,
    if (width == desiredWidth && height == desiredHeight)
    {
        return drawImage2(image, srcX, srcY, dstX, dstY,
                          width, height, useColor);
    }

    // When the desired image is smaller than the current one,
    // disable smooth effect.
    if (width > desiredWidth && height > desiredHeight)
        smooth = false;

    const SDL_Rect &imageRect = image->mBounds;
    srcX += imageRect.x;
    srcY += imageRect.y;

    if (!useColor)
        setColorAlpha(image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    // Draw a textured quad.
    drawRescaledQuad(image, srcX, srcY, dstX, dstY, width, height,
                     desiredWidth, desiredHeight);

    if (smooth)  // A basic smooth effect...
    {
        setColorAlpha(0.2F);
        drawRescaledQuad(image, srcX, srcY, dstX - 1, dstY - 1, width, height,
                         desiredWidth + 1, desiredHeight + 1);
        drawRescaledQuad(image, srcX, srcY, dstX + 1, dstY + 1, width, height,
                         desiredWidth - 1, desiredHeight - 1);

        drawRescaledQuad(image, srcX, srcY, dstX + 1, dstY, width, height,
                         desiredWidth - 1, desiredHeight);
        drawRescaledQuad(image, srcX, srcY, dstX, dstY + 1, width, height,
                         desiredWidth, desiredHeight - 1);
    }

    return true;
}

void NormalOpenGLGraphics::drawImagePattern(const Image *const image,
                                            const int x, const int y,
                                            const int w, const int h)
{
    FUNC_BLOCK("Graphics::drawImagePattern", 1)
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

                mFloatTexArray[vp + 0] = texX1;
                mFloatTexArray[vp + 1] = texY1;

                mFloatTexArray[vp + 2] = texX2;
                mFloatTexArray[vp + 3] = texY1;

                mFloatTexArray[vp + 4] = texX2;
                mFloatTexArray[vp + 5] = texY2;

                mFloatTexArray[vp + 6] = texX1;
                mFloatTexArray[vp + 7] = texY2;

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

                mIntTexArray[vp + 0] = srcX;
                mIntTexArray[vp + 1] = srcY;

                mIntTexArray[vp + 2] = srcX + width;
                mIntTexArray[vp + 3] = srcY;

                mIntTexArray[vp + 4] = srcX + width;
                mIntTexArray[vp + 5] = srcY + height;

                mIntTexArray[vp + 6] = srcX;
                mIntTexArray[vp + 7] = srcY + height;

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

void NormalOpenGLGraphics::drawRescaledImagePattern(const Image *const image,
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

                mFloatTexArray[vp + 0] = texX1;
                mFloatTexArray[vp + 1] = texY1;

                mFloatTexArray[vp + 2] = texX2;
                mFloatTexArray[vp + 3] = texY1;

                mFloatTexArray[vp + 4] = texX2;
                mFloatTexArray[vp + 5] = texY2;

                mFloatTexArray[vp + 6] = texX1;
                mFloatTexArray[vp + 7] = texY2;

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

void NormalOpenGLGraphics::calcImagePattern(ImageVertexes* const vert,
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

                floatTexArray[vp + 0] = texX1;
                floatTexArray[vp + 1] = texY1;

                floatTexArray[vp + 2] = texX2;
                floatTexArray[vp + 3] = texY1;

                floatTexArray[vp + 4] = texX2;
                floatTexArray[vp + 5] = texY2;

                floatTexArray[vp + 6] = texX1;
                floatTexArray[vp + 7] = texY2;

                intVertArray[vp + 0] = dstX;
                intVertArray[vp + 1] = dstY;

                intVertArray[vp + 2] = dstX + width;
                intVertArray[vp + 3] = dstY;

                intVertArray[vp + 4] = dstX + width;
                intVertArray[vp + 5] = dstY + height;

                intVertArray[vp + 6] = dstX;
                intVertArray[vp + 7] = dstY + height;

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

                intTexArray[vp + 0] = srcX;
                intTexArray[vp + 1] = srcY;

                intTexArray[vp + 2] = srcX + width;
                intTexArray[vp + 3] = srcY;

                intTexArray[vp + 4] = srcX + width;
                intTexArray[vp + 5] = srcY + height;

                intTexArray[vp + 6] = srcX;
                intTexArray[vp + 7] = srcY + height;

                intVertArray[vp + 0] = dstX;
                intVertArray[vp + 1] = dstY;

                intVertArray[vp + 2] = dstX + width;
                intVertArray[vp + 3] = dstY;

                intVertArray[vp + 4] = dstX + width;
                intVertArray[vp + 5] = dstY + height;

                intVertArray[vp + 6] = dstX;
                intVertArray[vp + 7] = dstY + height;

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
        calcTileVertexes(vert, image, x, y);
    }
    else
    {
        calcTileVertexes(vertCol->currentVert, image, x, y);
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

void NormalOpenGLGraphics::calcImagePattern(ImageCollection* const vertCol,
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

    calcImagePattern(vert, image, x, y, w, h);
}

void NormalOpenGLGraphics::calcTileVertexes(ImageVertexes *const vert,
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

        floatTexArray[vp + 0] = texX1;
        floatTexArray[vp + 1] = texY1;

        floatTexArray[vp + 2] = texX2;
        floatTexArray[vp + 3] = texY1;

        floatTexArray[vp + 4] = texX2;
        floatTexArray[vp + 5] = texY2;

        floatTexArray[vp + 6] = texX1;
        floatTexArray[vp + 7] = texY2;

        intVertArray[vp + 0] = dstX;
        intVertArray[vp + 1] = dstY;

        intVertArray[vp + 2] = dstX + w;
        intVertArray[vp + 3] = dstY;

        intVertArray[vp + 4] = dstX + w;
        intVertArray[vp + 5] = dstY + h;

        intVertArray[vp + 6] = dstX;
        intVertArray[vp + 7] = dstY + h;

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

        intTexArray[vp + 0] = srcX;
        intTexArray[vp + 1] = srcY;

        intTexArray[vp + 2] = srcX + w;
        intTexArray[vp + 3] = srcY;

        intTexArray[vp + 4] = srcX + w;
        intTexArray[vp + 5] = srcY + h;

        intTexArray[vp + 6] = srcX;
        intTexArray[vp + 7] = srcY + h;

        intVertArray[vp + 0] = dstX;
        intVertArray[vp + 1] = dstY;

        intVertArray[vp + 2] = dstX + w;
        intVertArray[vp + 3] = dstY;

        intVertArray[vp + 4] = dstX + w;
        intVertArray[vp + 5] = dstY + h;

        intVertArray[vp + 6] = dstX;
        intVertArray[vp + 7] = dstY + h;

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

bool NormalOpenGLGraphics::calcWindow(ImageCollection *const vertCol,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const ImageRect &imgRect)
{
    ImageVertexes *vert = nullptr;
    Image *const image = imgRect.grid[4];
    if (!image)
        return false;
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

    const Image *const *const grid = &imgRect.grid[0];
    return calcImageRect(vert, x, y, w, h,
        grid[0], grid[2], grid[6], grid[8],
        grid[1], grid[5], grid[7], grid[3],
        grid[4]);
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
// may be need clear?
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    BLOCK_END("Graphics::updateScreen")
}

void NormalOpenGLGraphics::_beginDraw()
{
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const int w = mRect.w;
    const int h = mRect.h;

#ifdef ANDROID
    glOrthof(0.0, static_cast<float>(w), static_cast<float>(h),
        0.0, -1.0, 1.0);
#else
    glOrtho(0.0, static_cast<double>(w), static_cast<double>(h),
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

    pushClipArea(gcn::Rectangle(0, 0, w, h));
}

void NormalOpenGLGraphics::_endDraw()
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

bool NormalOpenGLGraphics::pushClipArea(gcn::Rectangle area)
{
    int transX = 0;
    int transY = 0;

    if (!mClipStack.empty())
    {
        const gcn::ClipRectangle &clipArea = mClipStack.top();
        transX = -clipArea.xOffset;
        transY = -clipArea.yOffset;
    }

    const bool result = gcn::Graphics::pushClipArea(area);

    const gcn::ClipRectangle &clipArea = mClipStack.top();
    transX += clipArea.xOffset;
    transY += clipArea.yOffset;

    if (transX || transY)
    {
        glTranslatef(static_cast<GLfloat>(transX),
                     static_cast<GLfloat>(transY), 0);
    }
    glScissor(clipArea.x, mRect.h - clipArea.y - clipArea.height,
        clipArea.width, clipArea.height);

    return result;
}

void NormalOpenGLGraphics::popClipArea()
{
    if (mClipStack.empty())
        return;

    const gcn::ClipRectangle &clipArea1 = mClipStack.top();
    int transX = -clipArea1.xOffset;
    int transY = -clipArea1.yOffset;

    gcn::Graphics::popClipArea();

    if (mClipStack.empty())
        return;

    const gcn::ClipRectangle &clipArea = mClipStack.top();
    transX += clipArea.xOffset;
    transY += clipArea.yOffset;
    if (transX || transY)
    {
        glTranslatef(static_cast<GLfloat>(transX),
                     static_cast<GLfloat>(transY), 0);
    }
    glScissor(clipArea.x, mRect.h - clipArea.y - clipArea.height,
        clipArea.width, clipArea.height);
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

void NormalOpenGLGraphics::drawRectangle(const gcn::Rectangle& rect)
{
    drawRectangle(rect, false);
}

void NormalOpenGLGraphics::fillRectangle(const gcn::Rectangle& rect)
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

void NormalOpenGLGraphics::drawRectangle(const gcn::Rectangle& rect,
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
