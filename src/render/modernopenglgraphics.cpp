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

GLuint ModernOpenGLGraphics::mLastImage = 0;
#ifdef DEBUG_DRAW_CALLS
unsigned int ModernOpenGLGraphics::mDrawCalls = 0;
unsigned int ModernOpenGLGraphics::mLastDrawCalls = 0;
#endif

ModernOpenGLGraphics::ModernOpenGLGraphics() :
    mFloatTexArray(nullptr),
    mShortVertArray(nullptr),
    mFloatTexArrayCached(nullptr),
    mShortVertArrayCached(nullptr),
    mSimpleProgram(nullptr),
    mTextureProgram(nullptr),
    mAlphaCached(1.0F),
    mVpCached(0),
    mTexture(false),
    mIsByteColor(false),
    mByteColor(),
    mImageCached(0),
    mFloatColor(1.0F),
    mMaxVertices(500),
    mSimpleProgramId(0U),
    mTextureProgramId(0U),
    mSimpleColorUniform(0U),
    mSimplePosAttrib(0),
    mTexturePosAttrib(0),
    mTexAttrib(0),
    mSimpleScreenUniform(0U),
    mTextureScreenUniform(0U),
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
    if (mSimpleProgram)
        mSimpleProgram->decRef();
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
    if (!mFloatTexArray)
        mFloatTexArray = new GLfloat[sz];
    if (!mShortVertArray)
        mShortVertArray = new GLshort[sz];
    if (!mFloatTexArrayCached)
        mFloatTexArrayCached = new GLfloat[sz];
    if (!mShortVertArrayCached)
        mShortVertArrayCached = new GLshort[sz];
}

void ModernOpenGLGraphics::postInit()
{
    logger->log("Compiling shaders");
    mSimpleProgram = shaders.getSimpleProgram();
    mSimpleProgramId = mSimpleProgram->getProgramId();
    mTextureProgram = shaders.getTextureProgram();
    mTextureProgramId = mTextureProgram->getProgramId();
    if (mSimpleProgram && mTextureProgram)
    {
        logger->log("Shaders compilation done.");
        mglUseProgram(mSimpleProgramId);
        mSimplePosAttrib = mglGetAttribLocation(mSimpleProgramId, "position");
        mglEnableVertexAttribArray(mSimplePosAttrib);
        mSimpleColorUniform = mglGetUniformLocation(mSimpleProgramId, "color");
        mSimpleScreenUniform = mglGetUniformLocation(mSimpleProgramId, "screen");

        mTexturePosAttrib = mglGetAttribLocation(mTextureProgramId, "position");
        mTexAttrib = mglGetAttribLocation(mTextureProgramId, "texcoord");
        mTextureScreenUniform = mglGetUniformLocation(mTextureProgramId, "screen");

        screenResized();
    }
    else
    {
        logger->error("Shaders compilation error.");
    }
}

void ModernOpenGLGraphics::screenResized()
{
    mglUniform2f(mSimpleScreenUniform,
        static_cast<float>(mWidth) / 2.0f,
        static_cast<float>(mHeight) / 2.0f);
    mglUniform2f(mTextureScreenUniform,
        static_cast<float>(mWidth) / 2.0f,
        static_cast<float>(mHeight) / 2.0f);
}

void ModernOpenGLGraphics::deleteArrays()
{
    deleteArraysInternal();
}

void ModernOpenGLGraphics::deleteArraysInternal()
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
    mColor = color;
    mColor2 = color2;
    mColorAlpha = (color.a != 255);
    if (mColorAlpha)
    {
        mglUniform4f(mSimpleColorUniform,
            static_cast<float>(color.r) / 255.0F,
            static_cast<float>(color.g) / 255.0F,
            static_cast<float>(color.b) / 255.0F,
            static_cast<float>(color.a) / 255.0F);
    }
    else
    {
        mglUniform4f(mSimpleColorUniform,
            static_cast<float>(color.r) / 255.0F,
            static_cast<float>(color.g) / 255.0F,
            static_cast<float>(color.b) / 255.0F,
            1.0F);
    }
}

void ModernOpenGLGraphics::setColorAlpha(const float alpha)
{
    // here need set alpha uniform for textures only
/*
    if (!mIsByteColor && mFloatColor == alpha)
        return;

    glColor4f(1.0F, 1.0F, 1.0F, alpha);
    mIsByteColor = false;
    mFloatColor = alpha;
*/
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

    GLuint vbo;
    mglGenBuffers(1, &vbo);
    mglBindBuffer(GL_ARRAY_BUFFER, vbo);
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);

//    mglEnableVertexAttribArray(mTexturePosAttrib);
    mglVertexAttribPointer(mTexturePosAttrib, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(GLfloat), 0);
//    mglEnableVertexAttribArray(mTexAttrib);
    mglVertexAttribPointer(mTexAttrib, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

#ifdef DEBUG_DRAW_CALLS
    MobileOpenGLGraphics::mDrawCalls ++;
#endif
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static inline void drawRescaledQuad(const Image *const image,
                                    const int srcX, const int srcY,
                                    const int dstX, const int dstY,
                                    const int width, const int height,
                                    const int desiredWidth,
                                    const int desiredHeight)
{
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

    setColorAlpha(image->mAlpha);
#ifdef DEBUG_BIND_TEXTURE
    debugBindTexture(image);
#endif
    bindTexture(GL_TEXTURE_2D, image->mGLImage);
    setTexturingAndBlending(true);

    const SDL_Rect &imageRect = image->mBounds;
    drawQuad(image, imageRect.x, imageRect.y,
        dstX, dstY, imageRect.w, imageRect.h);
    return true;
}

void ModernOpenGLGraphics::drawImageCached(const Image *const image,
                                           int x, int y)
{
}

void ModernOpenGLGraphics::drawPatternCached(const Image *const image,
                                             const int x, const int y,
                                             const int w, const int h)
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
}

void ModernOpenGLGraphics::drawRescaledPattern(const Image *const image,
                                               const int x, const int y,
                                               const int w, const int h,
                                               const int scaledWidth,
                                               const int scaledHeight)
{
}

inline void ModernOpenGLGraphics::drawVertexes(const
                                               NormalOpenGLGraphicsVertexes
                                               &ogl)
{
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
}

void ModernOpenGLGraphics::drawTileVertexes(const ImageVertexes *const vert)
{
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

/*
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
*/
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
/*
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
*/
}

void ModernOpenGLGraphics::drawPoint(int x, int y)
{
}

void ModernOpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
{
}

void ModernOpenGLGraphics::drawRectangle(const Rect& rect)
{
    setTexturingAndBlending(false);
    const ClipRect &clipArea = mClipStack.top();
    const int x1 = rect.x + clipArea.xOffset;
    const int y1 = rect.y + clipArea.yOffset;
    const int x2 = x1 + rect.width;
    const int y2 = y1 + rect.height;
    GLfloat vertices[] =
    {
        x1, y1,
        x1, y2,
        x2, y2,
        x2, y1
    };
    GLuint vbo;
    mglGenBuffers(1, &vbo);
    mglBindBuffer(GL_ARRAY_BUFFER, vbo);
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);
//    mglEnableVertexAttribArray(mSimplePosAttrib);
    mglVertexAttribPointer(mSimplePosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_LOOP, 0, 4);

    mglDeleteBuffers(1, &vbo);
}

void ModernOpenGLGraphics::fillRectangle(const Rect& rect)
{
    setTexturingAndBlending(false);
    const ClipRect &clipArea = mClipStack.top();
    const int x1 = rect.x + clipArea.xOffset;
    const int y1 = rect.y + clipArea.yOffset;
    const int x2 = x1 + rect.width;
    const int y2 = y1 + rect.height;
    GLfloat vertices[] =
    {
        x1, y1,
        x2, y1,
        x1, y2,
        x2, y2
    };
    GLuint vbo;
    mglGenBuffers(1, &vbo);
    mglBindBuffer(GL_ARRAY_BUFFER, vbo);
    mglBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
        vertices, GL_DYNAMIC_DRAW);
//    mglEnableVertexAttribArray(mSimplePosAttrib);
    mglVertexAttribPointer(mSimplePosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    mglDeleteBuffers(1, &vbo);
}

void ModernOpenGLGraphics::setTexturingAndBlending(const bool enable)
{
    if (enable)
    {
        if (!mTextureDraw)
        {
            mTextureDraw = true;
            mglDisableVertexAttribArray(mSimplePosAttrib);
            mglUseProgram(mTextureProgramId);
            mglEnableVertexAttribArray(mTexturePosAttrib);
            mglEnableVertexAttribArray(mTexAttrib);
            mglUniform2f(mTextureScreenUniform,
                static_cast<float>(mWidth) / 2.0f,
                static_cast<float>(mHeight) / 2.0f);
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
            mglDisableVertexAttribArray(mTexturePosAttrib);
            mglDisableVertexAttribArray(mTexAttrib);
            mglUseProgram(mSimpleProgramId);
            mglEnableVertexAttribArray(mSimplePosAttrib);
            mglUniform2f(mSimpleScreenUniform,
                static_cast<float>(mWidth) / 2.0f,
                static_cast<float>(mHeight) / 2.0f);
            mglUniform4f(mSimpleColorUniform,
                static_cast<float>(mColor.r) / 255.0F,
                static_cast<float>(mColor.g) / 255.0F,
                static_cast<float>(mColor.b) / 255.0F,
                static_cast<float>(mColor.a) / 255.0F);
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
