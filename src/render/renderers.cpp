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

#include "render/renderers.h"

static RenderType getDefault()
{
#ifdef USE_OPENGL
#ifdef ANDROID
    return RENDER_GLES_OPENGL;
#else
    return RENDER_NORMAL_OPENGL;
#endif
#else
    return RENDER_SOFTWARE;
#endif
}

RenderType intToRenderType(const int mode)
{
    if (mode < 0 || mode >= RENDER_LAST)
        return getDefault();

    if (mode != RENDER_SOFTWARE
#if defined(USE_OPENGL) && defined(ANDROID) && defined(USE_SDL2)
        && mode != RENDER_GLES_OPENGL && mode != RENDER_SDL2_DEFAULT)
#elif defined(USE_OPENGL) && defined(ANDROID) && !defined(USE_SDL2)
        && mode != RENDER_GLES_OPENGL)
#elif defined(USE_OPENGL) && !defined(ANDROID) && defined(USE_SDL2)
        && mode != RENDER_NORMAL_OPENGL && mode != RENDER_SAFE_OPENGL
        && mode != RENDER_GLES_OPENGL && mode != RENDER_SDL2_DEFAULT)
#elif !defined(USE_OPENGL) && defined(USE_SDL2)
        && mode != RENDER_SDL2_DEFAULT)
#elif !defined(USE_OPENGL) && !defined(USE_SDL2)
        )
#elif defined(USE_OPENGL) && !defined(ANDROID) && !defined(USE_SDL2)
        && mode != RENDER_NORMAL_OPENGL && mode != RENDER_SAFE_OPENGL
        && mode != RENDER_GLES_OPENGL)
#endif
    {
        return getDefault();
    }

    return static_cast<RenderType>(mode);
}
