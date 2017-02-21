/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "resources/sdlmusic.h"

#include "fs/virtfs.h"
#include "fs/virtfsrwops.h"

#include "resources/loaders/musicloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/checkutils.h"

#include "debug.h"

namespace
{
    struct ResourceLoader final
    {
        A_DEFAULT_COPY(ResourceLoader)

        const std::string path;

        static Resource *load(const void *const v)
        {
            if (!v)
                return nullptr;
            const ResourceLoader *const
                rl = static_cast<const ResourceLoader *const>(v);
            SDL_RWops *const rw = VirtFs::RWopsOpenRead(rl->path);
            if (!rw)
            {
                reportAlways("Physfs error: %s", VirtFs::getLastError());
                reportAlways("Error loading resource: %s",
                    rl->path.c_str());
                return nullptr;
            }
#ifdef USE_SDL2
            if (Mix_Music *const music = Mix_LoadMUSType_RW(rw, MUS_OGG, 1))
            {
                return new SDLMusic(music, nullptr, rl->path);
            }
#else  // USE_SDL2

            // Mix_LoadMUSType_RW was added without version changed in SDL1.2 :(
            if (Mix_Music *const music = Mix_LoadMUS_RW(rw))
            {
                return new SDLMusic(music, rw, rl->path);
            }
#endif  // USE_SDL2
            else
            {
                logger->log("Error, failed to load music: %s", Mix_GetError());
                return nullptr;
            }
        }
    };
}  // namespace

SDLMusic *Loader::getMusic(const std::string &idPath)
{
    ResourceLoader rl = { idPath };
    return static_cast<SDLMusic*>(resourceManager->get(
        idPath, ResourceLoader::load, &rl));
}
