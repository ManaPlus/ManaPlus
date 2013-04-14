/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "resources/petdb.h"

#include "logger.h"

#include "resources/beinginfo.h"

#include "utils/dtor.h"
#include "configuration.h"

#include "debug.h"

namespace
{
    BeingInfos mPETInfos;
    bool mLoaded = false;
}

extern int serverVersion;

void PETDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing PET database...");

    XML::Document doc("pets.xml");
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "pets"))
    {
        logger->log("PET Database: Error while loading pets.xml!");
        mLoaded = true;
        return;
    }

    // iterate <pet>s
    for_each_xml_child_node(petNode, rootNode)
    {
        if (!xmlNameEqual(petNode, "pet"))
            continue;

        const int id = XML::getProperty(petNode, "id", 0);
        if (id == 0)
        {
            logger->log1("PET Database: PET with missing ID in pets.xml!");
            continue;
        }

        BeingInfo *const currentInfo = new BeingInfo;

        currentInfo->setTargetSelection(XML::getBoolProperty(petNode,
            "targetSelection", false));

        currentInfo->setTargetCursorSize(XML::getProperty(petNode,
            "targetCursor", "medium"));

        currentInfo->setHoverCursor(XML::getProperty(petNode,
            "hoverCursor", "talk"));

        currentInfo->setTargetOffsetX(XML::getProperty(petNode,
            "targetOffsetX", 0));

        currentInfo->setTargetOffsetY(XML::getProperty(petNode,
            "targetOffsetY", 0));

        currentInfo->setSortOffsetY(XML::getProperty(petNode,
            "sortOffsetY", 0));

        currentInfo->setDeadSortOffsetY(XML::getProperty(petNode,
            "deadSortOffsetY", 31));

        SpriteDisplay display;
        for_each_xml_child_node(spriteNode, petNode)
        {
            if (!spriteNode->xmlChildrenNode)
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                SpriteReference *const currentSprite = new SpriteReference;
                currentSprite->sprite = reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content);
                currentSprite->variant =
                    XML::getProperty(spriteNode, "variant", 0);
                display.sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                std::string particlefx = reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content);
                display.particles.push_back(particlefx);
            }
        }

        currentInfo->setDisplay(display);

        mPETInfos[id] = currentInfo;
    }

    mLoaded = true;
}

void PETDB::unload()
{
    delete_all(mPETInfos);
    mPETInfos.clear();

    mLoaded = false;
}

BeingInfo *PETDB::get(const int id)
{
    const BeingInfoIterator i = mPETInfos.find(id);

    if (i == mPETInfos.end())
    {
        logger->log("PETDB: Warning, unknown PET ID %d requested", id);
        return BeingInfo::unknown;
    }
    else
    {
        return i->second;
    }
}
