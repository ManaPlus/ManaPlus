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

#include "resources/chardb.h"

#include "client.h"
#include "logger.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    unsigned mMinHairColor = 0;
    unsigned mMaxHairColor = 0;
    unsigned mMinHairStyle = 0;
    unsigned mMaxHairStyle = 0;
    unsigned mMinStat = 0;
    unsigned mMaxStat = 0;
    unsigned mSumStat = 0;
    std::vector<int> mDefaultItems;
}  // namespace

void CharDB::load()
{
    if (mLoaded)
        unload();

    XML::Document *doc = new XML::Document("charcreation.xml");
    const XmlNodePtr root = doc->rootNode();

    if (!root || !xmlNameEqual(root, "chars"))
    {
        logger->log1("CharDB: Failed to parse charcreation.xml.");

        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "haircolor"))
        {
            loadMinMax(node, &mMinHairColor, &mMaxHairColor);
        }
        else if (xmlNameEqual(node, "hairstyle"))
        {
            loadMinMax(node, &mMinHairStyle, &mMaxHairStyle);
        }
        else if (xmlNameEqual(node, "stat"))
        {
            loadMinMax(node, &mMinStat, &mMaxStat);
            mSumStat = XML::getProperty(node, "sum", 0);
        }
        else if (xmlNameEqual(node, "item"))
        {
            const int id = XML::getProperty(node, "id", 0);
            if (id > 0)
                mDefaultItems.push_back(id);
        }
    }

    delete doc;

    mLoaded = true;
}

void CharDB::loadMinMax(const XmlNodePtr node,
                        unsigned *const min, unsigned *const max)
{
    *min = XML::getProperty(node, "min", 1);
    *max = XML::getProperty(node, "max", 10);
}

void CharDB::unload()
{
    logger->log1("Unloading chars database...");

    mLoaded = false;
}

unsigned CharDB::getMinHairColor()
{
    return mMinHairColor;
}

unsigned CharDB::getMaxHairColor()
{
    return mMaxHairColor;
}

unsigned CharDB::getMinHairStyle()
{
    return mMinHairStyle;
}

unsigned CharDB::getMaxHairStyle()
{
    return mMaxHairStyle;
}

unsigned CharDB::getMinStat()
{
    return mMinStat;
}

unsigned CharDB::getMaxStat()
{
    return mMaxStat;
}

unsigned CharDB::getSumStat()
{
    return mSumStat;
}

const std::vector<int> &CharDB::getDefaultItems()
{
    return mDefaultItems;
}
