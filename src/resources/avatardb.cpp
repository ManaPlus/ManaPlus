/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "resources/avatardb.h"

#include "logger.h"

#include "net/net.h"

#include "resources/beinginfo.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "configuration.h"

#include "debug.h"

namespace
{
    BeingInfos mAvatarInfos;
    bool mLoaded = false;
}

void AvatarDB::load()
{
    if (mLoaded)
        unload();

    XML::Document doc("avatars.xml");
    const XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "avatars"))
    {
        logger->log1("Avatars Database: Error while loading avatars.xml!");
        mLoaded = true;
        return;
    }

    for_each_xml_child_node(avatarNode, rootNode)
    {
        if (!xmlNameEqual(avatarNode, "avatar"))
            continue;

        BeingInfo *const currentInfo = new BeingInfo;

        currentInfo->setName(XML::langProperty(
            // TRANSLATORS: unknown info name
            avatarNode, "name", _("unnamed")));

        currentInfo->setTargetOffsetX(XML::getProperty(avatarNode,
            "targetOffsetX", 0));

        currentInfo->setTargetOffsetY(XML::getProperty(avatarNode,
            "targetOffsetY", 0));

        SpriteDisplay display;

        // iterate <sprite>s and <sound>s
        for_each_xml_child_node(spriteNode, avatarNode)
        {
            if (xmlNameEqual(spriteNode, "sprite"))
            {
                if (!spriteNode->xmlChildrenNode)
                    continue;

                SpriteReference *const currentSprite = new SpriteReference;
                currentSprite->sprite = reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content);

                currentSprite->variant = XML::getProperty(
                    spriteNode, "variant", 0);
                display.sprites.push_back(currentSprite);
            }
        }
        currentInfo->setDisplay(display);

        mAvatarInfos[XML::getProperty(avatarNode, "id", 0)] = currentInfo;
    }

    mLoaded = true;
}

void AvatarDB::unload()
{
    delete_all(mAvatarInfos);
    mAvatarInfos.clear();
    mLoaded = false;
}

BeingInfo *AvatarDB::get(const int id)
{
    BeingInfoIterator i = mAvatarInfos.find(id);
    if (i == mAvatarInfos.end())
        return BeingInfo::unknown;
    else
        return i->second;
}
