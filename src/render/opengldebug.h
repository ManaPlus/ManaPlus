/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#ifndef RENDER_OPENGLDEBUG_H
#define RENDER_OPENGLDEBUG_H

#include "render/mgl.h"

#if defined(DEBUG_OPENGL) && defined(USE_OPENGL)
#define GLDEBUG_START(text) if (mglPushGroupMarker) \
    mglPushGroupMarker(sizeof(text), text);
#define GLDEBUG_END() if (mglPopGroupMarker) \
    mglPopGroupMarker();
#else
#define GLDEBUG_START(text)
#define GLDEBUG_END()
#endif

#endif  // RENDER_OPENGLDEBUG_H
