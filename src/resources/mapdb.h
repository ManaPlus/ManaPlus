/*
 *  Color database
 *  Copyright (C) 2008  Aethyra Development Team
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef MAPDB_H
#define MAPDB_H

#include "utils/stringvector.h"

#include <map>

/**
 * Color information database.
 */
namespace MapDB
{
    struct MapInfo
    {
        std::string atlas;
    };

    /**
     * Loads the map remap data from <code>maps\remap.xml</code>.
     */
    void load();

    void loadRemap();

    void loadInfo();

    /**
     * Clear the remap data
     */
    void unload();

    std::string getMapName(const std::string &name);

    // Maps DB
    typedef std::map<std::string, std::string> Maps;
    typedef Maps::iterator MapIterator;
    // map to infos map
    typedef std::map<std::string, MapInfo> MapInfos;
    // atlas to files map
    typedef std::map<std::string, StringVect> Atlases;
}

#endif
