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


// bool GraphicsX::calcImageRect(ImageVertexes *const vert,
//                               const int x, const int y,
//                               const int w, const int h,
//                               const ImageRect &imgRect)

if (!vert)
    return;

BLOCK_START("Graphics::calcImageRect")

const Image *const *const grid = imgRect.grid;
const Image *const topLeft = grid[0];
const Image *const topRight = grid[2];
const Image *const bottomLeft = grid[6];
const Image *const bottomRight = grid[8];
const Image *const top = grid[1];
const Image *const right = grid[5];
const Image *const bottom = grid[7];
const Image *const left = grid[3];
const Image *const center = grid[4];

const bool drawMain = center && topLeft && topRight
    && bottomLeft && bottomRight;

// Draw the center area
if (center && drawMain)
{
    const int tlw = topLeft->getWidth();
    const int tlh = topLeft->getHeight();
    calcPatternInline(vert, center, tlw + x, tlh + y,
        w - tlw - topRight->getWidth(),
        h - tlh - bottomLeft->getHeight());
}
// Draw the sides
if (top && left && bottom && right)
{
    const int lw = left->getWidth();
    const int rw = right->getWidth();
    const int th = top->getHeight();
    const int bh = bottom->getHeight();
    calcPatternInline(vert, top, x + lw, y, w - lw - rw, th);
    calcPatternInline(vert, bottom, x + lw, y + h - bh, w - lw - rw, bh);
    calcPatternInline(vert, left, x, y + th, lw, h - th - bh);
    if (w > rw)
        calcPatternInline(vert, right, x + w - rw, y + th, rw, h - th - bh);
}

calcTileVertexesInline(vert, topLeft, x, y);
if (topRight)
{
    const int trw = topRight->getWidth();
    if (w > trw)
        calcTileVertexesInline(vert, topRight, x + w - trw, y);
}
if (bottomLeft)
{
    calcTileVertexesInline(vert, bottomLeft,
        x, y + h - bottomLeft->getHeight());
}
if (bottomRight)
{
    const int brw = bottomRight->getWidth();
    if (w > brw)
    {
        calcTileVertexesInline(vert, bottomRight, x + w - brw,
            y + h - bottomRight->getHeight());
    }
}

BLOCK_END("Graphics::calcImageRect")
