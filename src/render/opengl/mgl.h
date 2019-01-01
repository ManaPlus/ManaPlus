/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef RENDER_OPENGL_MGL_H
#define RENDER_OPENGL_MGL_H

#ifdef USE_OPENGL

#include "render/opengl/mgltypes.h"

#define defName(name) extern name##_t m##name

#include "render/opengl/mgl.hpp"

MGL_DEFINE

#undef defName
#undef MGL_DEFINE

#endif  // USE_OPENGL
#endif  // RENDER_OPENGL_MGL_H
