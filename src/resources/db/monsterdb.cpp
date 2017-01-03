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

#include "resources/db/monsterdb.h"

#include "enums/resources/map/blockmask.h"

#include "resources/beingcommon.h"
#include "resources/beinginfo.h"

#include "utils/checkutils.h"
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
    loadXmlFile(paths.getStringValue("monstersFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("monstersPatchFile"), SkipError_true);
    loadXmlDir("monstersPatchDir", loadXmlFile);

    mLoaded = true;
}

void MonsterDB::loadXmlFile(const std::string &fileName,
                            const SkipError skipError)
{
    XML::Document doc(fileName, UseResman_true, skipError);
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
                loadXmlFile(name, skipError);
            continue;
        }
        if (!xmlNameEqual(monsterNode, "monster"))
            continue;

        const int id = XML::getProperty(monsterNode, "id", 0);
        BeingInfo *currentInfo = nullptr;
        if (mMonsterInfos.find(fromInt(id + offset, BeingTypeId))
            != mMonsterInfos.end())
        {
            logger->log("MonsterDB: Redefinition of monster ID %d", id);
            currentInfo = mMonsterInfos[fromInt(id + offset, BeingTypeId)];
        }
        if (!currentInfo)
            currentInfo = new BeingInfo;

        currentInfo->setBlockType(BlockType::NONE);
        currentInfo->setName(XML::langProperty(
            // TRANSLATORS: unknown info name
            monsterNode, "name", _("unnamed")));

        BeingCommon::readBasicAttributes(currentInfo, monsterNode, "attack");
        BeingCommon::readWalkingAttributes(currentInfo, monsterNode,
            BlockMask::MONSTERWALL);

        currentInfo->setMaxHP(XML::getProperty(monsterNode, "maxHP", 0));

        currentInfo->setDeadSortOffsetY(XML::getProperty(
            monsterNode, "deadSortOffsetY", 31));

        currentInfo->setColorsList(XML::getProperty(monsterNode,
            "colors", ""));

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

        mMonsterInfos[fromInt(id + offset, BeingTypeId)] = currentInfo;
    }
}

void MonsterDB::unload()
{
    delete_all(mMonsterInfos);
    mMonsterInfos.clear();

    mLoaded = false;
}


BeingInfo *MonsterDB::get(const BeingTypeId id)
{
    BeingInfoIterator i = mMonsterInfos.find(id);

    if (i == mMonsterInfos.end())
    {
        i = mMonsterInfos.find(fromInt(toInt(
            id, int) + OLD_TMWATHENA_OFFSET, BeingTypeId));
        if (i == mMonsterInfos.end())
        {
            reportAlways("MonsterDB: Warning, unknown monster ID %d requested",
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
