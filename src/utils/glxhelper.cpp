/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2018  The ManaPlus Developers
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

#include "utils/glxhelper.h"

#if defined(USE_OPENGL) && defined(USE_X11)

#include "logger.h"

#include "render/opengl/mglcheck.h"

#include "render/openglx/mglx.h"

#include "render/opengl/mgldefines.h"
RENDER_OPENGL_MGLDEFINES_H

#include "debug.h"

static int ErrorHandler(Display *d A_UNUSED, XErrorEvent *e A_UNUSED)
{
    return 0;
}

void *GlxHelper::createContext(const unsigned long window,
                               void *const display0,
                               const int major,
                               const int minor,
                               const int profile)
{
    Display *const display = static_cast<Display*>(display0);
    XSync(display, false);
    int (*handler)(Display *, XErrorEvent *) = XSetErrorHandler(ErrorHandler);
    void *context = mglXGetCurrentContext();
    if (!display)
        return context;
    if (isGLNull(mglXGetCurrentContext)
        || isGLNull(mglXCreateContextAttribs)
        || isGLNull(mglXChooseFBConfig))
    {
        logger->log("Can't change context, functions in driver "
            "not implemented");
        XSetErrorHandler(handler);
        return context;
    }
    if (!context)
    {
        logger->log("Can't change context, because current "
            "context not created");
        XSetErrorHandler(handler);
        return context;
    }
    int glxAttribs[] =
    {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_RED_SIZE, 3,
        GLX_GREEN_SIZE, 3,
        GLX_BLUE_SIZE, 2,
        GLX_DOUBLEBUFFER, 1,
        0
    };

    int fbcount = 0;
    GLXFBConfig *framebuffer_config = mglXChooseFBConfig(display,
        DefaultScreen(display),
        glxAttribs,
        &fbcount);

    if (!framebuffer_config || !fbcount)
    {
        logger->log("No correct fb profile found");
        XSetErrorHandler(handler);
        return nullptr;
    }
    logger->log("Found %d frame buffer contexts.", fbcount);

    int attribs[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB, major,
        GLX_CONTEXT_MINOR_VERSION_ARB, minor,
        GLX_CONTEXT_PROFILE_MASK_ARB, profile,
        0, 0
    };

    void *const context2 = mglXCreateContextAttribs(display,
        framebuffer_config[0], context, true, attribs);
    if (!context2)
    {
        logger->log("context %d.%d creation failed", major, minor);
        XSetErrorHandler(handler);
        return nullptr;
    }

    XSync(display, false);
    XSetErrorHandler(handler);

    if (!mglXMakeCurrent(display, window, context2))
    {
        mglXDestroyContext(display, context2);
        logger->log("make current context %d.%d failed", major, minor);
        return nullptr;
    }

    if (mglXGetCurrentContext() != context2)
    {
        mglXDestroyContext(display, context2);
        logger->log("context cant be changed to %d.%d.", major, minor);
        return nullptr;
    }

//  do not delete SDL context, because on exit it will crash
//    mglXDestroyContext(display, context);
    logger->log("Context for %d.%d created", major, minor);
    return context2;
}

bool GlxHelper::makeCurrent(const unsigned long window,
                            void *const display,
                            void *const context)
{
    if (!display)
        return false;
    return mglXMakeCurrent(static_cast<Display*>(display), window, context);
}

#endif  // defined(USE_OPENGL) && defined(USE_X11)
