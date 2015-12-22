/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2015  The ManaPlus Developers
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

#ifndef RESOURCES_ATLAS_ATLASMANAGER_H
#define RESOURCES_ATLAS_ATLASMANAGER_H

#ifdef USE_OPENGL

#include "resources/image.h"

#include "utils/stringvector.h"

#include <SDL.h>

class AtlasResource;
class Resource;

struct AtlasItem;
struct TextureAtlas;

class AtlasManager final
{
    public:
        AtlasManager() A_CONST;

        A_DELETE_COPY(AtlasManager)

        static AtlasResource *loadTextureAtlas(const std::string &name,
                                               const StringVect &files)
                                               A_WARN_UNUSED;

        static void injectToResources(const AtlasResource *const resource);

        static void moveToDeleted(AtlasResource *const resource);

    private:
        static void loadImages(const StringVect &files,
                               std::vector<Image*> &images);

        static void simpleSort(const std::string &restrict name,
                               std::vector<TextureAtlas*> &restrict atlases,
                               const std::vector<Image*> &restrict images,
                               int size);

        static SDL_Surface *createSDLAtlas(TextureAtlas *const atlas)
                                           A_WARN_UNUSED A_NONNULL(1);


        static void convertAtlas(TextureAtlas *const atlas) A_NONNULL(1);
};

#endif  // USE_OPENGL
#endif  // RESOURCES_ATLAS_ATLASMANAGER_H
