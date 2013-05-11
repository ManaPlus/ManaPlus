/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Aethyra Development Team
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

#include "resources/colordb.h"

#include "client.h"
#include "logger.h"

#include "utils/xml.h"

#include "debug.h"

namespace
{
    int mHairColorsSize = 0;
    bool mLoaded = false;
    std::string mFail = "#ffffff";
    ColorDB::ColorLists mColorLists;
}

void ColorDB::load()
{
    if (mLoaded)
        unload();

    loadHair();
    if (serverVersion >= 1)
        loadColorLists();

    const ColorListsIterator it = mColorLists.find("hair");
    if (it != mColorLists.end())
        mHairColorsSize = static_cast<int>((*it).second.size());
    else
        mHairColorsSize = 0;
}

void ColorDB::loadHair()
{
    std::map <int, ItemColor> colors;
    const ColorListsIterator it = mColorLists.find("hair");

    if (it != mColorLists.end())
        colors = it->second;

    XML::Document *doc = new XML::Document("hair.xml");
    XmlNodePtr root = doc->rootNode();
    bool hairXml = true;

    if (!root || !xmlNameEqual(root, "colors"))
    {
        logger->log1("Trying to fall back on colors.xml");

        hairXml = false;

        delete doc;
        doc = new XML::Document("colors.xml");
        root = doc->rootNode();

        if (!root || !xmlNameEqual(root, "colors"))
        {
            logger->log1("ColorDB: Failed to find any color files.");
            colors[0] = ItemColor(0, "", "");
            mLoaded = true;

            delete doc;

            return;
        }
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "color"))
        {
            const int id = XML::getProperty(node, "id", 0);

            if (colors.find(id) != colors.end())
                logger->log("ColorDB: Redefinition of dye ID %d", id);

            colors[id] = ItemColor(id, XML::langProperty(node, "name", ""),
                XML::getProperty(node, hairXml ? "value" : "dye", "#FFFFFF"));
        }
    }

    delete doc;

    mColorLists["hair"] = colors;
    mLoaded = true;
}

void ColorDB::loadColorLists()
{
    XML::Document *doc = new XML::Document("itemcolors.xml");
    const XmlNodePtr root = doc->rootNode();
    if (!root)
    {
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "list"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (name.empty())
                continue;

            std::map <int, ItemColor> colors;
            const ColorListsIterator it = mColorLists.find(name);

            if (it != mColorLists.end())
                colors = it->second;

            for_each_xml_child_node(colorNode, node)
            {
                if (xmlNameEqual(colorNode, "color"))
                {
                    ItemColor c(XML::getProperty(colorNode, "id", -1),
                        XML::langProperty(colorNode, "name", ""),
                        XML::getProperty(colorNode, "value", ""));
                    if (c.id > -1)
                        colors[c.id] = c;
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

std::string &ColorDB::getHairColorName(const int id)
{
    if (!mLoaded)
        load();

    const ColorListsIterator it = mColorLists.find("hair");
    if (it == mColorLists.end())
    {
        logger->log1("ColorDB: Error, hair colors list empty");
        return mFail;
    }

    const ColorIterator i = (*it).second.find(id);

    if (i == (*it).second.end())
    {
        logger->log("ColorDB: Error, unknown dye ID# %d", id);
        return mFail;
    }
    else
    {
        return i->second.name;
    }
}

int ColorDB::getHairSize()
{
    return mHairColorsSize;
}

const std::map <int, ColorDB::ItemColor>
     *ColorDB::getColorsList(const std::string &name)
{
    const ColorListsIterator it = mColorLists.find(name);

    if (it != mColorLists.end())
        return &it->second;
    return nullptr;
}
