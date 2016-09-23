/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2016  The ManaPlus Developers
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

#include "render/opengl/mglemu.h"

#include "resources/openglimagehelper.h"

#include "debug.h"

void APIENTRY emuglTextureSubImage2DEXT(GLuint texture, GLenum target,
                                        GLint level,
                                        GLint xoffset, GLint yoffset,
                                        GLsizei width, GLsizei height,
                                        GLenum format, GLenum type,
                                        const void *pixels)
{
    OpenGLImageHelper::bindTexture(texture);
    glTexSubImage2D(target, level,
        xoffset, yoffset,
        width, height,
        format, type, pixels);
}

void APIENTRY emuglActiveTexture(GLenum texture A_UNUSED)
{
}

#endif  // USE_OPENGL
