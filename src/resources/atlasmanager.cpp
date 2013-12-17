/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2013  The ManaPlus Developers
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

#include "render/surfacegraphics.h"

#include "utils/mathutils.h"
#include "utils/physfscheckutils.h"
#include "utils/physfsrwops.h"
#include "utils/sdlcheckutils.h"

#include "resources/dye.h"
#include "resources/imagehelper.h"
#include "resources/openglimagehelper.h"
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
    const int maxSize = OpenGLImageHelper::getTextureSize();

    // sorting images on atlases.
    simpleSort(name, atlases, images, maxSize);

    FOR_EACH (std::vector<TextureAtlas*>::iterator, it, atlases)
    {
        TextureAtlas *const atlas = *it;
        if (!atlas)
            continue;

        // create atlas base on sorted images
        SDL_Surface *const surface = createSDLAtlas(atlas);

        if (!surface)
            continue;

        // debug save
//        ImageWriter::writePNG(surface, client->getTempDirectory()
//            + "/atlas" + name + toString(k) + ".png");
//        k ++;

        // convert SDL images to OpenGL
        convertAtlas(atlas);

        // free SDL atlas surface
        MSDL_FreeSurface(surface);

        resource->atlases.push_back(atlas);
    }

    return resource;
}

void AtlasManager::loadImages(const StringVect &files,
                              std::vector<Image*> &images)
{
    ResourceManager *const resman = ResourceManager::getInstance();

    FOR_EACH (StringVectCIter, it, files)
    {
        const std::string str = *it;
        // check is image with same name already in cache
        // and if yes, move it to deleted set
        Resource *const res = resman->getTempResource(str);
        if (res)
        {
            // increase counter because in moveToDeleted it will be decreased.
            res->incRef();
            resman->moveToDeleted(res);
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
#endif
                images.push_back(image);
            }
        }
        delete d;
    }
}

void AtlasManager::simpleSort(const std::string &restrict name,
                              std::vector<TextureAtlas*> &restrict atlases,
                              std::vector<Image*> &restrict images, int size)
{
    int x = 0;
    int y = 0;
    int tempHeight = 0;
    TextureAtlas *atlas = new TextureAtlas();
    std::vector<Image*>::iterator it = images.begin();
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

            // cant put image with this height
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
}

SDL_Surface *AtlasManager::createSDLAtlas(TextureAtlas *const atlas)
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

    const int width = atlas->width;
    const int height = atlas->height;
    // temp SDL surface for atlas
    SDL_Surface *const surface = MSDL_CreateRGBSurface(SDL_SWSURFACE,
        width, height, 32, rmask, gmask, bmask, amask);
    if (!surface)
        return nullptr;

    SurfaceGraphics *const graphics = new SurfaceGraphics();
    graphics->setTarget(surface);
    graphics->_beginDraw();

    // drawing SDL images to surface
    FOR_EACH (std::vector<AtlasItem*>::iterator, it, atlas->items)
    {
        AtlasItem *const item = *it;
        Image *const image = item->image;

        if (image)
        {
            if (image->mSDLSurface)
            {
#ifdef USE_SDL2
                SDL_SetSurfaceAlphaMod(image->mSDLSurface, SDL_ALPHA_OPAQUE);
                SDL_SetSurfaceBlendMode(image->mSDLSurface,
                    SDL_BLENDMODE_NONE);
#else
                SDL_SetAlpha(image->mSDLSurface, 0, SDL_ALPHA_OPAQUE);
#endif
                DRAW_IMAGE(graphics, image, item->x, item->y);
            }
        }
    }

    delete graphics;
    atlas->surface = surface;
    return surface;
}

void AtlasManager::convertAtlas(TextureAtlas *const atlas)
{
    // no check for null pointer in atlas because it was in caller
    // convert surface to OpemGL image
    atlas->atlasImage = imageHelper->load(atlas->surface);
    Image *const image = atlas->atlasImage;
    if (!image)
        return;

    image->mIdPath = atlas->name;
#ifdef DEBUG_IMAGES
    logger->log("set name %p, %s", static_cast<void*>(image),
        image->mIdPath.c_str());
#endif
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
#endif
            image2->incRef();
        }
    }
}

void AtlasManager::injectToResources(AtlasResource *const resource)
{
    ResourceManager *const resman = ResourceManager::getInstance();
    FOR_EACH (std::vector<TextureAtlas*>::iterator, it, resource->atlases)
    {
        // add each atlas image to resources
        TextureAtlas *const atlas = *it;
        if (atlas)
        {
            Image *const image = atlas->atlasImage;
            if (image)
                resman->addResource(atlas->name, image);
            FOR_EACH (std::vector<AtlasItem*>::iterator, it2, atlas->items)
            {
                AtlasItem *const item = *it2;
                if (!item)
                    continue;
                // add each atlas sub image to resources
                resman->addResource(item->name, item->image);
            }
        }
    }
}

void AtlasManager::moveToDeleted(AtlasResource *const resource)
{
    ResourceManager *const resman = ResourceManager::getInstance();
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
                resman->moveToDeleted(image);
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
                        resman->moveToDeleted(image2);
                    }
                }
            }
        }
    }
}

AtlasResource::~AtlasResource()
{
    FOR_EACH (std::vector<TextureAtlas*>::iterator, it, atlases)
    {
        TextureAtlas *const atlas = *it;
        if (atlas)
        {
            FOR_EACH (std::vector<AtlasItem*>::iterator, it2, atlas->items)
            {
                AtlasItem *const item = *it2;
                if (item)
                {
                    Image *const image2 = item->image;
                    if (image2)
                        image2->decRef();
                    delete item;
                }
            }
            Image *const image = atlas->atlasImage;
            if (image)
                image->decRef();
            delete atlas;
        }
    }
    ResourceManager *const resman = ResourceManager::getInstance();
    resman->clearDeleted(false);
}

void AtlasResource::incRef()
{
    if (!getRefCount())
        AtlasManager::injectToResources(this);
    Resource::incRef();
}

void AtlasResource::decRef()
{
    Resource::decRef();
    if (!getRefCount())
        AtlasManager::moveToDeleted(this);
}

#endif
