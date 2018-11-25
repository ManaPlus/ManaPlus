/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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
#ifdef USE_OPENGL
#include "graphicsmanager.h"
#endif  // USE_OPENGL
#include "main.h"

#include "const/resources/map/map.h"

#include "enums/resources/map/collisiontype.h"
#include "enums/resources/map/mapitemtype.h"

#include "fs/virtfs/fs.h"

#include "resources/map/map.h"
#include "resources/map/mapheights.h"
#include "resources/map/maplayer.h"
#include "resources/map/tileset.h"

#include "resources/beingcommon.h"
#include "resources/animation/animation.h"

#include "resources/image/image.h"

#ifdef USE_OPENGL
#include "resources/db/mapdb.h"
#include "resources/loaders/atlasloader.h"
#include "resources/loaders/emptyatlasloader.h"
#endif  // USE_OPENGL

#include "resources/map/tileanimation.h"

#include "resources/loaders/imageloader.h"

#include "resources/loaders/walklayerloader.h"

#include "utils/base64.h"
#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/stringmap.h"

#include "utils/translation/podict.h"

#include <zlib.h>

#include "debug.h"

typedef std::map<std::string, XmlNodePtr>::iterator LayerInfoIterator;
typedef std::set<XML::Document*>::iterator DocIterator;

#ifdef USE_OPENGL
Resource *MapReader::mEmptyAtlas = nullptr;
#endif  // USE_OPENGL

namespace
{
    std::map<std::string, XmlNodePtr> mKnownLayers;
    std::set<XML::Document*> mKnownDocs;
}  // namespace

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
    strm.zalloc = nullptr;
    strm.zfree = nullptr;
    strm.opaque = nullptr;
    strm.next_in = in;
    strm.avail_in = inLength;
    strm.next_out = out;
    strm.avail_out = bufferSize;

PRAGMACLANG6GCC(GCC diagnostic push)
PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
    int ret = inflateInit2(&strm, 15 + 32);
PRAGMACLANG6GCC(GCC diagnostic pop)

    if (ret != Z_OK)
        return ret;

    do
    {
        if (strm.next_out == nullptr)
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
                A_FALLTHROUGH
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

            if (out == nullptr)
            {
                inflateEnd(&strm);
                return Z_MEM_ERROR;
            }

            strm.next_out = out + CAST_SIZE(bufferSize);
            strm.avail_out = bufferSize;
            bufferSize *= 2;
        }
    }
    while (ret != Z_STREAM_END);

    outLength = bufferSize - strm.avail_out;
    (void) inflateEnd(&strm);
    return Z_OK;
}

int inflateMemory(unsigned char *restrict const in,
                  const unsigned int inLength,
                  unsigned char *&restrict out)
{
    unsigned int outLength = 0;
    const int ret = inflateMemory(in, inLength, out, outLength);

    if (ret != Z_OK || (out == nullptr))
    {
        if (ret == Z_MEM_ERROR)
        {
            reportAlways("Error: Out of memory while decompressing map data!")
        }
        else if (ret == Z_VERSION_ERROR)
        {
            reportAlways("Error: Incompatible zlib version!")
        }
        else if (ret == Z_DATA_ERROR)
        {
            reportAlways("Error: Incorrect zlib compressed data!")
        }
        else
        {
            reportAlways("Error: Unknown error while decompressing map data!")
        }

        free(out);
        out = nullptr;
        outLength = 0;
    }

    return outLength;
}

void MapReader::addLayerToList(const std::string &fileName,
                               const SkipError skipError)
{
    XML::Document *doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodePtrConst node = doc->rootNode();
    if (node == nullptr)
    {
        delete doc;
        return;
    }

    int cnt = 0;
    for_each_xml_child_node(childNode, node)
    {
        if (!xmlNameEqual(childNode, "layer"))
            continue;
        std::string name = XML::getProperty(childNode, "name", "");
        if (name.empty())
            continue;
        name = toLower(name);
        logger->log("found patch layer: " + name);
        mKnownLayers[name] = childNode;
        mKnownDocs.insert(doc);
        cnt ++;
    }
    if (cnt == 0)
        delete doc;
}

Map *MapReader::readMap(const std::string &restrict filename,
                        const std::string &restrict realFilename)
{
    BLOCK_START("MapReader::readMap str")
    logger->log("Attempting to read map %s", realFilename.c_str());

    XML::Document doc(realFilename, UseVirtFs_true, SkipError_false);
    if (!doc.isLoaded())
    {
        BLOCK_END("MapReader::readMap str")
        return createEmptyMap(filename, realFilename);
    }

    XmlNodePtrConst node = doc.rootNode();

    Map *map = nullptr;
    // Parse the inflated map data
    if (node != nullptr)
    {
        if (!xmlNameEqual(node, "map"))
            logger->log("Error: Not a map file (%s)!", realFilename.c_str());
        else
            map = readMap(node, realFilename);
    }
    else
    {
        reportAlways("Error while parsing map file (%s)!",
            realFilename.c_str())
    }

    if (map != nullptr)
    {
        map->setProperty("_filename", realFilename);
        map->setProperty("_realfilename", filename);

        if (map->getProperty("music", std::string()).empty())
            updateMusic(map);

        map->updateConditionLayers();
        map->preCacheLayers();
    }

    BLOCK_END("MapReader::readMap str")
    return map;
}

void MapReader::loadLayers(const std::string &path)
{
    BLOCK_START("MapReader::loadLayers")
    loadXmlDir2(path, addLayerToList, ".tmx", SkipError_false)
    BLOCK_END("MapReader::loadLayers")
}

void MapReader::unloadTempLayers()
{
    FOR_EACH (DocIterator, it, mKnownDocs)
        delete (*it);
    mKnownLayers.clear();
    mKnownDocs.clear();
}

static void loadReplaceLayer(const LayerInfoIterator &it,
                             Map *const map) A_NONNULL(2);
static void loadReplaceLayer(const LayerInfoIterator &it,
                             Map *const map)
{
    MapReader::readLayer((*it).second, map);
}

Map *MapReader::readMap(XmlNodePtrConst node, const std::string &path)
{
    if (node == nullptr)
        return nullptr;

    BLOCK_START("MapReader::readMap xml")
    // Take the filename off the path
    const std::string pathDir = path.substr(0, path.rfind('/') + 1);

    const int w = XML::getProperty(node, "width", 0);
    const int h = XML::getProperty(node, "height", 0);
    const int tilew = XML::getProperty(node, "tilewidth", -1);
    const int tileh = XML::getProperty(node, "tileheight", -1);

    const bool showWarps = config.getBoolValue("warpParticle");
    const std::string warpPath = pathJoin(paths.getStringValue("particles"),
        paths.getStringValue("portalEffectFile"));

    if (tilew < 0 || tileh < 0)
    {
        reportAlways("MapReader: Warning: "
            "Uninitialized tile width or height value for map: %s",
            path.c_str())
        BLOCK_END("MapReader::readMap xml")
        return nullptr;
    }

    logger->log("loading replace layer list");
    loadLayers(path + "_replace.d");

    Map *const map = new Map(path,
        w, h,
        tilew, tileh);

    const std::string fileName = path.substr(path.rfind(dirSeparator) + 1);
    map->setProperty("shortName", fileName);

#ifdef USE_OPENGL
    BLOCK_START("MapReader::readMap load atlas")
    if (graphicsManager.getUseAtlases())
    {
        const MapInfo *const info = MapDB::getMapAtlas(fileName);
        if (info != nullptr)
        {
            map->setAtlas(Loader::getAtlas(
                info->atlas,
                *info->files));
        }
        else
        {
            reportAlways("Missing atlas for map: %s",
                fileName.c_str())
        }
    }
    BLOCK_END("MapReader::readMap load atlas")
#endif  // USE_OPENGL

    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "tileset"))
        {
            Tileset *const tileset = readTileset(childNode, pathDir, map);
            if (tileset != nullptr)
                map->addTileset(tileset);
        }
        else if (xmlNameEqual(childNode, "layer"))
        {
            std::string name = XML::getProperty(childNode, "name", "");
            name = toLower(name);
            LayerInfoIterator it = mKnownLayers.find(name);
            if (it == mKnownLayers.end())
            {
                readLayer(childNode, map);
            }
            else
            {
                logger->log("load replace layer: " + name);
                loadReplaceLayer(it, map);
            }
        }
        else if (xmlNameEqual(childNode, "properties"))
        {
            readProperties(childNode, map);
            map->setVersion(atoi(map->getProperty(
                "manaplus version", std::string()).c_str()));
        }
        else if (xmlNameEqual(childNode, "objectgroup"))
        {
            // The object group offset is applied to each object individually
            const int tileOffsetX = XML::getProperty(childNode, "x", 0);
            const int tileOffsetY = XML::getProperty(childNode, "y", 0);
            const int offsetX = tileOffsetX * tilew;
            const int offsetY = tileOffsetY * tileh;
            const bool showParticles =
                config.getBoolValue("mapparticleeffects");

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

                        if (showParticles)
                        {
                            map->addParticleEffect(objName,
                                objX + offsetX,
                                objY + offsetY,
                                objW,
                                objH);
                        }
                        else
                        {
                            logger->log("Ignore particle effect: " + objName);
                        }
                    }
                    else if (objType == "WARP")
                    {
                        if (showWarps)
                        {
                            map->addParticleEffect(warpPath,
                                objX, objY, objW, objH);
                        }
                        map->addPortal(objName, MapItemType::PORTAL,
                                       objX, objY, objW, objH);
                    }
                    else if (objType == "SPAWN")
                    {
                        // TRANSLATORS: spawn name
//                      map->addPortal(_("Spawn: ") + objName,
//                            MapItemType::PORTAL,
//                            objX, objY, objW, objH);
                    }
                    else if (objType == "MUSIC")
                    {
                        map->addRange(objName, MapItemType::MUSIC,
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
    map->setActorsFix(0,
        atoi(map->getProperty("actorsfix", std::string()).c_str()));
    map->reduce();
    map->setWalkLayer(Loader::getWalkLayer(fileName, map));
    unloadTempLayers();
    map->updateDrawLayersList();
    BLOCK_END("MapReader::readMap xml")
    return map;
}

void MapReader::readProperties(XmlNodeConstPtrConst node,
                               Properties *const props)
{
    BLOCK_START("MapReader::readProperties")
    if (node == nullptr)
    {
        BLOCK_END("MapReader::readProperties")
        return;
    }

    for_each_xml_child_node(childNode, node)
    {
        if (!xmlNameEqual(childNode, "property"))
            continue;

        // Example: <property name="name" value="value"/>
        const std::string name = XML::getProperty(childNode, "name", "");
        const std::string value = XML::getProperty(childNode, "value", "");

        if (!name.empty() && !value.empty())
        {
            if (name == "name")
                props->setProperty(name, translator->getStr(value));
            else
                props->setProperty(name, value);
        }
    }
    BLOCK_END("MapReader::readProperties")
}

inline static void setTile(Map *const map,
                           MapLayer *const layer,
                           const MapLayerTypeT &layerType,
                           MapHeights *const heights,
                           const int x, const int y,
                           const int gid) A_NONNULL(1);

inline static void setTile(Map *const map,
                           MapLayer *const layer,
                           const MapLayerTypeT &layerType,
                           MapHeights *const heights,
                           const int x, const int y,
                           const int gid)
{
    const Tileset * const set = map->getTilesetWithGid(gid);
    switch (layerType)
    {
        case MapLayerType::TILES:
        {
            if (layer == nullptr)
                break;
            if (set != nullptr &&
                !set->isEmpty())
            {
                Image *const img = set->get(gid - set->getFirstGid());
                layer->setTile(x, y, img);
            }
            else
            {
                layer->setTile(x, y, nullptr);
            }
            break;
        }

        case MapLayerType::COLLISION:
        {
            if (set != nullptr)
            {
                if (map->getVersion() >= 1)
                {
                    const int collisionId = gid - set->getFirstGid();
                    CollisionTypeT type;
                    if (collisionId < 0 ||
                        collisionId >= CAST_S32(CollisionType::COLLISION_MAX))
                    {
                        type = CollisionType::COLLISION_EMPTY;
                    }
                    else
                    {
                        type = static_cast<CollisionTypeT>(collisionId);
                    }
                    switch (type)
                    {
                        case CollisionType::COLLISION_EMPTY:
                            map->addBlockMask(x, y, BlockType::GROUND);
                            break;
                        case CollisionType::COLLISION_WALL:
                            map->addBlockMask(x, y, BlockType::WALL);
                            break;
                        case CollisionType::COLLISION_AIR:
                            map->addBlockMask(x, y, BlockType::AIR);
                            break;
                        case CollisionType::COLLISION_WATER:
                            map->addBlockMask(x, y, BlockType::WATER);
                            break;
                        case CollisionType::COLLISION_GROUNDTOP:
                            map->addBlockMask(x, y, BlockType::GROUNDTOP);
                            break;
                        case CollisionType::COLLISION_PLAYER_WALL:
                            map->addBlockMask(x, y, BlockType::PLAYERWALL);
                            break;
                        case CollisionType::COLLISION_MONSTER_WALL:
                            map->addBlockMask(x, y, BlockType::MONSTERWALL);
                            break;
                        case CollisionType::COLLISION_MAX:
                        default:
                            break;
                    }
                }
                else
                {
                    if (gid - set->getFirstGid() != 0)
                        map->addBlockMask(x, y, BlockType::WALL);
                }
            }
            break;
        }

        case MapLayerType::HEIGHTS:
        {
            if (set == nullptr || heights == nullptr)
                break;
            if (map->getVersion() >= 2)
            {
                heights->setHeight(x, y, CAST_U8(
                    gid - set->getFirstGid() + 1));
            }
            else
            {
                Image *const img = set->get(gid - set->getFirstGid());
                if (layer != nullptr)
                    layer->setTile(x, y, img);
            }
            break;
        }

        default:
        case MapLayerType::ACTIONS:
            break;
    }
}

bool MapReader::readBase64Layer(XmlNodeConstPtrConst childNode,
                                Map *const map,
                                MapLayer *const layer,
                                const MapLayerTypeT &layerType,
                                MapHeights *const heights,
                                const std::string &compression,
                                int &restrict x, int &restrict y,
                                const int w, const int h)
{
    if (childNode == nullptr)
        return false;

    if (!compression.empty() && compression != "gzip"
        && compression != "zlib")
    {
        reportAlways("Warning: only gzip and zlib layer"
            " compression supported!")
        return false;
    }

    // Read base64 encoded map file
    if (!XmlHaveChildContent(childNode))
        return true;

    const size_t len = strlen(XmlChildContent(childNode)) + 1;
    unsigned char *charData = new unsigned char[len + 1];
    const char *const xmlChars = XmlChildContent(childNode);
    const char *charStart = reinterpret_cast<const char*>(xmlChars);
    if (charStart == nullptr)
    {
        delete [] charData;
        return false;
    }

    unsigned char *charIndex = charData;

    while (*charStart != 0)
    {
        if (*charStart != ' ' &&
            *charStart != '\t' &&
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
        CAST_S32(strlen(reinterpret_cast<char*>(
        charData))), &binLen);

    delete [] charData;
//    XmlFree(const_cast<char*>(xmlChars));

    if (binData != nullptr)
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

            if (inflated == nullptr)
            {
                reportAlways("Error: Could not decompress layer!")
                return false;
            }
        }

        const std::map<int, TileAnimation*> &tileAnimations
            = map->getTileAnimations();

        const bool hasAnimations = !tileAnimations.empty();
        if (hasAnimations)
        {
            for (int i = 0; i < binLen - 3; i += 4)
            {
                const int gid = binData[i] |
                    binData[i + 1] << 8 |
                    binData[i + 2] << 16 |
                    binData[i + 3] << 24;

                setTile(map, layer, layerType, heights, x, y, gid);
                TileAnimationMapCIter it = tileAnimations.find(gid);
                if (it != tileAnimations.end())
                {
                    TileAnimation *const ani = it->second;
                    if (ani != nullptr)
                        ani->addAffectedTile(layer, x + y * w);
                }

                x++;
                if (x == w)
                {
                    x = 0; y++;

                    // When we're done, don't crash on too much data
                    if (y == h)
                        break;
                }
            }
        }
        else
        {
            for (int i = 0; i < binLen - 3; i += 4)
            {
                const int gid = binData[i] |
                    binData[i + 1] << 8 |
                    binData[i + 2] << 16 |
                    binData[i + 3] << 24;

                setTile(map, layer, layerType, heights, x, y, gid);

                x++;
                if (x == w)
                {
                    x = 0; y++;

                    // When we're done, don't crash on too much data
                    if (y == h)
                        break;
                }
            }
        }
        free(binData);
    }
    return true;
}

bool MapReader::readCsvLayer(XmlNodeConstPtrConst childNode,
                             Map *const map,
                             MapLayer *const layer,
                             const MapLayerTypeT &layerType,
                             MapHeights *const heights,
                             int &restrict x, int &restrict y,
                             const int w, const int h)
{
    if (childNode == nullptr)
        return false;

    if (!XmlHaveChildContent(childNode))
        return true;

    const char *const xmlChars = XmlChildContent(childNode);
    const char *const data = reinterpret_cast<const char*>(xmlChars);
    if (data == nullptr)
        return false;

    std::string csv(data);
    size_t oldPos = 0;

    const std::map<int, TileAnimation*> &tileAnimations
        = map->getTileAnimations();
    const bool hasAnimations = !tileAnimations.empty();

    if (hasAnimations)
    {
        while (oldPos != std::string::npos)
        {
            const size_t pos = csv.find_first_of(',', oldPos);
            if (pos == std::string::npos)
                return false;

            const int gid = atoi(csv.substr(oldPos, pos - oldPos).c_str());
            setTile(map, layer, layerType, heights, x, y, gid);
            TileAnimationMapCIter it = tileAnimations.find(gid);
            if (it != tileAnimations.end())
            {
                TileAnimation *const ani = it->second;
                if (ani != nullptr)
                    ani->addAffectedTile(layer, x + y * w);
            }

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
    }
    else
    {
        while (oldPos != std::string::npos)
        {
            const size_t pos = csv.find_first_of(',', oldPos);
            if (pos == std::string::npos)
                return false;

            const int gid = atoi(csv.substr(oldPos, pos - oldPos).c_str());
            setTile(map, layer, layerType, heights, x, y, gid);

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
    }
    return true;
}

void MapReader::readLayer(XmlNodeConstPtr node, Map *const map)
{
    if (node == nullptr)
        return;

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
    const bool isActionsLayer = (name.substr(0, 7) == "actions");
    int mask = 1;
    int tileCondition = -1;
    int conditionLayer = 0;

    MapLayerTypeT layerType = MapLayerType::TILES;
    if (isCollisionLayer)
        layerType = MapLayerType::COLLISION;
    else if (isHeightLayer)
        layerType = MapLayerType::HEIGHTS;
    else if (isActionsLayer)
        layerType = MapLayerType::ACTIONS;

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
                else if (pname == "TileCondition")
                {
                    tileCondition = atoi(value.c_str());
                }
                else if (pname == "ConditionLayer")
                {
                    conditionLayer = atoi(value.c_str());
                }
                else if (pname == "SideView")
                {
                    if (value != "down")
                        return;
                }
            }
        }

        if (!xmlNameEqual(childNode, "data"))
            continue;

        // Disable for future usage "TileCondition" attribute
        // if already set ConditionLayer to non zero
        if (conditionLayer != 0)
            tileCondition = -1;

        switch (layerType)
        {
            case MapLayerType::TILES:
            {
                layer = new MapLayer(name,
                    offsetX, offsetY,
                    w, h,
                    isFringeLayer,
                    mask,
                    tileCondition);
                map->addLayer(layer);
                break;
            }
            case MapLayerType::HEIGHTS:
            {
                heights = new MapHeights(w, h);
                map->addHeights(heights);
                break;
            }
            default:
            case MapLayerType::ACTIONS:
            case MapLayerType::COLLISION:
                break;
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
                setTile(map, layer, layerType, heights, x, y, gid);
                if (hasAnimations)
                {
                    TileAnimationMapCIter it = tileAnimations.find(gid);
                    if (it != tileAnimations.end())
                    {
                        TileAnimation *const ani = it->second;
                        if (ani != nullptr)
                            ani->addAffectedTile(layer, x + y * w);
                    }
                }

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
        if (x != 0)
            std::cerr << "TOO SMALL!\n";

        // There can be only one data element
        break;
    }
}

Tileset *MapReader::readTileset(XmlNodePtr node,
                                const std::string &path,
                                Map *const map)
{
    BLOCK_START("MapReader::readTileset")
    if (node == nullptr)
    {
        BLOCK_END("MapReader::readTileset")
        return nullptr;
    }

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

        doc = new XML::Document(filename, UseVirtFs_true, SkipError_false);
        node = doc->rootNode();
        if (node == nullptr)
        {
            delete doc;
            BLOCK_END("MapReader::readTileset")
            return nullptr;
        }

        // Reset path to be realtive to the tsx file
        pathDir = filename.substr(0, filename.rfind('/') + 1);
    }

    const int tw = XML::getProperty(node, "tilewidth", map->getTileWidth());
    const int th = XML::getProperty(node, "tileheight", map->getTileHeight());

    for_each_xml_child_node(childNode, node)
    {
        if (xmlNameEqual(childNode, "image"))
        {
            // ignore second other <image> tags in tileset
            if (set != nullptr)
                continue;

            const std::string source = XML::getProperty(
                childNode, "source", "");

            if (!source.empty())
            {
                const std::string sourceResolved = resolveRelativePath(pathDir,
                    source);

                Image *const tilebmp = Loader::getImage(sourceResolved);

                if (tilebmp != nullptr)
                {
                    set = new Tileset(tilebmp,
                        tw, th,
                        firstGid,
                        margin,
                        spacing);
                    tilebmp->decRef();
#ifdef USE_OPENGL
                    if (MapDB::isEmptyTileset(sourceResolved))
                        set->setEmpty(true);
                    if (tilebmp->getType() == ImageType::Image &&
                        map->haveAtlas() == true &&
                        graphicsManager.getUseAtlases())
                    {
                        reportAlways("Error: image '%s' not present in atlas",
                            source.c_str())
                    }
#endif  // USE_OPENGL
                }
                else
                {
                    reportAlways("Error: Failed to load tileset (%s)",
                        source.c_str())
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
            bool haveAnimation(false);

            for_each_xml_child_node(tileNode, childNode)
            {
                const bool isProps = xmlNameEqual(tileNode, "properties");
                const bool isAnim = xmlNameEqual(tileNode, "animation");
                if (!isProps && !isAnim)
                    continue;

                const int tileGID = firstGid + XML::getProperty(
                    childNode, "id", 0);

                Animation *ani = new Animation("from map");

                if (isProps)
                {
                    // read tile properties to a map for simpler handling
                    StringIntMap tileProperties;
                    for_each_xml_child_node(propertyNode, tileNode)
                    {
                        if (!xmlNameEqual(propertyNode, "property"))
                            continue;

                        haveAnimation = true;
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
                    if (set == nullptr ||
                        !config.getBoolValue("playMapAnimations"))
                    {
                        delete ani;
                        continue;
                    }

                    for (int i = 0; ; i++)
                    {
                        const std::string iStr(toString(i));
                        StringIntMapCIter iFrame
                            = tileProperties.find("animation-frame" + iStr);
                        StringIntMapCIter iDelay
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
                }
                else if (isAnim && !haveAnimation)
                {
                    for_each_xml_child_node(frameNode, tileNode)
                    {
                        if (!xmlNameEqual(frameNode, "frame"))
                            continue;

                        const int tileId = XML::getProperty(
                            frameNode, "tileid", 0);
                        const int duration = XML::getProperty(
                            frameNode, "duration", 0) /  10;

                        if (set != nullptr)
                        {
                            ani->addFrame(set->get(tileId),
                                duration,
                                0, 0, 100);
                        }
                    }
                }

                if (ani->getLength() > 0)
                    map->addAnimation(tileGID, new TileAnimation(ani));
                else
                    delete2(ani)
            }
        }
    }

    delete doc;

    if (set != nullptr)
        set->setProperties(props);
    BLOCK_END("MapReader::readTileset")
    return set;
}

Map *MapReader::createEmptyMap(const std::string &restrict filename,
                               const std::string &restrict realFilename)
{
    logger->log1("Creating empty map");
    Map *const map = new Map("empty map",
        300, 300,
        mapTileSize, mapTileSize);
    map->setProperty("_filename", realFilename);
    map->setProperty("_realfilename", filename);
    updateMusic(map);
    map->setCustom(true);
    MapLayer *layer = new MapLayer("nolayer",
        0, 0,
        300, 300,
        false,
        1,
        -1);
    map->addLayer(layer);
    layer = new MapLayer("nolayer",
        0, 0,
        300, 300,
        true,
        1,
        -1);
    map->addLayer(layer);
    map->updateDrawLayersList();
    map->updateConditionLayers();
    map->preCacheLayers();

    return map;
}

void MapReader::updateMusic(Map *const map)
{
    std::string name = map->getProperty("shortName", std::string());
    const size_t p = name.rfind('.');
    if (p != std::string::npos)
        name = name.substr(0, p);
    name.append(".ogg");
    if (VirtFs::exists(pathJoin(paths.getStringValue("music"), name)))
        map->setProperty("music", name);
}

#ifdef USE_OPENGL
void MapReader::loadEmptyAtlas()
{
    if (!graphicsManager.getUseAtlases())
        return;

    const MapInfo *const info = MapDB::getAtlas(
        paths.getStringValue("emptyAtlasName"));
    if (info != nullptr)
    {
        mEmptyAtlas = Loader::getEmptyAtlas(
            info->atlas,
            *info->files);
        delete info;
    }
}

void MapReader::unloadEmptyAtlas()
{
    if (mEmptyAtlas != nullptr)
        mEmptyAtlas->decRef();
}
#endif  // USE_OPENGL
