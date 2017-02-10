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

#ifndef UTILS_DEBUGMEMORYOBJECT_H
#define UTILS_DEBUGMEMORYOBJECT_H

#ifdef DEBUG_PHYSFS

#include "utils/stringutils.h"

#include "localconsts.h"

namespace VirtFs
{
    struct DebugMemoryObject final
    {
        DebugMemoryObject(const char *const name,
                          const char *const file,
                          const unsigned int line) :
            mName(name),
            mAddFile(strprintf("%s:%u", file, line))
        {
        }

        A_DELETE_COPY(DebugMemoryObject)

        std::string mName;
        std::string mAddFile;
    };
}  // VirtFs

#endif  // DEBUG_PHYSFS
#endif  // UTILS_DEBUGMEMORYOBJECT_H
