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

#include "resources/memorymanager.h"

#include "logger.h"

#include "gui/widgets/tabs/chat/chattab.h"

#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <SDL_video.h>
#include <string>

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
    std::string str(level, ' ');
    logger->log("%s%d = %d + %d",
        str.c_str(),
        localSum + childsSum,
        localSum,
        childsSum);
}

void MemoryManager::printAllMemory(ChatTab *const tab A_DYECMD_UNUSED)
{
    if (!logger)
        return;
    int sz = 0;
    if (resourceManager)
        sz += resourceManager->calcMemory(0);
#ifndef DYECMD
    if (tab)
    {
        // TRANSLATORS: memory usage chat message
        tab->chatLog(strprintf(_("Calculated memory usage: %d"), sz),
            ChatMsgType::BY_SERVER);
    }
#endif
}
