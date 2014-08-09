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

#include "render/mgldefines.h"
#include "render/mgltypes.h"

#define defNameE(name) extern name##_t m##name

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
defNameE(glGenVertexArrays);
defNameE(glBindVertexArray);
defNameE(glDeleteVertexArrays);
defNameE(glVertexAttribPointer);
defNameE(glEnableVertexAttribArray);
defNameE(glDisableVertexAttribArray);
defNameE(glGenBuffers);
defNameE(glDeleteBuffers);
defNameE(glBindBuffer);
defNameE(glBufferData);
defNameE(glCreateShader);
defNameE(glDeleteShader);
defNameE(glGetShaderiv);
defNameE(glGetShaderInfoLog);
defNameE(glGetShaderSource);
defNameE(glShaderSource);
defNameE(glCompileShader);
defNameE(glLinkProgram);
defNameE(glDeleteProgram);
defNameE(glCreateProgram);
defNameE(glAttachShader);
defNameE(glDetachShader);
defNameE(glGetAttachedShaders);
defNameE(glGetUniformLocation);
defNameE(glGetActiveUniform);
defNameE(glGetProgramiv);
defNameE(glBindFragDataLocation);
defNameE(glUseProgram);
defNameE(glValidateProgram);
defNameE(glGetAttribLocation);
defNameE(glUniform1f);
defNameE(glUniform2f);
defNameE(glUniform3f);
defNameE(glUniform4f);
defNameE(glCheckFramebufferStatus);
defNameE(glGetProgramInfoLog);
defNameE(glProgramUniform1f);
defNameE(glProgramUniform2f);
defNameE(glProgramUniform3f);
defNameE(glProgramUniform4f);
defNameE(glBindVertexBuffer);
defNameE(glVertexAttribBinding);
defNameE(glVertexAttribFormat);
defNameE(glBindVertexBuffers);
defNameE(glIsBuffer);
defNameE(glVertexAttribIFormat);
defNameE(glVertexAttribIPointer);
defNameE(glInvalidateTexImage);
defNameE(glCopyImageSubData);
defNameE(glPushDebugGroup);
defNameE(glPopDebugGroup);
defNameE(glObjectLabel);
defNameE(glTexStorage2D);
defNameE(glGenQueries);
defNameE(glBeginQuery);
defNameE(glEndQuery);
defNameE(glDeleteQueries);
defNameE(glGetQueryObjectiv);
defNameE(glGetQueryObjectui64v);
defNameE(glTextureSubImage2D);
defNameE(glClearTexImage);
defNameE(glClearTexSubImage);

#ifdef WIN32
defNameE(wglGetExtensionsString);
#endif

#undef defNameE

#endif  // USE_OPENGL
#endif  // RENDER_MGL_H
