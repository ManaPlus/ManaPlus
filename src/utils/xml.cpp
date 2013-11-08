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

#include "utils/xml.h"

#include "logger.h"

#include "resources/resourcemanager.h"

#include "utils/fuzzer.h"

#include "utils/translation/podict.h"

#include <fstream>

#include "debug.h"

static void xmlErrorLogger(void *ctx A_UNUSED, const char *msg A_UNUSED, ...)
#ifdef __GNUC__
#ifdef __OpenBSD__
    __attribute__((__format__(printf, 2, 3)))
#else
    __attribute__((__format__(gnu_printf, 2, 3)))
#endif
#endif
;

static void xmlErrorLogger(void *ctx A_UNUSED, const char *msg, ...)
{
    unsigned size = 1024;
    const unsigned msgSize = strlen(msg);
    if (msgSize * 3 > size)
        size = static_cast<unsigned>(msgSize * 3);

    char* buf = new char[size + 1];
    va_list ap;

    // Use a temporary buffer to fill in the variables
    va_start(ap, msg);
    vsnprintf(buf, size, msg, ap);
    buf[size] = 0;
    va_end(ap);

    if (logger)
        logger->log1(buf);
    else
        puts(buf);

    // Delete temporary buffer
    delete [] buf;
}

namespace XML
{
    Document::Document(const std::string &filename, const bool useResman) :
        mDoc(nullptr)
    {
#ifdef USE_FUZZER
        if (Fuzzer::conditionTerminate(filename.c_str()))
            return;
#endif
        int size = 0;
        char *data = nullptr;
        if (useResman)
        {
            const ResourceManager *const resman
                = ResourceManager::getInstance();
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
                file.seekg(0, std::ios::end);
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

    Document::Document(const char *const data, const int size) :
        mDoc(data ? xmlParseMemory(data, size) : nullptr)
    {
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

    int getProperty(const XmlNodePtr node, const char *const name, int def)
    {
        int &ret = def;

        xmlChar *const prop = XmlGetProp(node, name);
        if (prop)
        {
            ret = atoi(reinterpret_cast<char*>(prop));
            xmlFree(prop);
        }

        return ret;
    }

    int getIntProperty(const XmlNodePtr node, const char *const name, int def,
                       const int min, const int max)
    {
        int &ret = def;

        xmlChar *const prop = XmlGetProp(node, name);
        if (prop)
        {
            ret = atoi(reinterpret_cast<char*>(prop));
            xmlFree(prop);
        }
        if (ret < min)
            ret = min;
        else if (ret > max)
            ret = max;
        return ret;
    }

    double getFloatProperty(const XmlNodePtr node, const char *const name,
                            double def)
    {
        double &ret = def;

        xmlChar *const prop = XmlGetProp(node, name);
        if (prop)
        {
            ret = atof(reinterpret_cast<char*>(prop));
            xmlFree(prop);
        }

        return ret;
    }

    std::string getProperty(const XmlNodePtr node, const char *const name,
                            const std::string &def)
    {
        xmlChar *const prop = XmlGetProp(node, name);
        if (prop)
        {
            std::string val = reinterpret_cast<char*>(prop);
            xmlFree(prop);
            return val;
        }

        return def;
    }

    std::string langProperty(const XmlNodePtr node, const char *const name,
                             const std::string &def)
    {
        std::string str = getProperty(node, name, def);
        if (!translator)
            return str;

        return translator->getStr(str);
    }

    bool getBoolProperty(const XmlNodePtr node, const char *const name,
                         const bool def)
    {
        const xmlChar *const prop = XmlGetProp(node, name);

        if (XmlStrEqual(prop, "true"))
            return true;
        if (XmlStrEqual(prop, "false"))
            return false;
        return def;
    }

    XmlNodePtr findFirstChildByName(const XmlNodePtr parent,
                                    const char *const name)
    {
        for_each_xml_child_node(child, parent)
        {
            if (xmlNameEqual(child, name))
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
        xmlSetGenericErrorFunc(nullptr, &xmlErrorLogger);
    }

    // Shutdown libxml
    void cleanupXML()
    {
        xmlCleanupParser();
    }

}  // namespace XML
