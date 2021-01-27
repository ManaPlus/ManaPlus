/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef RESOURCES_ATLAS_ATLASRESOURCE_H
#define RESOURCES_ATLAS_ATLASRESOURCE_H

#ifdef USE_OPENGL

#include "utils/vector.h"

#include "resources/resource.h"

struct TextureAtlas;

class AtlasResource final : public Resource
{
    public:
        AtlasResource() :
            atlases()
        { }

        A_DELETE_COPY(AtlasResource)

        ~AtlasResource() override final;

        void incRef() override final;

        void decRef() override final;

        int calcMemoryLocal() const override final;

        int calcMemoryChilds(const int level) const override final;

        STD_VECTOR<TextureAtlas*> atlases;
};

#endif  // USE_OPENGL
#endif  // RESOURCES_ATLAS_ATLASRESOURCE_H
