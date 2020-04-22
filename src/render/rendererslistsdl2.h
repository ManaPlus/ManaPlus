/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
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

#ifndef RENDER_RENDERERSLISTSDL2_H
#define RENDER_RENDERERSLISTSDL2_H

#ifdef USE_SDL2

#include "utils/gettext.h"

#include "render/renderers.h"

#ifdef USE_OPENGL
#ifdef ANDROID
// defined OPENGL ANDROID

// map for index to RenderType
const RenderType indexToRender[] =
{
    RENDER_SOFTWARE,
    RENDER_SDL2_DEFAULT,
    RENDER_GLES_OPENGL
};

const char *OPENGL_NAME[] =
{
    // TRANSLATORS: draw backend
    N_("Software"),
    // TRANSLATORS: draw backend
    N_("SDL2 default"),
    // TRANSLATORS: draw backend
    N_("Mobile OpenGL ES")
};

const int renderModesListSize = 3;

const int renderToIndex[] =
{
    0,  // RENDER_SOFTWARE
    2,  // RENDER_NORMAL_OPENGL
    2,  // RENDER_SAFE_OPENGL
    2,  // RENDER_GLES_OPENGL
    1,  // RENDER_SDL2_DEFAULT
    2,  // RENDER_MODERN_OPENGL
    2   // RENDER_GLES_OPENGL
};

#elif defined(__native_client__)
// defined OPENGL nacl

// map for index to RenderType
const RenderType indexToRender[] =
{
    RENDER_SOFTWARE,
    RENDER_SDL2_DEFAULT,
    RENDER_SAFE_OPENGL,
    RENDER_GLES2_OPENGL
};

const char *OPENGL_NAME[] =
{
    // TRANSLATORS: draw backend
    N_("Software"),
    // TRANSLATORS: draw backend
    N_("SDL2 default"),
    // TRANSLATORS: draw backend
    N_("Safe OpenGL"),
    // TRANSLATORS: draw backend
    N_("Mobile OpenGL ES 2")
};

const int renderModesListSize = 4;

const int renderToIndex[] =
{
    0,  // RENDER_SOFTWARE
    3,  // RENDER_NORMAL_OPENGL
    2,  // RENDER_SAFE_OPENGL
    3,  // RENDER_GLES_OPENGL
    1,  // RENDER_SDL2_DEFAULT
    3,  // RENDER_MODERN_OPENGL
    3   // RENDER_GLES2_OPENGL
};

#elif defined(__SWITCH__)

// map for index to RenderType
const RenderType indexToRender[] =
{
    RENDER_SOFTWARE,
    RENDER_SDL2_DEFAULT,
    RENDER_MODERN_OPENGL
};

const char *OPENGL_NAME[] =
{
    // TRANSLATORS: draw backend
    N_("Software"),
    // TRANSLATORS: draw backend
    N_("SDL2 default"),
    // TRANSLATORS: draw backend
    N_("Modern OpenGL")
};

const int renderModesListSize = 3;

const int renderToIndex[] =
{
    2,  // RENDER_SOFTWARE
    2,  // RENDER_NORMAL_OPENGL
    2,  // RENDER_SAFE_OPENGL
    2,  // RENDER_GLES_OPENGL
    1,  // RENDER_SDL2_DEFAULT
    2,  // RENDER_MODERN_OPENGL
    2   // RENDER_GLES2_OPENGL
};

#else  // ANDROID or nacl
// defined OPENGL

// map for index to RenderType
const RenderType indexToRender[] =
{
    RENDER_SOFTWARE,
    RENDER_SDL2_DEFAULT,
    RENDER_NORMAL_OPENGL,
    RENDER_SAFE_OPENGL,
    RENDER_GLES_OPENGL,
    RENDER_MODERN_OPENGL,
    RENDER_GLES2_OPENGL
};

const char *OPENGL_NAME[] =
{
    // TRANSLATORS: draw backend
    N_("Software"),
    // TRANSLATORS: draw backend
    N_("SDL2 default"),
    // TRANSLATORS: draw backend
    N_("Normal OpenGL"),
    // TRANSLATORS: draw backend
    N_("Safe OpenGL"),
    // TRANSLATORS: draw backend
    N_("Mobile OpenGL ES"),
    // TRANSLATORS: draw backend
    N_("Modern OpenGL"),
    // TRANSLATORS: draw backend
    N_("Mobile OpenGL ES 2")
};

const int renderModesListSize = 7;

const int renderToIndex[] =
{
    0,  // RENDER_SOFTWARE
    2,  // RENDER_NORMAL_OPENGL
    3,  // RENDER_SAFE_OPENGL
    4,  // RENDER_GLES_OPENGL
    1,  // RENDER_SDL2_DEFAULT
    5,  // RENDER_MODERN_OPENGL
    6   // RENDER_GLES2_OPENGL
};

#endif  // ANDROID

#else  // USE_OPENGL
// no defines

// map for index to RenderType
const RenderType indexToRender[] =
{
    RENDER_SOFTWARE,
    RENDER_SDL2_DEFAULT
};

const char *OPENGL_NAME[] =
{
    // TRANSLATORS: draw backend
    N_("Software"),
    // TRANSLATORS: draw backend
    N_("SDL2 default")
};

const int renderModesListSize = 2;

const int renderToIndex[] =
{
    0,  // RENDER_SOFTWARE
    0,  // RENDER_NORMAL_OPENGL
    0,  // RENDER_SAFE_OPENGL
    0,  // RENDER_GLES_OPENGL
    1,  // RENDER_SDL2_DEFAULT
    0,  // RENDER_MODERN_OPENGL
    0   // RENDER_GLES2_OPENGL
};

#endif  // USE_OPENGL

#endif  // USE_SDL2
#endif  // RENDER_RENDERERSLISTSDL2_H
