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

#ifndef RENDER_SDL2SOFTWAREGRAPHICS_H
#define RENDER_SDL2SOFTWAREGRAPHICS_H

#ifdef USE_SDL2

#include "render/graphics.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;
class MapLayer;

struct SDL_Surface;

/**
 * A central point of control for graphics.
 */
class SDL2SoftwareGraphics final : public Graphics
{
    public:
        /**
         * Constructor.
         */
        SDL2SoftwareGraphics();

        A_DELETE_COPY(SDL2SoftwareGraphics)

        /**
         * Destructor.
         */
        ~SDL2SoftwareGraphics();

        void _beginDraw();

        void _endDraw();

        bool pushClipArea(gcn::Rectangle rect);

        void popClipArea();

        bool drawRescaledImage(const Image *const image,
                               int srcX, int srcY,
                               int dstX, int dstY,
                               const int width, const int height,
                               const int desiredWidth,
                               const int desiredHeight,
                               const bool useColor = false) override final;

        void drawImagePattern(const Image *const image,
                              const int x, const int y,
                              const int w, const int h) override final;

        void drawRescaledImagePattern(const Image *const image,
                                      const int x, const int y,
                                      const int w, const int h,
                                      const int scaledWidth,
                                      const int scaledHeight) override final;

        void calcImagePattern(ImageVertexes *const vert,
                              const Image *const image,
                              const int x, const int y,
                              const int w, const int h) const override final;

        void calcImagePattern(ImageCollection *const vert,
                              const Image *const image,
                              const int x, const int y,
                              const int w, const int h) const override final;

        void calcTileVertexes(ImageVertexes *const vert,
                              const Image *const image,
                              int x, int y) const override final;

        void calcTileSDL(ImageVertexes *const vert,
                         int x, int y) const override final;

        void calcTileCollection(ImageCollection *const vertCol,
                                const Image *const image,
                                int x, int y) override final;

        void drawTileVertexes(const ImageVertexes *const vert) override final;

        void drawTileCollection(const ImageCollection *const vertCol)
                                override final;

        void updateScreen() override final;

        SDL_Surface *getScreenshot() override final A_WARN_UNUSED;

        bool drawNet(const int x1, const int y1,
                     const int x2, const int y2,
                     const int width, const int height) override final;

        bool calcWindow(ImageCollection *const vertCol,
                        const int x, const int y,
                        const int w, const int h,
                        const ImageRect &imgRect) override final;

        void fillRectangle(const gcn::Rectangle &rect) override final;

        void drawRectangle(const gcn::Rectangle &rect) override final;

        void drawPoint(int x, int y) override final;

        void drawLine(int x1, int y1, int x2, int y2) override final;

        bool setVideoMode(const int w, const int h,
                          const int bpp,
                          const bool fs,
                          const bool hwaccel,
                          const bool resize,
                          const bool noFrame) override final;

        void setRendererFlags(const uint32_t flags) override final
        { mRendererFlags = flags; }

        bool resizeScreen(const int width, const int height) override final;

        bool drawImage2(const Image *const image,
                        int srcX, int srcY,
                        int dstX, int dstY,
                        const int width, const int height,
                        const bool useColor) override final;

    protected:
        int SDL_FakeUpperBlit(const SDL_Surface *const src,
                              SDL_Rect *const srcrect,
                              const SDL_Surface *const dst,
                              SDL_Rect *dstrect) const;

        void drawHLine(int x1, int y, int x2);

        void drawVLine(int x, int y1, int y2);

        uint32_t mRendererFlags;
        SDL_Surface *mSurface;
        uint32_t mOldPixel;
        int mOldAlpha;
};

#endif  // USE_SDL2
#endif  // RENDER_SDL2SOFTWAREGRAPHICS_H
