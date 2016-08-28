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

#include "configuration.h"

#ifdef USE_OPENGL
#include "resources/image/image.h"
#endif  // USE_OPENGL

#include "resources/imageset.h"

#include "resources/memorymanager.h"

#include "resources/sprite/spritedef.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/physfsrwops.h"

#include <SDL_image.h>
#include <sstream>

#include <sys/time.h>

#include "debug.h"

ResourceManager *resourceManager = nullptr;
ResourceManager *ResourceManager::instance = nullptr;

ResourceManager::ResourceManager() :
    deletedSurfaces(),
    mResources(),
    mOrphanedResources(),
    mDeletedResources(),
    mOldestOrphan(0),
    mDestruction(0)
{
    logger->log1("Initializing resource manager...");
}

ResourceManager::~ResourceManager()
{
    mDestruction = true;
    mResources.insert(mOrphanedResources.begin(), mOrphanedResources.end());

    // Release any remaining spritedefs first because they depend on image sets
    ResourceIterator iter = mResources.begin();

#ifdef DEBUG_LEAKS
    while (iter != mResources.end())
    {
        if (iter->second)
        {
            if (iter->second->getRefCount())
            {
                logger->log(std::string("ResourceLeak: ").append(
                    iter->second->getIdPath()).append(" (").append(
                    toString(iter->second->getRefCount())).append(")"));
            }
        }
        ++iter;
    }

    iter = mResources.begin();
#endif

    while (iter != mResources.end())
    {
#ifdef DEBUG_LEAKS
        if (iter->second && iter->second->getRefCount())
        {
            ++iter;
            continue;
        }
#endif
        if (dynamic_cast<SpriteDef*>(iter->second))
        {
            cleanUp(iter->second);
            const ResourceIterator toErase = iter;
            ++iter;
            mResources.erase(toErase);
        }
        else
        {
            ++iter;
        }
    }

    // Release any remaining image sets first because they depend on images
    iter = mResources.begin();
    while (iter != mResources.end())
    {
#ifdef DEBUG_LEAKS
        if (iter->second && iter->second->getRefCount())
        {
            ++iter;
            continue;
        }
#endif
        if (dynamic_cast<ImageSet*>(iter->second))
        {
            cleanUp(iter->second);
            const ResourceIterator toErase = iter;
            ++iter;
            mResources.erase(toErase);
        }
        else
        {
            ++iter;
        }
    }

    // Release remaining resources, logging the number of dangling references.
    iter = mResources.begin();
    while (iter != mResources.end())
    {
#ifdef DEBUG_LEAKS
        if (iter->second && iter->second->getRefCount())
        {
            ++iter;
            continue;
        }
#endif
        if (iter->second)
        {
            cleanUp(iter->second);
            const ResourceIterator toErase = iter;
            ++iter;
            mResources.erase(toErase);
        }
        else
        {
            ++iter;
        }
    }
    clearDeleted();
    clearScheduled();
}

void ResourceManager::init()
{
    if (!resourceManager)
        resourceManager = new ResourceManager;
}

void ResourceManager::cleanUp(Resource *const res)
{
    if (!res)
        return;

    if (res->mRefCount > 0)
    {
        logger->log("ResourceManager::~ResourceManager() cleaning up %u "
                "reference%s to %s",
                res->mRefCount,
                (res->mRefCount == 1) ? "" : "s",
                res->mIdPath.c_str());
    }

    delete res;
#ifdef DEBUG_LEAKS
    cleanOrphans(true);
#endif
}

void ResourceManager::cleanProtected()
{
    ResourceIterator iter = mResources.begin();
    while (iter != mResources.end())
    {
        Resource *const res = iter->second;
        if (!res)
        {
            ++ iter;
            continue;
        }
        if (res->isProtected())
        {
            res->setProtected(false);
            res->decRef();
            iter = mResources.begin();
            continue;
        }

        ++ iter;
    }
}

bool ResourceManager::cleanOrphans(const bool always)
{
    timeval tv;
    gettimeofday(&tv, nullptr);
    // Delete orphaned resources after 30 seconds.
    time_t oldest = static_cast<time_t>(tv.tv_sec);
    const time_t threshold = oldest - 30;

    if (mOrphanedResources.empty() || (!always && mOldestOrphan >= threshold))
        return false;

    bool status(false);
    ResourceIterator iter = mOrphanedResources.begin();
    while (iter != mOrphanedResources.end())
    {
        Resource *const res = iter->second;
        if (!res)
        {
            ++iter;
            continue;
        }
        const time_t t = res->mTimeStamp;
        if (!always && t >= threshold)
        {
            if (t < oldest)
                oldest = t;
            ++ iter;
        }
        else
        {
            logResource(res);
            const ResourceIterator toErase = iter;
            ++iter;
            mOrphanedResources.erase(toErase);
            delete res;  // delete only after removal from list,
                         // to avoid issues in recursion
            status = true;
        }
    }

    mOldestOrphan = oldest;
    return status;
}

void ResourceManager::logResource(const Resource *const res)
{
    if (!res)
        return;
#ifdef USE_OPENGL
    const Image *const image = dynamic_cast<const Image *const>(res);
    if (image)
    {
        std::string src = image->getSource();
        const int count = image->getRefCount();
        if (count)
            src.append(" ").append(toString(count));
        logger->log("resource(%s, %u) %s", res->mIdPath.c_str(),
            image->getGLImage(), src.c_str());
    }
    else
    {
        logger->log("resource(%s)", res->mIdPath.c_str());
    }
#else
    logger->log("resource(%s)", res->mIdPath.c_str());
#endif
}

void ResourceManager::clearDeleted(const bool full)
{
    bool status(true);
    logger->log1("clear deleted");
    while (status)
    {
        status = false;
        std::set<Resource*>::iterator resDelIter = mDeletedResources.begin();
        while (resDelIter != mDeletedResources.end())
        {
            if (!(*resDelIter)->getRefCount())
            {
                status = true;
                Resource *res = *resDelIter;
                logResource(res);
                mDeletedResources.erase(resDelIter);
                delete res;
                break;
            }
            ++ resDelIter;
        }
    }
    if (full && !mDeletedResources.empty())
    {
        logger->log1("leaks in deleted");
        std::set<Resource*>::iterator resDelIter = mDeletedResources.begin();
        while (resDelIter != mDeletedResources.end())
        {
            logResource(*resDelIter);

            // for debug only
//            delete *resDelIter;
            // for debug only

            ++ resDelIter;
        }
    }
}
bool ResourceManager::setWriteDir(const std::string &path) const
{
    return static_cast<bool>(PhysFs::setWriteDir(path.c_str()));
}

bool ResourceManager::addToSearchPath(const std::string &path,
                                      const Append append) const
{
    logger->log("Adding to PhysicsFS: %s (%s)", path.c_str(),
        append == Append_true ? "append" : "prepend");
    if (!PhysFs::addToSearchPath(path.c_str(),
        append == Append_true ? 1 : 0))
    {
        logger->log("Error: %s: addToSearchPath failed: %s",
            path.c_str(),
            PHYSFS_getLastError());
        return false;
    }
    return true;
}

bool ResourceManager::removeFromSearchPath(const std::string &path) const
{
    logger->log("Removing from PhysicsFS: %s", path.c_str());
    if (!PhysFs::removeFromSearchPath(path.c_str()))
    {
        logger->log("Error: %s: removeFromSearchPath failed: %s",
            path.c_str(),
            PHYSFS_getLastError());
        return false;
    }
    return true;
}

void ResourceManager::searchAndAddArchives(const std::string &restrict path,
                                           const std::string &restrict ext,
                                           const Append append) const
{
    const char *const dirSep = dirSeparator;
    char **list = PhysFs::enumerateFiles(path.c_str());

    for (char **i = list; *i; i++)
    {
        const size_t len = strlen(*i);

        if (len > ext.length() && !ext.compare((*i) + (len - ext.length())))
        {
            const std::string file = path + (*i);
            const std::string realPath = std::string(
                PhysFs::getRealDir(file.c_str()));
            addToSearchPath(std::string(realPath).append(
                dirSep).append(file), append);
        }
    }

    PhysFs::freeList(list);
}

void ResourceManager::searchAndRemoveArchives(const std::string &restrict path,
                                              const std::string &restrict ext)
                                              const
{
    const char *const dirSep = dirSeparator;
    char **list = PhysFs::enumerateFiles(path.c_str());

    for (char **i = list; *i; i++)
    {
        const size_t len = strlen(*i);
        if (len > ext.length() && !ext.compare((*i) + (len - ext.length())))
        {
            const std::string file = path + (*i);
            const std::string realPath = std::string(
                PhysFs::getRealDir(file.c_str()));
            removeFromSearchPath(std::string(realPath).append(
                dirSep).append(file));
        }
    }

    PhysFs::freeList(list);
}

bool ResourceManager::addResource(const std::string &idPath,
                                  Resource *const resource)
{
    if (resource)
    {
        resource->incRef();
        resource->mIdPath = idPath;
#ifdef DEBUG_IMAGES
        logger->log("set name %p, %s", static_cast<void*>(resource),
            resource->mIdPath.c_str());
#endif
        mResources[idPath] = resource;
        return true;
    }
    return false;
}

Resource *ResourceManager::getFromCache(const std::string &filename,
                                        const int variant)
{
    std::stringstream ss;
    ss << filename << "[" << variant << "]";
    return getFromCache(ss.str());
}

bool ResourceManager::isInCache(const std::string &idPath) const
{
    const ResourceCIterator &resIter = mResources.find(idPath);
    return (resIter != mResources.end() && resIter->second);
}

Resource *ResourceManager::getTempResource(const std::string &idPath)
{
    const ResourceCIterator &resIter = mResources.find(idPath);
    if (resIter != mResources.end())
    {
        Resource *const res = resIter->second;
        if (resIter->second)
            return res;
    }
    return nullptr;
}

Resource *ResourceManager::getFromCache(const std::string &idPath)
{
    // Check if the id exists, and return the value if it does.
    ResourceIterator resIter = mResources.find(idPath);
    if (resIter != mResources.end())
    {
        if (resIter->second)
            resIter->second->incRef();
        return resIter->second;
    }

    resIter = mOrphanedResources.find(idPath);
    if (resIter != mOrphanedResources.end())
    {
        Resource *const res = resIter->second;
        mResources.insert(*resIter);
        mOrphanedResources.erase(resIter);
        if (res)
            res->incRef();
        return res;
    }
    return nullptr;
}

Resource *ResourceManager::get(const std::string &idPath,
                               generator fun,
                               const void *const data)
{
#ifndef DISABLE_RESOURCE_CACHING
    Resource *resource = getFromCache(idPath);
    if (resource)
        return resource;
    resource = fun(data);

    if (resource)
    {
        resource->incRef();
        resource->mIdPath = idPath;
#ifdef DEBUG_IMAGES
        logger->log("set name %p, %s", static_cast<void*>(resource),
            resource->mIdPath.c_str());
#endif
        mResources[idPath] = resource;
        cleanOrphans();
    }
    else
    {
        reportAlways("Error loading image: %s", idPath.c_str());
    }
#else
    Resource *resource = fun(data, idPath);

    if (resource)
    {
        resource->incRef();
        resource->mIdPath = idPath;
#ifdef DEBUG_IMAGES
        logger->log("set name %p, %s", static_cast<void*>(resource),
            resource->mIdPath.c_str());
#endif
    }
    else
    {
        reportAlways("Error loading image: " + idPath);
    }
#endif

    // Returns nullptr if the object could not be created.
    return resource;
}

void ResourceManager::release(Resource *const res)
{
    if (!res || mDestruction)
        return;

#ifndef DISABLE_RESOURCE_CACHING
    std::set<Resource*>::iterator resDelIter = mDeletedResources.find(res);
    if (resDelIter != mDeletedResources.end())
    {
        // we found zero counted image in deleted list. deleting it and exit.
        mDeletedResources.erase(resDelIter);
        delete res;
        return;
    }

    ResourceIterator resIter = mResources.find(res->mIdPath);

    if (resIter == mResources.end())
    {
        reportAlways("no resource in cache: %s",
            res->mIdPath.c_str());
        delete res;
        return;
    }
    if (resIter->second != res)
    {
        reportAlways("in cache other image: %s",
            res->mIdPath.c_str());
        delete res;
        return;
    }

    timeval tv;
    gettimeofday(&tv, nullptr);
    const time_t timestamp = static_cast<time_t>(tv.tv_sec);

    res->mTimeStamp = timestamp;
    if (mOrphanedResources.empty())
        mOldestOrphan = timestamp;

    mOrphanedResources.insert(*resIter);
    mResources.erase(resIter);
#else
    delete res;
#endif
}

void ResourceManager::moveToDeleted(Resource *const res)
{
    if (!res)
        return;

    bool found(false);
    const int count = res->getRefCount();
    if (count == 1)
        logResource(res);
    res->decRef();
    ResourceIterator resIter = mResources.find(res->mIdPath);
    if (resIter != mResources.end() && resIter->second == res)
    {
        mResources.erase(resIter);
        found = true;
    }
    else
    {
        resIter = mOrphanedResources.find(res->mIdPath);
        if (resIter != mOrphanedResources.end() && resIter->second == res)
        {
            mOrphanedResources.erase(resIter);
            found = true;
        }
    }
    if (found)
    {
        if (count > 1)
            mDeletedResources.insert(res);
        else
            delete res;
    }
}

void ResourceManager::decRefDelete(Resource *const res)
{
    if (!res)
        return;

    const int count = res->getRefCount();
    if (count == 1)
    {
        logResource(res);

        ResourceIterator resIter = mResources.find(res->mIdPath);
        if (resIter != mResources.end() && resIter->second == res)
        {
            mResources.erase(resIter);
        }
        else
        {
            resIter = mOrphanedResources.find(res->mIdPath);
            if (resIter != mOrphanedResources.end() && resIter->second == res)
                mOrphanedResources.erase(resIter);
        }

        delete res;
    }
    else
    {
        res->decRef();
    }
}

void ResourceManager::deleteInstance()
{
#ifdef DUMP_LEAKED_RESOURCES
    if (instance)
    {
        logger->log1("clean orphans start");
        instance->cleanProtected();
        while (instance->cleanOrphans(true))
            continue;
        logger->log1("clean orphans end");
        ResourceIterator iter = instance->mResources.begin();

        while (iter != instance->mResources.end())
        {
            const Resource *const res = iter->second;
            if (res)
            {
                if (res->getRefCount())
                {
                    logger->log(std::string("ResourceLeak: ").append(
                        res->getIdPath()).append(" (").append(toString(
                        res->getRefCount())).append(")"));
                }
            }
            ++iter;
        }
    }
#endif
    delete2(instance);
}

void ResourceManager::scheduleDelete(SDL_Surface *const surface)
{
    deletedSurfaces.insert(surface);
}

void ResourceManager::clearScheduled()
{
    BLOCK_START("ResourceManager::clearScheduled")
    FOR_EACH (std::set<SDL_Surface*>::iterator, i, deletedSurfaces)
        MSDL_FreeSurface(*i);
    deletedSurfaces.clear();
    BLOCK_END("ResourceManager::clearScheduled")
}

void ResourceManager::clearCache()
{
    cleanProtected();
    while (cleanOrphans(true))
        continue;
}

int ResourceManager::calcMemoryLocal() const
{
    int sz = sizeof(ResourceManager);
    FOR_EACH (std::set<SDL_Surface*>::iterator, it, deletedSurfaces)
    {
        sz += memoryManager.getSurfaceSize(*it);
    }
    return sz;
}

int ResourceManager::calcMemoryChilds(const int level) const
{
    int sz = 0;
    FOR_EACH (ResourceCIterator, it, mResources)
    {
        sz += static_cast<int>((*it).first.capacity());
        sz += (*it).second->calcMemory(level + 1);
    }
    FOR_EACH (ResourceCIterator, it, mOrphanedResources)
    {
        sz += static_cast<int>((*it).first.capacity());
        sz += (*it).second->calcMemory(level + 1);
    }
    FOR_EACH (std::set<Resource*>::const_iterator, it, mDeletedResources)
    {
        sz += (*it)->calcMemory(level + 1);
    }
    return sz;
}
