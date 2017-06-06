/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "render/mockgraphics.h"

#include "graphicsmanager.h"

#include "utils/sdlcheckutils.h"

#include "render/vertexes/imagecollection.h"

#include "debug.h"

MockGraphics::MockGraphics() :
    Graphics()
{
    mOpenGL = RENDER_SOFTWARE;
    mName = "Software";
}

MockGraphics::~MockGraphics()
{
}

void MockGraphics::drawRescaledImage(const Image *restrict const image
                                     A_UNUSED,
                                     int dstX A_UNUSED,
                                     int dstY A_UNUSED,
                                     const int desiredWidth A_UNUSED,
                                     const int desiredHeight A_UNUSED)
                                     restrict2
{
}

void MockGraphics::drawImage(const Image *restrict const image,
                             int dstX,
                             int dstY) restrict2
{
    mDraws.push_back(MockDrawItem(MockDrawType::DrawImage,
        image,
        dstX,
        dstY,
        0,
        0));
}

void MockGraphics::drawImageInline(const Image *restrict const image A_UNUSED,
                                   int dstX A_UNUSED,
                                   int dstY A_UNUSED) restrict2
{
}

void MockGraphics::copyImage(const Image *restrict const image A_UNUSED,
                             int dstX A_UNUSED,
                             int dstY A_UNUSED) restrict2
{
}

void MockGraphics::drawImageCached(const Image *restrict const image A_UNUSED,
                                   int x A_UNUSED,
                                   int y A_UNUSED) restrict2
{
}

void MockGraphics::drawPatternCached(const Image *restrict const image
                                     A_UNUSED,
                                     const int x A_UNUSED,
                                     const int y A_UNUSED,
                                     const int w A_UNUSED,
                                     const int h A_UNUSED) restrict2
{
}

void MockGraphics::completeCache() restrict2
{
}

void MockGraphics::drawPattern(const Image *restrict const image,
                               const int x,
                               const int y,
                               const int w,
                               const int h) restrict2
{
    mDraws.push_back(MockDrawItem(MockDrawType::DrawPattern,
        image,
        x,
        y,
        w,
        h));
}

void MockGraphics::drawPatternInline(const Image *restrict const image
                                     A_UNUSED,
                                     const int x A_UNUSED,
                                     const int y A_UNUSED,
                                     const int w A_UNUSED,
                                     const int h A_UNUSED) restrict2
{
}

void MockGraphics::drawRescaledPattern(const Image *restrict const image
                                       A_UNUSED,
                                       const int x A_UNUSED,
                                       const int y A_UNUSED,
                                       const int w A_UNUSED,
                                       const int h A_UNUSED,
                                       const int scaledWidth A_UNUSED,
                                       const int scaledHeight A_UNUSED)
                                       restrict2
{
}

void MockGraphics::calcPattern(ImageVertexes *restrict const vert A_UNUSED,
                               const Image *restrict const image A_UNUSED,
                               const int x A_UNUSED,
                               const int y A_UNUSED,
                               const int w A_UNUSED,
                               const int h A_UNUSED) const restrict2
{
}

void MockGraphics::calcPatternInline(ImageVertexes *restrict const vert
                                     A_UNUSED,
                                     const Image *restrict const image
                                     A_UNUSED,
                                     const int x A_UNUSED,
                                     const int y A_UNUSED,
                                     const int w A_UNUSED,
                                     const int h A_UNUSED) const restrict2
{
}

void MockGraphics::calcPattern(ImageCollection *restrict const vertCol
                               A_UNUSED,
                               const Image *restrict const image A_UNUSED,
                               const int x A_UNUSED,
                               const int y A_UNUSED,
                               const int w A_UNUSED,
                               const int h A_UNUSED) const restrict2
{
}

void MockGraphics::calcTileVertexes(ImageVertexes *restrict const vert,
                                   const Image *restrict const image,
                                   int x, int y) const restrict2
{
    vert->image = image;
    calcTileSDL(vert, x, y);
}

void MockGraphics::calcTileVertexesInline(ImageVertexes *restrict const vert
                                          A_UNUSED,
                                          const Image *restrict const image
                                          A_UNUSED,
                                          int x A_UNUSED,
                                          int y A_UNUSED) const restrict2
{
}

void MockGraphics::calcTileSDL(ImageVertexes *restrict const vert A_UNUSED,
                               int x A_UNUSED,
                               int y A_UNUSED) const restrict2
{
}

void MockGraphics::calcTileCollection(ImageCollection *restrict const vertCol
                                      A_UNUSED,
                                      const Image *restrict const image
                                      A_UNUSED,
                                      int x A_UNUSED,
                                      int y A_UNUSED) restrict2
{
}

void MockGraphics::drawTileCollection(const ImageCollection *restrict const
                                      vertCol A_UNUSED) restrict2
{
}

void MockGraphics::drawTileVertexes(const ImageVertexes *restrict const
                                    vert A_UNUSED) restrict2
{
}

void MockGraphics::updateScreen() restrict2
{
}

void MockGraphics::calcWindow(ImageCollection *restrict const vertCol A_UNUSED,
                              const int x A_UNUSED,
                              const int y A_UNUSED,
                              const int w A_UNUSED,
                              const int h A_UNUSED,
                              const ImageRect &restrict imgRect A_UNUSED)
                              restrict2
{
}

void MockGraphics::fillRectangle(const Rect &restrict rectangle A_UNUSED)
                                 restrict2
{
}

void MockGraphics::beginDraw() restrict2
{
    pushClipArea(Rect(0, 0, mRect.w, mRect.h));
}

void MockGraphics::endDraw() restrict2
{
    popClipArea();
}

void MockGraphics::pushClipArea(const Rect &restrict area) restrict2
{
    Graphics::pushClipArea(area);
}

void MockGraphics::popClipArea() restrict2
{
    Graphics::popClipArea();
}

void MockGraphics::drawPoint(int x A_UNUSED,
                             int y A_UNUSED) restrict2
{
}

void MockGraphics::drawRectangle(const Rect &restrict rectangle A_UNUSED)
                                 restrict2
{
}

void MockGraphics::drawLine(int x1 A_UNUSED,
                            int y1 A_UNUSED,
                            int x2 A_UNUSED,
                            int y2 A_UNUSED) restrict2
{
}

bool MockGraphics::setVideoMode(const int w, const int h,
                               const int scale,
                               const int bpp,
                               const bool fs,
                               const bool hwaccel,
                               const bool resize,
                               const bool noFrame) restrict2
{
    setMainFlags(w, h, scale, bpp, fs, hwaccel, resize, noFrame);

    if ((mWindow = graphicsManager.createWindow(w, h, bpp,
        getSoftwareFlags())) == nullptr)
    {
        mRect.w = 0;
        mRect.h = 0;
        return false;
    }

    mRect.w = CAST_U16(mRect.w);
    mRect.h = CAST_U16(mRect.h);

    return videoInfo();
}

void MockGraphics::drawImageRect(const int x A_UNUSED,
                                 const int y A_UNUSED,
                                 const int w A_UNUSED,
                                 const int h A_UNUSED,
                                 const ImageRect &restrict imgRect A_UNUSED)
                                 restrict2
{
}

void MockGraphics::calcImageRect(ImageVertexes *restrict const vert A_UNUSED,
                                 const int x A_UNUSED,
                                 const int y A_UNUSED,
                                 const int w A_UNUSED,
                                 const int h A_UNUSED,
                                 const ImageRect &restrict imgRect A_UNUSED)
                                 restrict2
{
}
