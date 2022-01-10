/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Aethyra Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef RESOURCES_DB_MAPDB_H
#define RESOURCES_DB_MAPDB_H

#include "enums/simpletypes/skiperror.h"

#include "resources/mapinfo.h"

#include <map>

#include "localconsts.h"

/**
 * Color information database.
 */
namespace MapDB
{
    /**
     * Loads the map remap data from <code>maps\remap.xml</code>.
     */
    void load();

    void loadRemapXmlFile(const std::string &fileName,
                          const SkipError skipError);

    void loadInfo(const std::string &fileName,
                  const SkipError skipError);

    /**
     * Clear the remap data
     */
    void unload();

    const std::string getMapName(const std::string &name) A_WARN_UNUSED;

    const MapInfo *getMapAtlas(const std::string &name) A_WARN_UNUSED;

    const MapInfo *getAtlas(const std::string &name) A_WARN_UNUSED;

    bool isEmptyTileset(const std::string &name);

    // Maps DB
    typedef std::map<std::string, std::string> Maps;
    typedef Maps::iterator MapIterator;
    // map to infos map
    typedef std::map<std::string, MapInfo> MapInfos;
    typedef MapInfos::iterator MapInfoIter;
    // atlas to files map
    typedef std::map<std::string, StringVect> Atlases;
    typedef Atlases::iterator AtlasIter;
    typedef Atlases::const_iterator AtlasCIter;
}  // namespace MapDB

#endif  // RESOURCES_DB_MAPDB_H
