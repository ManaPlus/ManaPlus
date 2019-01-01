/*
 *  The ManaPlus Client
 *  Copyright (C) 2017-2019  The ManaPlus Developers
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

#ifndef RENDER_MOCKDRAWITEM_H
#define RENDER_MOCKDRAWITEM_H

#ifdef UNITTESTS

#include "unittests/enums/render/mockdrawtype.h"

#include "localconsts.h"

class Image;

struct MockDrawItem final
{
    MockDrawItem(const MockDrawTypeT type,
                 const Image *const image0,
                 const int x0,
                 const int y0,
                 const int width0,
                 const int height0) :
        image(image0),
        drawType(type),
        x(x0),
        y(y0),
        width(width0),
        height(height0)
    {
    }

    A_DEFAULT_COPY(MockDrawItem)

    const Image *image;
    MockDrawTypeT drawType;
    int x;
    int y;
    int width;
    int height;
};

#endif  // UNITTESTS
#endif  // RENDER_MOCKDRAWITEM_H
