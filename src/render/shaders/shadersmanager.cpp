/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#include "logger.h"
#include "settings.h"

#include "render/mgl.h"

#include "render/shaders/shader.h"

#include "utils/files.h"

#include "debug.h"

ShadersManager shaders;

Shader *ShadersManager::createShader(const unsigned int type,
                                     const std::string &fileName)
{
    const std::string str = Files::loadTextFileString(fileName);
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
    char *buf = new char[len + 1];
    mglGetShaderInfoLog(shaderId, len, &len, buf);
    buf[len] = 0;
    logger->log("Shader '%s' compilation error: %s", fileName.c_str(), buf);
    delete [] buf;
    mglDeleteShader(shaderId);
    return nullptr;
}

#endif
