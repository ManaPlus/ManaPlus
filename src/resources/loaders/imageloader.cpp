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

#include "resources/imagehelper.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/dye/dye.h"

#include "utils/checkutils.h"
#include "utils/virtfs.h"
#include "utils/virtfsrwops.h"

#include "debug.h"

namespace
{
    struct DyedImageLoader final
    {
        A_DEFAULT_COPY(DyedImageLoader)

        std::string path;
        static Resource *load(const void *const v)
        {
            BLOCK_START("DyedImageLoader::load")
            if (!v)
            {
                BLOCK_END("DyedImageLoader::load")
                return nullptr;
            }

            const DyedImageLoader *const rl
                = static_cast<const DyedImageLoader *const>(v);

            std::string path1 = rl->path;
            const size_t p = path1.find('|');
            Dye *d = nullptr;
            if (p != std::string::npos)
            {
                d = new Dye(path1.substr(p + 1));
                path1 = path1.substr(0, p);
            }
            SDL_RWops *const rw = VirtFs::RWopsOpenRead(path1.c_str());
            if (!rw)
            {
                delete d;
                reportAlways("Image loading error: %s", path1.c_str());
                reportAlways("Physfs error: %s", VirtFs::getLastError());
                BLOCK_END("DyedImageLoader::load")
                return nullptr;
            }
            Resource *const res = d ? imageHelper->load(rw, *d)
                : imageHelper->load(rw);
            delete d;
            if (!res)
                reportAlways("Image loading error: %s", path1.c_str());
            BLOCK_END("DyedImageLoader::load")
            return res;
        }
    };
}  // namespace

Image *Loader::getImage(const std::string &idPath)
{
    DyedImageLoader rl = { idPath };
    return static_cast<Image*>(resourceManager->get(idPath,
        DyedImageLoader::load, &rl));
}
