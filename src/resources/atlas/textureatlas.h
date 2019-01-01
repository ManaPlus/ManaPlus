/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#include "utils/foreach.h"
#include "utils/stringvector.h"

#include "resources/atlas/atlasitem.h"

#include "localconsts.h"

class AtlasResource;
class Image;
class Resource;

struct AtlasItem;

struct TextureAtlas final : public MemoryCounter
{
    TextureAtlas() :
        MemoryCounter(),
        name(),
        atlasImage(nullptr),
        width(0),
        height(0),
        items()
    {
    }

    A_DELETE_COPY(TextureAtlas)

    int calcMemoryLocal() const override final
    {
        return static_cast<int>(sizeof(TextureAtlas) +
            items.capacity() * sizeof(AtlasItem*));
    }

    int calcMemoryChilds(const int level) const override final
    {
        int sz = 0;
        FOR_EACH (STD_VECTOR<AtlasItem*>::const_iterator, it, items)
        {
            AtlasItem *const item = *it;
            sz += item->calcMemory(level + 1);
        }
        return sz;
    }

    std::string getCounterName() const override
    { return name; }

    std::string name;
    Image *atlasImage;
    int width;
    int height;
    STD_VECTOR <AtlasItem*> items;
};

#endif  // USE_OPENGL
#endif  // RESOURCES_ATLAS_TEXTUREATLAS_H
