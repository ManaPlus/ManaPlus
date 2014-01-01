/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2014  The ManaPlus Developers
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

#ifndef RENDER_RENDERERS_H
#define RENDER_RENDERERS_H

enum RenderType
{
    RENDER_SOFTWARE = 0,
    RENDER_NORMAL_OPENGL = 1,
    RENDER_SAFE_OPENGL = 2,
    RENDER_GLES_OPENGL = 3,
    RENDER_SDL2_DEFAULT = 4,
    RENDER_NULL = 5,
    RENDER_LAST
};

RenderType intToRenderType(const int mode);

#define isBatchDrawRenders(val) (val) != RENDER_SAFE_OPENGL

#endif  // RENDER_RENDERERS_H
