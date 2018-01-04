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

#include "render/vertexes/imagevertexes.h"

#include "utils/dtor.h"

#include "debug.h"

ImageVertexes::ImageVertexes() :
    image(nullptr),
#ifdef USE_OPENGL
    ogl(),
#endif  // USE_OPENGL
    sdl()
{
    sdl.reserve(30);
}

ImageVertexes::~ImageVertexes()
{
    delete_all(sdl);
    sdl.clear();
}
