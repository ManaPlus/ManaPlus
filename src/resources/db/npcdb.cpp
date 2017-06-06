/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#include "resources/db/npcdb.h"

#include "configuration.h"

#include "resources/beingcommon.h"
#include "resources/beinginfo.h"

#include "resources/db/unitsdb.h"

#include "resources/sprite/spritereference.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

namespace
{
    BeingInfos mNPCInfos;
    bool mLoaded = false;
}

void NPCDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing NPC database...");

    loadXmlFile(paths.getStringValue("npcsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("npcsPatchFile"), SkipError_true);
    loadXmlDir("npcsPatchDir", loadXmlFile);

    mLoaded = true;
}

void NPCDB::loadXmlFile(const std::string &fileName,
                        const SkipError skipError)
{
    XML::Document doc(fileName, UseVirtFs_true, skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "npcs"))
    {
        logger->log("NPC Database: Error while loading %s!",
            paths.getStringValue("npcsFile").c_str());
        mLoaded = true;
        return;
    }

    // iterate <npc>s
    for_each_xml_child_node(npcNode, rootNode)
    {
        if (xmlNameEqual(npcNode, "include"))
        {
            const std::string name = XML::getProperty(npcNode, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }

        if (!xmlNameEqual(npcNode, "npc"))
            continue;

        const BeingTypeId id = fromInt(XML::getProperty(
            npcNode, "id", 0), BeingTypeId);
        BeingInfo *currentInfo = nullptr;
        if (id == BeingTypeId_zero)
        {
            reportAlways("NPC Database: NPC with missing ID in %s!",
                paths.getStringValue("npcsFile").c_str());
            continue;
        }
        else if (mNPCInfos.find(id) != mNPCInfos.end())
        {
            logger->log("NpcDB: Redefinition of npc ID %d", toInt(id, int));
            currentInfo = mNPCInfos[id];
        }
        if (currentInfo == nullptr)
            currentInfo = new BeingInfo;

        currentInfo->setTargetSelection(XML::getBoolProperty(npcNode,
            "targetSelection", true));

        BeingCommon::readBasicAttributes(currentInfo, npcNode, "talk");
        BeingCommon::readWalkingAttributes(currentInfo, npcNode, 0);

        currentInfo->setDeadSortOffsetY(XML::getProperty(npcNode,
            "deadSortOffsetY", 31));

        currentInfo->setAvatarId(fromInt(XML::getProperty(
            npcNode, "avatar", 0), BeingTypeId));

        currentInfo->setAllowDelete(XML::getBoolProperty(npcNode,
            "allowDelete", true));

        const std::string currency = XML::getProperty(npcNode,
            "currency", "default");
        if (UnitsDb::existsCurrency(currency) == false)
        {
            reportAlways("Not found currency '%s' for npc %d",
                currency.c_str(),
                CAST_S32(id));
        }
        currentInfo->setCurrency(currency);

        SpriteDisplay display;
        for_each_xml_child_node(spriteNode, npcNode)
        {
            if (xmlNameEqual(spriteNode, "sprite"))
            {
                if (!XmlHaveChildContent(spriteNode))
                    continue;

                SpriteReference *const currentSprite = new SpriteReference;
                currentSprite->sprite = XmlChildContent(spriteNode);
                currentSprite->variant =
                    XML::getProperty(spriteNode, "variant", 0);
                display.sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                if (!XmlHaveChildContent(spriteNode))
                    continue;

                display.particles.push_back(XmlChildContent(spriteNode));
            }
            else if (xmlNameEqual(spriteNode, "menu"))
            {
                std::string name = XML::getProperty(spriteNode, "name", "");
                std::string command = XML::langProperty(spriteNode,
                    "command", "");
                currentInfo->addMenu(name, command);
            }
        }

        currentInfo->setDisplay(display);
        if (currentInfo->getMenu().empty())
        {
            // TRANSLATORS: npc context menu item
            currentInfo->addMenu(_("Talk"), "talk 'NAME'");
            // TRANSLATORS: npc context menu item
            currentInfo->addMenu(_("Buy"), "buy 'NAME'");
            // TRANSLATORS: npc context menu item
            currentInfo->addMenu(_("Sell"), "sell 'NAME'");
        }
        mNPCInfos[id] = currentInfo;
    }
}

void NPCDB::unload()
{
    delete_all(mNPCInfos);
    mNPCInfos.clear();

    mLoaded = false;
}

BeingInfo *NPCDB::get(const BeingTypeId id)
{
    const BeingInfoIterator i = mNPCInfos.find(id);

    if (i == mNPCInfos.end())
    {
        reportAlways("NPCDB: Warning, unknown NPC ID %d requested",
            toInt(id, int));
        return BeingInfo::unknown;
    }
    else
    {
        return i->second;
    }
}

BeingTypeId NPCDB::getAvatarFor(const BeingTypeId id)
{
    const BeingInfo *const info = get(id);
    if (info == nullptr)
        return BeingTypeId_zero;
    return info->getAvatarId();
}
