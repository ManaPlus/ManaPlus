/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2017  The ManaPlus Developers
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

#include "settings.h"

#ifdef DEBUG_IMAGES
#include "logger.h"
#endif  // DEBUG_IMAGES

#include "fs/virtfs/rwops.h"

#include "utils/mathutils.h"
#include "utils/sdlcheckutils.h"

#include "resources/openglimagehelper.h"

#include "resources/atlas/atlasresource.h"
#include "resources/atlas/textureatlas.h"

#include "resources/dye/dye.h"

#include "resources/resourcemanager/resourcemanager.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER
PRAGMA48(GCC diagnostic pop)

#include "utils/checkutils.h"

#include "debug.h"

AtlasManager::AtlasManager()
{
}

AtlasResource *AtlasManager::loadTextureAtlas(const std::string &name,
                                              const StringVect &files)
{
    BLOCK_START("AtlasManager::loadTextureAtlas")
    STD_VECTOR<TextureAtlas*> atlases;
    STD_VECTOR<Image*> images;
    AtlasResource *resource = new AtlasResource;

    loadImages(files, images);
    int maxSize = OpenGLImageHelper::getTextureSize();
#if !defined(ANDROID) && !defined(__APPLE__)
    int sz = settings.textureSize;
    if (maxSize > sz)
        maxSize = sz;
#endif  // !defined(ANDROID) && !defined(__APPLE__)

    // sorting images on atlases.
    simpleSort(name, atlases, images, maxSize);

    FOR_EACH (STD_VECTOR<TextureAtlas*>::iterator, it, atlases)
    {
        TextureAtlas *const atlas = *it;
        if (atlas == nullptr)
            continue;

        createSDLAtlas(atlas);
        if (atlas->atlasImage == nullptr)
            continue;
        convertAtlas(atlas);
        resource->atlases.push_back(atlas);
    }

    BLOCK_END("AtlasManager::loadTextureAtlas")
    return resource;
}

AtlasResource *AtlasManager::loadEmptyAtlas(const std::string &name,
                                            const StringVect &files)
{
    STD_VECTOR<TextureAtlas*> atlases;
    STD_VECTOR<Image*> images;
    AtlasResource *resource = new AtlasResource;

    loadEmptyImages(files, images);

    // sorting images on atlases.
    emptySort(name, atlases, images);

    FOR_EACH (STD_VECTOR<TextureAtlas*>::iterator, it, atlases)
    {
        TextureAtlas *const atlas = *it;
        if (atlas == nullptr)
            continue;

        atlas->atlasImage = new Image(0,
            1024, 1024,
            1024, 1024);
        // convert SDL images to OpenGL
        convertEmptyAtlas(atlas);

        resource->atlases.push_back(atlas);
    }

    BLOCK_END("AtlasManager::loadTextureAtlas")
    return resource;
}

void AtlasManager::loadImages(const StringVect &files,
                              STD_VECTOR<Image*> &images)
{
    BLOCK_START("AtlasManager::loadImages")

    FOR_EACH (StringVectCIter, it, files)
    {
        const std::string str = *it;
        // check is image with same name already in cache
        // and if yes, move it to deleted set
        Resource *const res = ResourceManager::getTempResource(str);
        if (res != nullptr)
        {
            // increase counter because in moveToDeleted it will be decreased.
            res->incRef();
            ResourceManager::moveToDeleted(res);
        }

        std::string path = str;
        const size_t p = path.find('|');
        Dye *d = nullptr;
        if (p != std::string::npos)
        {
            d = new Dye(path.substr(p + 1));
            path = path.substr(0, p);
        }

        SDL_RWops *const rw = VirtFs::rwopsOpenRead(path);
        if (rw != nullptr)
        {
            Image *const image = d != nullptr ?
                surfaceImageHelper->load(rw, *d)
                : surfaceImageHelper->load(rw);

            if (image != nullptr)
            {
                image->mIdPath = str;
#ifdef DEBUG_IMAGES
                logger->log("set name %p, %s", static_cast<void*>(image),
                    image->mIdPath.c_str());
#endif  // DEBUG_IMAGES

                images.push_back(image);
            }
        }
        delete d;
    }
    BLOCK_END("AtlasManager::loadImages")
}

void AtlasManager::loadEmptyImages(const StringVect &files,
                                   STD_VECTOR<Image*> &images)
{
    BLOCK_START("AtlasManager::loadEmptyImages")

    FOR_EACH (StringVectCIter, it, files)
    {
        const std::string str = *it;
        // check is image with same name already in cache
        // and if yes, move it to deleted set
        Resource *const res = ResourceManager::getTempResource(str);
        if (res != nullptr)
        {
            // increase counter because in moveToDeleted it will be decreased.
            res->incRef();
            ResourceManager::moveToDeleted(res);
        }

        Image *const image = new Image(0,
            2048, 2048,
            2048, 2048);
        image->mIdPath = str;
        images.push_back(image);
    }
    BLOCK_END("AtlasManager::loadEmptyImages")
}

void AtlasManager::simpleSort(const std::string &restrict name,
                              STD_VECTOR<TextureAtlas*> &restrict atlases,
                              const STD_VECTOR<Image*> &restrict images,
                              int size)
{
    BLOCK_START("AtlasManager::simpleSort")
    int x = 0;
    int y = 0;
    int tempHeight = 0;
    TextureAtlas *atlas = new TextureAtlas;
    STD_VECTOR<Image*>::const_iterator it = images.begin();
    const STD_VECTOR<Image*>::const_iterator it_end = images.end();
    for (it = images.begin(); it != it_end; ++ it)
    {
        const Image *const img = *it;
        if (img != nullptr)
        {
            atlas->name = std::string("atlas_").append(name).append(
                "_").append(img->mIdPath);
            break;
        }
    }

    for (it = images.begin(); it != it_end; ++ it)
    {
        Image *const img = *it;
        if (img != nullptr)
        {
            AtlasItem *const item = new AtlasItem(img);
            item->name = img->mIdPath;
            // start next line
            if (x + img->mBounds.w > size)
            {
                x = 0;
                y += tempHeight;
                tempHeight = 0;
            }

            // can't put image with this height
            if (y + img->mBounds.h > size)
            {
                x = 0;
                y = 0;
                atlases.push_back(atlas);
                atlas = new TextureAtlas;
                atlas->name = std::string("atlas_").append(name).append(
                    "_").append(img->mIdPath);
            }

            if (img->mBounds.h > tempHeight)
                tempHeight = img->mBounds.h;

            item->x = x;
            item->y = y;
            atlas->items.push_back(item);

            // continue put textures in line
            x += img->mBounds.w;
            if (x > atlas->width)
                atlas->width = x;
            if (y + img->mBounds.h > atlas->height)
                atlas->height = y + img->mBounds.h;
        }
    }
    if (!atlas->items.empty())
        atlases.push_back(atlas);
    else
        delete atlas;
    BLOCK_END("AtlasManager::simpleSort")
}

void AtlasManager::emptySort(const std::string &restrict name,
                             STD_VECTOR<TextureAtlas*> &restrict atlases,
                             const STD_VECTOR<Image*> &restrict images)
{
    BLOCK_START("AtlasManager::simpleSort")
    TextureAtlas *atlas = new TextureAtlas;
    STD_VECTOR<Image*>::const_iterator it = images.begin();
    const STD_VECTOR<Image*>::const_iterator it_end = images.end();
    for (it = images.begin(); it != it_end; ++ it)
    {
        const Image *const img = *it;
        if (img != nullptr)
        {
            atlas->name = std::string("atlas_").append(name).append(
                "_").append(img->mIdPath);
            break;
        }
    }

    for (it = images.begin(); it != it_end; ++ it)
    {
        Image *const img = *it;
        if (img != nullptr)
        {
            AtlasItem *const item = new AtlasItem(img);
            item->name = img->mIdPath;
            item->x = 0;
            item->y = 0;
            atlas->items.push_back(item);
        }
    }
    if (!atlas->items.empty())
        atlases.push_back(atlas);
    else
        delete atlas;
    BLOCK_END("AtlasManager::simpleSort")
}

void AtlasManager::createSDLAtlas(TextureAtlas *const atlas)
{
    BLOCK_START("AtlasManager::createSDLAtlas")
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const unsigned int rmask = 0xff000000;
    const unsigned int gmask = 0x00ff0000;
    const unsigned int bmask = 0x0000ff00;
    const unsigned int amask = 0x000000ff;
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    const unsigned int rmask = 0x000000ff;
    const unsigned int gmask = 0x0000ff00;
    const unsigned int bmask = 0x00ff0000;
    const unsigned int amask = 0xff000000;
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    // do not create atlas based on only one image
    if (atlas->items.size() == 1)
    {
        logger->log("Skip atlas creation because only one image in atlas.");
        BLOCK_END("AtlasManager::createSDLAtlas")
        return;
    }

    // using only power of two sizes.
    atlas->width = powerOfTwo(atlas->width);
    atlas->height = powerOfTwo(atlas->height);

    const int width = atlas->width;
    const int height = atlas->height;
    BLOCK_START("AtlasManager::createSDLAtlas create surface")
    // temp SDL surface for atlas
    SDL_Surface *const surface = MSDL_CreateRGBSurface(SDL_SWSURFACE,
        width, height, 32U, rmask, gmask, bmask, amask);
    if (surface == nullptr)
    {
        reportAlways("Error creating surface for atlas. Size: %dx%d",
            width,
            height);
        BLOCK_END("AtlasManager::createSDLAtlas")
        return;
    }
    BLOCK_END("AtlasManager::createSDLAtlas create surface")

#ifdef OPENGLERRORS
    logger->log("OpenGL debug: creating atlase %dx%d", width, height);
#endif  // OPENGLERRORS

    Image *image = imageHelper->loadSurface(surface);
    // free SDL atlas surface
    MSDL_FreeSurface(surface);
    if (image == nullptr)
    {
        reportAlways("Error converting surface to texture. Size: %dx%d",
            width,
            height);
        return;
    }

    // drawing SDL images to surface
    FOR_EACH (STD_VECTOR<AtlasItem*>::iterator, it, atlas->items)
    {
        AtlasItem *const item = *it;
        imageHelper->copySurfaceToImage(image, item->x, item->y,
            item->image->mSDLSurface);
#ifdef OPENGLERRORS
        logger->log("OpenGL debug:  put atlas image %s (size %dx%d),"
            " into %d,%d to %d,%d",
            item->name.c_str(),
            item->image->mSDLSurface->w,
            item->image->mSDLSurface->h,
            item->x,
            item->y,
            item->x + item->image->mSDLSurface->w - 1,
            item->y + item->image->mSDLSurface->h - 1);
        if (item->x >= width)
            logger->log("OpenGL error: start x position outside of atlas");
        if (item->y >= height)
            logger->log("OpenGL error: start y position outside of atlas");
        if (item->x + item->image->mSDLSurface->w - 1 >= width)
            logger->log("OpenGL error: end x position outside of atlas");
        if (item->y + item->image->mSDLSurface->h - 1 >= height)
            logger->log("OpenGL error: end y position outside of atlas");
#endif  // OPENGLERRORS
    }
    atlas->atlasImage = image;
    BLOCK_END("AtlasManager::createSDLAtlas")
}

void AtlasManager::convertEmptyAtlas(TextureAtlas *const atlas)
{
    // no check for null pointer in atlas because it was in caller
    // convert surface to OpemGL image
    Image *const oldImage = atlas->atlasImage;

    if (oldImage->mSDLSurface != nullptr)
    {
        atlas->atlasImage = imageHelper->loadSurface(
            atlas->atlasImage->mSDLSurface);
        oldImage->decRef();
    }

    Image *const image = atlas->atlasImage;
    if (image == nullptr)
        return;

    image->mIdPath = atlas->name;
#ifdef DEBUG_IMAGES
    logger->log("set name %p, %s", static_cast<void*>(image),
        image->mIdPath.c_str());
#endif  // DEBUG_IMAGES

    image->incRef();

    FOR_EACH (STD_VECTOR<AtlasItem*>::iterator, it, atlas->items)
    {
        AtlasItem *const item = *it;
        // delete SDL Image
        delete item->image;
        // store OpenGL image
        item->image = image->getSubImage(item->x, item->y,
            item->width, item->height);
        Image *const image2 = item->image;
        if (image2 != nullptr)
        {
            image2->mIdPath = item->name;
#ifdef DEBUG_IMAGES
            logger->log("set empty name %p, %s", static_cast<void*>(image2),
                image2->mIdPath.c_str());
#endif  // DEBUG_IMAGES

            image2->incRef();
        }
    }
}

void AtlasManager::convertAtlas(TextureAtlas *const atlas)
{
    // no check for null pointer in atlas because it was in caller
    // convert surface to OpemGL image
    Image *const oldImage = atlas->atlasImage;

    if (oldImage->mSDLSurface != nullptr)
    {
        atlas->atlasImage = imageHelper->loadSurface(
            atlas->atlasImage->mSDLSurface);
        oldImage->decRef();
    }

    Image *const image = atlas->atlasImage;
    if (image == nullptr)
        return;

    image->mIdPath = atlas->name;
#ifdef DEBUG_IMAGES
    logger->log("set name %p, %s", static_cast<void*>(image),
        image->mIdPath.c_str());
#endif  // DEBUG_IMAGES

    image->incRef();

    FOR_EACH (STD_VECTOR<AtlasItem*>::iterator, it, atlas->items)
    {
        AtlasItem *const item = *it;
        // delete SDL Image
        delete item->image;
        // store OpenGL image
        item->image = image->getSubImage(item->x, item->y,
            item->width, item->height);
        Image *const image2 = item->image;
        if (image2 != nullptr)
        {
            image2->mIdPath = item->name;
#ifdef DEBUG_IMAGES
            logger->log("set name %p, %s", static_cast<void*>(image2),
                image2->mIdPath.c_str());
#endif  // DEBUG_IMAGES

            image2->incRef();
        }
    }
}

void AtlasManager::injectToResources(const AtlasResource *const resource)
{
    if (resource == nullptr)
        return;
    FOR_EACH (STD_VECTOR<TextureAtlas*>::const_iterator,
              it, resource->atlases)
    {
        // add each atlas image to resources
        TextureAtlas *const atlas = *it;
        if (atlas != nullptr)
        {
            Image *const image = atlas->atlasImage;
            if (image != nullptr)
                ResourceManager::addResource(atlas->name, image);
            FOR_EACH (STD_VECTOR<AtlasItem*>::iterator, it2, atlas->items)
            {
                AtlasItem *const item = *it2;
                if (item == nullptr)
                    continue;
                // add each atlas sub image to resources
                ResourceManager::addResource(item->name, item->image);
            }
        }
    }
}

void AtlasManager::moveToDeleted(AtlasResource *const resource)
{
    if (resource == nullptr)
        return;
    FOR_EACH (STD_VECTOR<TextureAtlas*>::iterator, it, resource->atlases)
    {
        // move each atlas image to deleted
        TextureAtlas *const atlas = *it;
        if (atlas != nullptr)
        {
            Image *const image = atlas->atlasImage;
            if (image != nullptr)
            {
                // move each atlas image to deleted
                ResourceManager::moveToDeleted(image);
            }
            FOR_EACH (STD_VECTOR<AtlasItem*>::iterator, it2, atlas->items)
            {
                AtlasItem *const item = *it2;
                if (item != nullptr)
                {
                    Image *const image2 = item->image;
                    if (image2 != nullptr)
                    {
                        // move each atlas sub image to deleted
                        ResourceManager::moveToDeleted(image2);
                    }
                }
            }
        }
    }
}

#endif  // USE_OPENGL
