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

#ifndef RENDER_OPENGLX_MGLX_H
#define RENDER_OPENGLX_MGLX_H

#if defined(USE_OPENGL) && defined(USE_X11)

#include "render/openglx/mglxtypes.h"

#define defName(name) extern name##_t m##name
#include "render/openglx/mglx.hpp"
MGLX_DEFINE
#undef defName
#undef MGLX_DEFINE

#endif  // defined(USE_OPENGL) && defined(USE_X11)
#endif  // RENDER_OPENGLX_MGLX_H
