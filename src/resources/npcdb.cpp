/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/npcdb.h"

#include "logger.h"

#include "resources/beinginfo.h"

#include "utils/dtor.h"
#include "utils/xml.h"
#include "configuration.h"

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

    XML::Document doc("npcs.xml");
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "npcs"))
    {
        logger->log("NPC Database: Error while loading npcs.xml!");
        mLoaded = true;
        return;
    }

    //iterate <npc>s
    for_each_xml_child_node(npcNode, rootNode)
    {
        if (!xmlStrEqual(npcNode->name, BAD_CAST "npc"))
            continue;

        int id = XML::getProperty(npcNode, "id", 0);
        if (id == 0)
        {
            logger->log1("NPC Database: NPC with missing ID in npcs.xml!");
            continue;
        }

        BeingInfo *currentInfo = new BeingInfo;

        currentInfo->setTargetCursorSize(XML::getProperty(npcNode,
                                         "targetCursor", "medium"));

        currentInfo->setTargetOffsetX(XML::getProperty(npcNode,
                                         "targetOffsetX", 0));

        currentInfo->setTargetOffsetY(XML::getProperty(npcNode,
                                         "targetOffsetY", 0));
        SpriteDisplay display;
        for_each_xml_child_node(spriteNode, npcNode)
        {
            if (!spriteNode->xmlChildrenNode)
                continue;

            if (xmlStrEqual(spriteNode->name, BAD_CAST "sprite"))
            {
                SpriteReference *currentSprite = new SpriteReference;
                currentSprite->sprite = reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content);
                currentSprite->variant =
                    XML::getProperty(spriteNode, "variant", 0);
                display.sprites.push_back(currentSprite);
            }
            else if (xmlStrEqual(spriteNode->name, BAD_CAST "particlefx"))
            {
                std::string particlefx = reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content);
                display.particles.push_back(particlefx);
            }
        }

        currentInfo->setDisplay(display);

        mNPCInfos[id] = currentInfo;
    }

    mLoaded = true;
}

void NPCDB::unload()
{
    delete_all(mNPCInfos);
    mNPCInfos.clear();

    mLoaded = false;
}

BeingInfo *NPCDB::get(int id)
{
    BeingInfoIterator i = mNPCInfos.find(id);

    if (i == mNPCInfos.end())
    {
        logger->log("NPCDB: Warning, unknown NPC ID %d requested", id);
        return BeingInfo::unknown;
    }
    else
    {
        return i->second;
    }
}
