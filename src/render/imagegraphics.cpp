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

#ifdef USE_OPENGL

#include "render/imagegraphics.h"

#include "debug.h"

ImegeGraphics::ImegeGraphics() :
    Graphics(),
    mTarget(nullptr)
{
    mOpenGL = RENDER_SOFTWARE;
    mName = "Image";
}

ImegeGraphics::~ImegeGraphics()
{
}

void ImegeGraphics::drawImage(const Image *restrict const image A_UNUSED,
                              int dstX A_UNUSED, int dstY A_UNUSED) restrict2
{
    // for now not implemented
}

void ImegeGraphics::copyImage(const Image *restrict const image,
                              int dstX A_UNUSED, int dstY A_UNUSED) restrict2
{
    if (!mTarget || !image)
        return;
}

void ImegeGraphics::drawImageCached(const Image *restrict const image,
                                    int x, int y) restrict2
{
    drawImage(image, x, y);
}

void ImegeGraphics::completeCache() restrict2
{
}

#endif  // USE_OPENGL
