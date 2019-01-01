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

#ifndef RENDER_OPENGL_MGLDEFINES_H
#define RENDER_OPENGL_MGLDEFINES_H

#ifdef USE_OPENGL

#ifndef GL_NUM_EXTENSIONS
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#endif  // GL_NUM_EXTENSIONS

#ifndef GL_COMPRESSED_RGBA_ARB
#define GL_COMPRESSED_RGBA_ARB            0x84EE
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#define GL_COMPRESSED_RGBA_FXT1_3DFX      0x86B1
#endif  // GL_COMPRESSED_RGBA_ARB
#ifndef GL_MAX_ELEMENTS_VERTICES
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#endif  // GL_MAX_ELEMENTS_VERTICES

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
#endif  // GL_DEBUG_OUTPUT

#ifndef GL_DEBUG_TYPE_PUSH_GROUP
#define GL_DEBUG_TYPE_PUSH_GROUP          0x8269
#endif  // GL_DEBUG_TYPE_PUSH_GROUP
#ifndef GL_DEBUG_TYPE_POP_GROUP
#define GL_DEBUG_TYPE_POP_GROUP           0x826a
#endif  // GL_DEBUG_TYPE_POP_GROUP

#ifndef GL_EXT_debug_label
#define GL_PROGRAM_PIPELINE_OBJECT_EXT    0x8A4F
#define GL_PROGRAM_OBJECT_EXT             0x8B40
#define GL_SHADER_OBJECT_EXT              0x8B48
#define GL_BUFFER_OBJECT_EXT              0x9151
#define GL_QUERY_OBJECT_EXT               0x9153
#define GL_VERTEX_ARRAY_OBJECT_EXT        0x9154
#endif  // GL_EXT_debug_label

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#endif  // GL_ARRAY_BUFFER

#ifndef GL_STREAM_DRAW
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#endif  // GL_STREAM_DRAW

#ifndef GL_COMPILE_STATUS
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#endif  // GL_COMPILE_STATUS

#ifndef GL_DEPTH_CLAMP
#define GL_DEPTH_CLAMP                    0x864F
#define GL_RASTERIZER_DISCARD             0x8C89
#define GL_SAMPLE_MASK                    0x8E51
#endif  // GL_DEPTH_CLAMP

#ifndef GL_POLYGON_SMOOTH
#define GL_POLYGON_SMOOTH                 0x0B41
#endif  // GL_POLYGON_SMOOTH

#ifndef GL_DEPTH_BOUNDS_TEST_EXT
#define GL_DEPTH_BOUNDS_TEST_EXT          0x8890
#endif  // GL_DEPTH_BOUNDS_TEST_EXT

#ifndef GL_TEXTURE_COMPRESSION_HINT_ARB
#define GL_TEXTURE_COMPRESSION_HINT_ARB   0x84EF
#endif  // GL_TEXTURE_COMPRESSION_HINT_ARB

#ifndef GLX_CONTEXT_PROFILE_MASK_ARB
#define GLX_CONTEXT_PROFILE_MASK_ARB      0x9126
#endif  // GLX_CONTEXT_PROFILE_MASK_ARB

#ifndef GLX_CONTEXT_MAJOR_VERSION_ARB
#define GLX_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB     0x2092
#define GLX_CONTEXT_FLAGS_ARB             0x2094
#endif  // GLX_CONTEXT_MAJOR_VERSION_ARB

#ifndef GL_COMPRESSED_RGBA_BPTC_UNORM_ARB
#define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB 0x8E8C
#endif  // GL_COMPRESSED_RGBA_BPTC_UNORM_ARB

#endif  // USE_OPENGL

#endif  // RENDER_OPENGL_MGLDEFINES_H
