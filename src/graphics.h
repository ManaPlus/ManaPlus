/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <guichan/sdl/sdlgraphics.hpp>

class GraphicsVertexes;
class Image;
class ImageVertexes;
class MapLayer;

struct SDL_Surface;

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
class Graphics : public gcn::SDLGraphics
{
    public:
        enum BlitMode
        {
            BLIT_NORMAL = 0,
            BLIT_GFX
        };

        /**
         * Constructor.
         */
        Graphics();

        /**
         * Destructor.
         */
        virtual ~Graphics();

        /**
         * Sets whether vertical refresh syncing is enabled. Takes effect after
         * the next call to setVideoMode(). Only implemented on MacOS for now.
         */
        void setSync(const bool sync);

        bool getSync() const
        { return mSync; }

        /**
         * Try to create a window with the given settings.
         */
        virtual bool setVideoMode(const int w, const int h, const int bpp,
                                  const bool fs, const bool hwaccel,
                                  const bool resize, const bool noFrame);

        /**
         * Set fullscreen mode.
         */
        bool setFullscreen(const bool fs);

        /**
         * Resize the window to the specified size.
         */
        bool resizeScreen(const int width, const int height);

        /**
         * Blits an image onto the screen.
         *
         * @return <code>true</code> if the image was blitted properly
         *         <code>false</code> otherwise.
         */
        bool drawImage(const Image *image, int x, int y);

        /**
         * Draws a resclaled version of the image
         */
        bool drawRescaledImage(Image *const image, int srcX, int srcY,
                               int dstX, int dstY,
                               const int width, const int height,
                               const int desiredWidth, const int desiredHeight)
        {
            return drawRescaledImage(image, srcX, srcY,
                                     dstX, dstY,
                                     width, height,
                                     desiredWidth, desiredHeight,
                                     false);
        };

        /**
         * Draws a resclaled version of the image
         */
        virtual bool drawRescaledImage(Image *const image, int srcX,
                                       int srcY, int dstX, int dstY,
                                       const int width, const int height,
                                       const int desiredWidth,
                                       const int desiredHeight,
                                       const bool useColor = false);


        virtual void drawImagePattern(const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h);

        /**
         * Draw a pattern based on a rescaled version of the given image...
         */
        virtual void drawRescaledImagePattern(const Image *const image,
                                              const int x, const int y,
                                              const int w, const int h,
                                              const int scaledWidth,
                                              const int scaledHeight);

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

        virtual bool calcImageRect(GraphicsVertexes *const vert,
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

        virtual void calcImagePattern(GraphicsVertexes *const vert,
                                      const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h);

        virtual void calcTile(ImageVertexes *const vert,
                              const Image *const image, int x, int y);

        virtual void calcTile(ImageVertexes *const vert, int x, int y);

        virtual void drawTile(ImageVertexes *const vert);

        virtual void drawImageRect2(GraphicsVertexes *const vert,
                                    const ImageRect &imgRect);

        virtual void drawImagePattern2(GraphicsVertexes *const vert,
                                       const Image *const img);

        bool calcWindow(GraphicsVertexes *const vert,
                        const int x, const int y, const int w, const int h,
                        const ImageRect &imgRect);

        /**
         * Draws a rectangle using images. 4 corner images, 4 side images and 1
         * image for the inside.
         */
        inline void drawImageRect(const gcn::Rectangle &area,
                                  const ImageRect &imgRect)
        { drawImageRect(area.x, area.y, area.width, area.height, imgRect); }

        void setBlitMode(const BlitMode mode)
        { mBlitMode = mode; }

        BlitMode getBlitMode() const
        { return mBlitMode; }

        void fillRectangle(const gcn::Rectangle& rectangle) override;

        /**
         * Updates the screen. This is done by either copying the buffer to the
         * screen or swapping pages.
         */
        virtual void updateScreen();

        /**
         * Returns the width of the screen.
         */
        int getWidth() const;

        /**
         * Returns the height of the screen.
         */
        int getHeight() const;

        /**
         * Takes a screenshot and returns it as SDL surface.
         */
        virtual SDL_Surface *getScreenshot();

        virtual void prepareScreenshot()
        { }

        int getMemoryUsage() const;

        virtual bool drawNet(const int x1, const int y1,
                             const int x2, const int y2,
                             const int width, const int height);

        gcn::Font *getFont() const
        { return mFont; }

        gcn::ClipRectangle &getTopClip()
        { return mClipStack.top(); }

        void setRedraw(const bool n)
        { mRedraw = n; }

        bool getRedraw() const
        { return mRedraw; }

        void setSecure(const bool n)
        { mSecure = n; }

        bool getSecure() const
        { return mSecure; }

        int getBpp() const
        { return mBpp; }

        bool getFullScreen() const
        { return mFullscreen; }

        bool getHWAccel() const
        { return mHWAccel; }

        bool getDoubleBuffer() const
        { return mDoubleBuffer; }

        int getOpenGL() const
        { return mOpenGL; }

        void setNoFrame(const bool n)
        { mNoFrame = n; }

        const std::string &getName() const
        { return mName; }

        virtual void initArrays()
        { }

        int mWidth;
        int mHeight;

    protected:
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
                                const bool useColor);


        void setMainFlags(int w, int h, int bpp, bool fs,
                          bool hwaccel, bool resize, bool noFrame);

        int getOpenGLFlags() const;

        int getSoftwareFlags() const;

        bool setOpenGLMode();

        void updateMemoryInfo();

        bool videoInfo();

        int SDL_FakeUpperBlit(const SDL_Surface *const src,
                              SDL_Rect *const srcrect,
                              const SDL_Surface *const dst,
                              SDL_Rect *dstrect) const;

        int mBpp;
        bool mFullscreen;
        bool mHWAccel;
        BlitMode mBlitMode;
        bool mRedraw;
        bool mDoubleBuffer;
        SDL_Rect mRect;
        bool mSecure;
        int mOpenGL;
        bool mEnableResize;
        bool mNoFrame;
        uint32_t mOldPixel;
        int mOldAlpha;
        std::string mName;
        int mStartFreeMem;
        bool mSync;
};

extern Graphics *mainGraphics;

#endif
