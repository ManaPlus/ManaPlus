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

#include "main.h"
#ifdef USE_OPENGL

#define GL_GLEXT_PROTOTYPES 1

#include <SDL_opengl.h>
#include <GL/glext.h>

#define GL_NUM_EXTENSIONS                 0x821D
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41

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
typedef void (APIENTRY *glDeleteFramebuffers_t) (GLsizei, const GLuint *);
typedef void (APIENTRY *glDeleteRenderbuffers_t) (GLsizei, const GLuint *);
typedef const GLubyte *(APIENTRY *glGetStringi_t) (GLenum, GLuint);

typedef void (APIENTRY *glGenSamplers_t) (GLsizei count, GLuint *samplers);
typedef void (APIENTRY *glDeleteSamplers_t)
    (GLsizei count, const GLuint * samplers);
typedef void (APIENTRY *glBindSampler_t) (GLuint unit, GLuint sampler);
typedef void (APIENTRY *glSamplerParameteri_t)
    (GLuint sampler, GLenum pname, GLint param);

defNameE(glGenRenderbuffers);
defNameE(glBindRenderbuffer);
defNameE(glRenderbufferStorage);
defNameE(glGenFramebuffers);
defNameE(glBindFramebuffer);
defNameE(glFramebufferTexture2D);
defNameE(glFramebufferRenderbuffer);
defNameE(glDeleteFramebuffers);
defNameE(glDeleteRenderbuffers);
defNameE(glGetStringi);
defNameE(glGenSamplers);
defNameE(glDeleteSamplers);
defNameE(glBindSampler);
defNameE(glSamplerParameteri);

#ifdef WIN32
typedef const char* (APIENTRY * wglGetExtensionsString_t) (HDC hdc);

defNameE(wglGetExtensionsString);
#endif

#endif
#endif
