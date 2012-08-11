/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#ifndef MGL_H
#define MGL_H

#define GL_GLEXT_PROTOTYPES 1

#include <SDL_opengl.h>
#include <GL/glext.h>

#define defNameE(name) extern name##_t m##name

typedef void (APIENTRY *glGenRenderbuffers_t)(GLsizei, GLuint *);
typedef void (APIENTRY *glBindRenderbuffer_t)(GLenum target,
    GLuint renderbuffer);
typedef void (APIENTRY *glRenderbufferStorage_t)(GLenum target,
    GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRY *glGenFramebuffers_t)(GLsizei n, GLuint *framebuffers);
typedef void (APIENTRY *glBindFramebuffer_t)(GLenum target,
    GLuint framebuffer);
typedef void (APIENTRY *glFramebufferTexture2D_t)(GLenum target,
    GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY *glFramebufferRenderbuffer_t)(GLenum target,
    GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

defNameE(glGenRenderbuffers);
defNameE(glBindRenderbuffer);
defNameE(glRenderbufferStorage);
defNameE(glGenFramebuffers);
defNameE(glBindFramebuffer);
defNameE(glFramebufferTexture2D);
defNameE(glFramebufferRenderbuffer);

#endif
