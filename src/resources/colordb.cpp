/*
 *  Color database
 *  Copyright (C) 2008  Aethyra Development Team
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

#include "resources/colordb.h"

#include "client.h"
#include "logger.h"

#include "utils/xml.h"

#include <libxml/tree.h>

#include "debug.h"

namespace
{
    ColorDB::Colors mHairColors;
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
}

void ColorDB::loadHair()
{
    XML::Document *doc = new XML::Document("hair.xml");
    xmlNodePtr root = doc->rootNode();
    bool hairXml = true;

    if (!root || !xmlStrEqual(root->name, BAD_CAST "colors"))
    {
        logger->log1("Trying to fall back on colors.xml");

        hairXml = false;

        delete doc;
        doc = new XML::Document("colors.xml");
        root = doc->rootNode();

        if (!root || !xmlStrEqual(root->name, BAD_CAST "colors"))
        {
            logger->log1("ColorDB: Failed to find any color files.");
            mHairColors[0] = mFail;
            mLoaded = true;

            delete doc;

            return;
        }
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlStrEqual(node->name, BAD_CAST "color"))
        {
            int id = XML::getProperty(node, "id", 0);

            if (mHairColors.find(id) != mHairColors.end())
                logger->log("ColorDB: Redefinition of dye ID %d", id);

            mHairColors[id] = hairXml ?
                XML::getProperty(node, "value", "#FFFFFF") :
                XML::getProperty(node, "dye", "#FFFFFF");
        }
    }

    delete doc;

    mLoaded = true;
}

void ColorDB::loadColorLists()
{
    XML::Document *doc = new XML::Document("itemcolors.xml");
    xmlNodePtr root = doc->rootNode();
    if (!root)
    {
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlStrEqual(node->name, BAD_CAST "list"))
        {
            std::string name = XML::getProperty(node, "name", "");
            if (name.empty())
                continue;

            std::map <int, ItemColor> colors;
            ColorListsIterator it = mColorLists.find(name);

            if (it != mColorLists.end())
                colors = it->second;

            for_each_xml_child_node(colorNode, node)
            {
                if (xmlStrEqual(colorNode->name, BAD_CAST "color"))
                {
                    ItemColor c(XML::getProperty(colorNode, "id", -1),
                        XML::getProperty(colorNode, "name", ""),
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

    mHairColors.clear();
    mColorLists.clear();
    mLoaded = false;
}

std::string &ColorDB::getHairColor(int id)
{
    if (!mLoaded)
        load();

    ColorIterator i = mHairColors.find(id);

    if (i == mHairColors.end())
    {
        logger->log("ColorDB: Error, unknown dye ID# %d", id);
        return mFail;
    }
    else
    {
        return i->second;
    }
}

int ColorDB::getHairSize()
{
    return static_cast<int>(mHairColors.size());
}

std::map <int, ColorDB::ItemColor> *ColorDB::getColorsList(std::string name)
{
    std::map <int, ItemColor> colors;
    ColorListsIterator it = mColorLists.find(name);

    if (it != mColorLists.end())
        return &it->second;
    return nullptr;
}
