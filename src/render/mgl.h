/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#ifndef RENDER_MGL_H
#define RENDER_MGL_H

#include "main.h"
#ifdef USE_OPENGL

#ifdef ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#define APIENTRY GL_APIENTRY
#else
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif
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

#ifndef GL_DEBUG_OUTPUT
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_DEBUG_SOURCE_API               0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM     0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER   0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY       0x8249
#define GL_DEBUG_SOURCE_APPLICATION       0x824A
#define GL_DEBUG_SOURCE_OTHER             0x824B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#endif

#ifndef GL_EXT_debug_label
#define GL_PROGRAM_PIPELINE_OBJECT_EXT    0x8A4F
#define GL_PROGRAM_OBJECT_EXT             0x8B40
#define GL_SHADER_OBJECT_EXT              0x8B48
#define GL_BUFFER_OBJECT_EXT              0x9151
#define GL_QUERY_OBJECT_EXT               0x9153
#define GL_VERTEX_ARRAY_OBJECT_EXT        0x9154
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
typedef void (APIENTRY *glFrameTerminator_t) (void);
typedef void (APIENTRY *glLabelObject_t) (GLenum type, GLuint object,
    GLsizei length, const GLchar *label);
typedef void (APIENTRY *glGetObjectLabel_t) (GLenum type, GLuint object,
    GLsizei bufSize, GLsizei *length, GLchar *label);
typedef void (APIENTRY *glInsertEventMarker_t)
    (GLsizei length, const char *marker);
typedef void (APIENTRY *glPushGroupMarker_t)
    (GLsizei length, const char *marker);
typedef void (APIENTRY *glPopGroupMarker_t) (void);

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
defNameE(glFrameTerminator);
defNameE(glLabelObject);
defNameE(glGetObjectLabel);
defNameE(glInsertEventMarker);
defNameE(glPushGroupMarker);
defNameE(glPopGroupMarker);

#ifdef WIN32
typedef const char* (APIENTRY * wglGetExtensionsString_t) (HDC hdc);

defNameE(wglGetExtensionsString);
#else
#define CALLBACK
#endif

#endif
#endif  // RENDER_MGL_H
