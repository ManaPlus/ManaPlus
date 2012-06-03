/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/mapreader.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"
#include "map.h"
#include "maplayer.h"
#include "tileset.h"

#include "resources/animation.h"
#include "resources/image.h"
#include "resources/mapdb.h"
#include "resources/resourcemanager.h"

#include "utils/base64.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <iostream>
#include <zlib.h>

#include "debug.h"

int inflateMemory(unsigned char *in, unsigned int inLength,
                  unsigned char *&out, unsigned int &outLength);

int inflateMemory(unsigned char *in, unsigned int inLength,
                  unsigned char *&out);

static std::string resolveRelativePath(std::string base, std::string relative)
{
    // Remove trailing "/", if present
    size_t i = base.length();
    if (base.at(i - 1) == '/')
        base.erase(i - 1, i);

    while (relative.substr(0, 3) == "../")
    {
        relative.erase(0, 3);  // Remove "../"
        if (!base.empty()) // If base is already empty, we can't trim anymore
        {
            i = base.find_last_of('/');
            if (i == std::string::npos)
                i = 0;
            base.erase(i, base.length()); // Remove deepest folder in base
        }
    }

    // Re-add trailing slash, if needed
    if (!base.empty() && base[base.length() - 1] != '/')
        base += '/';

    return base + relative;
}

/**
 * Inflates either zlib or gzip deflated memory. The inflated memory is
 * expected to be freed by the caller.
 */
int inflateMemory(unsigned char *in, unsigned int inLength,
                  unsigned char *&out, unsigned int &outLength)
{
    int bufferSize = 256 * 1024;
    int ret;
    z_stream strm;

    out = static_cast<unsigned char*>(calloc(bufferSize, 1));

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = in;
    strm.avail_in = inLength;
    strm.next_out = out;
    strm.avail_out = bufferSize;

    ret = inflateInit2(&strm, 15 + 32);

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
//    assert(strm.avail_in == 0);

    outLength = bufferSize - strm.avail_out;
    (void) inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int inflateMemory(unsigned char *in, unsigned int inLength,
                  unsigned char *&out)
{
    unsigned int outLength = 0;
    int ret = inflateMemory(in, inLength, out, outLength);

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

Map *MapReader::readMap(const std::string &filename,
                        const std::string &realFilename)
{
    logger->log("Attempting to read map %s", realFilename.c_str());
    // Load the file through resource manager
    ResourceManager *resman = ResourceManager::getInstance();
    int fileSize;
    void *buffer = resman->loadFile(realFilename, fileSize);
    Map *map = nullptr;

    if (!buffer)
    {
        logger->log("Map file not found (%s)", realFilename.c_str());
        return nullptr;
    }

    unsigned char *inflated;
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

    XmlNodePtr node = doc.rootNode();

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
    }

    return map;
}

Map *MapReader::readMap(XmlNodePtr node, const std::string &path)
{
    if (!node)
        return nullptr;

    // Take the filename off the path
    const std::string pathDir = path.substr(0, path.rfind("/") + 1);

    const int w = XML::getProperty(node, "width", 0);
    const int h = XML::getProperty(node, "height", 0);
    const int tilew = XML::getProperty(node, "tilewidth", -1);
    const int tileh = XML::getProperty(node, "tileheight", -1);

    bool showWarps = config.getBoolValue("warpParticle");
    const std::string warpPath = paths.getStringValue("particles")
        + paths.getStringValue("portalEffectFile");

    if (tilew < 0 || tileh < 0)
    {
        logger->log("MapReader: Warning: "
                    "Unitialized tile width or height value for map: %s",
                    path.c_str());
        return nullptr;
    }

    Map *map = new Map(w, h, tilew, tileh);

    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "tileset"))
        {
            Tileset *tileset = readTileset(childNode, pathDir, map);
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
//                        map->addPortal(_("Spawn: ") + objName, MapItem::PORTAL,
//                                       objX, objY, objW, objH);
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
    return map;
}

void MapReader::readProperties(XmlNodePtr node, Properties *props)
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

inline static void setTile(Map *map, MapLayer *layer, int x, int y, int gid)
{
    const Tileset * const set = map->getTilesetWithGid(gid);
    if (layer)
    {
        // Set regular tile on a layer
        Image * const img = set ? set->get(gid - set->getFirstGid()) : nullptr;
        layer->setTile(x, y, img);
    }
    else
    {
        // Set collision tile
//        if (set && (gid - set->getFirstGid() == 1))   buggy update
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
    }
}

void MapReader::readLayer(XmlNodePtr node, Map *map)
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

    map->indexTilesets();

    MapLayer *layer = nullptr;

    logger->log("- Loading layer \"%s\"", name.c_str());
    int x = 0;
    int y = 0;

    // Load the tile data
    for_each_xml_child_node(childNode, node)
    {
        if (serverVersion > 0 && xmlNameEqual(childNode, "properties"))
        {
            for_each_xml_child_node(prop, childNode)
            {
                if (!xmlNameEqual(prop, "property"))
                    continue;
                const std::string pname = XML::getProperty(prop, "name", "");
                const std::string value = XML::getProperty(prop, "value", "");
                // ignoring any layer if property Hidden is 1
                if (pname == "Hidden" && value == "1")
                    return;
                if (pname == "Version" && value > CHECK_VERSION)
                    return;
                if (pname == "NotVersion" && value <= CHECK_VERSION)
                    return;
            }
        }

        if (!xmlNameEqual(childNode, "data"))
            continue;

        if (!isCollisionLayer)
        {
            layer = new MapLayer(offsetX, offsetY, w, h, isFringeLayer);
            map->addLayer(layer);
        }

        const std::string encoding =
            XML::getProperty(childNode, "encoding", "");
        const std::string compression =
            XML::getProperty(childNode, "compression", "");

        if (encoding == "base64")
        {
            if (!compression.empty() && compression != "gzip"
                && compression != "zlib")
            {
                logger->log1("Warning: only gzip layer"
                             " compression supported!");
                return;
            }

            // Read base64 encoded map file
            XmlNodePtr dataChild = childNode->xmlChildrenNode;
            if (!dataChild)
                continue;

            int len = static_cast<int>(strlen(
                reinterpret_cast<const char*>(dataChild->content)) + 1);
            unsigned char *charData = new unsigned char[len + 1];
            xmlChar *xmlChars = xmlNodeGetContent(dataChild);
            const char *charStart = reinterpret_cast<const char*>(xmlChars);
            if (!charStart)
            {
                delete [] charData;
                return;
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
                    unsigned char *inflated;
                    unsigned int inflatedSize =
                        inflateMemory(binData, binLen, inflated);

                    free(binData);
                    binData = inflated;
                    binLen = inflatedSize;

                    if (!inflated)
                    {
                        logger->log1("Error: Could not decompress layer!");
                        return;
                    }
                }

                for (int i = 0; i < binLen - 3; i += 4)
                {
                    const int gid = binData[i] |
                        binData[i + 1] << 8 |
                        binData[i + 2] << 16 |
                        binData[i + 3] << 24;

                    setTile(map, layer, x, y, gid);

                    TileAnimation* ani = map->getAnimationForGid(gid);
                    if (ani)
                        ani->addAffectedTile(layer, x + y * w);

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
        }
        else if (encoding == "csv")
        {
            XmlNodePtr dataChild = childNode->xmlChildrenNode;
            if (!dataChild)
                continue;

            xmlChar *xmlChars = xmlNodeGetContent(dataChild);
            const char *data = reinterpret_cast<const char*>(xmlChars);
            if (!data)
                return;

            std::string csv(data);

            size_t pos = 0;
            size_t oldPos = 0;

            while (oldPos != csv.npos)
            {
                pos = csv.find_first_of(",", oldPos);
                if (pos == csv.npos)
                    return;

                const int gid = atoi(csv.substr(oldPos, pos - oldPos).c_str());

                setTile(map, layer, x, y, gid);

                x++;
                if (x == w)
                {
                    x = 0; y++;

                    // When we're done, don't crash on too much data
                    if (y == h)
                        break;
                }

                oldPos = pos + 1;
            }
            xmlFree(xmlChars);
        }
        else
        {
            // Read plain XML map file
            for_each_xml_child_node(childNode2, childNode)
            {
                if (!xmlNameEqual(childNode2, "tile"))
                    continue;

                const int gid = XML::getProperty(childNode2, "gid", -1);
                setTile(map, layer, x, y, gid);

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

Tileset *MapReader::readTileset(XmlNodePtr node, const std::string &path,
                                Map *map)
{
    if (!map)
        return nullptr;

    int firstGid = XML::getProperty(node, "firstgid", 0);
    int margin = XML::getProperty(node, "margin", 0);
    int spacing = XML::getProperty(node, "spacing", 0);
    XML::Document* doc = nullptr;
    Tileset *set = nullptr;
    std::string pathDir(path);

    if (xmlHasProp(node, BAD_CAST "source"))
    {
        std::string filename = XML::getProperty(node, "source", "");
        filename = resolveRelativePath(path, filename);

        doc = new XML::Document(filename);
        node = doc->rootNode();
        if (!node)
            return nullptr;

        // Reset path to be realtive to the tsx file
        pathDir = filename.substr(0, filename.rfind("/") + 1);
    }

    const int tw = XML::getProperty(node, "tilewidth", map->getTileWidth());
    const int th = XML::getProperty(node, "tileheight", map->getTileHeight());

    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "image"))
        {
            const std::string source = XML::getProperty(
                childNode, "source", "");

            if (!source.empty())
            {
                std::string sourceStr = resolveRelativePath(pathDir, source);

                ResourceManager *resman = ResourceManager::getInstance();
                Image* tilebmp = resman->getImage(sourceStr);

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
        else if (xmlNameEqual(childNode, "tile"))
        {
            for_each_xml_child_node(tileNode, childNode)
            {
                if (!xmlNameEqual(tileNode, "properties"))
                    continue;

                int tileGID = firstGid + XML::getProperty(childNode, "id", 0);

                // read tile properties to a map for simpler handling
                std::map<std::string, int> tileProperties;
                for_each_xml_child_node(propertyNode, tileNode)
                {
                    if (!xmlNameEqual(propertyNode, "property"))
                        continue;
                    std::string name = XML::getProperty(
                        propertyNode, "name", "");
                    int value = XML::getProperty(propertyNode, "value", 0);
                    tileProperties[name] = value;
                    logger->log("Tile Prop of %d \"%s\" = \"%d\"",
                                tileGID, name.c_str(), value);
                }

                // create animation
                if (!set)
                    continue;

                Animation *ani = new Animation;
                for (int i = 0; ; i++)
                {
                    std::map<std::string, int>::const_iterator iFrame, iDelay;
                    iFrame = tileProperties.find(
                        "animation-frame" + toString(i));
                    iDelay = tileProperties.find(
                        "animation-delay" + toString(i));
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
//                    logger->log("Animation length: %d", ani->getLength());
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

    return set;
}
