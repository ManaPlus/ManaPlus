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

#include "resources/imageset.h"

#include "resources/image/image.h"

#include "resources/loaders/subimagesetloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/stringutils.h"

#include "debug.h"

namespace
{
    struct SubImageSetLoader final
    {
        A_DELETE_COPY(SubImageSetLoader)

        Image *const parent;
        int width;
        int height;
        static Resource *load(const void *const v)
        {
            if (!v)
                return nullptr;

            const SubImageSetLoader *const
                rl = static_cast<const SubImageSetLoader *const>(v);

            if (!rl->parent)
                return nullptr;
            ImageSet *const res = new ImageSet(rl->parent,
                rl->width, rl->height);
            return res;
        }
    };
}  // namespace

ImageSet *Loader::getSubImageSet(Image *const parent,
                                 const int width,
                                 const int height)
{
    if (!parent)
        return nullptr;

    const SubImageSetLoader rl = { parent, width, height };
    std::string str = std::string(
        parent->getIdPath()).append(
        ", set[").append(toString(
        width)).append(
        "x").append(toString(
        height)).append(
        "]");
    return static_cast<ImageSet*>(resourceManager->get(str,
        SubImageSetLoader::load, &rl));
}
