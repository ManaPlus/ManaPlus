/*
 *  The ManaPlus Client
 *  Copyright (C) 2012  The ManaPlus Developers
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

#include "main.h"

#ifdef USE_OPENGL

#include "resources/atlasmanager.h"
#include "resources/openglimagehelper.h"

#include "client.h"
#include "graphics.h"
#include "graphicsmanager.h"
#include "logger.h"

#include "utils/mathutils.h"
#include "utils/physfsrwops.h"

#include "resources/fboinfo.h"
#include "resources/imagehelper.h"
#include "resources/imagewriter.h"
#include "resources/resourcemanager.h"

#include "debug.h"

AtlasManager::AtlasManager()
{
}

AtlasResource *AtlasManager::loadTextureAtlas(const std::string &name,
                                              const StringVect &files)
{
    std::vector<TextureAtlas*> atlases;
    std::vector<Image*> images;
    AtlasResource *resource = new AtlasResource;

    loadImages(files, images);
    int maxSize = OpenGLImageHelper::getTextureSize();
//    int maxSize = 1024;

    // sorting images on atlases.
    simpleSort(name, atlases, images, maxSize);

//    int k = 0;
    for (std::vector<TextureAtlas*>::iterator it = atlases.begin(),
         it_end = atlases.end(); it != it_end; ++ it)
    {
        TextureAtlas *atlas = *it;
        if (!atlas)
            continue;

        // create atlas base on sorted images
        SDL_Surface *surface = createSDLAtlas(atlas);

        if (!surface)
            continue;

        // debug save
//        ImageWriter::writePNG(surface, Client::getTempDirectory()
//            + "/atlas" + name + toString(k) + ".png");
//        k ++;

        // convert SDL images to OpenGL
        convertAtlas(atlas);

        // free SDL atlas surface
        SDL_FreeSurface(surface);

        resource->atlases.push_back(atlas);
    }

    return resource;
}

void AtlasManager::loadImages(const StringVect &files,
                              std::vector<Image*> &images)
{
    const ResourceManager *const resman = ResourceManager::getInstance();

    for (StringVectCIter it = files.begin(), it_end = files.end();
          it != it_end; ++ it)
    {
        const std::string str = *it;
        if (resman->isInCache(str))
        {
            logger->log("Resource %s already in cache", str.c_str());
            continue;
        }

        SDL_RWops *rw = PHYSFSRWOPS_openRead(str.c_str());
        Image *image = sdlImageHelper->load(rw);
        if (image)
        {
            image->mIdPath = str;
            images.push_back(image);
        }
    }
}

void AtlasManager::simpleSort(const std::string &name,
                              std::vector<TextureAtlas*> &atlases,
                              std::vector<Image*> &images, int size)
{
    int x = 0;
    int y = 0;
    int tempHeight = 0;
    TextureAtlas *atlas = new TextureAtlas();
    std::vector<Image*>::iterator it = images.begin();
    const std::vector<Image*>::const_iterator it_end = images.end();
    for (it = images.begin(); it != it_end; ++ it)
    {
        Image *img = *it;
        if (img)
        {
            atlas->name = "atlas_" + name + "_" + img->getIdPath();
            break;
        }
    }

    for (it = images.begin(); it != it_end; ++ it)
    {
        Image *img = *it;
        if (img)
        {
            AtlasItem *item = new AtlasItem(img);
            item->name = img->getIdPath();
            // start next line
            if (x + img->mBounds.w > size)
            {
                x = 0;
                y += tempHeight;
                tempHeight = 0;
            }

            // cant put image with this height
            if (y + img->mBounds.h > size)
            {
                x = 0;
                y = 0;
                atlases.push_back(atlas);
                atlas = new TextureAtlas();
                atlas->name = "atlas_" + name + "_" + img->getIdPath();
            }

            if (img->mBounds.h > tempHeight)
                tempHeight = img->mBounds.h;

//            logger->log("image draw position: %d,%d (%d,%d)",
//                x, y, img->mBounds.w, img->mBounds.h);
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
}

SDL_Surface *AtlasManager::createSDLAtlas(TextureAtlas *atlas)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
    const int amask = 0x000000ff;
#else
    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
    const int amask = 0xff000000;
#endif

    // do not create atlas based on only one image
    if (atlas->items.size() == 1)
        return nullptr;

    // using only power of two sizes.
    atlas->width = powerOfTwo(atlas->width);
    atlas->height = powerOfTwo(atlas->height);

    // temp SDL surface for atlas
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
        atlas->width, atlas->height, 32, rmask, gmask, bmask, amask);

    if (!surface)
        return nullptr;

    Graphics *graphics = new Graphics();
    graphics->setTarget(surface);
    graphics->_beginDraw();

    // drawing SDL images to surface
    for (std::vector<AtlasItem*>::iterator it = atlas->items.begin(),
         it_end = atlas->items.end();
         it != it_end; ++ it)
    {
        AtlasItem *item = *it;
        Image *image = item->image;

        if (image)
        {
            SDL_SetAlpha(image->mSDLSurface, 0, SDL_ALPHA_OPAQUE);
            graphics->drawImage(image, item->x, item->y);
        }
    }

    delete graphics;
    atlas->surface = surface;
    return surface;
}

void AtlasManager::convertAtlas(TextureAtlas *atlas)
{
    // convert surface to OpemGL image
    atlas->atlasImage = imageHelper->load(atlas->surface);
    Image *const image = atlas->atlasImage;

    for (std::vector<AtlasItem*>::iterator it = atlas->items.begin(),
         it_end = atlas->items.end();
         it != it_end; ++ it)
    {
        AtlasItem *const item = *it;
        // delete SDL Image
        delete item->image;
        // store OpenGL image
        item->image = image->getSubImage(item->x, item->y,
            item->width, item->height);
    }
}

void AtlasManager::injectToResources(AtlasResource *resource)
{
    ResourceManager *const resman = ResourceManager::getInstance();
    int k = 0;
    for (std::vector<TextureAtlas*>::iterator it = resource->atlases.begin(),
         it_end = resource->atlases.end(); it != it_end; ++ it, k ++)
    {
        // add each atlas image to resources
        TextureAtlas *const atlas = *it;
        resman->addResource(atlas->name, atlas->atlasImage);
        if (atlas)
        {
            for (std::vector<AtlasItem*>::iterator it2 = atlas->items.begin(),
                 it2_end = atlas->items.end();
                 it2 != it2_end; ++ it2)
            {
                AtlasItem *const item = *it2;
                // add each atlas sub image to resources
                resman->addResource(item->name, item->image);
            }
        }
    }
}

AtlasResource::~AtlasResource()
{
    for (std::vector<TextureAtlas*>::iterator it = atlases.begin(),
         it_end = atlases.end(); it != it_end; ++ it)
    {
        TextureAtlas *const atlas = *it;
        if (atlas)
        {
            for (std::vector<AtlasItem*>::iterator it2 = atlas->items.begin(),
                 it2_end = atlas->items.end();
                 it2 != it2_end; ++ it2)
            {
                AtlasItem *const item = *it2;
                if (item)
                {
                    Image *const image = item->image;
                    if (image)
                        image->decRef();
                    delete item;
                }
            }
            Image *const image = atlas->atlasImage;
            if (image)
                image->decRef();
            delete atlas;
        }
    }
}

#endif
