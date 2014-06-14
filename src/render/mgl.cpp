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

#include "render/mgl.h"

#ifdef USE_OPENGL

#include "debug.h"

#define defName(name) name##_t m##name = nullptr

defName(glGenRenderbuffers);
defName(glBindRenderbuffer);
defName(glRenderbufferStorage);
defName(glGenFramebuffers);
defName(glBindFramebuffer);
defName(glFramebufferTexture2D);
defName(glFramebufferRenderbuffer);
defName(glDeleteFramebuffers);
defName(glDeleteRenderbuffers);
defName(glGetStringi);
defName(glGenSamplers);
defName(glDeleteSamplers);
defName(glBindSampler);
defName(glSamplerParameteri);
defName(glDebugMessageControl);
defName(glDebugMessageCallback);
defName(glFrameTerminator);
defName(glLabelObject);
defName(glGetObjectLabel);
defName(glInsertEventMarker);
defName(glPushGroupMarker);
defName(glPopGroupMarker);
defName(glGenVertexArrays);
defName(glBindVertexArray);
defName(glDeleteVertexArrays);
defName(glVertexAttribPointer);
defName(glEnableVertexAttribArray);
defName(glDisableVertexAttribArray);
defName(glGenBuffers);
defName(glDeleteBuffers);
defName(glBindBuffer);
defName(glBufferData);
defName(glCreateShader);
defName(glDeleteShader);
defName(glGetShaderiv);
defName(glGetShaderInfoLog);
defName(glGetShaderSource);
defName(glShaderSource);
defName(glCompileShader);
defName(glLinkProgram);
defName(glDeleteProgram);
defName(glCreateProgram);
defName(glAttachShader);
defName(glDetachShader);
defName(glGetAttachedShaders);
defName(glGetUniformLocation);
defName(glGetActiveUniform);
defName(glGetProgramiv);
defName(glBindFragDataLocation);
defName(glUseProgram);
defName(glValidateProgram);
defName(glGetAttribLocation);
defName(glUniform1f);
defName(glUniform2f);
defName(glUniform3f);
defName(glUniform4f);
defName(glCheckFramebufferStatus);
defName(glGetProgramInfoLog);
defName(glProgramUniform1f);
defName(glProgramUniform2f);
defName(glProgramUniform3f);
defName(glProgramUniform4f);
defName(glBindVertexBuffer);
defName(glVertexAttribBinding);
defName(glVertexAttribFormat);
defName(glBindVertexBuffers);
defName(glIsBuffer);
defName(glVertexAttribIFormat);

#ifdef WIN32
defName(wglGetExtensionsString);
#endif

#endif
