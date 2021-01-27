/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#if defined USE_OPENGL && !defined ANDROID && !defined(__SWITCH__)
#include "render/safeopenglgraphics.h"

#ifdef DEBUG_OPENGL
#include "render/opengl/mgl.h"
#endif  // DEBUG_OPENGL

#include "resources/imagerect.h"
#include "resources/safeopenglimagehelper.h"

#include "resources/image/image.h"

#include "utils/sdlcheckutils.h"

#include "debug.h"

GLuint SafeOpenGLGraphics::mTextureBinded = 0;

SafeOpenGLGraphics::SafeOpenGLGraphics() :
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

void SafeOpenGLGraphics::deleteArrays() restrict2
{
}

bool SafeOpenGLGraphics::setVideoMode(const int w, const int h,
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

static inline void drawQuad(const Image *restrict image,
                            const int srcX,
                            const int srcY,
                            const int dstX,
                            const int dstY,
                            const int width,
                            const int height) A_NONNULL(1) A_INLINE;

static inline void drawQuad(const Image *restrict image,
                            const int srcX,
                            const int srcY,
                            const int dstX,
                            const int dstY,
                            const int width,
                            const int height)
{
    if (SafeOpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
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
    if (SafeOpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
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

void SafeOpenGLGraphics::drawImage(const Image *restrict const image,
                                   int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void SafeOpenGLGraphics::drawImageInline(const Image *restrict const image,
                                         int dstX, int dstY) restrict2
{
    FUNC_BLOCK("Graphics::drawImage", 1)
    if (image == nullptr)
        return;

    setColorAlpha(image->mAlpha);
    bindTexture(SafeOpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

    const SDL_Rect &bounds = image->mBounds;
    // Draw a textured quad.
    glBegin(GL_QUADS);
    drawQuad(image, bounds.x, bounds.y,
        dstX, dstY, bounds.w, bounds.h);
    glEnd();
}

void SafeOpenGLGraphics::copyImage(const Image *restrict const image,
                                   int dstX, int dstY) restrict2
{
    drawImageInline(image, dstX, dstY);
}

void SafeOpenGLGraphics::testDraw() restrict2
{
    if (SafeOpenGLImageHelper::mTextureType == GL_TEXTURE_2D)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(0.0F, 0.781250F);
        glVertex2i(0, 0);
        glTexCoord2f(0.0F, 0.781250F);
        glVertex2i(800, 0);
        glTexCoord2f(0.0F, 0.585938F);
        glVertex2i(800, 600);
        glTexCoord2f(0.0F, 0.585938F);
        glVertex2i(0, 600);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex2i(0, 0);
        glTexCoord2i(800, 0);
        glVertex2i(800, 0);
        glTexCoord2i(800, 600);
        glVertex2i(800, 600);
        glTexCoord2i(0, 600);
        glVertex2i(0, 600);
        glEnd();
    }
}

void SafeOpenGLGraphics::drawImageCached(const Image *restrict const image,
                                         int x, int y) restrict2
{
    FUNC_BLOCK("Graphics::drawImageCached", 1)
    if (image == nullptr)
        return;

    setColorAlpha(image->mAlpha);
    bindTexture(SafeOpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

    const SDL_Rect &bounds = image->mBounds;
    // Draw a textured quad.
    glBegin(GL_QUADS);
    drawQuad(image, bounds.x, bounds.y, x, y, bounds.w, bounds.h);
    glEnd();
}

void SafeOpenGLGraphics::drawPatternCached(const Image *restrict const image,
                                           const int x, const int y,
                                           const int w, const int h) restrict2
{
    FUNC_BLOCK("Graphics::drawPatternCached", 1)
    if (image == nullptr)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    setColorAlpha(image->mAlpha);
    bindTexture(SafeOpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

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

void SafeOpenGLGraphics::completeCache() restrict2
{
}

void SafeOpenGLGraphics::drawRescaledImage(const Image *restrict const image,
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
    bindTexture(SafeOpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

    // Draw a textured quad.
    glBegin(GL_QUADS);
    drawRescaledQuad(image, imageRect.x, imageRect.y, dstX, dstY,
        imageRect.w, imageRect.h, desiredWidth, desiredHeight);
    glEnd();
}

void SafeOpenGLGraphics::drawPattern(const Image *restrict const image,
                                     const int x, const int y,
                                     const int w, const int h) restrict2
{
    drawPatternInline(image, x, y, w, h);
}

void SafeOpenGLGraphics::drawPatternInline(const Image *restrict const image,
                                           const int x, const int y,
                                           const int w, const int h) restrict2
{
    FUNC_BLOCK("Graphics::drawPattern", 1)
    if (image == nullptr)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int iw = imageRect.w;
    const int ih = imageRect.h;
    if (iw == 0 || ih == 0)
        return;

    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    setColorAlpha(image->mAlpha);
    bindTexture(SafeOpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

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

void SafeOpenGLGraphics::drawRescaledPattern(const Image *restrict const image,
                                             const int x, const int y,
                                             const int w, const int h,
                                             const int scaledWidth,
                                             const int scaledHeight) restrict2
{
    if (image == nullptr)
        return;

    const int iw = scaledWidth;
    const int ih = scaledHeight;
    if (iw == 0 || ih == 0)
        return;

    const SDL_Rect &imageRect = image->mBounds;
    const int srcX = imageRect.x;
    const int srcY = imageRect.y;

    setColorAlpha(image->mAlpha);
    bindTexture(SafeOpenGLImageHelper::mTextureType, image->mGLImage);
    enableTexturingAndBlending();

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

void SafeOpenGLGraphics::calcTileCollection(ImageCollection *restrict const
                                            vertCol A_UNUSED,
                                            const Image *restrict const image
                                            A_UNUSED,
                                            int x A_UNUSED,
                                            int y A_UNUSED) restrict2
{
}

void SafeOpenGLGraphics::calcTileVertexes(ImageVertexes *restrict const vert
                                          A_UNUSED,
                                          const Image *restrict const image
                                          A_UNUSED,
                                          int x A_UNUSED,
                                          int y A_UNUSED) const restrict2
{
}

void SafeOpenGLGraphics::calcTileVertexesInline(ImageVertexes *restrict const
                                                vert A_UNUSED,
                                                const Image *restrict const
                                                image A_UNUSED,
                                                int x A_UNUSED,
                                                int y A_UNUSED) const restrict2
{
}

void SafeOpenGLGraphics::calcPattern(ImageVertexes *restrict const vert
                                     A_UNUSED,
                                     const Image *restrict const image
                                     A_UNUSED,
                                     const int x A_UNUSED,
                                     const int y A_UNUSED,
                                     const int w A_UNUSED,
                                     const int h A_UNUSED) const restrict2
{
}

void SafeOpenGLGraphics::calcPatternInline(ImageVertexes *restrict const vert
                                           A_UNUSED,
                                           const Image *restrict const image
                                           A_UNUSED,
                                           const int x A_UNUSED,
                                           const int y A_UNUSED,
                                           const int w A_UNUSED,
                                           const int h A_UNUSED)
                                           const restrict2
{
}

void SafeOpenGLGraphics::calcPattern(ImageCollection *restrict const vert
                                     A_UNUSED,
                                     const Image *restrict const image
                                     A_UNUSED,
                                     const int x A_UNUSED,
                                     const int y A_UNUSED,
                                     const int w A_UNUSED,
                                     const int h A_UNUSED) const restrict2
{
}

void SafeOpenGLGraphics::drawTileVertexes(const ImageVertexes
                                          *restrict const vert A_UNUSED)
                                          restrict2
{
}

void SafeOpenGLGraphics::drawTileCollection(const ImageCollection *
                                            restrict const vertCol A_UNUSED)
                                            restrict2
{
}

void SafeOpenGLGraphics::updateScreen() restrict2
{
    BLOCK_START("Graphics::updateScreen")
    glFlush();
    glFinish();
#ifdef USE_SDL2
    SDL_GL_SwapWindow(mWindow);
#else
    SDL_GL_SwapBuffers();
#endif  // USE_SDL2
#ifdef DEBUG_OPENGL
    if (isGLNotNull(mglFrameTerminator))
        mglFrameTerminator();
#endif  // DEBUG_OPENGL

    BLOCK_END("Graphics::updateScreen")
}

void SafeOpenGLGraphics::calcWindow(ImageCollection *restrict const vertCol
                                    A_UNUSED,
                                    const int x A_UNUSED, const int y A_UNUSED,
                                    const int w A_UNUSED, const int h A_UNUSED,
                                    const ImageRect &restrict imgRect A_UNUSED)
                                    restrict2
{
}

void SafeOpenGLGraphics::beginDraw() restrict2
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

    setOpenGLFlags();
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_COLOR_MATERIAL);

    glShadeModel(GL_FLAT);

    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void SafeOpenGLGraphics::endDraw() restrict2
{
    popClipArea();
}

void SafeOpenGLGraphics::pushClipArea(const Rect &restrict area) restrict2
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

    glPushMatrix();
    glTranslatef(static_cast<GLfloat>(transX + clipArea.xOffset),
                 static_cast<GLfloat>(transY + clipArea.yOffset), 0);
    glScissor(clipArea.x * mScale,
        (mRect.h - clipArea.y - clipArea.height) * mScale,
        clipArea.width * mScale,
        clipArea.height * mScale);
}

void SafeOpenGLGraphics::popClipArea() restrict2
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

void SafeOpenGLGraphics::drawPoint(int x, int y) restrict2
{
    disableTexturingAndBlending();
    restoreColor();

    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void SafeOpenGLGraphics::drawNet(const int x1, const int y1,
                                 const int x2, const int y2,
                                 const int width, const int height) restrict2
{
    disableTexturingAndBlending();
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
}

void SafeOpenGLGraphics::drawLine(int x1, int y1,
                                  int x2, int y2) restrict2
{
    disableTexturingAndBlending();
    restoreColor();

    glBegin(GL_LINES);
    glVertex2f(static_cast<float>(x1) + 0.5F, static_cast<float>(y1) + 0.5F);
    glVertex2f(static_cast<float>(x2) + 0.5F, static_cast<float>(y2) + 0.5F);
    glEnd();
}

void SafeOpenGLGraphics::drawRectangle(const Rect &restrict rect) restrict2
{
    drawRectangle(rect, false);
}

void SafeOpenGLGraphics::fillRectangle(const Rect &restrict rect) restrict2
{
    drawRectangle(rect, true);
}

void SafeOpenGLGraphics::enableTexturingAndBlending() restrict2
{
    if (!mTexture)
    {
        glEnable(SafeOpenGLImageHelper::mTextureType);
        mTexture = true;
    }

    if (!mAlpha)
    {
        glEnable(GL_BLEND);
        mAlpha = true;
    }
}

void SafeOpenGLGraphics::disableTexturingAndBlending() restrict2
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
        glDisable(SafeOpenGLImageHelper::mTextureType);
        mTexture = false;
    }
}

void SafeOpenGLGraphics::drawRectangle(const Rect &restrict rect,
                                       const bool filled) restrict2
{
    BLOCK_START("Graphics::drawRectangle")
    const float offset = filled ? 0 : 0.5F;

    disableTexturingAndBlending();
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

void SafeOpenGLGraphics::bindTexture(const GLenum target,
                                     const GLuint texture)
{
    if (mTextureBinded != texture)
    {
        mTextureBinded = texture;
        glBindTexture(target, texture);
    }
}

void SafeOpenGLGraphics::setColorAlpha(const float alpha) restrict2
{
    if (!mIsByteColor && mFloatColor == alpha)
        return;

    glColor4f(1.0F, 1.0F, 1.0F, alpha);
    mIsByteColor = false;
    mFloatColor = alpha;
}

void SafeOpenGLGraphics::restoreColor() restrict2
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

void SafeOpenGLGraphics::clearScreen() const restrict2
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void SafeOpenGLGraphics::drawImageRect(const int x, const int y,
                                       const int w, const int h,
                                       const ImageRect &restrict imgRect)
                                       restrict2
{
    #include "render/graphics_drawImageRect.hpp"
}

void SafeOpenGLGraphics::calcImageRect(ImageVertexes *restrict const vert,
                                       const int x, const int y,
                                       const int w, const int h,
                                       const ImageRect &restrict imgRect)
                                       restrict2
{
    #include "render/graphics_calcImageRect.hpp"
}

#endif  // USE_OPENGL
