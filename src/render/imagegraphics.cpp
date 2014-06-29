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

#ifdef USE_OPENGL

#include "render/imagegraphics.h"

#include "main.h"

#include "resources/image.h"

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

void ImegeGraphics::drawImage(const Image *const image A_UNUSED,
                              int dstX A_UNUSED, int dstY A_UNUSED)
{
    // for now not implimented
}

void ImegeGraphics::copyImage(const Image *const image,
                              int dstX A_UNUSED, int dstY A_UNUSED)
{
    if (!mTarget || !image)
        return;
}

void ImegeGraphics::drawImageCached(const Image *const image,
                                    int x, int y)
{
    drawImage(image, x, y);
}

void ImegeGraphics::completeCache()
{
}

#endif  // USE_OPENGL
