/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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
    std::map<int, std::string> mLanguages;
}  // namespace

void LanguageDb::load()
{
    unload();
    loadXmlFile(paths.getStringValue("languagesFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("languagesPatchFile"), SkipError_true);
    loadXmlDir("languagesPatchDir", loadXmlFile);
}

void LanguageDb::loadXmlFile(const std::string &fileName,
                             const SkipError skipError)
{
    XML::Document *doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst root = doc->rootNode();

    if (!root || !xmlNameEqual(root, "languages"))
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
                reportAlways("Missing lang id");
                continue;
            }
            const std::string icon = XML::getProperty(node, "icon", "");
            mLanguages[id] = icon;
        }
    }

    delete doc;
}

void LanguageDb::unload()
{
    mLanguages.clear();
}

const std::string &LanguageDb::getIcon(const int id)
{
    std::map<int, std::string>::const_iterator it = mLanguages.find(id);
    if (it == mLanguages.end())
        return mDefaultIcon;
    return (*it).second;
}
