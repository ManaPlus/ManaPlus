/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
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

#include "render/shaders/shadersmanager.h"

#ifdef USE_OPENGL

#include "configuration.h"
#include "logger.h"

#include "fs/virtfs/tools.h"

#include "utils/cast.h"

#include "render/opengl/mgl.h"
#ifdef __native_client__
#include "render/opengl/naclglfunctions.h"
#endif  // __native_client__

#include "render/shaders/shader.h"
#include "render/shaders/shaderprogram.h"

#include "resources/loaders/shaderloader.h"

#include "debug.h"

ShadersManager shaders;

Shader *ShadersManager::createShader(const unsigned int type,
                                     const std::string &fileName)
{
    const std::string str = VirtFs::loadTextFileString(fileName);
    const char *ptrStr = str.c_str();
    GLuint shaderId = mglCreateShader(type);
    mglShaderSource(shaderId, 1, &ptrStr, nullptr);
    mglCompileShader(shaderId);

    GLint isCompiled = 0;
    mglGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_TRUE)
        return new Shader(shaderId);
    GLint len = 0;
    mglGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
    char *buf = new char[CAST_SIZE(len) + 1];
    mglGetShaderInfoLog(shaderId, len, &len, buf);
    buf[len] = 0;
    logger->log("Shader '%s' compilation error: %s", fileName.c_str(), buf);
    delete [] buf;
    mglDeleteShader(shaderId);
    return nullptr;
}

ShaderProgram *ShadersManager::createProgram(const std::string &vertex,
                                             const std::string &fragment,
                                             const bool isNewShader)
{
    Shader *const vertexShader = static_cast<Shader*>(
        Loader::getShader(GL_VERTEX_SHADER, vertex));
    if (vertexShader == nullptr)
        return nullptr;

    Shader *const fragmentShader = static_cast<Shader*>(
        Loader::getShader(GL_FRAGMENT_SHADER, fragment));

    if (fragmentShader == nullptr)
    {
        vertexShader->decRef();
        return nullptr;
    }

    GLuint programId = mglCreateProgram();
    if (programId == 0U)
    {
        vertexShader->decRef();
        fragmentShader->decRef();
        return nullptr;
    }

    mglAttachShader(programId, vertexShader->getShaderId());
    mglAttachShader(programId, fragmentShader->getShaderId());
    if (isNewShader)
        mglBindFragDataLocation(programId, 0, "outColor");
    else
        mglBindAttribLocation(programId, 0, "position");
    mglLinkProgram(programId);
    GLint isLinked = 0;
    mglGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_TRUE)
    {
        mglValidateProgram(programId);
        GLint isValidated = 0;
        mglGetProgramiv(programId,  GL_VALIDATE_STATUS, &isValidated);
        if (isValidated == GL_TRUE)
            return new ShaderProgram(programId, vertexShader, fragmentShader);
        mglDeleteProgram(programId);
        return nullptr;
    }

    GLint len = 0;
    mglGetProgramiv(programId, GL_INFO_LOG_LENGTH, &len);
    char *buf = new char[CAST_SIZE(len) + 1];
    mglGetProgramInfoLog(programId, len, &len, buf);
    buf[len] = 0;
    logger->log("Program '%s, %s' compilation error: %s",
        vertexShader->mIdPath.c_str(),
        fragmentShader->mIdPath.c_str(),
        buf);
    delete [] buf;
    mglDeleteProgram(programId);
    return nullptr;
}

ShaderProgram *ShadersManager::getSimpleProgram()
{
    const std::string dir = paths.getStringValue("shaders");
    return createProgram(dir + paths.getStringValue("simpleVertexShader"),
        dir + paths.getStringValue("simpleFragmentShader"),
        true);
}

ShaderProgram *ShadersManager::getGles2Program()
{
    const std::string dir = paths.getStringValue("shaders");
    return createProgram(dir + paths.getStringValue("gles2VertexShader"),
        dir + paths.getStringValue("gles2FragmentShader"),
        false);
}

#endif  // USE_OPENGL
