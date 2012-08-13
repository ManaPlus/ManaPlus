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

#ifndef ATLASMANAGER_H
#define ATLASMANAGER_H

#ifdef USE_OPENGL

#include "resources/image.h"

#include "utils/stringvector.h"

#include <SDL/SDL.h>

class Resource;

struct AtlasItem
{
    AtlasItem(Image *image0) :
        image(image0),
        x(0),
        y(0),
        width(image0->mBounds.w),
        height(image0->mBounds.h)
    {
    }

    Image *image;
    std::string name;
    int x;
    int y;
    int width;
    int height;
};

struct TextureAtlas
{
    TextureAtlas() :
        atlasImage(nullptr),
        surface(nullptr),
        width(0),
        height(0)
    {
    }

    std::string name;
    Image *atlasImage;
    SDL_Surface *surface;
    int width;
    int height;
    std::vector <AtlasItem*> items;
};

class AtlasResource : public Resource
{
    public:
        ~AtlasResource();

        std::vector<TextureAtlas*> atlases;
};

class AtlasManager
{
    public:
        AtlasManager();

        static AtlasResource *loadTextureAtlas(const std::string &name,
                                               const StringVect &files);

        static void injectToResources(AtlasResource *resource);

    private:
        static void loadImages(const StringVect &files,
                               std::vector<Image*> &images);

        static void simpleSort(const std::string &name,
                               std::vector<TextureAtlas*> &atlases,
                               std::vector<Image*> &images, int size);

        static SDL_Surface *createSDLAtlas(TextureAtlas *atlas);


        static void convertAtlas(TextureAtlas *atlas);
};

#endif
#endif
