/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2019  The ManaPlus Developers
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

#ifndef UTILS_GLXHELPER_H
#define UTILS_GLXHELPER_H

#if defined(USE_OPENGL) && defined(USE_X11)

#include "localconsts.h"

namespace GlxHelper
{
    void *createContext(const unsigned long window,
                        void *const display,
                        const int major,
                        const int minor,
                        const int profile);

    bool makeCurrent(const unsigned long window,
                     void *const display,
                     void *const context);
}  // namespace GlxHelper

#endif  // defined(USE_OPENGL) && defined(USE_X11)
#endif  // UTILS_GLXHELPER_H
