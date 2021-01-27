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

#include "resources/loaders/subimageloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace
{
    struct SubImageLoader final
    {
        A_DEFAULT_COPY(SubImageLoader)

        Image *const parent;
        const int x;
        const int y;
        const int width;
        const int height;
        static Resource *load(const void *const v)
        {
            if (v == nullptr)
                return nullptr;

            const SubImageLoader *const
                rl = static_cast<const SubImageLoader *>(v);
            if (rl->parent == nullptr)
                return nullptr;

            Image *const res = rl->parent->getSubImage(rl->x, rl->y,
                rl->width, rl->height);
            if (res == nullptr)
            {
                reportAlways("SubImage loading error: %s",
                    rl->parent->mSource.c_str())
            }
            return res;
        }
    };
}  // namespace

Image *Loader::getSubImage(Image *const parent,
                           const int x,
                           const int y,
                           const int width,
                           const int height)
{
    if (parent == nullptr)
        return nullptr;

    const SubImageLoader rl = { parent, x, y, width, height};

    const std::string str = std::string(parent->mIdPath).append(
        ",[").append(
        toString(x)).append(
        ",").append(
        toString(y)).append(
        ",").append(
        toString(width)).append(
        "x").append(
        toString(height)).append(
        "]");
    return static_cast<Image*>(ResourceManager::get(str,
        SubImageLoader::load, &rl));
}
