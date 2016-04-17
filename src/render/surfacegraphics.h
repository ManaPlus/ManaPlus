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

#ifndef RENDER_SURFACEGRAPHICS_H
#define RENDER_SURFACEGRAPHICS_H

#include "enums/render/blitmode.h"

#include "render/graphics.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;

struct SDL_Surface;

/**
 * A central point of control for graphics.
 */
class SurfaceGraphics final : public Graphics
{
    public:
        SurfaceGraphics();

        A_DELETE_COPY(SurfaceGraphics)

        ~SurfaceGraphics();

        void setTarget(SDL_Surface *restrict const target) restrict2 noexcept
        { mTarget = target; }

        SDL_Surface *getTarget() const restrict2 noexcept
        { return mTarget; }

        void beginDraw() restrict2 final
        { }

        void endDraw() restrict2 final
        { }

        void pushClipArea(const Rect &restrict rect A_UNUSED)
                          restrict2 final
        { }

        void popClipArea() restrict2 final
        { }

        void drawRescaledImage(const Image *restrict const image A_UNUSED,
                               int dstX A_UNUSED, int dstY A_UNUSED,
                               const int desiredWidth A_UNUSED,
                               const int desiredHeight A_UNUSED)
                               restrict2 final
        { }

        void drawPattern(const Image *restrict const image A_UNUSED,
                         const int x A_UNUSED,
                         const int y A_UNUSED,
                         const int w A_UNUSED,
                         const int h A_UNUSED) restrict2 final
        { }

        void drawRescaledPattern(const Image *const image A_UNUSED,
                                 const int x A_UNUSED,
                                 const int y A_UNUSED,
                                 const int w A_UNUSED,
                                 const int h A_UNUSED,
                                 const int scaledWidth A_UNUSED,
                                 const int scaledHeight A_UNUSED)
                                 final
        { }

        void calcPattern(ImageVertexes *restrict const vert A_UNUSED,
                         const Image *restrict const image A_UNUSED,
                         const int x A_UNUSED,
                         const int y A_UNUSED,
                         const int w A_UNUSED,
                         const int h A_UNUSED) const restrict2 final
        { }

        void calcPattern(ImageCollection *restrict const vert A_UNUSED,
                         const Image *restrict const image A_UNUSED,
                         const int x A_UNUSED,
                         const int y A_UNUSED,
                         const int w A_UNUSED,
                         const int h A_UNUSED) const restrict2 final
        { }

        void calcTileVertexes(ImageVertexes *restrict const vert A_UNUSED,
                              const Image *restrict const image A_UNUSED,
                              int x A_UNUSED,
                              int y A_UNUSED) const restrict2 final
                              A_NONNULL(2, 3)
        { }

        void calcTileSDL(ImageVertexes *restrict const vert A_UNUSED,
                         int x A_UNUSED,
                         int y A_UNUSED) const restrict2 final
        { }

        void calcTileCollection(ImageCollection *restrict const
                                vertCol A_UNUSED,
                                const Image *restrict const image A_UNUSED,
                                int x A_UNUSED,
                                int y A_UNUSED) restrict2 final
        { }

        void drawTileVertexes(const ImageVertexes *restrict const
                              vert A_UNUSED) restrict2 final
        { }

        void drawTileCollection(const ImageCollection *restrict const
                                vertCol A_UNUSED)
                                restrict2 final A_NONNULL(2)
        { }

        void updateScreen() final
        { }

        void drawNet(const int x1 A_UNUSED,
                     const int y1 A_UNUSED,
                     const int x2 A_UNUSED,
                     const int y2 A_UNUSED,
                     const int width A_UNUSED,
                     const int height A_UNUSED) restrict2 final
        { }

        void calcWindow(ImageCollection *restrict const vertCol A_UNUSED,
                        const int x A_UNUSED, const int y A_UNUSED,
                        const int w A_UNUSED, const int h A_UNUSED,
                        const ImageRect &restrict imgRect A_UNUSED)
                        restrict2 final A_NONNULL(2)
        { }

        void setBlitMode(const BlitModeT mode) restrict2 noexcept
        { mBlitMode = mode; }

        BlitModeT getBlitMode() const restrict2 noexcept A_WARN_UNUSED
        { return mBlitMode; }

        void fillRectangle(const Rect &restrict rect A_UNUSED)
                           restrict2 final
        { }

        void drawRectangle(const Rect &restrict rect A_UNUSED)
                           restrict2 final
        { }

        void drawPoint(int x A_UNUSED, int y A_UNUSED) restrict2 final
        { }

        void drawLine(int x1 A_UNUSED, int y1 A_UNUSED,
                      int x2 A_UNUSED, int y2 A_UNUSED)
                      restrict2 final
        { }

        bool setVideoMode(const int w A_UNUSED, const int h A_UNUSED,
                          const int scale A_UNUSED,
                          const int bpp A_UNUSED,
                          const bool fs A_UNUSED, const bool hwaccel A_UNUSED,
                          const bool resize A_UNUSED,
                          const bool noFrame A_UNUSED) restrict2 final
        { return false; }

        void drawImage(const Image *restrict const image,
                       int dstX, int dstY) restrict2 final;

        void copyImage(const Image *restrict const image,
                       int dstX, int dstY) restrict2 final;

        void drawImageCached(const Image *restrict const image,
                             int x, int y) restrict2 final;

        void drawPatternCached(const Image *restrict const image A_UNUSED,
                               const int x A_UNUSED,
                               const int y A_UNUSED,
                               const int w A_UNUSED,
                               const int h A_UNUSED) restrict2 final
        { }

        void completeCache() restrict2 final;

        /**
         * Draws a rectangle using images. 4 corner images, 4 side images and 1
         * image for the inside.
         */
        void drawImageRect(const int x A_UNUSED, const int y A_UNUSED,
                           const int w A_UNUSED, const int h A_UNUSED,
                           const ImageRect &restrict imgRect A_UNUSED)
                           restrict2 final
        { }

    protected:
        BlitModeT mBlitMode;
        SDL_Surface *mTarget;
};

#endif  // RENDER_SURFACEGRAPHICS_H
