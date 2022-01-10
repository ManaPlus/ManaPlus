/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#define MGL_DEFINE

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
defName(glDisableVertexAttribArray);
defName(glDeleteShader);
defName(glGetShaderInfoLog);
defName(glGetShaderSource);
defName(glDetachShader);
defName(glGetAttachedShaders);
defName(glGetActiveUniform);
defName(glBindFragDataLocation);
#ifndef __native_client__
defName(glDeleteProgram);
defName(glCreateProgram);
defName(glGetProgramiv);
defName(glLinkProgram);
defName(glAttachShader);
defName(glGetShaderiv);
defName(glCompileShader);
defName(glShaderSource);
defName(glCreateShader);
defName(glGetUniformLocation);
defName(glGenBuffers);
defName(glDeleteBuffers);
defName(glBindBuffer);
defName(glBufferData);
defName(glEnableVertexAttribArray);
defName(glVertexAttribPointer);
defName(glUseProgram);
defName(glUniform1f);
defName(glUniform2f);
defName(glUniform4f);
defName(glValidateProgram);
defName(glGetProgramInfoLog);
defName(glBindAttribLocation);
defName(glActiveTexture);

#define mglDrawArrays(...) \
    glDrawArrays(__VA_ARGS__)
#define mglDisable(...) \
    glDisable(__VA_ARGS__)
#define mglHint(...) \
    glHint(__VA_ARGS__)
#define mglScissor(...) \
    glScissor(__VA_ARGS__)
#define mglEnable(...) \
    glEnable(__VA_ARGS__)
#define mglBindTexture(...) \
    glBindTexture(__VA_ARGS__)
#define mglGetIntegerv(...) \
    glGetIntegerv(__VA_ARGS__)
#define mglClear(...) \
    glClear(__VA_ARGS__)
#define mglGetString(...) \
    glGetString(__VA_ARGS__)
#define mglTexParameteri(...) \
    glTexParameteri(__VA_ARGS__)
#define mglTexImage2D(...) \
    glTexImage2D(__VA_ARGS__)
#define mglGenTextures(...) \
    glGenTextures(__VA_ARGS__)
#define mglPixelStorei(...) \
    glPixelStorei(__VA_ARGS__)
#define mglReadPixels(...) \
    glReadPixels(__VA_ARGS__)

#endif
defName(glGetAttribLocation);
defName(glUniform3f);
defName(glCheckFramebufferStatus);
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
defName(glVertexAttribIPointer);
defName(glInvalidateTexImage);
defName(glCopyImageSubData);
defName(glPushDebugGroup);
defName(glPopDebugGroup);
defName(glObjectLabel);
defName(glTexStorage2D);
defName(glGenQueries);
defName(glBeginQuery);
defName(glEndQuery);
defName(glDeleteQueries);
defName(glGetQueryObjectiv);
defName(glGetQueryObjectui64v);
defName(glTextureSubImage2D);
defName(glTextureSubImage2DEXT);
defName(glClearTexImage);
defName(glClearTexSubImage);
#ifdef WIN32
defName(wglGetExtensionsString);
#endif
