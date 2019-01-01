/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2019  The ManaPlus Developers
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

#ifndef RENDER_NACL_NACLFUNCTIONS_H
#define RENDER_NACL_NACLFUNCTIONS_H

#if defined(__native_client__) && defined(USE_OPENGL)

#include <ppapi/c/ppb_graphics_3d.h>

extern const struct PPB_Graphics3D_1_0 *graphics3dInterface;
extern PP_Resource gles2Context;

#define naclResizeBuffers(...) \
    graphics3dInterface->ResizeBuffers(gles2Context, __VA_ARGS__)

#endif  // defined(__native_client__) && defined(USE_OPENGL)
#endif  // RENDER_NACL_NACLFUNCTIONS_H
