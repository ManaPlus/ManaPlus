/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef ENUMS_RENDER_RENDERTYPE_H
#define ENUMS_RENDER_RENDERTYPE_H

enum RenderType
{
    RENDER_SOFTWARE = 0,
    RENDER_NORMAL_OPENGL = 1,
    RENDER_SAFE_OPENGL = 2,
    RENDER_GLES_OPENGL = 3,
    RENDER_SDL2_DEFAULT = 4,
    RENDER_MODERN_OPENGL = 5,
    RENDER_GLES2_OPENGL = 6,
    RENDER_NULL = 7,
    RENDER_LAST
};

#endif  // ENUMS_RENDER_RENDERTYPE_H
