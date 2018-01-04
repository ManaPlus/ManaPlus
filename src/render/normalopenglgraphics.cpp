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

#if defined(USE_OPENGL) && !defined(ANDROID) && !defined(__native_client__)

#include "render/normalopenglgraphics.h"

#ifdef OPENGLERRORS
#include "graphicsmanager.h"
#endif  // OPENGLERRORS

#ifdef DEBUG_OPENGL
#include "render/opengl/mgl.h"
#endif  // DEBUG_OPENGL

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

namespace
{
    const void *vertPtr = nullptr;
}  // namespace

GLuint NormalOpenGLGraphics::mTextureBinded = 0;
#ifdef DEBUG_DRAW_CALLS
unsigned int NormalOpenGLGraphics::mDrawCalls = 0;
unsigned int NormalOpenGLGraphics::mLastDrawCalls = 0;
#endif  // DEBUG_DRAW_CALLS
#ifdef DEBUG_BIND_TEXTURE
unsigned int NormalOpenGLGraphics::mBinds = 0;
unsigned int NormalOpenGLGraphics::mLastBinds = 0;
#endif  // DEBUG_BIND_TEXTURE

NormalOpenGLGraphics::NormalOpenGLGraphics() :
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
#endif  // DEBUG_BIND_TEXTURE
    mFbo()
{
    mOpenGL = RENDER_NORMAL_OPENGL;
    mName = "normal OpenGL";
}

NormalOpenGLGraphics::~NormalOpenGLGraphics()
{
    deleteArraysInternal();
}

void NormalOpenGLGraphics::initArrays(const int vertCount) restrict2
{
    mMaxVertices = vertCount;
    if (mMaxVertices < 500)
        mMaxVertices = 500;
    else if (mMaxVertices > 1024)
        mMaxVertices = 1024;

    // need alocate small size, after if limit reached reallocate to double size
    vertexBufSize = mMaxVertices;
    const size_t sz = mMaxVertices * 4 + 30;
    if (mFloatTexArray == nullptr)
        mFloatTexArray = new GLfloat[sz];
    if (mIntTexArray == nullptr)
        mIntTexArray = new GLint[sz];
    if (mIntVertArray == nullptr)
        mIntVertArray = new GLint[sz];
    if (mFloatTexArrayCached == nullptr)
        mFloatTexArrayCached = new GLfloat[sz];
    if (mIntTexArrayCached == nullptr)
        mIntTexArrayCached = new GLint[sz];
    if (mIntVertArrayCached == nullptr)
        mIntVertArrayCached = new GLint[sz];
}

void NormalOpenGLGraphics::deleteArrays() restrict2
{
    deleteArraysInternal();
}

void NormalOpenGLGraphics::deleteArraysInternal() restrict2
{
    delete [] mFloatTexArray;
    mFloatTexArray = nullptr;
    delete [] mIntTexArray;
    mIntTexArray = nullptr;
    delete [] mIntVertArray;
    mIntVertArray = nullptr;
    delete [] mFloatTexArrayCached;
    mFloatTexArrayCached = nullptr;
    delete [] mIntTexArrayCached;
    mIntTexArrayCached = nullptr;
    delete [] mIntVertArrayCached;
    mIntVertArrayCached = nullptr;
}

bool NormalOpenGLGraphics::setVideoMode(const int w, const int h,
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

    return setOpenGLMode();
}

static inline void bindPointerIntFloat(const GLint *restrict const vert,
                                       const GLfloat *restrict const tex)
                                       A_INLINE;

static inline void bindPointerIntFloat(const GLint *restrict const vert,
                                       const GLfloat *restrict const tex)
{
    if (vertPtr != vert)
    {
        vertPtr = vert;
        glVertexPointer(2, GL_INT, 0, vert);
        glTexCoordPointer(2, GL_FLOAT, 0, tex);
    }
}

static inline void bindPointerInt(const GLint *restrict const vert,
                                  const GLint *restrict const tex) A_INLINE;

static inline void bindPointerInt(const GLint *restrict const vert,
                                  const GLint *restrict const tex)
{
    if (vertPtr != vert)
    {
        vertPtr = vert;
        glVertexPointer(2, GL_INT, 0, vert);
        glTexCoordPointer(2, GL_INT, 0, tex);
    }
}

static inline void drawQuad(const Image *restrict const image,
                            const int srcX, const int srcY,
                            const int dstX, const int dstY,
                            const int width, const int height)
                            A_NONNULL(1) A_INLINE;

static inline void drawQuad(const Image *restrict const image,
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

        bindPointerIntFloat(&vert[0], &tex[0]);
#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

        glDrawArrays(GL_QUADS, 0, 4);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
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

        bindPointerInt(&vert[0], &tex[0]);
#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

        glDrawArrays(GL_QUADS, 0, 4);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
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

        bindPointerIntFloat(&vert[0], &tex[0]);
#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

        glDrawArrays(GL_QUADS, 0, 4);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
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

        bindPointerInt(&vert[0], &tex[0]);
#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

        glDrawArrays(GL_QUADS, 0, 4);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
    }
}

void NormalOpenGLGraphics::drawImage(const Image *restrict const image,
                                     int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void NormalOpenGLGraphics::drawImageInline(const Image *restrict const image,
                                           int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (image == nullptr)
        return;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

    const SDL_Rect &imageRect = image->mBounds;
    drawQuad(image, imageRect.x, imageRect.y,
        dstX, dstY, imageRect.w, imageRect.h);
}

void NormalOpenGLGraphics::copyImage(const Image *restrict const image,
                                     int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void NormalOpenGLGraphics::testDraw() restrict2
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        GLfloat tex[] =
        {
            0.0f, 0.781250f,
            0.0f, 0.781250f,
            0.0f, 0.585938f,
            0.0f, 0.585938f
        };

        GLint vert[] =
        {
            0, 0,
            800, 0,
            800, 600,
            0, 600
        };

        bindPointerIntFloat(&vert[0], &tex[0]);
#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

        glDrawArrays(GL_QUADS, 0, 4);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
    }
    else
    {
        GLint tex[] =
        {
            0, 0,
            800, 0,
            800, 600,
            0, 600
        };

        GLint vert[] =
        {
            0, 0,
            800, 0,
            800, 600,
            0, 600
        };

        bindPointerInt(&vert[0], &tex[0]);
#ifdef DEBUG_DRAW_CALLS
        NormalOpenGLGraphics::mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

        glDrawArrays(GL_QUADS, 0, 4);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
    }
}

void NormalOpenGLGraphics::drawImageCached(const Image *restrict const image,
                                           int x, int y) restrict2
{
    if (image == nullptr)
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

void NormalOpenGLGraphics::drawPatternCached(const Image *restrict const image,
                                             const int x,
                                             const int y,
                                             const int w,
                                             const int h) restrict2
{
    FUNC_BLOCK("Graphics::drawPatternCached", 1)
    if (image == nullptr)
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

void NormalOpenGLGraphics::completeCache() restrict2
{
    if (mImageCached == 0u)
        return;

    setColorAlpha(mAlphaCached);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, mImageCached);
    enableTexturingAndBlending();

    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
        drawQuadArrayfiCached(mVpCached);
    else
        drawQuadArrayiiCached(mVpCached);

    mImageCached = 0;
    mVpCached = 0;
}

void NormalOpenGLGraphics::drawRescaledImage(const Image *restrict const image,
                                             int dstX, int dstY,
                                             const int desiredWidth,
                                             const int desiredHeight) restrict2
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    if (image == nullptr)
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
    enableTexturingAndBlending();

    // Draw a textured quad.
    drawRescaledQuad(image, imageRect.x, imageRect.y, dstX, dstY,
        imageRect.w, imageRect.h, desiredWidth, desiredHeight);
}

void NormalOpenGLGraphics::drawPattern(const Image *restrict const image,
                                       const int x, const int y,
                                       const int w, const int h) restrict2
{
    drawPatternInline(image, x, y, w, h);
}

void NormalOpenGLGraphics::drawPatternInline(const Image *restrict const image,
                                             const int x,
                                             const int y,
                                             const int w,
                                             const int h) restrict2
{
    FUNC_BLOCK("Graphics::drawPattern", 1)
    if (image == nullptr)
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

    enableTexturingAndBlending();

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

void NormalOpenGLGraphics::drawRescaledPattern(const Image *
                                               restrict const image,
                                               const int x, const int y,
                                               const int w, const int h,
                                               const int scaledWidth,
                                               const int scaledHeight)
                                               restrict2
{
    if (image == nullptr)
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
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    enableTexturingAndBlending();

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
                                               OpenGLGraphicsVertexes
                                               &restrict ogl) restrict2
{
    const STD_VECTOR<GLint*> &intVertPool = ogl.mIntVertPool;
    STD_VECTOR<GLint*>::const_iterator iv;
    const STD_VECTOR<GLint*>::const_iterator iv_end = intVertPool.end();
    const STD_VECTOR<int> &vp = ogl.mVp;
    STD_VECTOR<int>::const_iterator ivp;
    const STD_VECTOR<int>::const_iterator ivp_end = vp.end();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const STD_VECTOR<GLfloat*> &floatTexPool = ogl.mFloatTexPool;
        STD_VECTOR<GLfloat*>::const_iterator ft;
        const STD_VECTOR<GLfloat*>::const_iterator
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
        const STD_VECTOR<GLint*> &intTexPool = ogl.mIntTexPool;
        STD_VECTOR<GLint*>::const_iterator it;
        const STD_VECTOR<GLint*>::const_iterator it_end = intTexPool.end();

        for (iv = intVertPool.begin(), it = intTexPool.begin(),
             ivp = vp.begin();
             iv != iv_end && it != it_end && ivp != ivp_end;
             ++ iv, ++ it, ++ ivp)
        {
            drawQuadArrayii(*iv, *it, *ivp);
        }
    }
}

void NormalOpenGLGraphics::calcPattern(ImageVertexes *restrict const vert,
                                       const Image *restrict const image,
                                       const int x,
                                       const int y,
                                       const int w,
                                       const int h) const restrict2
{
    calcPatternInline(vert, image, x, y, w, h);
}

void NormalOpenGLGraphics::calcPatternInline(ImageVertexes *
                                             restrict const vert,
                                             const Image *restrict const image,
                                             const int x,
                                             const int y,
                                             const int w,
                                             const int h) const restrict2
{
    if (image == nullptr || vert == nullptr)
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

    OpenGLGraphicsVertexes &ogl = vert->ogl;
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

void NormalOpenGLGraphics::calcTileCollection(ImageCollection *
                                              restrict const vertCol,
                                              const Image *
                                              restrict const image,
                                              int x, int y) restrict2
{
    if (vertCol == nullptr || image == nullptr)
        return;
    if (vertCol->currentGLImage != image->mGLImage)
    {
        ImageVertexes *const vert = new ImageVertexes;
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
                                              *restrict const vertCol)
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
        enableTexturingAndBlending();
        drawVertexes(vert->ogl);
    }
}

void NormalOpenGLGraphics::calcPattern(ImageCollection *restrict const vertCol,
                                       const Image *restrict const image,
                                       const int x,
                                       const int y,
                                       const int w,
                                       const int h) const restrict2
{
    if (vertCol == nullptr || image == nullptr)
        return;
    ImageVertexes *vert = nullptr;
    if (vertCol->currentGLImage != image->mGLImage)
    {
        vert = new ImageVertexes;
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

void NormalOpenGLGraphics::calcTileVertexes(ImageVertexes *restrict const vert,
                                            const Image *restrict const image,
                                            int dstX, int dstY) const restrict2
{
    calcTileVertexesInline(vert, image, dstX, dstY);
}

void NormalOpenGLGraphics::calcTileVertexesInline(ImageVertexes *
                                                  restrict const vert,
                                                  const Image *
                                                  restrict const image,
                                                  int dstX,
                                                  int dstY) const restrict2
{
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

    OpenGLGraphicsVertexes &ogl = vert->ogl;

//    STD_VECTOR<int> *vps = ogl.getVp();
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

void NormalOpenGLGraphics::drawTileVertexes(const ImageVertexes *
                                            restrict const vert) restrict2
{
    if (vert == nullptr)
        return;
    const Image *const image = vert->image;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();
    drawVertexes(vert->ogl);
}

void NormalOpenGLGraphics::calcWindow(ImageCollection *restrict const vertCol,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const ImageRect &restrict imgRect)
                                      restrict2
{
    ImageVertexes *vert = nullptr;
    Image *const image = imgRect.grid[4];
    if (image == nullptr)
        return;
    if (vertCol->currentGLImage != image->mGLImage)
    {
        vert = new ImageVertexes;
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

void NormalOpenGLGraphics::updateScreen() restrict2
{
    BLOCK_START("Graphics::updateScreen")
//    glFlush();
//    glFinish();
#ifdef DEBUG_DRAW_CALLS
    mLastDrawCalls = mDrawCalls;
    mDrawCalls = 0;
#endif  // DEBUG_DRAW_CALLS
#ifdef DEBUG_BIND_TEXTURE
    mLastBinds = mBinds;
    mBinds = 0;
#endif  // DEBUG_BIND_TEXTURE
#ifdef USE_SDL2
    SDL_GL_SwapWindow(mWindow);
#else  // USE_SDL2
    SDL_GL_SwapBuffers();
#endif  // USE_SDL2
#ifdef DEBUG_OPENGL
    if (isGLNotNull(mglFrameTerminator))
        mglFrameTerminator();
#endif  // DEBUG_OPENGL

// may be need clear?
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    BLOCK_END("Graphics::updateScreen")
}

void NormalOpenGLGraphics::beginDraw() restrict2
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
#else  // ANDROID

    glOrtho(0.0, static_cast<double>(w),
        static_cast<double>(h),
        0.0, -1.0, 1.0);
#endif  // ANDROID

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setOpenGLFlags();
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_COLOR_MATERIAL);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glShadeModel(GL_FLAT);
    glDepthMask(GL_FALSE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
#ifndef ANDROID
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
#endif  // ANDROID

    pushClipArea(Rect(0, 0, w, h));
}

void NormalOpenGLGraphics::endDraw() restrict2
{
    popClipArea();
}

void NormalOpenGLGraphics::pushClipArea(const Rect &restrict area) restrict2
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

    if (transX != 0 || transY != 0)
    {
        glTranslatef(static_cast<GLfloat>(transX),
                     static_cast<GLfloat>(transY), 0);
    }
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void NormalOpenGLGraphics::popClipArea() restrict2
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
    if (transX != 0 || transY != 0)
    {
        glTranslatef(static_cast<GLfloat>(transX),
                     static_cast<GLfloat>(transY), 0);
    }
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void NormalOpenGLGraphics::drawPoint(int x, int y) restrict2
{
    disableTexturingAndBlending();
    restoreColor();

#ifdef ANDROID
    // TODO need fix
#else  // ANDROID

    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
#endif  // ANDROID
}

void NormalOpenGLGraphics::drawLine(int x1, int y1,
                                    int x2, int y2) restrict2
{
    disableTexturingAndBlending();
    restoreColor();

    mFloatTexArray[0] = static_cast<float>(x1) + 0.5F;
    mFloatTexArray[1] = static_cast<float>(y1) + 0.5F;
    mFloatTexArray[2] = static_cast<float>(x2) + 0.5F;
    mFloatTexArray[3] = static_cast<float>(y2) + 0.5F;

    drawLineArrayf(4);
}

void NormalOpenGLGraphics::drawRectangle(const Rect &restrict rect) restrict2
{
    drawRectangle(rect, false);
}

void NormalOpenGLGraphics::fillRectangle(const Rect &restrict rect) restrict2
{
    drawRectangle(rect, true);
}

void NormalOpenGLGraphics::enableTexturingAndBlending() restrict2
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

void NormalOpenGLGraphics::disableTexturingAndBlending() restrict2
{
    mTextureBinded = 0;
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

void NormalOpenGLGraphics::drawRectangle(const Rect &restrict rect,
                                         const bool filled) restrict2
{
    BLOCK_START("Graphics::drawRectangle")
    const float offset = filled ? 0 : 0.5F;
    const float x = static_cast<float>(rect.x);
    const float y = static_cast<float>(rect.y);
    const float width = static_cast<float>(rect.width);
    const float height = static_cast<float>(rect.height);

    disableTexturingAndBlending();
    restoreColor();

    GLfloat vert[] =
    {
        x + offset, y + offset,
        x + width - offset, y + offset,
        x + width - offset, y + height - offset,
        x + offset, y + height - offset
    };

    glVertexPointer(2, GL_FLOAT, 0, &vert);
    vertPtr = nullptr;
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(filled ? GL_QUADS : GL_LINE_LOOP, 0, 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
    BLOCK_END("Graphics::drawRectangle")
}

void NormalOpenGLGraphics::drawNet(const int x1, const int y1,
                                   const int x2, const int y2,
                                   const int width, const int height) restrict2
{
    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    disableTexturingAndBlending();
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
}

void NormalOpenGLGraphics::bindTexture(const GLenum target,
                                       const GLuint texture)
{
    if (mTextureBinded != texture)
    {
        mTextureBinded = texture;
        glBindTexture(target, texture);
#ifdef DEBUG_BIND_TEXTURE
        mBinds ++;
#endif  // DEBUG_BIND_TEXTURE
    }
}

inline void NormalOpenGLGraphics::drawQuadArrayfi(const int size) restrict2
{
    bindPointerIntFloat(&mIntVertArray[0], &mFloatTexArray[0]);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(GL_QUADS, 0, size / 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

inline void NormalOpenGLGraphics::drawQuadArrayfiCached(const int size)
                                                        restrict2
{
    bindPointerIntFloat(&mIntVertArrayCached[0], &mFloatTexArrayCached[0]);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(GL_QUADS, 0, size / 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

inline void NormalOpenGLGraphics::drawQuadArrayfi(const GLint *restrict const
                                                  intVertArray,
                                                  const GLfloat *restrict const
                                                  floatTexArray,
                                                  const int size) restrict2
{
    vertPtr = intVertArray;
    glVertexPointer(2, GL_INT, 0, intVertArray);
    glTexCoordPointer(2, GL_FLOAT, 0, floatTexArray);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(GL_QUADS, 0, size / 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

inline void NormalOpenGLGraphics::drawQuadArrayii(const int size) restrict2
{
    bindPointerInt(&mIntVertArray[0], &mIntTexArray[0]);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(GL_QUADS, 0, size / 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

inline void NormalOpenGLGraphics::drawQuadArrayiiCached(const int size)
                                                        restrict2
{
    bindPointerInt(&mIntVertArrayCached[0], &mIntTexArrayCached[0]);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(GL_QUADS, 0, size / 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

inline void NormalOpenGLGraphics::drawQuadArrayii(const GLint *restrict const
                                                  intVertArray,
                                                  const GLint *restrict const
                                                  intTexArray,
                                                  const int size) restrict2
{
    vertPtr = intVertArray;
    glVertexPointer(2, GL_INT, 0, intVertArray);
    glTexCoordPointer(2, GL_INT, 0, intTexArray);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(GL_QUADS, 0, size / 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

inline void NormalOpenGLGraphics::drawLineArrayi(const int size) restrict2
{
    glVertexPointer(2, GL_INT, 0, mIntVertArray);
    vertPtr = nullptr;
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(GL_LINES, 0, size / 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

inline void NormalOpenGLGraphics::drawLineArrayf(const int size) restrict2
{
    glVertexPointer(2, GL_FLOAT, 0, mFloatTexArray);
    vertPtr = nullptr;
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS

    glDrawArrays(GL_LINES, 0, size / 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
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
        if (test[0] != 0 || test[1] != 0 || test[2] != 0 || test[3] != 0)
        {
            logger->log("\n%d = %d, %d, %d, %d", f,
                test[0], test[1], test[2], test[3]);
        }
    }
}

void NormalOpenGLGraphics::setColorAlpha(const float alpha) restrict2
{
    if (!mIsByteColor && mFloatColor == alpha)
        return;

    glColor4f(1.0F, 1.0F, 1.0F, alpha);
    mIsByteColor = false;
    mFloatColor = alpha;
}

void NormalOpenGLGraphics::restoreColor() restrict2
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
                                         const ImageRect &restrict imgRect)
                                         restrict2
{
    #include "render/graphics_drawImageRect.hpp"
}

void NormalOpenGLGraphics::calcImageRect(ImageVertexes *restrict const vert,
                                         const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &restrict imgRect)
                                         restrict2
{
    #include "render/graphics_calcImageRect.hpp"
}

void NormalOpenGLGraphics::clearScreen() const restrict2
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

#ifdef DEBUG_BIND_TEXTURE
void NormalOpenGLGraphics::debugBindTexture(const Image *restrict const image)
                                            restrict2
{
    const std::string texture = image->mIdPath;
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

void NormalOpenGLGraphics::debugBindTexture(const Image *restrict const
                                            image A_UNUSED) restrict2
{
}
#endif  // DEBUG_BIND_TEXTURE

#endif  // defined(USE_OPENGL) && !defined(ANDROID) &&
        // !defined(__native_client__)
