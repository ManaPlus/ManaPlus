/*
 *  The ManaPlus Client
 *  Copyright (C) 2014-2017  The ManaPlus Developers
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
                      std::vector<int> &arr,
                      const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
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
            if (xmlNameEqual(childNode, itemName.c_str()))
            {
                const int val = XML::getProperty(childNode,
                    attributeName.c_str(), -1);
                if (val == -1)
                    continue;
                arr.push_back(val);
            }
            else if (xmlNameEqual(childNode, "include"))
            {
                const std::string name = XML::getProperty(
                    childNode, "name", "");
                if (!name.empty())
                {
                    readXmlIntVector(name,
                        rootName,
                        sectionName,
                        itemName,
                        attributeName,
                        arr,
                        skipError);
                }
            }
        }
    }
}

void readXmlStringMap(const std::string &fileName,
                      const std::string &rootName,
                      const std::string &sectionName,
                      const std::string &itemName,
                      const std::string &attributeKeyName,
                      const std::string &attributeValueName,
                      std::map<std::string, std::string> &arr,
                      const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
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
            if (xmlNameEqual(childNode, itemName.c_str()))
            {
                const std::string key = XML::getProperty(childNode,
                    attributeKeyName.c_str(), "");
                if (key.empty())
                    continue;
                const std::string val = XML::getProperty(childNode,
                    attributeValueName.c_str(), "");
                arr[key] = val;
            }
            else if (xmlNameEqual(childNode, "include"))
            {
                const std::string name = XML::getProperty(
                    childNode, "name", "");
                if (!name.empty())
                {
                    readXmlStringMap(name,
                        rootName,
                        sectionName,
                        itemName,
                        attributeKeyName,
                        attributeValueName,
                        arr,
                        skipError);
                }
            }
        }
    }
}

void readXmlIntMap(const std::string &fileName,
                   const std::string &rootName,
                   const std::string &sectionName,
                   const std::string &itemName,
                   const std::string &attributeKeyName,
                   const std::string &attributeValueName,
                   std::map<int32_t, int32_t> &arr,
                   const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
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
            if (xmlNameEqual(childNode, itemName.c_str()))
            {
                const std::string key = XML::getProperty(childNode,
                    attributeKeyName.c_str(), "");
                if (key.empty())
                    continue;
                const int32_t val = XML::getProperty(childNode,
                    attributeValueName.c_str(), 0);
                arr[atoi(key.c_str())] = val;
            }
            else if (xmlNameEqual(childNode, "include"))
            {
                const std::string name = XML::getProperty(
                    childNode, "name", "");
                if (!name.empty())
                {
                    readXmlIntMap(name,
                        rootName,
                        sectionName,
                        itemName,
                        attributeKeyName,
                        attributeValueName,
                        arr,
                        skipError);
                }
            }
        }
    }
}
