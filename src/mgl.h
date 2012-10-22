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

#ifdef ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#define APIENTRY GL_APIENTRY
#else
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <GL/glext.h>
#endif

#define GL_NUM_EXTENSIONS                 0x821D
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41

#ifndef GL_COMPRESSED_RGBA_ARB
#define GL_COMPRESSED_RGBA_ARB            0x84EE
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#define GL_COMPRESSED_RGBA_FXT1_3DFX      0x86B1
#endif
#ifndef GL_MAX_ELEMENTS_VERTICES
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#endif

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
typedef void (APIENTRY *glDebugMessageControl_t) (GLenum source, GLenum type,
    GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);

// callback
typedef void (APIENTRY *GLDEBUGPROC_t) (GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);

typedef void (APIENTRY *glDebugMessageCallback_t) (GLDEBUGPROC_t callback,
    const void *userParam);

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
defNameE(glDebugMessageControl);
defNameE(glDebugMessageCallback);

#ifdef WIN32
typedef const char* (APIENTRY * wglGetExtensionsString_t) (HDC hdc);

defNameE(wglGetExtensionsString);
#endif

#endif
#endif
