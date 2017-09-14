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

#ifdef USE_X11

#include "utils/x11logger.h"

#include "logger.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_events.h>
#include <SDL_syswm.h>
PRAGMA48(GCC diagnostic pop)

#include "utils/cast.h"
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

#ifdef USE_SDL2
    XEvent &xEvent = event.syswm.msg->msg.x11.event;
#else  // USE_SDL2

    XEvent &xEvent = event.syswm.msg->event.xevent;
#endif  // USE_SDL2

    const int type = xEvent.type;

    switch (type)
    {
        logType(2, strprintf("KeyPress: %u,%u %d,%d",
            xEvent.xkey.keycode,
            xEvent.xkey.state,
            xEvent.xkey.x,
            xEvent.xkey.y));
        logType(3, strprintf("KeyRelease: %u,%u %d,%d",
            xEvent.xkey.keycode,
            xEvent.xkey.state,
            xEvent.xkey.x,
            xEvent.xkey.y));
        logType(4, strprintf("ButtonPress: %u,%u %d,%d",
            xEvent.xbutton.button,
            xEvent.xbutton.state,
            xEvent.xbutton.x,
            xEvent.xbutton.y));
        logType(5, strprintf("ButtonRelease: %u,%u %d,%d",
            xEvent.xbutton.button,
            xEvent.xbutton.state,
            xEvent.xbutton.x,
            xEvent.xbutton.y));
        logType(6, strprintf("MotionNotify: %d,%u %d,%d",
            CAST_S32(xEvent.xmotion.is_hint),
            xEvent.xmotion.state,
            xEvent.xmotion.x,
            xEvent.xmotion.y));
        logType(7, strprintf("EnterNotify: %d,%d,%d,%u, %d,%d",
            xEvent.xcrossing.mode,
            xEvent.xcrossing.detail,
            xEvent.xcrossing.focus ? 1 : 0,
            xEvent.xcrossing.state,
            xEvent.xcrossing.x,
            xEvent.xcrossing.y));
        logType(8, strprintf("LeaveNotify: %d,%d,%d,%u, %d,%d",
            xEvent.xcrossing.mode,
            xEvent.xcrossing.detail,
            xEvent.xcrossing.focus ? 1 : 0,
            xEvent.xcrossing.state,
            xEvent.xcrossing.x,
            xEvent.xcrossing.y));
        logType(9, strprintf("FocusIn: %d,%d",
            xEvent.xfocus.mode,
            xEvent.xfocus.detail));
        logType(10, strprintf("FocusOut: %d,%d",
            xEvent.xfocus.mode,
            xEvent.xfocus.detail));
        case 11:
            typeStr = "KeymapNotify: ";
            for (int f = 0; f < 32; f ++)
            {
                typeStr.append(strprintf("%u ",
                    CAST_U32(xEvent.xkeymap.key_vector[f])));
            }
            break;
        logType(12, strprintf("Expose: %d,%d,%d,%d %d",
            xEvent.xexpose.x,
            xEvent.xexpose.y,
            xEvent.xexpose.width,
            xEvent.xexpose.height,
            xEvent.xexpose.count));
        logType(13, strprintf("GraphicsExpose: %d,%d,%d,%d %d,%d,%d",
            xEvent.xgraphicsexpose.x,
            xEvent.xgraphicsexpose.y,
            xEvent.xgraphicsexpose.width,
            xEvent.xgraphicsexpose.height,
            xEvent.xgraphicsexpose.count,
            xEvent.xgraphicsexpose.major_code,
            xEvent.xgraphicsexpose.minor_code));
        logType(14, strprintf("NoExpose: %d,%d",
            xEvent.xnoexpose.major_code,
            xEvent.xnoexpose.minor_code));
        logType(15, strprintf("VisibilityNotify: %d",
            xEvent.xvisibility.state));
        logType(16, strprintf("CreateNotify: %d,%d,%d,%d %d,%d",
            xEvent.xcreatewindow.x,
            xEvent.xcreatewindow.y,
            xEvent.xcreatewindow.width,
            xEvent.xcreatewindow.height,
            xEvent.xcreatewindow.border_width,
            xEvent.xcreatewindow.override_redirect ? 1 : 0));
        logType(17, "DestroyNotify");
        logType(18, strprintf("UnmapNotify: %d",
            xEvent.xunmap.from_configure ? 1: 0));
        logType(19, strprintf("MapNotify: %d",
            xEvent.xmap.override_redirect ? 1 : 0));
        logType(20, "MapRequest");
        logType(21, strprintf("ReparentNotify: %d,%d, %d",
            xEvent.xreparent.x,
            xEvent.xreparent.y,
            xEvent.xreparent.override_redirect ? 1 : 0));
        logType(22, strprintf("ConfigureNotify: %d,%d %d,%d %d,%d",
            xEvent.xconfigure.x,
            xEvent.xconfigure.y,
            xEvent.xconfigure.width,
            xEvent.xconfigure.height,
            xEvent.xconfigure.border_width,
            xEvent.xconfigure.override_redirect ? 1 : 0));
        logType(23, strprintf("ConfigureRequest: %d,%d %d,%d %d,%d",
            xEvent.xconfigurerequest.x,
            xEvent.xconfigurerequest.y,
            xEvent.xconfigurerequest.width,
            xEvent.xconfigurerequest.height,
            xEvent.xconfigurerequest.border_width,
            xEvent.xconfigurerequest.detail));
        logType(24, strprintf("GravityNotify: %d,%d",
            xEvent.xgravity.x,
            xEvent.xgravity.y));
        logType(25, strprintf("ResizeRequest: %d,%d",
            xEvent.xresizerequest.width,
            xEvent.xresizerequest.height));
        logType(26, strprintf("CirculateNotify: %d",
            xEvent.xcirculate.place));
        logType(27, strprintf("CirculateRequest: %d",
            xEvent.xcirculaterequest.place));
        logType(28, strprintf("PropertyNotify: %u, %d",
            CAST_U32(xEvent.xproperty.atom),
            xEvent.xproperty.state));
        logType(29, strprintf("SelectionClear: %u",
            CAST_U32(xEvent.xselectionclear.selection)));
        logType(30, strprintf("SelectionRequest: %u,%u,%u",
            CAST_U32(xEvent.xselectionrequest.selection),
            CAST_U32(xEvent.xselectionrequest.target),
            CAST_U32(xEvent.xselectionrequest.property)));
        logType(31, strprintf("SelectionNotify: %u,%u,%u",
            CAST_U32(xEvent.xselection.selection),
            CAST_U32(xEvent.xselection.target),
            CAST_U32(xEvent.xselection.property)));
        logType(32, strprintf("ColormapNotify: %u,%d",
            CAST_U32(xEvent.xcolormap.colormap),
//            xEvent.xcolormap.new ? 1 : 0,
            xEvent.xcolormap.state));
        case 33:
            typeStr = strprintf("ClientMessage: %u,%d (",
                CAST_U32(xEvent.xclient.message_type),
                xEvent.xclient.format);
            for (int f = 0; f < 20; f ++)
                typeStr.append(strprintf("%c", xEvent.xclient.data.b[f]));
            typeStr.append(")");
            break;
        logType(34, strprintf("MappingNotify: %d,%d,%d",
            xEvent.xmapping.request,
            xEvent.xmapping.first_keycode,
            xEvent.xmapping.count));
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
