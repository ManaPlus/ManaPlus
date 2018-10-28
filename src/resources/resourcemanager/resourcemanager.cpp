/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifdef USE_OPENGL
#include "resources/image/image.h"
#endif  // USE_OPENGL

#include "resources/imageset.h"

#include "resources/memorymanager.h"

#include "resources/sprite/spritedef.h"

#include "utils/cast.h"
#include "utils/checkutils.h"
#include "utils/foreach.h"
#include "utils/stringutils.h"

#if !defined(DEBUG_DUMP_LEAKS) && !defined(UNITTESTS)
#include "resources/resourcetypes.h"
#endif  // defined(DEBUG_DUMP_LEAKS) || defined(UNITTESTS)

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifndef USE_OPENGL
#include <SDL_image.h>
#endif  // USE_OPENGL
PRAGMA48(GCC diagnostic pop)

#include <sstream>

#include <sys/time.h>

#include "debug.h"

namespace ResourceManager
{

std::set<SDL_Surface*> deletedSurfaces;
Resources mResources;
Resources mOrphanedResources;
std::set<Resource*> mDeletedResources;
time_t mOldestOrphan = 0;
bool mDestruction = false;

void deleteResourceManager()
{
    mDestruction = true;
    mResources.insert(mOrphanedResources.begin(), mOrphanedResources.end());

    // Release any remaining spritedefs first because they depend on image sets
    ResourceIterator iter = mResources.begin();

#ifdef DEBUG_LEAKS
#ifdef UNITTESTS
    bool status(false);
#endif  // UNITTESTS

    while (iter != mResources.end())
    {
        if (iter->second)
        {
            if (iter->second->mRefCount)
            {
                logger->log(std::string("ResourceLeak: ").append(
                    iter->second->mIdPath).append(" (").append(
                    toString(iter->second->mRefCount)).append(")"));
#ifdef UNITTESTS
                status = true;
#endif  // UNITTESTS
            }
        }
        ++iter;
    }

#ifdef UNITTESTS
    if (status)
        reportAlways("Found leaked resources.");
#endif  // UNITTESTS

    iter = mResources.begin();
#endif  // DEBUG_LEAKS

    while (iter != mResources.end())
    {
#ifdef DEBUG_LEAKS
        if (iter->second && iter->second->mRefCount)
        {
            ++iter;
            continue;
        }
#endif  // DEBUG_LEAKS

        if (dynamic_cast<SpriteDef*>(iter->second) != nullptr)
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
        if (iter->second && iter->second->mRefCount)
        {
            ++iter;
            continue;
        }
#endif  // DEBUG_LEAKS

        if (dynamic_cast<ImageSet*>(iter->second) != nullptr)
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
        if (iter->second && iter->second->mRefCount)
        {
            ++iter;
            continue;
        }
#endif  // DEBUG_LEAKS

        if (iter->second != nullptr)
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
    clearDeleted(true);
    clearScheduled();
    mDestruction = false;
}

void cleanUp(Resource *const res)
{
    if (res == nullptr)
        return;

    const unsigned refCount = res->mRefCount;
    if (refCount > 0)
    {
        logger->log("ResourceManager::~ResourceManager() cleaning up %u "
                "reference%s to %s",
                refCount,
                (refCount == 1) ? "" : "s",
                res->mIdPath.c_str());
    }

    delete res;
#ifdef DEBUG_LEAKS
    cleanOrphans(true);
#endif  // DEBUG_LEAKS
}

void cleanProtected()
{
    ResourceIterator iter = mResources.begin();
    while (iter != mResources.end())
    {
        Resource *const res = iter->second;
        if (res == nullptr)
        {
            ++ iter;
            continue;
        }
        if (res->mProtected)
        {
            res->mProtected = false;
            res->decRef();
            iter = mResources.begin();
            continue;
        }

        ++ iter;
    }
}

bool cleanOrphans(const bool always)
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
        if (res == nullptr)
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

void logResource(const Resource *const res)
{
    if (res == nullptr)
        return;
#ifdef USE_OPENGL
    const Image *const image = dynamic_cast<const Image *>(res);
    if (image != nullptr)
    {
        std::string src = image->mSource;
        const int count = image->mRefCount;
        if (count != 0)
            src.append(" ").append(toString(count));
        logger->log("resource(%s, %u) %s", res->mIdPath.c_str(),
            image->getGLImage(), src.c_str());
    }
    else
    {
        std::string src = res->mSource;
        const int count = res->mRefCount;
        if (count > 0)
            src.append(" ").append(toString(count));
        logger->log("resource(%s) %s", res->mIdPath.c_str(), src.c_str());
    }
#else  // USE_OPENGL

    logger->log("resource(%s)", res->mIdPath.c_str());
#endif  // USE_OPENGL
}

void logResources(const std::string &msg)
{
    logger->log("start of resources: " + msg);
    logger->log("resources");
    FOR_EACH(ResourceIterator, it, mResources)
    {
        logResource((*it).second);
    }
    logger->log("orphaned resources");
    FOR_EACH(ResourceIterator, it, mOrphanedResources)
    {
        logResource((*it).second);
    }
    logger->log("deleted resources");
    FOR_EACH(std::set<Resource*>::iterator, it, mDeletedResources)
    {
        logResource(*it);
    }
    logger->log("end of resources");
}

void clearDeleted(const bool full)
{
    bool status(true);
    logger->log1("clear deleted");
    while (status)
    {
        status = false;
        std::set<Resource*>::iterator resDelIter = mDeletedResources.begin();
        while (resDelIter != mDeletedResources.end())
        {
            if ((*resDelIter)->mRefCount == 0U)
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

bool addResource(const std::string &idPath,
                 Resource *const resource)
{
    if (resource != nullptr)
    {
        resource->incRef();
        resource->mIdPath = idPath;
#ifdef DEBUG_IMAGES
        logger->log("set name %p, %s", static_cast<void*>(resource),
            resource->mIdPath.c_str());
#endif  // DEBUG_IMAGES

        mResources[idPath] = resource;
        return true;
    }
    return false;
}

Resource *getFromCache(const std::string &filename,
                       const int variant)
{
    std::stringstream ss;
    ss << filename << "[" << variant << "]";
    return getFromCache(ss.str());
}

bool isInCache(const std::string &idPath)
{
    const ResourceCIterator &resIter = mResources.find(idPath);
    return (resIter != mResources.end() && (resIter->second != nullptr));
}

Resource *getTempResource(const std::string &idPath)
{
    const ResourceCIterator &resIter = mResources.find(idPath);
    if (resIter != mResources.end())
    {
        Resource *const res = resIter->second;
        if (resIter->second != nullptr)
            return res;
    }
    return nullptr;
}

Resource *getFromCache(const std::string &idPath)
{
    // Check if the id exists, and return the value if it does.
    ResourceIterator resIter = mResources.find(idPath);
    if (resIter != mResources.end())
    {
        if (resIter->second != nullptr)
            resIter->second->incRef();
        return resIter->second;
    }

    resIter = mOrphanedResources.find(idPath);
    if (resIter != mOrphanedResources.end())
    {
        Resource *const res = resIter->second;
        mResources.insert(*resIter);
        mOrphanedResources.erase(resIter);
        if (res != nullptr)
            res->incRef();
        return res;
    }
    return nullptr;
}

Resource *get(const std::string &idPath,
              generator fun,
              const void *const data)
{
#ifndef DISABLE_RESOURCE_CACHING
    Resource *resource = getFromCache(idPath);
    if (resource != nullptr)
        return resource;
    resource = fun(data);

    if (resource != nullptr)
    {
        resource->incRef();
        resource->mIdPath = idPath;
#ifdef DEBUG_IMAGES
        logger->log("set name %p, %s", static_cast<void*>(resource),
            resource->mIdPath.c_str());
#endif  // DEBUG_IMAGES

        mResources[idPath] = resource;
    }
    else
    {
        reportAlways("Error loading image: %s", idPath.c_str());
    }
#else  // DISABLE_RESOURCE_CACHING

    Resource *resource = fun(data, idPath);

    if (resource)
    {
        resource->incRef();
        resource->mIdPath = idPath;
#ifdef DEBUG_IMAGES
        logger->log("set name %p, %s", static_cast<void*>(resource),
            resource->mIdPath.c_str());
#endif  // DEBUG_IMAGES
    }
    else
    {
        reportAlways("Error loading image: " + idPath);
    }
#endif  // DISABLE_RESOURCE_CACHING

    // Returns nullptr if the object could not be created.
    return resource;
}

void release(Resource *const res)
{
    if ((res == nullptr) || mDestruction)
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
// +++ need reenable after Resource will have type field
//        reportAlways("in cache other image: %s",
//            res->mIdPath.c_str());
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
#else  // DISABLE_RESOURCE_CACHING

    delete res;
#endif  // DISABLE_RESOURCE_CACHING
}

void moveToDeleted(Resource *const res)
{
    if (res == nullptr)
        return;

    bool found(false);
    const int count = res->mRefCount;
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

void decRefDelete(Resource *const res)
{
    if (res == nullptr)
        return;

    const int count = res->mRefCount;
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

void deleteInstance()
{
#ifdef DUMP_LEAKED_RESOURCES
    logger->log1("clean orphans start");
    ResourceManager::cleanProtected();
    while (ResourceManager::cleanOrphans(true))
        continue;
    logger->log1("clean orphans end");
    ResourceIterator iter = ResourceManager::mResources.begin();

#ifdef UNITTESTS
    bool status(false);
#endif  // UNITTESTS

    while (iter != ResourceManager::mResources.end())
    {
        const Resource *const res = iter->second;
        if (res != nullptr)
        {
            if (res->mRefCount != 0U)
            {
                logger->log(std::string("ResourceLeak: ").append(
                    res->mIdPath).append(" (").append(toString(
                    res->mRefCount)).append(")"));
#ifdef UNITTESTS
                status = true;
#endif  // UNITTESTS
            }
        }
        ++iter;
    }
#ifdef UNITTESTS
    if (status)
        reportAlways("Found leaked resources.");
#endif  // UNITTESTS
#endif  // DUMP_LEAKED_RESOURCES

    deleteResourceManager();
}

void scheduleDelete(SDL_Surface *const surface)
{
    deletedSurfaces.insert(surface);
}

void clearScheduled()
{
    BLOCK_START("ResourceManager::clearScheduled")
    FOR_EACH (std::set<SDL_Surface*>::iterator, i, deletedSurfaces)
        MSDL_FreeSurface(*i);
    deletedSurfaces.clear();
    BLOCK_END("ResourceManager::clearScheduled")
}

void clearCache()
{
    cleanProtected();
    while (cleanOrphans(true))
        continue;
}

int calcMemoryLocal()
{
    int sz = 24;
    FOR_EACH (std::set<SDL_Surface*>::iterator, it, deletedSurfaces)
    {
        sz += memoryManager.getSurfaceSize(*it);
    }
    return sz;
}

int calcMemoryChilds(const int level)
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

int calcMemory(const int level)
{
    const int sumLocal = calcMemoryLocal();
    const int sumChilds = calcMemoryChilds(0);
    memoryManager.printMemory("resource manager",
        level,
        sumLocal,
        sumChilds);
    return sumLocal + sumChilds;
}

int size() noexcept2
{
    return CAST_S32(mResources.size());
}

#if defined(DEBUG_DUMP_LEAKS) || defined(UNITTESTS)
Resources &getResources()
{
    return mResources;
}

Resources &getOrphanedResources()
{
    return mOrphanedResources;
}

const std::set<Resource*> &getDeletedResources()
{
    return mDeletedResources;
}
#endif  // defined(DEBUG_DUMP_LEAKS) || defined(UNITTESTS)

}  // namespace ResourceManager
