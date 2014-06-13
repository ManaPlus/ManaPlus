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

#include "render/modernopenglgraphics.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "render/mgl.h"

#include "render/shaders/shaderprogram.h"
#include "render/shaders/shadersmanager.h"

#include "resources/image.h"
#include "resources/imagerect.h"
#include "resources/openglimagehelper.h"

#include "utils/sdlcheckutils.h"

#include "debug.h"

#define vertFill2D(var, x1, y1, x2, y2, dstX, dstY, w, h) \
    var[vp + 0] = dstX; \
    var[vp + 1] = dstY; \
    var[vp + 2] = x1; \
    var[vp + 3] = y1; \
    \
    var[vp + 4] = dstX + w; \
    var[vp + 5] = dstY; \
    var[vp + 6] = x2; \
    var[vp + 7] = y1; \
    \
    var[vp + 8] = dstX + w; \
    var[vp + 9] = dstY + h; \
    var[vp + 10] = x2; \
    var[vp + 11] = y2; \
    \
    var[vp + 12] = dstX; \
    var[vp + 13] = dstY; \
    var[vp + 14] = x1; \
    var[vp + 15] = y1; \
    \
    var[vp + 16] = dstX; \
    var[vp + 17] = dstY + h; \
    var[vp + 18] = x1; \
    var[vp + 19] = y2; \
    \
    var[vp + 20] = dstX + w; \
    var[vp + 21] = dstY + h; \
    var[vp + 22] = x2; \
    var[vp + 23] = y2;


GLuint ModernOpenGLGraphics::mLastImage = 0;
#ifdef DEBUG_DRAW_CALLS
unsigned int ModernOpenGLGraphics::mDrawCalls = 0;
unsigned int ModernOpenGLGraphics::mLastDrawCalls = 0;
#endif

ModernOpenGLGraphics::ModernOpenGLGraphics() :
    mFloatArray(nullptr),
    mFloatArrayCached(nullptr),
    mProgram(nullptr),
    mAlphaCached(1.0F),
    mVpCached(0),
    mFloatColor(1.0F),
    mMaxVertices(500),
    mProgramId(0U),
    mSimpleColorUniform(0U),
    mPosAttrib(0),
    mTextureColorUniform(0U),
    mScreenUniform(0U),
    mDrawTypeUniform(0U),
    mVao(0U),
    mVbo(0U),
    mVboCached(0U),
    mAttributesCached(0U),
    mColorAlpha(false),
    mTextureDraw(false),
#ifdef DEBUG_BIND_TEXTURE
    mOldTexture(),
    mOldTextureId(0),
#endif
    mFbo()
{
    mOpenGL = RENDER_MODERN_OPENGL;
    mName = "modern OpenGL";
}

ModernOpenGLGraphics::~ModernOpenGLGraphics()
{
    deleteArraysInternal();
    if (mProgram)
        mProgram->decRef();
    if (mVbo)
        mglDeleteBuffers(1, &mVbo);
    if (mVao)
        mglDeleteVertexArrays(1, &mVao);
}

void ModernOpenGLGraphics::initArrays(const int vertCount)
{
    mMaxVertices = vertCount;
    if (mMaxVertices < 500)
        mMaxVertices = 500;
    else if (mMaxVertices > 1024)
        mMaxVertices = 1024;

    // need alocate small size, after if limit reached reallocate to double size
    const size_t sz = mMaxVertices * 4 + 30;
    vertexBufSize = mMaxVertices;
    if (!mFloatArray)
        mFloatArray = new GLfloat[sz];
    if (!mFloatArrayCached)
        mFloatArrayCached = new GLfloat[sz];
}

void ModernOpenGLGraphics::postInit()
{
    mglGenVertexArrays(1, &mVao);
    mglBindVertexArray(mVao);
    mglGenBuffers(1, &mVbo);
    bindArrayBuffer(mVbo);

    logger->log("Compiling shaders");
    mProgram = shaders.getSimpleProgram();
    mProgramId = mProgram->getProgramId();
    if (!mProgram)
        logger->error("Shaders compilation error.");

    logger->log("Shaders compilation done.");
    mglUseProgram(mProgramId);

    mPosAttrib = mglGetAttribLocation(mProgramId, "position");
    mglEnableVertexAttribArray(mPosAttrib);
    mglVertexAttribFormat(mPosAttrib, 4, GL_FLOAT, GL_FALSE, 0);

    mSimpleColorUniform = mglGetUniformLocation(mProgramId, "color");
    mScreenUniform = mglGetUniformLocation(mProgramId, "screen");
    mDrawTypeUniform = mglGetUniformLocation(mProgramId, "drawType");
    mTextureColorUniform = mglGetUniformLocation(mProgramId, "alpha");

    mglUniform1f(mTextureColorUniform, 1.0f);

    mglBindVertexBuffer(0, mVbo, 0, 4 * sizeof(GLfloat));
    mglVertexAttribBinding(mPosAttrib, 0);
    mAttributesCached = mVbo;

    screenResized();
}

void ModernOpenGLGraphics::screenResized()
{
    mglUniform2f(mScreenUniform,
        static_cast<float>(mWidth) / 2.0f,
        static_cast<float>(mHeight) / 2.0f);
}

void ModernOpenGLGraphics::deleteArrays()
{
    deleteArraysInternal();
}

void ModernOpenGLGraphics::deleteArraysInternal()
{
    delete [] mFloatArray;
    mFloatArray = nullptr;
    delete [] mFloatArrayCached;
    mFloatArrayCached = nullptr;
}

bool ModernOpenGLGraphics::setVideoMode(const int w, const int h,
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

void ModernOpenGLGraphics::setColor(const Color &color)
{
    setColorAll(color, color);
}

void ModernOpenGLGraphics::setColorAll(const Color &color,
                                       const Color &color2)
{
    mColor2 = color2;
    mColorAlpha = (color.a != 255);
    if (mColor != color)
    {
        mColor = color;
        mglUniform4f(mSimpleColorUniform,
            static_cast<float>(color.r) / 255.0F,
            static_cast<float>(color.g) / 255.0F,
            static_cast<float>(color.b) / 255.0F,
            static_cast<float>(color.a) / 255.0F);
    }
}

void ModernOpenGLGraphics::setColorAlpha(const float alpha)
{
    if (mAlphaCached != alpha)
    {
        mAlphaCached = alpha;
        mglUniform1f(mTextureColorUniform, alpha);
    }
}

void ModernOpenGLGraphics::drawQuad(const Image *const image,
                                    const int srcX, const int srcY,
                                    const int dstX, const int dstY,
                                    const int width, const int height)
{

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);
    // Find OpenGL normalized texture coordinates.
    const float texX1 = static_cast<float>(srcX) / tw;
    const float texY1 = static_cast<float>(srcY) / th;
    const float texX2 = static_cast<float>(srcX + width) / tw;
    const float texY2 = static_cast<float>(srcY + height) / th;
    const float x1 = static_cast<GLfloat>(dstX);
    const float y1 = static_cast<GLfloat>(dstY);
    const float x2 = x1 + static_cast<GLfloat>(width);
    const float y2 = y1 + static_cast<GLfloat>(height);

    GLfloat vertices[] =
    {
        x1, y1, texX1, texY1,
        x2, y1, texX2, texY1,
        x1, y2, texX1, texY2,
        x2, y2, texX2, texY2
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ModernOpenGLGraphics::drawRescaledQuad(const Image *const image,
                                            const int srcX, const int srcY,
                                            const int dstX, const int dstY,
                                            const int width, const int height,
                                            const int desiredWidth,
                                            const int desiredHeight)
{
    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);
    // Find OpenGL normalized texture coordinates.
    const float texX1 = static_cast<float>(srcX) / tw;
    const float texY1 = static_cast<float>(srcY) / th;
    const float texX2 = static_cast<float>(srcX + width) / tw;
    const float texY2 = static_cast<float>(srcY + height) / th;

    const float x1 = static_cast<GLfloat>(dstX);
    const float y1 = static_cast<GLfloat>(dstY);
    const float x2 = x1 + static_cast<GLfloat>(desiredWidth);
    const float y2 = y1 + static_cast<GLfloat>(desiredHeight);

    GLfloat vertices[] =
    {
        x1, y1, texX1, texY1,
        x2, y1, texX2, texY1,
        x1, y2, texX1, texY2,
        x2, y2, texX2, texY2
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool ModernOpenGLGraphics::drawImage(const Image *const image,
                                     int dstX, int dstY)
{
    return drawImageInline(image, dstX, dstY);
}

bool ModernOpenGLGraphics::drawImageInline(const Image *const image,
                                           int dstX, int dstY)
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (!image)
        return false;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(GL_TEXTURE_2D, image->mGLImage);
    setTexturingAndBlending(true);
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    const ClipRect &clipArea = mClipStack.top();
    const SDL_Rect &imageRect = image->mBounds;
    drawQuad(image,
        imageRect.x, imageRect.y,
        dstX + clipArea.xOffset, dstY + clipArea.yOffset,
        imageRect.w, imageRect.h);
    return true;
}

void ModernOpenGLGraphics::drawImageCached(const Image *const image A_UNUSED,
                                           int A_UNUSED x, int y A_UNUSED)
{
}

void ModernOpenGLGraphics::drawPatternCached(const Image *const image A_UNUSED,
                                             const int x A_UNUSED,
                                             const int y A_UNUSED,
                                             const int w A_UNUSED,
                                             const int h A_UNUSED)
{
}

void ModernOpenGLGraphics::completeCache()
{
}

bool ModernOpenGLGraphics::drawRescaledImage(const Image *const image,
                                             int dstX, int dstY,
                                             const int desiredWidth,
                                             const int desiredHeight)
{
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
    bindArrayBufferAndAttributes(mVbo);

    const ClipRect &clipArea = mClipStack.top();
    // Draw a textured quad.
    drawRescaledQuad(image,
        imageRect.x, imageRect.y,
        dstX + clipArea.xOffset, dstY + clipArea.yOffset,
        imageRect.w, imageRect.h,
        desiredWidth, desiredHeight);

    return true;
}

void ModernOpenGLGraphics::drawPattern(const Image *const image,
                                       const int x, const int y,
                                       const int w, const int h)
{
    drawPatternInline(image, x, y, w, h);
}

void ModernOpenGLGraphics::drawPatternInline(const Image *const image,
                                             const int x, const int y,
                                             const int w, const int h)
{
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
    const ClipRect &clipArea = mClipStack.top();
    const int x2 = x + clipArea.xOffset;
    const int y2 = y + clipArea.yOffset;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    const float texX1 = static_cast<float>(srcX) / tw;
    const float texY1 = static_cast<float>(srcY) / th;

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const float texY2 = static_cast<float>(srcY + height) / th;
        const int dstY = y2 + py;
        for (int px = 0; px < w; px += iw)
        {
            const int width = (px + iw >= w) ? w - px : iw;
            const int dstX = x2 + px;

            const float texX2 = static_cast<float>(srcX + width) / tw;

            vertFill2D(mFloatArray,
                texX1, texY1, texX2, texY2,
                dstX, dstY, width, height);

            vp += 24;
            if (vp >= vLimit)
            {
                drawTriangleArray(vp);
                vp = 0;
            }
        }
    }
    if (vp > 0)
        drawTriangleArray(vp);
}

void ModernOpenGLGraphics::drawRescaledPattern(const Image *const image,
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
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    setTexturingAndBlending(true);
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);
    const ClipRect &clipArea = mClipStack.top();
    const int x2 = x + clipArea.xOffset;
    const int y2 = y + clipArea.yOffset;

    const float texX1 = static_cast<float>(srcX) / tw;
    const float texY1 = static_cast<float>(srcY) / th;

    const float tFractionW = iw / tw;
    const float tFractionH = ih / th;

    for (int py = 0; py < h; py += scaledHeight)
    {
        const int height = (py + scaledHeight >= h)
            ? h - py : scaledHeight;
        const int dstY = y2 + py;
        const float visibleFractionH = static_cast<float>(height)
            / scaledHeight;
        const float texY2 = texY1 + tFractionH * visibleFractionH;
        for (int px = 0; px < w; px += scaledWidth)
        {
            const int width = (px + scaledWidth >= w)
                ? w - px : scaledWidth;
            const int dstX = x2 + px;
            const float visibleFractionW = static_cast<float>(width)
                / scaledWidth;
            const float texX2 = texX1 + tFractionW * visibleFractionW;

            vertFill2D(mFloatArray,
                texX1, texY1, texX2, texY2,
                dstX, dstY, width, height);

            vp += 24;
            if (vp >= vLimit)
            {
                drawTriangleArray(vp);
                vp = 0;
            }
        }
    }
    if (vp > 0)
        drawTriangleArray(vp);
}

inline void ModernOpenGLGraphics::drawVertexes(const
                                               OpenGLGraphicsVertexes &ogl)
{
    const std::vector<int> &vp = ogl.mVp;
    const std::vector<GLuint> &vbos = ogl.mVbo;
    std::vector<int>::const_iterator ivp;
    std::vector<GLuint>::const_iterator ivbo;
    const std::vector<int>::const_iterator ivp_end = vp.end();

    for (ivp = vp.begin(), ivbo = vbos.begin();
         ivp != ivp_end;
         ++ ivp, ++ ivbo)
    {
        bindArrayBufferAndAttributes(*ivbo);
#ifdef DEBUG_DRAW_CALLS
        mDrawCalls ++;
#endif
        glDrawArrays(GL_TRIANGLES, 0, *ivp / 4);
    }
}

void ModernOpenGLGraphics::calcPattern(ImageVertexes *const vert,
                                       const Image *const image,
                                       const int x, const int y,
                                       const int w, const int h) const
{
    calcPatternInline(vert, image, x, y, w, h);
}

void ModernOpenGLGraphics::calcPatternInline(ImageVertexes *const vert,
                                             const Image *const image,
                                             const int x, const int y,
                                             const int w, const int h) const
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
    const ClipRect &clipArea = mClipStack.top();
    const int x2 = x + clipArea.xOffset;
    const int y2 = y + clipArea.yOffset;

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;
    unsigned int vp = ogl.continueVp();

    const float texX1 = static_cast<float>(srcX) / tw;
    const float texY1 = static_cast<float>(srcY) / th;

    GLfloat *floatArray = ogl.continueFloatTexArray();

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const int dstY = y2 + py;
        const float texY2 = static_cast<float>(srcY + height) / th;
        for (int px = 0; px < w; px += iw)
        {
            const int width = (px + iw >= w) ? w - px : iw;
            const int dstX = x2 + px;
            const float texX2 = static_cast<float>(srcX + width) / tw;

            vertFill2D(floatArray,
                texX1, texY1, texX2, texY2,
                dstX, dstY, width, height);

            vp += 24;
            if (vp >= vLimit)
            {
                floatArray = ogl.switchFloatTexArray();
                ogl.switchVp(vp);
                vp = 0;
            }
        }
    }
    ogl.switchVp(vp);
}

void ModernOpenGLGraphics::calcTileCollection(ImageCollection *const vertCol,
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

void ModernOpenGLGraphics::drawTileCollection(const ImageCollection
                                              *const vertCol)
{
    setTexturingAndBlending(true);
//    bindArrayBuffer(vbo);
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
        drawVertexes(vert->ogl);
    }
}

void ModernOpenGLGraphics::calcPattern(ImageCollection* const vertCol,
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

void ModernOpenGLGraphics::calcTileVertexes(ImageVertexes *const vert,
                                            const Image *const image,
                                            int dstX, int dstY) const
{
    calcTileVertexesInline(vert, image, dstX, dstY);
}

void ModernOpenGLGraphics::calcTileVertexesInline(ImageVertexes *const vert,
                                                  const Image *const image,
                                                  int dstX, int dstY) const
{
    if (!vert || !image)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int w = imageRect.w;
    const int h = imageRect.h;

    if (w == 0 || h == 0)
        return;

    const float tw = static_cast<float>(image->mTexWidth);
    const float th = static_cast<float>(image->mTexHeight);
    const ClipRect &clipArea = mClipStack.top();
    const int x2 = dstX + clipArea.xOffset;
    const int y2 = dstY + clipArea.yOffset;

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;

    unsigned int vp = ogl.continueVp();

    float texX1 = static_cast<float>(srcX) / tw;
    float texY1 = static_cast<float>(srcY) / th;
    float texX2 = static_cast<float>(srcX + w) / tw;
    float texY2 = static_cast<float>(srcY + h) / th;

    GLfloat *const floatArray = ogl.continueFloatTexArray();

    vertFill2D(floatArray,
        texX1, texY1, texX2, texY2,
        x2, y2, w, h);

    vp += 24;
    if (vp >= vLimit)
    {
        ogl.switchFloatTexArray();
        ogl.switchVp(vp);
        vp = 0;
    }

    ogl.switchVp(vp);
}

void ModernOpenGLGraphics::drawTileVertexes(const ImageVertexes *const vert)
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
    bindArrayBufferAndAttributes(mVbo);

    drawVertexes(vert->ogl);
}

void ModernOpenGLGraphics::calcWindow(ImageCollection *const vertCol,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const ImageRect &imgRect)
{
    ImageVertexes *vert = nullptr;
    const Image *const image = imgRect.grid[4];
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

void ModernOpenGLGraphics::updateScreen()
{
    BLOCK_START("Graphics::updateScreen")
#ifdef DEBUG_DRAW_CALLS
    mLastDrawCalls = mDrawCalls;
    mDrawCalls = 0;
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

void ModernOpenGLGraphics::beginDraw()
{
    glEnable(GL_SCISSOR_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_COLOR_LOGIC_OP);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_FLAT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef ANDROID
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
#ifndef __MINGW32__
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
#endif
#endif

    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void ModernOpenGLGraphics::endDraw()
{
    popClipArea();
}

void ModernOpenGLGraphics::prepareScreenshot()
{
    if (config.getBoolValue("usefbo"))
        graphicsManager.createFBO(mRect.w, mRect.h, &mFbo);
}

SDL_Surface* ModernOpenGLGraphics::getScreenshot()
{
    const int h = mRect.h;
    const int w = mRect.w - (mRect.w % 4);
    GLint pack = 1;

    SDL_Surface *const screenshot = MSDL_CreateRGBSurface(
        SDL_SWSURFACE, w, h, 24,
        0xff0000, 0x00ff00, 0x0000ff, 0x000000);

    if (!screenshot)
        return nullptr;

    if (SDL_MUSTLOCK(screenshot))
        SDL_LockSurface(screenshot);

    const size_t lineSize = 3 * w;
    GLubyte *const buf = new GLubyte[lineSize];

    // Grap the pixel buffer and write it to the SDL surface
    glGetIntegerv(GL_PACK_ALIGNMENT, &pack);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, screenshot->pixels);

    // Flip the screenshot, as OpenGL has 0,0 in bottom left
    const int h2 = h / 2;
    for (int i = 0; i < h2; i++)
    {
        GLubyte *const top = static_cast<GLubyte*>(
            screenshot->pixels) + lineSize * i;
        GLubyte *const bot = static_cast<GLubyte*>(
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

bool ModernOpenGLGraphics::pushClipArea(const Rect &area)
{
    const bool result = Graphics::pushClipArea(area);
    const ClipRect &clipArea = mClipStack.top();

    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
    return result;
}

void ModernOpenGLGraphics::popClipArea()
{
    if (mClipStack.empty())
        return;
    Graphics::popClipArea();
    if (mClipStack.empty())
        return;

    const ClipRect &clipArea = mClipStack.top();
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void ModernOpenGLGraphics::drawPoint(int x, int y)
{
    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    GLfloat vertices[] =
    {
        x + clipArea.xOffset, y + clipArea.yOffset, 0.0f, 0.0f
    };
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_POINTS, 0, 1);
}

void ModernOpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
{
    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    GLfloat vertices[] =
    {
        x1 + clipArea.xOffset, y1 + clipArea.yOffset, 0.0f, 0.0f,
        x2 + clipArea.xOffset, y2 + clipArea.yOffset, 0.0f, 0.0f
    };
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_LINES, 0, 2);
}

void ModernOpenGLGraphics::drawRectangle(const Rect& rect)
{
    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const int x1 = rect.x + clipArea.xOffset;
    const int y1 = rect.y + clipArea.yOffset;
    const int x2 = x1 + rect.width;
    const int y2 = y1 + rect.height;
    GLfloat vertices[] =
    {
        x1, y1, 0.0f, 0.0f,
        x1, y2, 0.0f, 0.0f,
        x2, y2, 0.0f, 0.0f,
        x2, y1, 0.0f, 0.0f
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void ModernOpenGLGraphics::fillRectangle(const Rect& rect)
{
    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const int x1 = rect.x + clipArea.xOffset;
    const int y1 = rect.y + clipArea.yOffset;
    const int x2 = x1 + rect.width;
    const int y2 = y1 + rect.height;
    GLfloat vertices[] =
    {
        x1, y1, 0.0f, 0.0f,
        x2, y1, 0.0f, 0.0f,
        x1, y2, 0.0f, 0.0f,
        x2, y2, 0.0f, 0.0f
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ModernOpenGLGraphics::setTexturingAndBlending(const bool enable)
{
    if (enable)
    {
        if (!mTextureDraw)
        {
            mTextureDraw = true;
            mglUniform1f(mDrawTypeUniform, 1.0f);
        }
        if (!mAlpha)
        {
            glEnable(GL_BLEND);
            mAlpha = true;
        }
    }
    else
    {
        if (mTextureDraw)
        {
            mTextureDraw = false;
            mglUniform1f(mDrawTypeUniform, 0.0f);
        }
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
    }
}

void ModernOpenGLGraphics::drawRectangle(const Rect& rect A_UNUSED,
                                         const bool filled A_UNUSED)
{
}

bool ModernOpenGLGraphics::drawNet(const int x1, const int y1,
                                   const int x2, const int y2,
                                   const int width, const int height)
{
    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const GLfloat dx = clipArea.xOffset;
    const GLfloat dy = clipArea.yOffset;

    const GLfloat xs1 = static_cast<GLfloat>(x1) + dx;
    const GLfloat xs2 = static_cast<GLfloat>(x2) + dx;
    const GLfloat ys1 = static_cast<GLfloat>(y1) + dy;
    const GLfloat ys2 = static_cast<GLfloat>(y2) + dy;

    for (int y = y1; y < y2; y += height)
    {
        mFloatArray[vp + 0] = xs1;
        mFloatArray[vp + 1] = y;
        mFloatArray[vp + 2] = 0.0f;
        mFloatArray[vp + 3] = 0.0f;

        mFloatArray[vp + 4] = xs2;
        mFloatArray[vp + 5] = y;
        mFloatArray[vp + 6] = 0.0f;
        mFloatArray[vp + 7] = 0.0f;

        vp += 8;
        if (vp >= vLimit)
        {
            drawLineArrays(vp);
            vp = 0;
        }
    }

    for (int x = x1; x < x2; x += width)
    {
        mFloatArray[vp + 0] = x;
        mFloatArray[vp + 1] = ys1;
        mFloatArray[vp + 2] = 0.0f;
        mFloatArray[vp + 3] = 0.0f;

        mFloatArray[vp + 4] = x;
        mFloatArray[vp + 5] = ys2;
        mFloatArray[vp + 6] = 0.0f;
        mFloatArray[vp + 7] = 0.0f;

        vp += 8;
        if (vp >= vLimit)
        {
            drawLineArrays(vp);
            vp = 0;
        }
    }

    if (vp > 0)
        drawLineArrays(vp);

    return true;
}

void ModernOpenGLGraphics::bindTexture(const GLenum target,
                                       const GLuint texture)
{
    if (mLastImage != texture)
    {
        mLastImage = texture;
        glBindTexture(target, texture);
    }
}

void ModernOpenGLGraphics::bindArrayBuffer(const GLuint vbo)
{
    if (mVboCached != vbo)
    {
        mVboCached = vbo;
        mglBindBuffer(GL_ARRAY_BUFFER, vbo);
        mAttributesCached = 0U;
    }
}

void ModernOpenGLGraphics::bindArrayBufferAndAttributes(const GLuint vbo)
{
    if (mVboCached != vbo)
    {
        mVboCached = vbo;
        mglBindBuffer(GL_ARRAY_BUFFER, vbo);

        mAttributesCached = mVboCached;
        mglBindVertexBuffer(0, mVboCached, 0, 4 * sizeof(GLfloat));
//        mglVertexAttribBinding(mPosAttrib, 0);
    }
    else if (mAttributesCached != mVboCached)
    {
        mAttributesCached = mVboCached;
        mglBindVertexBuffer(0, mVboCached, 0, 4 * sizeof(GLfloat));
//        mglVertexAttribBinding(mPosAttrib, 0);
    }
}

void ModernOpenGLGraphics::bindAttributes()
{
    if (mAttributesCached != mVboCached)
    {
        mAttributesCached = mVboCached;
        mglBindVertexBuffer(0, mVboCached, 0, 4 * sizeof(GLfloat));
//        mglVertexAttribBinding(mPosAttrib, 0);
    }
}

void ModernOpenGLGraphics::dumpSettings()
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

void ModernOpenGLGraphics::drawImageRect(const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &imgRect)
{
    #include "render/graphics_drawImageRect.hpp"
}

void ModernOpenGLGraphics::calcImageRect(ImageVertexes *const vert,
                                         const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &imgRect)
{
}

void ModernOpenGLGraphics::clearScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void ModernOpenGLGraphics::finalize(ImageCollection *const col)
{
    FOR_EACH (ImageCollectionIter, it, col->draws)
        finalize(*it);
}

void ModernOpenGLGraphics::finalize(ImageVertexes *const vert)
{
    // in future need convert in each switchVp/continueVp

    OpenGLGraphicsVertexes &ogl = vert->ogl;
    const std::vector<int> &vp = ogl.mVp;
    std::vector<int>::const_iterator ivp;
    const std::vector<int>::const_iterator ivp_end = vp.end();
    std::vector<GLfloat*> &floatTexPool = ogl.mFloatTexPool;
    std::vector<GLfloat*>::const_iterator ft;
    const std::vector<GLfloat*>::const_iterator ft_end = floatTexPool.end();
    std::vector<GLuint> &vbos = ogl.mVbo;
    std::vector<GLuint>::const_iterator ivbo;

    const int sz = floatTexPool.size();
    vbos.resize(sz);
    mglGenBuffers(sz, &vbos[0]);

    for (ft = floatTexPool.begin(), ivp = vp.begin(), ivbo = vbos.begin();
         ft != ft_end && ivp != ivp_end;
         ++ ft, ++ ivp, ++ ivbo)
    {
        bindArrayBuffer(*ivbo);
        mglBufferData(GL_ARRAY_BUFFER, (*ivp) * sizeof(GLfloat),
            *ft, GL_DYNAMIC_DRAW);
    }

    for (std::vector<GLfloat*>::iterator it = floatTexPool.begin();
        it != floatTexPool.end(); ++ it)
    {
        delete [] (*it);
    }
    floatTexPool.clear();
}

void ModernOpenGLGraphics::drawTriangleArray(const int size)
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        mFloatArray, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_TRIANGLES, 0, size / 4);
}

void ModernOpenGLGraphics::drawTriangleArray(const GLfloat *const array,
                                             const int size)
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        array, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_TRIANGLES, 0, size / 4);
}

void ModernOpenGLGraphics::drawLineArrays(const int size)
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        mFloatArray, GL_DYNAMIC_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    glDrawArrays(GL_LINES, 0, size / 4);
}

#ifdef DEBUG_BIND_TEXTURE
void ModernOpenGLGraphics::debugBindTexture(const Image *const image)
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
void ModernOpenGLGraphics::debugBindTexture(const Image *const image A_UNUSED)
{
}
#endif

#endif  // USE_OPENGL
