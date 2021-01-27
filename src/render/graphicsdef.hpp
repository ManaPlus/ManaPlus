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

#define RENDER_GRAPHICSDEF_HPP

public:
    /**
    * Draws a rectangle using images. 4 corner images, 4 side images and 1
    * image for the inside.
    */
    void drawImageRect(int x, int y,
                       int w, int h,
                       const ImageRect &restrict imgRect)
                       restrict2 override final;

    void beginDraw() restrict2 override final;

    void endDraw() restrict2 override final;

    void pushClipArea(const Rect &restrict area) restrict2 override final;

    void popClipArea() restrict2 override final;

    /**
    * Draws a resclaled version of the image
    */
    void drawRescaledImage(const Image *restrict const image,
                           int dstX, int dstY,
                           const int desiredWidth,
                           const int desiredHeight) restrict2 override final;

    void drawPattern(const Image *restrict const image,
                     const int x, const int y,
                     const int w, const int h) restrict2 override final;

    void inline drawPatternInline(const Image *restrict const image,
                                  const int x, const int y,
                                  const int w, const int h) restrict2 A_INLINE;

    void drawRescaledPattern(const Image *restrict const image,
                             const int x, const int y,
                             const int w, const int h,
                             const int scaledWidth,
                             const int scaledHeight) restrict2 override final;

    void calcPattern(ImageVertexes *restrict const vert,
                     const Image *restrict const image,
                     const int x, const int y,
                     const int w, const int h) const restrict2 override final;

    void calcPattern(ImageCollection *restrict const vert,
                     const Image *restrict const image,
                     const int x, const int y,
                     const int w, const int h) const restrict2 override final;

    void calcTileVertexes(ImageVertexes *restrict const vert,
                          const Image *restrict const image,
                          int x, int y) const restrict2 override final
                          A_NONNULL(2, 3);

    void calcTileCollection(ImageCollection *restrict const vertCol,
                            const Image *restrict const image,
                            int x, int y) restrict2 override final;

    void drawTileVertexes(const ImageVertexes *restrict const vert)
                          restrict2 override final;

    void drawTileCollection(const ImageCollection
                            *restrict const vertCol) restrict2 override final
                            A_NONNULL(2);

    void updateScreen() restrict2 override final;

    void calcWindow(ImageCollection *restrict const vertCol,
                    const int x, const int y,
                    const int w, const int h,
                    const ImageRect &restrict imgRect)
                    restrict2 override final A_NONNULL(2);

    void drawRectangle(const Rect &restrict rect) restrict2 override final;

    void fillRectangle(const Rect &restrict rect) restrict2 override final;

    void drawPoint(int x, int y) restrict2 override final;

    void drawLine(int x1, int y1,
                  int x2, int y2) restrict2 override final;

    bool setVideoMode(const int w, const int h,
                      const int scalle,
                      const int bpp,
                      const bool fs,
                      const bool hwaccel,
                      const bool resize,
                      const bool noFrame,
                      const bool allowHighDPI) restrict2 override final;

    void drawImage(const Image *restrict const image,
                   int dstX, int dstY) restrict2 override final;

    void copyImage(const Image *restrict const image,
                   int dstX, int dstY) restrict2 override final;

    void drawImageCached(const Image *restrict const image,
                         int x, int y) restrict2 override final;

    void drawPatternCached(const Image *restrict const image,
                           const int x, const int y,
                           const int w, const int h) restrict2 override final;

    void completeCache() restrict2 override final;

private:
    void inline calcImageRect(ImageVertexes *restrict const vert,
                              int x, int y,
                              int w, int h,
                              const ImageRect &restrict imgRect)
                              restrict2 A_INLINE;

    void inline calcPatternInline(ImageVertexes *restrict const vert,
                                  const Image *restrict const image,
                                  const int x,
                                  const int y,
                                  const int w,
                                  const int h) const restrict2 A_INLINE;

    void inline calcTileVertexesInline(ImageVertexes *restrict const vert,
                                       const Image *restrict const image,
                                       int x,
                                       int y) const restrict2
                                       A_INLINE A_NONNULL(2, 3);

    void inline drawImageInline(const Image *restrict const image,
                                int dstX,
                                int dstY) restrict2 A_INLINE;
