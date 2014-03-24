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

#include "SDL_video.h"

#include "sdlshared.h"

#include "gui/color.h"

#include "render/renderers.h"

#include "gui/cliprect.h"

#ifdef USE_SDL2
#include <SDL_render.h>
#endif

#include <stack>
#include <string>

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;

struct SDL_Window;

static const int defaultScreenWidth = 800;
static const int defaultScreenHeight = 600;

/**
 * 9 images defining a rectangle. 4 corners, 4 sides and a middle area. The
 * topology is as follows:
 *
 * <pre>
 *  !-----!-----------------!-----!
 *  !  0  !        1        !  2  !
 *  !-----!-----------------!-----!
 *  !  3  !        4        !  5  !
 *  !-----!-----------------!-----!
 *  !  6  !        7        !  8  !
 *  !-----!-----------------!-----!
 * </pre>
 *
 * Sections 0, 2, 6 and 8 will remain as is. 1, 3, 4, 5 and 7 will be
 * repeated to fit the size of the widget.
 */
class ImageRect final
{
    public:
        ImageRect()
        {
            for (int f = 0; f < 9; f ++)
                grid[f] = nullptr;
        }

        A_DELETE_COPY(ImageRect)

        enum ImagePosition
        {
            UPPER_LEFT = 0,
            UPPER_CENTER = 1,
            UPPER_RIGHT = 2,
            LEFT = 3,
            CENTER = 4,
            RIGHT = 5,
            LOWER_LEFT = 6,
            LOWER_CENTER = 7,
            LOWER_RIGHT = 8
        };

        Image *grid[9];
};

/**
 * A central point of control for graphics.
 */
class Graphics
{
    public:
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

        void setWindow(SDL_Window *const window,
                       const int width, const int height)
        {
            mWindow = window;
            mRect.w = width;
            mRect.h = height;
        }

        SDL_Window *getWindow() const
        { return mWindow; }

        /**
         * Sets whether vertical refresh syncing is enabled. Takes effect after
         * the next call to setVideoMode(). Only implemented on MacOS for now.
         */
        void setSync(const bool sync);

        bool getSync() const A_WARN_UNUSED
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
                                  const bool noFrame) = 0;

        /**
         * Set fullscreen mode.
         */
        bool setFullscreen(const bool fs);

        /**
         * Resize the window to the specified size.
         */
        virtual bool resizeScreen(const int width, const int height);

        /**
         * Draws a resclaled version of the image
         */
        virtual bool drawRescaledImage(const Image *const image,
                                       int dstX, int dstY,
                                       const int desiredWidth,
                                       const int desiredHeight) = 0;

        virtual void drawPattern(const Image *const image,
                                 const int x, const int y,
                                 const int w, const int h) = 0;

        /**
         * Draw a pattern based on a rescaled version of the given image...
         */
        virtual void drawRescaledPattern(const Image *const image,
                                         const int x, const int y,
                                         const int w, const int h,
                                         const int scaledWidth,
                                         const int scaledHeight) = 0;

        virtual void drawImageRect(const int x, const int y,
                                   const int w, const int h,
                                   const ImageRect &imgRect) = 0;

        virtual void calcPattern(ImageVertexes *const vert,
                                 const Image *const image,
                                 const int x, const int y,
                                 const int w, const int h) const = 0;

        virtual void calcPattern(ImageCollection *const vert,
                                 const Image *const image,
                                 const int x, const int y,
                                 const int w, const int h) const = 0;

        virtual void calcTileVertexes(ImageVertexes *const vert,
                                      const Image *const image,
                                      int x, int y) const = 0;

        virtual void calcTileSDL(ImageVertexes *const vert A_UNUSED,
                                 int x A_UNUSED, int y A_UNUSED) const
        {
        }

        virtual void drawTileVertexes(const ImageVertexes *const vert) = 0;

        virtual void drawTileCollection(const ImageCollection
                                        *const vertCol) = 0;

        virtual void calcTileCollection(ImageCollection *const vertCol,
                                        const Image *const image,
                                        int x, int y) = 0;

        virtual void calcWindow(ImageCollection *const vertCol,
                                const int x, const int y,
                                const int w, const int h,
                                const ImageRect &imgRect) = 0;

        virtual void fillRectangle(const Rect& rectangle) = 0;

        /**
         * Updates the screen. This is done by either copying the buffer to the
         * screen or swapping pages.
         */
        virtual void updateScreen() = 0;

        void setWindowSize(const int width, const int height);

        /**
         * Returns the width of the screen.
         */
        int getWidth() const A_WARN_UNUSED;

        /**
         * Returns the height of the screen.
         */
        int getHeight() const A_WARN_UNUSED;

        /**
         * Takes a screenshot and returns it as SDL surface.
         */
        virtual SDL_Surface *getScreenshot() A_WARN_UNUSED = 0;

        virtual void prepareScreenshot()
        { }

        int getMemoryUsage() const A_WARN_UNUSED;

        virtual bool drawNet(const int x1, const int y1,
                             const int x2, const int y2,
                             const int width, const int height);

        ClipRect &getTopClip() A_WARN_UNUSED
        { return mClipStack.top(); }

        void setRedraw(const bool n)
        { mRedraw = n; }

        bool getRedraw() const A_WARN_UNUSED
        { return mRedraw; }

        void setSecure(const bool n)
        { mSecure = n; }

        bool getSecure() const A_WARN_UNUSED
        { return mSecure; }

        int getBpp() const A_WARN_UNUSED
        { return mBpp; }

        bool getFullScreen() const A_WARN_UNUSED
        { return mFullscreen; }

        bool getHWAccel() const A_WARN_UNUSED
        { return mHWAccel; }

        bool getDoubleBuffer() const A_WARN_UNUSED
        { return mDoubleBuffer; }

        RenderType getOpenGL() const A_WARN_UNUSED
        { return mOpenGL; }

        void setNoFrame(const bool n)
        { mNoFrame = n; }

        const std::string &getName() const A_WARN_UNUSED
        { return mName; }

        virtual void initArrays()
        { }

        virtual void setColor(const Color &color)
        {
            mColor = color;
            mColor2 = color;
            mAlpha = (color.a != 255);
        }

        void setColor2(const Color &color)
        { mColor2 = color; }

        void setColorAll(const Color &color, const Color &color2)
        {
            mColor = color;
            mColor2 = color2;
            mAlpha = (color.a != 255);
        }

        const Color &getColor() const
        { return mColor; }

        const Color &getColor2() const
        { return mColor2; }

#ifdef DEBUG_DRAW_CALLS
        virtual unsigned int getDrawCalls() const
        { return 0; }
#endif
#ifdef DEBUG_BIND_TEXTURE
        virtual unsigned int getBinds() const
        { return 0; }
#endif
#ifdef USE_SDL2
        void dumpRendererInfo(const char *const str,
                              const SDL_RendererInfo &info);

        virtual void setRendererFlags(const uint32_t flags A_UNUSED)
        { }
#endif

        /**
         * Blits an image onto the screen.
         *
         * @return <code>true</code> if the image was blitted properly
         *         <code>false</code> otherwise.
         */
        virtual bool drawImage(const Image *const image,
                               int dstX, int dstY) = 0;

        virtual void drawImageCached(const Image *const image,
                                     int srcX, int srcY) = 0;

        virtual void drawPatternCached(const Image *const image,
                                       const int x, const int y,
                                       const int w, const int h) = 0;

        virtual void completeCache() = 0;

        int getScale() const
        { return mScale; }

        virtual bool isAllowScale() const
        { return false; }

        void setScale(int scale);

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
         * @return False if the the new area lays outside the current clip 
         *         area.
         */
        virtual bool pushClipArea(Rect area);

        /**
         * Removes the top most clip area from the stack.
         *
         * @throws Exception if the stack is empty.
         */
        virtual void popClipArea();

        /**
         * Ddraws a line.
         *
         * @param x1 The first x coordinate.
         * @param y1 The first y coordinate.
         * @param x2 The second x coordinate.
         * @param y2 The second y coordinate.
         */
        virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

        /**
         * Draws a simple, non-filled, rectangle with a one pixel width.
         *
         * @param rectangle The rectangle to draw.
         */
        virtual void drawRectangle(const Rect &rectangle) = 0;

        /**
         * Gets the current clip area. Usefull if you want to do drawing
         * bypassing Graphics.
         *
         * @return The current clip area.
         */
        virtual const ClipRect *getCurrentClipArea() const;

        /**
         * Draws a single point/pixel.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         */
        virtual void drawPoint(int x, int y) = 0;

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
        virtual void beginDraw()
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
        virtual void endDraw()
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
                          const bool noFrame);

        int getOpenGLFlags() const A_WARN_UNUSED;

        int getSoftwareFlags() const A_WARN_UNUSED;

        bool setOpenGLMode();

        void updateMemoryInfo();

        bool videoInfo();

        /**
         * Holds the clip area stack.
         */
        std::stack<ClipRect> mClipStack;

        SDL_Window *mWindow;

#ifdef USE_SDL2
        SDL_Renderer *mRenderer;
#ifdef USE_OPENGL
        SDL_GLContext mGLContext;
#endif
#endif
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
        Color mColor2;
};

extern Graphics *mainGraphics;

#endif  // RENDER_GRAPHICS_H
