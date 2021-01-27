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

#include "resources/imageset.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"
#include "resources/loaders/imagesetloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/checkutils.h"
#include "utils/stringutils.h"

#include "debug.h"

struct ImageSetLoader final
{
    const std::string path;
    const int w;
    const int h;

    A_DEFAULT_COPY(ImageSetLoader)

    static Resource *load(const void *const v)
    {
        if (v == nullptr)
            return nullptr;

        const ImageSetLoader *const
            rl = static_cast<const ImageSetLoader *>(v);

        Image *const img = Loader::getImage(rl->path);
        if (img == nullptr)
        {
            reportAlways("Image loading error: %s", rl->path.c_str())
            return nullptr;
        }
        ImageSet *const res = new ImageSet(img, rl->w, rl->h, 0, 0);
        img->decRef();
        return res;
    }
};

ImageSet *Loader::getImageSet(const std::string &imagePath,
                              const int w,
                              const int h)
{
    ImageSetLoader rl = { imagePath, w, h };
    const std::string str = std::string(
        imagePath).append(
        "[").append(toString(
        w)).append(
        "x").append(toString(
        h)).append(
        "]");
    return static_cast<ImageSet*>(ResourceManager::get(str,
        ImageSetLoader::load, &rl));
}
