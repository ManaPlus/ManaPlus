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

#ifdef ENABLE_PUGIXML

#include "utils/xml/pugixml.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/fuzzer.h"
#include "utils/virtfstools.h"
#include "utils/stringutils.h"

#include "utils/translation/podict.h"

#include "debug.h"

namespace
{
    bool valid = false;
}  // namespace

namespace XML
{
    static void showErrorStatus(pugi::xml_parse_result &result)
    {
/*
        switch (result.status)
        {
            case pugi::status_ok:
                break;
            case pugi::status_file_not_found:
                logger->log("xml error: %s", result.description());
                break;
        }
*/
        logger->log("xml error: %s", result.description());
    }

    Document::Document(const std::string &filename,
                       const UseResman useResman,
                       const SkipError skipError) :
        Resource(),
        mDoc(),
        mData(nullptr),
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
                reportAlways("Error loading XML file %s",
                    filename.c_str());
            }
        }

        if (data)
        {
            // +++ use other pugi::parse_* flags
            pugi::xml_parse_result result = mDoc.load_buffer_inplace(data,
                size,
                pugi::parse_default,
                pugi::encoding_utf8);
            if (result.status != pugi::status_ok)
            {
                showErrorStatus(result);
                free(data);
            }
            else
            {
                mData = data;
            }

//            if (!mDoc)
//                logger->log("Error parsing XML file %s", filename.c_str());
        }
        else if (skipError == SkipError_false)
        {
            reportAlways("Error loading %s", filename.c_str());
        }
        mIsValid = valid;
        BLOCK_END("XML::Document::Document")
    }

    Document::Document(const char *const data, const int size) :
        mDoc(),
        mData(nullptr),
        mIsValid(true)
    {
        if (!data)
            return;

        char *buf = static_cast<char*>(calloc(size + 1, 1));
        strncpy(buf, data, size);
        buf[size] = 0;
        pugi::xml_parse_result result = mDoc.load_buffer_inplace(buf,
            size,
            pugi::parse_default,
            pugi::encoding_utf8);
        if (result.status != pugi::status_ok)
        {
            showErrorStatus(result);
            free(buf);
        }
        else
        {
            mData = buf;
        }
    }

    Document::~Document()
    {
        free(mData);
        mData = nullptr;
//        if (mDoc)
//            xmlFreeDoc(mDoc);
    }

    XmlNodePtr Document::rootNode()
    {
        return mDoc.first_child();
    }

    int getProperty(const XmlNodePtr node,
                    const char *const name,
                    int def)
    {
        int &ret = def;

        if (!node)
            return ret;
        const pugi::xml_attribute &attr = node.attribute(name);
        if (!attr.empty())
            ret = atoi(attr.value());

        return ret;
    }

    int getIntProperty(const XmlNodePtr node,
                       const char *const name,
                       int def,
                       const int min,
                       const int max)
    {
        int &ret = def;

        if (!node)
            return ret;
        const pugi::xml_attribute &attr = node.attribute(name);
        if (!attr.empty())
            ret = atoi(attr.value());

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

        if (!node)
            return ret;
        const pugi::xml_attribute &attr = node.attribute(name);
        if (!attr.empty())
            ret = atof(attr.value());

        return ret;
    }

    std::string getProperty(const XmlNodePtr node,
                            const char *const name,
                            const std::string &def)
    {
        if (!node)
            return def;
        const pugi::xml_attribute &attr = node.attribute(name);
        if (!attr.empty())
            return attr.value();

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
        if (!node)
            return def;
        const pugi::xml_attribute &attr = node.attribute(name);
        if (!attr.empty())
        {
            std::string val = attr.value();
            if (val == "true")
                return true;
            if (val == "false")
                return false;
        }

        return def;
    }

    XmlNodePtr findFirstChildByName(const XmlNodePtrConst parent,
                                    const char *const name)
    {
        if (!parent || !name)
            return pugi::xml_node();
        for_each_xml_child_node(child, parent)
        {
            if (!strcmp(child.name(), name))
                return child;
        }

        return pugi::xml_node();
    }

    // Initialize libxml2 and check for potential ABI mismatches between
    // compiled version and the shared library actually used.
    void initXML()
    {
//        xmlInitParser();
//        LIBXML_TEST_VERSION;

        // Suppress libxml2 error messages
//        xmlSetGenericErrorFunc(nullptr, &xmlErrorLogger);
    }

    // Shutdown libxml
    void cleanupXML()
    {
//        xmlCleanupParser();
    }

    bool Document::validateXml(const std::string &fileName)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(fileName.c_str(),
            pugi::parse_default,
            pugi::encoding_utf8);

        if (result.status != pugi::status_ok)
        {
            showErrorStatus(result);
            return false;
        }

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

#endif  // ENABLE_PUGIXML
