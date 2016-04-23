/*
 *  The ManaPlus Client
 *  Copyright (C) 2016  The ManaPlus Developers
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

#include "memorymanager.h"

#include <SDL_video.h>

#include "debug.h"

MemoryManager memoryManager;

MemoryManager::MemoryManager()
{
}


int MemoryManager::getSurfaceSize(const SDL_Surface *const surface)
{
    if (!surface)
        return 0;
    return CAST_S32(sizeof(SDL_Surface)) +
        CAST_S32(sizeof(SDL_PixelFormat)) +
        // aproximation for sizeof(SDL_BlitMap)
        28 +
        // pixels
        surface->w * surface->h * 4 +
        // private_hdata aproximation
        10;
}

void MemoryManager::printMemory(const int level A_UNUSED,
                                const int localSum A_UNUSED,
                                const int childsSum A_UNUSED)
{
}
