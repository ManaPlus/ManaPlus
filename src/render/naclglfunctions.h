/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2015  The ManaPlus Developers
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

#ifndef RENDER_NACLGLFUNCTIONS_H
#define RENDER_NACLGLFUNCTIONS_H

#if defined(__native_client__) && defined(USE_OPENGL)

#include <ppapi/c/ppb_opengles2.h>

extern const struct PPB_OpenGLES2* gles2Interface;
extern PP_Resource gles2Context;

#define mglDrawArrays(...) \
    gles2Interface->DrawArrays(gles2Context, __VA_ARGS__)
#define mglEnableVertexAttribArray(...) \
    gles2Interface->EnableVertexAttribArray(gles2Context, __VA_ARGS__)
#define mglVertexAttribPointer(...) \
    gles2Interface->VertexAttribPointer(gles2Context, __VA_ARGS__)
#define mglUseProgram(...) \
    gles2Interface->UseProgram(gles2Context, __VA_ARGS__)
#define mglViewport(...) \
    gles2Interface->Viewport(gles2Context, __VA_ARGS__)
#define mglGenBuffers(...) \
    gles2Interface->GenBuffers(gles2Context, __VA_ARGS__)
#define mglBindBuffer(...) \
    gles2Interface->BindBuffer(gles2Context, __VA_ARGS__)
#define mglBufferData(...) \
    gles2Interface->BufferData(gles2Context, __VA_ARGS__)
#define mglDeleteBuffers(...) \
    gles2Interface->DeleteBuffers(gles2Context, __VA_ARGS__)
#define mglGetUniformLocation(...) \
    gles2Interface->GetUniformLocation(gles2Context, __VA_ARGS__)
#define mglCreateShader(...) \
    gles2Interface->CreateShader(gles2Context, __VA_ARGS__)
#define mglShaderSource(...) \
    gles2Interface->ShaderSource(gles2Context, __VA_ARGS__)
#define mglCompileShader(...) \
    gles2Interface->CompileShader(gles2Context, __VA_ARGS__)
#define mglGetShaderiv(...) \
    gles2Interface->GetShaderiv(gles2Context, __VA_ARGS__)
#define mglAttachShader(...) \
    gles2Interface->AttachShader(gles2Context, __VA_ARGS__)
#define mglBindAttribLocation(...) \
    gles2Interface->BindAttribLocation(gles2Context, __VA_ARGS__)
#define mglLinkProgram(...) \
    gles2Interface->LinkProgram(gles2Context, __VA_ARGS__)
#define mglGetProgramiv(...) \
    gles2Interface->GetProgramiv(gles2Context, __VA_ARGS__)
#define mglClearColor(...) \
    gles2Interface->ClearColor(gles2Context, __VA_ARGS__)
#define mglUniform1f(...) \
    gles2Interface->Uniform1f(gles2Context, __VA_ARGS__)
#define mglUniform2f(...) \
    gles2Interface->Uniform2f(gles2Context, __VA_ARGS__)
#define mglUniform4f(...) \
    gles2Interface->Uniform4f(gles2Context, __VA_ARGS__)
#define mglActiveTexture(...) \
    gles2Interface->ActiveTexture(gles2Context, __VA_ARGS__)
#define mglBindTexture(...) \
    gles2Interface->BindTexture(gles2Context, __VA_ARGS__)
#define mglEnable(...) \
    gles2Interface->Enable(gles2Context, __VA_ARGS__)
#define mglClear(...) \
    gles2Interface->Clear(gles2Context, __VA_ARGS__)
#define mglValidateProgram(...) \
    gles2Interface->ValidateProgram(gles2Context, __VA_ARGS__)
#define mglDeleteProgram(...) \
    gles2Interface->DeleteProgram(gles2Context, __VA_ARGS__)
#define mglCreateProgram() \
    gles2Interface->CreateProgram(gles2Context)
#define mglGetProgramInfoLog(...) \
    gles2Interface->GetProgramInfoLog(gles2Context, __VA_ARGS__)
#define mglDisable(...) \
    gles2Interface->Disable(gles2Context, __VA_ARGS__)
#define mglHint(...) \
    gles2Interface->Hint(gles2Context, __VA_ARGS__)
#define mglScissor(...) \
    gles2Interface->Scissor(gles2Context, __VA_ARGS__)
#define mglEnable(...) \
    gles2Interface->Enable(gles2Context, __VA_ARGS__)
#define mglGetIntegerv(...) \
    gles2Interface->GetIntegerv(gles2Context, __VA_ARGS__)

#endif  // defined(__native_client__) && defined(USE_OPENGL)
#endif  // RENDER_NACLGLFUNCTIONS_H
