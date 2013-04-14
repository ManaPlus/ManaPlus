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

#ifndef ATLASMANAGER_H
#define ATLASMANAGER_H

#ifdef USE_OPENGL

#include "resources/image.h"

#include "utils/stringvector.h"

#include <SDL/SDL.h>

class Resource;

struct AtlasItem final
{
    explicit AtlasItem(Image *const image0) :
        image(image0),
        x(0),
        y(0),
        width(image0->mBounds.w),
        height(image0->mBounds.h)
    {
    }

    A_DELETE_COPY(AtlasItem)

    Image *image;
    std::string name;
    int x;
    int y;
    int width;
    int height;
};

struct TextureAtlas final
{
    TextureAtlas() :
        atlasImage(nullptr),
        surface(nullptr),
        width(0),
        height(0)
    {
    }

    A_DELETE_COPY(TextureAtlas)

    std::string name;
    Image *atlasImage;
    SDL_Surface *surface;
    int width;
    int height;
    std::vector <AtlasItem*> items;
};

class AtlasResource final : public Resource
{
    public:
        AtlasResource()
        { }

        A_DELETE_COPY(AtlasResource)

        ~AtlasResource();

        void incRef();

        void decRef();

        std::vector<TextureAtlas*> atlases;
};

class AtlasManager final
{
    public:
        AtlasManager();

        A_DELETE_COPY(AtlasManager)

        static AtlasResource *loadTextureAtlas(const std::string &name,
                                               const StringVect &files)
                                               A_WARN_UNUSED;

        static void injectToResources(AtlasResource *const resource);

        static void moveToDeleted(AtlasResource *const resource);

    private:
        static void loadImages(const StringVect &files,
                               std::vector<Image*> &images);

        static void simpleSort(const std::string &name,
                               std::vector<TextureAtlas*> &atlases,
                               std::vector<Image*> &images, int size);

        static SDL_Surface *createSDLAtlas(TextureAtlas *const atlas)
                                           A_WARN_UNUSED;


        static void convertAtlas(TextureAtlas *const atlas);
};

#endif
#endif
