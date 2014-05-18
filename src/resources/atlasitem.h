/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef RESOURCES_ATLASITEM_H
#define RESOURCES_ATLASITEM_H

#ifdef USE_OPENGL

#include "resources/image.h"

#include <string>

struct AtlasItem final
{
    explicit AtlasItem(Image *const image0) :
        image(image0),
        name(),
        x(0),
        y(0),
        width(image0->mBounds.w),
        height(image0->mBounds.h)
    {
    }

    A_DELETE_COPY(AtlasItem)

    Image *image;
    std::string name;
    int x;
    int y;
    int width;
    int height;
};

#endif  // USE_OPENGL
#endif  // RESOURCES_ATLASITEM_H
