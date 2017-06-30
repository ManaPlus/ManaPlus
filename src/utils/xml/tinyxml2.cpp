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

#ifdef ENABLE_TINYXML2

#include "utils/xml/tinyxml2.h"

#include "fs/virtfs/fs.h"

#include "utils/cast.h"
#include "utils/checkutils.h"
#include "utils/fuzzer.h"
#include "utils/stringutils.h"

#include "utils/translation/podict.h"

#include "debug.h"

namespace
{
    bool valid = false;
}  // namespace

namespace XML
{
    static void showErrorStatus(tinyxml2::XMLDocument &doc)
    {
        logger->log("xml error: %s, in lines: %s\n%s",
            doc.ErrorName(),
            doc.GetErrorStr1(),
            doc.GetErrorStr2());
    }

    Document::Document(const std::string &filename,
                       const UseVirtFs useResman,
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
        if (useResman == UseVirtFs_true)
        {
            data = const_cast<char*>(VirtFs::loadFile(
                filename.c_str(),
                size));
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
                    data = new char[size];
                    file.read(data, size);
                }
                file.close();
            }
            else if (skipError == SkipError_false)
            {
                reportAlways("Error loading XML file %s",
                    filename.c_str());
            }
        }

        if (data)
        {
            tinyxml2::XMLError result = mDoc.Parse(data,
                size);
            if (result != tinyxml2::XML_SUCCESS)
            {
                showErrorStatus(mDoc);
                delete [] data;
            }
            else
            {
                mData = data;
            }
        }
        else if (skipError == SkipError_false)
        {
            reportAlways("Error loading %s", filename.c_str());
        }
        mIsValid = valid;
        BLOCK_END("XML::Document::Document")
    }

    Document::Document(const char *const data, const int size) :
        Resource(),
        mDoc(),
        mData(nullptr),
        mIsValid(true)
    {
        if (!data)
            return;

        char *buf = new char[size + 1];
        strncpy(buf, data, size);
        buf[size] = 0;

        tinyxml2::XMLError result = mDoc.Parse(buf,
            size);
        if (result != tinyxml2::XML_SUCCESS)
        {
            showErrorStatus(mDoc);
            delete [] buf;
        }
        else
        {
            mData = buf;
        }
    }

    Document::~Document()
    {
        delete [] mData;
        mData = nullptr;
    }

    XmlNodeConstPtr Document::rootNode()
    {
        return mDoc.FirstChildElement();
    }

    int getProperty(XmlNodeConstPtr node,
                    const char *const name,
                    int def)
    {
        int &ret = def;

        if (!node)
            return ret;
        const char *attr = node->Attribute(name);
        if (attr != nullptr)
            ret = atoi(attr);

        return ret;
    }

    int getIntProperty(XmlNodeConstPtr node,
                       const char *const name,
                       int def,
                       const int min,
                       const int max)
    {
        int &ret = def;

        if (!node)
            return ret;
        const char *attr = node->Attribute(name);
        if (attr != nullptr)
            ret = atoi(attr);

        if (ret < min)
            ret = min;
        else if (ret > max)
            ret = max;
        return ret;
    }

    double getFloatProperty(XmlNodeConstPtr node,
                            const char *const name,
                            double def)
    {
        double &ret = def;

        if (!node)
            return ret;
        const char *attr = node->Attribute(name);
        if (attr != nullptr)
            ret = atof(attr);

        return ret;
    }

    std::string getProperty(XmlNodeConstPtr node,
                            const char *const name,
                            const std::string &def)
    {
        if (!node)
            return def;
        const char *attr = node->Attribute(name);
        if (attr != nullptr)
            return attr;

        return def;
    }

    std::string langProperty(XmlNodeConstPtr node,
                             const char *const name,
                             const std::string &def)
    {
        std::string str = getProperty(node, name, def);
        if (!translator)
            return str;

        return translator->getStr(str);
    }

    bool getBoolProperty(XmlNodeConstPtr node,
                         const char *const name,
                         const bool def)
    {
        if (!node)
            return def;
        const char *attr = node->Attribute(name);
        if (attr != nullptr)
        {
            std::string val = attr;
            if (val == "true")
                return true;
            if (val == "false")
                return false;
        }

        return def;
    }

    XmlNodeConstPtr findFirstChildByName(XmlNodeConstPtrConst parent,
                                         const char *const name)
    {
        if (!parent || !name)
            return nullptr;
        return parent->FirstChildElement(name);
    }

    // Initialize xml
    void initXML()
    {
    }

    // Shutdown xml
    void cleanupXML()
    {
    }

    bool Document::validateXml(const std::string &fileName)
    {
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError result = doc.LoadFile(fileName.c_str());

        if (result != tinyxml2::XML_SUCCESS)
        {
            showErrorStatus(doc);
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

#endif  // ENABLE_TINYXML2
