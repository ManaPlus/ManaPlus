/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/checkutils.h"
#include "utils/physfsrwops.h"

#include "resources/loaders/xmlloader.h"

#include "debug.h"

namespace
{
    struct ResourceLoader final
    {
        std::string path;

        static Resource *load(const void *const v)
        {
            if (!v)
                return nullptr;
            const ResourceLoader *const
                rl = static_cast<const ResourceLoader *const>(v);
            SDL_RWops *const rw = MPHYSFSRWOPS_openRead(rl->path.c_str());
            if (!rw)
            {
                reportAlways("Error loading resource: %s",
                    rl->path.c_str());
                return nullptr;
            }
            Resource *const res = nullptr;
            return res;
        }
    };

}  // namespace

XML::Document *Loader::getXml(const std::string &idPath)
{
    ResourceLoader rl = { idPath};
    return static_cast<XML::Document*>(resourceManager->get(
        idPath, ResourceLoader::load, &rl));
}

