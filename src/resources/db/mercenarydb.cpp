/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "resources/db/mercenarydb.h"

#include "resources/beingcommon.h"
#include "resources/beinginfo.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/gettext.h"

#include "configuration.h"

#include "debug.h"

namespace
{
    BeingInfos mMercenaryInfos;
    bool mLoaded = false;
}  // namespace

void MercenaryDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing mercenary database...");
    loadXmlFile(paths.getStringValue("mercenariesFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("mercenariesPatchFile"), SkipError_true);
    loadXmlDir("mercenariesPatchDir", loadXmlFile)

    mLoaded = true;
}

void MercenaryDB::loadXmlFile(const std::string &fileName,
                              const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtr rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "mercenaries"))
    {
        logger->log("MercenaryDB: Error while loading %s!",
            paths.getStringValue("mercenariesFile").c_str());
        mLoaded = true;
        return;
    }

    const int offset = XML::getProperty(rootNode, "offset", 0);

    // iterate <mercenary>s
    for_each_xml_child_node(mercenaryNode, rootNode)
    {
        if (xmlNameEqual(mercenaryNode, "include"))
        {
            const std::string name = XML::getProperty(
                mercenaryNode, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
        if (!xmlNameEqual(mercenaryNode, "mercenary"))
            continue;

        const int id = XML::getProperty(mercenaryNode, "id", 0);
        BeingInfo *currentInfo = nullptr;
        if (mMercenaryInfos.find(fromInt(id + offset, BeingTypeId))
            != mMercenaryInfos.end())
        {
            logger->log("MercenaryDB: Redefinition of mercenary ID %d", id);
            currentInfo = mMercenaryInfos[fromInt(id + offset, BeingTypeId)];
        }
        if (currentInfo == nullptr)
            currentInfo = new BeingInfo;

        currentInfo->setBlockType(BlockType::NONE);
        currentInfo->setName(XML::langProperty(
            // TRANSLATORS: unknown info name
            mercenaryNode, "name", _("unnamed")));
        BeingCommon::readBasicAttributes(currentInfo, mercenaryNode, "attack");
        BeingCommon::readWalkingAttributes(currentInfo, mercenaryNode, 0);
        BeingCommon::readAiAttributes(currentInfo, mercenaryNode);

        currentInfo->setMaxHP(XML::getProperty(mercenaryNode, "maxHP", 0));

        currentInfo->setDeadSortOffsetY(XML::getProperty(
            mercenaryNode, "deadSortOffsetY", 31));

        currentInfo->setColorsList(XML::getProperty(mercenaryNode,
            "colors", ""));

        if (currentInfo->getMaxHP() != 0)
            currentInfo->setStaticMaxHP(true);

        SpriteDisplay display;

        // iterate <sprite>s and <sound>s
        for_each_xml_child_node(spriteNode, mercenaryNode)
        {
            BeingCommon::readObjectNodes(spriteNode, display,
                currentInfo, "MonsterDB");
        }
        currentInfo->setDisplay(display);

        mMercenaryInfos[fromInt(id + offset, BeingTypeId)] = currentInfo;
    }
}

void MercenaryDB::unload()
{
    logger->log1("Unloading mercenary database...");
    delete_all(mMercenaryInfos);
    mMercenaryInfos.clear();

    mLoaded = false;
}


BeingInfo *MercenaryDB::get(const BeingTypeId id)
{
    BeingInfoIterator i = mMercenaryInfos.find(id);

    if (i == mMercenaryInfos.end())
    {
        i = mMercenaryInfos.find(id);
        if (i == mMercenaryInfos.end())
        {
            reportAlways("MercenaryDB: Warning, unknown mercenary ID "
                "%d requested",
                toInt(id, int))
            return BeingInfo::unknown;
        }
    }
    return i->second;
}
