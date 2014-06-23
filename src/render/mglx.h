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

#ifndef RENDER_MGLX_H
#define RENDER_MGLX_H

#include "main.h"
#if defined(USE_OPENGL) && defined(USE_X11)

#include "render/mglxtypes.h"

#define defNameE(name) extern name##_t m##name

defNameE(glXCreateContext);
defNameE(glXGetCurrentContext);
defNameE(glXCreateContextAttribs);
defNameE(glXChooseFBConfig);

#undef defNameE

#endif  // defined(USE_OPENGL) && defined(USE_X11)
#endif  // RENDER_MGLX_H
