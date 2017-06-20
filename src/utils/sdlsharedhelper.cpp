/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "utils/sdlsharedhelper.h"

#ifdef __native_client__
#include <ppapi/c/ppb_mouse_cursor.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/mouse_cursor.h>
#include <ppapi_simple/ps.h>
#endif  // __native_client__

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_events.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

void SDL::showCursor(const bool show)
{
#ifdef __native_client__
    PP_MouseCursor_Type cursor;
    if (show)
        cursor = PP_MOUSECURSOR_TYPE_POINTER;  // show default cursor
    else
        cursor = PP_MOUSECURSOR_TYPE_NONE;     // hide cursor

    pp::MouseCursor::SetCursor(
        pp::InstanceHandle(PSGetInstanceId()),
        cursor);
#endif  // __native_client__

    if (show)
        SDL_ShowCursor(SDL_ENABLE);
    else
        SDL_ShowCursor(SDL_DISABLE);
}
