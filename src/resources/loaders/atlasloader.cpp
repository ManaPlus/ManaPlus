/*
 *  The ManaPlus Client
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

#ifdef USE_OPENGL

#include "resources/atlas/atlasmanager.h"
#include "resources/atlas/atlasresource.h"

#include "resources/loaders/atlasloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/checkutils.h"

#include "debug.h"

struct AtlasLoader final
{
    A_DEFAULT_COPY(AtlasLoader)

    const std::string name;
    const StringVect *const files;

    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const AtlasLoader *const rl = static_cast<const AtlasLoader *>(v);
        AtlasResource *const resource = AtlasManager::loadTextureAtlas(
            rl->name, *rl->files);
        if (!resource)
            reportAlways("Atlas creation error: %s", rl->name.c_str());
        return resource;
    }
};

Resource *Loader::getAtlas(const std::string &name,
                           const StringVect &files)
{
    AtlasLoader rl = { name, &files };
    return ResourceManager::get("atlas_" + name, AtlasLoader::load, &rl);
}

#endif  // USE_OPENGL
