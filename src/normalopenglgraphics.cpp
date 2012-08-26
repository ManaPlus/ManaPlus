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

#include "main.h"

#ifdef USE_OPENGL

#include "normalopenglgraphics.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "resources/image.h"
#include "resources/openglimagehelper.h"

#include "utils/stringutils.h"

#include <SDL.h>

#include "debug.h"

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#endif

const unsigned int vertexBufSize = 500;

GLuint NormalOpenGLGraphics::mLastImage = 0;

NormalOpenGLGraphics::NormalOpenGLGraphics():
    mFloatTexArray(new GLfloat[vertexBufSize * 4 + 30]),
    mIntTexArray(new GLint[vertexBufSize * 4 + 30]),
    mIntVertArray(new GLint[vertexBufSize * 4 + 30]),
    mAlpha(false),
    mTexture(false),
#ifdef DEBUG_BIND_TEXTURE
    mColorAlpha(false),
    mOldTextureId(0)
#else
    mColorAlpha(false)
#endif
{
    mOpenGL = 1;
    mName = "fast OpenGL";
}

NormalOpenGLGraphics::~NormalOpenGLGraphics()
{
    delete [] mFloatTexArray;
    delete [] mIntTexArray;
    delete [] mIntVertArray;
}

bool NormalOpenGLGraphics::setVideoMode(const int w, const int h,
                                        const int bpp, const bool fs,
                                        const bool hwaccel, const bool resize,
                                        const bool noFrame)
{
    setMainFlags(w, h, bpp, fs, hwaccel, resize, noFrame);

    return setOpenGLMode();
}

static inline void drawQuad(const Image *image,
                            int srcX, int srcY, int dstX, int dstY,
                            int width, int height)
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        // Find OpenGL normalized texture coordinates.
        const float texX1 = static_cast<float>(srcX) /
                            static_cast<float>(image->getTextureWidth());
        const float texY1 = static_cast<float>(srcY) /
                            static_cast<float>(image->getTextureHeight());
        const float texX2 = static_cast<float>(srcX + width) /
                            static_cast<float>(image->getTextureWidth());
        const float texY2 = static_cast<float>(srcY + height) /
                            static_cast<float>(image->getTextureHeight());

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

        glDrawArrays(GL_QUADS, 0, 4);
    }
}

static inline void drawRescaledQuad(const Image *const image,
                                    int srcX, int srcY, int dstX, int dstY,
                                    int width, int height,
                                    int desiredWidth, int desiredHeight)
{
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        // Find OpenGL normalized texture coordinates.
        const float texX1 = static_cast<float>(srcX) /
                            static_cast<float>(image->getTextureWidth());
        const float texY1 = static_cast<float>(srcY) /
                            static_cast<float>(image->getTextureHeight());
        const float texX2 = static_cast<float>(srcX + width) /
                            static_cast<float>(image->getTextureWidth());
        const float texY2 = static_cast<float>(srcY + height) /
                            static_cast<float>(image->getTextureHeight());

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

        glDrawArrays(GL_QUADS, 0, 4);
    }
}


bool NormalOpenGLGraphics::drawImage2(const Image *const image,
                                      int srcX, int srcY,
                                      int dstX, int dstY,
                                      const int width, const int height,
                                      const bool useColor)
{
    if (!image)
        return false;

    srcX += image->mBounds.x;
    srcY += image->mBounds.y;

    if (!useColor)
        glColor4f(1.0f, 1.0f, 1.0f, image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    drawQuad(image, srcX, srcY, dstX, dstY, width, height);

    if (!useColor)
    {
        glColor4ub(static_cast<GLubyte>(mColor.r),
                   static_cast<GLubyte>(mColor.g),
                   static_cast<GLubyte>(mColor.b),
                   static_cast<GLubyte>(mColor.a));
    }

    return true;
}

bool NormalOpenGLGraphics::drawRescaledImage(Image *const image, int srcX,
                                             int srcY, int dstX, int dstY,
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

bool NormalOpenGLGraphics::drawRescaledImage(Image *const image, int srcX,
                                             int srcY, int dstX, int dstY,
                                             const int width, const int height,
                                             const int desiredWidth,
                                             const int desiredHeight,
                                             const bool useColor,
                                             bool smooth)
{
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

    srcX += image->mBounds.x;
    srcY += image->mBounds.y;

    if (!useColor)
        glColor4f(1.0f, 1.0f, 1.0f, image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    // Draw a textured quad.
    drawRescaledQuad(image, srcX, srcY, dstX, dstY, width, height,
                     desiredWidth, desiredHeight);

    if (smooth) // A basic smooth effect...
    {
        glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
        drawRescaledQuad(image, srcX, srcY, dstX - 1, dstY - 1, width, height,
                         desiredWidth + 1, desiredHeight + 1);
        drawRescaledQuad(image, srcX, srcY, dstX + 1, dstY + 1, width, height,
                         desiredWidth - 1, desiredHeight - 1);

        drawRescaledQuad(image, srcX, srcY, dstX + 1, dstY, width, height,
                         desiredWidth - 1, desiredHeight);
        drawRescaledQuad(image, srcX, srcY, dstX, dstY + 1, width, height,
                         desiredWidth, desiredHeight - 1);
    }

    if (!useColor)
    {
        glColor4ub(static_cast<GLubyte>(mColor.r),
                   static_cast<GLubyte>(mColor.g),
                   static_cast<GLubyte>(mColor.b),
                   static_cast<GLubyte>(mColor.a));
    }

    return true;
}

void NormalOpenGLGraphics::drawImagePattern(const Image *const image,
                                            const int x, const int y,
                                            const int w, const int h)
{
    if (!image)
        return;

    const int srcX = image->mBounds.x;
    const int srcY = image->mBounds.y;

    const int iw = image->mBounds.w;
    const int ih = image->mBounds.h;

    if (iw == 0 || ih == 0)
        return;

    const float tw = static_cast<float>(image->getTextureWidth());
    const float th = static_cast<float>(image->getTextureHeight());

    glColor4f(1.0f, 1.0f, 1.0f, image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    unsigned int vp = 0;
    const unsigned int vLimit = vertexBufSize * 4;
    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        float texX1 = static_cast<float>(srcX) / tw;
        float texY1 = static_cast<float>(srcY) / th;

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            for (int px = 0; px < w; px += iw)
            {
                int width = (px + iw >= w) ? w - px : iw;
                int dstX = x + px;

                float texX2 = static_cast<float>(srcX + width) / tw;
                float texY2 = static_cast<float>(srcY + height) / th;

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
                int width = (px + iw >= w) ? w - px : iw;
                int dstX = x + px;

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

    glColor4ub(static_cast<GLubyte>(mColor.r),
               static_cast<GLubyte>(mColor.g),
               static_cast<GLubyte>(mColor.b),
               static_cast<GLubyte>(mColor.a));
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

    const int srcX = image->mBounds.x;
    const int srcY = image->mBounds.y;

    const int iw = image->getWidth();
    const int ih = image->getHeight();
    if (iw == 0 || ih == 0)
        return;

    glColor4f(1.0f, 1.0f, 1.0f, image->mAlpha);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);

    unsigned int vp = 0;
    const unsigned int vLimit = vertexBufSize * 4;

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        const float tw = static_cast<float>(image->getTextureWidth());
        const float th = static_cast<float>(image->getTextureHeight());

        const float texX1 = static_cast<float>(srcX) / tw;
        const float texY1 = static_cast<float>(srcY) / th;

        const float tFractionW = iw / tw;
        const float tFractionH = ih / th;

        for (int py = 0; py < h; py += scaledHeight)
        {
            const int height = (py + scaledHeight >= h)
                ? h - py : scaledHeight;
            const int dstY = y + py;
            for (int px = 0; px < w; px += scaledWidth)
            {
                int width = (px + scaledWidth >= w) ? w - px : scaledWidth;
                int dstX = x + px;
                const float visibleFractionW = static_cast<float>(width)
                    / scaledWidth;
                const float visibleFractionH = static_cast<float>(height)
                    / scaledHeight;

                const float texX2 = texX1 + tFractionW * visibleFractionW;
                const float texY2 = texY1 + tFractionH * visibleFractionH;

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

    glColor4ub(static_cast<GLubyte>(mColor.r),
               static_cast<GLubyte>(mColor.g),
               static_cast<GLubyte>(mColor.b),
               static_cast<GLubyte>(mColor.a));
}

void NormalOpenGLGraphics::drawImagePattern2(GraphicsVertexes *const vert,
                                             const Image *const image)
{
    if (!image)
        return;

    NormalOpenGLGraphicsVertexes *ogl = vert->getOGL();

    glColor4f(1.0f, 1.0f, 1.0f, image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    std::vector<GLint*> *intVertPool = ogl->getIntVertPool();
    std::vector<GLint*>::const_iterator iv;
    std::vector<GLint*>::const_iterator iv_end = intVertPool->end();
    std::vector<int> *vp = ogl->getVp();
    std::vector<int>::const_iterator ivp;
    std::vector<int>::const_iterator ivp_end = vp->end();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        std::vector<GLfloat*> *floatTexPool = ogl->getFloatTexPool();
        std::vector<GLfloat*>::const_iterator ft;
        std::vector<GLfloat*>::const_iterator ft_end = floatTexPool->end();

        for (iv = intVertPool->begin(), ft = floatTexPool->begin(),
             ivp = vp->begin();
             iv != iv_end, ft != ft_end,
             ivp != ivp_end;
             ++ iv, ++ ft, ++ ivp)
        {
            drawQuadArrayfi(*iv, *ft, *ivp);
        }
    }
    else
    {
        std::vector<GLint*> *intTexPool = ogl->getIntTexPool();
        std::vector<GLint*>::const_iterator it;
        std::vector<GLint*>::const_iterator it_end = intTexPool->end();

        for (iv = intVertPool->begin(), it = intTexPool->begin(),
             ivp = vp->begin();
             iv != iv_end, it != it_end,
             ivp != ivp_end;
             ++ iv, ++ it, ++ ivp)
        {
            drawQuadArrayii(*iv, *it, *ivp);
        }
    }

    glColor4ub(static_cast<GLubyte>(mColor.r),
               static_cast<GLubyte>(mColor.g),
               static_cast<GLubyte>(mColor.b),
               static_cast<GLubyte>(mColor.a));

}

void NormalOpenGLGraphics::calcImagePattern(GraphicsVertexes *const vert,
                                            const Image *const image,
                                            const int x, const int y,
                                            const int w, const int h)
{
    if (!image)
    {
        vert->incPtr(1);
        return;
    }

    const int srcX = image->mBounds.x;
    const int srcY = image->mBounds.y;

    const int iw = image->mBounds.w;
    const int ih = image->mBounds.h;

    if (iw == 0 || ih == 0)
    {
        vert->incPtr(1);
        return;
    }

    const float tw = static_cast<float>(image->getTextureWidth());
    const float th = static_cast<float>(image->getTextureHeight());

    unsigned int vp = 0;
    const unsigned int vLimit = vertexBufSize * 4;

    NormalOpenGLGraphicsVertexes *ogl = vert->getOGL();
    ogl->init();

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        float texX1 = static_cast<float>(srcX) / tw;
        float texY1 = static_cast<float>(srcY) / th;

        GLfloat *floatTexArray = ogl->switchFloatTexArray();
        GLint *intVertArray = ogl->switchIntVertArray();

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            for (int px = 0; px < w; px += iw)
            {
                int width = (px + iw >= w) ? w - px : iw;
                int dstX = x + px;

                float texX2 = static_cast<float>(srcX + width) / tw;
                float texY2 = static_cast<float>(srcY + height) / th;

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
                    floatTexArray = ogl->switchFloatTexArray();
                    intVertArray = ogl->switchIntVertArray();
                    ogl->switchVp(vp);
                    vp = 0;
                }
            }
        }
    }
    else
    {
        GLint *intTexArray = ogl->switchIntTexArray();
        GLint *intVertArray = ogl->switchIntVertArray();

        for (int py = 0; py < h; py += ih)
        {
            const int height = (py + ih >= h) ? h - py : ih;
            const int dstY = y + py;
            for (int px = 0; px < w; px += iw)
            {
                int width = (px + iw >= w) ? w - px : iw;
                int dstX = x + px;

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
                    intTexArray = ogl->switchIntTexArray();
                    intVertArray = ogl->switchIntVertArray();
                    ogl->switchVp(vp);
                    vp = 0;
                }
            }
        }
    }
    ogl->switchVp(vp);
    vert->incPtr(1);
}

void NormalOpenGLGraphics::calcTile(ImageVertexes *const vert,
                                    int dstX, int dstY)
{
    if (!vert)
        return;

    Image *image = vert->image;
    const int srcX = image->mBounds.x;
    const int srcY = image->mBounds.y;

    const int w = image->mBounds.w;
    const int h = image->mBounds.h;

    if (w == 0 || h == 0)
        return;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);

    const unsigned int vLimit = vertexBufSize * 4;

    NormalOpenGLGraphicsVertexes *ogl = vert->ogl;

    unsigned int vp = ogl->ptr;

    // Draw a set of textured rectangles
    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        float texX1 = static_cast<float>(srcX) / tw;
        float texY1 = static_cast<float>(srcY) / th;

        if (!ogl->mFloatTexArray)
            ogl->mFloatTexArray = new GLfloat[vertexBufSize * 4 + 30];
        if (!ogl->mIntVertArray)
            ogl->mIntVertArray = new GLint[vertexBufSize * 4 + 30];

        GLfloat *floatTexArray = ogl->mFloatTexArray;
        GLint *intVertArray = ogl->mIntVertArray;

        float texX2 = static_cast<float>(srcX + w) / tw;
        float texY2 = static_cast<float>(srcY + h) / th;

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
            ogl->ptr = vp;
            return;
        }
    }
    else
    {
        if (!ogl->mIntTexArray)
            ogl->mIntTexArray = new GLint[vertexBufSize * 4 + 30];
        if (!ogl->mIntVertArray)
            ogl->mIntVertArray = new GLint[vertexBufSize * 4 + 30];

        GLint *intTexArray = ogl->mIntTexArray;
        GLint *intVertArray = ogl->mIntVertArray;

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
            ogl->ptr = vp;
            return;
        }
    }
    ogl->ptr = vp;
}

void NormalOpenGLGraphics::drawTile(const ImageVertexes *const vert)
{
    if (!vert)
        return;
    Image *image = vert->image;

    NormalOpenGLGraphicsVertexes *ogl = vert->ogl;

    glColor4f(1.0f, 1.0f, 1.0f, image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    if (OpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
        drawQuadArrayfi(ogl->mIntVertArray, ogl->mFloatTexArray, ogl->ptr);
    else
        drawQuadArrayii(ogl->mIntVertArray, ogl->mIntTexArray, ogl->ptr);

    glColor4ub(static_cast<GLubyte>(mColor.r),
               static_cast<GLubyte>(mColor.g),
               static_cast<GLubyte>(mColor.b),
               static_cast<GLubyte>(mColor.a));
}

void NormalOpenGLGraphics::updateScreen()
{
//    glFlush();
//    glFinish();
    SDL_GL_SwapBuffers();
// may be need clear?
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void NormalOpenGLGraphics::_beginDraw()
{
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, static_cast<double>(mTarget->w),
        static_cast<double>(mTarget->h), 0.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_SCISSOR_TEST);
    glDisable(GL_DITHER);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

#ifndef __MINGW32__
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
#endif

//    glScalef(0.5f, 0.5f, 0.5f);

    pushClipArea(gcn::Rectangle(0, 0, mTarget->w, mTarget->h));
}

void NormalOpenGLGraphics::_endDraw()
{
    popClipArea();
}

void NormalOpenGLGraphics::prepareScreenshot()
{
    if (config.getBoolValue("usefbo"))
        graphicsManager.createFBO(mTarget->w, mTarget->h, &mFbo);
}

SDL_Surface* NormalOpenGLGraphics::getScreenshot()
{
    const int h = mTarget->h;
    const int w = mTarget->w - (mTarget->w % 4);
    GLint pack = 1;

    SDL_Surface *screenshot = SDL_CreateRGBSurface(
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
    unsigned int lineSize = 3 * w;
    GLubyte* buf = static_cast<GLubyte*>(malloc(lineSize));

    for (int i = 0; i < (h / 2); i++)
    {
        GLubyte *top = static_cast<GLubyte*>(
            screenshot->pixels) + lineSize * i;
        GLubyte *bot = static_cast<GLubyte*>(
            screenshot->pixels) + lineSize * (h - 1 - i);

        memcpy(buf, top, lineSize);
        memcpy(top, bot, lineSize);
        memcpy(bot, buf, lineSize);
    }

    free(buf);

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

    bool result = gcn::Graphics::pushClipArea(area);

    const gcn::ClipRectangle &clipArea = mClipStack.top();
    transX += clipArea.xOffset;
    transY += clipArea.yOffset;

    glPushMatrix();
    if (transX || transY)
    {
        glTranslatef(static_cast<GLfloat>(transX),
                     static_cast<GLfloat>(transY), 0);
    }
    glScissor(clipArea.x, mTarget->h - clipArea.y - clipArea.height,
        clipArea.width, clipArea.height);

    return result;
}

void NormalOpenGLGraphics::popClipArea()
{
    gcn::Graphics::popClipArea();

    if (mClipStack.empty())
        return;

    glPopMatrix();
    const gcn::ClipRectangle &clipArea = mClipStack.top();
    glScissor(clipArea.x, mTarget->h - clipArea.y - clipArea.height,
        clipArea.width, clipArea.height);
}

void NormalOpenGLGraphics::setColor(const gcn::Color& color)
{
    mColor = color;
    glColor4ub(static_cast<GLubyte>(color.r),
               static_cast<GLubyte>(color.g),
               static_cast<GLubyte>(color.b),
               static_cast<GLubyte>(color.a));

    mColorAlpha = (color.a != 255);
}

void NormalOpenGLGraphics::drawPoint(int x, int y)
{
    setTexturingAndBlending(false);

    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void NormalOpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
{
    setTexturingAndBlending(false);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    mFloatTexArray[0] = static_cast<float>(x1) + 0.5f;
    mFloatTexArray[1] = static_cast<float>(y1) + 0.5f;
    mFloatTexArray[2] = static_cast<float>(x2) + 0.5f;
    mFloatTexArray[3] = static_cast<float>(y2) + 0.5f;

    drawLineArrayf(4);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void NormalOpenGLGraphics::drawRectangle(const gcn::Rectangle& rect)
{
    drawRectangle(rect, false);
}

void NormalOpenGLGraphics::fillRectangle(const gcn::Rectangle& rect)
{
    drawRectangle(rect, true);
}

void NormalOpenGLGraphics::setTargetPlane(int width A_UNUSED,
                                          int height A_UNUSED)
{
}

void NormalOpenGLGraphics::setTexturingAndBlending(bool enable)
{
    if (enable)
    {
        if (!mTexture)
        {
            glEnable(OpenGLImageHelper::mTextureType);
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
            mTexture = false;
        }
    }
}

void NormalOpenGLGraphics::drawRectangle(const gcn::Rectangle& rect,
                                         bool filled)
{
    const float offset = filled ? 0 : 0.5f;
    const float x = static_cast<float>(rect.x);
    const float y = static_cast<float>(rect.y);
    const float width = static_cast<float>(rect.width);
    const float height = static_cast<float>(rect.height);

    setTexturingAndBlending(false);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    GLfloat vert[] =
    {
        x + offset, y + offset,
        x + width - offset, y + offset,
        x + width - offset, y + height - offset,
        x + offset, y + height - offset
    };

    glVertexPointer(2, GL_FLOAT, 0, &vert);
    glDrawArrays(filled ? GL_QUADS : GL_LINE_LOOP, 0, 4);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

bool NormalOpenGLGraphics::drawNet(const int x1, const int y1,
                                   const int x2, const int y2,
                                   const int width, const int height)
{
    unsigned int vp = 0;
    const unsigned int vLimit = vertexBufSize * 4;

    setTexturingAndBlending(false);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

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

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    return true;
}

void NormalOpenGLGraphics::bindTexture(GLenum target, GLuint texture)
{
    if (mLastImage != texture)
    {
        mLastImage = texture;
        glBindTexture(target, texture);
    }
}

inline void NormalOpenGLGraphics::drawQuadArrayfi(int size)
{
    glVertexPointer(2, GL_INT, 0, mIntVertArray);
    glTexCoordPointer(2, GL_FLOAT, 0, mFloatTexArray);

    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawQuadArrayfi(GLint *intVertArray,
                                                  GLfloat *floatTexArray,
                                                  int size)
{
    glVertexPointer(2, GL_INT, 0, intVertArray);
    glTexCoordPointer(2, GL_FLOAT, 0, floatTexArray);

    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawQuadArrayii(int size)
{
    glVertexPointer(2, GL_INT, 0, mIntVertArray);
    glTexCoordPointer(2, GL_INT, 0, mIntTexArray);

    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawQuadArrayii(GLint *intVertArray,
                                                  GLint *intTexArray, int size)
{
    glVertexPointer(2, GL_INT, 0, intVertArray);
    glTexCoordPointer(2, GL_INT, 0, intTexArray);

    glDrawArrays(GL_QUADS, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawLineArrayi(int size)
{
    glVertexPointer(2, GL_INT, 0, mIntVertArray);

    glDrawArrays(GL_LINES, 0, size / 2);
}

inline void NormalOpenGLGraphics::drawLineArrayf(int size)
{
    glVertexPointer(2, GL_FLOAT, 0, mFloatTexArray);

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

#ifdef DEBUG_BIND_TEXTURE
void NormalOpenGLGraphics::debugBindTexture(const Image *image)
{
    const std::string texture = image->getIdPath();
    if (mOldTexture != texture)
    {
        if (!mOldTexture.empty() && !texture.empty()
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
void NormalOpenGLGraphics::debugBindTexture(const Image *image A_UNUSED)
{
}
#endif

#endif // USE_OPENGL
