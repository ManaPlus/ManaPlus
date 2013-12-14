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
#if defined USE_OPENGL

#include "render/nullopenglgraphics.h"

#include "graphicsmanager.h"
#include "graphicsvertexes.h"

#include "resources/image.h"
#include "resources/openglimagehelper.h"

#include "debug.h"

GLuint NullOpenGLGraphics::mLastImage = 0;
#ifdef DEBUG_DRAW_CALLS
unsigned int NullOpenGLGraphics::mDrawCalls = 0;
unsigned int NullOpenGLGraphics::mLastDrawCalls = 0;
#endif

NullOpenGLGraphics::NullOpenGLGraphics():
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
    mOpenGL = RENDER_NULL;
    mName = "null OpenGL";
}

NullOpenGLGraphics::~NullOpenGLGraphics()
{
    delete [] mFloatTexArray;
    delete [] mIntTexArray;
    delete [] mIntVertArray;
}

void NullOpenGLGraphics::initArrays()
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

bool NullOpenGLGraphics::setVideoMode(const int w, const int h,
                                      const int bpp, const bool fs,
                                      const bool hwaccel, const bool resize,
                                      const bool noFrame)
{
    setMainFlags(w, h, bpp, fs, hwaccel, resize, noFrame);

    return setOpenGLMode();
}

static inline void drawQuad(const Image *const image A_UNUSED,
                            const int srcX A_UNUSED, const int srcY A_UNUSED,
                            const int dstX A_UNUSED, const int dstY A_UNUSED,
                            const int width A_UNUSED,
                            const int height A_UNUSED)
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
#ifdef DEBUG_DRAW_CALLS
        NullOpenGLGraphics::mDrawCalls ++;
#endif
    }
    else
    {
#ifdef DEBUG_DRAW_CALLS
        NullOpenGLGraphics::mDrawCalls ++;
#endif
    }
}

static inline void drawRescaledQuad(const Image *const image A_UNUSED,
                                    const int srcX A_UNUSED,
                                    const int srcY A_UNUSED,
                                    const int dstX A_UNUSED,
                                    const int dstY A_UNUSED,
                                    const int width A_UNUSED,
                                    const int height A_UNUSED,
                                    const int desiredWidth A_UNUSED,
                                    const int desiredHeight A_UNUSED)
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
#ifdef DEBUG_DRAW_CALLS
        NullOpenGLGraphics::mDrawCalls ++;
#endif
    }
    else
    {
#ifdef DEBUG_DRAW_CALLS
        NullOpenGLGraphics::mDrawCalls ++;
#endif
    }
}


bool NullOpenGLGraphics::drawImage2(const Image *const image,
                                    int srcX, int srcY,
                                    int dstX, int dstY,
                                    const int width, const int height,
                                    const bool useColor)
{
    FUNC_BLOCK("Graphics::drawImage2", 1)
    if (!image)
        return false;

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

    drawQuad(image, srcX, srcY, dstX, dstY, width, height);

    return true;
}

bool NullOpenGLGraphics::drawRescaledImage(const Image *const image,
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

bool NullOpenGLGraphics::drawRescaledImage(const Image *const image,
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

void NullOpenGLGraphics::drawImagePattern(const Image *const image,
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

void NullOpenGLGraphics::drawRescaledImagePattern(const Image *const image,
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

inline void NullOpenGLGraphics::drawVertexes(const
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

void NullOpenGLGraphics::calcImagePattern(ImageVertexes* const vert,
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

void NullOpenGLGraphics::calcTileCollection(ImageCollection *const vertCol,
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

void NullOpenGLGraphics::drawTileCollection(const ImageCollection
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

void NullOpenGLGraphics::calcImagePattern(ImageCollection* const vertCol,
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

void NullOpenGLGraphics::calcTileVertexes(ImageVertexes *const vert,
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

void NullOpenGLGraphics::drawTileVertexes(const ImageVertexes *const vert)
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

bool NullOpenGLGraphics::calcWindow(ImageCollection *const vertCol,
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

    return calcImageRect(vert, x, y, w, h,
        imgRect.grid[0], imgRect.grid[2], imgRect.grid[6], imgRect.grid[8],
        imgRect.grid[1], imgRect.grid[5], imgRect.grid[7], imgRect.grid[3],
        imgRect.grid[4]);
}

void NullOpenGLGraphics::updateScreen()
{
    BLOCK_START("Graphics::updateScreen")
#ifdef DEBUG_DRAW_CALLS
    mLastDrawCalls = mDrawCalls;
    mDrawCalls = 0;
#endif
    BLOCK_END("Graphics::updateScreen")
}

void NullOpenGLGraphics::_beginDraw()
{
    pushClipArea(gcn::Rectangle(0, 0, 640, 480));
}

void NullOpenGLGraphics::_endDraw()
{
    popClipArea();
}

void NullOpenGLGraphics::prepareScreenshot()
{
}

SDL_Surface* NullOpenGLGraphics::getScreenshot()
{
    return nullptr;
}

bool NullOpenGLGraphics::pushClipArea(gcn::Rectangle area)
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

    return result;
}

void NullOpenGLGraphics::popClipArea()
{
    gcn::Graphics::popClipArea();

    if (mClipStack.empty())
        return;
}

void NullOpenGLGraphics::drawPoint(int x A_UNUSED, int y A_UNUSED)
{
    setTexturingAndBlending(false);
    restoreColor();
}

void NullOpenGLGraphics::drawLine(int x1, int y1,
                                  int x2, int y2)
{
    setTexturingAndBlending(false);
    restoreColor();

    mFloatTexArray[0] = static_cast<float>(x1) + 0.5F;
    mFloatTexArray[1] = static_cast<float>(y1) + 0.5F;
    mFloatTexArray[2] = static_cast<float>(x2) + 0.5F;
    mFloatTexArray[3] = static_cast<float>(y2) + 0.5F;

    drawLineArrayf(4);
}

void NullOpenGLGraphics::drawRectangle(const gcn::Rectangle& rect)
{
    drawRectangle(rect, false);
}

void NullOpenGLGraphics::fillRectangle(const gcn::Rectangle& rect)
{
    drawRectangle(rect, true);
}

void NullOpenGLGraphics::setTexturingAndBlending(const bool enable)
{
    if (enable)
    {
        if (!mTexture)
            mTexture = true;

        if (!mAlpha)
            mAlpha = true;
    }
    else
    {
        mLastImage = 0;
        if (mAlpha && !mColorAlpha)
            mAlpha = false;
        else if (!mAlpha && mColorAlpha)
            mAlpha = true;

        if (mTexture)
            mTexture = false;
    }
}

void NullOpenGLGraphics::drawRectangle(const gcn::Rectangle& rect A_UNUSED,
                                       const bool filled A_UNUSED)
{
    BLOCK_START("Graphics::drawRectangle")
    setTexturingAndBlending(false);
    restoreColor();

#ifdef DEBUG_DRAW_CALLS
        mDrawCalls ++;
#endif
    BLOCK_END("Graphics::drawRectangle")
}

bool NullOpenGLGraphics::drawNet(const int x1, const int y1,
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

void NullOpenGLGraphics::bindTexture(const GLenum target A_UNUSED,
                                     const GLuint texture)
{
    if (mLastImage != texture)
        mLastImage = texture;
}

inline void NullOpenGLGraphics::drawQuadArrayfi(const int size A_UNUSED)
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
}

inline void NullOpenGLGraphics::drawQuadArrayfi(const GLint *const
                                                intVertArray A_UNUSED,
                                                const GLfloat *const
                                                floatTexArray A_UNUSED,
                                                const int size A_UNUSED)
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
}

inline void NullOpenGLGraphics::drawQuadArrayii(const int size A_UNUSED)
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
}

inline void NullOpenGLGraphics::drawQuadArrayii(const GLint *const
                                                intVertArray A_UNUSED,
                                                const GLint *const
                                                intTexArray A_UNUSED,
                                                const int size A_UNUSED)
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
}

inline void NullOpenGLGraphics::drawLineArrayi(const int size A_UNUSED)
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
}

inline void NullOpenGLGraphics::drawLineArrayf(const int size A_UNUSED)
{
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
}

void NullOpenGLGraphics::dumpSettings()
{
}

void NullOpenGLGraphics::setColorAlpha(const float alpha)
{
    if (!mIsByteColor && mFloatColor == alpha)
        return;

    mIsByteColor = false;
    mFloatColor = alpha;
}

void NullOpenGLGraphics::restoreColor()
{
    if (mIsByteColor && mByteColor == mColor)
        return;

    mIsByteColor = true;
    mByteColor = mColor;
}

#ifdef DEBUG_BIND_TEXTURE
void NullOpenGLGraphics::debugBindTexture(const Image *const image)
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
void NullOpenGLGraphics::debugBindTexture(const Image *const image A_UNUSED)
{
}
#endif

#endif  // USE_OPENGL
