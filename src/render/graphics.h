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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RENDER_GRAPHICS_H
#define RENDER_GRAPHICS_H

#include "sdlshared.h"

#include "enums/render/rendertype.h"

#include "gui/color.h"
#include "gui/cliprect.h"

#include "resources/mstack.h"

#ifdef USE_SDL2
#include <SDL_render.h>
#else  // USE_SDL2
#include <SDL_video.h>
#endif  // USE_SDL2

#include "localconsts.h"

#ifdef USE_SDL2
#define RectPos int32_t
#define RectSize int32_t
#else  // USE_SDL2
#define RectPos int16_t
#define RectSize uint16_t
#endif  // USE_SDL2

class Image;
class ImageCollection;
class ImageRect;
class ImageVertexes;

struct SDL_Window;

/**
 * A central point of control for graphics.
 */
class Graphics notfinal
{
    public:
#ifdef USE_OPENGL
        friend class OpenGLScreenshotHelper;
#endif  // USE_SDL2

        friend class SdlScreenshotHelper;

        A_DELETE_COPY(Graphics)

        /**
         * Destructor.
         */
        virtual ~Graphics();

        /**
         * Alignments for text drawing.
         */
        enum Alignment
        {
            LEFT = 0,
            CENTER,
            RIGHT
        };

        void setWindow(SDL_Window *restrict const window,
                       const int width, const int height) restrict2 noexcept2
        {
            mWindow = window;
            mRect.w = static_cast<RectSize>(width);
            mRect.h = static_cast<RectSize>(height);
        }

        SDL_Window *getWindow() const restrict2 noexcept2
        { return mWindow; }

        /**
         * Sets whether vertical refresh syncing is enabled. Takes effect after
         * the next call to setVideoMode(). Only implemented on MacOS for now.
         */
        void setSync(const bool sync) restrict2;

        bool getSync() const restrict2 noexcept2 A_WARN_UNUSED
        { return mSync; }

        /**
         * Try to create a window with the given settings.
         */
        virtual bool setVideoMode(const int w, const int h,
                                  const int scale,
                                  const int bpp,
                                  const bool fs,
                                  const bool hwaccel,
                                  const bool resize,
                                  const bool noFrame) restrict2 = 0;

        /**
         * Set fullscreen mode.
         */
        bool setFullscreen(const bool fs) restrict2;

        /**
         * Resize the window to the specified size.
         */
        virtual bool resizeScreen(const int width,
                                  const int height) restrict2;

        virtual void restoreContext() restrict2
        { }

        /**
         * Draws a resclaled version of the image
         */
        virtual void drawRescaledImage(const Image *restrict const image,
                                       int dstX, int dstY,
                                       const int desiredWidth,
                                       const int desiredHeight) restrict2 = 0;

        virtual void drawPattern(const Image *restrict const image,
                                 const int x, const int y,
                                 const int w, const int h) restrict2 = 0;

        /**
         * Draw a pattern based on a rescaled version of the given image...
         */
        virtual void drawRescaledPattern(const Image *restrict const image,
                                         const int x, const int y,
                                         const int w, const int h,
                                         const int scaledWidth,
                                         const int scaledHeight) restrict2 = 0;

        virtual void drawImageRect(const int x, const int y,
                                   const int w, const int h,
                                   const ImageRect &restrict imgRect)
                                   restrict2 = 0;

        virtual void calcPattern(ImageVertexes *restrict const vert,
                                 const Image *restrict const image,
                                 const int x, const int y,
                                 const int w, const int h) const restrict2 = 0;

        virtual void calcPattern(ImageCollection *restrict const vert,
                                 const Image *restrict const image,
                                 const int x, const int y,
                                 const int w, const int h) const restrict2 = 0;

        virtual void calcTileVertexes(ImageVertexes *restrict const vert,
                                      const Image *restrict const image,
                                      int x,
                                      int y) const restrict2 A_NONNULL(2, 3)
                                      = 0;

        virtual void calcTileSDL(ImageVertexes *restrict const vert A_UNUSED,
                                 int x A_UNUSED,
                                 int y A_UNUSED) const restrict2
        {
        }

        virtual void drawTileVertexes(const ImageVertexes *restrict const vert)
                                      restrict2 = 0;

        virtual void drawTileCollection(const ImageCollection
                                        *restrict const vertCol) restrict2
                                        A_NONNULL(2) = 0;

        virtual void calcTileCollection(ImageCollection *restrict const
                                        vertCol,
                                        const Image *restrict const image,
                                        int x,
                                        int y) restrict2 = 0;

        virtual void calcWindow(ImageCollection *restrict const vertCol,
                                const int x, const int y,
                                const int w, const int h,
                                const ImageRect &restrict imgRect)
                                restrict2 A_NONNULL(2) = 0;

        virtual void fillRectangle(const Rect &restrict rectangle)
                                   restrict2 = 0;

        /**
         * Updates the screen. This is done by either copying the buffer to the
         * screen or swapping pages.
         */
        virtual void updateScreen() restrict2 = 0;

        void setWindowSize(const int width,
#ifdef USE_SDL2
            const int height) restrict2;
#else  // USE_SDL2
            const int height) restrict2 A_CONST;
#endif  // USE_SDL2

        /**
         * Returns the width of the screen.
         */
        int getWidth() const restrict2 A_WARN_UNUSED;

        /**
         * Returns the height of the screen.
         */
        int getHeight() const restrict2 A_WARN_UNUSED;

        int getMemoryUsage() const restrict2 A_WARN_UNUSED;

        virtual void drawNet(const int x1, const int y1,
                             const int x2, const int y2,
                             const int width, const int height) restrict2;

        ClipRect &getTopClip() const restrict2 A_WARN_UNUSED
        { return mClipStack.top(); }

        void setRedraw(const bool n) restrict2 noexcept2
        { mRedraw = n; }

        bool getRedraw() const restrict2 noexcept2 A_WARN_UNUSED
        { return mRedraw; }

        void setSecure(const bool n) restrict2 noexcept2
        { mSecure = n; }

        bool getSecure() const restrict2 noexcept2 A_WARN_UNUSED
        { return mSecure; }

        int getBpp() const restrict2 noexcept2 A_WARN_UNUSED
        { return mBpp; }

        bool getFullScreen() const restrict2 noexcept2 A_WARN_UNUSED
        { return mFullscreen; }

        bool getHWAccel() const restrict2 noexcept2 A_WARN_UNUSED
        { return mHWAccel; }

        bool getDoubleBuffer() const restrict2 noexcept2 A_WARN_UNUSED
        { return mDoubleBuffer; }

        RenderType getOpenGL() const restrict2 noexcept2 A_WARN_UNUSED
        { return mOpenGL; }

        void setNoFrame(const bool n) restrict2 noexcept2
        { mNoFrame = n; }

        const std::string &getName() const restrict2 noexcept2 A_WARN_UNUSED
        { return mName; }

        virtual void initArrays(const int vertCount A_UNUSED) restrict2
        { }

        virtual void setColor(const Color &restrict color) restrict2
        {
            mColor = color;
            mAlpha = (color.a != 255);
        }

        const Color &getColor() const restrict2 noexcept2
        { return mColor; }

#ifdef DEBUG_DRAW_CALLS
        virtual unsigned int getDrawCalls() const restrict2
        { return 0; }
#endif  // DEBUG_DRAW_CALLS
#ifdef DEBUG_BIND_TEXTURE
        virtual unsigned int getBinds() const restrict2
        { return 0; }
#endif  // DEBUG_BIND_TEXTURE
#ifdef USE_SDL2
        void dumpRendererInfo(const char *restrict const str,
                              const SDL_RendererInfo &restrict info) restrict2;

        virtual void setRendererFlags(const uint32_t flags A_UNUSED) restrict2
        { }
#endif  // USE_SDL2

        /**
         * Blits an image onto the screen.
         *
         * @return <code>true</code> if the image was blitted properly
         *         <code>false</code> otherwise.
         */
        virtual void drawImage(const Image *restrict const image,
                               int dstX, int dstY) restrict2 = 0;

        virtual void copyImage(const Image *restrict const image,
                               int dstX, int dstY) restrict2 = 0;

        virtual void drawImageCached(const Image *restrict const image,
                                     int srcX, int srcY) restrict2 = 0;

        virtual void drawPatternCached(const Image *restrict const image,
                                       const int x, const int y,
                                       const int w, const int h) restrict2 = 0;

        virtual void completeCache() restrict2 = 0;

        int getScale() const restrict2 noexcept2
        { return mScale; }

        virtual bool isAllowScale() const restrict2 noexcept2
        { return false; }

        void setScale(int scale) restrict2;

        /**
         * Pushes a clip area onto the stack. The x and y coordinates in the
         * rectangle is  relative to the last pushed clip area.
         * If the new area falls outside the current clip area, it will be
         * clipped as necessary.
         *
         * If a clip area is outside of the top clip area a clip area with
         * zero width and height will be pushed.
         *
         * @param area The clip area to be pushed onto the stack.
         */
        virtual void pushClipArea(const Rect &restrict area) restrict2;

        /**
         * Removes the top most clip area from the stack.
         *
         * @throws Exception if the stack is empty.
         */
        virtual void popClipArea() restrict2;

        /**
         * Ddraws a line.
         *
         * @param x1 The first x coordinate.
         * @param y1 The first y coordinate.
         * @param x2 The second x coordinate.
         * @param y2 The second y coordinate.
         */
        virtual void drawLine(int x1, int y1,
                              int x2, int y2) restrict2 = 0;

        /**
         * Draws a simple, non-filled, rectangle with a one pixel width.
         *
         * @param rectangle The rectangle to draw.
         */
        virtual void drawRectangle(const Rect &restrict rectangle)
                                   restrict2 = 0;

#ifdef USE_OPENGL
#ifdef USE_SDL2
        virtual void createGLContext(const bool custom) restrict2;
#else  // USE_SDL2

        virtual void createGLContext(const bool custom) restrict2 A_CONST;
#endif  // USE_SDL2
#endif  // USE_OPENGL

        /**
         * Draws a single point/pixel.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         */
        virtual void drawPoint(int x, int y) restrict2 = 0;

        /**
          * Initializes drawing. Called by the Gui when Gui::draw() is called.
          * It is needed by some implementations of Graphics to perform
          * preparations before drawing. An example of such an implementation
          * is the OpenGLGraphics.
          *
          * NOTE: You will never need to call this function yourself, unless
          *       you use a Graphics object outside of Guichan.
          *
          * @see endDraw, Gui::draw
          */
        virtual void beginDraw() restrict2
        { }

        /**
          * Deinitializes drawing. Called by the Gui when a Gui::draw() is done.
          * done. It should reset any state changes made by beginDraw().
          *
          * NOTE: You will never need to call this function yourself, unless
          *       you use a Graphics object outside of Guichan.
          *
          * @see beginDraw, Gui::draw
          */
        virtual void endDraw() restrict2
        { }

        virtual void clearScreen() const restrict2
        { }

        virtual void deleteArrays() restrict2
        { }

        virtual void postInit() restrict2
        { }

        virtual void finalize(ImageCollection *restrict const col A_UNUSED)
                              restrict2
        { }

        virtual void finalize(ImageVertexes *restrict const vert A_UNUSED)
                              restrict2
        { }

        virtual void testDraw() restrict2
        { }

        virtual void removeArray(const uint32_t sz A_UNUSED,
                                 uint32_t *restrict const arr A_UNUSED)
                                 restrict2
        { }

        virtual void screenResized() restrict2
        { }

        int mWidth;
        int mHeight;
        int mActualWidth;
        int mActualHeight;

    protected:
        /**
         * Constructor.
         */
        Graphics();

        void setMainFlags(const int w, const int h,
                          const int scale,
                          const int bpp,
                          const bool fs,
                          const bool hwaccel,
                          const bool resize,
                          const bool noFrame) restrict2;

        int getOpenGLFlags() const restrict2 A_WARN_UNUSED;

        int getSoftwareFlags() const restrict2 A_WARN_UNUSED;

        bool setOpenGLMode() restrict2;

        void updateMemoryInfo() restrict2;

        bool videoInfo() restrict2;

#ifdef USE_OPENGL
        void setOpenGLFlags() restrict2;
#endif  // USE_OPENGL

        /**
         * Holds the clip area stack.
         */
        MStack<ClipRect> mClipStack;

        SDL_Window *restrict mWindow;

#ifdef USE_SDL2
        SDL_Renderer *restrict mRenderer;
#endif  // USE_SDL2
#ifdef USE_OPENGL
#ifdef USE_SDL2
        SDL_GLContext mGLContext;
#else  // USE_SDL2

        void *restrict mGLContext;
#endif  // USE_SDL2
#endif  // USE_OPENGL

        int mBpp;
        bool mAlpha;
        bool mFullscreen;
        bool mHWAccel;
        bool mRedraw;
        bool mDoubleBuffer;
        SDL_Rect mRect;
        bool mSecure;
        RenderType mOpenGL;
        bool mEnableResize;
        bool mNoFrame;
        std::string mName;
        int mStartFreeMem;
        bool mSync;
        int mScale;
        Color mColor;
};

extern Graphics *mainGraphics A_NONNULLPOINTER;

#endif  // RENDER_GRAPHICS_H
