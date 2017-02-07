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

#include "resources/db/petdb.h"

#include "configuration.h"

#include "resources/beingcommon.h"
#include "resources/beinginfo.h"

#include "resources/sprite/spritereference.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

namespace
{
    BeingInfos mPETInfos;
    bool mLoaded = false;
}

void PETDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing PET database...");
    loadXmlFile(paths.getStringValue("petsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("petsPatchFile"), SkipError_true);
    loadXmlDir("petsPatchDir", loadXmlFile);
    mLoaded = true;
}

void PETDB::loadXmlFile(const std::string &fileName,
                        const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "pets"))
    {
        logger->log("PET Database: Error while loading %s!",
            fileName.c_str());
        return;
    }

    // iterate <pet>s
    for_each_xml_child_node(petNode, rootNode)
    {
        if (xmlNameEqual(petNode, "include"))
        {
            const std::string name = XML::getProperty(petNode, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
        else if (!xmlNameEqual(petNode, "pet"))
        {
            continue;
        }

        const BeingTypeId id = fromInt(XML::getProperty(
            petNode, "id", -1), BeingTypeId);
        if (id == BeingTypeId_negOne)
        {
            reportAlways("PET Database: PET with missing ID in %s!",
                paths.getStringValue("petsFile").c_str());
            continue;
        }

        BeingInfo *currentInfo = nullptr;
        if (mPETInfos.find(id) != mPETInfos.end())
            currentInfo = mPETInfos[id];
        if (!currentInfo)
            currentInfo = new BeingInfo;

        currentInfo->setName(XML::langProperty(petNode,
            // TRANSLATORS: unknown info name
            "name", _("pet")));

        currentInfo->setTargetSelection(XML::getBoolProperty(petNode,
            "targetSelection", true));

        BeingCommon::readBasicAttributes(currentInfo, petNode, "talk");
        BeingCommon::readWalkingAttributes(currentInfo, petNode, 0);

        currentInfo->setDeadSortOffsetY(XML::getProperty(petNode,
            "deadSortOffsetY", 31));

        const std::string returnMessage = XML::langProperty(petNode,
            // TRANSLATORS: popup menu item
            // TRANSLATORS: pet return to egg
            "removeMessage", _("Return to egg"));
        currentInfo->setString(0, returnMessage);

        SpriteDisplay display;
        for_each_xml_child_node(spriteNode, petNode)
        {
            if (!XmlHaveChildContent(spriteNode))
                continue;

            if (xmlNameEqual(spriteNode, "sprite"))
            {
                SpriteReference *const currentSprite = new SpriteReference;
                currentSprite->sprite = XmlChildContent(spriteNode);
                currentSprite->variant =
                    XML::getProperty(spriteNode, "variant", 0);
                display.sprites.push_back(currentSprite);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                std::string particlefx = XmlChildContent(spriteNode);
                display.particles.push_back(particlefx);
            }
        }

        currentInfo->setDisplay(display);

        mPETInfos[id] = currentInfo;
    }
}

void PETDB::unload()
{
    delete_all(mPETInfos);
    mPETInfos.clear();

    mLoaded = false;
}

BeingInfo *PETDB::get(const BeingTypeId id)
{
    const BeingInfoIterator i = mPETInfos.find(id);

    if (i == mPETInfos.end())
    {
        reportAlways("PETDB: Warning, unknown PET ID %d requested",
            toInt(id, int));
        return BeingInfo::unknown;
    }
    else
    {
        return i->second;
    }
}
