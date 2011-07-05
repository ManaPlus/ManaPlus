/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "client.h"
#include "configuration.h"
#include "log.h"
#include "main.h"

#include "resources/dye.h"
#include "resources/image.h"
#include "resources/imageset.h"
#include "resources/music.h"
#include "resources/soundeffect.h"
#include "resources/spritedef.h"

#include "utils/mkdir.h"

#include <physfs.h>
#include <SDL_image.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include <sys/stat.h>
#include <sys/time.h>

#include "debug.h"

#define THEMES_FOLDER "themes"

ResourceManager *ResourceManager::instance = NULL;

ResourceManager::ResourceManager() :
    mOldestOrphan(0),
    mSelectedSkin(""),
    mSkinName(""),
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
                logger->log("ResourceLeak: " + iter->second->getIdPath()
                            + " (" + toString(iter->second->getRefCount())
                            + ")");
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
        if (dynamic_cast<SpriteDef*>(iter->second) != 0)
        {
            cleanUp(iter->second);
            ResourceIterator toErase = iter;
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
        if (dynamic_cast<ImageSet*>(iter->second) != 0)
        {
            cleanUp(iter->second);
            ResourceIterator toErase = iter;
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
            ResourceIterator toErase = iter;
            ++iter;
            mResources.erase(toErase);
        }
        else
        {
            ++iter;
        }
    }
    clearScheduled();
}

void ResourceManager::cleanUp(Resource *res)
{
    if (!res)
        return;

    if (res->mRefCount > 0)
    {
        logger->log("ResourceManager::~ResourceManager() cleaning up %d "
                "reference%s to %s",
                res->mRefCount,
                (res->mRefCount == 1) ? "" : "s",
                res->mIdPath.c_str());
    }

    delete res;
}

void ResourceManager::cleanOrphans(bool always)
{
    timeval tv;
    gettimeofday(&tv, NULL);
    // Delete orphaned resources after 30 seconds.
    time_t oldest = tv.tv_sec, threshold = oldest - 30;

    if (mOrphanedResources.empty() || (!always && mOldestOrphan >= threshold))
        return;

    ResourceIterator iter = mOrphanedResources.begin();
    while (iter != mOrphanedResources.end())
    {
        Resource *res = iter->second;
        if (!res)
        {
            ++iter;
            continue;
        }
        time_t t = res->mTimeStamp;
        if (t >= threshold)
        {
            if (t < oldest) oldest = t;
            ++iter;
        }
        else
        {
            logger->log("ResourceManager::release(%s)", res->mIdPath.c_str());
            ResourceIterator toErase = iter;
            ++iter;
            mOrphanedResources.erase(toErase);
            delete res; // delete only after removal from list,
                        // to avoid issues in recursion
        }
    }

    mOldestOrphan = oldest;
}

bool ResourceManager::setWriteDir(const std::string &path)
{
    return static_cast<bool>(PHYSFS_setWriteDir(path.c_str()));
}

bool ResourceManager::addToSearchPath(const std::string &path, bool append)
{
    logger->log("Adding to PhysicsFS: %s (%s)", path.c_str(),
                append ? "append" : "prepend");
    if (!PHYSFS_addToSearchPath(path.c_str(), append ? 1 : 0))
    {
        logger->log("Error: %s", PHYSFS_getLastError());
        return false;
    }
    return true;
}

bool ResourceManager::removeFromSearchPath(const std::string &path)
{
    logger->log("Removing from PhysicsFS: %s", path.c_str());
    if (!PHYSFS_removeFromSearchPath(path.c_str()))
    {
        logger->log("Error: %s", PHYSFS_getLastError());
        return false;
    }
    return true;
}

void ResourceManager::searchAndAddArchives(const std::string &path,
                                           const std::string &ext,
                                           bool append)
{
    const char *dirSep = PHYSFS_getDirSeparator();
    char **list = PHYSFS_enumerateFiles(path.c_str());

    for (char **i = list; *i; i++)
    {
        size_t len = strlen(*i);

        if (len > ext.length() && !ext.compare((*i) + (len - ext.length())))
        {
            std::string file, realPath, archive, realFixPath;

            file = path + (*i);
            realPath = std::string(PHYSFS_getRealDir(file.c_str()));
            archive = realPath + dirSep + file;

            addToSearchPath(archive, append);
        }
    }

    PHYSFS_freeList(list);
}

void ResourceManager::searchAndRemoveArchives(const std::string &path,
                                              const std::string &ext)
{
    const char *dirSep = PHYSFS_getDirSeparator();
    char **list = PHYSFS_enumerateFiles(path.c_str());

    for (char **i = list; *i; i++)
    {
        size_t len = strlen(*i);

        if (len > ext.length() && !ext.compare((*i) + (len - ext.length())))
        {
            std::string file, realPath, archive, realFixPath;

            file = path + (*i);
            realPath = std::string(PHYSFS_getRealDir(file.c_str()));
            archive = realPath + dirSep + file;

            removeFromSearchPath(archive);
        }
    }

    PHYSFS_freeList(list);
}

bool ResourceManager::mkdir(const std::string &path)
{
    return static_cast<bool>(PHYSFS_mkdir(path.c_str()));
}

bool ResourceManager::exists(const std::string &path)
{
    return PHYSFS_exists(path.c_str());
}

bool ResourceManager::existsLocal(const std::string &path)
{
    bool flg(false);
    std::fstream file;
    file.open(path.c_str(), std::ios::in);
    if (file.is_open())
        flg = true;
    file.close();
    return flg;
}

bool ResourceManager::isDirectory(const std::string &path)
{
    return PHYSFS_isDirectory(path.c_str());
}

std::string ResourceManager::getPath(const std::string &file)
{
    // get the real path to the file
    const char* tmp = PHYSFS_getRealDir(file.c_str());
    std::string path;

    // if the file is not in the search path, then its NULL
    if (tmp)
    {
        path = std::string(tmp) + "/" + file;
    }
    else
    {
        // if not found in search path return the default path
        path = Client::getPackageDirectory() + "/" + file;
    }

    return path;
}

bool ResourceManager::addResource(const std::string &idPath,
                                  Resource* resource)
{
    if (resource)
    {
        resource->incRef();
        resource->mIdPath = idPath;
        mResources[idPath] = resource;
        return true;
    }
    return false;
}

Resource *ResourceManager::get(const std::string &idPath, generator fun,
                               void *data)
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
        Resource *res = resIter->second;
        mResources.insert(*resIter);
        mOrphanedResources.erase(resIter);
        if (res)
            res->incRef();
        return res;
    }

    Resource *resource = fun(data);

    if (resource)
    {
        resource->incRef();
        resource->mIdPath = idPath;
        mResources[idPath] = resource;
        cleanOrphans();
    }
    else
    {
        logger->log("Error loaging image: " + idPath);
    }

    // Returns NULL if the object could not be created.
    return resource;
}

struct ResourceLoader
{
    ResourceManager *manager;
    std::string path;
    ResourceManager::loader fun;
    static Resource *load(void *v)
    {
        if (!v)
            return NULL;
        ResourceLoader *l = static_cast< ResourceLoader * >(v);
        int fileSize;
        if (!l->manager)
            return NULL;
        void *buffer = l->manager->loadFile(l->path, fileSize);
        if (!buffer)
            return NULL;
        Resource *res = l->fun(buffer, fileSize);
        free(buffer);
        return res;
    }
};

Resource *ResourceManager::load(const std::string &path, loader fun)
{
    ResourceLoader l = { this, path, fun };
    return get(path, ResourceLoader::load, &l);
}

Music *ResourceManager::getMusic(const std::string &idPath)
{
    return static_cast<Music*>(load(idPath, Music::load));
}

SoundEffect *ResourceManager::getSoundEffect(const std::string &idPath)
{
    return static_cast<SoundEffect*>(load(idPath, SoundEffect::load));
}

struct DyedImageLoader
{
    ResourceManager *manager;
    std::string path;
    static Resource *load(void *v)
    {
        if (!v)
            return NULL;

        DyedImageLoader *l = static_cast< DyedImageLoader * >(v);
        if (!l->manager)
            return NULL;

        std::string path = l->path;
        std::string::size_type p = path.find('|');
        Dye *d = NULL;
        if (p != std::string::npos)
        {
            d = new Dye(path.substr(p + 1));
            path = path.substr(0, p);
        }
        int fileSize;
        void *buffer = l->manager->loadFile(path, fileSize);
        if (!buffer)
        {
            delete d;
            return 0;
        }
        Resource *res = d ? Image::load(buffer, fileSize, *d)
                          : Image::load(buffer, fileSize);
        free(buffer);
        delete d;
        return res;
    }
};

Image *ResourceManager::getImage(const std::string &idPath)
{
    DyedImageLoader l = { this, idPath };
    return static_cast<Image*>(get(idPath, DyedImageLoader::load, &l));
}

/*
Image *ResourceManager::getSkinImage(const std::string &idPath)
{
    if (mSelectedSkin.empty())
        return getImage(idPath);

    DyedImageLoader l = { this, mSelectedSkin + idPath };
    void *ptr = get(idPath, DyedImageLoader::load, &l);
    if (ptr)
        return static_cast<Image*>(ptr);
    else
        return getImage(idPath);
}
*/

struct ImageSetLoader
{
    ResourceManager *manager;
    std::string path;
    int w, h;
    static Resource *load(void *v)
    {
        if (!v)
            return NULL;

        ImageSetLoader *l = static_cast< ImageSetLoader * >(v);
        if (!l->manager)
            return NULL;

        Image *img = l->manager->getImage(l->path);
        if (!img)
            return NULL;
        ImageSet *res = new ImageSet(img, l->w, l->h);
        img->decRef();
        return res;
    }
};

ImageSet *ResourceManager::getImageSet(const std::string &imagePath,
                                       int w, int h)
{
    ImageSetLoader l = { this, imagePath, w, h };
    std::stringstream ss;
    ss << imagePath << "[" << w << "x" << h << "]";
    return static_cast<ImageSet*>(get(ss.str(), ImageSetLoader::load, &l));
}

struct SpriteDefLoader
{
    std::string path;
    int variant;
    static Resource *load(void *v)
    {
        if (!v)
            return NULL;

        SpriteDefLoader *l = static_cast< SpriteDefLoader * >(v);
        return SpriteDef::load(l->path, l->variant);
    }
};

SpriteDef *ResourceManager::getSprite(const std::string &path, int variant)
{
    SpriteDefLoader l = { path, variant };
    std::stringstream ss;
    ss << path << "[" << variant << "]";
    return static_cast<SpriteDef*>(get(ss.str(), SpriteDefLoader::load, &l));
}

void ResourceManager::release(Resource *res)
{
    if (!res || mDestruction)
        return;

    ResourceIterator resIter = mResources.find(res->mIdPath);

    // The resource has to exist
    assert(resIter != mResources.end() && resIter->second == res);

    timeval tv;
    gettimeofday(&tv, NULL);
    time_t timestamp = tv.tv_sec;

    res->mTimeStamp = timestamp;
    if (mOrphanedResources.empty())
        mOldestOrphan = timestamp;

    mOrphanedResources.insert(*resIter);
    mResources.erase(resIter);
}

ResourceManager *ResourceManager::getInstance()
{
    // Create a new instance if necessary.
    if (!instance)
        instance = new ResourceManager;
    return instance;
}

void ResourceManager::deleteInstance()
{
    delete instance;
    instance = 0;
}

void *ResourceManager::loadFile(const std::string &fileName, int &fileSize)
{
    // Attempt to open the specified file using PhysicsFS
    PHYSFS_file *file = PHYSFS_openRead(fileName.c_str());

    // If the handler is an invalid pointer indicate failure
    if (file == NULL)
    {
        logger->log("Warning: Failed to load %s: %s",
                    fileName.c_str(), PHYSFS_getLastError());
        return NULL;
    }

    // Log the real dir of the file
    logger->log("Loaded %s/%s", PHYSFS_getRealDir(fileName.c_str()),
                fileName.c_str());

    // Get the size of the file
    fileSize = static_cast<int>(PHYSFS_fileLength(file));

    // Allocate memory and load the file
    void *buffer = calloc(fileSize, 1);
    PHYSFS_read(file, buffer, 1, fileSize);

    // Close the file and let the user deallocate the memory
    PHYSFS_close(file);

    return buffer;
}

bool ResourceManager::copyFile(const std::string &src, const std::string &dst)
{
    PHYSFS_file *srcFile = PHYSFS_openRead(src.c_str());
    if (!srcFile)
    {
        logger->log("Read error: %s", PHYSFS_getLastError());
        return false;
    }
    PHYSFS_file *dstFile = PHYSFS_openWrite(dst.c_str());
    if (!dstFile)
    {
        logger->log("Write error: %s", PHYSFS_getLastError());
        PHYSFS_close(srcFile);
        return false;
    }

    int fileSize = static_cast<int>(PHYSFS_fileLength(srcFile));
    void *buf = malloc(fileSize);
    PHYSFS_read(srcFile, buf, 1, fileSize);
    PHYSFS_write(dstFile, buf, 1, fileSize);

    PHYSFS_close(srcFile);
    PHYSFS_close(dstFile);
    free(buf);
    return true;
}

std::vector<std::string> ResourceManager::loadTextFile(
        const std::string &fileName)
{
    int contentsLength;
    char *fileContents = static_cast<char*>(
        loadFile(fileName, contentsLength));
    std::vector<std::string> lines;

    if (!fileContents)
    {
        logger->log("Couldn't load text file: %s", fileName.c_str());
        return lines;
    }

    std::istringstream iss(std::string(fileContents, contentsLength));
    std::string line;

    while (getline(iss, line))
        lines.push_back(line);

    free(fileContents);
    return lines;
}

std::vector<std::string> ResourceManager::loadTextFileLocal(
        const std::string &fileName)
{
    std::ifstream file;
    char line[501];
    std::vector<std::string> lines;

    file.open(fileName.c_str(), std::ios::in);

    if (!file.is_open())
    {
        logger->log("Couldn't load text file: %s", fileName.c_str());
        return lines;
    }

    while (file.getline(line, 500))
        lines.push_back(line);

    return lines;
}

void ResourceManager::saveTextFile(std::string path, std::string name,
                                   std::string text)
{
    if (!mkdir_r(path.c_str()))
    {
        std::ofstream file;
        std::string fileName = path + "/" + name;

        file.open(fileName.c_str(), std::ios::out);
        if (file.is_open())
            file << text << std::endl;
        file.close();
    }
}

SDL_Surface *ResourceManager::loadSDLSurface(const std::string &filename)
{
    int fileSize;
    void *buffer = loadFile(filename, fileSize);
    SDL_Surface *tmp = NULL;

    if (buffer)
    {
        SDL_RWops *rw = SDL_RWFromMem(buffer, fileSize);
        tmp = IMG_Load_RW(rw, 1);
        ::free(buffer);
    }

    return tmp;
}

void ResourceManager::scheduleDelete(SDL_Surface* surface)
{
    deletedSurfaces.insert(surface);
}

void ResourceManager::clearScheduled()
{
    for (std::set<SDL_Surface*>::iterator i = deletedSurfaces.begin(),
         i_end = deletedSurfaces.end(); i != i_end; ++i)
    {
        SDL_FreeSurface(*i);
    }
    deletedSurfaces.clear();
}

struct RescaledLoader
{
    ResourceManager *manager;
    Image *image;
    int width;
    int height;
    static Resource *load(void *v)
    {
        if (!v)
            return NULL;
        RescaledLoader *l = static_cast< RescaledLoader * >(v);
        if (!l->manager)
            return NULL;
        Image *rescaled = l->image->SDLgetScaledImage(l->width, l->height);
        if (!rescaled)
            return NULL;
        return rescaled;
    }
};

Image *ResourceManager::getRescaled(Image *image, int width, int height)
{
    if (!image)
        return 0;

    std::string idPath = image->getIdPath() + strprintf(
        "_rescaled%dx%d", width, height);
    RescaledLoader l = { this, image, width, height };
    Image *img = static_cast<Image*>(get(idPath, RescaledLoader::load, &l));
    return img;
}
