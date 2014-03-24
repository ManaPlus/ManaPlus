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
#include "render/safeopenglgraphics.h"

#include "configuration.h"
#include "graphicsmanager.h"

#include "resources/image.h"
#include "resources/openglimagehelper.h"

#include "utils/sdlcheckutils.h"

#include "debug.h"

GLuint SafeOpenGLGraphics::mLastImage = 0;

SafeOpenGLGraphics::SafeOpenGLGraphics():
    mTexture(false),
    mIsByteColor(false),
    mByteColor(),
    mFloatColor(1.0F),
    mColorAlpha(false),
    mFbo()
{
    mOpenGL = RENDER_SAFE_OPENGL;
    mName = "safe OpenGL";
}

SafeOpenGLGraphics::~SafeOpenGLGraphics()
{
}

bool SafeOpenGLGraphics::setVideoMode(const int w, const int h,
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

static inline void drawQuad(const Image *image,
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

        glTexCoord2f(texX1, texY1);
        glVertex2i(dstX, dstY);
        glTexCoord2f(texX2, texY1);
        glVertex2i(dstX + width, dstY);
        glTexCoord2f(texX2, texY2);
        glVertex2i(dstX + width, dstY + height);
        glTexCoord2f(texX1, texY2);
        glVertex2i(dstX, dstY + height);
    }
    else
    {
        glTexCoord2i(srcX, srcY);
        glVertex2i(dstX, dstY);
        glTexCoord2i(srcX + width, srcY);
        glVertex2i(dstX + width, dstY);
        glTexCoord2i(srcX + width, srcY + height);
        glVertex2i(dstX + width, dstY + height);
        glTexCoord2i(srcX, srcY + height);
        glVertex2i(dstX, dstY + height);
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

        glTexCoord2f(texX1, texY1);
        glVertex2i(dstX, dstY);
        glTexCoord2f(texX2, texY1);
        glVertex2i(dstX + desiredWidth, dstY);
        glTexCoord2f(texX2, texY2);
        glVertex2i(dstX + desiredWidth, dstY + desiredHeight);
        glTexCoord2f(texX1, texY2);
        glVertex2i(dstX, dstY + desiredHeight);
    }
    else
    {
        glTexCoord2i(srcX, srcY);
        glVertex2i(dstX, dstY);
        glTexCoord2i(srcX + width, srcY);
        glVertex2i(dstX + desiredWidth, dstY);
        glTexCoord2i(srcX + width, srcY + height);
        glVertex2i(dstX + desiredWidth, dstY + desiredHeight);
        glTexCoord2i(srcX, srcY + height);
        glVertex2i(dstX, dstY + desiredHeight);
    }
}

bool SafeOpenGLGraphics::drawImage(const Image *const image,
                                   int dstX, int dstY)
{
    return drawImageInline(image, dstX, dstY);
}

bool SafeOpenGLGraphics::drawImageInline(const Image *const image,
                                         int dstX, int dstY)
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (!image)
        return false;

    setColorAlpha(image->mAlpha);
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    const SDL_Rect &bounds = image->mBounds;
    // Draw a textured quad.
    glBegin(GL_QUADS);
    drawQuad(image, bounds.x, bounds.y,
        dstX, dstY, bounds.w, bounds.h);
    glEnd();

    return true;
}

void SafeOpenGLGraphics::drawImageCached(const Image *const image,
                                         int x, int y)
{
    FUNC_BLOCK("Graphics::drawImageCached", 1)
    if (!image)
        return;

    setColorAlpha(image->mAlpha);
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    const SDL_Rect &bounds = image->mBounds;
    // Draw a textured quad.
    glBegin(GL_QUADS);
    drawQuad(image, bounds.x, bounds.y, x, y, bounds.w, bounds.h);
    glEnd();
}

void SafeOpenGLGraphics::drawPatternCached(const Image *const image,
                                           const int x, const int y,
                                           const int w, const int h)
{
    FUNC_BLOCK("Graphics::drawPatternCached", 1)
    if (!image)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    setColorAlpha(image->mAlpha);
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    // Draw a set of textured rectangles
    glBegin(GL_QUADS);

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const int dstY = y + py;
        for (int px = 0; px < w; px += iw)
        {
            int width = (px + iw >= w) ? w - px : iw;
            int dstX = x + px;
            drawQuad(image, srcX, srcY, dstX, dstY, width, height);
        }
    }

    glEnd();
}

void SafeOpenGLGraphics::completeCache()
{
}

bool SafeOpenGLGraphics::drawRescaledImage(const Image *const image,
                                           int dstX, int dstY,
                                           const int desiredWidth,
                                           const int desiredHeight)
{
    FUNC_BLOCK("Graphics::drawRescaledImage", 1)
    if (!image)
        return false;

    const SDL_Rect &imageRect = image->mBounds;

    // Just draw the image normally when no resizing is necessary,
    if (imageRect.w == desiredWidth && imageRect.h == desiredHeight)
        return drawImageInline(image, dstX, dstY);

    setColorAlpha(image->mAlpha);
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    // Draw a textured quad.
    glBegin(GL_QUADS);
    drawRescaledQuad(image, imageRect.x, imageRect.y, dstX, dstY,
        imageRect.w, imageRect.h, desiredWidth, desiredHeight);
    glEnd();

    return true;
}

void SafeOpenGLGraphics::drawPattern(const Image *const image,
                                     const int x, const int y,
                                     const int w, const int h)
{
    drawPatternInline(image, x, y, w, h);
}

void SafeOpenGLGraphics::drawPatternInline(const Image *const image,
                                           const int x, const int y,
                                           const int w, const int h)
{
    FUNC_BLOCK("Graphics::drawPattern", 1)
    if (!image)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    setColorAlpha(image->mAlpha);
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    // Draw a set of textured rectangles
    glBegin(GL_QUADS);

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const int dstY = y + py;
        for (int px = 0; px < w; px += iw)
        {
            int width = (px + iw >= w) ? w - px : iw;
            int dstX = x + px;
            drawQuad(image, srcX, srcY, dstX, dstY, width, height);
        }
    }

    glEnd();
}

void SafeOpenGLGraphics::drawRescaledPattern(const Image *const image,
                                             const int x, const int y,
                                             const int w, const int h,
                                             const int scaledWidth,
                                             const int scaledHeight)
{
    if (!image)
        return;

    const int iw = scaledWidth;
    const int ih = scaledHeight;
    if (iw == 0 || ih == 0)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    setColorAlpha(image->mAlpha);
    bindTexture(OpenGLImageHelper::mTextureType, image->mGLImage);
    setTexturingAndBlending(true);

    // Draw a set of textured rectangles
    glBegin(GL_QUADS);

    const float scaleFactorW = static_cast<float>(scaledWidth)
        / image->getWidth();
    const float scaleFactorH = static_cast<float>(scaledHeight)
        / image->getHeight();

    for (int py = 0; py < h; py += ih)
    {
        const int height = (py + ih >= h) ? h - py : ih;
        const int dstY = y + py;
        for (int px = 0; px < w; px += iw)
        {
            int width = (px + iw >= w) ? w - px : iw;
            int dstX = x + px;

            drawRescaledQuad(image, srcX, srcY, dstX, dstY,
                width / scaleFactorW, height / scaleFactorH,
                scaledWidth, scaledHeight);
        }
    }

    glEnd();
}

void SafeOpenGLGraphics::calcTileCollection(ImageCollection *const
                                            vertCol A_UNUSED,
                                            const Image *const image A_UNUSED,
                                            int x A_UNUSED, int y A_UNUSED)
{
}

void SafeOpenGLGraphics::calcTileVertexes(ImageVertexes *const vert A_UNUSED,
                                          const Image *const image A_UNUSED,
                                          int x A_UNUSED, int y A_UNUSED) const
{
}

void SafeOpenGLGraphics::calcTileVertexesInline(ImageVertexes *const
                                                vert A_UNUSED,
                                                const Image *const
                                                image A_UNUSED,
                                                int x A_UNUSED,
                                                int y A_UNUSED) const
{
}

void SafeOpenGLGraphics::calcPattern(ImageVertexes *const vert A_UNUSED,
                                     const Image *const image A_UNUSED,
                                     const int x A_UNUSED,
                                     const int y A_UNUSED,
                                     const int w A_UNUSED,
                                     const int h A_UNUSED) const
{
}

void SafeOpenGLGraphics::calcPatternInline(ImageVertexes *const vert A_UNUSED,
                                           const Image *const image A_UNUSED,
                                           const int x A_UNUSED,
                                           const int y A_UNUSED,
                                           const int w A_UNUSED,
                                           const int h A_UNUSED) const
{
}

void SafeOpenGLGraphics::calcPattern(ImageCollection *const vert A_UNUSED,
                                     const Image *const image A_UNUSED,
                                     const int x A_UNUSED,
                                     const int y A_UNUSED,
                                     const int w A_UNUSED,
                                     const int h A_UNUSED) const
{
}

void SafeOpenGLGraphics::drawTileVertexes(const ImageVertexes
                                          *const vert A_UNUSED)
{
}

void SafeOpenGLGraphics::drawTileCollection(const ImageCollection *const
                                            vertCol A_UNUSED)
{
}

void SafeOpenGLGraphics::updateScreen()
{
    BLOCK_START("Graphics::updateScreen")
    glFlush();
    glFinish();
#ifdef USE_SDL2
    SDL_GL_SwapWindow(mWindow);
#else
    SDL_GL_SwapBuffers();
#endif
    BLOCK_END("Graphics::updateScreen")
}

void SafeOpenGLGraphics::calcWindow(ImageCollection *const vertCol A_UNUSED,
                                    const int x A_UNUSED, const int y A_UNUSED,
                                    const int w A_UNUSED, const int h A_UNUSED,
                                    const ImageRect &imgRect A_UNUSED)
{
}

void SafeOpenGLGraphics::beginDraw()
{
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, static_cast<double>(mRect.w),
        static_cast<double>(mRect.h),
        0.0, -1.0, 1.0);

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

    glShadeModel(GL_FLAT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void SafeOpenGLGraphics::endDraw()
{
    popClipArea();
}

void SafeOpenGLGraphics::prepareScreenshot()
{
    if (config.getBoolValue("usefbo"))
        graphicsManager.createFBO(mRect.w, mRect.h, &mFbo);
}

SDL_Surface* SafeOpenGLGraphics::getScreenshot()
{
    const int h = mRect.h;
    const int w = mRect.w - (mRect.w % 4);
    GLint pack = 1;

    SDL_Surface *const screenshot = MSDL_CreateRGBSurface(
        SDL_SWSURFACE, w, h, 24, 0xff0000, 0x00ff00, 0x0000ff, 0x000000);

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
    GLubyte* buf = new GLubyte[lineSize];

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

bool SafeOpenGLGraphics::pushClipArea(Rect area)
{
    int transX = 0;
    int transY = 0;

    if (!mClipStack.empty())
    {
        const ClipRect &clipArea = mClipStack.top();
        transX = -clipArea.xOffset;
        transY = -clipArea.yOffset;
    }

    const bool result = Graphics::pushClipArea(area);

    const ClipRect &clipArea = mClipStack.top();

    glPushMatrix();
    glTranslatef(static_cast<GLfloat>(transX + clipArea.xOffset),
                 static_cast<GLfloat>(transY + clipArea.yOffset), 0);
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
    return result;
}

void SafeOpenGLGraphics::popClipArea()
{
    Graphics::popClipArea();

    if (mClipStack.empty())
        return;

    glPopMatrix();
    const ClipRect &clipArea = mClipStack.top();
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void SafeOpenGLGraphics::drawPoint(int x, int y)
{
    setTexturingAndBlending(false);
    restoreColor();

    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

bool SafeOpenGLGraphics::drawNet(const int x1, const int y1,
                                 const int x2, const int y2,
                                 const int width, const int height)
{
    setTexturingAndBlending(false);
    restoreColor();

    glBegin(GL_LINES);
    for (int y = y1; y < y2; y += height)
    {
        glVertex2f(static_cast<float>(x1) + 0.5F,
            static_cast<float>(y) + 0.5F);
        glVertex2f(static_cast<float>(x2) + 0.5F,
            static_cast<float>(y) + 0.5F);
    }

    for (int x = x1; x < x2; x += width)
    {
        glVertex2f(static_cast<float>(x) + 0.5F,
            static_cast<float>(y1) + 0.5F);
        glVertex2f(static_cast<float>(x) + 0.5F,
            static_cast<float>(y2) + 0.5F);
    }
    glEnd();

    return true;
}

void SafeOpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
{
    setTexturingAndBlending(false);
    restoreColor();

    glBegin(GL_LINES);
    glVertex2f(static_cast<float>(x1) + 0.5F, static_cast<float>(y1) + 0.5F);
    glVertex2f(static_cast<float>(x2) + 0.5F, static_cast<float>(y2) + 0.5F);
    glEnd();
}

void SafeOpenGLGraphics::drawRectangle(const Rect& rect)
{
    drawRectangle(rect, false);
}

void SafeOpenGLGraphics::fillRectangle(const Rect& rect)
{
    drawRectangle(rect, true);
}

void SafeOpenGLGraphics::setTexturingAndBlending(const bool enable)
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

void SafeOpenGLGraphics::drawRectangle(const Rect& rect,
                                       const bool filled)
{
    BLOCK_START("Graphics::drawRectangle")
    const float offset = filled ? 0 : 0.5F;

    setTexturingAndBlending(false);
    restoreColor();

    glBegin(filled ? GL_QUADS : GL_LINE_LOOP);
    glVertex2f(static_cast<float>(rect.x) + offset,
        static_cast<float>(rect.y) + offset);
    glVertex2f(static_cast<float>(rect.x + rect.width) - offset,
        static_cast<float>(rect.y) + offset);
    glVertex2f(static_cast<float>(rect.x + rect.width) - offset,
        static_cast<float>(rect.y + rect.height) - offset);
    glVertex2f(static_cast<float>(rect.x) + offset,
        static_cast<float>(rect.y + rect.height) - offset);
    glEnd();
    BLOCK_END("Graphics::drawRectangle")
}

void SafeOpenGLGraphics::bindTexture(const GLenum target, const GLuint texture)
{
    if (mLastImage != texture)
    {
        mLastImage = texture;
        glBindTexture(target, texture);
    }
}

void SafeOpenGLGraphics::setColorAlpha(const float alpha)
{
    if (!mIsByteColor && mFloatColor == alpha)
        return;

    glColor4f(1.0F, 1.0F, 1.0F, alpha);
    mIsByteColor = false;
    mFloatColor = alpha;
}

void SafeOpenGLGraphics::restoreColor()
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

void SafeOpenGLGraphics::drawImageRect(const int x, const int y,
                                       const int w, const int h,
                                       const ImageRect &imgRect)
{
    #include "render/graphics_drawImageRect.hpp"
}

void SafeOpenGLGraphics::calcImageRect(ImageVertexes *const vert,
                                       const int x, const int y,
                                       const int w, const int h,
                                       const ImageRect &imgRect)
{
    #include "render/graphics_calcImageRect.hpp"
}

#endif  // USE_OPENGL
