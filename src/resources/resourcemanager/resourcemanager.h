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

#ifndef RESOURCES_RESOURCEMANAGER_RESOURCEMANAGER_H
#define RESOURCES_RESOURCEMANAGER_RESOURCEMANAGER_H

#include "resources/resourcefunctiontypes.h"

#if defined(DEBUG_DUMP_LEAKS) || defined(UNITTESTS)
#include "resources/resourcetypes.h"

#include <set>
#endif  // defined(DEBUG_DUMP_LEAKS) || defined(UNITTESTS)

#include "localconsts.h"

class Resource;

struct SDL_Surface;

namespace ResourceManager
{
    /**
     * Cleans up remaining resources, warning about resources
     * that were still referenced.
     */
    void deleteResourceManager();

    /**
     * Creates a resource and adds it to the resource map.
     *
     * @param idPath The resource identifier path.
     * @param fun    A function for generating the resource.
     * @param data   Extra parameters for the generator.
     * @return A valid resource or <code>NULL</code> if the resource could
     *         not be generated.
     */
    Resource *get(const std::string &idPath,
                  generator fun,
                  const void *const data) A_WARN_UNUSED;

    Resource *getFromCache(const std::string &idPath) A_WARN_UNUSED;

    Resource *getFromCache(const std::string &filename,
                           const int variant) A_WARN_UNUSED;

    bool addResource(const std::string &idPath,
                     Resource *const resource);

    /**
     * Releases a resource, placing it in the set of orphaned resources.
     */
    void release(Resource *const res);

    void clearDeleted(const bool full);

    void decRefDelete(Resource *const res);

    /**
     * Move resource to deleted resources list.
     */
    void moveToDeleted(Resource *const res);

    void scheduleDelete(SDL_Surface *const surface);

    void clearScheduled();

    /**
     * Deletes the class instance if it exists.
     */
    void deleteInstance();

    int size() noexcept2 A_WARN_UNUSED;

#if defined(DEBUG_DUMP_LEAKS) || defined(UNITTESTS)
    Resources &getResources() A_WARN_UNUSED;

    Resources &getOrphanedResources() A_WARN_UNUSED;

    const std::set<Resource*> &getDeletedResources() A_WARN_UNUSED;
#endif  // defined(DEBUG_DUMP_LEAKS) || defined(UNITTESTS)

    bool cleanOrphans(const bool always);

    void cleanProtected();

    bool isInCache(const std::string &idPath) A_WARN_UNUSED;

    Resource *getTempResource(const std::string &idPath) A_WARN_UNUSED;

    void clearCache();

    int calcMemoryLocal();

    int calcMemoryChilds(const int level);

    int calcMemory(const int level);

    void cleanUp(Resource *const resource);

    void logResource(const Resource *const res);

    void logResources(const std::string &msg);
}  // namespace ResourceManager

#endif  // RESOURCES_RESOURCEMANAGER_RESOURCEMANAGER_H
