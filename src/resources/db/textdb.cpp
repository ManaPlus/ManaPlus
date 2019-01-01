/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2019  The ManaPlus Developers
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

#include "resources/db/textdb.h"

#include "configuration.h"

#include "utils/checkutils.h"

#include "resources/beingcommon.h"

#include "debug.h"

namespace
{
    StringVect mTexts;
}  // namespace

void TextDb::load()
{
    unload();
    logger->log1("Initializing text database...");
    loadXmlFile(paths.getStringValue("textsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("textsPatchFile"), SkipError_true);
    loadXmlDir("textsPatchDir", loadXmlFile)
}

void TextDb::loadXmlFile(const std::string &fileName,
                         const SkipError skipError)
{
    XML::Document *doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst root = doc->rootNode();

    if ((root == nullptr) || !xmlNameEqual(root, "texts"))
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
        else if (xmlNameEqual(node, "text"))
        {
            const bool show = XML::getBoolProperty(node, "show", false);
            if (show == true)
            {
                if (!XmlHaveChildContent(node))
                    continue;

                std::string text = XmlChildContent(node);
                mTexts.push_back(text);
            }
        }
    }

    delete doc;
}

void TextDb::unload()
{
    logger->log1("Unloading text database...");
    mTexts.clear();
}

const StringVect &TextDb::getTexts()
{
    return mTexts;
}

std::string TextDb::getByIndex(const int index)
{
    if (index < 0 ||
        static_cast<size_t>(index) >= mTexts.size())
    {
        return std::string();
    }
    return mTexts[index];
}
