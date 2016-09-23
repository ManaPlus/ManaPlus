/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#if defined(USE_OPENGL) && !defined(__native_client__)

#include "render/mobileopenglgraphics.h"

#include "render/opengl/mgl.h"
#ifdef __native_client__
#include "render/opengl/naclglfunctions.h"
#endif  // __native_client__

#include "render/vertexes/imagecollection.h"

#include "resources/imagerect.h"
#include "resources/openglimagehelper.h"

#include "resources/image/image.h"

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
    tVar[vp + 7] = y1; \
    tVar[vp + 8] = x1; \
    tVar[vp + 9] = y2; \
    tVar[vp + 10] = x2; \
    tVar[vp + 11] = y2; \
    vVar[vp + 0] = static_cast<GLshort>(dstX); \
    vVar[vp + 1] = static_cast<GLshort>(dstY); \
    vVar[vp + 2] = static_cast<GLshort>(dstX + w); \
    vVar[vp + 3] = static_cast<GLshort>(dstY); \
    vVar[vp + 4] = static_cast<GLshort>(dstX + w); \
    vVar[vp + 5] = static_cast<GLshort>(dstY + h); \
    vVar[vp + 6] = static_cast<GLshort>(dstX); \
    vVar[vp + 7] = static_cast<GLshort>(dstY); \
    vVar[vp + 8] = static_cast<GLshort>(dstX); \
    vVar[vp + 9] = static_cast<GLshort>(dstY + h); \
    vVar[vp + 10] = static_cast<GLshort>(dstX + w); \
    vVar[vp + 11] = static_cast<GLshort>(dstY + h);

GLuint MobileOpenGLGraphics::mTextureBinded = 0;
#ifdef DEBUG_DRAW_CALLS
unsigned int MobileOpenGLGraphics::mDrawCalls = 0;
unsigned int MobileOpenGLGraphics::mLastDrawCalls = 0;
#endif  // DEBUG_DRAW_CALLS

MobileOpenGLGraphics::MobileOpenGLGraphics() :
    mFloatTexArray(nullptr),
    mShortVertArray(nullptr),
    mFloatTexArrayCached(nullptr),
    mShortVertArrayCached(nullptr),
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
#endif  // DEBUG_BIND_TEXTURE
    mFbo()
{
    mOpenGL = RENDER_GLES_OPENGL;
    mName = "mobile OpenGL ES";
}

MobileOpenGLGraphics::~MobileOpenGLGraphics()
{
    deleteArraysInternal();
}

void MobileOpenGLGraphics::postInit() restrict2
{
//    glesTest();
}

void MobileOpenGLGraphics::initArrays(const int vertCount) restrict2
{
    mMaxVertices = vertCount;
    if (mMaxVertices < 500)
        mMaxVertices = 500;
    else if (mMaxVertices > 1024)
        mMaxVertices = 1024;

    // need alocate small size, after if limit reached reallocate to double size
    const size_t sz = mMaxVertices * 4 + 30;
    vertexBufSize = mMaxVertices;
    if (!mFloatTexArray)
        mFloatTexArray = new GLfloat[sz];
    if (!mShortVertArray)
        mShortVertArray = new GLshort[sz];
    if (!mFloatTexArrayCached)
        mFloatTexArrayCached = new GLfloat[sz];
    if (!mShortVertArrayCached)
        mShortVertArrayCached = new GLshort[sz];
}

void MobileOpenGLGraphics::deleteArrays() restrict2
{
    deleteArraysInternal();
}

void MobileOpenGLGraphics::deleteArraysInternal() restrict2
{
    delete [] mFloatTexArray;
    mFloatTexArray = nullptr;
    delete [] mShortVertArray;
    mShortVertArray = nullptr;
    delete [] mFloatTexArrayCached;
    mFloatTexArrayCached = nullptr;
    delete [] mShortVertArrayCached;
    mShortVertArrayCached = nullptr;
}

bool MobileOpenGLGraphics::setVideoMode(const int w, const int h,
                                        const int scale,
                                        const int bpp,
                                        const bool fs,
                                        const bool hwaccel,
                                        const bool resize,
                                        const bool noFrame) restrict2
{
    setMainFlags(w, h, scale, bpp, fs, hwaccel, resize, noFrame);

    return setOpenGLMode();
}

static inline void drawQuad(const Image *restrict const image,
                            const int srcX,
                            const int srcY,
                            const int dstX,
                            const int dstY,
                            const int width,
                            const int height) A_NONNULL(1) A_INLINE;

static inline void drawQuad(const Image *restrict const image,
                            const int srcX,
                            const int srcY,
                            const int dstX,
                            const int dstY,
                            const int width,
                            const int height)
{
//    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
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
            texX1, texY2,
            texX2, texY2
        };

        GLshort vert[] =
        {
            static_cast<GLshort>(dstX), static_cast<GLshort>(dstY),
            static_cast<GLshort>(dstX + width), static_cast<GLshort>(dstY),
            static_cast<GLshort>(dstX), static_cast<GLshort>(dstY + height),
            static_cast<GLshort>(dstX + width),
                static_cast<GLshort>(dstY + height)
        };

        glVertexPointer(2, GL_SHORT, 0, &vert);
        glTexCoordPointer(2, GL_FLOAT, 0, &tex);

#ifdef DEBUG_DRAW_CALLS
        MobileOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
        mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

static inline void drawRescaledQuad(const Image *restrict const image,
                                    const int srcX, const int srcY,
                                    const int dstX, const int dstY,
                                    const int width, const int height,
                                    const int desiredWidth,
                                    const int desiredHeight)
                                    A_NONNULL(1) A_INLINE;

static inline void drawRescaledQuad(const Image *restrict const image,
                                    const int srcX, const int srcY,
                                    const int dstX, const int dstY,
                                    const int width, const int height,
                                    const int desiredWidth,
                                    const int desiredHeight)
{
//    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
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
            texX1, texY2,
            texX2, texY2
        };

        GLshort vert[] =
        {
            static_cast<GLshort>(dstX), static_cast<GLshort>(dstY),
            static_cast<GLshort>(dstX + desiredWidth),
                static_cast<GLshort>(dstY),
            static_cast<GLshort>(dstX), static_cast<GLshort>(
                dstY + desiredHeight),
            static_cast<GLshort>(dstX + desiredWidth),
                static_cast<GLshort>(dstY + desiredHeight)
        };
        glVertexPointer(2, GL_SHORT, 0, &vert);
        glTexCoordPointer(2, GL_FLOAT, 0, &tex);

#ifdef DEBUG_DRAW_CALLS
        MobileOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
        mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

void MobileOpenGLGraphics::drawImage(const Image *restrict const image,
                                     int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void MobileOpenGLGraphics::drawImageInline(const Image *restrict const image,
                                           int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (!image)
        return;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    const SDL_Rect &imageRect = image->mBounds;
    drawQuad(image, imageRect.x, imageRect.y,
        dstX, dstY, imageRect.w, imageRect.h);
}

void MobileOpenGLGraphics::copyImage(const Image *restrict const image,
                                     int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void MobileOpenGLGraphics::drawImageCached(const Image *restrict const image,
                                           int x, int y) restrict2
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
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int w = imageRect.w;
    const int h = imageRect.h;

    if (w == 0 || h == 0)
        return;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);

    const unsigned int vLimit = mMaxVertices * 4;

    unsigned int vp = mVpCached;

    // Draw a set of textured rectangles
//    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        float texX1 = static_cast<float>(srcX) / tw;
        float texY1 = static_cast<float>(srcY) / th;
        float texX2 = static_cast<float>(srcX + w) / tw;
        float texY2 = static_cast<float>(srcY + h) / th;

        vertFill2D(mFloatTexArrayCached, mShortVertArrayCached,
            texX1, texY1, texX2, texY2,
            x, y, w, h);

        vp += 12;
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

void MobileOpenGLGraphics::drawPatternCached(const Image *restrict const image,
                                             const int x,
                                             const int y,
                                             const int w,
                                             const int h) restrict2
{
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
    const float texX1 = static_cast<float>(srcX) / tw;
    const float texY1 = static_cast<float>(srcY) / th;

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const float texY2 = static_cast<float>(srcY + height) / th;
        const int dstY = y + py;
        for (int px = 0; px < w; px += iw)
        {
            const int width = (px + iw >= w) ? w - px : iw;
            const int dstX = x + px;

            const float texX2 = static_cast<float>(srcX + width) / tw;

            vertFill2D(mFloatTexArrayCached, mShortVertArrayCached,
                texX1, texY1, texX2, texY2,
                dstX, dstY, width, height);

            vp += 12;
            if (vp >= vLimit)
            {
                completeCache();
                vp = 0;
            }
        }
    }
    mVpCached = vp;
}

void MobileOpenGLGraphics::completeCache() restrict2
{
    if (!mImageCached)
        return;

    setColorAlpha(mAlphaCached);
#ifdef DEBUG_BIND_TEXTURE
//    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture(OpenGLImageHelper::mTextureType, mImageCached);
    setTexturingAndBlending(true);

    drawTriangleArrayfsCached(mVpCached);
    mImageCached = 0;
    mVpCached = 0;
}

void MobileOpenGLGraphics::drawRescaledImage(const Image *restrict const image,
                                             int dstX, int dstY,
                                             const int desiredWidth,
                                             const int desiredHeight) restrict2
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    if (!image)
        return;

    const SDL_Rect &imageRect = image->mBounds;

    // Just draw the image normally when no resizing is necessary,
    if (imageRect.w == desiredWidth && imageRect.h == desiredHeight)
    {
        drawImageInline(image, dstX, dstY);
        return;
    }

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    // Draw a textured quad.
    drawRescaledQuad(image, imageRect.x, imageRect.y, dstX, dstY,
        imageRect.w, imageRect.h, desiredWidth, desiredHeight);
}

void MobileOpenGLGraphics::drawPattern(const Image *restrict const image,
                                       const int x, const int y,
                                       const int w, const int h) restrict2
{
    drawPatternInline(image, x, y, w, h);
}

void MobileOpenGLGraphics::drawPatternInline(const Image *restrict const image,
                                             const int x,
                                             const int y,
                                             const int w,
                                             const int h) restrict2
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
#endif  // DEBUG_BIND_TEXTURE
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;
    // Draw a set of textured rectangles
    const float texX1 = static_cast<float>(srcX) / tw;
    const float texY1 = static_cast<float>(srcY) / th;

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const float texY2 = static_cast<float>(srcY + height) / th;
        const int dstY = y + py;
        for (int px = 0; px < w; px += iw)
        {
            const int width = (px + iw >= w) ? w - px : iw;
            const int dstX = x + px;

            const float texX2 = static_cast<float>(srcX + width) / tw;

            vertFill2D(mFloatTexArray, mShortVertArray,
                texX1, texY1, texX2, texY2,
                dstX, dstY, width, height);

            vp += 12;
            if (vp >= vLimit)
            {
                drawTriangleArrayfs(vp);
                vp = 0;
            }
        }
    }
    if (vp > 0)
        drawTriangleArrayfs(vp);
}

void MobileOpenGLGraphics::drawRescaledPattern(const Image *
                                               restrict const image,
                                               const int x, const int y,
                                               const int w, const int h,
                                               const int scaledWidth,
                                               const int scaledHeight)
                                               restrict2
{
    if (!image)
        return;

    if (scaledWidth == 0 || scaledHeight == 0)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

    setColorAlpha(image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    // Draw a set of textured rectangles
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

            vertFill2D(mFloatTexArray, mShortVertArray,
                texX1, texY1, texX2, texY2,
                dstX, dstY, width, height);

            vp += 12;
            if (vp >= vLimit)
            {
                drawTriangleArrayfs(vp);
                vp = 0;
            }
        }
    }
    if (vp > 0)
        drawTriangleArrayfs(vp);
}

inline void MobileOpenGLGraphics::drawVertexes(const
                                               OpenGLGraphicsVertexes
                                               &restrict ogl) restrict2
{
    const std::vector<GLshort*> &shortVertPool = ogl.mShortVertPool;
    std::vector<GLshort*>::const_iterator iv;
    const std::vector<GLshort*>::const_iterator iv_end = shortVertPool.end();
    const std::vector<int> &vp = ogl.mVp;
    std::vector<int>::const_iterator ivp;
    const std::vector<int>::const_iterator ivp_end = vp.end();

    // Draw a set of textured rectangles
//    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const std::vector<GLfloat*> &floatTexPool = ogl.mFloatTexPool;
        std::vector<GLfloat*>::const_iterator ft;
        const std::vector<GLfloat*>::const_iterator
            ft_end = floatTexPool.end();

        for (iv = shortVertPool.begin(), ft = floatTexPool.begin(),
             ivp = vp.begin();
             iv != iv_end && ft != ft_end && ivp != ivp_end;
             ++ iv, ++ ft, ++ ivp)
        {
            drawTriangleArrayfs(*iv, *ft, *ivp);
        }
    }
}

void MobileOpenGLGraphics::calcPattern(ImageVertexes *restrict const vert,
                                       const Image *restrict const image,
                                       const int x,
                                       const int y,
                                       const int w,
                                       const int h) const restrict2
{
    calcPatternInline(vert, image, x, y, w, h);
}

void MobileOpenGLGraphics::calcPatternInline(ImageVertexes *
                                             restrict const vert,
                                             const Image *restrict const image,
                                             const int x,
                                             const int y,
                                             const int w,
                                             const int h) const restrict2
{
    if (!image || !vert)
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

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;
    unsigned int vp = ogl.continueVp();

    // Draw a set of textured rectangles
//    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        GLfloat *floatTexArray = ogl.continueFloatTexArray();
        GLshort *shortVertArray = ogl.continueShortVertArray();

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

                vertFill2D(floatTexArray, shortVertArray,
                    texX1, texY1, texX2, texY2,
                    dstX, dstY, width, height);

                vp += 12;
                if (vp >= vLimit)
                {
                    floatTexArray = ogl.switchFloatTexArray();
                    shortVertArray = ogl.switchShortVertArray();
                    ogl.switchVp(vp);
                    vp = 0;
                }
            }
        }
    }
    ogl.switchVp(vp);
}

void MobileOpenGLGraphics::calcTileCollection(ImageCollection *
                                              restrict const vertCol,
                                              const Image *
                                              restrict const image,
                                              int x, int y) restrict2
{
    if (!vertCol || !image)
        return;
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

void MobileOpenGLGraphics::drawTileCollection(const ImageCollection *
                                              restrict const vertCol)
                                              restrict2
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
#endif  // DEBUG_BIND_TEXTURE
        bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
        setTexturingAndBlending(true);
        drawVertexes(vert->ogl);
    }
}

void MobileOpenGLGraphics::calcPattern(ImageCollection *restrict const vertCol,
                                       const Image *restrict const image,
                                       const int x,
                                       const int y,
                                       const int w,
                                       const int h) const restrict2
{
    if (!vertCol || !image)
        return;
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

void MobileOpenGLGraphics::calcTileVertexes(ImageVertexes *restrict const vert,
                                            const Image *restrict const image,
                                            int dstX, int dstY) const restrict2
{
    calcTileVertexesInline(vert, image, dstX, dstY);
}

void MobileOpenGLGraphics::calcTileVertexesInline(ImageVertexes *
                                                  restrict const vert,
                                                  const Image *
                                                  restrict const image,
                                                  int dstX,
                                                  int dstY) const restrict2
{
    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int w = imageRect.w;
    const int h = imageRect.h;

    if (w == 0 || h == 0)
        return;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;

//    std::vector<int> *vps = ogl.getVp();
    unsigned int vp = ogl.continueVp();

    // Draw a set of textured rectangles
//    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        float texX1 = static_cast<float>(srcX) / tw;
        float texY1 = static_cast<float>(srcY) / th;
        float texX2 = static_cast<float>(srcX + w) / tw;
        float texY2 = static_cast<float>(srcY + h) / th;

        GLfloat *const floatTexArray = ogl.continueFloatTexArray();
        GLshort *const shortVertArray = ogl.continueShortVertArray();

        vertFill2D(floatTexArray, shortVertArray,
            texX1, texY1, texX2, texY2,
            dstX, dstY, w, h);

        vp += 12;
        if (vp >= vLimit)
        {
            ogl.switchFloatTexArray();
            ogl.switchShortVertArray();
            ogl.switchVp(vp);
            vp = 0;
        }
    }
    ogl.switchVp(vp);
}

void MobileOpenGLGraphics::drawTileVertexes(const ImageVertexes *
                                            restrict const vert) restrict2
{
    if (!vert)
        return;
    const Image *const image = vert->image;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);
    drawVertexes(vert->ogl);
}

void MobileOpenGLGraphics::calcWindow(ImageCollection *restrict const vertCol,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const ImageRect &restrict imgRect)
                                      restrict2
{
    ImageVertexes *vert = nullptr;
    const Image *const image = imgRect.grid[4];
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


void MobileOpenGLGraphics::updateScreen() restrict2
{
    BLOCK_START("Graphics::updateScreen")
//    glFlush();
//    glFinish();
#ifdef DEBUG_DRAW_CALLS
    mLastDrawCalls = mDrawCalls;
    mDrawCalls = 0;
#endif  // DEBUG_DRAW_CALLS
#ifdef USE_SDL2
    SDL_GL_SwapWindow(mWindow);
#else  // USE_SDL2

    SDL_GL_SwapBuffers();
#endif  // USE_SDL2
#ifdef DEBUG_OPENGL
    if (isGLNotNull(mglFrameTerminator))
        mglFrameTerminator();
#endif  // DEBUG_OPENGL

    BLOCK_END("Graphics::updateScreen")
}

void MobileOpenGLGraphics::beginDraw() restrict2
{
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

#ifdef ANDROID
    glOrthof(0.0, static_cast<float>(mRect.w),
        static_cast<float>(mRect.h),
        0.0, -1.0, 1.0);
#else  // ANDROID

    glOrtho(0.0, static_cast<double>(mRect.w),
        static_cast<double>(mRect.h),
        0.0, -1.0, 1.0);
#endif  // ANDROID

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setOpenGLFlags();
    mglDisable(GL_LIGHTING);
    mglDisable(GL_FOG);
    mglDisable(GL_COLOR_MATERIAL);

    glShadeModel(GL_FLAT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

#ifndef ANDROID
    mglHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    mglHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    mglHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    mglHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
#endif  // ANDROID

    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void MobileOpenGLGraphics::endDraw() restrict2
{
    popClipArea();
}

void MobileOpenGLGraphics::pushClipArea(const Rect &restrict area) restrict2
{
    int transX = 0;
    int transY = 0;

    if (!mClipStack.empty())
    {
        const ClipRect &clipArea = mClipStack.top();
        transX = -clipArea.xOffset;
        transY = -clipArea.yOffset;
    }

    Graphics::pushClipArea(area);

    const ClipRect &clipArea = mClipStack.top();
    transX += clipArea.xOffset;
    transY += clipArea.yOffset;

    if (transX || transY)
    {
        glTranslatef(static_cast<GLfloat>(transX),
                     static_cast<GLfloat>(transY), 0);
    }
    mglScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void MobileOpenGLGraphics::popClipArea() restrict2
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
    mglScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

#ifdef ANDROID
void MobileOpenGLGraphics::drawPoint(int x A_UNUSED, int y A_UNUSED) restrict2
#else  // ANDROID
void MobileOpenGLGraphics::drawPoint(int x, int y) restrict2
#endif  // ANDROID
{
    setTexturingAndBlending(false);
    restoreColor();

#ifdef ANDROID
    // TODO need fix
#else  // ANDROID

    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
#endif  // ANDROID
}

void MobileOpenGLGraphics::drawLine(int x1, int y1,
                                    int x2, int y2) restrict2
{
    setTexturingAndBlending(false);
    restoreColor();

    mShortVertArray[0] = static_cast<GLshort>(x1);
    mShortVertArray[1] = static_cast<GLshort>(y1);
    mShortVertArray[2] = static_cast<GLshort>(x2);
    mShortVertArray[3] = static_cast<GLshort>(y2);

    drawLineArrays(4);
}

void MobileOpenGLGraphics::drawRectangle(const Rect &restrict rect) restrict2
{
    drawRectangle(rect, false);
}

void MobileOpenGLGraphics::fillRectangle(const Rect &restrict rect) restrict2
{
    drawRectangle(rect, true);
}

void MobileOpenGLGraphics::setTexturingAndBlending(const bool enable) restrict2
{
    if (enable)
    {
        if (!mTexture)
        {
            mglEnable(OpenGLImageHelper::mTextureType);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            mTexture = true;
        }

        if (!mAlpha)
        {
            mglEnable(GL_BLEND);
            mAlpha = true;
        }
    }
    else
    {
        mTextureBinded = 0;
        if (mAlpha && !mColorAlpha)
        {
            mglDisable(GL_BLEND);
            mAlpha = false;
        }
        else if (!mAlpha && mColorAlpha)
        {
            mglEnable(GL_BLEND);
            mAlpha = true;
        }

        if (mTexture)
        {
            mglDisable(OpenGLImageHelper::mTextureType);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            mTexture = false;
        }
    }
}

void MobileOpenGLGraphics::drawRectangle(const Rect &restrict rect,
                                         const bool filled) restrict2
{
    BLOCK_START("Graphics::drawRectangle")
    setTexturingAndBlending(false);
    restoreColor();

    const GLshort x = static_cast<GLshort>(rect.x);
    const GLshort y = static_cast<GLshort>(rect.y);
    const GLshort width = static_cast<GLshort>(rect.width);
    const GLshort height = static_cast<GLshort>(rect.height);
    const GLshort xw = static_cast<GLshort>(rect.x + width);
    const GLshort yh = static_cast<GLshort>(rect.y + height);

    if (filled)
    {
        GLshort vert[] =
        {
            x, y,
            xw, y,
            x, yh,
            xw, yh
        };

        glVertexPointer(2, GL_SHORT, 0, &vert);
#ifdef DEBUG_DRAW_CALLS
        mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

        mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    else
    {
        GLshort vert[] =
        {
            x, y,
            xw, y,
            xw, yh,
            x, yh
        };

        glVertexPointer(2, GL_SHORT, 0, &vert);
#ifdef DEBUG_DRAW_CALLS
        mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

        mglDrawArrays(GL_LINE_LOOP, 0, 4);
    }
    BLOCK_END("Graphics::drawRectangle")
}

void MobileOpenGLGraphics::drawNet(const int x1, const int y1,
                                   const int x2, const int y2,
                                   const int width, const int height) restrict2
{
    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    setTexturingAndBlending(false);
    restoreColor();

    const GLshort xs1 = static_cast<GLshort>(x1);
    const GLshort xs2 = static_cast<GLshort>(x2);
    const GLshort ys1 = static_cast<GLshort>(y1);
    const GLshort ys2 = static_cast<GLshort>(y2);
    const int16_t width1 = CAST_S16(width);
    const int16_t height1 = CAST_S16(height);

    for (int16_t y = CAST_S16(y1); y < y2; y += height1)
    {
        mShortVertArray[vp + 0] = xs1;
        mShortVertArray[vp + 1] = y;

        mShortVertArray[vp + 2] = xs2;
        mShortVertArray[vp + 3] = y;

        vp += 4;
        if (vp >= vLimit)
        {
            drawLineArrays(vp);
            vp = 0;
        }
    }

    for (int16_t x = CAST_S16(x1); x < x2; x += width1)
    {
        mShortVertArray[vp + 0] = x;
        mShortVertArray[vp + 1] = ys1;

        mShortVertArray[vp + 2] = x;
        mShortVertArray[vp + 3] = ys2;

        vp += 4;
        if (vp >= vLimit)
        {
            drawLineArrays(vp);
            vp = 0;
        }
    }

    if (vp > 0)
        drawLineArrays(vp);
}

void MobileOpenGLGraphics::bindTexture(const GLenum target,
                                       const GLuint texture)
{
    if (mTextureBinded != texture)
    {
        mTextureBinded = texture;
        mglBindTexture(target, texture);
    }
}

inline void MobileOpenGLGraphics::drawTriangleArrayfs(const int size) restrict2
{
    glVertexPointer(2, GL_SHORT, 0, mShortVertArray);
    glTexCoordPointer(2, GL_FLOAT, 0, mFloatTexArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    mglDrawArrays(GL_TRIANGLES, 0, size / 2);
}

inline void MobileOpenGLGraphics::drawTriangleArrayfsCached(const int size)
                                                            restrict2
{
    glVertexPointer(2, GL_SHORT, 0, mShortVertArrayCached);
    glTexCoordPointer(2, GL_FLOAT, 0, mFloatTexArrayCached);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    mglDrawArrays(GL_TRIANGLES, 0, size / 2);
}

inline void MobileOpenGLGraphics::drawTriangleArrayfs(const GLshort *restrict
                                                      const shortVertArray,
                                                      const GLfloat *restrict
                                                      const floatTexArray,
                                                      const int size) restrict2
{
    glVertexPointer(2, GL_SHORT, 0, shortVertArray);
    glTexCoordPointer(2, GL_FLOAT, 0, floatTexArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    mglDrawArrays(GL_TRIANGLES, 0, size / 2);
}

inline void MobileOpenGLGraphics::drawLineArrays(const int size) restrict2
{
    glVertexPointer(2, GL_SHORT, 0, mShortVertArray);

#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    mglDrawArrays(GL_LINES, 0, size / 2);
}

void MobileOpenGLGraphics::dumpSettings()
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
        mglGetIntegerv(f, &test[0]);
        if (test[0] || test[1] || test[2] || test[3])
        {
            logger->log("\n%d = %d, %d, %d, %d", f,
                test[0], test[1], test[2], test[3]);
        }
    }
}

void MobileOpenGLGraphics::setColorAlpha(const float alpha) restrict2
{
    if (!mIsByteColor && mFloatColor == alpha)
        return;

    glColor4f(1.0F, 1.0F, 1.0F, alpha);
    mIsByteColor = false;
    mFloatColor = alpha;
}

void MobileOpenGLGraphics::restoreColor() restrict2
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

void MobileOpenGLGraphics::drawImageRect(const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &restrict imgRect)
                                         restrict2
{
    #include "render/graphics_drawImageRect.hpp"
}

void MobileOpenGLGraphics::calcImageRect(ImageVertexes *restrict const vert,
                                         const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &restrict imgRect)
                                         restrict2
{
    #include "render/graphics_calcImageRect.hpp"
}

void MobileOpenGLGraphics::clearScreen() const restrict2
{
    mglClear(GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT |
        GL_STENCIL_BUFFER_BIT);
}

#ifdef DEBUG_BIND_TEXTURE
void MobileOpenGLGraphics::debugBindTexture(const Image *restrict const image)
                                            restrict2
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
#else  // DEBUG_BIND_TEXTURE

void MobileOpenGLGraphics::debugBindTexture(const Image *restrict const
                                            image A_UNUSED) restrict2
{
}
#endif  // DEBUG_BIND_TEXTURE

#endif  // defined(USE_OPENGL) && !defined(__native_client__)
