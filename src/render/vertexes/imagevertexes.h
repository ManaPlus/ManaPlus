/*
 *  The ManaPlus Client
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

#ifndef RENDER_VERTEXES_IMAGEVERTEXES_H
#define RENDER_VERTEXES_IMAGEVERTEXES_H

#include "resources/rect/doublerect.h"

#ifdef USE_OPENGL
#include "render/vertexes/openglgraphicsvertexes.h"
#else  // USE_OPENGL
#include "utils/vector.h"
#endif  // USE_OPENGL

#include "localconsts.h"

class Image;

typedef STD_VECTOR<DoubleRect*> DoubleRects;

class ImageVertexes final
{
    public:
        ImageVertexes();

        A_DELETE_COPY(ImageVertexes)

        ~ImageVertexes();

        const Image *restrict image;
#ifdef USE_OPENGL
        OpenGLGraphicsVertexes ogl;
#endif  // USE_OPENGL

        DoubleRects sdl;
};

typedef STD_VECTOR<ImageVertexes*> ImageVertexesVector;
typedef ImageVertexesVector::iterator ImageCollectionIter;
typedef ImageVertexesVector::const_iterator ImageCollectionCIter;

#endif  // RENDER_VERTEXES_IMAGEVERTEXES_H
