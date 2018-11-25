/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Aethyra Development Team
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "resources/db/colordb.h"

#include "configuration.h"

#include "utils/cast.h"
#include "utils/checkutils.h"

#include "resources/beingcommon.h"

#include "debug.h"

namespace
{
    int mHairColorsSize = 0;
    bool mLoaded = false;
    std::string mFail("#ffffff");
    ColorDB::ColorLists mColorLists;
}  // namespace

void ColorDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing color database...");

    std::map<ItemColor, ItemColorData> colors;
    ColorListsIterator it = mColorLists.find("hair");
    if (it != mColorLists.end())
        colors = it->second;
    loadHair(paths.getStringValue("hairColorFile"),
        colors,
        SkipError_true);
    loadHair(paths.getStringValue("hairColorPatchFile"),
        colors,
        SkipError_true);
    StringVect list;
    VirtFs::getFilesInDir(paths.getStringValue(
        "hairColorPatchDir"), list, ".xml");
    FOR_EACH (StringVectCIter, it2, list)
        loadHair(*it2, colors, SkipError_true);

    mColorLists["hair"] = colors;

    loadColorLists(paths.getStringValue("itemColorsFile"),
        SkipError_false);
    loadColorLists(paths.getStringValue("itemColorsPatchFile"),
        SkipError_true);
    loadXmlDir("itemColorsPatchDir", loadColorLists)

    it = mColorLists.find("hair");
    if (it != mColorLists.end())
        mHairColorsSize = CAST_S32((*it).second.size());
    else
        mHairColorsSize = 0;
    mLoaded = true;
}

void ColorDB::loadHair(const std::string &fileName,
                       std::map<ItemColor, ItemColorData> &colors,
                       const SkipError skipError)
{
    XML::Document *doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst root = doc->rootNode();

    if ((root == nullptr) || !xmlNameEqual(root, "colors"))
    {
        logger->log("ColorDB: Failed to find hair colors file.");
        if (colors.find(ItemColor_zero) == colors.end())
            colors[ItemColor_zero] = ItemColorData(ItemColor_zero, "", "");
        delete doc;
        return;
    }

    reportAlways("Found legacy hair.xml")
    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadHair(name, colors, skipError);
            continue;
        }
        else if (xmlNameEqual(node, "color"))
        {
            const ItemColor id = fromInt(XML::getProperty(
                node, "id", 0), ItemColor);

            if (colors.find(id) != colors.end())
            {
                reportAlways("ColorDB: Redefinition of dye ID %d",
                    toInt(id, int))
            }

            colors[id] = ItemColorData(id, XML::langProperty(node, "name", ""),
                XML::getProperty(node, "value", "#FFFFFF"));
        }
    }

    delete doc;
}

void ColorDB::loadColorLists(const std::string &fileName,
                             const SkipError skipError)
{
    XML::Document *doc = new XML::Document(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst root = doc->rootNode();
    if (root == nullptr)
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
                loadColorLists(name, skipError);
            continue;
        }
        else if (xmlNameEqual(node, "list"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (name.empty())
                continue;

            std::map <ItemColor, ItemColorData> colors;
            const ColorListsIterator it = mColorLists.find(name);

            if (it != mColorLists.end())
                colors = it->second;

            for_each_xml_child_node(colorNode, node)
            {
                if (xmlNameEqual(colorNode, "color"))
                {
                    const int id = XML::getProperty(colorNode, "id", -1);
                    if (id > -1)
                    {
                        ItemColorData c(fromInt(id, ItemColor),
                            XML::langProperty(colorNode, "name", ""),
                            XML::getProperty(colorNode, "value", ""));
                            colors[c.id] = c;
                    }
                }
            }
            mColorLists[name] = colors;
        }
    }
    delete doc;
}

void ColorDB::unload()
{
    logger->log1("Unloading color database...");

    mColorLists.clear();
    mLoaded = false;
}

std::string &ColorDB::getHairColorName(const ItemColor id)
{
    if (!mLoaded)
        load();

    const ColorListsIterator it = mColorLists.find("hair");
    if (it == mColorLists.end())
    {
        reportAlways("ColorDB: Error, hair colors list empty")
        return mFail;
    }

    const ColorIterator i = (*it).second.find(id);

    if (i == (*it).second.end())
    {
        reportAlways("ColorDB: Error, unknown dye ID# %d",
            toInt(id, int))
        return mFail;
    }
    return i->second.name;
}

int ColorDB::getHairSize()
{
    return mHairColorsSize;
}

const std::map <ItemColor, ItemColorData>
     *ColorDB::getColorsList(const std::string &name)
{
    const ColorListsIterator it = mColorLists.find(name);

    if (it != mColorLists.end())
        return &it->second;
    return nullptr;
}
