/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "graphicsmanager.h"

#include "render/opengl/mgl.h"
#ifdef __native_client__
#include "render/opengl/naclglfunctions.h"
#endif  // __native_client__

#include "render/shaders/shaderprogram.h"
#include "render/shaders/shadersmanager.h"

#include "render/vertexes/imagecollection.h"

#include "resources/imagerect.h"

#include "resources/image/image.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
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
    var[vp + 23] = y2

#define toGL static_cast<GLfloat>

GLuint MobileOpenGL2Graphics::mTextureBinded = 0U;
GLuint MobileOpenGL2Graphics::mTextureSizeUniform = 0U;
int MobileOpenGL2Graphics::mTextureWidth = 1;
int MobileOpenGL2Graphics::mTextureHeight = 1;
#ifdef DEBUG_DRAW_CALLS
unsigned int MobileOpenGL2Graphics::mDrawCalls = 0U;
unsigned int MobileOpenGL2Graphics::mLastDrawCalls = 0U;
#endif  // DEBUG_DRAW_CALLS

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
#endif  // __native_client__
    mVbo(0U),
    mVboBinded(0U),
    mAttributesBinded(0U),
    mColorAlpha(false),
    mTextureDraw(false),
#ifdef DEBUG_BIND_TEXTURE
    mOldTexture(),
    mOldTextureId(0),
#endif  // DEBUG_BIND_TEXTURE
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

void MobileOpenGL2Graphics::deleteGLObjects() restrict2
{
    delete2(mProgram)
    if (mVbo != 0U)
        mglDeleteBuffers(1, &mVbo);
#ifndef __native_client__
    if (mVao != 0U)
        mglDeleteVertexArrays(1, &mVao);
#endif  // __native_client__
}

void MobileOpenGL2Graphics::initArrays(const int vertCount) restrict2
{
    mMaxVertices = vertCount;
    if (mMaxVertices < 500)
        mMaxVertices = 500;
    else if (mMaxVertices > 1024)
        mMaxVertices = 1024;

    // need alocate small size, after if limit reached reallocate to double size
    const size_t sz = mMaxVertices * 4 + 30;
    vertexBufSize = mMaxVertices;
    if (mFloatArray == nullptr)
        mFloatArray = new GLfloat[sz];
    if (mFloatArrayCached == nullptr)
        mFloatArrayCached = new GLfloat[sz];
}

void MobileOpenGL2Graphics::postInit() restrict2
{
#ifndef __native_client__
    mglGenVertexArrays(1, &mVao);
    mglBindVertexArray(mVao);
#endif  // __native_client__
    mglGenBuffers(1, &mVbo);
//    logger->log("gen vbo buffer: %u", mVbo);
    bindArrayBuffer(mVbo);

    logger->log("Compiling shaders");
    mProgram = shaders.getGles2Program();
    if (mProgram == nullptr)
    {
        graphicsManager.logError();
        logger->safeError("Shader creation error. See manaplus.log.");
    }
    mProgramId = mProgram->getProgramId();
    if (mProgramId == 0U)
        logger->safeError("Shaders compilation error.");

    logger->log("Shaders compilation done.");
    mglUseProgram(mProgramId);

    mPosAttrib = 0;

    mSimpleColorUniform = mglGetUniformLocation(mProgramId, "color");
    mScreenUniform = mglGetUniformLocation(mProgramId, "screen");
    mDrawTypeUniform = mglGetUniformLocation(mProgramId, "drawType");
    mTextureColorUniform = mglGetUniformLocation(mProgramId, "alpha");
    mTextureSizeUniform = mglGetUniformLocation(mProgramId, "textureSize");

    mglUniform1f(mTextureColorUniform, 1.0F);

    mglVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    mglEnableVertexAttribArray(0);
    mAttributesBinded = mVbo;

    mglUniform2f(mScreenUniform,
        static_cast<float>(mWidth) / 2.0F,
        static_cast<float>(mHeight) / 2.0F);
    // for safty init texture size to 1x1
    mglUniform2f(mTextureSizeUniform,
        1.0F,
        1.0F);
    mglUniform4f(mSimpleColorUniform,
        0.0F,
        0.0F,
        0.0F,
        0.0F);

    mglActiveTexture(GL_TEXTURE0);
}

void MobileOpenGL2Graphics::screenResized() restrict2
{
    deleteGLObjects();
    mVboBinded = 0U;
    mAttributesBinded = 0U;
    mColor = Color(0, 0, 0, 0);
    postInit();
}

void MobileOpenGL2Graphics::deleteArrays() restrict2
{
    deleteArraysInternal();
}

void MobileOpenGL2Graphics::deleteArraysInternal() restrict2
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

void MobileOpenGL2Graphics::setColor(const Color &restrict color) restrict2
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

void MobileOpenGL2Graphics::setColorAlpha(const float alpha) restrict2
{
    if (mAlphaCached != alpha)
    {
        mAlphaCached = alpha;
        mglUniform1f(mTextureColorUniform, alpha);
    }
}

void MobileOpenGL2Graphics::drawQuad(const int srcX,
                                     const int srcY,
                                     const int dstX,
                                     const int dstY,
                                     const int width,
                                     const int height) restrict2
{
    const GLfloat texX2 = static_cast<GLfloat>(srcX + width);
    const GLfloat texY2 = static_cast<GLfloat>(srcY + height);
    const GLfloat x2 = static_cast<GLfloat>(dstX + width);
    const GLfloat y2 = static_cast<GLfloat>(dstY + height);
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
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::drawRescaledQuad(const int srcX, const int srcY,
                                             const int dstX, const int dstY,
                                             const int width, const int height,
                                             const int desiredWidth,
                                             const int desiredHeight) restrict2
{
    const GLfloat texX2 = static_cast<GLfloat>(srcX + width);
    const GLfloat texY2 = static_cast<GLfloat>(srcY + height);
    const GLfloat x2 = static_cast<GLfloat>(dstX + desiredWidth);
    const GLfloat y2 = static_cast<GLfloat>(dstY + desiredHeight);
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
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::drawImage(const Image *restrict const image,
                                      int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void MobileOpenGL2Graphics::drawImageInline(const Image *restrict const image,
                                            int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (image == nullptr)
        return;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture2(GL_TEXTURE_2D, image);
    enableTexturingAndBlending();
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

void MobileOpenGL2Graphics::copyImage(const Image *restrict const image,
                                      int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void MobileOpenGL2Graphics::testDraw() restrict2
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
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::drawImageCached(const Image *restrict const image
                                            A_UNUSED,
                                            int A_UNUSED x,
                                            int y A_UNUSED) restrict2
{
}

void MobileOpenGL2Graphics::drawPatternCached(const Image *restrict const image
                                              A_UNUSED,
                                              const int x A_UNUSED,
                                              const int y A_UNUSED,
                                              const int w A_UNUSED,
                                              const int h A_UNUSED) restrict2
{
}

void MobileOpenGL2Graphics::completeCache() restrict2
{
}

void MobileOpenGL2Graphics::drawRescaledImage(const Image *
                                              restrict const image,
                                              int dstX, int dstY,
                                              const int desiredWidth,
                                              const int desiredHeight)
                                              restrict2
{
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
    bindTexture2(GL_TEXTURE_2D, image);
    enableTexturingAndBlending();
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

void MobileOpenGL2Graphics::drawPattern(const Image *restrict const image,
                                        const int x, const int y,
                                        const int w, const int h) restrict2
{
    drawPatternInline(image, x, y, w, h);
}

void MobileOpenGL2Graphics::drawPatternInline(const Image *
                                              restrict const image,
                                              const int x, const int y,
                                              const int w, const int h)
                                              restrict2
{
    if (image == nullptr)
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
#endif  // DEBUG_BIND_TEXTURE
    bindTexture2(GL_TEXTURE_2D, image);

    enableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const GLfloat texY2 = static_cast<GLfloat>(srcY + height);
        const GLfloat dstY = static_cast<GLfloat>(y2 + py);
        for (int px = 0; px < w; px += iw)
        {
            const int width = (px + iw >= w) ? w - px : iw;
            const GLfloat dstX = static_cast<GLfloat>(x2 + px);
            const GLfloat texX2 = static_cast<GLfloat>(srcX + width);

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

void MobileOpenGL2Graphics::drawRescaledPattern(const Image *
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
    const GLfloat srcX2 = toGL(srcX);
    const GLfloat srcY2 = toGL(srcY);

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture2(GL_TEXTURE_2D, image);

    enableTexturingAndBlending();
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
        const GLfloat dstY = static_cast<GLfloat>(y2 + py);
        const GLfloat scaledY = srcY + height / scaleFactorH;
        for (int px = 0; px < w; px += scaledWidth)
        {
            const int width = (px + scaledWidth >= w)
                ? w - px : scaledWidth;
            const GLfloat dstX = static_cast<GLfloat>(x2 + px);
            const GLfloat scaledX = srcX + width / scaleFactorW;

            vertFill2D(mFloatArray,
                srcX2, srcY2,
                scaledX, scaledY,
                dstX, dstY,
                static_cast<GLfloat>(width), static_cast<GLfloat>(height));

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
                                                OpenGLGraphicsVertexes &
                                                restrict ogl) restrict2
{
    const STD_VECTOR<int> &vp = ogl.mVp;
    const STD_VECTOR<GLuint> &vbos = ogl.mVbo;
    STD_VECTOR<int>::const_iterator ivp;
    STD_VECTOR<GLuint>::const_iterator ivbo;
    const STD_VECTOR<int>::const_iterator ivp_end = vp.end();

    for (ivp = vp.begin(), ivbo = vbos.begin();
         ivp != ivp_end;
         ++ ivp, ++ ivbo)
    {
        bindArrayBufferAndAttributes(*ivbo);
#ifdef DEBUG_DRAW_CALLS
        mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
        mglDrawArrays(GL_TRIANGLES, 0, *ivp / 4);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
    }
}

void MobileOpenGL2Graphics::calcPattern(ImageVertexes *restrict const vert,
                                        const Image *restrict const image,
                                        const int x,
                                        const int y,
                                        const int w,
                                        const int h) const restrict2
{
    calcPatternInline(vert, image, x, y, w, h);
}

void MobileOpenGL2Graphics::calcPatternInline(ImageVertexes *
                                              restrict const vert,
                                              const Image *
                                              restrict const image,
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
        const GLfloat height = static_cast<GLfloat>(
            (py + ih >= h) ? h - py : ih);
        const GLfloat dstY = static_cast<GLfloat>(y2 + py);
        const GLfloat texY2 = srcY + height;
        for (int px = 0; px < w; px += iw)
        {
            const GLfloat width = static_cast<GLfloat>(
                (px + iw >= w) ? w - px : iw);
            const GLfloat dstX = static_cast<GLfloat>(x2 + px);
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

void MobileOpenGL2Graphics::calcTileCollection(ImageCollection *
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

void MobileOpenGL2Graphics::drawTileCollection(const ImageCollection
                                               *restrict const vertCol)
                                               restrict2
{
    enableTexturingAndBlending();
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
        bindTexture2(GL_TEXTURE_2D, image);
        drawVertexes(vert->ogl);
    }
}

void MobileOpenGL2Graphics::calcPattern(ImageCollection *restrict const
                                        vertCol,
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

void MobileOpenGL2Graphics::calcTileVertexes(ImageVertexes *
                                             restrict const vert,
                                             const Image *restrict const image,
                                             int dstX,
                                             int dstY) const restrict2
{
    calcTileVertexesInline(vert, image, dstX, dstY);
}

void MobileOpenGL2Graphics::calcTileVertexesInline(ImageVertexes *
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
    const GLfloat srcX2 = toGL(srcX);
    const GLfloat srcY2 = toGL(srcY);

    const ClipRect &clipArea = mClipStack.top();
    const GLfloat x2 = static_cast<GLfloat>(dstX + clipArea.xOffset);
    const GLfloat y2 = static_cast<GLfloat>(dstY + clipArea.yOffset);

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;

    unsigned int vp = ogl.continueVp();

    GLfloat texX2 = static_cast<GLfloat>(srcX + w);
    GLfloat texY2 = static_cast<GLfloat>(srcY + h);

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

void MobileOpenGL2Graphics::drawTileVertexes(const ImageVertexes *
                                             restrict const vert) restrict2
{
    if (vert == nullptr)
        return;
    const Image *const image = vert->image;

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture2(GL_TEXTURE_2D, image);
    enableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);

    drawVertexes(vert->ogl);
}

void MobileOpenGL2Graphics::calcWindow(ImageCollection *restrict const vertCol,
                                       const int x, const int y,
                                       const int w, const int h,
                                       const ImageRect &restrict imgRect)
                                       restrict2
{
    ImageVertexes *vert = nullptr;
    const Image *const image = imgRect.grid[4];
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

void MobileOpenGL2Graphics::updateScreen() restrict2
{
    BLOCK_START("Graphics::updateScreen")
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

void MobileOpenGL2Graphics::beginDraw() restrict2
{
    setOpenGLFlags();
#ifndef __native_client__
    mglDisable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
    mglHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB, GL_FASTEST);
#endif  // __native_client__
    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void MobileOpenGL2Graphics::endDraw() restrict2
{
    popClipArea();
}

void MobileOpenGL2Graphics::pushClipArea(const Rect &restrict area) restrict2
{
    Graphics::pushClipArea(area);
    const ClipRect &clipArea = mClipStack.top();

    mglScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void MobileOpenGL2Graphics::popClipArea() restrict2
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

void MobileOpenGL2Graphics::drawPoint(int x, int y) restrict2
{
    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    GLfloat vertices[] =
    {
        toGL(x + clipArea.xOffset), toGL(y + clipArea.yOffset), 0.0F, 0.0F
    };
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_POINTS, 0, 1);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::drawLine(int x1, int y1,
                                     int x2, int y2) restrict2
{
    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    GLfloat vertices[] =
    {
        toGL(x1 + clipArea.xOffset), toGL(y1 + clipArea.yOffset), 0.0F, 0.0F,
        toGL(x2 + clipArea.xOffset), toGL(y2 + clipArea.yOffset), 0.0F, 0.0F
    };
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_LINES, 0, 2);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::drawRectangle(const Rect &restrict rect) restrict2
{
    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const GLfloat x1 = static_cast<GLfloat>(rect.x + clipArea.xOffset);
    const GLfloat y1 = static_cast<GLfloat>(rect.y + clipArea.yOffset);
    const GLfloat x2 = x1 + static_cast<GLfloat>(rect.width);
    const GLfloat y2 = y1 + static_cast<GLfloat>(rect.height);
    GLfloat vertices[] =
    {
        x1, y1, 0.0F, 0.0F,
        x1, y2, 0.0F, 0.0F,
        x2, y2, 0.0F, 0.0F,
        x2, y1, 0.0F, 0.0F
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_LINE_LOOP, 0, 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::fillRectangle(const Rect &restrict rect) restrict2
{
    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const GLfloat x1 = static_cast<GLfloat>(rect.x + clipArea.xOffset);
    const GLfloat y1 = static_cast<GLfloat>(rect.y + clipArea.yOffset);
    const GLfloat x2 = x1 + static_cast<GLfloat>(rect.width);
    const GLfloat y2 = y1 + static_cast<GLfloat>(rect.height);
    GLfloat vertices[] =
    {
        x1, y1, 0.0F, 0.0F,
        x2, y1, 0.0F, 0.0F,
        x1, y2, 0.0F, 0.0F,
        x2, y2, 0.0F, 0.0F
    };

    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::enableTexturingAndBlending() restrict2
{
    if (!mTextureDraw)
    {
        mTextureDraw = true;
        mglUniform1f(mDrawTypeUniform, 1.0F);
    }
    if (!mAlpha)
    {
        mglEnable(GL_BLEND);
        mAlpha = true;
    }
}

void MobileOpenGL2Graphics::disableTexturingAndBlending() restrict2
{
    if (mTextureDraw)
    {
        mTextureDraw = false;
        mglUniform1f(mDrawTypeUniform, 0.0F);
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

void MobileOpenGL2Graphics::drawRectangle(const Rect &restrict rect A_UNUSED,
                                          const bool filled A_UNUSED) restrict2
{
}

void MobileOpenGL2Graphics::drawNet(const int x1,
                                    const int y1,
                                    const int x2,
                                    const int y2,
                                    const int width,
                                    const int height) restrict2
{
    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const GLfloat dx = static_cast<GLfloat>(clipArea.xOffset);
    const GLfloat dy = static_cast<GLfloat>(clipArea.yOffset);

    const GLfloat xs1 = x1 + dx;
    const GLfloat xs2 = x2 + dx;
    const GLfloat ys1 = y1 + dy;
    const GLfloat ys2 = y2 + dy;

    for (int y = y1; y < y2; y += height)
    {
        mFloatArray[vp + 0] = xs1;
        mFloatArray[vp + 1] = toGL(y);
        mFloatArray[vp + 2] = 0.0F;
        mFloatArray[vp + 3] = 0.0F;

        mFloatArray[vp + 4] = xs2;
        mFloatArray[vp + 5] = toGL(y);
        mFloatArray[vp + 6] = 0.0F;
        mFloatArray[vp + 7] = 0.0F;

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
        mFloatArray[vp + 2] = 0.0F;
        mFloatArray[vp + 3] = 0.0F;

        mFloatArray[vp + 4] = toGL(x);
        mFloatArray[vp + 5] = ys2;
        mFloatArray[vp + 6] = 0.0F;
        mFloatArray[vp + 7] = 0.0F;

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
                                         const Image *restrict const image)
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
                static_cast<GLfloat>(image->mTexWidth),
                static_cast<GLfloat>(image->mTexHeight));
        }
    }
}

void MobileOpenGL2Graphics::bindTexture(const GLenum target,
                                        const GLuint texture)
{
    if (mTextureBinded != texture)
    {
        // for safty not storing textures because cant update size uniform
        mTextureBinded = 0;
        mglBindTexture(target, texture);
    }
}

void MobileOpenGL2Graphics::removeArray(const uint32_t sz,
                                        uint32_t *restrict const arr) restrict2
{
    mglDeleteBuffers(sz, arr);
    for (size_t f = 0; f < sz; f ++)
    {
        if (arr[f] == mVboBinded)
            mVboBinded = 0;
    }
}

void MobileOpenGL2Graphics::bindArrayBuffer(const GLuint vbo) restrict2
{
    if (mVboBinded != vbo)
    {
        mVboBinded = vbo;
        mglBindBuffer(GL_ARRAY_BUFFER, vbo);
        mAttributesBinded = 0U;
    }
}

void MobileOpenGL2Graphics::bindArrayBufferAndAttributes(const GLuint vbo)
                                                         restrict2
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

void MobileOpenGL2Graphics::bindAttributes() restrict2
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
        if (test[0] != 0 || test[1] != 0 || test[2] != 0 || test[3] != 0)
        {
            logger->log("\n%d = %d, %d, %d, %d", f,
                test[0], test[1], test[2], test[3]);
        }
    }
}

void MobileOpenGL2Graphics::drawImageRect(const int x, const int y,
                                          const int w, const int h,
                                          const ImageRect &restrict imgRect)
                                          restrict2
{
    #include "render/graphics_drawImageRect.hpp"
}

void MobileOpenGL2Graphics::calcImageRect(ImageVertexes *restrict const vert,
                                          const int x, const int y,
                                          const int w, const int h,
                                          const ImageRect &restrict imgRect)
                                          restrict2
{
    #include "render/graphics_calcImageRect.hpp"
}

void MobileOpenGL2Graphics::clearScreen() const restrict2
{
    mglClear(GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT |
        GL_STENCIL_BUFFER_BIT);
}

void MobileOpenGL2Graphics::createGLContext(const bool custom) restrict2
{
    Graphics::createGLContext(custom);
/*
    if (mGLContext)
        SDL::makeCurrentContext(mGLContext);
    else
        mGLContext = SDL::createGLContext(mWindow, 2, 0, 0x04);
*/
}

void MobileOpenGL2Graphics::finalize(ImageCollection *restrict const col)
                                     restrict2
{
    if (col == nullptr)
        return;
    FOR_EACH (ImageCollectionIter, it, col->draws)
        finalize(*it);
}

void MobileOpenGL2Graphics::finalize(ImageVertexes *restrict const vert)
                                     restrict2
{
    // in future need convert in each switchVp/continueVp

    if (vert == nullptr)
        return;
    OpenGLGraphicsVertexes &ogl = vert->ogl;
    const STD_VECTOR<int> &vp = ogl.mVp;
    STD_VECTOR<int>::const_iterator ivp;
    const STD_VECTOR<int>::const_iterator ivp_end = vp.end();
    STD_VECTOR<GLfloat*> &floatTexPool = ogl.mFloatTexPool;
    STD_VECTOR<GLfloat*>::const_iterator ft;
    const STD_VECTOR<GLfloat*>::const_iterator ft_end = floatTexPool.end();
    STD_VECTOR<GLuint> &vbos = ogl.mVbo;
    STD_VECTOR<GLuint>::const_iterator ivbo;

    const int sz = CAST_S32(floatTexPool.size());
    if (sz == 0)
        return;
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

    for (STD_VECTOR<GLfloat*>::iterator it = floatTexPool.begin();
        it != floatTexPool.end(); ++ it)
    {
        delete [] (*it);
    }
    floatTexPool.clear();
}

void MobileOpenGL2Graphics::drawTriangleArray(const int size) restrict2
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        mFloatArray, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLES, 0, size / 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::drawTriangleArray(const GLfloat *restrict const
                                              array,
                                              const int size) restrict2
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        array, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLES, 0, size / 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void MobileOpenGL2Graphics::drawLineArrays(const int size) restrict2
{
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat),
        mFloatArray, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_LINES, 0, size / 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

#ifdef DEBUG_BIND_TEXTURE
void MobileOpenGL2Graphics::debugBindTexture(const Image *restrict const image)
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
void MobileOpenGL2Graphics::debugBindTexture(const Image *restrict const
                                             image A_UNUSED) restrict2
{
}
#endif  // DEBUG_BIND_TEXTURE

#endif  // USE_OPENGL
