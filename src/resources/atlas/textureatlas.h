/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2016  The ManaPlus Developers
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

#ifndef RESOURCES_ATLAS_TEXTUREATLAS_H
#define RESOURCES_ATLAS_TEXTUREATLAS_H

#ifdef USE_OPENGL

#include "utils/stringvector.h"

#include <vector>

#include <SDL.h>

#include "localconsts.h"

class AtlasResource;
class Image;
class Resource;

struct AtlasItem;

struct TextureAtlas final
{
    TextureAtlas() :
        name(),
        atlasImage(nullptr),
        width(0),
        height(0),
        items()
    {
    }

    A_DELETE_COPY(TextureAtlas)

    std::string name;
    Image *atlasImage;
    int width;
    int height;
    std::vector <AtlasItem*> items;
};

#endif  // USE_OPENGL
#endif  // RESOURCES_ATLAS_TEXTUREATLAS_H
