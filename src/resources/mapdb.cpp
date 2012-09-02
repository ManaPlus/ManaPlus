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

#include "resources/mapdb.h"

#include "configuration.h"
#include "client.h"
#include "logger.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    MapDB::Maps mMaps;
}

void MapDB::load()
{
    if (mLoaded)
        unload();

    XML::Document *doc = new XML::Document(
        paths.getStringValue("maps") + "remap.xml");

    const XmlNodePtr root = doc->rootNode();
    if (!root)
    {
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "map"))
        {
            std::string name = XML::getProperty(node, "name", "");
            if (name.empty())
                continue;

            std::string value = XML::getProperty(node, "value", "");
            if (value.empty())
                continue;

            mMaps[name] = value;
        }
    }

    delete doc;

    mLoaded = true;
}


void MapDB::unload()
{
    logger->log1("Unloading map database...");

    mMaps.clear();
    mLoaded = false;
}

std::string MapDB::getMapName(const std::string &name)
{
    const MapIterator it = mMaps.find(name);

    if (it != mMaps.end())
        return it->second;
    return name;
}
