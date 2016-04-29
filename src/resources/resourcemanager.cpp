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

#include "resources/resourcemanager.h"

#include "configuration.h"
#include "logger.h"
#include "navigationmanager.h"

#include "resources/map/walklayer.h"

#ifdef USE_OPENGL
#include "resources/atlas/atlasmanager.h"
#include "resources/atlas/atlasresource.h"
#else  // USE_OPENGL
#include "resources/image.h"
#endif  // USE_OPENGL
#include "resources/imagehelper.h"
#include "resources/imageset.h"
#include "resources/memorymanager.h"
#include "resources/sdlmusic.h"
#include "resources/soundeffect.h"

#include "resources/dye/dye.h"

#include "resources/sprite/spritedef.h"

#include "utils/delete2.h"
#include "utils/physfscheckutils.h"
#include "utils/physfsrwops.h"
#include "utils/sdlcheckutils.h"

#ifdef USE_OPENGL
#include "render/shaders/shader.h"
#include "render/shaders/shaderprogram.h"
#include "render/shaders/shadersmanager.h"
#endif

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
    mDestruction(0),
    mUseLongLiveSprites(config.getBoolValue("uselonglivesprites"))
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
        logger->log("Error: %s", PHYSFS_getLastError());
        return false;
    }
    return true;
}

bool ResourceManager::removeFromSearchPath(const std::string &path) const
{
    logger->log("Removing from PhysicsFS: %s", path.c_str());
    if (!PhysFs::removeFromSearchPath(path.c_str()))
    {
        logger->log("Error: %s", PHYSFS_getLastError());
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
        logger->log("Error loading image: " + idPath);
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
        logger->log("Error loading image: " + idPath);
    }
#endif

    // Returns nullptr if the object could not be created.
    return resource;
}

struct ResourceLoader final
{
    ResourceManager *manager;
    std::string path;
    ResourceManager::loader fun;

    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;
        const ResourceLoader *const
            rl = static_cast<const ResourceLoader *const>(v);
        SDL_RWops *const rw = MPHYSFSRWOPS_openRead(rl->path.c_str());
        if (!rw)
            return nullptr;
        Resource *const res = rl->fun(rw, rl->path);
        return res;
    }
};

SDLMusic *ResourceManager::getMusic(const std::string &idPath)
{
    ResourceLoader rl = { this, idPath, &SDLMusic::load };
    return static_cast<SDLMusic*>(get(idPath, ResourceLoader::load, &rl));
}

SoundEffect *ResourceManager::getSoundEffect(const std::string &idPath)
{
    ResourceLoader rl = { this, idPath, &SoundEffect::load };
    return static_cast<SoundEffect*>(get(idPath, ResourceLoader::load, &rl));
}

struct DyedImageLoader final
{
    ResourceManager *manager;
    std::string path;
    static Resource *load(const void *const v)
    {
        BLOCK_START("DyedImageLoader::load")
        if (!v)
        {
            BLOCK_END("DyedImageLoader::load")
            return nullptr;
        }

        const DyedImageLoader *const rl
            = static_cast<const DyedImageLoader *const>(v);
        if (!rl->manager)
        {
            BLOCK_END("DyedImageLoader::load")
            return nullptr;
        }

        std::string path1 = rl->path;
        const size_t p = path1.find('|');
        Dye *d = nullptr;
        if (p != std::string::npos)
        {
            d = new Dye(path1.substr(p + 1));
            path1 = path1.substr(0, p);
        }
        SDL_RWops *const rw = MPHYSFSRWOPS_openRead(path1.c_str());
        if (!rw)
        {
            delete d;
            BLOCK_END("DyedImageLoader::load")
            return nullptr;
        }
        Resource *const res = d ? imageHelper->load(rw, *d)
            : imageHelper->load(rw);
        delete d;
        BLOCK_END("DyedImageLoader::load")
        return res;
    }
};

Image *ResourceManager::getImage(const std::string &idPath)
{
    DyedImageLoader rl = { this, idPath };
    return static_cast<Image*>(get(idPath, DyedImageLoader::load, &rl));
}

struct ImageSetLoader final
{
    ResourceManager *manager;
    std::string path;
    int w, h;
    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const ImageSetLoader *const
            rl = static_cast<const ImageSetLoader *const>(v);
        if (!rl->manager)
            return nullptr;

        Image *const img = rl->manager->getImage(rl->path);
        if (!img)
            return nullptr;
        ImageSet *const res = new ImageSet(img, rl->w, rl->h);
        img->decRef();
        return res;
    }
};

ImageSet *ResourceManager::getImageSet(const std::string &imagePath,
                                       const int w, const int h)
{
    ImageSetLoader rl = { this, imagePath, w, h };
    std::stringstream ss;
    ss << imagePath << "[" << w << "x" << h << "]";
    return static_cast<ImageSet*>(get(ss.str(), ImageSetLoader::load, &rl));
}


struct SubImageSetLoader final
{
    ResourceManager *manager;
    Image *parent;
    int width, height;
    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const SubImageSetLoader *const
            rl = static_cast<const SubImageSetLoader *const>(v);
        if (!rl->manager)
            return nullptr;

        if (!rl->parent)
            return nullptr;
        ImageSet *const res = new ImageSet(rl->parent, rl->width, rl->height);
        return res;
    }
};

ImageSet *ResourceManager::getSubImageSet(Image *const parent,
                                          const int width, const int height)
{
    if (!parent)
        return nullptr;

    const SubImageSetLoader rl = { this, parent, width, height };
    std::stringstream ss;
    ss << parent->getIdPath() << ", set[" << width << "x" << height << "]";
    return static_cast<ImageSet*>(get(ss.str(),
        SubImageSetLoader::load, &rl));
}

struct SubImageLoader final
{
    ResourceManager *manager;
    Image *parent;
    int x, y;
    int width, height;
    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const SubImageLoader *const
            rl = static_cast<const SubImageLoader *const>(v);
        if (!rl->manager || !rl->parent)
            return nullptr;

        Image *const res = rl->parent->getSubImage(rl->x, rl->y,
            rl->width, rl->height);
        return res;
    }
};

Image *ResourceManager::getSubImage(Image *const parent,
                                    const int x, const int y,
                                    const int width, const int height)
{
    if (!parent)
        return nullptr;

    const SubImageLoader rl = { this, parent, x, y, width, height};

    std::stringstream ss;
    ss << parent->getIdPath() << ",[" << x << "," << y << ","
        << width << "x" << height << "]";
    return static_cast<Image*>(get(ss.str(), SubImageLoader::load, &rl));
}

#ifdef USE_OPENGL
struct AtlasLoader final
{
    const std::string name;
    const StringVect *files;

    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const AtlasLoader *const rl = static_cast<const AtlasLoader *const>(v);
        AtlasResource *const resource = AtlasManager::loadTextureAtlas(
            rl->name, *rl->files);
        return resource;
    }
};

Resource *ResourceManager::getAtlas(const std::string &name,
                                    const StringVect &files)
{
    AtlasLoader rl = { name, &files };
    return get("atlas_" + name, AtlasLoader::load, &rl);
}

struct ShaderLoader final
{
    const std::string name;
    const unsigned int type;

    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const ShaderLoader *const rl
            = static_cast<const ShaderLoader *const>(v);
        Shader *const resource = shaders.createShader(rl->type, rl->name);
        return resource;
    }
};

Resource *ResourceManager::getShader(const unsigned int type,
                                     const std::string &name)
{
    ShaderLoader rl = { name, type };
    return get("shader_" + name, ShaderLoader::load, &rl);
}

struct ShaderProgramLoader final
{
    const std::string vertex;
    const std::string fragment;
    const bool isNewShader;

    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const ShaderProgramLoader *const rl
            = static_cast<const ShaderProgramLoader *const>(v);
        ShaderProgram *const resource = shaders.createProgram(
            rl->vertex,
            rl->fragment,
            rl->isNewShader);
        return resource;
    }
};

Resource *ResourceManager::getShaderProgram(const std::string &vertex,
                                            const std::string &fragment,
                                            const bool isNewShader)
{
    ShaderProgramLoader rl = { vertex, fragment, isNewShader };
    return get("program_" + vertex + " + " + fragment,
        ShaderProgramLoader::load, &rl);
}
#endif

#ifndef DYECMD
struct WalkLayerLoader final
{
    const std::string name;
    Map *map;

    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const WalkLayerLoader *const rl = static_cast<const
            WalkLayerLoader *const>(v);
        Resource *const resource = NavigationManager::loadWalkLayer(rl->map);
        return resource;
    }
};
#endif

#ifndef DYECMD
WalkLayer *ResourceManager::getWalkLayer(const std::string &name,
                                         Map *const map)
{
    WalkLayerLoader rl = {name, map};
    return static_cast<WalkLayer*>(get("map_" + name,
        WalkLayerLoader::load, &rl));
}
#else
WalkLayer *ResourceManager::getWalkLayer(const std::string &name A_UNUSED,
                                         Map *const map A_UNUSED)
{
    return nullptr;
}
#endif

struct SpriteDefLoader final
{
    std::string path;
    int variant;
    bool useLongLiveSprites;
    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;

        const SpriteDefLoader *const
            rl = static_cast<const SpriteDefLoader *const>(v);
        return SpriteDef::load(rl->path, rl->variant, rl->useLongLiveSprites);
    }
};

SpriteDef *ResourceManager::getSprite(const std::string &path,
                                      const int variant)
{
    SpriteDefLoader rl = { path, variant, mUseLongLiveSprites };
    std::stringstream ss;
    ss << path << "[" << variant << "]";
    return static_cast<SpriteDef*>(get(ss.str(), SpriteDefLoader::load, &rl));
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
        logger->log("no resource in cache: " + res->mIdPath);
        delete res;
        return;
    }
    if (resIter->second != res)
    {
        logger->log("in cache other image: " + res->mIdPath);
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

SDL_Surface *ResourceManager::loadSDLSurface(const std::string &filename) const
{
    if (SDL_RWops *const rw = MPHYSFSRWOPS_openRead(filename.c_str()))
    {
        if (!IMG_isPNG(rw))
        {
            logger->log("Error, image is not png: " + filename);
            return nullptr;
        }
        SDL_Surface *const surface = MIMG_LoadPNG_RW(rw);
        SDL_RWclose(rw);
        return surface;
    }
    return nullptr;
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

struct RescaledLoader final
{
    ResourceManager *manager;
    const Image *image;
    int width;
    int height;
    static Resource *load(const void *const v)
    {
        if (!v)
            return nullptr;
        const RescaledLoader *const rl
            = static_cast<const RescaledLoader *const>(v);
        if (!rl->manager || !rl->image)
            return nullptr;
        Image *const rescaled = rl->image->SDLgetScaledImage(
            rl->width, rl->height);
        if (!rescaled)
            return nullptr;
        return rescaled;
    }
};

Image *ResourceManager::getRescaled(const Image *const image,
                                    const int width, const int height)
{
    if (!image)
        return nullptr;

    std::string idPath = image->getIdPath() + strprintf(
        "_rescaled%dx%d", width, height);
    const RescaledLoader rl = { this, image, width, height };
    Image *const img = static_cast<Image *const>(
        get(idPath, RescaledLoader::load, &rl));
    return img;
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
        sz += (*it).first.capacity();
        sz += (*it).second->calcMemory(level + 1);
    }
    FOR_EACH (ResourceCIterator, it, mOrphanedResources)
    {
        sz += (*it).first.capacity();
        sz += (*it).second->calcMemory(level + 1);
    }
    FOR_EACH (std::set<Resource*>::const_iterator, it, mDeletedResources)
    {
        sz += (*it)->calcMemory(level + 1);
    }
    return sz;
}
