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

#ifndef RENDER_MGLFUNCTIONS_H
#define RENDER_MGLFUNCTIONS_H

#ifdef USE_OPENGL

#include "logger.h"

#ifdef WIN32
#define getFunction(name) wglGetProcAddress(name)
#elif defined ANDROID
#define getFunction(name) eglGetProcAddress(name)
#elif defined __APPLE__
#define getFunction(name) nullptr
#else
#define getFunction(name) glXGetProcAddress(\
    reinterpret_cast<const GLubyte*>(name))
#endif

#define assignFunction(func, name) \
    { \
        m##func = reinterpret_cast<func##_t>(getFunction(name)); \
        if (m##func == nullptr) \
            logger->log(std::string("function not found: ") + name); \
        else \
            logger->log(std::string("assigned function: ") + name); \
    }

#define assignFunctionEmu(func, name) \
    { \
        m##func = reinterpret_cast<func##_t>(getFunction(name)); \
        if (m##func == nullptr) \
        { \
            m##func = emu##func; \
            logger->log(std::string("emulated function: ") + name); \
        } \
        else \
        { \
            logger->log(std::string("assigned function: ") + name); \
        } \
    }

#define emulateFunction(func) m##func = emu##func; \
    logger->log("emulated function: #func")

#endif  // USE_OPENGL
#endif  // RENDER_MGLFUNCTIONS_H
