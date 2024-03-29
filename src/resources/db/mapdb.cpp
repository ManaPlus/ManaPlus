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

#include "resources/db/mapdb.h"

#include "configuration.h"
#include "logger.h"

#include "resources/beingcommon.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    MapDB::Maps mMaps;
    MapDB::MapInfos mInfos;
    MapDB::Atlases mAtlases;
    std::set<std::string> mEmptyTilesets;
}  // namespace

namespace MapDB
{
    void readMap(XmlNodePtrConst node);
    void readAtlas(XmlNodePtrConst node);
}  // namespace MapDB

void MapDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing maps database...");
    loadRemapXmlFile(paths.getStringValue("mapsRemapFile"),
        SkipError_true);
    loadRemapXmlFile(paths.getStringValue("mapsRemapPatchFile"),
        SkipError_true);
    loadXmlDir("mapsRemapPatchDir", loadRemapXmlFile)

    loadInfo(paths.getStringValue("mapsFile"), SkipError_false);
    loadInfo(paths.getStringValue("mapsPatchFile"), SkipError_true);
    loadXmlDir("mapsPatchDir", loadInfo)
    mLoaded = true;
}

void MapDB::loadRemapXmlFile(const std::string &fileName,
                             const SkipError skipError)
{
    XML::Document *const doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);

    XmlNodeConstPtrConst root = doc->rootNode();
    if (root == nullptr)
    {
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "map"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (name.empty())
                continue;

            const std::string value = XML::getProperty(node, "value", "");
            if (value.empty())
                continue;

            mMaps[name] = value;
        }
        else if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadRemapXmlFile(name, skipError);
            continue;
        }
    }

    delete doc;
}

void MapDB::readMap(XmlNodePtrConst node)
{
    if (node == nullptr)
        return;
    const std::string map = XML::getProperty(node, "name", "");
    if (map.empty())
        return;

    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "atlas"))
        {
            const std::string atlas = XML::getProperty(childNode, "name", "");
            if (atlas.empty())
                continue;
            mInfos[map].atlas = atlas;
        }
    }
}

void MapDB::readAtlas(XmlNodePtrConst node)
{
    if (node == nullptr)
        return;
    const std::string atlas = XML::getProperty(node, "name", "");
    if (atlas.empty())
        return;
    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "file"))
        {
            const std::string file = XML::getProperty(childNode, "name", "");
            if (file.empty())
                continue;
            mAtlases[atlas].push_back(file);
        }
    }
    if (atlas == paths.getStringValue("emptyAtlasName"))
    {
        const StringVect *files = &mAtlases[atlas];
        FOR_EACHP (StringVectCIter, it, files)
        {
            mEmptyTilesets.insert(*it);
            logger->log("empty tileset: " + *it);
        }
    }
    else if (atlas != "all")
    {
        const AtlasCIter &allAtlas = mAtlases.find("all");
        if (allAtlas != mAtlases.end())
        {
            FOR_EACH (StringVectCIter, it, (*allAtlas).second)
                mAtlases[atlas].push_back(*it);
        }
    }
}

void MapDB::loadInfo(const std::string &fileName,
                     const SkipError skipError)
{
    XML::Document *doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst root = doc->rootNode();
    if (root == nullptr)
    {
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "map"))
        {
            readMap(node);
        }
        else if (xmlNameEqual(node, "atlas"))
        {
            readAtlas(node);
        }
        else if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadInfo(name, skipError);
            continue;
        }
    }
    delete doc;
}

void MapDB::unload()
{
    logger->log1("Unloading map database...");

    mMaps.clear();
    mLoaded = false;
}

const std::string MapDB::getMapName(const std::string &name)
{
    const MapIterator it = mMaps.find(name);

    if (it != mMaps.end())
        return it->second;
    return name;
}

const MapInfo *MapDB::getMapAtlas(const std::string &name)
{
    const MapInfoIter it = mInfos.find(name);
    if (it == mInfos.end())
        return nullptr;
    MapInfo *const info = &(*it).second;
    const AtlasCIter it2 = mAtlases.find(info->atlas);
    if (it2 == mAtlases.end())
        return nullptr;
    info->files = &((*it2).second);
    return info;
}

const MapInfo *MapDB::getAtlas(const std::string &name)
{
    const AtlasCIter it = mAtlases.find(name);
    if (it == mAtlases.end())
        return nullptr;

    MapInfo *const info = new MapInfo;
    info->atlas = name;
    info->files = &(*it).second;
    return info;
}

bool MapDB::isEmptyTileset(const std::string &name)
{
    return mEmptyTilesets.find(name) != mEmptyTilesets.end();
}
