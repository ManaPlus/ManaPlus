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

public:
    /**
    * Draws a rectangle using images. 4 corner images, 4 side images and 1
    * image for the inside.
    */
    void drawImageRect(int x, int y,
                       int w, int h,
                       const ImageRect &imgRect);

    bool drawNet(const int x1, const int y1,
                 const int x2, const int y2,
                 const int width, const int height) override final;

    void beginDraw();

    void endDraw();

    bool pushClipArea(Rect area) override final;

    void popClipArea() override final;

    /**
    * Draws a resclaled version of the image
    */
    bool drawRescaledImage(const Image *const image,
                           int dstX, int dstY,
                           const int desiredWidth,
                           const int desiredHeight) override final;

    void drawPattern(const Image *const image,
                     const int x, const int y,
                     const int w, const int h) override final;

    void inline drawPatternInline(const Image *const image,
                                  const int x, const int y,
                                  const int w, const int h);

    void drawRescaledPattern(const Image *const image,
                             const int x, const int y,
                             const int w, const int h,
                             const int scaledWidth,
                             const int scaledHeight) override final;

    void calcPattern(ImageVertexes *const vert,
                     const Image *const image,
                     const int x, const int y,
                     const int w, const int h) const override final;

    void calcPattern(ImageCollection *const vert,
                     const Image *const image,
                     const int x, const int y,
                     const int w, const int h) const override final;

    void calcTileVertexes(ImageVertexes *const vert,
                          const Image *const image,
                          int x, int y) const override final;

    void calcTileCollection(ImageCollection *const vertCol,
                            const Image *const image,
                            int x, int y) override final;

    void drawTileVertexes(const ImageVertexes *const vert) override final;

    void drawTileCollection(const ImageCollection
                            *const vertCol) override final;

    void updateScreen() override final;

    /**
    * Takes a screenshot and returns it as SDL surface.
    */
    SDL_Surface *getScreenshot() override final A_WARN_UNUSED;

    void calcWindow(ImageCollection *const vertCol,
                    const int x, const int y,
                    const int w, const int h,
                    const ImageRect &imgRect) override final;

    void drawRectangle(const Rect &rect) override final;

    void fillRectangle(const Rect &rect) override final;

    void drawPoint(int x, int y) override final;

    void drawLine(int x1, int y1,
                  int x2, int y2) override final;

    bool setVideoMode(const int w, const int h,
                      const int scalle,
                      const int bpp,
                      const bool fs,
                      const bool hwaccel,
                      const bool resize,
                      const bool noFrame) override final;

    bool drawImage(const Image *const image,
                   int dstX, int dstY) override final;

    void drawImageCached(const Image *const image,
                         int x, int y) override final;

    void drawPatternCached(const Image *const image,
                           const int x, const int y,
                           const int w, const int h) override final;

    void completeCache() override final;

private:
    void inline calcImageRect(ImageVertexes *const vert,
                              int x, int y,
                              int w, int h,
                              const ImageRect &imgRect);

    void inline calcPatternInline(ImageVertexes *const vert,
                                  const Image *const image,
                                  const int x, const int y,
                                  const int w, const int h) const;

    void inline calcTileVertexesInline(ImageVertexes *const vert,
                                       const Image *const image,
                                       int x, int y) const;

    bool inline drawImageInline(const Image *const image,
                                int dstX, int dstY);
