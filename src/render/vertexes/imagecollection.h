/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef RENDER_VERTEXES_IMAGECOLLECTION_H
#define RENDER_VERTEXES_IMAGECOLLECTION_H

#include "render/vertexes/imagevertexes.h"

#include "localconsts.h"

class Image;

class ImageCollection final
{
    public:
        ImageCollection();

        A_DELETE_COPY(ImageCollection)

        ~ImageCollection();

        void clear() restrict2;

#ifdef USE_OPENGL
        GLuint currentGLImage;
#endif  // USE_OPENGL

        const Image *restrict currentImage;

        ImageVertexes *restrict currentVert;

        ImageVertexesVector draws;
};

#endif  // RENDER_VERTEXES_IMAGECOLLECTION_H
