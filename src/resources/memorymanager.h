/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
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

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <string>

#include "localconsts.h"

class ChatTab;

struct SDL_Surface;

class MemoryManager final
{
    public:
        MemoryManager();

        A_DELETE_COPY(MemoryManager)

        ~MemoryManager()
        { }

        static int getSurfaceSize(const SDL_Surface *const surface);

        static void printMemory(const std::string &name,
                                const int level,
                                const int localSum,
                                const int childsSum);

        static void printAllMemory(ChatTab *const tab);
};

extern MemoryManager memoryManager;

#endif  // MEMORYMANAGER_H
