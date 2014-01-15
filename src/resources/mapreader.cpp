/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "resources/mapreader.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "logger.h"
#include "main.h"
#include "map.h"
#include "tileset.h"

#include "resources/animation.h"
#include "resources/image.h"
#include "resources/resourcemanager.h"

#include "resources/db/mapdb.h"

#include "utils/base64.h"

#include <iostream>
#include <zlib.h>

#include "debug.h"

static int inflateMemory(unsigned char *restrict const in,
                         const unsigned int inLength,
                         unsigned char *&restrict out,
                         unsigned int &restrict outLength);

static int inflateMemory(unsigned char *restrict const in,
                         const unsigned int inLength,
                         unsigned char *&restrict out);

static std::string resolveRelativePath(std::string base, std::string relative)
{
    // Remove trailing "/", if present
    size_t i = base.length();
    if (base.at(i - 1) == '/')
        base.erase(i - 1, i);

    while (relative.substr(0, 3) == "../")
    {
        relative.erase(0, 3);  // Remove "../"
        if (!base.empty())  // If base is already empty, we can't trim anymore
        {
            i = base.find_last_of('/');
            if (i == std::string::npos)
                i = 0;
            base.erase(i, base.length());  // Remove deepest folder in base
        }
    }

    // Re-add trailing slash, if needed
    if (!base.empty() && base[base.length() - 1] != '/')
        base.append("/");

    return base + relative;
}

/**
 * Inflates either zlib or gzip deflated memory. The inflated memory is
 * expected to be freed by the caller.
 */
int inflateMemory(unsigned char *restrict const in,
                  const unsigned int inLength,
                  unsigned char *&restrict out,
                  unsigned int &restrict outLength)
{
    int bufferSize = 256 * 1024;
    out = static_cast<unsigned char*>(calloc(bufferSize, 1));

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = in;
    strm.avail_in = inLength;
    strm.next_out = out;
    strm.avail_out = bufferSize;

    int ret = inflateInit2(&strm, 15 + 32);
    if (ret != Z_OK)
        return ret;

    do
    {
        if (!strm.next_out)
        {
            inflateEnd(&strm);
            return Z_MEM_ERROR;
        }

        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR)
            return ret;

        switch (ret)
        {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void) inflateEnd(&strm);
                return ret;
            default:
                break;
        }

        if (ret != Z_STREAM_END)
        {
            out = static_cast<unsigned char*>(realloc(out, bufferSize * 2));

            if (!out)
            {
                inflateEnd(&strm);
                return Z_MEM_ERROR;
            }

            strm.next_out = out + bufferSize;
            strm.avail_out = bufferSize;
            bufferSize *= 2;
        }
    }
    while (ret != Z_STREAM_END);

    outLength = bufferSize - strm.avail_out;
    (void) inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int inflateMemory(unsigned char *restrict const in,
                  const unsigned int inLength,
                  unsigned char *&restrict out)
{
    unsigned int outLength = 0;
    const int ret = inflateMemory(in, inLength, out, outLength);

    if (ret != Z_OK || !out)
    {
        if (ret == Z_MEM_ERROR)
        {
            logger->log1("Error: Out of memory while decompressing map data!");
        }
        else if (ret == Z_VERSION_ERROR)
        {
            logger->log1("Error: Incompatible zlib version!");
        }
        else if (ret == Z_DATA_ERROR)
        {
            logger->log1("Error: Incorrect zlib compressed data!");
        }
        else
        {
            logger->log1("Error: Unknown error while decompressing map data!");
        }

        free(out);
        out = nullptr;
        outLength = 0;
    }

    return outLength;
}

Map *MapReader::readMap(const std::string &restrict filename,
                        const std::string &restrict realFilename)
{
    BLOCK_START("MapReader::readMap")
    logger->log("Attempting to read map %s", realFilename.c_str());
    // Load the file through resource manager
    const ResourceManager *const resman = ResourceManager::getInstance();
    int fileSize;
    void *buffer = resman->loadFile(realFilename, fileSize);
    Map *map = nullptr;

    if (!buffer)
    {
        BLOCK_END("MapReader::readMap")
        return createEmptyMap(filename, realFilename);
    }

    unsigned char *inflated = nullptr;
    unsigned int inflatedSize;

    if (realFilename.find(".gz", realFilename.length() - 3)
        != std::string::npos)
    {
        // Inflate the gzipped map data
        inflatedSize = inflateMemory(static_cast<unsigned char*>(buffer),
            fileSize, inflated);
        free(buffer);

        if (!inflated)
        {
            logger->log("Could not decompress map file (%s)",
                        realFilename.c_str());
            BLOCK_END("MapReader::readMap")
            return nullptr;
        }
    }
    else
    {
        inflated = static_cast<unsigned char*>(buffer);
        inflatedSize = fileSize;
    }

    XML::Document doc(reinterpret_cast<char*>(inflated), inflatedSize);
    free(inflated);

    XmlNodePtrConst node = doc.rootNode();

    // Parse the inflated map data
    if (node)
    {
        if (!xmlNameEqual(node, "map"))
            logger->log("Error: Not a map file (%s)!", realFilename.c_str());
        else
            map = readMap(node, realFilename);
    }
    else
    {
        logger->log("Error while parsing map file (%s)!",
                    realFilename.c_str());
    }

    if (map)
    {
        map->setProperty("_filename", realFilename);
        map->setProperty("_realfilename", filename);

        if (map->getProperty("music").empty())
            updateMusic(map);
    }

    BLOCK_END("MapReader::readMap")
    return map;
}

Map *MapReader::readMap(XmlNodePtrConst node, const std::string &path)
{
    if (!node)
        return nullptr;

    // Take the filename off the path
    const std::string pathDir = path.substr(0, path.rfind("/") + 1);

    const int w = XML::getProperty(node, "width", 0);
    const int h = XML::getProperty(node, "height", 0);
    const int tilew = XML::getProperty(node, "tilewidth", -1);
    const int tileh = XML::getProperty(node, "tileheight", -1);

    const bool showWarps = config.getBoolValue("warpParticle");
    const std::string warpPath = paths.getStringValue("particles")
        .append(paths.getStringValue("portalEffectFile"));

    if (tilew < 0 || tileh < 0)
    {
        logger->log("MapReader: Warning: "
                    "Unitialized tile width or height value for map: %s",
                    path.c_str());
        return nullptr;
    }

    Map *const map = new Map(w, h, tilew, tileh);

    const std::string fileName = path.substr(path.rfind("/") + 1);
    map->setProperty("shortName", fileName);
    ResourceManager *const resman = ResourceManager::getInstance();

#ifdef USE_OPENGL
    if (graphicsManager.getUseAtlases())
    {
        const MapDB::MapInfo *const info = MapDB::getMapAtlas(fileName);
        if (info)
        {
            map->setAtlas(resman->getAtlas(
                info->atlas, *info->files));
        }
    }
#endif

    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "tileset"))
        {
            Tileset *const tileset = readTileset(childNode, pathDir, map);
            if (tileset)
                map->addTileset(tileset);
        }
        else if (xmlNameEqual(childNode, "layer"))
        {
            readLayer(childNode, map);
        }
        else if (xmlNameEqual(childNode, "properties"))
        {
            readProperties(childNode, map);
            map->setVersion(atoi(map->getProperty(
                "manaplus version").c_str()));
        }
        else if (xmlNameEqual(childNode, "objectgroup"))
        {
            // The object group offset is applied to each object individually
            const int tileOffsetX = XML::getProperty(childNode, "x", 0);
            const int tileOffsetY = XML::getProperty(childNode, "y", 0);
            const int offsetX = tileOffsetX * tilew;
            const int offsetY = tileOffsetY * tileh;

            for_each_xml_child_node(objectNode, childNode)
            {
                if (xmlNameEqual(objectNode, "object"))
                {
                    std::string objType = XML::getProperty(
                        objectNode, "type", "");

                    objType = toUpper(objType);

/*
                    if (objType == "NPC" ||
                        objType == "SCRIPT")
                    {
                        logger->log("hidden obj: " + objType);
                        // Silently skip server-side objects.
                        continue;
                    }
*/

                    const std::string objName = XML::getProperty(
                        objectNode, "name", "");
                    const int objX = XML::getProperty(objectNode, "x", 0);
                    const int objY = XML::getProperty(objectNode, "y", 0);
                    const int objW = XML::getProperty(objectNode, "width", 0);
                    const int objH = XML::getProperty(objectNode, "height", 0);

                    logger->log("- Loading object name: %s type: %s at %d:%d"
                        " (%dx%d)", objName.c_str(), objType.c_str(),
                        objX, objY, objW, objH);

                    if (objType == "PARTICLE_EFFECT")
                    {
                        if (objName.empty())
                        {
                            logger->log1("   Warning: No particle file given");
                            continue;
                        }

                        map->addParticleEffect(objName,
                                               objX + offsetX,
                                               objY + offsetY,
                                               objW, objH);
                    }
                    else if (objType == "WARP")
                    {
                        if (showWarps)
                        {
                            map->addParticleEffect(warpPath,
                                objX, objY, objW, objH);
                        }
                        map->addPortal(objName, MapItem::PORTAL,
                                       objX, objY, objW, objH);
                    }
                    else if (objType == "SPAWN")
                    {
//                      map->addPortal(_("Spawn: ") + objName, MapItem::PORTAL,
//                                     objX, objY, objW, objH);
                    }
                    else if (objType == "MUSIC")
                    {
                        map->addRange(objName, MapItem::MUSIC,
                            objX, objY, objW, objH);
                    }
                    else
                    {
                        logger->log1("   Warning: Unknown object type");
                    }
                }
            }
        }
    }

    map->initializeAmbientLayers();
    map->clearIndexedTilesets();
    map->setActorsFix(0, atoi(map->getProperty("actorsfix").c_str()));
    map->reduce();
    map->setWalkLayer(resman->getWalkLayer(fileName, map));
    return map;
}

void MapReader::readProperties(const XmlNodePtrConst node,
                               Properties *const props)
{
    if (!node || !props)
        return;

    for_each_xml_child_node(childNode, node)
    {
        if (!xmlNameEqual(childNode, "property"))
            continue;

        // Example: <property name="name" value="value"/>
        const std::string name = XML::getProperty(childNode, "name", "");
        const std::string value = XML::getProperty(childNode, "value", "");

        if (!name.empty() && !value.empty())
            props->setProperty(name, value);
    }
}

inline static void setTile(Map *const map, MapLayer *const layer,
                           const MapLayer::Type &layerType,
                           MapHeights *const heights,
                           const int x, const int y, const int gid)
{
    const Tileset * const set = map->getTilesetWithGid(gid);
    switch (layerType)
    {
        case MapLayer::TILES:
        {
            Image *const img = set ? set->get(gid - set->getFirstGid())
                : nullptr;
            layer->setTile(x, y, img);
            break;
        }

        case MapLayer::COLLISION:
        {
            if (set)
            {
                if (map->getVersion() >= 1)
                {
                    switch (gid - set->getFirstGid())
                    {
                        case Map::COLLISION_EMPTY:
                            map->blockTile(x, y, Map::BLOCKTYPE_GROUND);
                            break;
                        case Map::COLLISION_WALL:
                            map->blockTile(x, y, Map::BLOCKTYPE_WALL);
                            break;
                        case Map::COLLISION_AIR:
                            map->blockTile(x, y, Map::BLOCKTYPE_AIR);
                            break;
                        case Map::COLLISION_WATER:
                            map->blockTile(x, y, Map::BLOCKTYPE_WATER);
                            break;
                        case Map::COLLISION_GROUNDTOP:
                            map->blockTile(x, y, Map::BLOCKTYPE_GROUNDTOP);
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    if (gid - set->getFirstGid() != 0)
                        map->blockTile(x, y, Map::BLOCKTYPE_WALL);
                }
            }
            break;
        }

        case MapLayer::HEIGHTS:
        {
            if (!set)
                break;
            if (map->getVersion() >= 2)
            {
                heights->setHeight(x, y, gid - set->getFirstGid() + 1);
            }
            else
            {
                Image *const img = set->get(gid - set->getFirstGid());
                if (layer)
                    layer->setTile(x, y, img);
            }
            break;
        }

        default:
            break;
    }
}

#define addTile() \
    setTile(map, layer, layerType, heights, x, y, gid); \
    if (hasAnimations) \
    { \
        TileAnimationMapCIter it = tileAnimations.find(gid); \
        if (it != tileAnimations.end()) \
        { \
            TileAnimation *const ani = it->second; \
            if (ani) \
                ani->addAffectedTile(layer, x + y * w); \
        } \
    } \

bool MapReader::readBase64Layer(const XmlNodePtrConst childNode,
                                Map *const map,
                                MapLayer *const layer,
                                const MapLayer::Type &layerType,
                                MapHeights *const heights,
                                const std::string &compression,
                                int &restrict x, int &restrict y,
                                const int w, const int h)
{
    if (!compression.empty() && compression != "gzip"
        && compression != "zlib")
    {
        logger->log1("Warning: only gzip and zlib layer"
            " compression supported!");
        return false;
    }

    // Read base64 encoded map file
    XmlNodePtrConst dataChild = childNode->xmlChildrenNode;
    if (!dataChild)
        return true;

    const int len = static_cast<int>(strlen(
        reinterpret_cast<const char*>(dataChild->content)) + 1);
    unsigned char *charData = new unsigned char[len + 1];
    xmlChar *const xmlChars = xmlNodeGetContent(dataChild);
    const char *charStart = reinterpret_cast<const char*>(xmlChars);
    if (!charStart)
    {
        delete [] charData;
        return false;
    }

    unsigned char *charIndex = charData;

    while (*charStart)
    {
        if (*charStart != ' ' && *charStart != '\t' &&
            *charStart != '\n')
        {
            *charIndex = *charStart;
            charIndex++;
        }
        charStart++;
    }
    *charIndex = '\0';

    int binLen;
    unsigned char *binData = php3_base64_decode(charData,
        static_cast<int>(strlen(reinterpret_cast<char*>(
        charData))), &binLen);

    delete [] charData;
    xmlFree(xmlChars);

    if (binData)
    {
        if (compression == "gzip" || compression == "zlib")
        {
            // Inflate the gzipped layer data
            unsigned char *inflated = nullptr;
            const unsigned int inflatedSize =
                inflateMemory(binData, binLen, inflated);

            free(binData);
            binData = inflated;
            binLen = inflatedSize;

            if (!inflated)
            {
                logger->log1("Error: Could not decompress layer!");
                return false;
            }
        }

        const std::map<int, TileAnimation*> &tileAnimations
            = map->getTileAnimations();

        const bool hasAnimations = !tileAnimations.empty();
        for (int i = 0; i < binLen - 3; i += 4)
        {
            const int gid = binData[i] |
                binData[i + 1] << 8 |
                binData[i + 2] << 16 |
                binData[i + 3] << 24;

            addTile();

            x++;
            if (x == w)
            {
                x = 0; y++;

                // When we're done, don't crash on too much data
                if (y == h)
                    break;
            }
        }
        free(binData);
    }
    return true;
}

bool MapReader::readCsvLayer(const XmlNodePtrConst childNode,
                             Map *const map,
                             MapLayer *const layer,
                             const MapLayer::Type &layerType,
                             MapHeights *const heights,
                             int &restrict x, int &restrict y,
                             const int w, const int h)
{
    XmlNodePtrConst dataChild = childNode->xmlChildrenNode;
    if (!dataChild)
        return true;

    xmlChar *const xmlChars = xmlNodeGetContent(dataChild);
    const char *const data = reinterpret_cast<const char*>(xmlChars);
    if (!data)
        return false;

    std::string csv(data);
    size_t oldPos = 0;

    const std::map<int, TileAnimation*> &tileAnimations
        = map->getTileAnimations();
    const bool hasAnimations = !tileAnimations.empty();

    while (oldPos != csv.npos)
    {
        const size_t pos = csv.find_first_of(",", oldPos);
        if (pos == csv.npos)
            return false;

        const int gid = atoi(csv.substr(oldPos, pos - oldPos).c_str());
        addTile();

        x++;
        if (x == w)
        {
            x = 0; y++;

            // When we're done, don't crash on too much data
            if (y == h)
                return false;
        }

        oldPos = pos + 1;
    }
    xmlFree(xmlChars);
    return true;
}

void MapReader::readLayer(const XmlNodePtr node, Map *const map)
{
    // Layers are not necessarily the same size as the map
    const int w = XML::getProperty(node, "width", map->getWidth());
    const int h = XML::getProperty(node, "height", map->getHeight());
    const int offsetX = XML::getProperty(node, "x", 0);
    const int offsetY = XML::getProperty(node, "y", 0);
    std::string name = XML::getProperty(node, "name", "");
    name = toLower(name);

    const bool isFringeLayer = (name.substr(0, 6) == "fringe");
    const bool isCollisionLayer = (name.substr(0, 9) == "collision");
    const bool isHeightLayer = (name.substr(0, 7) == "heights");
    int mask = 1;

    MapLayer::Type layerType = MapLayer::TILES;
    if (isCollisionLayer)
        layerType = MapLayer::COLLISION;
    else if (isHeightLayer)
        layerType = MapLayer::HEIGHTS;

    map->indexTilesets();

    MapLayer *layer = nullptr;
    MapHeights *heights = nullptr;

    logger->log("- Loading layer \"%s\"", name.c_str());
    int x = 0;
    int y = 0;

    // Load the tile data
    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "properties"))
        {
            for_each_xml_child_node(prop, childNode)
            {
                if (!xmlNameEqual(prop, "property"))
                    continue;
                const std::string pname = XML::getProperty(prop, "name", "");
                const std::string value = XML::getProperty(prop, "value", "");
                // ignoring any layer if property Hidden is 1
                if (pname == "Hidden")
                {
                    if (value == "1")
                        return;
                }
                else if (pname == "Version")
                {
                    if (value > CHECK_VERSION)
                        return;
                }
                else if (pname == "NotVersion")
                {
                    if (value <= CHECK_VERSION)
                        return;
                }
                else if (pname == "Mask")
                {
                    mask = atoi(value.c_str());
                }
            }
        }

        if (!xmlNameEqual(childNode, "data"))
            continue;

        if (layerType == MapLayer::TILES)
        {
            layer = new MapLayer(offsetX, offsetY, w, h, isFringeLayer, mask);
            map->addLayer(layer);
        }
        else if (layerType == MapLayer::HEIGHTS)
        {
            heights = new MapHeights(w, h);
            map->addHeights(heights);
        }

        const std::string encoding =
            XML::getProperty(childNode, "encoding", "");
        const std::string compression =
            XML::getProperty(childNode, "compression", "");

        if (encoding == "base64")
        {
            if (readBase64Layer(childNode, map, layer, layerType,
                heights, compression, x, y, w, h))
            {
                continue;
            }
            else
            {
                return;
            }
        }
        else if (encoding == "csv")
        {
            if (readCsvLayer(childNode, map, layer, layerType,
                heights, x, y, w, h))
            {
                continue;
            }
            else
            {
                return;
            }
        }
        else
        {
            const std::map<int, TileAnimation*> &tileAnimations
                = map->getTileAnimations();
            const bool hasAnimations = !tileAnimations.empty();

            // Read plain XML map file
            for_each_xml_child_node(childNode2, childNode)
            {
                if (!xmlNameEqual(childNode2, "tile"))
                    continue;

                const int gid = XML::getProperty(childNode2, "gid", -1);
                addTile();

                x++;
                if (x == w)
                {
                    x = 0; y++;
                    if (y >= h)
                        break;
                }
            }
        }

        if (y < h)
            std::cerr << "TOO SMALL!\n";
        if (x)
            std::cerr << "TOO SMALL!\n";

        // There can be only one data element
        break;
    }
}

Tileset *MapReader::readTileset(XmlNodePtr node,
                                const std::string &path,
                                Map *const map)
{
    if (!map)
        return nullptr;

    const int firstGid = XML::getProperty(node, "firstgid", 0);
    const int margin = XML::getProperty(node, "margin", 0);
    const int spacing = XML::getProperty(node, "spacing", 0);
    XML::Document* doc = nullptr;
    Tileset *set = nullptr;
    std::string pathDir(path);
    std::map<std::string, std::string> props;

    if (XmlHasProp(node, "source"))
    {
        std::string filename = XML::getProperty(node, "source", "");
        filename = resolveRelativePath(path, filename);

        doc = new XML::Document(filename);
        node = doc->rootNode();
        if (!node)
        {
            delete doc;
            return nullptr;
        }

        // Reset path to be realtive to the tsx file
        pathDir = filename.substr(0, filename.rfind("/") + 1);
    }

    const int tw = XML::getProperty(node, "tilewidth", map->getTileWidth());
    const int th = XML::getProperty(node, "tileheight", map->getTileHeight());

    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "image"))
        {
            // ignore second other <image> tags in tileset
            if (set)
                continue;

            const std::string source = XML::getProperty(
                childNode, "source", "");

            if (!source.empty())
            {
                ResourceManager *const resman = ResourceManager::getInstance();
                Image *const tilebmp = resman->getImage(
                    resolveRelativePath(pathDir, source));

                if (tilebmp)
                {
                    set = new Tileset(tilebmp, tw, th, firstGid, margin,
                                      spacing);
                    tilebmp->decRef();
                }
                else
                {
                    logger->log("Warning: Failed to load tileset (%s)",
                            source.c_str());
                }
            }
        }
        else if (xmlNameEqual(childNode, "properties"))
        {
            for_each_xml_child_node(propertyNode, childNode)
            {
                if (!xmlNameEqual(propertyNode, "property"))
                    continue;
                const std::string name = XML::getProperty(
                    propertyNode, "name", "");
                if (!name.empty())
                    props[name] = XML::getProperty(propertyNode, "value", "");
            }
        }
        else if (xmlNameEqual(childNode, "tile"))
        {
            for_each_xml_child_node(tileNode, childNode)
            {
                if (!xmlNameEqual(tileNode, "properties"))
                    continue;

                const int tileGID = firstGid + XML::getProperty(
                    childNode, "id", 0);

                // read tile properties to a map for simpler handling
                std::map<std::string, int> tileProperties;
                for_each_xml_child_node(propertyNode, tileNode)
                {
                    if (!xmlNameEqual(propertyNode, "property"))
                        continue;
                    const std::string name = XML::getProperty(
                        propertyNode, "name", "");
                    const int value = XML::getProperty(
                        propertyNode, "value", 0);
                    if (!name.empty())
                    {
                        tileProperties[name] = value;
                        logger->log("Tile Prop of %d \"%s\" = \"%d\"",
                            tileGID, name.c_str(), value);
                    }
                }

                // create animation
                if (!set || !config.getBoolValue("playMapAnimations"))
                    continue;

                Animation *ani = new Animation;
                for (int i = 0; ; i++)
                {
                    const std::string iStr(toString(i));
                    const std::map<std::string, int>::const_iterator iFrame
                        = tileProperties.find("animation-frame" + iStr);
                    const std::map<std::string, int>::const_iterator iDelay
                        = tileProperties.find("animation-delay" + iStr);
                    // possible need add random attribute?
                    if (iFrame != tileProperties.end()
                        && iDelay != tileProperties.end())
                    {
                        ani->addFrame(set->get(iFrame->second),
                            iDelay->second, 0, 0, 100);
                    }
                    else
                    {
                        break;
                    }
                }

                if (ani->getLength() > 0)
                {
                    map->addAnimation(tileGID, new TileAnimation(ani));
                }
                else
                {
                    delete ani;
                    ani = nullptr;
                }
            }
        }
    }

    delete doc;

    if (set)
        set->setProperties(props);
    return set;
}

Map *MapReader::createEmptyMap(const std::string &restrict filename,
                               const std::string &restrict realFilename)
{
    logger->log1("Creating empty map");
    Map *const map = new Map(300, 300, mapTileSize, mapTileSize);
    map->setProperty("_filename", realFilename);
    map->setProperty("_realfilename", filename);
    updateMusic(map);
    map->setCustom(true);
    MapLayer *layer = new MapLayer(0, 0, 300, 300, false, 1);
    map->addLayer(layer);
    layer = new MapLayer(0, 0, 300, 300, true, 1);
    map->addLayer(layer);

    return map;
}

void MapReader::updateMusic(Map *const map)
{
    std::string name = map->getProperty("shortName");
    const size_t p = name.rfind(".");
    if (p != std::string::npos)
        name = name.substr(0, p);
    name.append(".ogg");
    map->setProperty("music", name);
}
