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

#ifndef SDL2GRAPHICS_H
#define SDL2GRAPHICS_H

#ifdef USE_SDL2

#include "graphics.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;
class MapLayer;

struct SDL_Surface;

/**
 * A central point of control for graphics.
 */
class SDLGraphics : public Graphics
{
    public:
        /**
         * Constructor.
         */
        SDLGraphics();

        A_DELETE_COPY(SDLGraphics)

        /**
         * Destructor.
         */
        virtual ~SDLGraphics();

        void _beginDraw();

        void _endDraw();

        bool pushClipArea(gcn::Rectangle rect);

        void popClipArea();

        virtual bool drawRescaledImage(const Image *const image, int srcX,
                                       int srcY, int dstX, int dstY,
                                       const int width, const int height,
                                       const int desiredWidth,
                                       const int desiredHeight,
                                       const bool useColor = false);

        virtual void drawImagePattern(const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h);

        virtual void drawRescaledImagePattern(const Image *const image,
                                              const int x, const int y,
                                              const int w, const int h,
                                              const int scaledWidth,
                                              const int scaledHeight);

        virtual void calcImagePattern(ImageVertexes *const vert,
                                      const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h) const;

        virtual void calcImagePattern(ImageCollection *const vert,
                                      const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h) const;

        virtual void calcTile(ImageVertexes *const vert,
                              const Image *const image, int x, int y) const;

        virtual void calcTileSDL(ImageVertexes *const vert,
                                 int x, int y) const;

        virtual void calcTile(ImageCollection *const vertCol,
                              const Image *const image,
                              int x, int y);

        virtual void drawTile(const ImageVertexes *const vert);

        virtual void drawTile(const ImageCollection *const vertCol);

        virtual void updateScreen();

        virtual SDL_Surface *getScreenshot() A_WARN_UNUSED;

        virtual bool drawNet(const int x1, const int y1,
                             const int x2, const int y2,
                             const int width, const int height);

        virtual bool calcWindow(ImageCollection *const vertCol,
                                const int x, const int y,
                                const int w, const int h,
                                const ImageRect &imgRect);

        void fillRectangle(const gcn::Rectangle &rect) override;

        void drawRectangle(const gcn::Rectangle &rect) override;

        void drawPoint(int x, int y) override;

        void drawLine(int x1, int y1, int x2, int y2) override;

        bool setVideoMode(const int w, const int h, const int bpp,
                          const bool fs, const bool hwaccel,
                          const bool resize, const bool noFrame);

    protected:
        virtual bool drawImage2(const Image *const image,
                                int srcX, int srcY,
                                int dstX, int dstY,
                                const int width, const int height,
                                const bool useColor);

        uint32_t mOldPixel;
        int mOldAlpha;
};

#endif  // USE_SDL2
#endif  // SDL2GRAPHICS_H
