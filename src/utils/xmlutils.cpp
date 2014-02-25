/*
 *  The ManaPlus Client
 *  Copyright (C) 2014  The ManaPlus Developers
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

#include "utils/xmlutils.h"

#include "logger.h"

#include "utils/xml.h"

#include "debug.h"

void readXmlIntVector(const std::string &fileName,
                      const std::string &rootName,
                      const std::string &sectionName,
                      const std::string &itemName,
                      const std::string &attributeName,
                      std::vector<int> &arr)
{
    arr.clear();
    XML::Document doc(fileName);
    const XmlNodePtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, rootName.c_str()))
    {
        logger->log("Error while loading %s!", fileName.c_str());
        return;
    }

    for_each_xml_child_node(sectionNode, rootNode)
    {
        if (!xmlNameEqual(sectionNode, sectionName.c_str()))
            continue;
        for_each_xml_child_node(childNode, sectionNode)
        {
            if (!xmlNameEqual(childNode, itemName.c_str()))
                continue;

            const int val = XML::getProperty(childNode,
                attributeName.c_str(), -1);
            if (val == -1)
                continue;

            arr.push_back(val);
        }
    }
}
