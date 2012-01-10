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

#include "utils/xml.h"

#include "logger.h"

#include "resources/resourcemanager.h"

#include <iostream>
#include <fstream>
#include <cstring>

#include "debug.h"

static void xmlNullLogger(void *ctx A_UNUSED, const char *msg A_UNUSED, ...)
{
    // Does nothing, that's the whole point of it
}

namespace XML
{
    Document::Document(const std::string &filename, bool useResman):
        mDoc(nullptr)
    {
        int size;
        char *data = nullptr;
        if (useResman)
        {
            ResourceManager *resman = ResourceManager::getInstance();
            data = static_cast<char*>(resman->loadFile(
                filename.c_str(), size));
        }
        else
        {
            std::ifstream file;
            file.open(filename.c_str(), std::ios::in);

            if (file.is_open())
            {
                // Get length of file
                file.seekg (0, std::ios::end);
                size = static_cast<int>(file.tellg());
                file.seekg(0, std::ios::beg);

                data = static_cast<char*>(malloc(size));

                file.read(data, size);
                file.close();
            }
            else
            {
                logger->log("Error loading XML file %s", filename.c_str());
            }
        }

        if (data)
        {
            mDoc = xmlParseMemory(data, size);
            free(data);

            if (!mDoc)
                logger->log("Error parsing XML file %s", filename.c_str());
        }
        else
        {
            logger->log("Error loading %s", filename.c_str());
        }
    }

    Document::Document(const char *data, int size)
    {
        if (data)
            mDoc = xmlParseMemory(data, size);
        else
            mDoc = nullptr;
    }

    Document::~Document()
    {
        if (mDoc)
            xmlFreeDoc(mDoc);
    }

    XmlNodePtr Document::rootNode()
    {
        return mDoc ? xmlDocGetRootElement(mDoc) : nullptr;
    }

    int getProperty(XmlNodePtr node, const char* name, int def)
    {
        int &ret = def;

        xmlChar *prop = xmlGetProp(node, BAD_CAST name);
        if (prop)
        {
            ret = atoi(reinterpret_cast<char*>(prop));
            xmlFree(prop);
        }

        return ret;
    }

    double getFloatProperty(XmlNodePtr node, const char* name, double def)
    {
        double &ret = def;

        xmlChar *prop = xmlGetProp(node, BAD_CAST name);
        if (prop)
        {
            ret = atof(reinterpret_cast<char*>(prop));
            xmlFree(prop);
        }

        return ret;
    }

    std::string getProperty(XmlNodePtr node, const char *name,
                            const std::string &def)
    {
        xmlChar *prop = xmlGetProp(node, BAD_CAST name);
        if (prop)
        {
            std::string val = reinterpret_cast<char*>(prop);
            xmlFree(prop);
            return val;
        }

        return def;
    }

    bool getBoolProperty(XmlNodePtr node, const char* name, bool def)
    {
        xmlChar *prop = xmlGetProp(node, BAD_CAST name);

        if (xmlStrEqual(prop, BAD_CAST "true" ))
            return true;
        if (xmlStrEqual(prop, BAD_CAST "false"))
            return false;
        return def;
    }

    XmlNodePtr findFirstChildByName(XmlNodePtr parent, const char *name)
    {
        for_each_xml_child_node(child, parent)
        {
            if (xmlStrEqual(child->name, BAD_CAST name))
                return child;
        }

        return nullptr;
    }

    // Initialize libxml2 and check for potential ABI mismatches between
    // compiled version and the shared library actually used.
    void initXML()
    {
        xmlInitParser();
        LIBXML_TEST_VERSION;

        // Suppress libxml2 error messages
        xmlSetGenericErrorFunc(nullptr, xmlNullLogger);
    }

    // Shutdown libxml
    void cleanupXML()
    {
        xmlCleanupParser();
    }

} // namespace XML
