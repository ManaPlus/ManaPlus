/*
 *  The ManaPlus Client
 *  Copyright (C) 2010  The Mana Developers
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

#include "resources/specialdb.h"

#include "logger.h"

#include "utils/dtor.h"
#include "utils/xml.h"

#include "debug.h"

namespace
{
    SpecialInfos mSpecialInfos;
    bool mLoaded = false;
}

SpecialInfo::TargetMode SpecialDB::targetModeFromString(const std::string& str)
{
    if (str == "self")        return SpecialInfo::TARGET_SELF;
    else if (str == "friend") return SpecialInfo::TARGET_FRIEND;
    else if (str == "enemy")  return SpecialInfo::TARGET_ENEMY;
    else if (str == "being")  return SpecialInfo::TARGET_BEING;
    else if (str == "point")  return SpecialInfo::TARGET_POINT;

    logger->log("SpecialDB: Warning, unknown target mode \"%s\"",
                str.c_str() );
    return SpecialInfo::TARGET_SELF;
}

void SpecialDB::load()
{
    if (mLoaded)
        unload();

    logger->log("Initializing special database...");

    XML::Document doc("specials.xml");
    XmlNodePtr root = doc.rootNode();

    if (!root || !xmlNameEqual(root, "specials"))
    {
        logger->log("Error loading specials file specials.xml");
        return;
    }

    std::string setName;

    for_each_xml_child_node(set, root)
    {
        if (xmlNameEqual(set, "set"))
        {
            setName = XML::getProperty(set, "name", "Actions");

            for_each_xml_child_node(special, set)
            {
                if (xmlNameEqual(special, "special"))
                {
                    SpecialInfo *info = new SpecialInfo();
                    int id = XML::getProperty(special, "id", 0);
                    info->id = id;
                    info->set = setName;
                    info->name = XML::getProperty(special, "name", "");
                    info->icon = XML::getProperty(special, "icon", "");

                    info->isActive = XML::getBoolProperty(
                        special, "active", false);
                    info->targetMode = targetModeFromString(XML::getProperty(
                        special, "target", "self"));

                    info->level = XML::getProperty(special, "level", -1);
                    info->hasLevel = info->level > -1;

                    info->hasRechargeBar = XML::getBoolProperty(special,
                        "recharge", false);
                    info->rechargeNeeded = 0;
                    info->rechargeCurrent = 0;

                    if (mSpecialInfos.find(id) != mSpecialInfos.end())
                    {
                        logger->log("SpecialDB: Duplicate special ID"
                                    " %d (ignoring)", id);
                    }
                    else
                    {
                        mSpecialInfos[id] = info;
                    }
                }
            }
        }
    }

    mLoaded = true;
}

void SpecialDB::unload()
{
    delete_all(mSpecialInfos);
    mSpecialInfos.clear();

    mLoaded = false;
}


SpecialInfo *SpecialDB::get(int id)
{
    SpecialInfos::const_iterator i = mSpecialInfos.find(id);

    if (i == mSpecialInfos.end())
        return nullptr;
    else
        return i->second;
}
