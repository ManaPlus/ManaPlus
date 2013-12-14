/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef RENDER_GRAPHICS_H
#define RENDER_GRAPHICS_H

#include "SDL_video.h"

#include "sdlshared.h"

#include "render/renderers.h"

#include <guichan/color.hpp>
#include <guichan/graphics.hpp>

#ifdef USE_SDL2
#include <SDL_render.h>
#endif

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;

struct SDL_Window;

static const int defaultScreenWidth = 800;
static const int defaultScreenHeight = 600;

#define DRAW_IMAGE(graphics, image, x, y) \
    { \
        if (image) \
        { \
            (graphics)->drawImage2(image, 0, 0, x, y, \
                (image)->mBounds.w, (image)->mBounds.h, false); \
        } \
    }

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
class Graphics : public gcn::Graphics
{
    public:
        A_DELETE_COPY(Graphics)

        /**
         * Destructor.
         */
        virtual ~Graphics();

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
        virtual bool setVideoMode(const int w, const int h, const int bpp,
                                  const bool fs, const bool hwaccel,
                                  const bool resize, const bool noFrame) = 0;

        /**
         * Set fullscreen mode.
         */
        bool setFullscreen(const bool fs);

        /**
         * Resize the window to the specified size.
         */
        virtual bool resizeScreen(const int width, const int height);

        /**
         * Blits an image onto the screen.
         *
         * @return <code>true</code> if the image was blitted properly
         *         <code>false</code> otherwise.
         */
        bool drawImage(const Image *image, int x, int y);

        //  override unused abstract function
        void drawImage(const gcn::Image* image A_UNUSED,
                       int srcX A_UNUSED, int srcY A_UNUSED,
                       int dstX A_UNUSED, int dstY A_UNUSED,
                       int width A_UNUSED, int height A_UNUSED) override
        {
        }

        /**
         * Draws a resclaled version of the image
         */
        virtual bool drawRescaledImage(const Image *const image, int srcX,
                                       int srcY, int dstX, int dstY,
                                       const int width, const int height,
                                       const int desiredWidth,
                                       const int desiredHeight,
                                       const bool useColor = false) = 0;

        virtual void drawImagePattern(const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h) = 0;

        /**
         * Draw a pattern based on a rescaled version of the given image...
         */
        virtual void drawRescaledImagePattern(const Image *const image,
                                              const int x, const int y,
                                              const int w, const int h,
                                              const int scaledWidth,
                                              const int scaledHeight) = 0;

        /**
         * Draws a rectangle using images. 4 corner images, 4 side images and 1
         * image for the inside.
         */
        void drawImageRect(const int x, const int y, const int w, const int h,
                           const Image *const topLeft,
                           const Image *const topRight,
                           const Image *const bottomLeft,
                           const Image *const bottomRight,
                           const Image *const top,
                           const Image *const right,
                           const Image *const bottom,
                           const Image *const left,
                           const Image *const center);

        /**
         * Draws a rectangle using images. 4 corner images, 4 side images and 1
         * image for the inside.
         */
        void drawImageRect(int x, int y, int w, int h,
                           const ImageRect &imgRect);

        bool calcImageRect(ImageVertexes *const vert,
                           const int x, const int y,
                           const int w, const int h,
                           const Image *const topLeft,
                           const Image *const topRight,
                           const Image *const bottomLeft,
                           const Image *const bottomRight,
                           const Image *const top,
                           const Image *const right,
                           const Image *const bottom,
                           const Image *const left,
                           const Image *const center);

        virtual void calcImagePattern(ImageVertexes *const vert,
                                      const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h) const = 0;

        virtual void calcImagePattern(ImageCollection *const vert,
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

        virtual bool calcWindow(ImageCollection *const vertCol,
                                const int x, const int y,
                                const int w, const int h,
                                const ImageRect &imgRect) = 0;

        /**
         * Draws a rectangle using images. 4 corner images, 4 side images and 1
         * image for the inside.
         */
        inline void drawImageRect(const gcn::Rectangle &area,
                                  const ImageRect &imgRect)
        { drawImageRect(area.x, area.y, area.width, area.height, imgRect); }

        virtual void fillRectangle(const gcn::Rectangle& rectangle)
                                   override = 0;

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

        const gcn::Font *getFont() const A_WARN_UNUSED
        { return mFont; }

        gcn::ClipRectangle &getTopClip() A_WARN_UNUSED
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

        void setColor(const gcn::Color &color) override
        {
            mColor = color;
            mColor2 = color;
            mAlpha = (color.a != 255);
        }

        void setColor2(const gcn::Color &color)
        { mColor2 = color; }

        void setColorAll(const gcn::Color &color, const gcn::Color &color2)
        {
            mColor = color;
            mColor2 = color2;
            mAlpha = (color.a != 255);
        }

        const gcn::Color &getColor() const override
        { return mColor; }

        const gcn::Color &getColor2() const
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
        virtual bool drawImage2(const Image *const image,
                                int srcX, int srcY,
                                int dstX, int dstY,
                                const int width, const int height,
                                const bool useColor) = 0;

        int mWidth;
        int mHeight;

    protected:
        /**
         * Constructor.
         */
        Graphics();

        void setMainFlags(const int w, const int h, const int bpp,
                          const bool fs, const bool hwaccel,
                          const bool resize, const bool noFrame);

        int getOpenGLFlags() const A_WARN_UNUSED;

        int getSoftwareFlags() const A_WARN_UNUSED;

        bool setOpenGLMode();

        void updateMemoryInfo();

        bool videoInfo();

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
        gcn::Color mColor;
        gcn::Color mColor2;
};

extern Graphics *mainGraphics;

#endif  // RENDER_GRAPHICS_H
