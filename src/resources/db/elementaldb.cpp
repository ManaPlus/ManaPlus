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

#include "resources/db/elementaldb.h"

#include "configuration.h"

#include "resources/beingcommon.h"
#include "resources/beinginfo.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"

#include "debug.h"

namespace
{
    BeingInfos mElementalInfos;
    bool mLoaded = false;
}

void ElementalDb::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing elemental database...");
    loadXmlFile(paths.getStringValue("elementalsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("elementalsPatchFile"), SkipError_true);
    loadXmlDir("elementalsPatchDir", loadXmlFile);

    mLoaded = true;
}

void ElementalDb::loadXmlFile(const std::string &fileName,
                              const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
    XmlNodeConstPtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "elementals"))
    {
        logger->log("Elemental Database: Error while loading %s!",
            paths.getStringValue("elementalsFile").c_str());
        mLoaded = true;
        return;
    }

    const int offset = XML::getProperty(rootNode, "offset", 0);

    // iterate <elemental>s
    for_each_xml_child_node(elementalNode, rootNode)
    {
        if (xmlNameEqual(elementalNode, "include"))
        {
            const std::string name = XML::getProperty(
                elementalNode, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
        if (!xmlNameEqual(elementalNode, "elemental"))
            continue;

        const int id = XML::getProperty(elementalNode, "id", 0);
        BeingInfo *currentInfo = nullptr;
        if (mElementalInfos.find(fromInt(id + offset, BeingTypeId))
            != mElementalInfos.end())
        {
            logger->log("ElementalDb: Redefinition of elemental ID %d", id);
            currentInfo = mElementalInfos[fromInt(id + offset, BeingTypeId)];
        }
        if (!currentInfo)
            currentInfo = new BeingInfo;

        currentInfo->setBlockType(BlockType::NONE);
        BeingCommon::readBasicAttributes(currentInfo,
            elementalNode, "attack");
        BeingCommon::readWalkingAttributes(currentInfo,
            elementalNode,
            0);

        currentInfo->setMaxHP(XML::getProperty(elementalNode, "maxHP", 0));

        currentInfo->setDeadSortOffsetY(XML::getProperty(
            elementalNode, "deadSortOffsetY", 31));

        currentInfo->setColorsList(XML::getProperty(elementalNode,
            "colors", ""));

        if (currentInfo->getMaxHP())
            currentInfo->setStaticMaxHP(true);

        SpriteDisplay display;

        // iterate <sprite>s and <sound>s
        for_each_xml_child_node(spriteNode, elementalNode)
        {
            BeingCommon::readObjectNodes(spriteNode, display,
                currentInfo, "ElementalDb");
        }
        currentInfo->setDisplay(display);

        mElementalInfos[fromInt(id + offset, BeingTypeId)] = currentInfo;
    }
}

void ElementalDb::unload()
{
    delete_all(mElementalInfos);
    mElementalInfos.clear();

    mLoaded = false;
}


BeingInfo *ElementalDb::get(const BeingTypeId id)
{
    BeingInfoIterator i = mElementalInfos.find(id);

    if (i == mElementalInfos.end())
    {
        i = mElementalInfos.find(id);
        if (i == mElementalInfos.end())
        {
            reportAlways("ElementalDb: Warning, unknown elemental ID "
                "%d requested",
                toInt(id, int));
            return BeingInfo::unknown;
        }
        else
        {
            return i->second;
        }
    }
    else
    {
        return i->second;
    }
}
