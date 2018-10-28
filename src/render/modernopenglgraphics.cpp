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

#include "render/modernopenglgraphics.h"

#include "graphicsmanager.h"

#include "render/opengl/mgl.h"
#ifdef __native_client__
#include "render/opengl/naclglfunctions.h"
#endif  // __native_client__

#include "render/shaders/shaderprogram.h"
#include "render/shaders/shadersmanager.h"

#include "render/vertexes/imagecollection.h"

#include "resources/imagerect.h"
#include "resources/openglimagehelper.h"

#include "resources/image/image.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
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

GLuint ModernOpenGLGraphics::mTextureBinded = 0;
#ifdef DEBUG_DRAW_CALLS
unsigned int ModernOpenGLGraphics::mDrawCalls = 0;
unsigned int ModernOpenGLGraphics::mLastDrawCalls = 0;
#endif  // DEBUG_DRAW_CALLS

ModernOpenGLGraphics::ModernOpenGLGraphics() :
    mIntArray(nullptr),
    mIntArrayCached(nullptr),
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
    mEbo(0U),
    mVboBinded(0U),
    mEboBinded(0U),
    mAttributesBinded(0U),
    mColorAlpha(false),
    mTextureDraw(false),
#ifdef DEBUG_BIND_TEXTURE
    mOldTexture(),
    mOldTextureId(0),
#endif  // DEBUG_BIND_TEXTURE
    mFbo()
{
    mOpenGL = RENDER_MODERN_OPENGL;
    mName = "modern OpenGL";
}

ModernOpenGLGraphics::~ModernOpenGLGraphics()
{
    deleteArraysInternal();
    deleteGLObjects();
}

void ModernOpenGLGraphics::deleteGLObjects() restrict2
{
    delete2(mProgram);
    if (mVbo != 0U)
    {
//        logger->log("delete buffer vbo: %u", mVbo);
        mglDeleteBuffers(1, &mVbo);
    }
    if (mEbo != 0U)
    {
//        logger->log("delete buffer ebo: %u", mEbo);
        mglDeleteBuffers(1, &mEbo);
    }
    if (mVao != 0U)
        mglDeleteVertexArrays(1, &mVao);
}

void ModernOpenGLGraphics::initArrays(const int vertCount) restrict2
{
    mMaxVertices = vertCount;
    if (mMaxVertices < 500)
        mMaxVertices = 500;
    else if (mMaxVertices > 1024)
        mMaxVertices = 1024;

    // need alocate small size, after if limit reached reallocate to double size
    const size_t sz = mMaxVertices * 4 + 30;
    vertexBufSize = mMaxVertices;
    if (mIntArray == nullptr)
        mIntArray = new GLint[sz];
    if (mIntArrayCached == nullptr)
        mIntArrayCached = new GLint[sz];
}

void ModernOpenGLGraphics::postInit() restrict2
{
    mglGenVertexArrays(1, &mVao);
    mglBindVertexArray(mVao);
    mglGenBuffers(1, &mVbo);
//    logger->log("gen vbo buffer: %u", mVbo);
    bindArrayBuffer(mVbo);
    mglGenBuffers(1, &mEbo);
//    logger->log("gen ebo buffer: %u", mEbo);
    bindElementBuffer(mEbo);

    logger->log("Compiling shaders");
    mProgram = shaders.getSimpleProgram();
    if (mProgram == nullptr)
    {
        graphicsManager.logError();
        logger->safeError("Shader creation error. See manaplus.log.");
    }
    mProgramId = mProgram->getProgramId();
    if (mProgramId == 0U)
        logger->error("Shaders compilation error.");

    logger->log("Shaders compilation done.");
    mglUseProgram(mProgramId);

    mPosAttrib = mglGetAttribLocation(mProgramId, "position");
    mglEnableVertexAttribArray(mPosAttrib);
    mglVertexAttribIFormat(mPosAttrib, 4, GL_INT, 0);

    mSimpleColorUniform = mglGetUniformLocation(mProgramId, "color");
    mScreenUniform = mglGetUniformLocation(mProgramId, "screen");
    mDrawTypeUniform = mglGetUniformLocation(mProgramId, "drawType");
    mTextureColorUniform = mglGetUniformLocation(mProgramId, "alpha");

    mglUniform1f(mTextureColorUniform, 1.0F);

    mglBindVertexBuffer(0, mVbo, 0, 4 * sizeof(GLint));
    mglVertexAttribBinding(mPosAttrib, 0);
//    mglVertexAttribIPointer(mPosAttrib, 4, GL_INT, 4 * sizeof(GLint), 0);
    mAttributesBinded = mVbo;

    mglUniform2f(mScreenUniform,
        static_cast<float>(mWidth) / 2.0F,
        static_cast<float>(mHeight) / 2.0F);
    mglUniform4f(mSimpleColorUniform,
        0.0F,
        0.0F,
        0.0F,
        0.0F);
}

void ModernOpenGLGraphics::screenResized() restrict2
{
    deleteGLObjects();
    mVboBinded = 0U;
    mEboBinded = 0U;
    mAttributesBinded = 0U;
    mColor = Color(0, 0, 0, 0);
    postInit();
}

void ModernOpenGLGraphics::deleteArrays() restrict2
{
    deleteArraysInternal();
}

void ModernOpenGLGraphics::deleteArraysInternal() restrict2
{
    delete [] mIntArray;
    mIntArray = nullptr;
    delete [] mIntArrayCached;
    mIntArrayCached = nullptr;
}

bool ModernOpenGLGraphics::setVideoMode(const int w, const int h,
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

void ModernOpenGLGraphics::setColor(const Color &restrict color) restrict2
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

void ModernOpenGLGraphics::setColorAlpha(const float alpha) restrict2
{
    if (mAlphaCached != alpha)
    {
        mAlphaCached = alpha;
        mglUniform1f(mTextureColorUniform, alpha);
    }
}

void ModernOpenGLGraphics::drawQuad(const int srcX,
                                    const int srcY,
                                    const int dstX,
                                    const int dstY,
                                    const int width,
                                    const int height) restrict2
{
    const int texX2 = srcX + width;
    const int texY2 = srcY + height;
    const int x2 = dstX + width;
    const int y2 = dstY + height;

    GLint vertices[] =
    {
        dstX, dstY, srcX, srcY,
        x2, dstY, texX2, srcY,
        dstX, y2, srcX, texY2,
        x2, y2, texX2, texY2
    };

//    logger->log("allocate: %d, %ld", mVboBinded, sizeof(vertices));
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

void ModernOpenGLGraphics::drawRescaledQuad(const int srcX, const int srcY,
                                            const int dstX, const int dstY,
                                            const int width, const int height,
                                            const int desiredWidth,
                                            const int desiredHeight) restrict2
{
    const int texX2 = srcX + width;
    const int texY2 = srcY + height;
    const int x2 = dstX + desiredWidth;
    const int y2 = dstY + desiredHeight;

    GLint vertices[] =
    {
        dstX, dstY, srcX, srcY,
        x2, dstY, texX2, srcY,
        dstX, y2, srcX, texY2,
        x2, y2, texX2, texY2
    };

//    logger->log("allocate: %d, %ld", mVboBinded, sizeof(vertices));
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

void ModernOpenGLGraphics::drawImage(const Image *restrict const image,
                                     int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void ModernOpenGLGraphics::drawImageInline(const Image *restrict const image,
                                           int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (image == nullptr)
        return;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE
    bindTexture(GL_TEXTURE_2D, image->mGLImage);
    enableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    const ClipRect &clipArea = mClipStack.top();
    const SDL_Rect &imageRect = image->mBounds;
    drawQuad(imageRect.x, imageRect.y,
        dstX + clipArea.xOffset, dstY + clipArea.yOffset,
        imageRect.w, imageRect.h);
}

void ModernOpenGLGraphics::copyImage(const Image *restrict const image,
                                     int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void ModernOpenGLGraphics::testDraw() restrict2
{
/*
    GLint vertices[] =
    {
        0, 0, 0, 0,
        800, 0, 800, 0,
        0, 600, 0, 600,
        800, 600, 800, 600
    };
*/
//    logger->log("allocate: %d, %ld", mVboBinded, sizeof(vertices));
//    logger->log("allocate ebo: %d, %ld", mEboBinded, sizeof(elements));
//    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
//        vertices, GL_STREAM_DRAW);
//    mglBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements),
//        elements, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void ModernOpenGLGraphics::drawImageCached(const Image *restrict const image
                                           A_UNUSED,
                                           int A_UNUSED x,
                                           int y A_UNUSED) restrict2
{
}

void ModernOpenGLGraphics::drawPatternCached(const Image *restrict const image
                                             A_UNUSED,
                                             const int x A_UNUSED,
                                             const int y A_UNUSED,
                                             const int w A_UNUSED,
                                             const int h A_UNUSED) restrict2
{
}

void ModernOpenGLGraphics::completeCache() restrict2
{
}

void ModernOpenGLGraphics::drawRescaledImage(const Image *restrict const image,
                                             int dstX, int dstY,
                                             const int desiredWidth,
                                             const int desiredHeight) restrict2
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
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);

    const ClipRect &clipArea = mClipStack.top();
    // Draw a textured quad.
    drawRescaledQuad(imageRect.x, imageRect.y,
        dstX + clipArea.xOffset, dstY + clipArea.yOffset,
        imageRect.w, imageRect.h,
        desiredWidth, desiredHeight);
}

void ModernOpenGLGraphics::drawPattern(const Image *restrict const image,
                                       const int x, const int y,
                                       const int w, const int h) restrict2
{
    drawPatternInline(image, x, y, w, h);
}

void ModernOpenGLGraphics::drawPatternInline(const Image *restrict const image,
                                             const int x,
                                             const int y,
                                             const int w,
                                             const int h) restrict2
{
    if (image == nullptr)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int iw = imageRect.w;
    const int ih = imageRect.h;

    if (iw == 0 || ih == 0)
        return;

    const ClipRect &clipArea = mClipStack.top();
    const int x2 = x + clipArea.xOffset;
    const int y2 = y + clipArea.yOffset;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

    enableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    setColorAlpha(image->mAlpha);

    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const int texY2 = srcY + height;
        const int dstY = y2 + py;
        for (int px = 0; px < w; px += iw)
        {
            const int width = (px + iw >= w) ? w - px : iw;
            const int dstX = x2 + px;

            const int texX2 = srcX + width;

            vertFill2D(mIntArray,
                srcX, srcY, texX2, texY2,
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

void ModernOpenGLGraphics::drawRescaledPattern(const Image *
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
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif  // DEBUG_BIND_TEXTURE

    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);

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
        const int dstY = y2 + py;
        const int scaledY = srcY + height / scaleFactorH;
        for (int px = 0; px < w; px += scaledWidth)
        {
            const int width = (px + scaledWidth >= w)
                ? w - px : scaledWidth;
            const int dstX = x2 + px;
            const int scaledX = srcX + width / scaleFactorW;

            vertFill2D(mIntArray,
                srcX, srcY, scaledX, scaledY,
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
                                               OpenGLGraphicsVertexes &
                                               restrict ogl) restrict2
{
    const STD_VECTOR<int> &vp = ogl.mVp;
    const STD_VECTOR<GLuint> &vbos = ogl.mVbo;
    STD_VECTOR<int>::const_iterator ivp;
    STD_VECTOR<GLuint>::const_iterator ivbo;
    const STD_VECTOR<int>::const_iterator ivp_end = vp.end();

/*
    if (vp.size() != vbos.size())
        logger->log("different size in vp and vbos");
*/

    for (ivp = vp.begin(), ivbo = vbos.begin();
         ivp != ivp_end;
         ++ ivp, ++ ivbo)
    {
        bindArrayBufferAndAttributes(*ivbo);
#ifdef DEBUG_DRAW_CALLS
        mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
//        logger->log("draw from array: %u", *ivbo);
        mglDrawArrays(GL_TRIANGLES, 0, *ivp / 4);
#ifdef OPENGLERRORS
        graphicsManager.logError();
#endif  // OPENGLERRORS
    }
}

void ModernOpenGLGraphics::calcPattern(ImageVertexes *restrict const vert,
                                       const Image *restrict const image,
                                       const int x,
                                       const int y,
                                       const int w,
                                       const int h) const restrict2
{
    calcPatternInline(vert, image, x, y, w, h);
}

void ModernOpenGLGraphics::calcPatternInline(ImageVertexes *
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
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;
    const int iw = imageRect.w;
    const int ih = imageRect.h;

    if (iw == 0 || ih == 0)
        return;

    const ClipRect &clipArea = mClipStack.top();
    const int x2 = x + clipArea.xOffset;
    const int y2 = y + clipArea.yOffset;

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;
    unsigned int vp = ogl.continueVp();

    GLint *intArray = ogl.continueIntTexArray();

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const int dstY = y2 + py;
        const int texY2 = srcY + height;
        for (int px = 0; px < w; px += iw)
        {
            const int width = (px + iw >= w) ? w - px : iw;
            const int dstX = x2 + px;
            const int texX2 = srcX + width;

            vertFill2D(intArray,
                srcX, srcY, texX2, texY2,
                dstX, dstY, width, height);

            vp += 24;
            if (vp >= vLimit)
            {
                intArray = ogl.switchIntTexArray();
                ogl.switchVp(vp);
                vp = 0;
            }
        }
    }
    ogl.switchVp(vp);
}

void ModernOpenGLGraphics::calcTileCollection(ImageCollection *
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

void ModernOpenGLGraphics::drawTileCollection(const ImageCollection
                                              *restrict const vertCol)
                                              restrict2
{
    enableTexturingAndBlending();
/*
    if (!vertCol)
    {
        logger->log("ModernOpenGLGraphics::drawTileCollection"
            " vertCol is nullptr");
    }
*/
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
        drawVertexes(vert->ogl);
    }
}

void ModernOpenGLGraphics::calcPattern(ImageCollection *restrict const vertCol,
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

void ModernOpenGLGraphics::calcTileVertexes(ImageVertexes *restrict const vert,
                                            const Image *restrict const image,
                                            int dstX, int dstY) const restrict2
{
    calcTileVertexesInline(vert, image, dstX, dstY);
}

void ModernOpenGLGraphics::calcTileVertexesInline(ImageVertexes *
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

    const ClipRect &clipArea = mClipStack.top();
    const int x2 = dstX + clipArea.xOffset;
    const int y2 = dstY + clipArea.yOffset;

    const unsigned int vLimit = mMaxVertices * 4;

    OpenGLGraphicsVertexes &ogl = vert->ogl;

    unsigned int vp = ogl.continueVp();

    int texX2 = srcX + w;
    int texY2 = srcY + h;

    GLint *const intArray = ogl.continueIntTexArray();

    vertFill2D(intArray,
        srcX, srcY, texX2, texY2,
        x2, y2, w, h);

    vp += 24;
    if (vp >= vLimit)
    {
        ogl.switchIntTexArray();
        ogl.switchVp(vp);
        vp = 0;
    }

    ogl.switchVp(vp);
}

void ModernOpenGLGraphics::drawTileVertexes(const ImageVertexes *
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
    bindArrayBufferAndAttributes(mVbo);

    drawVertexes(vert->ogl);
}

void ModernOpenGLGraphics::calcWindow(ImageCollection *restrict const vertCol,
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

void ModernOpenGLGraphics::updateScreen() restrict2
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

void ModernOpenGLGraphics::beginDraw() restrict2
{
    setOpenGLFlags();
    mglDisable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
    mglHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB, GL_FASTEST);
    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void ModernOpenGLGraphics::endDraw() restrict2
{
    popClipArea();
}

void ModernOpenGLGraphics::pushClipArea(const Rect &restrict area) restrict2
{
    Graphics::pushClipArea(area);
    const ClipRect &clipArea = mClipStack.top();

    mglScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void ModernOpenGLGraphics::popClipArea() restrict2
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

void ModernOpenGLGraphics::drawPoint(int x, int y) restrict2
{
    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    GLint vertices[] =
    {
        x + clipArea.xOffset, y + clipArea.yOffset, 0, 0
    };
//    logger->log("allocate: %d, %ld", mVboBinded, sizeof(vertices));
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

void ModernOpenGLGraphics::drawLine(int x1, int y1,
                                    int x2, int y2) restrict2
{
    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    GLint vertices[] =
    {
        x1 + clipArea.xOffset, y1 + clipArea.yOffset, 0, 0,
        x2 + clipArea.xOffset, y2 + clipArea.yOffset, 0, 0
    };
//    logger->log("allocate: %d, %ld", mVboBinded, sizeof(vertices));
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

void ModernOpenGLGraphics::drawRectangle(const Rect &restrict rect) restrict2
{
    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const int x1 = rect.x + clipArea.xOffset;
    const int y1 = rect.y + clipArea.yOffset;
    const int x2 = x1 + rect.width;
    const int y2 = y1 + rect.height;
    GLint vertices[] =
    {
        x1, y1, 0, 0,
        x1, y2, 0, 0,
        x2, y2, 0, 0,
        x2, y1, 0, 0
    };

//    logger->log("allocate: %d, %ld", mVboBinded, sizeof(vertices));
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

void ModernOpenGLGraphics::fillRectangle(const Rect &restrict rect) restrict2
{
    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const int x1 = rect.x + clipArea.xOffset;
    const int y1 = rect.y + clipArea.yOffset;
    const int x2 = x1 + rect.width;
    const int y2 = y1 + rect.height;
    GLint vertices[] =
    {
        x1, y1, 0, 0,
        x2, y1, 0, 0,
        x1, y2, 0, 0,
        x2, y2, 0, 0
    };

//    logger->log("allocate: %d, %ld", mVboBinded, sizeof(vertices));
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

void ModernOpenGLGraphics::enableTexturingAndBlending() restrict2
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

void ModernOpenGLGraphics::disableTexturingAndBlending() restrict2
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

void ModernOpenGLGraphics::drawRectangle(const Rect &restrict rect A_UNUSED,
                                         const bool filled A_UNUSED) restrict2
{
}

void ModernOpenGLGraphics::drawNet(const int x1, const int y1,
                                   const int x2, const int y2,
                                   const int width, const int height) restrict2
{
    unsigned int vp = 0;
    const unsigned int vLimit = mMaxVertices * 4;

    disableTexturingAndBlending();
    bindArrayBufferAndAttributes(mVbo);
    const ClipRect &clipArea = mClipStack.top();
    const GLint dx = clipArea.xOffset;
    const GLint dy = clipArea.yOffset;

    const GLint xs1 = x1 + dx;
    const GLint xs2 = x2 + dx;
    const GLint ys1 = y1 + dy;
    const GLint ys2 = y2 + dy;

    for (int y = y1; y < y2; y += height)
    {
        mIntArray[vp + 0] = xs1;
        mIntArray[vp + 1] = y;
        mIntArray[vp + 2] = 0;
        mIntArray[vp + 3] = 0;

        mIntArray[vp + 4] = xs2;
        mIntArray[vp + 5] = y;
        mIntArray[vp + 6] = 0;
        mIntArray[vp + 7] = 0;

        vp += 8;
        if (vp >= vLimit)
        {
            drawLineArrays(vp);
            vp = 0;
        }
    }

    for (int x = x1; x < x2; x += width)
    {
        mIntArray[vp + 0] = x;
        mIntArray[vp + 1] = ys1;
        mIntArray[vp + 2] = 0.0F;
        mIntArray[vp + 3] = 0.0F;

        mIntArray[vp + 4] = x;
        mIntArray[vp + 5] = ys2;
        mIntArray[vp + 6] = 0.0F;
        mIntArray[vp + 7] = 0.0F;

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

void ModernOpenGLGraphics::bindTexture(const GLenum target,
                                       const GLuint texture)
{
    if (mTextureBinded != texture)
    {
        mTextureBinded = texture;
        mglBindTexture(target, texture);
    }
}

void ModernOpenGLGraphics::removeArray(const uint32_t sz,
                                       uint32_t *restrict const arr) restrict2
{
    mglDeleteBuffers(sz, arr);
    for (size_t f = 0; f < sz; f ++)
    {
        if (arr[f] == mVboBinded)
            mVboBinded = 0;
//        logger->log("delete buffers: %u", arr[f]);
    }
}

void ModernOpenGLGraphics::bindArrayBuffer(const GLuint vbo) restrict2
{
    if (mVboBinded != vbo)
    {
        mVboBinded = vbo;
//        logger->log("bind array: %u", vbo);
        mglBindBuffer(GL_ARRAY_BUFFER, vbo);
/*
        if (mglIsBuffer(vbo) != GL_TRUE)
            logger->log("bind wrong buffer: %u", vbo);
*/
        mAttributesBinded = 0U;
    }
}

void ModernOpenGLGraphics::bindElementBuffer(const GLuint ebo) restrict2
{
    if (mEboBinded != ebo)
    {
        mEboBinded = ebo;
        logger->log("bind element: %u", ebo);
        mglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
/*
        if (mglIsBuffer(ebo) != GL_TRUE)
            logger->log("bind wrong buffer: %u", vbo);
*/
    }
}

void ModernOpenGLGraphics::bindArrayBufferAndAttributes(const GLuint vbo)
                                                        restrict2
{
    if (mVboBinded != vbo)
    {
        mVboBinded = vbo;
//        logger->log("bind array: %u", vbo);
        mglBindBuffer(GL_ARRAY_BUFFER, vbo);
/*
        if (mglIsBuffer(vbo) != GL_TRUE)
            logger->log("bind wrong buffer: %u", vbo);
*/

        mAttributesBinded = mVboBinded;
//        logger->log("bind vertex buffer: %u", mVboBinded);
        mglBindVertexBuffer(0, mVboBinded, 0, 4 * sizeof(GLint));
//        mglVertexAttribIPointer(mPosAttrib, 4, GL_INT, 4 * sizeof(GLint), 0);
//        mglVertexAttribBinding(mPosAttrib, 0);
    }
    else if (mAttributesBinded != mVboBinded)
    {
        mAttributesBinded = mVboBinded;
//        logger->log("bind vertex buffer: %u", mVboBinded);
        mglBindVertexBuffer(0, mVboBinded, 0, 4 * sizeof(GLint));
//        mglVertexAttribIPointer(mPosAttrib, 4, GL_INT, 4 * sizeof(GLint), 0);
//        mglVertexAttribBinding(mPosAttrib, 0);
    }
}

void ModernOpenGLGraphics::bindAttributes() restrict2
{
    if (mAttributesBinded != mVboBinded)
    {
        mAttributesBinded = mVboBinded;
        mglBindVertexBuffer(0, mVboBinded, 0, 4 * sizeof(GLint));
//        mglVertexAttribIPointer(mPosAttrib, 4, GL_INT, 4 * sizeof(GLint), 0);
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
        mglGetIntegerv(f, &test[0]);
        if (test[0] != 0 || test[1] != 0 || test[2] != 0 || test[3] != 0)
        {
            logger->log("\n%d = %d, %d, %d, %d", f,
                test[0], test[1], test[2], test[3]);
        }
    }
}

void ModernOpenGLGraphics::drawImageRect(const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &restrict imgRect)
                                         restrict2
{
    #include "render/graphics_drawImageRect.hpp"
}

void ModernOpenGLGraphics::calcImageRect(ImageVertexes *restrict const vert,
                                         const int x, const int y,
                                         const int w, const int h,
                                         const ImageRect &restrict imgRect)
                                         restrict2
{
    #include "render/graphics_calcImageRect.hpp"
}

void ModernOpenGLGraphics::clearScreen() const restrict2
{
    mglClear(GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT |
        GL_STENCIL_BUFFER_BIT);
}

void ModernOpenGLGraphics::createGLContext(const bool custom) restrict2
{
    if (custom)
    {
        if (mGLContext != nullptr)
            SDL::makeCurrentContext(mGLContext);
        else
            mGLContext = SDL::createGLContext(mWindow, 3, 3, 0x01);
    }
    else
    {
        Graphics::createGLContext(false);
    }
}

void ModernOpenGLGraphics::finalize(ImageCollection *restrict const col)
                                    restrict2
{
    if (col == nullptr)
        return;
    FOR_EACH (ImageCollectionIter, it, col->draws)
        finalize(*it);
}

void ModernOpenGLGraphics::finalize(ImageVertexes *restrict const vert)
                                    restrict2
{
    // in future need convert in each switchVp/continueVp

    if (vert == nullptr)
        return;
    OpenGLGraphicsVertexes &ogl = vert->ogl;
    const STD_VECTOR<int> &vp = ogl.mVp;
    STD_VECTOR<int>::const_iterator ivp;
    const STD_VECTOR<int>::const_iterator ivp_end = vp.end();
    STD_VECTOR<GLint*> &intTexPool = ogl.mIntTexPool;
    STD_VECTOR<GLint*>::const_iterator ft;
    const STD_VECTOR<GLint*>::const_iterator ft_end = intTexPool.end();
    STD_VECTOR<GLuint> &vbos = ogl.mVbo;
    STD_VECTOR<GLuint>::const_iterator ivbo;

    const int sz = CAST_S32(intTexPool.size());
    if (sz == 0)
        return;
    vbos.resize(sz);
    mglGenBuffers(sz, &vbos[0]);
/*
    for (int f = 0; f < sz; f ++)
        logger->log("gen buffers: %u", vbos[f]);
*/

    for (ft = intTexPool.begin(), ivp = vp.begin(), ivbo = vbos.begin();
         ft != ft_end && ivp != ivp_end;
         ++ ft, ++ ivp, ++ ivbo)
    {
        bindArrayBuffer(*ivbo);
/*
        logger->log("allocate: %d, %ld", mVboBinded,
            (*ivp) * sizeof(GLint));
*/
        mglBufferData(GL_ARRAY_BUFFER, (*ivp) * sizeof(GLint),
            *ft, GL_STATIC_DRAW);
    }

    for (STD_VECTOR<GLint*>::iterator it = intTexPool.begin();
        it != intTexPool.end(); ++ it)
    {
        delete [] (*it);
    }
    intTexPool.clear();
}

void ModernOpenGLGraphics::drawTriangleArray(const int size) restrict2
{
//    logger->log("allocate: %d, %ld", mVboBinded, size * sizeof(GLint));
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLint),
        mIntArray, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLES, 0, size / 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void ModernOpenGLGraphics::drawTriangleArray(const GLint *restrict const array,
                                             const int size) restrict2
{
//    logger->log("allocate: %d, %ld", mVboBinded, size * sizeof(GLint));
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLint),
        array, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_TRIANGLES, 0, size / 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

void ModernOpenGLGraphics::drawLineArrays(const int size)
{
//    logger->log("allocate: %d, %ld", mVboBinded, size * sizeof(GLint));
    mglBufferData(GL_ARRAY_BUFFER, size * sizeof(GLint),
        mIntArray, GL_STREAM_DRAW);
#ifdef DEBUG_DRAW_CALLS
    mDrawCalls ++;
#endif  // DEBUG_DRAW_CALLS
    mglDrawArrays(GL_LINES, 0, size / 4);
#ifdef OPENGLERRORS
    graphicsManager.logError();
#endif  // OPENGLERRORS
}

#ifdef DEBUG_BIND_TEXTURE
void ModernOpenGLGraphics::debugBindTexture(const Image *restrict const image)
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
void ModernOpenGLGraphics::debugBindTexture(const Image *restrict const
                                            image A_UNUSED) restrict2
{
}
#endif  // DEBUG_BIND_TEXTURE

#endif  // defined(USE_OPENGL) && !defined(ANDROID) &&
        // !defined(__native_client__)
