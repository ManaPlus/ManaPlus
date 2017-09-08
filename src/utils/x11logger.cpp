/*
 *  The ManaPlus Client
 *  Copyright (C) 2017  The ManaPlus Developers
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

#ifdef USE_SDL2
#ifdef USE_X11

#include "utils/x11logger.h"

#include "logger.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_events.h>
#include <SDL_syswm.h>
PRAGMA48(GCC diagnostic pop)

#include "utils/stringutils.h"

#include "debug.h"

#define logType(val, str) \
    case val: \
        typeStr = str; \
        break

bool X11Logger::logEvent(const SDL_Event &event)
{
    if (event.syswm.msg->subsystem != SDL_SYSWM_X11)
        return false;
    std::string typeStr;

    const int type = event.syswm.msg->msg.x11.event.type;
    switch (type)
    {
        logType(2, "KeyPress");
        logType(3, "KeyRelease");
        logType(4, "ButtonPress");
        logType(5, "ButtonRelease");
        logType(6, "MotionNotify");
        logType(7, "EnterNotify");
        logType(8, "LeaveNotify");
        logType(9, "FocusIn");
        logType(10, "FocusOut");
        logType(11, "KeymapNotify");
        logType(12, "Expose");
        logType(13, "GraphicsExpose");
        logType(14, "NoExpose");
        logType(15, "VisibilityNotify");
        logType(16, "CreateNotify");
        logType(17, "DestroyNotify");
        logType(18, "UnmapNotify");
        logType(19, "MapNotify");
        logType(20, "MapRequest");
        logType(21, "ReparentNotify");
        logType(22, "ConfigureNotify");
        logType(23, "ConfigureRequest");
        logType(24, "GravityNotify");
        logType(25, "ResizeRequest");
        logType(26, "CirculateNotify");
        logType(27, "CirculateRequest");
        logType(28, "PropertyNotify");
        logType(29, "SelectionClear");
        logType(30, "SelectionRequest");
        logType(31, "SelectionNotify");
        logType(32, "ColormapNotify");
        logType(33, "ClientMessage");
        logType(34, "MappingNotify");
        logType(35, "GenericEvent");
        default:
            typeStr = strprintf("Unknown: %d", type);
            break;
    }

    logger->log("event: SDL_SYSWMEVENT: X11: %s",
        typeStr.c_str());
    return true;
}

#endif  // USE_X11
#endif  // USE_SDL2
