/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "resources/db/languagedb.h"

#include "configuration.h"

#include "utils/checkutils.h"

#include "resources/beingcommon.h"

#include "debug.h"

namespace
{
    std::string mDefaultIcon;
    std::string mDefaultPo;
    std::map<int, std::string> mIcons;
    std::map<int, std::string> mPo;
}  // namespace

void LanguageDb::load()
{
    unload();
    logger->log1("Initializing languages database...");
    loadXmlFile(paths.getStringValue("languagesFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("languagesPatchFile"), SkipError_true);
    loadXmlDir("languagesPatchDir", loadXmlFile)
}

void LanguageDb::loadXmlFile(const std::string &fileName,
                             const SkipError skipError)
{
    XML::Document *doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst root = doc->rootNode();

    if ((root == nullptr) || !xmlNameEqual(root, "languages"))
    {
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
        else if (xmlNameEqual(node, "lang"))
        {
            const int id = XML::getProperty(node, "id", -1);
            if (id < 0)
            {
                reportAlways("Missing lang id")
                continue;
            }
            const std::string icon = XML::getProperty(node, "icon", "");
            const std::string po = XML::getProperty(node, "po", "");
            if (icon.empty())
            {
                reportAlways("LanguageDb: empty icon field")
            }
            else
            {
                mIcons[id] = icon;
            }
            if (po.empty())
            {
                reportAlways("LanguageDb: empty po field")
            }
            else
            {
                mPo[id] = po;
            }
        }
    }

    delete doc;
}

void LanguageDb::unload()
{
    logger->log1("Unloading languages database...");
    mIcons.clear();
    mPo.clear();
}

const std::string &LanguageDb::getIcon(const int id)
{
    std::map<int, std::string>::const_iterator it = mIcons.find(id);
    if (it == mIcons.end())
        return mDefaultIcon;
    return (*it).second;
}

const std::string &LanguageDb::getPo(const int id)
{
    std::map<int, std::string>::const_iterator it = mPo.find(id);
    if (it == mPo.end())
        return mDefaultPo;
    return (*it).second;
}
