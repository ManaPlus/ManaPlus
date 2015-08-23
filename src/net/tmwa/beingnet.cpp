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

#include "net/tmwa/beingnet.h"

#include "actormanager.h"
#include "effectmanager.h"
#include "guild.h"
#include "party.h"

#include "being/localplayer.h"

#include "particle/particle.h"

#include "input/keyboardconfig.h"

#include "gui/windows/outfitwindow.h"
#include "gui/windows/socialwindow.h"

#include "net/serverfeatures.h"

#include "net/ea/beingnet.h"

#include "net/tmwa/guildmanager.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"
#include "net/tmwa/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/stringutils.h"
#include "utils/timer.h"

#include "debug.h"

extern int serverVersion;

namespace TmwAthena
{

void BeingNet::processBeingChangeLook(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingChangeLook")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processBeingChangeLook")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    const uint8_t type = msg.readUInt8("type");
    const int16_t id = static_cast<int16_t>(msg.readUInt8("id"));
    const int id2 = 1;

    if (!localPlayer || !dstBeing)
    {
        BLOCK_END("BeingNet::processBeingChangeLook")
        return;
    }
    processBeingChangeLookContinue(msg, dstBeing, type, id, id2);
    BLOCK_END("BeingNet::processBeingChangeLook")
}

void BeingNet::processBeingChangeLook2(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingChangeLook")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processBeingChangeLook")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    const uint8_t type = msg.readUInt8("type");
    int id2 = 0;

    const int16_t id = msg.readInt16("id1");
    if (type == 2)
    {
        id2 = msg.readInt16("id2");
    }
    else
    {
        msg.readInt16("id2");
        id2 = 1;
    }

    if (!localPlayer || !dstBeing)
    {
        BLOCK_END("BeingNet::processBeingChangeLook")
        return;
    }
    processBeingChangeLookContinue(msg, dstBeing, type, id, id2);
    BLOCK_END("BeingNet::processBeingChangeLook")
}

void BeingNet::processBeingChangeLookContinue(Net::MessageIn &msg,
                                           Being *const dstBeing,
                                           const uint8_t type,
                                           const int id,
                                           const int id2)
{
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setOtherTime();

    const std::string color;
    switch (type)
    {
        case 0:     // change race
            dstBeing->setSubtype(fromInt(id, BeingTypeId),
                dstBeing->getLook());
            break;
        case 1:     // eAthena LOOK_HAIR
        {
            const uint16_t look = static_cast<uint16_t>(id / 256);
            const int hair = id % 256;
            dstBeing->setHairStyle(SPRITE_HAIR_COLOR, hair * -1);
            dstBeing->setLook(look);
            break;
        }
        case 2:     // Weapon ID in id, Shield ID in id2
            dstBeing->setSprite(SPRITE_BODY, id, "", ItemColor_one, true);
            dstBeing->setSprite(SPRITE_FLOOR, id2);
            localPlayer->imitateOutfit(dstBeing, SPRITE_FLOOR);
            break;
        case 3:     // Change lower headgear for eAthena, pants for us
            dstBeing->setSprite(SPRITE_WEAPON, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_WEAPON);
            break;
        case 4:     // Change upper headgear for eAthena, hat for us
            dstBeing->setSprite(SPRITE_CLOTHES_COLOR, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_CLOTHES_COLOR);
            break;
        case 5:     // Change middle headgear for eathena, armor for us
            dstBeing->setSprite(SPRITE_HEAD_BOTTOM, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_HEAD_BOTTOM);
            break;
        case 6:     // eAthena LOOK_HAIR_COLOR
            dstBeing->setHairColor(SPRITE_HAIR_COLOR,
                fromInt(id, ItemColor));
            break;
        case 7:     // Clothes color
            // ignoring it
            break;
        case 8:     // eAthena LOOK_SHIELD
            dstBeing->setSprite(SPRITE_FLOOR, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_FLOOR);
            break;
        case 9:     // eAthena LOOK_SHOES
            dstBeing->setSprite(SPRITE_HAIR, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_HAIR);
            break;
        case 10:   // LOOK_GLOVES
            dstBeing->setSprite(SPRITE_SHOES, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHOES);
            break;
        case 11:  // LOOK_CAPE
            dstBeing->setSprite(SPRITE_SHIELD, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHIELD);
            break;
        case 12:
            dstBeing->setSprite(SPRITE_HEAD_TOP, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_HEAD_TOP);
            break;
        case 13:
            dstBeing->setSprite(SPRITE_HEAD_MID, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_HEAD_MID);
            break;
        case 14:
            dstBeing->setSprite(SPRITE_ROBE, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_ROBE);
            break;
        case 15:
            dstBeing->setSprite(SPRITE_EVOL2, id, color,
                ItemColor_one);
            localPlayer->imitateOutfit(dstBeing, SPRITE_EVOL2);
            break;
        case 16:
            dstBeing->setLook(static_cast<uint16_t>(id));
            break;
        default:
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void BeingNet::processPlayerUpdate1(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processPlayerMoveUpdate")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingNet::processPlayerMoveUpdate")
        return;
    }

    // An update about a player, potentially including movement.
    const BeingId id = msg.readBeingId("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    uint32_t statusEffects = msg.readInt16("opt2");
    statusEffects |= (static_cast<uint32_t>(msg.readInt16("options")))
        << 16;
    const int16_t job = msg.readInt16("job");
    int disguiseId = 0;
    if (toInt(id, int) < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
    {
        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingNet::processPlayerMoveUpdate")
            return;
        }

        dstBeing = Ea::BeingNet::createBeing(id, job);

        if (!dstBeing)
        {
            BLOCK_END("BeingNet::processPlayerMoveUpdate")
            return;
        }
    }
    else if (disguiseId)
    {
        actorManager->undelete(dstBeing);
        if (serverVersion < 1)
            beingHandler->requestNameById(id);
    }

    uint8_t dir = dstBeing->getDirectionDelayed();
    if (dir)
    {
        if (dir != dstBeing->getDirection())
            dstBeing->setDirection(dir);
    }

    if (Party *const party = localPlayer->getParty())
    {
        if (party->isMember(id))
            dstBeing->setParty(party);
    }

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));

    const uint8_t hairStyle = msg.readUInt8("hair style");
    const uint16_t look = msg.readUInt8("look");
    dstBeing->setSubtype(fromInt(job, BeingTypeId), look);
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headBottom = msg.readInt16("head bottom");

    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const ItemColor hairColor = fromInt(
        msg.readUInt8("hair color"), ItemColor);
    msg.readUInt8("unused");
    msg.readInt32("unused");

    const int guild = msg.readInt32("guild");

    if (!guildManager || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(static_cast<int16_t>(guild)));
    }

    msg.readInt16("emblem");
    dstBeing->setManner(msg.readInt16("manner"));
    dstBeing->setStatusEffectBlock(32, msg.readInt16("opt3"));
    dstBeing->setKarma(msg.readUInt8("karma"));
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(
        static_cast<uint8_t>(msg.readUInt8("gender") & 3)));

    if (!disguiseId)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_BODY, weapon, "", ItemColor_one, true);
        dstBeing->updateSprite(SPRITE_FLOOR, shield);
        dstBeing->updateSprite(SPRITE_WEAPON, headBottom);
        dstBeing->updateSprite(SPRITE_HEAD_BOTTOM, headMid);
        dstBeing->updateSprite(SPRITE_CLOTHES_COLOR, headTop);
        dstBeing->updateSprite(SPRITE_HAIR_COLOR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        dstBeing->setHairColor(hairColor);
    }
    localPlayer->imitateOutfit(dstBeing);

    uint16_t x, y;
    msg.readCoordinates(x, y, dir, "position");
    dstBeing->setTileCoords(x, y);
    dstBeing->setDirection(dir);

    localPlayer->imitateDirection(dstBeing, dir);

    const uint16_t gmstatus = msg.readInt16("gm status");

    if (gmstatus & 0x80)
        dstBeing->setGM(true);

    applyPlayerAction(msg, dstBeing, msg.readUInt8("action type"));
    const int level = static_cast<int>(msg.readUInt8("level"));
    if (level)
        dstBeing->setLevel(level);

    msg.readUInt8("unused");

    dstBeing->setActionTime(tick_time);

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));

    BLOCK_END("BeingNet::processPlayerMoveUpdate")
}

void BeingNet::processPlayerUpdate2(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processPlayerMoveUpdate")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingNet::processPlayerMoveUpdate")
        return;
    }

    // An update about a player, potentially including movement.
    const BeingId id = msg.readBeingId("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    uint32_t statusEffects = msg.readInt16("opt2");
    statusEffects |= (static_cast<uint32_t>(msg.readInt16("options")))
        << 16;
    const int16_t job = msg.readInt16("job");
    int disguiseId = 0;
    if (toInt(id, int) < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
    {
        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingNet::processPlayerMoveUpdate")
            return;
        }

        dstBeing = Ea::BeingNet::createBeing(id, job);

        if (!dstBeing)
        {
            BLOCK_END("BeingNet::processPlayerMoveUpdate")
            return;
        }
    }
    else if (disguiseId)
    {
        actorManager->undelete(dstBeing);
        if (serverVersion < 1)
            beingHandler->requestNameById(id);
    }

    uint8_t dir = dstBeing->getDirectionDelayed();
    if (dir)
    {
        if (dir != dstBeing->getDirection())
            dstBeing->setDirection(dir);
    }

    if (Party *const party = localPlayer->getParty())
    {
        if (party->isMember(id))
            dstBeing->setParty(party);
    }

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));

    const uint8_t hairStyle = msg.readUInt8("hair style");
    const uint16_t look = msg.readUInt8("look");
    dstBeing->setSubtype(fromInt(job, BeingTypeId), look);
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headBottom = msg.readInt16("head bottom");
    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const ItemColor hairColor = fromInt(
        msg.readUInt8("hair color"), ItemColor);
    msg.readUInt8("unused");
    msg.readInt32("unused");

    const int guild = msg.readInt32("guild");

    if (!guildManager || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(static_cast<int16_t>(guild)));
    }

    msg.readInt16("emblem");
    dstBeing->setManner(msg.readInt16("manner"));
    dstBeing->setStatusEffectBlock(32, msg.readInt16("opt3"));
    dstBeing->setKarma(msg.readUInt8("karma"));
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(
        static_cast<uint8_t>(msg.readUInt8("gender") & 3)));

    if (!disguiseId)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_BODY, weapon, "", ItemColor_one, true);
        dstBeing->updateSprite(SPRITE_FLOOR, shield);
        dstBeing->updateSprite(SPRITE_WEAPON, headBottom);
        dstBeing->updateSprite(SPRITE_HEAD_BOTTOM, headMid);
        dstBeing->updateSprite(SPRITE_CLOTHES_COLOR, headTop);
        dstBeing->updateSprite(SPRITE_HAIR_COLOR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        dstBeing->setHairColor(hairColor);
    }
    localPlayer->imitateOutfit(dstBeing);

    uint16_t x, y;
    msg.readCoordinates(x, y, dir, "position");
    dstBeing->setTileCoords(x, y);
    dstBeing->setDirection(dir);

    localPlayer->imitateDirection(dstBeing, dir);

    const uint16_t gmstatus = msg.readInt16("gm status");

    if (gmstatus & 0x80)
        dstBeing->setGM(true);

    applyPlayerAction(msg, dstBeing, msg.readUInt8("action type"));
    const int level = static_cast<int>(msg.readUInt8("level"));
    if (level)
        dstBeing->setLevel(level);

    dstBeing->setActionTime(tick_time);
    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));

    BLOCK_END("BeingNet::processPlayerMoveUpdate")
}

void BeingNet::processPlayerMove(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processPlayerMoveUpdate")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingNet::processPlayerMoveUpdate")
        return;
    }

    // An update about a player, potentially including movement.
    const BeingId id = msg.readBeingId("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    uint32_t statusEffects = msg.readInt16("opt2");
    statusEffects |= (static_cast<uint32_t>(msg.readInt16("options")))
        << 16;
    const int16_t job = msg.readInt16("job");
    int disguiseId = 0;
    if (toInt(id, int) < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
    {
        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingNet::processPlayerMoveUpdate")
            return;
        }

        dstBeing = Ea::BeingNet::createBeing(id, job);

        if (!dstBeing)
        {
            BLOCK_END("BeingNet::processPlayerMoveUpdate")
            return;
        }
    }
    else if (disguiseId)
    {
        actorManager->undelete(dstBeing);
        if (serverVersion < 1)
            beingHandler->requestNameById(id);
    }

    const uint8_t dir = dstBeing->getDirectionDelayed();
    if (dir)
    {
        if (dir != dstBeing->getDirection())
            dstBeing->setDirection(dir);
    }

    if (Party *const party = localPlayer->getParty())
    {
        if (party->isMember(id))
            dstBeing->setParty(party);
    }

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));

    const uint8_t hairStyle = msg.readUInt8("hair style");
    const uint16_t look = msg.readUInt8("look");
    dstBeing->setSubtype(fromInt(job, BeingTypeId), look);
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headBottom = msg.readInt16("head bottom");

    msg.readInt32("tick");

    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const ItemColor hairColor = fromInt(
        msg.readUInt8("hair color"), ItemColor);
    msg.readUInt8("unused");
    msg.readInt32("unused");

    const int guild = msg.readInt32("guild");

    if (!guildManager || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(static_cast<int16_t>(guild)));
    }

    msg.readInt16("emblem");
    dstBeing->setManner(msg.readInt16("manner"));
    dstBeing->setStatusEffectBlock(32, msg.readInt16("opt3"));
    dstBeing->setKarma(msg.readUInt8("karma"));
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(
        static_cast<uint8_t>(msg.readUInt8("gender") & 3)));

    if (!disguiseId)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_BODY, weapon, "", ItemColor_one, true);
        dstBeing->updateSprite(SPRITE_FLOOR, shield);
        dstBeing->updateSprite(SPRITE_WEAPON, headBottom);
        dstBeing->updateSprite(SPRITE_HEAD_BOTTOM, headMid);
        dstBeing->updateSprite(SPRITE_CLOTHES_COLOR, headTop);
        dstBeing->updateSprite(SPRITE_HAIR_COLOR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        dstBeing->setHairColor(hairColor);
    }
    localPlayer->imitateOutfit(dstBeing);

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "moving path");

    localPlayer->followMoveTo(dstBeing, srcX, srcY, dstX, dstY);

    dstBeing->setTileCoords(srcX, srcY);
    dstBeing->setDestination(dstX, dstY);

    // because server don't send direction in move packet,
    // we fixing it

    if (srcX != dstX || srcY != dstY)
    {
        const int d = dstBeing->calcDirection(dstX, dstY);

        if (d && dstBeing->getDirection() != d)
            dstBeing->setDirectionDelayed(static_cast<uint8_t>(d));
    }

    if (localPlayer->getCurrentAction() != BeingAction::STAND)
        localPlayer->imitateAction(dstBeing, BeingAction::STAND);
    if (localPlayer->getDirection() != dstBeing->getDirection())
    {
        localPlayer->imitateDirection(dstBeing,
            dstBeing->getDirection());
    }

    const uint16_t gmstatus = msg.readInt16("gm status");

    if (gmstatus & 0x80)
        dstBeing->setGM(true);

    msg.readUInt8("unused");

    const int level = static_cast<int>(msg.readUInt8("level"));
    if (level)
        dstBeing->setLevel(level);

    msg.readUInt8("unused");

    if (dstBeing->getType() != ActorType::Player)
        dstBeing->setActionTime(tick_time);

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();
    BLOCK_END("BeingNet::processPlayerMoveUpdate")
}

void BeingNet::processBeingVisible(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingVisibleOrMove")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processBeingVisibleOrMove")
        return;
    }

    BeingId spawnId;

    // Information about a being in range
    const BeingId id = msg.readBeingId("being id");
    if (id == Ea::BeingNet::mSpawnId)
        spawnId = Ea::BeingNet::mSpawnId;
    else
        spawnId = BeingId_zero;
    Ea::BeingNet::mSpawnId = BeingId_zero;
    int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    uint32_t statusEffects = msg.readInt16("opt2");
    statusEffects |= (static_cast<uint32_t>(msg.readInt16("option"))) << 16;
    const int16_t job = msg.readInt16("class");
    int disguiseId = 0;
    if (id == localPlayer->getId() && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);

    if (dstBeing && dstBeing->getType() == ActorType::Monster
        && !dstBeing->isAlive())
    {
        actorManager->destroy(dstBeing);
        actorManager->erase(dstBeing);
        dstBeing = nullptr;
    }

    if (!dstBeing)
    {
        // Being with id >= 110000000 and job 0 are better
        // known as ghosts, so don't create those.
        if (job == 0 && toInt(id, int) >= 110000000)
        {
            BLOCK_END("BeingNet::processBeingVisibleOrMove")
            return;
        }

        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingNet::processBeingVisibleOrMove")
            return;
        }

        dstBeing = Ea::BeingNet::createBeing(id, job);

        if (!dstBeing)
        {
            BLOCK_END("BeingNet::processBeingVisibleOrMove")
            return;
        }
    }
    else
    {
        if (dstBeing->getType() == ActorType::Npc)
        {
            actorManager->undelete(dstBeing);
            if (serverVersion < 1)
                beingHandler->requestNameById(id);
        }
    }

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();

    if (spawnId != BeingId_zero)
    {
        dstBeing->setAction(BeingAction::SPAWN, 0);
    }
    else
    {
        dstBeing->clearPath();
        dstBeing->setActionTime(tick_time);
        dstBeing->setAction(BeingAction::STAND, 0);
    }

    // Prevent division by 0 when calculating frame
    if (speed == 0)
        speed = 150;

    const uint8_t hairStyle = msg.readUInt8("hair style");
    const uint16_t look = msg.readUInt8("look");
    dstBeing->setSubtype(fromInt(job, BeingTypeId), look);
    if (dstBeing->getType() == ActorType::Monster && localPlayer)
        localPlayer->checkNewName(dstBeing);
    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t headBottom = msg.readInt16("head bottom");

    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const ItemColor hairColor = fromInt(msg.readUInt8("hair color"), ItemColor);
    msg.readUInt8("unused");
    const uint16_t shoes = msg.readInt16("shoes / clothes color");

    uint16_t gloves;
    if (dstBeing->getType() == ActorType::Monster)
    {
        if (serverFeatures->haveServerHp())
        {
            const int hp = msg.readInt32("hp");
            const int maxHP = msg.readInt32("max hp");
            if (hp && maxHP)
            {
                dstBeing->setMaxHP(maxHP);
                const int oldHP = dstBeing->getHP();
                if (!oldHP || oldHP > hp)
                    dstBeing->setHP(hp);
            }
        }
        else
        {
            msg.readInt32("unused");
            msg.readInt32("unused");
        }
        gloves = 0;
    }
    else
    {
        gloves = msg.readInt16("gloves / head dir");
        msg.readInt32("guild");
        msg.readInt16("guild emblem");
    }

    dstBeing->setManner(msg.readInt16("manner"));
    dstBeing->setStatusEffectBlock(32, msg.readInt16("opt3"));
    if (serverFeatures->haveMonsterAttackRange()
        && dstBeing->getType() == ActorType::Monster)
    {
        const int attackRange = static_cast<int>(
            msg.readUInt8("attack range (was karma)"));
        dstBeing->setAttackRange(attackRange);
    }
    else
    {
        dstBeing->setKarma(msg.readUInt8("karma"));
    }
    uint8_t gender = msg.readUInt8("gender");

    if (!disguiseId && dstBeing->getType() == ActorType::Player)
    {
        // reserving bits for future usage
        gender &= 3;
        dstBeing->setGender(Being::intToGender(gender));
        // Set these after the gender, as the sprites may be gender-specific
        Ea::BeingNet::setSprite(dstBeing, SPRITE_HAIR_COLOR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        dstBeing->setHairColor(hairColor);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_WEAPON, headBottom);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_HEAD_BOTTOM, headMid);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_CLOTHES_COLOR, headTop);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_HAIR, shoes);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_SHOES, gloves);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_BODY, weapon, "",
            ItemColor_one, true);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_FLOOR, shield);
    }
    else if (dstBeing->getType() == ActorType::Npc
             && serverFeatures->haveNpcGender())
    {
        setServerGender(dstBeing, gender);
    }

    uint8_t dir;
    uint16_t x, y;
    msg.readCoordinates(x, y, dir, "position");
    dstBeing->setTileCoords(x, y);

    if (job == 45 && socialWindow && outfitWindow)
    {
        const int num = socialWindow->getPortalIndex(x, y);
        if (num >= 0)
        {
            dstBeing->setName(keyboard.getKeyShortString(
                outfitWindow->keyName(num)));
        }
        else
        {
            dstBeing->setName("");
        }
    }

    dstBeing->setDirection(dir);

    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));
    BLOCK_END("BeingNet::processBeingVisibleOrMove")
}

void BeingNet::processBeingMove(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingVisibleOrMove")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processBeingVisibleOrMove")
        return;
    }

    BeingId spawnId;

    // Information about a being in range
    const BeingId id = msg.readBeingId("being id");
    if (id == Ea::BeingNet::mSpawnId)
        spawnId = Ea::BeingNet::mSpawnId;
    else
        spawnId = BeingId_zero;
    Ea::BeingNet::mSpawnId = BeingId_zero;
    int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    uint32_t statusEffects = msg.readInt16("opt2");
    statusEffects |= (static_cast<uint32_t>(msg.readInt16("option"))) << 16;
    const int16_t job = msg.readInt16("class");
    int disguiseId = 0;
    if (id == localPlayer->getId() && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);

    if (dstBeing && dstBeing->getType() == ActorType::Monster
        && !dstBeing->isAlive())
    {
        actorManager->destroy(dstBeing);
        actorManager->erase(dstBeing);
        dstBeing = nullptr;
    }

    if (!dstBeing)
    {
        // Being with id >= 110000000 and job 0 are better
        // known as ghosts, so don't create those.
        if (job == 0 && toInt(id, int) >= 110000000)
        {
            BLOCK_END("BeingNet::processBeingVisibleOrMove")
            return;
        }

        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingNet::processBeingVisibleOrMove")
            return;
        }

        dstBeing = Ea::BeingNet::createBeing(id, job);

        if (!dstBeing)
        {
            BLOCK_END("BeingNet::processBeingVisibleOrMove")
            return;
        }
    }
    else
    {
        if (dstBeing->getType() == ActorType::Npc)
        {
            actorManager->undelete(dstBeing);
            if (serverVersion < 1)
                beingHandler->requestNameById(id);
        }
    }

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();

    if (spawnId != BeingId_zero)
        dstBeing->setAction(BeingAction::SPAWN, 0);

    // Prevent division by 0 when calculating frame
    if (speed == 0)
        speed = 150;

    const uint8_t hairStyle = msg.readUInt8("hair style");
    const uint16_t look = msg.readUInt8("look");
    dstBeing->setSubtype(fromInt(job, BeingTypeId), look);
    if (dstBeing->getType() == ActorType::Monster && localPlayer)
        localPlayer->checkNewName(dstBeing);
    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t headBottom = msg.readInt16("head bottom");

    msg.readInt32("tick");

    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const ItemColor hairColor = fromInt(
        msg.readUInt8("hair color"), ItemColor);
    msg.readUInt8("unused");
    const uint16_t shoes = msg.readInt16("shoes / clothes color");

    uint16_t gloves;
    if (dstBeing->getType() == ActorType::Monster)
    {
        if (serverFeatures->haveServerHp())
        {
            const int hp = msg.readInt32("hp");
            const int maxHP = msg.readInt32("max hp");
            if (hp && maxHP)
            {
                dstBeing->setMaxHP(maxHP);
                const int oldHP = dstBeing->getHP();
                if (!oldHP || oldHP > hp)
                    dstBeing->setHP(hp);
            }
        }
        else
        {
            msg.readInt32("unused");
            msg.readInt32("unused");
        }
        gloves = 0;
    }
    else
    {
        gloves = msg.readInt16("gloves / head dir");
        msg.readInt32("guild");
        msg.readInt16("guild emblem");
    }

    dstBeing->setManner(msg.readInt16("manner"));
    dstBeing->setStatusEffectBlock(32, msg.readInt16("opt3"));
    if (serverFeatures->haveMonsterAttackRange()
        && dstBeing->getType() == ActorType::Monster)
    {
        const int attackRange = static_cast<int>(
            msg.readUInt8("attack range (was karma)"));
        dstBeing->setAttackRange(attackRange);
    }
    else
    {
        dstBeing->setKarma(msg.readUInt8("karma"));
    }
    uint8_t gender = msg.readUInt8("gender");

    if (!disguiseId && dstBeing->getType() == ActorType::Player)
    {
        // reserving bits for future usage
        gender &= 3;
        dstBeing->setGender(Being::intToGender(gender));
        // Set these after the gender, as the sprites may be gender-specific
        Ea::BeingNet::setSprite(dstBeing, SPRITE_HAIR_COLOR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        dstBeing->setHairColor(hairColor);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_WEAPON, headBottom);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_HEAD_BOTTOM, headMid);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_CLOTHES_COLOR, headTop);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_HAIR, shoes);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_SHOES, gloves);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_BODY, weapon, "",
            ItemColor_one, true);
        Ea::BeingNet::setSprite(dstBeing, SPRITE_FLOOR, shield);
    }
    else if (dstBeing->getType() == ActorType::Npc
             && serverFeatures->haveNpcGender())
    {
        setServerGender(dstBeing, gender);
    }

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    if (!disguiseId)
    {
        dstBeing->setAction(BeingAction::STAND, 0);
        dstBeing->setTileCoords(srcX, srcY);
        if (!serverFeatures->haveMove3())
            dstBeing->setDestination(dstX, dstY);
    }

    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));
    BLOCK_END("BeingNet::processBeingVisibleOrMove")
}

void BeingNet::processBeingSpawn(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingSpawn")
    // skipping this packet
    Ea::BeingNet::mSpawnId = msg.readBeingId("being id");
    msg.readInt16("speed");
    msg.readInt16("opt1");
    msg.readInt16("opt2");
    msg.readInt16("option");
    msg.readInt16("disguise");
    msg.skip(25, "unused");
    BLOCK_END("BeingNet::processBeingSpawn")
}

void BeingNet::processSkillCasting(Net::MessageIn &msg)
{
    msg.readInt32("src id");
    msg.readInt32("dst id");
    msg.readInt16("dst x");
    msg.readInt16("dst y");
    msg.readInt16("skill num");
    msg.readInt32("skill get p1");
    msg.readInt32("cast time");
}

void BeingNet::processBeingStatusChange(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingStatusChange")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processBeingStatusChange")
        return;
    }

    // Status change
    const uint16_t status = msg.readInt16("status");
    const BeingId id = msg.readBeingId("being id");
    const Enable flag = fromBool(
        msg.readUInt8("flag: 0: stop, 1: start"), Enable);

    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing)
        dstBeing->setStatusEffect(status, flag);
    BLOCK_END("BeingNet::processBeingStatusChange")
}

void BeingNet::processBeingMove2(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingMove2")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processBeingMove2")
        return;
    }

    /*
      * A simplified movement packet, used by the
      * later versions of eAthena for both mobs and
      * players
      */
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    /*
      * This packet doesn't have enough info to actually
      * create a new being, so if the being isn't found,
      * we'll just pretend the packet didn't happen
      */

    if (!dstBeing)
    {
        BLOCK_END("BeingNet::processBeingMove2")
        return;
    }

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    msg.readInt32("tick");

    dstBeing->setAction(BeingAction::STAND, 0);
    dstBeing->setTileCoords(srcX, srcY);
    dstBeing->setDestination(dstX, dstY);
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();
    BLOCK_END("BeingNet::processBeingMove2")
}

void BeingNet::processBeingChangeDirection(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingChangeDirection")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processBeingChangeDirection")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    if (!dstBeing)
    {
        msg.readInt16("unused");
        msg.readUInt8("direction");
        BLOCK_END("BeingNet::processBeingChangeDirection");
        return;
    }

    msg.readInt16("unused");

    const uint8_t dir = Net::MessageIn::fromServerDirection(
        static_cast<uint8_t>(msg.readUInt8("direction") & 0x0FU));
    dstBeing->setDirection(dir);
    if (localPlayer)
        localPlayer->imitateDirection(dstBeing, dir);
    BLOCK_END("BeingNet::processBeingChangeDirection")
}

void BeingNet::processPlaterStatusChange(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processPlayerStop")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processPlayerStop")
        return;
    }

    // Change in players' flags
    const BeingId id = msg.readBeingId("account id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    const uint16_t stunMode = msg.readInt16("stun mode");
    uint32_t statusEffects = msg.readInt16("status effect");
    statusEffects |= (static_cast<uint32_t>(msg.readInt16("opt?"))) << 16;
    msg.readUInt8("Unused?");

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));
    BLOCK_END("BeingNet::processPlayerStop")
}

void BeingNet::processBeingResurrect(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingResurrect")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingNet::processBeingResurrect")
        return;
    }

    // A being changed mortality status

    const BeingId id = msg.readBeingId("being id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
    {
        BLOCK_END("BeingNet::processBeingResurrect")
        return;
    }

    // If this is player's current target, clear it.
    if (dstBeing == localPlayer->getTarget())
        localPlayer->stopAttack();

    if (msg.readUInt8("flag?") == 1U)
        dstBeing->setAction(BeingAction::STAND, 0);
    BLOCK_END("BeingNet::processBeingResurrect")
}

void BeingNet::processPlayerGuilPartyInfo(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processPlayerGuilPartyInfo")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processPlayerGuilPartyInfo")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    if (dstBeing)
    {
        dstBeing->setPartyName(msg.readString(24, "party name"));
        if (!guildManager || !GuildManager::getEnableGuildBot())
        {
            dstBeing->setGuildName(msg.readString(24, "guild name"));
            dstBeing->setGuildPos(msg.readString(24, "guild pos"));
        }
        else
        {
            msg.readString(24, "guild name");
            msg.readString(24, "guild pos");
        }
        dstBeing->addToCache();
        msg.readString(24, "?");
    }
    else
    {
        msg.readString(24, "party name");
        msg.readString(24, "guild name");
        msg.readString(24, "guild pos");
        msg.readString(24, "?");
    }
    BLOCK_END("BeingNet::processPlayerGuilPartyInfo")
}

void BeingNet::processBeingSelfEffect(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processBeingSelfEffect")
    if (!effectManager || !actorManager)
    {
        BLOCK_END("BeingNet::processBeingSelfEffect")
        return;
    }

    const BeingId id = msg.readBeingId("being id");
    Being *const being = actorManager->findBeing(id);
    if (!being)
    {
        BLOCK_END("BeingNet::processBeingSelfEffect")
        return;
    }

    const int effectType = msg.readInt32("effect type");

    if (Particle::enabled)
        effectManager->trigger(effectType, being);

    // +++ need dehard code effectType == 3
    if (effectType == 3 && being->getType() == ActorType::Player
        && socialWindow)
    {   // reset received damage
        socialWindow->resetDamage(being->getName());
    }
    BLOCK_END("BeingNet::processBeingSelfEffect")
}

void BeingNet::processSkillCastCancel(Net::MessageIn &msg)
{
    msg.readInt32("skill id");
}

void BeingNet::processIpResponse(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processIpResponse")
    if (!actorManager)
    {
        BLOCK_END("BeingNet::processIpResponse")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));
    if (dstBeing)
        dstBeing->setIp(ipToString(msg.readInt32("ip address")));
    BLOCK_END("BeingNet::processIpResponse")
}

void BeingNet::processPvpSet(Net::MessageIn &msg)
{
    BLOCK_START("BeingNet::processPvpSet")
    const BeingId id = msg.readBeingId("being id");
    const int rank = msg.readInt32("rank");
    int teamId = 0;
    if (serverFeatures->haveTeamId())
        teamId = msg.readInt32("team");
    else
        msg.readInt32("num");
    if (actorManager)
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing)
        {
            dstBeing->setPvpRank(rank);
            dstBeing->setTeamId(static_cast<uint16_t>(teamId));
            dstBeing->addToCache();
        }
    }
    BLOCK_END("BeingNet::processPvpSet")
}

void BeingNet::applyPlayerAction(Net::MessageIn &msg,
                                 Being *const being,
                                 const uint8_t type)
{
    if (!being)
        return;
    switch (type)
    {
        case 0:
            being->setAction(BeingAction::STAND, 0);
            localPlayer->imitateAction(being, BeingAction::STAND);
            break;

        case 1:
            if (being->getCurrentAction() != BeingAction::DEAD)
            {
                being->setAction(BeingAction::DEAD, 0);
                being->recalcSpritesOrder();
            }
            break;

        case 2:
            being->setAction(BeingAction::SIT, 0);
            localPlayer->imitateAction(being, BeingAction::SIT);
            break;

        default:
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void BeingNet::setServerGender(Being *const being, const uint8_t gender)
{
    if (!being)
        return;
    switch (gender)
    {
        case 2:
            being->setGender(Gender::FEMALE);
            break;
        case 3:
            being->setGender(Gender::MALE);
            break;
        case 4:
            being->setGender(Gender::OTHER);
            break;
        default:
            being->setGender(Gender::UNSPECIFIED);
            break;
    }
}

}  // namespace TmwAthena
