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

#include "utils/mathutils.h"
#include "utils/physfscheckutils.h"
#include "utils/physfsrwops.h"
#include "utils/sdlcheckutils.h"

#include "resources/openglimagehelper.h"

#include "resources/atlas/atlasresource.h"
#include "resources/atlas/textureatlas.h"

#include "resources/dye/dye.h"

#include "resources/resourcemanager/resourcemanager.h"

#ifndef SDL_BIG_ENDIAN
#include <SDL_endian.h>
#endif  // SDL_BYTEORDER

#include "utils/checkutils.h"

#include "debug.h"

AtlasManager::AtlasManager()
{
}

AtlasResource *AtlasManager::loadTextureAtlas(const std::string &name,
                                              const StringVect &files)
{
    BLOCK_START("AtlasManager::loadTextureAtlas")
    std::vector<TextureAtlas*> atlases;
    std::vector<Image*> images;
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

    FOR_EACH (std::vector<TextureAtlas*>::iterator, it, atlases)
    {
        TextureAtlas *const atlas = *it;
        if (!atlas)
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
    std::vector<TextureAtlas*> atlases;
    std::vector<Image*> images;
    AtlasResource *resource = new AtlasResource;

    loadEmptyImages(files, images);
    int maxSize = OpenGLImageHelper::getTextureSize();

    // sorting images on atlases.
    simpleSort(name, atlases, images, maxSize);

    FOR_EACH (std::vector<TextureAtlas*>::iterator, it, atlases)
    {
        TextureAtlas *const atlas = *it;
        if (!atlas)
            continue;

        atlas->atlasImage = new Image(0,
            8192, 8192,
            8192, 8192);
        // convert SDL images to OpenGL
        convertAtlas(atlas);

        resource->atlases.push_back(atlas);
    }

    BLOCK_END("AtlasManager::loadTextureAtlas")
    return resource;
}

void AtlasManager::loadImages(const StringVect &files,
                              std::vector<Image*> &images)
{
    BLOCK_START("AtlasManager::loadImages")

    FOR_EACH (StringVectCIter, it, files)
    {
        const std::string str = *it;
        // check is image with same name already in cache
        // and if yes, move it to deleted set
        Resource *const res = resourceManager->getTempResource(str);
        if (res)
        {
            // increase counter because in moveToDeleted it will be decreased.
            res->incRef();
            resourceManager->moveToDeleted(res);
        }

        std::string path = str;
        const size_t p = path.find('|');
        Dye *d = nullptr;
        if (p != std::string::npos)
        {
            d = new Dye(path.substr(p + 1));
            path = path.substr(0, p);
        }

        SDL_RWops *const rw = MPHYSFSRWOPS_openRead(path.c_str());
        if (rw)
        {
            Image *const image = d ? surfaceImageHelper->load(rw, *d)
                : surfaceImageHelper->load(rw);

            if (image)
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
                                   std::vector<Image*> &images)
{
    BLOCK_START("AtlasManager::loadEmptyImages")

    FOR_EACH (StringVectCIter, it, files)
    {
        const std::string str = *it;
        // check is image with same name already in cache
        // and if yes, move it to deleted set
        Resource *const res = resourceManager->getTempResource(str);
        if (res)
        {
            // increase counter because in moveToDeleted it will be decreased.
            res->incRef();
            resourceManager->moveToDeleted(res);
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
                              std::vector<TextureAtlas*> &restrict atlases,
                              const std::vector<Image*> &restrict images,
                              int size)
{
    BLOCK_START("AtlasManager::simpleSort")
    int x = 0;
    int y = 0;
    int tempHeight = 0;
    TextureAtlas *atlas = new TextureAtlas();
    std::vector<Image*>::const_iterator it = images.begin();
    const std::vector<Image*>::const_iterator it_end = images.end();
    for (it = images.begin(); it != it_end; ++ it)
    {
        const Image *const img = *it;
        if (img)
        {
            atlas->name = std::string("atlas_").append(name).append(
                "_").append(img->getIdPath());
            break;
        }
    }

    for (it = images.begin(); it != it_end; ++ it)
    {
        Image *const img = *it;
        if (img)
        {
            AtlasItem *const item = new AtlasItem(img);
            item->name = img->getIdPath();
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
                atlas = new TextureAtlas();
                atlas->name = std::string("atlas_").append(name).append(
                    "_").append(img->getIdPath());
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
    if (!surface)
    {
        reportAlways("Error creating surface for atlas. Size: %dx%d",
            width,
            height);
        BLOCK_END("AtlasManager::createSDLAtlas")
        return;
    }
    BLOCK_END("AtlasManager::createSDLAtlas create surface")

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
    FOR_EACH (std::vector<AtlasItem*>::iterator, it, atlas->items)
    {
        AtlasItem *const item = *it;
        imageHelper->copySurfaceToImage(image, item->x, item->y,
            item->image->mSDLSurface);
    }
    atlas->atlasImage = image;
    BLOCK_END("AtlasManager::createSDLAtlas")
}

void AtlasManager::convertAtlas(TextureAtlas *const atlas)
{
    // no check for null pointer in atlas because it was in caller
    // convert surface to OpemGL image
    Image *const oldImage = atlas->atlasImage;

    if (oldImage->mSDLSurface)
    {
        atlas->atlasImage = imageHelper->loadSurface(
            atlas->atlasImage->mSDLSurface);
        oldImage->decRef();
    }

    Image *const image = atlas->atlasImage;
    if (!image)
        return;

    image->mIdPath = atlas->name;
#ifdef DEBUG_IMAGES
    logger->log("set name %p, %s", static_cast<void*>(image),
        image->mIdPath.c_str());
#endif  // DEBUG_IMAGES

    image->incRef();

    FOR_EACH (std::vector<AtlasItem*>::iterator, it, atlas->items)
    {
        AtlasItem *const item = *it;
        // delete SDL Image
        delete item->image;
        // store OpenGL image
        item->image = image->getSubImage(item->x, item->y,
            item->width, item->height);
        Image *const image2 = item->image;
        if (image2)
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
    if (!resource)
        return;
    FOR_EACH (std::vector<TextureAtlas*>::const_iterator,
              it, resource->atlases)
    {
        // add each atlas image to resources
        TextureAtlas *const atlas = *it;
        if (atlas)
        {
            Image *const image = atlas->atlasImage;
            if (image)
                resourceManager->addResource(atlas->name, image);
            FOR_EACH (std::vector<AtlasItem*>::iterator, it2, atlas->items)
            {
                AtlasItem *const item = *it2;
                if (!item)
                    continue;
                // add each atlas sub image to resources
                resourceManager->addResource(item->name, item->image);
            }
        }
    }
}

void AtlasManager::moveToDeleted(AtlasResource *const resource)
{
    if (!resource)
        return;
    FOR_EACH (std::vector<TextureAtlas*>::iterator, it, resource->atlases)
    {
        // move each atlas image to deleted
        TextureAtlas *const atlas = *it;
        if (atlas)
        {
            Image *const image = atlas->atlasImage;
            if (image)
            {
                // move each atlas image to deleted
                resourceManager->moveToDeleted(image);
            }
            FOR_EACH (std::vector<AtlasItem*>::iterator, it2, atlas->items)
            {
                AtlasItem *const item = *it2;
                if (item)
                {
                    Image *const image2 = item->image;
                    if (image2)
                    {
                        // move each atlas sub image to deleted
                        resourceManager->moveToDeleted(image2);
                    }
                }
            }
        }
    }
}

#endif  // USE_OPENGL
