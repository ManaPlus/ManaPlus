/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "debug.h"

static RenderType getDefault() noexcept2 A_CONST;

static RenderType getDefault() noexcept2
{
#ifdef USE_OPENGL
#ifdef ANDROID
    return RENDER_GLES_OPENGL;
#else  // ANDROID

    return RENDER_NORMAL_OPENGL;
#endif  // ANDROID
#else  // USE_OPENGL

    return RENDER_SOFTWARE;
#endif  // USE_OPENGL
}

RenderType intToRenderType(const int mode) noexcept2
{
    if (mode < 0 || mode >= RENDER_LAST)
        return getDefault();

    if (mode != RENDER_SOFTWARE
#if defined(USE_OPENGL)

// with OpenGL start
#if defined(ANDROID)

// with OpenGL + with ANDROID start
#if defined(USE_SDL2)
        && mode != RENDER_GLES_OPENGL
        && mode != RENDER_SDL2_DEFAULT)
#else  // defined(USE_SDL2)

        && mode != RENDER_GLES_OPENGL)
#endif  // defined(USE_SDL2)
// with OpenGL + with ANDROID end

#elif defined(__native_client__)

// with OpenGL + with nacl start
#if defined(USE_SDL2)
        && mode != RENDER_SAFE_OPENGL
        && mode != RENDER_GLES2_OPENGL
        && mode != RENDER_SDL2_DEFAULT)
#else  //  defined(USE_SDL2)

        && mode != RENDER_SAFE_OPENGL
        && mode != RENDER_GLES2_OPENGL)
#endif  // defined(USE_SDL2)
// with OpenGL + with nacl end

#else  // defined(ANDROID)

// with OpenGL + without ANDROID start
#if defined(USE_SDL2)
        && mode != RENDER_NORMAL_OPENGL
        && mode != RENDER_MODERN_OPENGL
        && mode != RENDER_SAFE_OPENGL
        && mode != RENDER_GLES_OPENGL
        && mode != RENDER_GLES2_OPENGL
        && mode != RENDER_SDL2_DEFAULT)
#else  //  defined(USE_SDL2)

        && mode != RENDER_NORMAL_OPENGL
        && mode != RENDER_MODERN_OPENGL
        && mode != RENDER_SAFE_OPENGL
        && mode != RENDER_GLES_OPENGL
        && mode != RENDER_GLES2_OPENGL)
#endif  // defined(USE_SDL2)
// with OpenGL + without ANDROID end

#endif  // defined(ANDROID)
// with OpenGL end

#else  // defined(USE_OPENGL)

// without OpenGL start
#if defined(USE_SDL2)
        && mode != RENDER_SDL2_DEFAULT)
#elif !defined(USE_SDL2)
        )
#endif  // defined(USE_SDL2)
// without OpenGL end

#endif  // defined(USE_OPENGL)

    {
        return getDefault();
    }

    return static_cast<RenderType>(mode);
}
