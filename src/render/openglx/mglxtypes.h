/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef RENDER_OPENGLX_MGLXTYPES_H
#define RENDER_OPENGLX_MGLXTYPES_H

#if defined(USE_OPENGL) && defined(USE_X11)

#include <GL/glx.h>

typedef void *(*glXCreateContext_t) (Display *dpy, XVisualInfo *vis,
    void *shareList, bool direct);
typedef void *(*glXGetCurrentContext_t) (void);
typedef void *(*glXCreateContextAttribs_t) (Display *dpy, GLXFBConfig config,
    void *share_context, bool direct, const int *attrib_list);
typedef GLXFBConfig *(*glXChooseFBConfig_t) (Display *dpy, int screen,
    const int *attrib_list, int *nelements);
typedef void (*glXDestroyContext_t) (Display *dpy, void *ctx);
typedef bool (*glXMakeCurrent_t) (Display *dpy,
    GLXDrawable drawable, void *ctx);
typedef void (*glXSwapBuffers_t) (Display *dpy, GLXDrawable drawable);

#endif  // USE_OPENGL
#endif  // RENDER_OPENGLX_MGLXTYPES_H
