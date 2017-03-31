/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2017  The ManaPlus Developers
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

#if defined(__native_client__) && defined(USE_OPENGL)

#include "render/nacl/naclgles.h"

#include "logger.h"

#include "render/opengl/mglfunctions.h"

#include <ppapi_simple/ps.h>

#include <ppapi/c/ppb_graphics_3d.h>
#include <ppapi/c/ppb_opengles2.h>

#include <ppapi/gles2/gl2ext_ppapi.h>

#include "debug.h"

const struct PPB_OpenGLES2* gles2Interface = nullptr;
PP_Resource gles2Context = nullptr;
const struct PPB_Graphics3D_1_0 *graphics3dInterface = nullptr;

void NaclGles::initGles()
{
    gles2Interface = static_cast<const PPB_OpenGLES2*>(
        PSGetInterface(PPB_OPENGLES2_INTERFACE));
    graphics3dInterface = static_cast<const PPB_Graphics3D_1_0*>(
        PSGetInterface(PPB_GRAPHICS_3D_INTERFACE_1_0));
    gles2Context = glGetCurrentContextPPAPI();

    logger->log("InitGles: %p, %d",
        reinterpret_cast<const void*>(gles2Interface),
        gles2Context);
}

#endif  // defined(__native_client__) && defined(USE_OPENGL)
