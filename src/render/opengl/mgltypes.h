/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#ifndef RENDER_OPENGL_MGLTYPES_H
#define RENDER_OPENGL_MGLTYPES_H

#ifdef USE_OPENGL

#ifdef ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#define APIENTRY GL_APIENTRY
#else  // ANDROID
#include "localconsts.h"
#ifndef USE_SDL2
#define GL_GLEXT_PROTOTYPES 1
#endif  // USE_SDL2
#ifdef HAVE_GLEXT
#define NO_SDL_GLEXT
#endif  // HAVE_GLEXT
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
// ignore -Wredundant-decls for SDL 1.2
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <SDL_opengl.h>
PRAGMA45(GCC diagnostic pop)
PRAGMA48(GCC diagnostic pop)
#ifdef HAVE_GLEXT
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <GL/glext.h>
PRAGMA45(GCC diagnostic pop)
#endif  // HAVE_GLEXT
#ifdef __native_client__
#include <GL/Regal.h>
#else  // __native_client__
#if defined(__APPLE__)
#include <OpenGL/glext.h>
#else  // defined(__APPLE__)
#ifndef __glext_h_
// probably this include need for some os / libs
PRAGMA45(GCC diagnostic push)
PRAGMA45(GCC diagnostic ignored "-Wredundant-decls")
#include <GL/glext.h>
PRAGMA45(GCC diagnostic pop)
// #error missing include <GL/glext.h>
#endif  //  __glext_h_
#endif  // defined(__APPLE__)
#endif  // __native_client__
#endif  // ANDROID

#include "render/opengl/mgldefines.h"
RENDER_OPENGL_MGLDEFINES_H

#ifndef USE_SDL2
#if defined(__GXX_EXPERIMENTAL_CXX0X__)
#include <cstdint>
#endif  // defined(__GXX_EXPERIMENTAL_CXX0X__)
#endif  // USE_SDL2

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
typedef void (APIENTRY *glGenVertexArrays_t) (GLsizei n, GLuint *arrays);
typedef void (APIENTRY *glBindVertexArray_t) (GLuint array);
typedef void (APIENTRY *glDeleteVertexArrays_t) (GLsizei n, GLuint *arrays);
typedef void (APIENTRY *glDisableVertexAttribArray_t) (GLuint index);
typedef void (APIENTRY *glDeleteShader_t) (GLenum shader);
typedef void (APIENTRY *glGetShaderInfoLog_t) (GLuint shader,
    GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *glGetShaderSource_t) (GLuint shader,
    GLsizei bufSize, GLsizei *length, GLchar *source);
typedef void (APIENTRY *glDetachShader_t) (GLuint program, GLuint shader);
typedef void (APIENTRY *glGetAttachedShaders_t) (GLuint program,
    GLsizei maxCount, GLsizei *count, GLuint *shaders);
typedef void (APIENTRY *glGetActiveUniform_t) (GLuint program, GLuint index,
    GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef GLint (APIENTRY *glBindFragDataLocation_t) (GLuint program,
    GLuint colorNumber, const char *name);
#ifndef __native_client__
typedef void (APIENTRY *glDeleteProgram_t) (GLuint program);
typedef GLuint (APIENTRY *glCreateProgram_t) (void);
typedef void (APIENTRY *glGetProgramiv_t) (GLuint program,
    GLenum pname, GLint *params);
typedef void (APIENTRY *glLinkProgram_t) (GLuint program);
typedef void (APIENTRY *glAttachShader_t) (GLuint program, GLuint shader);
typedef void (APIENTRY *glGetShaderiv_t) (GLuint shader,
    GLenum pname, GLint *params);
typedef void (APIENTRY *glCompileShader_t) (GLuint shader);
typedef void (APIENTRY *glShaderSource_t) (GLuint shader,
    GLsizei count, const GLchar **string, const GLint *length);
typedef GLuint (APIENTRY *glCreateShader_t) (GLenum shaderType);
typedef GLint (APIENTRY *glGetUniformLocation_t) (GLuint program,
    const GLchar *name);
typedef void (APIENTRY *glGenBuffers_t) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *glDeleteBuffers_t) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *glBindBuffer_t) (GLenum target, GLuint buffer);
typedef void (APIENTRY *glBufferData_t) (GLenum target, GLsizeiptr size,
    const GLvoid *data, GLenum usage);
typedef void (APIENTRY *glEnableVertexAttribArray_t) (GLuint index);
typedef void (APIENTRY *glVertexAttribPointer_t) (GLuint index, GLint size,
    GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY *glUseProgram_t) (GLuint program);
typedef void (APIENTRY *glUniform1f_t) (GLint location, GLfloat v0);
typedef void (APIENTRY *glUniform2f_t) (GLint location,
    GLfloat v0, GLfloat v1);
typedef void (APIENTRY *glUniform4f_t) (GLint location,
    GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY *glValidateProgram_t) (GLuint program);
typedef void (APIENTRY *glGetProgramInfoLog_t) (GLuint program,
    GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *glBindAttribLocation_t) (GLuint program,
    GLuint index, const GLchar *name);
typedef void (APIENTRY *glActiveTexture_t) (GLenum texture);
#endif  // __native_client__

typedef GLint (APIENTRY *glGetAttribLocation_t) (GLuint program,
    const GLchar *name);
typedef void (APIENTRY *glUniform3f_t) (GLint location,
    GLfloat v0, GLfloat v1, GLfloat v2);
typedef GLenum (APIENTRY *glCheckFramebufferStatus_t) (GLenum target);
typedef void (APIENTRY *glProgramUniform1f_t) (GLuint program,
    GLint location, GLfloat v0);
typedef void (APIENTRY *glProgramUniform2f_t) (GLuint program,
    GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY *glProgramUniform3f_t) (GLuint program,
    GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *glProgramUniform4f_t) (GLuint program,
    GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY *glBindVertexBuffer_t) (GLuint bindingindex,
    GLuint buffer, GLintptr offset, GLintptr stride);
typedef void (APIENTRY *glVertexAttribBinding_t) (GLuint attribindex,
    GLuint bindingindex);
typedef void (APIENTRY *glVertexAttribFormat_t) (GLuint attribindex,
    GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (APIENTRY *glBindVertexBuffers_t) (GLuint first, GLsizei count,
    const GLuint *buffers, const GLuint *offsets, const GLsizei *strides);
typedef GLboolean (APIENTRY *glIsBuffer_t) (GLuint buffer);
typedef void (APIENTRY *glVertexAttribIFormat_t) (GLuint attribindex,
    GLint size, GLenum type, GLuint relativeoffset);
typedef void (APIENTRY *glVertexAttribIPointer_t) (GLuint index, GLint size,
    GLenum type, GLsizei stride, const GLvoid * pointer);
typedef void (APIENTRY *glInvalidateTexImage_t) (GLuint texture, GLint level);
typedef void (APIENTRY *glCopyImageSubData_t) (GLuint srcName,
    GLenum srcTarget, GLint srcLevel,
    GLint srcX, GLint srcY, GLint srcZ,
    GLuint dstName, GLenum dstTarget, GLint dstLevel,
    GLint dstX, GLint dstY, GLint dstZ,
    GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
typedef void (APIENTRY *glPushDebugGroup_t) (GLenum source, GLuint id,
        GLsizei length, const GLchar * message);
typedef void (APIENTRY *glPopDebugGroup_t) (void);
typedef void (APIENTRY *glObjectLabel_t) (GLenum identifier, GLuint name,
    GLsizei length, const GLchar *label);
typedef void (APIENTRY *glTexStorage2D_t) (GLenum target, GLsizei levels,
    GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRY *glGenQueries_t) (GLsizei n, GLuint *ids);
typedef void (APIENTRY *glBeginQuery_t) (GLenum target, GLuint id);
typedef void (APIENTRY *glEndQuery_t) (GLenum target);
typedef void (APIENTRY *glDeleteQueries_t) (GLsizei n, const GLuint *ids);
typedef void (APIENTRY *glGetQueryObjectiv_t) (GLuint id,
    GLenum pname, GLint *params);
typedef void (APIENTRY *glGetQueryObjectui64v_t) (GLuint id,
    GLenum pname, uint64_t *params);
typedef void (APIENTRY *glTextureSubImage2DEXT_t) (GLuint texture,
    GLenum target, GLint level, GLint xoffset, GLint yoffset,
    GLsizei width, GLsizei height, GLenum format, GLenum type,
    const void *pixels);
typedef void (APIENTRY *glTextureSubImage2D_t) (GLuint texture,
    GLint level, GLint xoffset, GLint yoffset,
    GLsizei width, GLsizei height, GLenum format, GLenum type,
    const void *pixels);
typedef void (APIENTRY *glClearTexImage_t) (GLuint texture, GLint level,
    GLenum format, GLenum type, const void * data);
typedef void (APIENTRY *glClearTexSubImage_t) (GLuint texture, GLint level,
    GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
    GLsizei depth, GLenum format, GLenum type, const void * data);

// callback
typedef void (APIENTRY *GLDEBUGPROC_t) (GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);

typedef void (APIENTRY *glDebugMessageCallback_t) (GLDEBUGPROC_t callback,
    const void *userParam);


#ifdef WIN32
typedef const char* (APIENTRY * wglGetExtensionsString_t) (HDC hdc);
#else  // WIN32
#define CALLBACK
#endif  // WIN32

#endif  // USE_OPENGL
#endif  // RENDER_OPENGL_MGLTYPES_H
