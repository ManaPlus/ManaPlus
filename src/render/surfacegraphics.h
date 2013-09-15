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

#ifndef RENDER_SURFACEGRAPHICS_H
#define RENDER_SURFACEGRAPHICS_H

#include "render/graphics.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;

struct SDL_Surface;

/**
 * A central point of control for graphics.
 */
class SurfaceGraphics : public Graphics
{
    public:
        enum BlitMode
        {
            BLIT_NORMAL = 0,
            BLIT_GFX
        };

        SurfaceGraphics();

        A_DELETE_COPY(SurfaceGraphics)

        virtual ~SurfaceGraphics();

        void setTarget(SDL_Surface *const target)
        { mTarget = target; }

        SDL_Surface *getTarget() const
        { return mTarget; }

        void _beginDraw()
        { }

        void _endDraw()
        { }

        bool pushClipArea(gcn::Rectangle rect A_UNUSED)
        { return true; }

        void popClipArea()
        { }

        bool drawRescaledImage(const Image *const image A_UNUSED,
                               int srcX A_UNUSED, int srcY A_UNUSED,
                               int dstX A_UNUSED, int dstY A_UNUSED,
                               const int width A_UNUSED,
                               const int height A_UNUSED,
                               const int desiredWidth A_UNUSED,
                               const int desiredHeight A_UNUSED,
                               const bool useColor A_UNUSED = false)
        { return false; }

        void drawImagePattern(const Image *const image A_UNUSED,
                              const int x A_UNUSED, const int y A_UNUSED,
                              const int w A_UNUSED, const int h A_UNUSED)
        { }

        void drawRescaledImagePattern(const Image *const image A_UNUSED,
                                      const int x A_UNUSED,
                                      const int y A_UNUSED,
                                      const int w A_UNUSED,
                                      const int h A_UNUSED,
                                      const int scaledWidth A_UNUSED,
                                      const int scaledHeight A_UNUSED)
        { }

        void calcImagePattern(ImageVertexes *const vert A_UNUSED,
                              const Image *const image A_UNUSED,
                              const int x A_UNUSED,
                              const int y A_UNUSED,
                              const int w A_UNUSED,
                              const int h A_UNUSED) const
        { }

        void calcImagePattern(ImageCollection *const vert A_UNUSED,
                              const Image *const image A_UNUSED,
                              const int x A_UNUSED, const int y A_UNUSED,
                              const int w A_UNUSED, const int h A_UNUSED) const
        { }

        void calcTile(ImageVertexes *const vert A_UNUSED,
                      const Image *const image A_UNUSED,
                      int x A_UNUSED, int y A_UNUSED) const
        { }

        void calcTileSDL(ImageVertexes *const vert A_UNUSED,
                         int x A_UNUSED, int y A_UNUSED) const
        { }

        void calcTile(ImageCollection *const vertCol A_UNUSED,
                      const Image *const image A_UNUSED,
                      int x A_UNUSED, int y A_UNUSED)
        { }

        void drawTile(const ImageVertexes *const vert A_UNUSED)
        { }

        void drawTile(const ImageCollection *const vertCol A_UNUSED)
        { }

        void updateScreen()
        { }

        SDL_Surface *getScreenshot() A_WARN_UNUSED
        { return nullptr; }

        bool drawNet(const int x1 A_UNUSED, const int y1 A_UNUSED,
                     const int x2 A_UNUSED, const int y2 A_UNUSED,
                     const int width A_UNUSED, const int height A_UNUSED)
        { return false; }

        bool calcWindow(ImageCollection *const vertCol A_UNUSED,
                        const int x A_UNUSED, const int y A_UNUSED,
                        const int w A_UNUSED, const int h A_UNUSED,
                        const ImageRect &imgRect A_UNUSED)
        { return false; }

        void setBlitMode(const BlitMode mode)
        { mBlitMode = mode; }

        BlitMode getBlitMode() const A_WARN_UNUSED
        { return mBlitMode; }

        void fillRectangle(const gcn::Rectangle &rect A_UNUSED) override
        { }

        void drawRectangle(const gcn::Rectangle &rect A_UNUSED) override
        { }

        void drawPoint(int x A_UNUSED, int y A_UNUSED) override
        { }

        void drawLine(int x1 A_UNUSED, int y1 A_UNUSED,
                      int x2 A_UNUSED, int y2 A_UNUSED) override
        { }

        bool setVideoMode(const int w A_UNUSED, const int h A_UNUSED,
                          const int bpp A_UNUSED,
                          const bool fs A_UNUSED, const bool hwaccel A_UNUSED,
                          const bool resize A_UNUSED,
                          const bool noFrame A_UNUSED)
        { return false; }

    protected:
        bool drawImage2(const Image *const image,
                        int srcX, int srcY,
                        int dstX, int dstY,
                        const int width, const int height,
                        const bool useColor);

        BlitMode mBlitMode;
        SDL_Surface *mTarget;
};

#endif  // RENDER_SURFACEGRAPHICS_H
