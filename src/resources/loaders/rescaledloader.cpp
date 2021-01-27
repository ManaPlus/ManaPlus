/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "resources/image/image.h"

#include "resources/loaders/rescaledloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace
{
    struct RescaledLoader final
    {
        A_DEFAULT_COPY(RescaledLoader)

        const Image *const image;
        const int width;
        const int height;
        static Resource *load(const void *const v)
        {
            if (v == nullptr)
                return nullptr;
            const RescaledLoader *const rl
                = static_cast<const RescaledLoader *>(v);
            if (rl->image == nullptr)
                return nullptr;
            Image *const rescaled = rl->image->SDLgetScaledImage(
                rl->width, rl->height);
            if (rescaled == nullptr)
            {
                reportAlways("Rescale image failed: %s",
                    rl->image->mIdPath.c_str())
                return nullptr;
            }
            return rescaled;
        }
    };
}  // namespace

Image *Loader::getRescaled(const Image *const image,
                           const int width,
                           const int height)
{
    if (image == nullptr)
        return nullptr;

    const std::string idPath = image->mIdPath + strprintf(
        "_rescaled%dx%d", width, height);
    const RescaledLoader rl = { image, width, height };
    return static_cast<Image *>(
        ResourceManager::get(idPath, RescaledLoader::load, &rl));
}
