/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef RESOURCES_MAPREADER_H
#define RESOURCES_MAPREADER_H

#include "enums/resources/map/maplayertype.h"

#include "utils/xml.h"

class Map;
class MapHeights;
class MapLayer;
class Properties;
class Resource;
class Tileset;

/**
 * Reader for XML map files (*.tmx)
 */
class MapReader final
{
    public:
        A_DELETE_COPY(MapReader)

        /**
         * Read an XML map from a file.
         */
        static Map *readMap(const std::string &restrict filename,
                            const std::string &restrict realFilename)
                            A_WARN_UNUSED;

        /**
         * Read an XML map from a parsed XML tree. The path is used to find the
         * location of referenced tileset images.
         */
        static Map *readMap(XmlNodePtrConst node,
                            const std::string &path) A_WARN_UNUSED;

        static Map *createEmptyMap(const std::string &restrict filename,
                                   const std::string &restrict realFilename)
                                   A_WARN_UNUSED;

        /**
         * Reads a map layer and adds it to the given map.
         */
        static void readLayer(XmlNodeConstPtr node,
                              Map *const map) A_NONNULL(2);

#ifdef USE_OPENGL
        static void loadEmptyAtlas();
        static void unloadEmptyAtlas();
#endif  // USE_OPENGL

    private:
        /**
         * Reads the properties element.
         *
         * @param node  The <code>properties</code> element.
         * @param props The Properties instance to which the properties will
         *              be assigned.
         */
        static void readProperties(XmlNodeConstPtrConst node,
                                   Properties *const props) A_NONNULL(2);

        static bool readBase64Layer(XmlNodeConstPtrConst childNode,
                                    Map *const map,
                                    MapLayer *const layer,
                                    const MapLayerTypeT &layerType,
                                    MapHeights *const heights,
                                    const std::string &compression,
                                    int &restrict x, int &restrict y,
                                    const int w, const int h) A_NONNULL(2);

        static bool readCsvLayer(XmlNodeConstPtrConst childNode,
                                 Map *const map,
                                 MapLayer *const layer,
                                 const MapLayerTypeT &layerType,
                                 MapHeights *const heights,
                                 int &restrict x, int &restrict y,
                                 const int w, const int h) A_NONNULL(2);

        /**
         * Reads a tile set.
         */
        static Tileset *readTileset(XmlNodePtr node,
                                    const std::string &path,
                                    Map *const map) A_WARN_UNUSED A_NONNULL(3);

        static void updateMusic(Map *const map) A_NONNULL(1);

        static void addLayerToList(const std::string &fileName,
                                   const SkipError skipError);

        static void loadLayers(const std::string &path);

        static void unloadTempLayers();

#ifdef USE_OPENGL
        static Resource *mEmptyAtlas;
#endif  // USE_OPENGL
};

#endif  // RESOURCES_MAPREADER_H
