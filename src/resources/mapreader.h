/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "utils/xml.h"

#include "mapheights.h"
#include "maplayer.h"

#include <string>

class Map;
class MapLayer;
class Properties;
class Tileset;

/**
 * Reader for XML map files (*.tmx)
 */
class MapReader final
{
    public:
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
        static Map *readMap(XmlNodePtr node,
                            const std::string &path) A_WARN_UNUSED;

        static Map *createEmptyMap(const std::string &restrict filename,
                                   const std::string &restrict realFilename)
                                   A_WARN_UNUSED;

    private:
        /**
         * Reads the properties element.
         *
         * @param node  The <code>properties</code> element.
         * @param props The Properties instance to which the properties will
         *              be assigned.
         */
        static void readProperties(const XmlNodePtr node,
                                   Properties *const props);

        /**
         * Reads a map layer and adds it to the given map.
         */
        static void readLayer(const XmlNodePtr node, Map *const map);

        static bool readBase64Layer(const XmlNodePtr childNode, Map *const map,
                                    MapLayer *const layer,
                                    const MapLayer::Type &layerType,
                                    MapHeights *const heights,
                                    const std::string &compression,
                                    int &restrict x, int &restrict y,
                                    const int w, const int h);

        static bool readCsvLayer(const XmlNodePtr childNode, Map *const map,
                                 MapLayer *const layer,
                                 const MapLayer::Type &layerType,
                                 MapHeights *const heights,
                                 int &restrict x, int &restrict y,
                                 const int w, const int h);

        /**
         * Reads a tile set.
         */
        static Tileset *readTileset(XmlNodePtr node, const std::string &path,
                                    Map *const map) A_WARN_UNUSED;

        static void updateMusic(Map *const map);
};

#endif  // RESOURCES_MAPREADER_H
