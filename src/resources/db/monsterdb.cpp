/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "resources/db/monsterdb.h"

#include "logger.h"

#include "resources/beingcommon.h"
#include "resources/beinginfo.h"

#include "resources/map/blockmask.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "configuration.h"

#include "debug.h"

static const unsigned int OLD_TMWATHENA_OFFSET = 1002;

namespace
{
    BeingInfos mMonsterInfos;
    bool mLoaded = false;
}

void MonsterDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing monster database...");
    loadXmlFile(paths.getStringValue("monstersFile"));
    loadXmlFile(paths.getStringValue("monstersPatchFile"));
    loadXmlDir("monstersPatchDir", loadXmlFile);

    mLoaded = true;
}

void MonsterDB::loadXmlFile(const std::string &fileName)
{
    XML::Document doc(fileName, true, false);
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "monsters"))
    {
        logger->log("Monster Database: Error while loading %s!",
            paths.getStringValue("monstersFile").c_str());
        mLoaded = true;
        return;
    }

    const int offset = XML::getProperty(rootNode,
        "offset", OLD_TMWATHENA_OFFSET);

    // iterate <monster>s
    for_each_xml_child_node(monsterNode, rootNode)
    {
        if (xmlNameEqual(monsterNode, "include"))
        {
            const std::string name = XML::getProperty(monsterNode, "name", "");
            if (!name.empty())
                loadXmlFile(name);
            continue;
        }
        if (!xmlNameEqual(monsterNode, "monster"))
            continue;

        const int id = XML::getProperty(monsterNode, "id", 0);
        BeingInfo *currentInfo = nullptr;
        if (mMonsterInfos.find(id + offset) != mMonsterInfos.end())
        {
            logger->log("MonsterDB: Redefinition of monster ID %d", id);
            currentInfo = mMonsterInfos[id + offset];
        }
        if (!currentInfo)
            currentInfo = new BeingInfo;

        currentInfo->setBlockType(BlockType::MONSTER);
        currentInfo->setName(XML::langProperty(
            // TRANSLATORS: unknown info name
            monsterNode, "name", _("unnamed")));

        BeingCommon::readBasicAttributes(currentInfo, monsterNode, "attack");

        currentInfo->setMaxHP(XML::getProperty(monsterNode, "maxHP", 0));

        currentInfo->setDeadSortOffsetY(XML::getProperty(
            monsterNode, "deadSortOffsetY", 31));

        currentInfo->setColorsList(XML::getProperty(monsterNode,
            "colors", ""));

        unsigned char block = 0;
        std::string walkStr = XML::getProperty(
            monsterNode, "walkType", "walk");
        if (walkStr == "walk")
            block = BlockMask::WATER | BlockMask::AIR;
        else if (walkStr == "fly")
            block = 0;
        else if (walkStr == "swim")
            block = BlockMask::GROUND | BlockMask::AIR;
        else if (walkStr == "walkswim" || walkStr == "swimwalk")
            block = BlockMask::AIR;

        currentInfo->setBlockWalkMask(static_cast<unsigned char>(
            BlockMask::WALL | block));

        if (currentInfo->getMaxHP())
            currentInfo->setStaticMaxHP(true);

        SpriteDisplay display;

        // iterate <sprite>s and <sound>s
        for_each_xml_child_node(spriteNode, monsterNode)
        {
            BeingCommon::readObjectNodes(spriteNode, display,
                currentInfo, "MonsterDB");
        }
        currentInfo->setDisplay(display);

        mMonsterInfos[id + offset] = currentInfo;
    }
}

void MonsterDB::unload()
{
    delete_all(mMonsterInfos);
    mMonsterInfos.clear();

    mLoaded = false;
}


BeingInfo *MonsterDB::get(const int id)
{
    BeingInfoIterator i = mMonsterInfos.find(id);

    if (i == mMonsterInfos.end())
    {
        i = mMonsterInfos.find(id + OLD_TMWATHENA_OFFSET);
        if (i == mMonsterInfos.end())
        {
            logger->log("MonsterDB: Warning, unknown monster ID %d requested",
                        id);
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
