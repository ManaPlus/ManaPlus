/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "resources/monsterdb.h"

#include "logger.h"

#include "net/net.h"

#include "resources/beinginfo.h"

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

    XML::Document doc("monsters.xml");
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "monsters"))
    {
        logger->log("Monster Database: Error while loading monster.xml!");
        mLoaded = true;
        return;
    }

#ifdef MANASERV_SUPPORT
    const int offset = XML::getProperty(rootNode, "offset",
        Net::getNetworkType() != ServerInfo::MANASERV
        ? OLD_TMWATHENA_OFFSET : 0);
#else
    const int offset = XML::getProperty(rootNode,
        "offset", OLD_TMWATHENA_OFFSET);
#endif

    //iterate <monster>s
    for_each_xml_child_node(monsterNode, rootNode)
    {
        if (!xmlNameEqual(monsterNode, "monster"))
            continue;

        BeingInfo *const currentInfo = new BeingInfo;

        currentInfo->setWalkMask(Map::BLOCKMASK_WALL
            | Map::BLOCKMASK_CHARACTER | Map::BLOCKMASK_MONSTER);
        currentInfo->setBlockType(Map::BLOCKTYPE_MONSTER);

        currentInfo->setName(XML::langProperty(
            monsterNode, "name", _("unnamed")));

        currentInfo->setTargetCursorSize(XML::getProperty(monsterNode,
            "targetCursor", "medium"));

        currentInfo->setTargetOffsetX(XML::getProperty(monsterNode,
            "targetOffsetX", 0));

        currentInfo->setTargetOffsetY(XML::getProperty(monsterNode,
            "targetOffsetY", 0));

        currentInfo->setMaxHP(XML::getProperty(monsterNode, "maxHP", 0));

        currentInfo->setSortOffsetY(XML::getProperty(
            monsterNode, "sortOffsetY", 0));

        currentInfo->setDeadSortOffsetY(XML::getProperty(
            monsterNode, "deadSortOffsetY", 31));

        unsigned char block = 0;
        std::string walkStr = XML::getProperty(
            monsterNode, "walkType", "walk");
        if (walkStr == "walk")
            block = Map::BLOCKMASK_WATER | Map::BLOCKMASK_AIR;
        else if (walkStr == "fly")
            block = 0;
        else if (walkStr == "swim")
            block = Map::BLOCKMASK_GROUND | Map::BLOCKMASK_AIR;
        else if (walkStr == "walkswim" || walkStr == "swimwalk")
            block = Map::BLOCKMASK_AIR;

        currentInfo->setWalkMask(static_cast<unsigned char>(
            Map::BLOCKMASK_WALL | block));

        if (currentInfo->getMaxHP())
            currentInfo->setStaticMaxHP(true);

        SpriteDisplay display;

        //iterate <sprite>s and <sound>s
        for_each_xml_child_node(spriteNode, monsterNode)
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
            else if (xmlNameEqual(spriteNode, "sound"))
            {
                if (!spriteNode->xmlChildrenNode)
                    continue;

                std::string event = XML::getProperty(spriteNode, "event", "");
                const char *filename;
                filename = reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content);

                if (event == "hit")
                {
                    currentInfo->addSound(SOUND_EVENT_HIT, filename);
                }
                else if (event == "miss")
                {
                    currentInfo->addSound(SOUND_EVENT_MISS, filename);
                }
                else if (event == "hurt")
                {
                    currentInfo->addSound(SOUND_EVENT_HURT, filename);
                }
                else if (event == "die")
                {
                    currentInfo->addSound(SOUND_EVENT_DIE, filename);
                }
                else if (event == "move")
                {
                    currentInfo->addSound(SOUND_EVENT_MOVE, filename);
                }
                else if (event == "sit")
                {
                    currentInfo->addSound(SOUND_EVENT_SIT, filename);
                }
                else if (event == "sittop")
                {
                    currentInfo->addSound(SOUND_EVENT_SITTOP, filename);
                }
                else if (event == "spawn")
                {
                    currentInfo->addSound(SOUND_EVENT_SPAWN, filename);
                }
                else
                {
                    logger->log("MonsterDB: Warning, sound effect %s for "
                                "unknown event %s of monster %s",
                                filename, event.c_str(),
                                currentInfo->getName().c_str());
                }
            }
            else if (xmlNameEqual(spriteNode, "attack"))
            {
                const int id = XML::getProperty(spriteNode, "id", 0);
                const std::string particleEffect = XML::getProperty(
                        spriteNode, "particle-effect", "");
                const std::string spriteAction = XML::getProperty(spriteNode,
                                                                  "action",
                                                                  "attack");
                const std::string missileParticle = XML::getProperty(
                        spriteNode, "missile-particle", "");
                currentInfo->addAttack(id, spriteAction,
                                       particleEffect, missileParticle);
            }
            else if (xmlNameEqual(spriteNode, "particlefx"))
            {
                if (!spriteNode->xmlChildrenNode)
                    continue;

                display.particles.push_back(reinterpret_cast<const char*>(
                    spriteNode->xmlChildrenNode->content));
            }
        }
        currentInfo->setDisplay(display);

        mMonsterInfos[XML::getProperty(
            monsterNode, "id", 0) + offset] = currentInfo;
    }

    mLoaded = true;
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
        i = mMonsterInfos.find(id + /*1002*/ OLD_TMWATHENA_OFFSET);
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
