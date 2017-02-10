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

#ifdef ENABLE_LIBXML

#include "utils/xml/libxml.h"

#include "utils/checkutils.h"
#include "utils/fuzzer.h"
#include "utils/virtfs.h"
#include "utils/stringutils.h"

#include "utils/translation/podict.h"

#include "debug.h"

namespace
{
    bool valid = false;
}  // namespace

static void xmlErrorLogger(void *ctx A_UNUSED, const char *msg A_UNUSED, ...)
#ifdef __GNUC__
#ifdef __OpenBSD__
    __attribute__((__format__(printf, 2, 3)))
#else  // __OpenBSD__
    __attribute__((__format__(gnu_printf, 2, 3)))
#endif  // __OpenBSD__
#endif  // __GNUC__
;

static void xmlErrorLogger(void *ctx A_UNUSED, const char *msg, ...)
{
    if (!msg)
        return;

    size_t size = 1024;
    const size_t msgSize = strlen(msg);
    if (msgSize * 3 > size)
        size = msgSize * 3;

    char* buf = new char[size + 1];
    va_list ap;

    // Use a temporary buffer to fill in the variables
    va_start(ap, msg);
    vsnprintf(buf, size, msg, ap);
    buf[size] = 0;
    va_end(ap);

    if (logger)
        logger->log_r("%s", buf);
    else
        puts(buf);

    // Delete temporary buffer
    delete [] buf;
    valid = false;
}

namespace XML
{
    Document::Document(const std::string &filename,
                       const UseResman useResman,
                       const SkipError skipError) :
        Resource(),
        mDoc(nullptr),
        mIsValid(false)
    {
#ifdef USE_FUZZER
        if (Fuzzer::conditionTerminate(filename.c_str()))
            return;
#endif  // USE_FUZZER

        BLOCK_START("XML::Document::Document")
        int size = 0;
        char *data = nullptr;
        valid = true;
        if (useResman == UseResman_true)
        {
            data = static_cast<char*>(VirtFs::loadFile(
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
                size = CAST_S32(file.tellg());
                if (size < 0)
                {
                    reportAlways("Error loading XML file %s",
                        filename.c_str());
                }
                else
                {
                    file.seekg(0, std::ios::beg);
                    data = static_cast<char*>(malloc(size));
                    file.read(data, size);
                }
                file.close();
            }
            else
            {
                reportAlways("Error loading XML file %s", filename.c_str());
            }
        }

        if (data)
        {
            mDoc = xmlParseMemory(data, size);
            free(data);

            if (!mDoc)
            {
                reportAlways("Error parsing XML file %s", filename.c_str());
            }
        }
        else if (skipError == SkipError_false)
        {
            reportAlways("Error loading XML file %s", filename.c_str());
        }
        mIsValid = valid;
        BLOCK_END("XML::Document::Document")
    }

    Document::Document(const char *const data, const int size) :
        mDoc(data ? xmlParseMemory(data, size) : nullptr),
        mIsValid(true)
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

    int getProperty(const XmlNodePtr node,
                    const char *const name,
                    int def)
    {
        int &ret = def;

        xmlChar *const prop = XmlGetProp(node, name);
        if (prop)
        {
            ret = atoi(reinterpret_cast<const char*>(prop));
            xmlFree(prop);
        }

        return ret;
    }

    int getIntProperty(const XmlNodePtr node,
                       const char *const name,
                       int def,
                       const int min,
                       const int max)
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

    double getFloatProperty(const XmlNodePtr node,
                            const char *const name,
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

    std::string getProperty(const XmlNodePtr node,
                            const char *const name,
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

    std::string langProperty(const XmlNodePtr node,
                             const char *const name,
                             const std::string &def)
    {
        std::string str = getProperty(node, name, def);
        if (!translator)
            return str;

        return translator->getStr(str);
    }

    bool getBoolProperty(const XmlNodePtr node,
                         const char *const name,
                         const bool def)
    {
        xmlChar *const prop = XmlGetProp(node, name);

        if (XmlStrEqual(prop, "true"))
            return true;
        if (XmlStrEqual(prop, "false"))
            return false;
        return def;
    }

    XmlNodePtr findFirstChildByName(const XmlNodePtrConst parent,
                                    const char *const name)
    {
        if (!parent)
            return nullptr;
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

    bool Document::validateXml(const std::string &fileName)
    {
        const xmlDocPtr doc = xmlReadFile(fileName.c_str(),
            nullptr, XML_PARSE_PEDANTIC);
        const bool valid1(doc);
        xmlFreeDoc(doc);
        if (!valid1)
            return false;

        std::ifstream file;
        file.open(fileName.c_str(), std::ios::in);
        if (!file.is_open())
        {
            file.close();
            return false;
        }
        char line[101];
        if (!file.getline(line, 100))
            return false;
        file.close();

        const std::string str = line;
        if (!strStartWith(str, "<?xml "))
            return false;

        return true;
    }
}  // namespace XML

#endif  // ENABLE_LIBXML
