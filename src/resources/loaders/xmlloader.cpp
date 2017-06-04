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

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/checkutils.h"

#include "resources/loaders/xmlloader.h"

#include "debug.h"

namespace
{
    struct ResourceLoader final
    {
        A_DEFAULT_COPY(ResourceLoader)

        const std::string path;
        const UseVirtFs useResman;
        const SkipError skipError;

        static Resource *load(const void *const v)
        {
            if (!v)
                return nullptr;
            const ResourceLoader *const
                rl = static_cast<const ResourceLoader *>(v);
            Resource *const res = new XML::Document(rl->path,
                rl->useResman,
                rl->skipError);
            return res;
        }
    };

}  // namespace

XML::Document *Loader::getXml(const std::string &idPath,
                              const UseVirtFs useResman,
                              const SkipError skipError)
{
    ResourceLoader rl = { idPath, useResman, skipError };
    return static_cast<XML::Document*>(ResourceManager::get(
        "xml_" + idPath, ResourceLoader::load, &rl));
}

