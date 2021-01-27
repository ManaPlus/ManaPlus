/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "resources/memorymanager.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_video.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

MemoryManager memoryManager;

MemoryManager::MemoryManager()
{
}


int MemoryManager::getSurfaceSize(const SDL_Surface *const surface)
{
    if (surface == nullptr)
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

void MemoryManager::printMemory(const std::string &name,
                                const int level,
                                const int localSum,
                                const int childsSum)
{
    std::string str(level, ' ');
    if (childsSum > 0)
    {
        logger->log("%s%s: %d = %d + %d",
            str.c_str(),
            name.c_str(),
            localSum + childsSum,
            localSum,
            childsSum);
    }
    else
    {
        logger->log("%s%s: %d",
            str.c_str(),
            name.c_str(),
            localSum);
    }
}

void MemoryManager::printAllMemory(ChatTab *const tab A_DYECMD_UNUSED)
{
    if (logger == nullptr)
        return;

#ifdef DYECMD
    ResourceManager::calcMemory(0);
#else  // DYECMD

    if (tab != nullptr)
    {
        int sz = ResourceManager::calcMemory(0);
        // TRANSLATORS: memory usage chat message
        tab->chatLog(strprintf(_("Calculated memory usage: %d"), sz),
            ChatMsgType::BY_SERVER,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }
#endif  // DYECMD
}
