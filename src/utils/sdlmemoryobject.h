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

#ifndef UTILS_SDLMEMORYOBJECT_H
#define UTILS_SDLMEMORYOBJECT_H

#ifdef DEBUG_SDL_SURFACES

#include "logger.h"

#include "utils/stringutils.h"

#include <string>

#include "localconsts.h"

struct SDLMemoryObject final
{
    SDLMemoryObject(const std::string &name, const char *const file,
                    const unsigned int line) :
        mName(name),
        mAddFile(strprintf("%s:%u", file, line)),
        mRemoveFile(),
        mCnt(1)
    {
    }

    std::string mName;
    std::string mAddFile;
    std::string mRemoveFile;
    int mCnt;
};

#endif  // DEBUG_SDL_SURFACES
#endif  // UTILS_SDLMEMORYOBJECT_H
