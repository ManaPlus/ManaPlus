/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef RENDER_OPENGL_NACLGLFUNCTIONS_H
#define RENDER_OPENGL_NACLGLFUNCTIONS_H

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
#define mglUniform3f(...) \
    gles2Interface->Uniform3f(gles2Context, __VA_ARGS__)
#define mglUniform4f(...) \
    gles2Interface->Uniform4f(gles2Context, __VA_ARGS__)
#define mglUniform1i(...) \
    gles2Interface->Uniform1i(gles2Context, __VA_ARGS__)
#define mglUniform2i(...) \
    gles2Interface->Uniform2i(gles2Context, __VA_ARGS__)
#define mglUniform3i(...) \
    gles2Interface->Uniform3i(gles2Context, __VA_ARGS__)
#define mglUniform4i(...) \
    gles2Interface->Uniform4i(gles2Context, __VA_ARGS__)
#define mglUniform1fv(...) \
    gles2Interface->Uniform1fv(gles2Context, __VA_ARGS__)
#define mglUniform2fv(...) \
    gles2Interface->Uniform2fv(gles2Context, __VA_ARGS__)
#define mglUniform3fv(...) \
    gles2Interface->Uniform3fv(gles2Context, __VA_ARGS__)
#define mglUniform4fv(...) \
    gles2Interface->Uniform4fv(gles2Context, __VA_ARGS__)
#define mglUniform1iv(...) \
    gles2Interface->Uniform1iv(gles2Context, __VA_ARGS__)
#define mglUniform2iv(...) \
    gles2Interface->Uniform2iv(gles2Context, __VA_ARGS__)
#define mglUniform3iv(...) \
    gles2Interface->Uniform3iv(gles2Context, __VA_ARGS__)
#define mglUniform4iv(...) \
    gles2Interface->Uniform4iv(gles2Context, __VA_ARGS__)
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
#define mglGetString(...) \
    gles2Interface->GetString(gles2Context, __VA_ARGS__)
#define mglBindFramebuffer(...) \
    gles2Interface->BindFramebuffer(gles2Context, __VA_ARGS__)
#define mglBindRenderbuffer(...) \
    gles2Interface->BindRenderbuffer(gles2Context, __VA_ARGS__)
#define mglBlendColor(...) \
    gles2Interface->BlendColor(gles2Context, __VA_ARGS__)
#define mglBlendEquation(...) \
    gles2Interface->BlendEquation(gles2Context, __VA_ARGS__)
#define mglBlendEquationSeparate(...) \
    gles2Interface->BlendEquationSeparate(gles2Context, __VA_ARGS__)
#define mglBlendFunc(...) \
    gles2Interface->BlendFunc(gles2Context, __VA_ARGS__)
#define mglBlendFuncSeparate(...) \
    gles2Interface->BlendFuncSeparate(gles2Context, __VA_ARGS__)
#define mglBufferSubData(...) \
    gles2Interface->BufferSubData(gles2Context, __VA_ARGS__)
#define mglCheckFramebufferStatus(...) \
    gles2Interface->CheckFramebufferStatus(gles2Context, __VA_ARGS__)
#define mglClearDepthf(...) \
    gles2Interface->ClearDepthf(gles2Context, __VA_ARGS__)
#define mglClearStencil(...) \
    gles2Interface->ClearStencil(gles2Context, __VA_ARGS__)
#define mglColorMask(...) \
    gles2Interface->ColorMask(gles2Context, __VA_ARGS__)
#define mglCompressedTexImage2D(...) \
    gles2Interface->CompressedTexImage2D(gles2Context, __VA_ARGS__)
#define mglCompressedTexSubImage2D(...) \
    gles2Interface->CompressedTexSubImage2D(gles2Context, __VA_ARGS__)
#define mglCopyTexImage2D(...) \
    gles2Interface->CopyTexImage2D(gles2Context, __VA_ARGS__)
#define mglCopyTexSubImage2D(...) \
    gles2Interface->CopyTexSubImage2D(gles2Context, __VA_ARGS__)
#define mglCullFace(...) \
    gles2Interface->CullFace(gles2Context, __VA_ARGS__)
#define mglDeleteFramebuffers(...) \
    gles2Interface->DeleteFramebuffers(gles2Context, __VA_ARGS__)
#define mglDeleteRenderbuffers(...) \
    gles2Interface->DeleteRenderbuffers(gles2Context, __VA_ARGS__)
#define mglDeleteShader(...) \
    gles2Interface->DeleteShader(gles2Context, __VA_ARGS__)
#define mglDeleteTextures(...) \
    gles2Interface->DeleteTextures(gles2Context, __VA_ARGS__)
#define mglDepthFunc(...) \
    gles2Interface->DepthFunc(gles2Context, __VA_ARGS__)
#define mglDepthMask(...) \
    gles2Interface->DepthMask(gles2Context, __VA_ARGS__)
#define mglDepthRangef(...) \
    gles2Interface->DepthRangef(gles2Context, __VA_ARGS__)
#define mglDetachShader(...) \
    gles2Interface->DetachShader(gles2Context, __VA_ARGS__)
#define mglDisableVertexAttribArray(...) \
    gles2Interface->DisableVertexAttribArray(gles2Context, __VA_ARGS__)
#define mglDrawElements(...) \
    gles2Interface->DrawElements(gles2Context, __VA_ARGS__)
#define mglFinish(...) \
    gles2Interface->Finish(gles2Context, __VA_ARGS__)
#define mglFlush(...) \
    gles2Interface->Flush(gles2Context, __VA_ARGS__)
#define mglFramebufferRenderbuffer(...) \
    gles2Interface->FramebufferRenderbuffer(gles2Context, __VA_ARGS__)
#define mglFramebufferTexture2D(...) \
    gles2Interface->FramebufferTexture2D(gles2Context, __VA_ARGS__)
#define mglFrontFace(...) \
    gles2Interface->FrontFace(gles2Context, __VA_ARGS__)
#define mglGenerateMipmap(...) \
    gles2Interface->GenerateMipmap(gles2Context, __VA_ARGS__)
#define mglGenFramebuffers(...) \
    gles2Interface->GenFramebuffers(gles2Context, __VA_ARGS__)
#define mglGenRenderbuffers(...) \
    gles2Interface->GenRenderbuffers(gles2Context, __VA_ARGS__)
#define mglGenTextures(...) \
    gles2Interface->GenTextures(gles2Context, __VA_ARGS__)
#define mglGetActiveAttrib(...) \
    gles2Interface->GetActiveAttrib(gles2Context, __VA_ARGS__)
#define mglGetActiveUniform(...) \
    gles2Interface->GetActiveUniform(gles2Context, __VA_ARGS__)
#define mglGetAttachedShaders(...) \
    gles2Interface->GetAttachedShaders(gles2Context, __VA_ARGS__)
#define mglGetAttribLocation(...) \
    gles2Interface->GetAttribLocation(gles2Context, __VA_ARGS__)
#define mglGetBooleanv(...) \
    gles2Interface->GetBooleanv(gles2Context, __VA_ARGS__)
#define mglGetBufferParameteriv(...) \
    gles2Interface->GetBufferParameteriv(gles2Context, __VA_ARGS__)
#define mglGetError(...) \
    gles2Interface->GetError(gles2Context, __VA_ARGS__)
#define mglGetFloatv(...) \
    gles2Interface->GetFloatv(gles2Context, __VA_ARGS__)
#define mglGetFramebufferAttachmentParameteriv(...) \
    gles2Interface->GetFramebufferAttachmentParameteriv(gles2Context, \
    __VA_ARGS__)
#define mglGetIntegerv(...) \
    gles2Interface->GetIntegerv(gles2Context, __VA_ARGS__)
#define mglGetRenderbufferParameteriv(...) \
    gles2Interface->GetRenderbufferParameteriv(gles2Context, __VA_ARGS__)
#define mglGetShaderInfoLog(...) \
    gles2Interface->GetShaderInfoLog(gles2Context, __VA_ARGS__)
#define mglGetShaderPrecisionFormat(...) \
    gles2Interface->GetShaderPrecisionFormat(gles2Context, __VA_ARGS__)
#define mglGetShaderSource(...) \
    gles2Interface->GetShaderSource(gles2Context, __VA_ARGS__)
#define mglGetTexParameterfv(...) \
    gles2Interface->GetTexParameterfv(gles2Context, __VA_ARGS__)
#define mglGetTexParameteriv(...) \
    gles2Interface->GetTexParameteriv(gles2Context, __VA_ARGS__)
#define mglGetUniformfv(...) \
    gles2Interface->GetUniformfv(gles2Context, __VA_ARGS__)
#define mglGetUniformiv(...) \
    gles2Interface->GetUniformiv(gles2Context, __VA_ARGS__)
#define mglGetVertexAttribfv(...) \
    gles2Interface->GetVertexAttribfv(gles2Context, __VA_ARGS__)
#define mglGetVertexAttribiv(...) \
    gles2Interface->GetVertexAttribiv(gles2Context, __VA_ARGS__)
#define mglGetVertexAttribPointerv(...) \
    gles2Interface->GetVertexAttribPointerv(gles2Context, __VA_ARGS__)
#define mglIsBuffer(...) \
    gles2Interface->IsBuffer(gles2Context, __VA_ARGS__)
#define mglIsEnabled(...) \
    gles2Interface->IsEnabled(gles2Context, __VA_ARGS__)
#define mglIsFramebuffer(...) \
    gles2Interface->IsFramebuffer(gles2Context, __VA_ARGS__)
#define mglIsProgram(...) \
    gles2Interface->IsProgram(gles2Context, __VA_ARGS__)
#define mglIsRenderbuffer(...) \
    gles2Interface->IsRenderbuffer(gles2Context, __VA_ARGS__)
#define mglIsShader(...) \
    gles2Interface->IsShader(gles2Context, __VA_ARGS__)
#define mglIsTexture(...) \
    gles2Interface->IsTexture(gles2Context, __VA_ARGS__)
#define mglLineWidth(...) \
    gles2Interface->LineWidth(gles2Context, __VA_ARGS__)
#define mglPixelStorei(...) \
    gles2Interface->PixelStorei(gles2Context, __VA_ARGS__)
#define mglPolygonOffset(...) \
    gles2Interface->PolygonOffset(gles2Context, __VA_ARGS__)
#define mglReadPixels(...) \
    gles2Interface->ReadPixels(gles2Context, __VA_ARGS__)
#define mglReleaseShaderCompiler(...) \
    gles2Interface->ReleaseShaderCompiler(gles2Context, __VA_ARGS__)
#define mglRenderbufferStorage(...) \
    gles2Interface->RenderbufferStorage(gles2Context, __VA_ARGS__)
#define mglSampleCoverage(...) \
    gles2Interface->SampleCoverage(gles2Context, __VA_ARGS__)
#define mglShaderBinary(...) \
    gles2Interface->ShaderBinary(gles2Context, __VA_ARGS__)
#define mglStencilFunc(...) \
    gles2Interface->StencilFunc(gles2Context, __VA_ARGS__)
#define mglStencilFuncSeparate(...) \
    gles2Interface->StencilFuncSeparate(gles2Context, __VA_ARGS__)
#define mglStencilMask(...) \
    gles2Interface->StencilMask(gles2Context, __VA_ARGS__)
#define mglStencilMaskSeparate(...) \
    gles2Interface->StencilMaskSeparate(gles2Context, __VA_ARGS__)
#define mglStencilOp(...) \
    gles2Interface->StencilOp(gles2Context, __VA_ARGS__)
#define mglStencilOpSeparate(...) \
    gles2Interface->StencilOpSeparate(gles2Context, __VA_ARGS__)
#define mglTexImage2D(...) \
    gles2Interface->TexImage2D(gles2Context, __VA_ARGS__)
#define mglTexParameterf(...) \
    gles2Interface->TexParameterf(gles2Context, __VA_ARGS__)
#define mglTexParameterfv(...) \
    gles2Interface->TexParameterfv(gles2Context, __VA_ARGS__)
#define mglTexParameteri(...) \
    gles2Interface->TexParameteri(gles2Context, __VA_ARGS__)
#define mglTexParameteriv(...) \
    gles2Interface->TexParameteriv(gles2Context, __VA_ARGS__)
#define mglTexSubImage2D(...) \
    gles2Interface->TexSubImage2D(gles2Context, __VA_ARGS__)
#define mglUniformMatrix2fv(...) \
    gles2Interface->UniformMatrix2fv(gles2Context, __VA_ARGS__)
#define mglUniformMatrix3fv(...) \
    gles2Interface->UniformMatrix3fv(gles2Context, __VA_ARGS__)
#define mglUniformMatrix4fv(...) \
    gles2Interface->UniformMatrix4fv(gles2Context, __VA_ARGS__)
#define mglVertexAttrib1f(...) \
    gles2Interface->VertexAttrib1f(gles2Context, __VA_ARGS__)
#define mglVertexAttrib2f(...) \
    gles2Interface->VertexAttrib2f(gles2Context, __VA_ARGS__)
#define mglVertexAttrib3f(...) \
    gles2Interface->VertexAttrib3f(gles2Context, __VA_ARGS__)
#define mglVertexAttrib4f(...) \
    gles2Interface->VertexAttrib4f(gles2Context, __VA_ARGS__)
#define mglVertexAttrib1fv(...) \
    gles2Interface->VertexAttrib1fv(gles2Context, __VA_ARGS__)
#define mglVertexAttrib2fv(...) \
    gles2Interface->VertexAttrib2fv(gles2Context, __VA_ARGS__)
#define mglVertexAttrib3fv(...) \
    gles2Interface->VertexAttrib3fv(gles2Context, __VA_ARGS__)
#define mglVertexAttrib4fv(...) \
    gles2Interface->VertexAttrib4fv(gles2Context, __VA_ARGS__)

#endif  // defined(__native_client__) && defined(USE_OPENGL)
#endif  // RENDER_OPENGL_NACLGLFUNCTIONS_H
