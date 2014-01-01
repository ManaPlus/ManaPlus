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

#include "localconsts.h"

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

#ifdef WIN32
defName(wglGetExtensionsString);
#endif

#endif
