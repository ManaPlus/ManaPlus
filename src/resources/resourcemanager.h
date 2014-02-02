/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef RESOURCES_RESOURCEMANAGER_H
#define RESOURCES_RESOURCEMANAGER_H

#include "main.h"

#include "utils/stringvector.h"

#include <ctime>
#include <list>
#include <map>
#include <set>

#include "localconsts.h"

class AnimationDelayLoad;
class Image;
class ImageSet;
class Map;
class SDLMusic;
class Resource;
class SoundEffect;
class SpriteDef;
class WalkLayer;

struct SDL_Surface;
struct SDL_RWops;

typedef std::list<AnimationDelayLoad*> DelayedAnim;
typedef DelayedAnim::iterator DelayedAnimIter;

/**
 * A class for loading and managing resources.
 */
class ResourceManager final
{
    friend class Resource;

    public:
        typedef Resource *(*loader)(SDL_RWops *);
        typedef Resource *(*generator)(const void *const);

        ResourceManager();

        A_DELETE_COPY(ResourceManager)

        /**
         * Destructor. Cleans up remaining resources, warning about resources
         * that were still referenced.
         */
        ~ResourceManager();

        /**
         * Sets the write directory.
         *
         * @param path The path of the directory to be added.
         * @return <code>true</code> on success, <code>false</code> otherwise.
         */
        bool setWriteDir(const std::string &path) const;

        /**
         * Adds a directory or archive to the search path. If append is true
         * then the directory is added to the end of the search path, otherwise
         * it is added at the front.
         *
         * @return <code>true</code> on success, <code>false</code> otherwise.
         */
        bool addToSearchPath(const std::string &path, const bool append) const;

        /**
         * Remove a directory or archive from the search path.
         *
         * @return <code>true</code> on success, <code>false</code> otherwise.
         */
        bool removeFromSearchPath(const std::string &path) const;

        /**
        * Searches for zip files and adds them to the search path.
        */
        void searchAndAddArchives(const std::string &restrict path,
                                  const std::string &restrict ext,
                                  const bool append) const;

        /**
        * Searches for zip files and remove them from the search path.
        */
        void searchAndRemoveArchives(const std::string &restrict path,
                                     const std::string &restrict ext) const;

        /**
         * Creates a directory in the write path
         */
        bool mkdir(const std::string &path) const;

        /**
         * Checks whether the given file or directory exists in the search path
         * (PhysFS)
         */
        bool exists(const std::string &path) const A_WARN_UNUSED;

        /**
         * Checks whether the given file or directory exists
         */
        static bool existsLocal(const std::string &path) A_WARN_UNUSED;

        /**
         * Checks whether the given path is a directory.
         */
        bool isDirectory(const std::string &path) const A_WARN_UNUSED;

        /**
         * Returns the real path to a file. Note that this method will always
         * return a path, it does not check whether the file exists.
         *
         * @param file The file to get the real path to.
         * @return The real path.
         */
        static std::string getPath(const std::string &file) A_WARN_UNUSED;

        /**
         * Creates a resource and adds it to the resource map.
         *
         * @param idPath The resource identifier path.
         * @param fun    A function for generating the resource.
         * @param data   Extra parameters for the generator.
         * @return A valid resource or <code>NULL</code> if the resource could
         *         not be generated.
         */
        Resource *get(const std::string &idPath, const generator fun,
                      const void *const data) A_WARN_UNUSED;

        Resource *getFromCache(const std::string &idPath) A_WARN_UNUSED;

        Resource *getFromCache(const std::string &filename,
                               const int variant) A_WARN_UNUSED;

        /**
         * Loads a resource from a file and adds it to the resource map.
         *
         * @param path The file name.
         * @param fun  A function for parsing the file.
         * @return A valid resource or <code>NULL</code> if the resource could
         *         not be loaded.
         */
        Resource *load(const std::string &path,
                       const loader fun) A_WARN_UNUSED;

        /**
         * Adds a preformatted resource to the resource map.
         *
         * @param path The file name.
         * @param Resource  The Resource to add.
         * @return true if successfull, false otherwise.
         */
        bool addResource(const std::string &idPath, Resource *const resource);

        /**
        * Copies a file from one place to another (useful for extracting
        * raw files from a zip archive, for example)
        *
        * @param src Source file name
        * @param dst Destination file name
        * @return true on success, false on failure. An error message should be
        *         in the log file.
        */
        static bool copyFile(const std::string &restrict src,
                             const std::string &restrict dst);

        /**
         * Convenience wrapper around ResourceManager::get for loading
         * images.
         */
        Image *getImage(const std::string &idPath) A_WARN_UNUSED;

        /**
         * Convenience wrapper around ResourceManager::get for loading
         * songs.
         */
        SDLMusic *getMusic(const std::string &idPath) A_WARN_UNUSED;

        /**
         * Convenience wrapper around ResourceManager::get for loading
         * samples.
         */
        SoundEffect *getSoundEffect(const std::string &idPath) A_WARN_UNUSED;

        /**
         * Creates a image set based on the image referenced by the given
         * path and the supplied sprite sizes
         */
        ImageSet *getImageSet(const std::string &imagePath,
                              const int w, const int h) A_WARN_UNUSED;

        ImageSet *getSubImageSet(Image *const parent,
                                 const int width,
                                 const int height) A_WARN_UNUSED;

        Image *getSubImage(Image *const parent, const int x, const int y,
                           const int width, const int height) A_WARN_UNUSED;

#ifdef USE_OPENGL
        Resource *getAtlas(const std::string &name,
                           const StringVect &files) A_WARN_UNUSED;
#endif

        WalkLayer *getWalkLayer(const std::string &name, Map *const map);

        /**
         * Creates a sprite definition based on a given path and the supplied
         * variant.
         */
        SpriteDef *getSprite(const std::string &path,
                             const int variant = 0) A_WARN_UNUSED;

        /**
         * Releases a resource, placing it in the set of orphaned resources.
         */
        void release(Resource *const res);

        void clearDeleted(const bool full = true);

        void decRefDelete(Resource *const res);

        static void logResource(const Resource *const res);

        /**
         * Move resource to deleted resources list.
         */
        void moveToDeleted(Resource *const res);

        /**
         * Allocates data into a buffer pointer for raw data loading. The
         * returned data is expected to be freed using <code>free()</code>.
         *
         * @param fileName The name of the file to be loaded.
         * @param fileSize The size of the file that was loaded.
         *
         * @return An allocated byte array containing the data that was loaded,
         *         or <code>NULL</code> on fail.
         */
        static void *loadFile(const std::string &fileName,
                              int &fileSize) A_WARN_UNUSED;

        /**
         * Retrieves the contents of a text file (PhysFS).
         */
        static bool loadTextFile(const std::string &fileName,
                                 StringVect &lines);

        /**
         * Retrieves the contents of a text file.
         */
        static bool loadTextFileLocal(const std::string &fileName,
                                      StringVect &lines);

        static void saveTextFile(std::string path,
                                 const std::string &restrict name,
                                 const std::string &restrict text);

        Image *getRescaled(Image *const image,
                           const int width, const int height) A_WARN_UNUSED;

        /**
         * Loads the given filename as an SDL surface. The returned surface is
         * expected to be freed by the caller using SDL_FreeSurface.
         */
        SDL_Surface *loadSDLSurface(const std::string &filename)
                                    const A_WARN_UNUSED;

        void scheduleDelete(SDL_Surface *const surface);

        void clearScheduled();

        /**
         * Returns an instance of the class, creating one if it does not
         * already exist.
         */
        static ResourceManager *getInstance() A_WARN_UNUSED;

        /**
         * Deletes the class instance if it exists.
         */
        static void deleteInstance();

        int size() const A_WARN_UNUSED
        { return static_cast<int>(mResources.size()); }

        typedef std::map<std::string, Resource*> Resources;
        typedef Resources::iterator ResourceIterator;
        typedef Resources::const_iterator ResourceCIterator;

#ifdef DEBUG_DUMP_LEAKS
        Resources* getResources() A_WARN_UNUSED
        { return &mResources; }

        Resources* getOrphanedResources() A_WARN_UNUSED
        { return &mOrphanedResources; }
#endif

        bool cleanOrphans(const bool always = false);

        void cleanProtected();

        bool isInCache(const std::string &idPath) const A_WARN_UNUSED;

        Resource *getTempResource(const std::string &idPath) A_WARN_UNUSED;

        void clearCache();

        static void addDelayedAnimation(AnimationDelayLoad *const animation)
        { mDelayedAnimations.push_back(animation); }

        static void delayedLoad();

        static void removeDelayLoad(const AnimationDelayLoad
                                    *const delayedLoad);

        static void deleteFilesInDirectory(std::string path);

    private:
        /**
         * Deletes the resource after logging a cleanup message.
         */
        static void cleanUp(Resource *const resource);

        static ResourceManager *instance;
        std::set<SDL_Surface*> deletedSurfaces;
        Resources mResources;
        Resources mOrphanedResources;
        std::set<Resource*> mDeletedResources;
        time_t mOldestOrphan;
        bool mDestruction;
        bool mUseLongLiveSprites;
        static DelayedAnim mDelayedAnimations;
};

#endif  // RESOURCES_RESOURCEMANAGER_H
