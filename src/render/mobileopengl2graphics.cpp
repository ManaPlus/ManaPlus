/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#if defined USE_OPENGL && !defined ANDROID

#include "render/mobileopengl2graphics.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "graphicsvertexes.h"
#include "logger.h"

#include "render/mgl.h"
#ifdef __native_client__
#include "render/naclglfunctions.h"
#endif

#include "render/shaders/shaderprogram.h"
#include "render/shaders/shadersmanager.h"

#include "resources/image.h"
#include "resources/imagerect.h"
#include "resources/openglimagehelper.h"

#include "utils/delete2.h"
#include "utils/sdlcheckutils.h"
#include "utils/sdlhelper.h"

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

#define toGL static_cast<GLfloat>

GLuint MobileOpenGL2Graphics::mTextureBinded = 0U;
GLuint MobileOpenGL2Graphics::mTextureSizeUniform = 0U;
int MobileOpenGL2Graphics::mTextureWidth = 1;
int MobileOpenGL2Graphics::mTextureHeight = 1;
#ifdef DEBUG_DRAW_CALLS
unsigned int MobileOpenGL2Graphics::mDrawCalls = 0U;
unsigned int MobileOpenGL2Graphics::mLastDrawCalls = 0U;
#endif

MobileOpenGL2Graphics::MobileOpenGL2Graphics() :
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
#ifndef __native_client__
    mVao(0U),
#endif
    mVbo(0U),
    mVboBinded(0U),
    mAttributesBinded(0U),
    mColorAlpha(false),
    mTextureDraw(false),
#ifdef DEBUG_BIND_TEXTURE
    mOldTexture(),
    mOldTextureId(0),
#endif
    mFbo()
{
    mOpenGL = RENDER_GLES2_OPENGL;
    mName = "mobile OpenGL ES 2";
}

MobileOpenGL2Graphics::~MobileOpenGL2Graphics()
{
    deleteArraysInternal();
    deleteGLObjects();
}

void MobileOpenGL2Graphics::deleteGLObjects()
{
    delete2(mProgram);
    if (mVbo)
        mglDeleteBuffers(1, &mVbo);
#ifndef __native_client__
    if (mVao)
        mglDeleteVertexArrays(1, &mVao);
#endif
}

void MobileOpenGL2Graphics::initArrays(const int vertCount)
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

void MobileOpenGL2Graphics::postInit()
{
#ifndef __native_client__
    mglGenVertexArrays(1, &mVao);
    mglBindVertexArray(mVao);
#endif
    mglGenBuffers(1, &mVbo);
//    logger->log("gen vbo buffer: %u", mVbo);
    bindArrayBuffer(mVbo);

    logger->log("Compiling shaders");
    mProgram = shaders.getGles2Program();
    if (!mProgram)
    {
        graphicsManager.logError();
        logger->safeError("Shader creation error. See manaplus.log.");
    }
    mProgramId = mProgram->getProgramId();
    if (!mProgram)
        logger->safeError("Shaders compilation error.");

    logger->log("Shaders compilation done.");
    mglUseProgram(mProgramId);

    mPosAttrib = 0;

    mSimpleColorUniform = mglGetUniformLocation(mProgramId, "color");
    mScreenUniform = mglGetUniformLocation(mProgramId, "screen");
    mDrawTypeUniform = mglGetUniformLocation(mProgramId, "drawType");
    mTextureColorUniform = mglGetUniformLocation(mProgramId, "alpha");
    mTextureSizeUniform = mglGetUniformLocation(mProgramId, "textureSize");

    mglUniform1f(mTextureColorUniform, 1.0f);

    mglVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    mglEnableVertexAttribArray(0);
    mAttributesBinded = mVbo;

    mglUniform2f(mScreenUniform,
        static_cast<float>(mWidth) / 2.0f,
        static_cast<float>(mHeight) / 2.0f);
    // for safty init texture size to 1x1
    mglUniform2f(mTextureSizeUniform,
        1.0f,
        1.0f);

    mglActiveTexture(GL_TEXTURE0);
}

void MobileOpenGL2Graphics::screenResized()
{
    deleteGLObjects();
    mVboBinded = 0U;
    mAttributesBinded = 0U;
    postInit();
}

void MobileOpenGL2Graphics::deleteArrays()
{
    deleteArraysInternal();
}

void MobileOpenGL2Graphics::deleteArraysInternal()
{
    delete [] mFloatArray;
    mFloatArray = nullptr;
    delete [] mFloatArrayCached;
    mFloatArrayCached = nullptr;
}

bool MobileOpenGL2Graphics::setVideoMode(const int w, const int h,
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

void MobileOpenGL2Graphics::setColor(const Color &color)
{
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

void MobileOpenGL2Graphics::setColorAlpha(const float alpha)
{
    if (mAlphaCached != alpha)
    {
        mAlphaCached = alpha;
        mglUniform1f(mTextureColorUniform, alpha);
    }
}

void MobileOpenGL2Graphics::drawQuad(const int srcX, const int srcY,
                                     const int dstX, const int dstY,
                                     const int width, const int height)
{
    const GLfloat texX2 = srcX + width;
    const GLfloat texY2 = srcY + height;
    const GLfloat x2 = dstX + width;
    const GLfloat y2 = dstY + height;
    const GLfloat srcX2 = toGL(srcX);
    const GLfloat srcY2 = toGL(srcY);
    const GLfloat dstX2 = toGL(dstX);
    const GLfloat dstY2 = toGL(dstY);

    GLfloat vertices[] =
    {
        dstX2, dstY2, srcX2, srcY2,
        x2,    dstY2, texX2, srcY2,
        dstX2, y2,    srcX2, texY2,
        x2,    y2,    texX2, texY2
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MobileOpenGL2Graphics::drawRescaledQuad(const int srcX, const int srcY,
                                             const int dstX, const int dstY,
                                             const int width, const int height,
                                             const int desiredWidth,
                                             const int desiredHeight)
{
    const GLfloat texX2 = srcX + width;
    const GLfloat texY2 = srcY + height;
    const GLfloat x2 = dstX + desiredWidth;
    const GLfloat y2 = dstY + desiredHeight;
    const GLfloat srcX2 = toGL(srcX);
    const GLfloat srcY2 = toGL(srcY);
    const GLfloat dstX2 = toGL(dstX);
    const GLfloat dstY2 = toGL(dstY);

    GLfloat vertices[] =
    {
        dstX2, dstY2, srcX2, srcY2,
        x2,    dstY2, texX2, srcY2,
        dstX2, y2,    srcX2, texY2,
        x2,    y2,    texX2, texY2
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MobileOpenGL2Graphics::drawImage(const Image *const image,
                                      int dstX, int dstY)
{
    drawImageInline(image, dstX, dstY);
}

void MobileOpenGL2Graphics::drawImageInline(const Image *const image,
                                            int dstX, int dstY)
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (!image)
        return;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture2(GL_TEXTURE_2D, image);
    setTexturingAndBlending(true);
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    const ClipRect &clipArea = mClipStack.top();
    const SDL_Rect &imageRect = image->mBounds;
    drawQuad(imageRect.x,
        imageRect.y,
        dstX + clipArea.xOffset,
        dstY + clipArea.yOffset,
        imageRect.w,
        imageRect.h);
}

void MobileOpenGL2Graphics::copyImage(const Image *const image,
                                      int dstX, int dstY)
{
    drawImageInline(image, dstX, dstY);
}

void MobileOpenGL2Graphics::testDraw()
{
/*
    GLfloat vertices[] =
    {
        0, 0, 0, 0,
        800, 0, 800, 0,
        0, 600, 0, 600,
        800, 600, 800, 600
    };
*/
//    logger->log("allocate: %d, %ld", mVboBinded, sizeof(vertices));
//    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
//        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
}

void MobileOpenGL2Graphics::drawImageCached(const Image *const image A_UNUSED,
                                            int A_UNUSED x, int y A_UNUSED)
{
}

void MobileOpenGL2Graphics::drawPatternCached(const Image *const image A_UNUSED,
                                              const int x A_UNUSED,
                                              const int y A_UNUSED,
                                              const int w A_UNUSED,
                                              const int h A_UNUSED)
{
}

void MobileOpenGL2Graphics::completeCache()
{
}

void MobileOpenGL2Graphics::drawRescaledImage(const Image *const image,
                                              int dstX, int dstY,
                                              const int desiredWidth,
                                              const int desiredHeight)
{
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
#endif
    bindTexture2(GL_TEXTURE_2D, image);
    setTexturingAndBlending(true);
    bindArrayBufferAndAttributes(mVbo);

    const ClipRect &clipArea = mClipStack.top();
    // Draw a textured quad.
    drawRescaledQuad(imageRect.x,
        imageRect.y,
        dstX + clipArea.xOffset,
        dstY + clipArea.yOffset,
        imageRect.w,
        imageRect.h,
        desiredWidth,
        desiredHeight);
}

void MobileOpenGL2Graphics::drawPattern(const Image *const image,
                                        const int x, const int y,
                                        const int w, const int h)
{ 
    drawPatternInline(image, x, y, w, h);
}

void MobileOpenGL2Graphics::drawPatternInline(const Image *const image,
                                              const int x, const int y,
                                              const int w, const int h)
{
    if (!image)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;

    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const GLfloat srcX2 = toGL(srcX);
    const GLfloat srcY2 = toGL(srcY);
    const ClipRect &clipArea = mClipStack.top();
    const int x2 = x + clipArea.xOffset;
    const int y2 = y + clipArea.yOffset;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture2(GL_TEXTURE_2D, image);

    setTexturingAndBlending(true);
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const GLfloat texY2 = srcY + height;
        const GLfloat dstY = y2 + py;
        for (int px = 0; px < w; px += iw)
        {
            const int width = (px + iw >= w) ? w - px : iw;
            const GLfloat dstX = x2 + px;
            const GLfloat texX2 = srcX + width;

            vertFill2D(mFloatArray,
                srcX2, srcY2, texX2, texY2,
                dstX,  dstY,  width, height);

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

void MobileOpenGL2Graphics::drawRescaledPattern(const Image *const image,
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
    const GLfloat srcX2 = toGL(srcX);
    const GLfloat srcY2 = toGL(srcY);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture2(GL_TEXTURE_2D, image);

    setTexturingAndBlending(true);
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    const ClipRect &clipArea = mClipStack.top();
    const int x2 = x + clipArea.xOffset;
    const int y2 = y + clipArea.yOffset;

    const float scaleFactorW = static_cast<float>(scaledWidth) / iw;
    const float scaleFactorH = static_cast<float>(scaledHeight) / ih;

    for (int py = 0; py < h; py += scaledHeight)
    {
        const int height = (py + scaledHeight >= h)
            ? h - py : scaledHeight;
        const GLfloat dstY = y2 + py;
        const GLfloat scaledY = srcY + height / scaleFactorH;
        for (int px = 0; px < w; px += scaledWidth)
        {
            const int width = (px + scaledWidth >= w)
                ? w - px : scaledWidth;
            const int dstX = x2 + px;
            const GLfloat scaledX = srcX + width / scaleFactorW;

            vertFill2D(mFloatArray,
                srcX2, srcY2, scaledX, scaledY,
                dstX,  dstY,  width,   height);

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

inline void MobileOpenGL2Graphics::drawVertexes(const
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
        mglDrawArrays(GL_TRIANGLES, 0, *ivp / 4);
    }
}

void MobileOpenGL2Graphics::calcPattern(ImageVertexes *const vert,
                                        const Image *const image,
                                        const int x, const int y,
                                        const int w, const int h) const
{
    calcPatternInline(vert, image, x, y, w, h);
}

void MobileOpenGL2Graphics::calcPatternInline(ImageVertexes *const vert,
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
    const GLfloat srcX2 = toGL(srcX);
    const GLfloat srcY2 = toGL(srcY);

    const ClipRect &clipArea = mClipStack.top();
    const int x2 = x + clipArea.xOffset;
    const int y2 = y + clipArea.yOffset;

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;
    unsigned int vp = ogl.continueVp();

    GLfloat *floatArray = ogl.continueFloatTexArray();

    for (int py = 0; py < h; py += ih)
    {
        const GLfloat height = (py + ih >= h) ? h - py : ih;
        const GLfloat dstY = y2 + py;
        const GLfloat texY2 = srcY + height;
        for (int px = 0; px < w; px += iw)
        {
            const GLfloat width = (px + iw >= w) ? w - px : iw;
            const GLfloat dstX = x2 + px;
            const GLfloat texX2 = srcX2 + width;

            vertFill2D(floatArray,
                srcX2, srcY2, texX2, texY2,
                dstX,  dstY,  width, height);

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

void MobileOpenGL2Graphics::calcTileCollection(ImageCollection *const vertCol,
                                               const Image *const image,
                                               int x, int y)
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

void MobileOpenGL2Graphics::drawTileCollection(const ImageCollection
                                               *const vertCol)
{
    if (!vertCol)
        return;
    setTexturingAndBlending(true);
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
        bindTexture2(GL_TEXTURE_2D, image);
        drawVertexes(vert->ogl);
    }
}

void MobileOpenGL2Graphics::calcPattern(ImageCollection* const vertCol,
                                        const Image *const image,
                                        const int x, const int y,
                                        const int w, const int h) const
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

void MobileOpenGL2Graphics::calcTileVertexes(ImageVertexes *const vert,
                                             const Image *const image,
                                             int dstX, int dstY) const
{
    calcTileVertexesInline(vert, image, dstX, dstY);
}

void MobileOpenGL2Graphics::calcTileVertexesInline(ImageVertexes *const vert,
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
    const GLfloat srcX2 = toGL(srcX);
    const GLfloat srcY2 = toGL(srcY);

    const ClipRect &clipArea = mClipStack.top();
    const GLfloat x2 = dstX + clipArea.xOffset;
    const GLfloat y2 = dstY + clipArea.yOffset;

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;

    unsigned int vp = ogl.continueVp();

    GLfloat texX2 = srcX + w;
    GLfloat texY2 = srcY + h;

    GLfloat *const floatArray = ogl.continueFloatTexArray();

    vertFill2D(floatArray,
        srcX2, srcY2, texX2, texY2,
        x2,    y2,    w,     h);

    vp += 24;
    if (vp >= vLimit)
    {
        ogl.switchFloatTexArray();
        ogl.switchVp(vp);
        vp = 0;
    }

    ogl.switchVp(vp);
}

void MobileOpenGL2Graphics::drawTileVertexes(const ImageVertexes *const vert)
{
    if (!vert)
        return;
    const Image *const image = vert->image;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture2(GL_TEXTURE_2D, image);
    setTexturingAndBlending(true);
    bindArrayBufferAndAttributes(mVbo);

    drawVertexes(vert->ogl);
}

void MobileOpenGL2Graphics::calcWindow(ImageCollection *const vertCol,
                                       const int x, const int y,
                                       const int w, const int h,
                                       const ImageRect &imgRect)
{
    if (!vertCol)
        return;
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

void MobileOpenGL2Graphics::updateScreen()
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
    if (isGLNotNull(mglFrameTerminator))
        mglFrameTerminator();
#endif
    BLOCK_END("Graphics::updateScreen")
}

void MobileOpenGL2Graphics::beginDraw()
{
    setOpenGLFlags();
#ifndef __native_client__
    mglDisable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
    mglHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB, GL_FASTEST);
#endif
    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void MobileOpenGL2Graphics::endDraw()
{
    popClipArea();
}

void MobileOpenGL2Graphics::prepareScreenshot()
{
    if (config.getBoolValue("usefbo"))
        graphicsManager.createFBO(mRect.w, mRect.h, &mFbo);
}

SDL_Surface* MobileOpenGL2Graphics::getScreenshot()
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
    mglGetIntegerv(GL_PACK_ALIGNMENT, &pack);
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

void MobileOpenGL2Graphics::pushClipArea(const Rect &area)
{
    Graphics::pushClipArea(area);
    const ClipRect &clipArea = mClipStack.top();

    mglScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void MobileOpenGL2Graphics::popClipArea()
{
    if (mClipStack.empty())
        return;
    Graphics::popClipArea();
    if (mClipStack.empty())
        return;

    const ClipRect &clipArea = mClipStack.top();
    mglScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void MobileOpenGL2Graphics::drawPoint(int x, int y)
{
    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    GLfloat vertices[] =
    {
        toGL(x + clipArea.xOffset), toGL(y + clipArea.yOffset), 0.0f, 0.0f
    };
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_POINTS, 0, 1);
}

void MobileOpenGL2Graphics::drawLine(int x1, int y1, int x2, int y2)
{
    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    GLfloat vertices[] =
    {
        toGL(x1 + clipArea.xOffset), toGL(y1 + clipArea.yOffset), 0.0f, 0.0f,
        toGL(x2 + clipArea.xOffset), toGL(y2 + clipArea.yOffset), 0.0f, 0.0f
    };
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_LINES, 0, 2);
}

void MobileOpenGL2Graphics::drawRectangle(const Rect& rect)
{
    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const GLfloat x1 = rect.x + clipArea.xOffset;
    const GLfloat y1 = rect.y + clipArea.yOffset;
    const GLfloat x2 = x1 + rect.width;
    const GLfloat y2 = y1 + rect.height;
    GLfloat vertices[] =
    {
        x1, y1, 0.0f, 0.0f,
        x1, y2, 0.0f, 0.0f,
        x2, y2, 0.0f, 0.0f,
        x2, y1, 0.0f, 0.0f
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_LINE_LOOP, 0, 4);
}

void MobileOpenGL2Graphics::fillRectangle(const Rect& rect)
{
    setTexturingAndBlending(false);
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const GLfloat x1 = rect.x + clipArea.xOffset;
    const GLfloat y1 = rect.y + clipArea.yOffset;
    const GLfloat x2 = x1 + rect.width;
    const GLfloat y2 = y1 + rect.height;
    GLfloat vertices[] =
    {
        x1, y1, 0.0f, 0.0f,
        x2, y1, 0.0f, 0.0f,
        x1, y2, 0.0f, 0.0f,
        x2, y2, 0.0f, 0.0f
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MobileOpenGL2Graphics::setTexturingAndBlending(const bool enable)
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
            mglEnable(GL_BLEND);
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
            mglDisable(GL_BLEND);
            mAlpha = false;
        }
        else if (!mAlpha && mColorAlpha)
        {
            mglEnable(GL_BLEND);
            mAlpha = true;
        }
    }
}

void MobileOpenGL2Graphics::drawRectangle(const Rect& rect A_UNUSED,
                                          const bool filled A_UNUSED)
{
}

void MobileOpenGL2Graphics::drawNet(const int x1, const int y1,
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

    const GLfloat xs1 = x1 + dx;
    const GLfloat xs2 = x2 + dx;
    const GLfloat ys1 = y1 + dy;
    const GLfloat ys2 = y2 + dy;

    for (int y = y1; y < y2; y += height)
    {
        mFloatArray[vp + 0] = xs1;
        mFloatArray[vp + 1] = toGL(y);
        mFloatArray[vp + 2] = 0.0f;
        mFloatArray[vp + 3] = 0.0f;

        mFloatArray[vp + 4] = xs2;
        mFloatArray[vp + 5] = toGL(y);
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
        mFloatArray[vp + 0] = toGL(x);
        mFloatArray[vp + 1] = ys1;
        mFloatArray[vp + 2] = 0.0f;
        mFloatArray[vp + 3] = 0.0f;

        mFloatArray[vp + 4] = toGL(x);
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
}

void MobileOpenGL2Graphics::bindTexture2(const GLenum target,
                                         const Image *const image)
{
    const GLuint texture = image->mGLImage;
    if (mTextureBinded != texture)
    {
        mTextureBinded = texture;
        mglBindTexture(target, texture);
        if (mTextureWidth != image->mTexWidth ||
            mTextureHeight != image->mTexHeight)
        {
            mTextureWidth = image->mTexWidth;
            mTextureHeight = image->mTexHeight;
            mglUniform2f(mTextureSizeUniform,
                image->mTexWidth,
                image->mTexHeight);
        }
    }
}

void MobileOpenGL2Graphics::bindTexture(const GLenum target A_UNUSED,
                                        const GLuint texture A_UNUSED)
{
    if (mTextureBinded != texture)
    {
        // for safty not storing textures because cant update size uniform
        mTextureBinded = 0;
        mglBindTexture(target, texture);
    }
}

void MobileOpenGL2Graphics::removeArray(const uint32_t sz,
                                        uint32_t *const arr)
{
    mglDeleteBuffers(sz, arr);
    for (size_t f = 0; f < sz; f ++)
    {
        if (arr[f] == mVboBinded)
            mVboBinded = 0;
    }
}

void MobileOpenGL2Graphics::bindArrayBuffer(const GLuint vbo)
{
    if (mVboBinded != vbo)
    {
        mVboBinded = vbo;
        mglBindBuffer(GL_ARRAY_BUFFER, vbo);
        mAttributesBinded = 0U;
    }
}

void MobileOpenGL2Graphics::bindArrayBufferAndAttributes(const GLuint vbo)
{
    if (mVboBinded != vbo)
    {
        mVboBinded = vbo;
        mglBindBuffer(GL_ARRAY_BUFFER, vbo);

        mAttributesBinded = mVboBinded;
        mglVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
    else if (mAttributesBinded != mVboBinded)
    {
        mAttributesBinded = mVboBinded;
        mglVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
}

void MobileOpenGL2Graphics::bindAttributes()
{
    if (mAttributesBinded != mVboBinded)
    {
        mAttributesBinded = mVboBinded;
        mglVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
}

void MobileOpenGL2Graphics::dumpSettings()
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

void MobileOpenGL2Graphics::drawImageRect(const int x, const int y,
                                          const int w, const int h,
                                          const ImageRect &imgRect)
{
    #include "render/graphics_drawImageRect.hpp"
}

void MobileOpenGL2Graphics::calcImageRect(ImageVertexes *const vert,
                                          const int x, const int y,
                                          const int w, const int h,
                                          const ImageRect &imgRect)
{
    #include "render/graphics_calcImageRect.hpp"
}

void MobileOpenGL2Graphics::clearScreen() const
{
    mglClear(GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT |
        GL_STENCIL_BUFFER_BIT);
}

void MobileOpenGL2Graphics::createGLContext()
{
    Graphics::createGLContext();
/*
    if (mGLContext)
        SDL::makeCurrentContext(mGLContext);
    else
        mGLContext = SDL::createGLContext(mWindow, 2, 0, 0x04);
*/
}

void MobileOpenGL2Graphics::finalize(ImageCollection *const col)
{
    if (!col)
        return;
    FOR_EACH (ImageCollectionIter, it, col->draws)
        finalize(*it);
}

void MobileOpenGL2Graphics::finalize(ImageVertexes *const vert)
{
    // in future need convert in each switchVp/continueVp

    if (!vert)
        return;
    OpenGLGraphicsVertexes &ogl = vert->ogl;
    const std::vector<int> &vp = ogl.mVp;
    std::vector<int>::const_iterator ivp;
    const std::vector<int>::const_iterator ivp_end = vp.end();
    std::vector<GLfloat*> &floatTexPool = ogl.mFloatTexPool;
    std::vector<GLfloat*>::const_iterator ft;
    const std::vector<GLfloat*>::const_iterator ft_end = floatTexPool.end();
    std::vector<GLuint> &vbos = ogl.mVbo;
    std::vector<GLuint>::const_iterator ivbo;

    const int sz = static_cast<int>(floatTexPool.size());
    vbos.resize(sz);
    mglGenBuffers(sz, &vbos[0]);

    for (ft = floatTexPool.begin(), ivp = vp.begin(), ivbo = vbos.begin();
         ft != ft_end && ivp != ivp_end;
         ++ ft, ++ ivp, ++ ivbo)
    {
        bindArrayBuffer(*ivbo);
        mglBufferData(GL_ARRAY_BUFFER, (*ivp) * sizeof(GLfloat),
            *ft, GL_STATIC_DRAW);
    }

    for (std::vector<GLfloat*>::iterator it = floatTexPool.begin();
        it != floatTexPool.end(); ++ it)
    {
        delete [] (*it);
    }
    floatTexPool.clear();
}

void MobileOpenGL2Graphics::drawTriangleArray(const int size)
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        mFloatArray, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_TRIANGLES, 0, size / 4);
}

void MobileOpenGL2Graphics::drawTriangleArray(const GLfloat *const array,
                                              const int size)
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        array, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_TRIANGLES, 0, size / 4);
}

void MobileOpenGL2Graphics::drawLineArrays(const int size)
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        mFloatArray, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif
    mglDrawArrays(GL_LINES, 0, size / 4);
}

#ifdef DEBUG_BIND_TEXTURE
void MobileOpenGL2Graphics::debugBindTexture(const Image *const image)
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
void MobileOpenGL2Graphics::debugBindTexture(const Image *const image A_UNUSED)
{
}
#endif

#endif  // USE_OPENGL
