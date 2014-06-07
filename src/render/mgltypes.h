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

#ifndef RENDER_MGLTYPES_H
#define RENDER_MGLTYPES_H

#include "main.h"
#ifdef USE_OPENGL

#include "render/mgldefines.h"

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
typedef void (APIENTRY *glVertexAttribPointer_t) (GLuint index, GLint size,
    GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY *glEnableVertexAttribArray_t) (GLuint index);
typedef void (APIENTRY *glDisableVertexAttribArray_t) (GLuint index);
typedef void (APIENTRY *glGenBuffers_t) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *glDeleteBuffers_t) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *glBindBuffer_t) (GLenum target, GLuint buffer);
typedef void (APIENTRY *glBufferData_t) (GLenum target, GLsizeiptr size,
    const GLvoid *data, GLenum usage);
typedef void (APIENTRY *glCreateShader_t) (GLenum shaderType);
typedef void (APIENTRY *glDeleteShader_t) (GLenum shader);
typedef void (APIENTRY *glGetShaderiv_t) (GLuint shader,
    GLenum pname, GLint *params);
typedef void (APIENTRY *glGetShaderInfoLog_t) (GLuint shader,
    GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *glGetShaderSource_t) (GLuint shader,
    GLsizei bufSize, GLsizei *length, GLchar *source);
typedef void (APIENTRY *glShaderSource_t) (GLuint shader,
    GLsizei count, const GLchar **string, const GLint *length);
typedef void (APIENTRY *glCompileShader_t) (GLuint shader);
typedef void (APIENTRY *glLinkProgram_t) (GLuint program);
typedef void (APIENTRY *glDeleteProgram_t) (GLuint program);
typedef GLuint (APIENTRY *glCreateProgram_t) (void);
typedef void (APIENTRY *glAttachShader_t) (GLuint program, GLuint shader);
typedef void (APIENTRY *glDetachShader_t) (GLuint program, GLuint shader);
typedef void (APIENTRY *glGetAttachedShaders_t) (GLuint program,
    GLsizei maxCount, GLsizei *count, GLuint *shaders);
typedef void (APIENTRY *glGetUniformLocation_t) (GLuint program,
    const GLchar *name);
typedef void (APIENTRY *glGetActiveUniform_t) (GLuint program, GLuint index,
    GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (APIENTRY *glGetProgramiv_t) (GLuint program,
    GLenum pname, GLint *params);
typedef GLint (APIENTRY *glBindFragDataLocation_t) (GLuint program,
    GLuint colorNumber, const char *name);
typedef void (APIENTRY *glUseProgram_t) (GLuint program);
typedef void (APIENTRY *glValidateProgram_t) (GLuint program);
typedef GLint (APIENTRY *glGetAttribLocation_t) (GLuint program,
    const GLchar *name);
typedef void (APIENTRY *glUniform1f_t) (GLint location, GLfloat v0);
typedef void (APIENTRY *glUniform2f_t) (GLint location,
    GLfloat v0, GLfloat v1);
typedef void (APIENTRY *glUniform3f_t) (GLint location,
    GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *glUniform4f_t) (GLint location,
    GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef GLenum (APIENTRY *glCheckFramebufferStatus_t) (GLenum target);

// callback
typedef void (APIENTRY *GLDEBUGPROC_t) (GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);

typedef void (APIENTRY *glDebugMessageCallback_t) (GLDEBUGPROC_t callback,
    const void *userParam);


#ifdef WIN32
typedef const char* (APIENTRY * wglGetExtensionsString_t) (HDC hdc);
#else
#define CALLBACK
#endif

#endif  // USE_OPENGL
#endif  // RENDER_MGLTYPES_H
