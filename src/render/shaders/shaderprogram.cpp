/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2018  The ManaPlus Developers
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

#ifdef USE_OPENGL

#include "render/shaders/shaderprogram.h"

#include "render/opengl/mgl.h"
#ifdef __native_client__
#include "render/opengl/naclglfunctions.h"
#endif  // __native_client__

#include "render/shaders/shader.h"

#include "debug.h"

ShaderProgram::ShaderProgram(const unsigned int id,
                             Shader *const vertex,
                             Shader *const fragment) :
    Resource(),
    mProgramId(id),
    mVertex(vertex),
    mFragment(fragment)
{
}

ShaderProgram::~ShaderProgram()
{
    if (mProgramId != 0U)
        mglDeleteProgram(mProgramId);
    if (mVertex != nullptr)
        mVertex->decRef();
    if (mFragment != nullptr)
        mFragment->decRef();
}

#endif  // USE_OPENGL
