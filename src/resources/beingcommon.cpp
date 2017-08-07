/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#include "resources/beingcommon.h"

#include "configuration.h"
#include "logger.h"

#include "enums/resources/map/blockmask.h"

#include "utils/cast.h"

#include "resources/beinginfo.h"

#include "resources/sprite/spritereference.h"

#include "debug.h"

void BeingCommon::readBasicAttributes(BeingInfo *const info,
                                      XmlNodePtrConst node,
                                      const std::string &hoverCursor)
{
    info->setTargetCursorSize(XML::getProperty(node,
        "targetCursor", "medium"));

    info->setHoverCursor(XML::getProperty(node, "hoverCursor", hoverCursor));

    info->setTargetOffsetX(XML::getProperty(node, "targetOffsetX", 0));
    info->setTargetOffsetY(XML::getProperty(node, "targetOffsetY", 0));

    info->setNameOffsetX(XML::getProperty(node, "nameOffsetX", 0));
    info->setNameOffsetY(XML::getProperty(node, "nameOffsetY", 0));
    info->setSortOffsetY(XML::getProperty(node, "sortOffsetY", 0));

    info->setHpBarOffsetX(XML::getProperty(node, "hpBarOffsetX", 0));
    info->setHpBarOffsetY(XML::getProperty(node, "hpBarOffsetY", 0));

    info->setQuickActionEffectId(XML::getProperty(node,
        "quickActionEffect", -1));
}

void BeingCommon::readWalkingAttributes(BeingInfo *const info,
                                        XmlNodePtrConst node,
                                        const int moreBlockFlags)
{
    uint8_t block = 0;
    std::string walkStr = XML::getProperty(
        node, "walkType", "walk");

    const uint8_t allFlags = CAST_U8(
        BlockMask::GROUND |
        BlockMask::WALL |
        BlockMask::WATER |
        BlockMask::AIR |
        moreBlockFlags);
    StringVect tokens;
    splitToStringVector(tokens, walkStr, ',');
    FOR_EACH (StringVectCIter, it, tokens)
    {
        if (walkStr == "walk" || walkStr == "ground")
            block |= BlockMask::GROUND;
        else if (walkStr == "fly" || walkStr == "air")
            block |= BlockMask::GROUND | BlockMask::WATER | BlockMask::AIR;
        else if (walkStr == "all")
            block |= allFlags;
        else if (walkStr == "wall")
            block |= BlockMask::WALL;
        else if (walkStr == "monsterwall")
            block |= BlockMask::MONSTERWALL;
        else if (walkStr == "swim" || walkStr == "water")
            block |= BlockMask::WATER;
        else if (walkStr == "walkswim" || walkStr == "swimwalk")  // legacy
            block |= BlockMask::GROUND | BlockMask::WATER;
    }
    info->setBlockWalkMask(CAST_U8(block ^ allFlags));
}

void BeingCommon::readAiAttributes(BeingInfo *const info,
                                   XmlNodePtrConst node)
{
    info->setStartFollowDist(XML::getProperty(node,
        "startFollowDistance", 3));
    info->setFollowDist(XML::getProperty(node,
        "followDistance", 0));
    info->setWarpDist(XML::getProperty(node,
        "warpDistance", 11));

    info->setTargetOffsetX(XML::getProperty(node,
        "offsetX", 0));
    info->setTargetOffsetY(XML::getProperty(node,
        "offsetY", 1));
    info->setSitOffsetX(XML::getProperty(node,
        "sitOffsetX", 0));
    info->setSitOffsetY(XML::getProperty(node,
        "sitOffsetY", 1));
    info->setMoveOffsetX(XML::getProperty(node,
        "moveOffsetX", 0));
    info->setMoveOffsetY(XML::getProperty(node,
        "moveOffsetY", 1));
    info->setDeadOffsetX(XML::getProperty(node,
        "deadOffsetX", 0));
    info->setDeadOffsetY(XML::getProperty(node,
        "deadOffsetY", 1));
    info->setAttackOffsetX(XML::getProperty(node,
        "attackOffsetX", info->getTargetOffsetX()));
    info->setAttackOffsetY(XML::getProperty(node,
        "attackOffsetY", info->getTargetOffsetY()));

    info->setThinkTime(XML::getProperty(node,
        "thinkTime", 500) / 10);

    info->setDirectionType(XML::getProperty(node,
        "directionType", 1));
    info->setSitDirectionType(XML::getProperty(node,
        "sitDirectionType", 1));
    info->setDeadDirectionType(XML::getProperty(node,
        "deadDirectionType", 1));
    info->setAttackDirectionType(XML::getProperty(node,
        "attackDirectionType", 4));
}

bool BeingCommon::readObjectNodes(XmlNodePtrConst &spriteNode,
                                  SpriteDisplay &display,
                                  BeingInfo *const currentInfo,
                                  const std::string &dbName)
{
    if (xmlNameEqual(spriteNode, "sprite"))
    {
        if (!XmlHaveChildContent(spriteNode))
            return true;

        SpriteReference *const currentSprite = new SpriteReference;
        currentSprite->sprite = XmlChildContent(spriteNode);

        currentSprite->variant = XML::getProperty(
            spriteNode, "variant", 0);
        display.sprites.push_back(currentSprite);
        return true;
    }
    else if (xmlNameEqual(spriteNode, "sound"))
    {
        if (!XmlHaveChildContent(spriteNode))
            return true;

        const std::string event = XML::getProperty(
            spriteNode, "event", "");
        const int delay = XML::getProperty(
            spriteNode, "delay", 0);
        const char *const filename = XmlChildContent(spriteNode);

        if (event == "hit")
        {
            currentInfo->addSound(ItemSoundEvent::HIT, filename, delay);
        }
        else if (event == "miss")
        {
            currentInfo->addSound(ItemSoundEvent::MISS, filename, delay);
        }
        else if (event == "hurt")
        {
            currentInfo->addSound(ItemSoundEvent::HURT, filename, delay);
        }
        else if (event == "die")
        {
            currentInfo->addSound(ItemSoundEvent::DIE, filename, delay);
        }
        else if (event == "move")
        {
            currentInfo->addSound(ItemSoundEvent::MOVE, filename, delay);
        }
        else if (event == "sit")
        {
            currentInfo->addSound(ItemSoundEvent::SIT, filename, delay);
        }
        else if (event == "sittop")
        {
            currentInfo->addSound(ItemSoundEvent::SITTOP, filename, delay);
        }
        else if (event == "spawn")
        {
            currentInfo->addSound(ItemSoundEvent::SPAWN, filename, delay);
        }
        else
        {
            logger->log((dbName + ": Warning, sound effect %s for "
                "unknown event %s of monster %s").c_str(),
                filename, event.c_str(),
                currentInfo->getName().c_str());
        }
        return true;
    }
    else if (xmlNameEqual(spriteNode, "attack"))
    {
        const int attackId = XML::getProperty(spriteNode, "id", 0);
        const int effectId = XML::getProperty(spriteNode, "effect-id",
            paths.getIntValue("effectId"));
        const int hitEffectId = XML::getProperty(spriteNode, "hit-effect-id",
            paths.getIntValue("hitEffectId"));
        const int criticalHitEffectId = XML::getProperty(spriteNode,
            "critical-hit-effect-id",
            paths.getIntValue("criticalHitEffectId"));
        const int missEffectId = XML::getProperty(spriteNode, "miss-effect-id",
            paths.getIntValue("missEffectId"));

        const std::string spriteAction = XML::getProperty(spriteNode, "action",
            "attack");
        const std::string skySpriteAction = XML::getProperty(spriteNode,
            "skyaction", "skyattack");
        const std::string waterSpriteAction = XML::getProperty(spriteNode,
            "wateraction", "waterattack");
        const std::string rideSpriteAction = XML::getProperty(spriteNode,
            "rideaction", "rideattack");

        const std::string missileParticle = XML::getProperty(spriteNode,
            "missile-particle", "");

        const float missileZ = static_cast<float>(XML::getFloatProperty(
            spriteNode, "missile-z", 32.0f));
        const int missileLifeTime = static_cast<float>(XML::getProperty(
            spriteNode, "missile-lifetime", 500));
        const float missileSpeed = static_cast<float>(XML::getFloatProperty(
            spriteNode, "missile-speed", 7.0f));
        const float missileDieDistance = static_cast<float>(
            XML::getFloatProperty(spriteNode, "missile-diedistance", 8.0f));

        currentInfo->addAttack(attackId,
            spriteAction,
            skySpriteAction,
            waterSpriteAction,
            rideSpriteAction,
            effectId,
            hitEffectId,
            criticalHitEffectId,
            missEffectId,
            missileParticle,
            missileZ,
            missileSpeed,
            missileDieDistance,
            missileLifeTime);
        return true;
    }
    else if (xmlNameEqual(spriteNode, "particlefx"))
    {
        if (!XmlHaveChildContent(spriteNode))
            return true;

        display.particles.push_back(XmlChildContent(spriteNode));
        return true;
    }
    return false;
}
