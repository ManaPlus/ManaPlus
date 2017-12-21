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

#include "net/tmwa/beingrecv.h"

#include "actormanager.h"
#include "configuration.h"
#include "effectmanager.h"
#include "guild.h"
#include "party.h"

#include "being/localplayer.h"

#include "particle/particleengine.h"

#include "input/keyboardconfig.h"

#include "gui/windows/okdialog.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/socialwindow.h"

#include "net/beinghandler.h"
#include "net/serverfeatures.h"

#include "net/messagein.h"

#include "net/ea/beingrecv.h"

#include "net/tmwa/guildmanager.h"
#include "net/tmwa/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/timer.h"

#include "debug.h"

extern OkDialog *deathNotice;

namespace TmwAthena
{

static void setGm(Being *const dstBeing,
                  const uint16_t gmstatus)
{
    if (dstBeing != localPlayer)
    {
        if ((gmstatus & 0x80) != 0)
        {
            dstBeing->setGroupId(paths.getIntValue("gmDefaultLevel"));
            dstBeing->setGM(true);
        }
        else
        {
            dstBeing->setGroupId(0);
            dstBeing->setGM(false);
        }
    }
    else
    {
        if ((gmstatus & 0x80) != 0)
            dstBeing->setGM(true);
        else
            dstBeing->setGM(false);
    }
}

void BeingRecv::processBeingChangeLook(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingChangeLook")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingChangeLook")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    const uint8_t type = msg.readUInt8("type");
    const int16_t id = CAST_S16(msg.readUInt8("id"));
    const int id2 = 1;

    if ((localPlayer == nullptr) || (dstBeing == nullptr))
    {
        BLOCK_END("BeingRecv::processBeingChangeLook")
        return;
    }
    processBeingChangeLookContinue(msg, dstBeing, type, id, id2);
    BLOCK_END("BeingRecv::processBeingChangeLook")
}

void BeingRecv::processBeingChangeLook2(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingChangeLook")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingChangeLook")
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

    if ((localPlayer == nullptr) || (dstBeing == nullptr))
    {
        BLOCK_END("BeingRecv::processBeingChangeLook")
        return;
    }
    processBeingChangeLookContinue(msg, dstBeing, type, id, id2);
    BLOCK_END("BeingRecv::processBeingChangeLook")
}

void BeingRecv::processBeingChangeLookContinue(const Net::MessageIn &msg,
                                               Being *const dstBeing,
                                               const uint8_t type,
                                               const int id,
                                               const int id2)
{
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setOtherTime();

    switch (type)
    {
        case 0:     // change race
            dstBeing->setSubtype(fromInt(id, BeingTypeId),
                dstBeing->getLook());
            break;
        case 1:     // eAthena LOOK_HAIR
        {
            const uint16_t look = CAST_U16(id / 256);
            const int hair = id % 256;
            dstBeing->setHairStyle(SPRITE_HAIR_COLOR, hair * -1);
            dstBeing->setLook(look);
            break;
        }
        case 2:     // Weapon ID in id, Shield ID in id2
            dstBeing->setSpriteId(SPRITE_BODY,
                id);
            dstBeing->setWeaponId(id);
            dstBeing->setSpriteId(SPRITE_FLOOR,
                id2);
            localPlayer->imitateOutfit(dstBeing, SPRITE_FLOOR);
            break;
        case 3:     // Change lower headgear for eAthena, pants for us
            dstBeing->setSpriteId(SPRITE_WEAPON,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_WEAPON);
            break;
        case 4:     // Change upper headgear for eAthena, hat for us
            dstBeing->setSpriteId(SPRITE_CLOTHES_COLOR,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_CLOTHES_COLOR);
            break;
        case 5:     // Change middle headgear for eathena, armor for us
            dstBeing->setSpriteId(SPRITE_HEAD_BOTTOM,
                id);
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
            dstBeing->setSpriteId(SPRITE_FLOOR,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_FLOOR);
            break;
        case 9:     // eAthena LOOK_SHOES
            dstBeing->setSpriteId(SPRITE_HAIR,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_HAIR);
            break;
        case 10:   // LOOK_GLOVES
            dstBeing->setSpriteId(SPRITE_SHOES,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHOES);
            break;
        case 11:  // LOOK_CAPE
            dstBeing->setSpriteId(SPRITE_SHIELD,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHIELD);
            break;
        case 12:
            dstBeing->setSpriteId(SPRITE_HEAD_TOP,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_HEAD_TOP);
            break;
        case 13:
            dstBeing->setSpriteId(SPRITE_HEAD_MID,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_HEAD_MID);
            break;
        case 14:
            dstBeing->setSpriteId(SPRITE_ROBE,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_ROBE);
            break;
        case 15:
            dstBeing->setSpriteId(SPRITE_EVOL2,
                id);
            localPlayer->imitateOutfit(dstBeing, SPRITE_EVOL2);
            break;
        case 16:
            dstBeing->setLook(CAST_U16(id));
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(type);
            break;
    }
}

void BeingRecv::processPlayerUpdate1(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPlayerMoveUpdate")
    if ((actorManager == nullptr) || (localPlayer == nullptr))
    {
        BLOCK_END("BeingRecv::processPlayerMoveUpdate")
        return;
    }

    // An update about a player, potentially including movement.
    const BeingId id = msg.readBeingId("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint32_t opt1 = msg.readInt16("opt1");
    const uint32_t opt2 = msg.readInt16("opt2");
    const uint32_t option = msg.readInt16("option");
    const int16_t job = msg.readInt16("job");
    int disguiseId = 0;
    if (toInt(id, int) < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);
    if (dstBeing == nullptr)
    {
        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingRecv::processPlayerMoveUpdate")
            return;
        }

        dstBeing = Ea::BeingRecv::createBeing(id, job);

        if (dstBeing == nullptr)
        {
            BLOCK_END("BeingRecv::processPlayerMoveUpdate")
            return;
        }
    }
    else if (disguiseId != 0)
    {
        actorManager->undelete(dstBeing);
        beingHandler->requestNameById(id);
    }

    uint8_t dir = dstBeing->getDirectionDelayed();
    if (dir != 0u)
    {
        if (dir != dstBeing->getDirection())
            dstBeing->setDirection(dir);
    }

    if (Party *const party = localPlayer->getParty())
    {
        if (party->isMember(id))
            dstBeing->setParty(party);
    }

    dstBeing->setWalkSpeed(speed);

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

    if ((guildManager == nullptr) || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(CAST_S16(guild)));
    }

    msg.readInt16("emblem");
    dstBeing->setManner(msg.readInt16("manner"));
    const uint32_t opt3 = msg.readInt16("opt3");
    dstBeing->setKarma(msg.readUInt8("karma"));
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(
        CAST_U8(msg.readUInt8("gender") & 3)));

    if (disguiseId == 0)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_BODY,
            weapon,
            std::string());
        dstBeing->setWeaponId(weapon);
        dstBeing->updateSprite(SPRITE_FLOOR, shield, std::string());
        dstBeing->updateSprite(SPRITE_WEAPON, headBottom, std::string());
        dstBeing->updateSprite(SPRITE_HEAD_BOTTOM, headMid, std::string());
        dstBeing->updateSprite(SPRITE_CLOTHES_COLOR, headTop, std::string());
        if (hairStyle == 0)
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR,
                0,
                std::string());
        }
        else
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR,
                hairStyle * -1,
                ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        }
        dstBeing->setHairColor(hairColor);
    }
    localPlayer->imitateOutfit(dstBeing, -1);

    uint16_t x, y;
    msg.readCoordinates(x, y, dir, "position");
    dstBeing->setTileCoords(x, y);
    dstBeing->setDirection(dir);

    localPlayer->imitateDirection(dstBeing, dir);

    const uint16_t gmstatus = msg.readInt16("gm status");

    setGm(dstBeing, gmstatus);

    applyPlayerAction(msg, dstBeing, msg.readUInt8("action type"));
    const int level = CAST_S32(msg.readUInt8("level"));
    if (level != 0)
        dstBeing->setLevel(level);

    msg.readUInt8("unused");

    dstBeing->setActionTime(tick_time);

    dstBeing->setStatusEffectOpitons(option,
        opt1,
        opt2,
        opt3);

    BLOCK_END("BeingRecv::processPlayerMoveUpdate")
}

void BeingRecv::processPlayerUpdate2(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPlayerMoveUpdate")
    if ((actorManager == nullptr) || (localPlayer == nullptr))
    {
        BLOCK_END("BeingRecv::processPlayerMoveUpdate")
        return;
    }

    // An update about a player, potentially including movement.
    const BeingId id = msg.readBeingId("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint32_t opt1 = msg.readInt16("opt1");
    const uint32_t opt2 = msg.readInt16("opt2");
    const uint32_t option = msg.readInt16("option");
    const int16_t job = msg.readInt16("job");
    int disguiseId = 0;
    if (toInt(id, int) < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);
    if (dstBeing == nullptr)
    {
        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingRecv::processPlayerMoveUpdate")
            return;
        }

        dstBeing = Ea::BeingRecv::createBeing(id, job);

        if (dstBeing == nullptr)
        {
            BLOCK_END("BeingRecv::processPlayerMoveUpdate")
            return;
        }
    }
    else if (disguiseId != 0)
    {
        actorManager->undelete(dstBeing);
        beingHandler->requestNameById(id);
    }

    uint8_t dir = dstBeing->getDirectionDelayed();
    if (dir != 0u)
    {
        if (dir != dstBeing->getDirection())
            dstBeing->setDirection(dir);
    }

    if (Party *const party = localPlayer->getParty())
    {
        if (party->isMember(id))
            dstBeing->setParty(party);
    }

    dstBeing->setWalkSpeed(speed);

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

    if ((guildManager == nullptr) || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(CAST_S16(guild)));
    }

    msg.readInt16("emblem");
    dstBeing->setManner(msg.readInt16("manner"));
    const uint32_t opt3 = msg.readInt16("opt3");
    dstBeing->setKarma(msg.readUInt8("karma"));
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(
        CAST_U8(msg.readUInt8("gender") & 3)));

    if (disguiseId == 0)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_BODY,
            weapon,
            std::string());
        dstBeing->setWeaponId(weapon);
        dstBeing->updateSprite(SPRITE_FLOOR, shield, std::string());
        dstBeing->updateSprite(SPRITE_WEAPON, headBottom, std::string());
        dstBeing->updateSprite(SPRITE_HEAD_BOTTOM, headMid, std::string());
        dstBeing->updateSprite(SPRITE_CLOTHES_COLOR, headTop, std::string());
        if (hairStyle == 0)
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR,
                0,
                std::string());
        }
        else
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR,
                hairStyle * -1,
                ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        }
        dstBeing->setHairColor(hairColor);
    }
    localPlayer->imitateOutfit(dstBeing, -1);

    uint16_t x, y;
    msg.readCoordinates(x, y, dir, "position");
    dstBeing->setTileCoords(x, y);
    dstBeing->setDirection(dir);

    localPlayer->imitateDirection(dstBeing, dir);

    const uint16_t gmstatus = msg.readInt16("gm status");

    setGm(dstBeing, gmstatus);

    applyPlayerAction(msg, dstBeing, msg.readUInt8("action type"));
    const int level = CAST_S32(msg.readUInt8("level"));
    if (level != 0)
        dstBeing->setLevel(level);

    dstBeing->setActionTime(tick_time);
    dstBeing->setStatusEffectOpitons(option,
        opt1,
        opt2,
        opt3);

    BLOCK_END("BeingRecv::processPlayerMoveUpdate")
}

void BeingRecv::processPlayerMove(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPlayerMoveUpdate")
    if ((actorManager == nullptr) || (localPlayer == nullptr))
    {
        BLOCK_END("BeingRecv::processPlayerMoveUpdate")
        return;
    }

    // An update about a player, potentially including movement.
    const BeingId id = msg.readBeingId("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint32_t opt1 = msg.readInt16("opt1");
    const uint32_t opt2 = msg.readInt16("opt2");
    const uint32_t option = msg.readInt16("option");
    const int16_t job = msg.readInt16("job");
    int disguiseId = 0;
    if (toInt(id, int) < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);
    if (dstBeing == nullptr)
    {
        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingRecv::processPlayerMoveUpdate")
            return;
        }

        dstBeing = Ea::BeingRecv::createBeing(id, job);

        if (dstBeing == nullptr)
        {
            BLOCK_END("BeingRecv::processPlayerMoveUpdate")
            return;
        }
    }
    else if (disguiseId != 0)
    {
        actorManager->undelete(dstBeing);
        beingHandler->requestNameById(id);
    }

    const uint8_t dir = dstBeing->getDirectionDelayed();
    if (dir != 0u)
    {
        if (dir != dstBeing->getDirection())
            dstBeing->setDirection(dir);
    }

    if (Party *const party = localPlayer->getParty())
    {
        if (party->isMember(id))
            dstBeing->setParty(party);
    }

    dstBeing->setWalkSpeed(speed);

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

    if ((guildManager == nullptr) || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(CAST_S16(guild)));
    }

    msg.readInt16("emblem");
    dstBeing->setManner(msg.readInt16("manner"));
    const uint32_t opt3 = msg.readInt16("opt3");
    dstBeing->setKarma(msg.readUInt8("karma"));
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(
        CAST_U8(msg.readUInt8("gender") & 3)));

    if (disguiseId == 0)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_BODY,
            weapon,
            std::string());
        dstBeing->setWeaponId(weapon);
        dstBeing->updateSprite(SPRITE_FLOOR, shield, std::string());
        dstBeing->updateSprite(SPRITE_WEAPON, headBottom, std::string());
        dstBeing->updateSprite(SPRITE_HEAD_BOTTOM, headMid, std::string());
        dstBeing->updateSprite(SPRITE_CLOTHES_COLOR, headTop, std::string());
        if (hairStyle == 0)
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR,
                0,
                std::string());
        }
        else
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR,
                hairStyle * -1,
                ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        }
        dstBeing->setHairColor(hairColor);
    }
    localPlayer->imitateOutfit(dstBeing, -1);

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

        if ((d != 0) && dstBeing->getDirection() != d)
            dstBeing->setDirectionDelayed(CAST_U8(d));
    }

    if (localPlayer->getCurrentAction() != BeingAction::STAND)
        localPlayer->imitateAction(dstBeing, BeingAction::STAND);
    if (localPlayer->getDirection() != dstBeing->getDirection())
    {
        localPlayer->imitateDirection(dstBeing,
            dstBeing->getDirection());
    }

    const uint16_t gmstatus = msg.readInt16("gm status");

    setGm(dstBeing, gmstatus);

    msg.readUInt8("unused");

    const int level = CAST_S32(msg.readUInt8("level"));
    if (level != 0)
        dstBeing->setLevel(level);

    msg.readUInt8("unused");

    if (dstBeing->getType() != ActorType::Player)
        dstBeing->setActionTime(tick_time);

    dstBeing->setStatusEffectOpitons(option,
        opt1,
        opt2,
        opt3);

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();
    BLOCK_END("BeingRecv::processPlayerMoveUpdate")
}

void BeingRecv::processBeingVisible(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingVisibleOrMove")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingVisibleOrMove")
        return;
    }

    BeingId spawnId;

    // Information about a being in range
    const BeingId id = msg.readBeingId("being id");
    if (id == Ea::BeingRecv::mSpawnId)
        spawnId = Ea::BeingRecv::mSpawnId;
    else
        spawnId = BeingId_zero;
    Ea::BeingRecv::mSpawnId = BeingId_zero;
    int16_t speed = msg.readInt16("speed");
    const uint32_t opt1 = msg.readInt16("opt1");
    const uint32_t opt2 = msg.readInt16("opt2");
    const uint32_t option = msg.readInt16("option");
    const int16_t job = msg.readInt16("class");
    int disguiseId = 0;
    if (id == localPlayer->getId() && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);

    if ((dstBeing != nullptr) && dstBeing->getType() == ActorType::Monster
        && !dstBeing->isAlive())
    {
        actorManager->destroy(dstBeing);
        actorManager->erase(dstBeing);
        dstBeing = nullptr;
    }

    if (dstBeing == nullptr)
    {
        // Being with id >= 110000000 and job 0 are better
        // known as ghosts, so don't create those.
        if (job == 0 && toInt(id, int) >= 110000000)
        {
            BLOCK_END("BeingRecv::processBeingVisibleOrMove")
            return;
        }

        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingRecv::processBeingVisibleOrMove")
            return;
        }

        dstBeing = Ea::BeingRecv::createBeing(id, job);

        if (dstBeing == nullptr)
        {
            BLOCK_END("BeingRecv::processBeingVisibleOrMove")
            return;
        }
    }
    else
    {
        if (dstBeing->getType() == ActorType::Npc)
        {
            actorManager->undelete(dstBeing);
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
    if (dstBeing->getType() == ActorType::Monster && (localPlayer != nullptr))
        localPlayer->checkNewName(dstBeing);
    dstBeing->setWalkSpeed(speed);
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t headBottom = msg.readInt16("head bottom");

    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const ItemColor hairColor = fromInt(msg.readUInt8("hair color"),
        ItemColor);
    msg.readUInt8("unused");
    const uint16_t shoes = msg.readInt16("shoes / clothes color");

    uint16_t gloves;
    if (dstBeing->getType() == ActorType::Monster)
    {
        if (serverFeatures->haveServerHp())
        {
            const int hp = msg.readInt32("hp");
            const int maxHP = msg.readInt32("max hp");
            if ((hp != 0) && (maxHP != 0))
            {
                dstBeing->setMaxHP(maxHP);
                const int oldHP = dstBeing->getHP();
                if ((oldHP == 0) || oldHP > hp)
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
    const uint32_t opt3 = msg.readInt16("opt3");
    if (serverFeatures->haveMonsterAttackRange()
        && dstBeing->getType() == ActorType::Monster)
    {
        const int attackRange = CAST_S32(
            msg.readUInt8("attack range (was karma)"));
        dstBeing->setAttackRange(attackRange);
    }
    else
    {
        dstBeing->setKarma(msg.readUInt8("karma"));
    }
    uint8_t gender = msg.readUInt8("gender");

    if ((disguiseId == 0) && dstBeing->getType() == ActorType::Player)
    {
        // reserving bits for future usage
        gender &= 3;
        dstBeing->setGender(Being::intToGender(gender));
        // Set these after the gender, as the sprites may be gender-specific
        if (hairStyle == 0)
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR, 0, std::string());
        }
        else
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR,
                hairStyle * -1,
                ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        }
        dstBeing->setHairColor(hairColor);
        dstBeing->updateSprite(SPRITE_WEAPON, headBottom, std::string());
        dstBeing->updateSprite(SPRITE_HEAD_BOTTOM, headMid, std::string());
        dstBeing->updateSprite(SPRITE_CLOTHES_COLOR, headTop, std::string());
        dstBeing->updateSprite(SPRITE_HAIR, shoes, std::string());
        dstBeing->updateSprite(SPRITE_SHOES, gloves, std::string());
        dstBeing->updateSprite(SPRITE_BODY, weapon, std::string());
        dstBeing->setWeaponId(weapon);
        dstBeing->updateSprite(SPRITE_FLOOR, shield, std::string());
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

    if (job == 45 && (socialWindow != nullptr) && (outfitWindow != nullptr))
    {
        const int num = socialWindow->getPortalIndex(x, y);
        if (num >= 0)
        {
            dstBeing->setName(KeyboardConfig::getKeyShortString(
                OutfitWindow::keyName(num)));
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

    dstBeing->setStatusEffectOpitons(option,
        opt1,
        opt2,
        opt3);
    BLOCK_END("BeingRecv::processBeingVisibleOrMove")
}

void BeingRecv::processBeingMove(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingVisibleOrMove")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingVisibleOrMove")
        return;
    }

    BeingId spawnId;

    // Information about a being in range
    const BeingId id = msg.readBeingId("being id");
    if (id == Ea::BeingRecv::mSpawnId)
        spawnId = Ea::BeingRecv::mSpawnId;
    else
        spawnId = BeingId_zero;
    Ea::BeingRecv::mSpawnId = BeingId_zero;
    int16_t speed = msg.readInt16("speed");
    const uint32_t opt1 = msg.readInt16("opt1");
    const uint32_t opt2 = msg.readInt16("opt2");
    const uint32_t option = msg.readInt16("option");
    const int16_t job = msg.readInt16("class");
    int disguiseId = 0;
    if (id == localPlayer->getId() && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);

    if ((dstBeing != nullptr) && dstBeing->getType() == ActorType::Monster
        && !dstBeing->isAlive())
    {
        actorManager->destroy(dstBeing);
        actorManager->erase(dstBeing);
        dstBeing = nullptr;
    }

    if (dstBeing == nullptr)
    {
        // Being with id >= 110000000 and job 0 are better
        // known as ghosts, so don't create those.
        if (job == 0 && toInt(id, int) >= 110000000)
        {
            BLOCK_END("BeingRecv::processBeingVisibleOrMove")
            return;
        }

        if (actorManager->isBlocked(id) == true)
        {
            BLOCK_END("BeingRecv::processBeingVisibleOrMove")
            return;
        }

        dstBeing = Ea::BeingRecv::createBeing(id, job);

        if (dstBeing == nullptr)
        {
            BLOCK_END("BeingRecv::processBeingVisibleOrMove")
            return;
        }
    }
    else
    {
        if (dstBeing->getType() == ActorType::Npc)
        {
            actorManager->undelete(dstBeing);
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
    if (dstBeing->getType() == ActorType::Monster && (localPlayer != nullptr))
        localPlayer->checkNewName(dstBeing);
    dstBeing->setWalkSpeed(speed);
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
            if ((hp != 0) && (maxHP != 0))
            {
                dstBeing->setMaxHP(maxHP);
                const int oldHP = dstBeing->getHP();
                if ((oldHP == 0) || oldHP > hp)
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
    const uint32_t opt3 = msg.readInt16("opt3");
    if (serverFeatures->haveMonsterAttackRange()
        && dstBeing->getType() == ActorType::Monster)
    {
        const int attackRange = CAST_S32(
            msg.readUInt8("attack range (was karma)"));
        dstBeing->setAttackRange(attackRange);
    }
    else
    {
        dstBeing->setKarma(msg.readUInt8("karma"));
    }
    uint8_t gender = msg.readUInt8("gender");

    if ((disguiseId == 0) && dstBeing->getType() == ActorType::Player)
    {
        // reserving bits for future usage
        gender &= 3;
        dstBeing->setGender(Being::intToGender(gender));
        // Set these after the gender, as the sprites may be gender-specific
        if (hairStyle == 0)
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR, 0, std::string());
        }
        else
        {
            dstBeing->updateSprite(SPRITE_HAIR_COLOR,
                hairStyle * -1,
                ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        }
        dstBeing->setHairColor(hairColor);
        dstBeing->updateSprite(SPRITE_WEAPON, headBottom, std::string());
        dstBeing->updateSprite(SPRITE_HEAD_BOTTOM, headMid, std::string());
        dstBeing->updateSprite(SPRITE_CLOTHES_COLOR, headTop, std::string());
        dstBeing->updateSprite(SPRITE_HAIR, shoes, std::string());
        dstBeing->updateSprite(SPRITE_SHOES, gloves, std::string());
        dstBeing->updateSprite(SPRITE_BODY, weapon, std::string());
        dstBeing->setWeaponId(weapon);
        dstBeing->updateSprite(SPRITE_FLOOR, shield, std::string());
    }
    else if (dstBeing->getType() == ActorType::Npc
             && serverFeatures->haveNpcGender())
    {
        setServerGender(dstBeing, gender);
    }

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    if (disguiseId == 0)
    {
        dstBeing->setAction(BeingAction::STAND, 0);
        dstBeing->setTileCoords(srcX, srcY);
        if (serverFeatures->haveMove3())
            dstBeing->setCachedDestination(dstX, dstY);
        else
            dstBeing->setDestination(dstX, dstY);
    }

    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");
    msg.readUInt8("unknown");

    dstBeing->setStatusEffectOpitons(option,
        opt1,
        opt2,
        opt3);
    BLOCK_END("BeingRecv::processBeingVisibleOrMove")
}

void BeingRecv::processBeingSpawn(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingSpawn")
    // skipping this packet
    Ea::BeingRecv::mSpawnId = msg.readBeingId("being id");
    msg.readInt16("speed");
    msg.readInt16("opt1");
    msg.readInt16("opt2");
    msg.readInt16("option");
    msg.readInt16("disguise");
    msg.skip(25, "unused");
    BLOCK_END("BeingRecv::processBeingSpawn")
}

void BeingRecv::processSkillCasting(Net::MessageIn &msg)
{
    msg.readInt32("src id");
    msg.readInt32("dst id");
    msg.readInt16("dst x");
    msg.readInt16("dst y");
    msg.readInt16("skill num");
    msg.readInt32("skill get p1");
    msg.readInt32("cast time");
}

void BeingRecv::processBeingStatusChange(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingStatusChange")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingStatusChange")
        return;
    }

    // Status change
    const uint16_t status = msg.readInt16("status");
    const BeingId id = msg.readBeingId("being id");
    const Enable flag = fromBool(
        msg.readUInt8("flag: 0: stop, 1: start"), Enable);

    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing != nullptr)
    {
        // dont know on legacy servers is effect really started
        // or not. Because this always sending IsStart_true
        dstBeing->setStatusEffect(status, flag, IsStart_true);
    }
    BLOCK_END("BeingRecv::processBeingStatusChange")
}

void BeingRecv::processBeingMove2(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingMove2")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingMove2")
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

    if (dstBeing == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingMove2")
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
    BLOCK_END("BeingRecv::processBeingMove2")
}

void BeingRecv::processBeingChangeDirection(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingChangeDirection")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingChangeDirection")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    if (dstBeing == nullptr)
    {
        DEBUGLOGSTR("invisible player?");
        msg.readInt16("unused");
        msg.readUInt8("direction");
        BLOCK_END("BeingRecv::processBeingChangeDirection");
        return;
    }

    msg.readInt16("unused");

    const uint8_t dir = Net::MessageIn::fromServerDirection(
        CAST_U8(msg.readUInt8("direction") & 0x0FU));
    dstBeing->setDirection(dir);
    if (localPlayer != nullptr)
        localPlayer->imitateDirection(dstBeing, dir);
    BLOCK_END("BeingRecv::processBeingChangeDirection")
}

void BeingRecv::processPlayerStatusChange(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPlayerStop")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processPlayerStop")
        return;
    }

    // Change in players' flags
    const BeingId id = msg.readBeingId("account id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing == nullptr)
    {
        DEBUGLOGSTR("invisible player?");
        msg.readInt16("stun mode");
        msg.readInt16("status effect");
        msg.readInt16("opt?");
        msg.readUInt8("Unused?");
        return;
    }

    const uint32_t opt1 = msg.readInt16("opt1");
    const uint32_t opt2 = msg.readInt16("opt2");
    const uint32_t option = msg.readInt16("option");
    msg.readUInt8("Unused?");

    dstBeing->setStatusEffectOpitons(option,
        opt1,
        opt2);
    BLOCK_END("BeingRecv::processPlayerStop")
}

void BeingRecv::processBeingResurrect(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingResurrect")
    if (actorManager == nullptr || localPlayer == nullptr)
    {
        BLOCK_END("BeingRecv::processBeingResurrect")
        return;
    }

    // A being changed mortality status

    const BeingId id = msg.readBeingId("being id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing == nullptr)
    {
        DEBUGLOGSTR("insible player?");
        msg.readInt16("flag?");
        BLOCK_END("BeingRecv::processBeingResurrect")
        return;
    }

    // If this is player's current target, clear it.
    if (dstBeing == localPlayer->getTarget())
        localPlayer->stopAttack(false);
    if (dstBeing == localPlayer &&
        deathNotice != nullptr)
    {
        deathNotice->scheduleDelete();
        deathNotice = nullptr;
    }

    if (msg.readInt16("flag?") == 1)
        dstBeing->setAction(BeingAction::STAND, 0);
    BLOCK_END("BeingRecv::processBeingResurrect")
}

void BeingRecv::processPlayerGuilPartyInfo(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPlayerGuilPartyInfo")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processPlayerGuilPartyInfo")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    if (dstBeing != nullptr)
    {
        dstBeing->setPartyName(msg.readString(24, "party name"));
        if ((guildManager == nullptr) || !GuildManager::getEnableGuildBot())
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
    BLOCK_END("BeingRecv::processPlayerGuilPartyInfo")
}

void BeingRecv::processBeingSelfEffect(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processBeingSelfEffect")
    if ((effectManager == nullptr) || (actorManager == nullptr))
    {
        BLOCK_END("BeingRecv::processBeingSelfEffect")
        return;
    }

    const BeingId id = msg.readBeingId("being id");
    Being *const being = actorManager->findBeing(id);
    if (being == nullptr)
    {
        DEBUGLOGSTR("insible player?");
        msg.readInt32("effect type");
        BLOCK_END("BeingRecv::processBeingSelfEffect")
        return;
    }

    const int effectType = msg.readInt32("effect type");

    if (ParticleEngine::enabled)
        effectManager->trigger(effectType, being, 0);

    // +++ need dehard code effectType == 3
    if (effectType == 3 && being->getType() == ActorType::Player
        && (socialWindow != nullptr))
    {   // reset received damage
        socialWindow->resetDamage(being->getName());
    }
    BLOCK_END("BeingRecv::processBeingSelfEffect")
}

void BeingRecv::processSkillCastCancel(Net::MessageIn &msg)
{
    msg.readInt32("skill id");
}

void BeingRecv::processIpResponse(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processIpResponse")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processIpResponse")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));
    if (dstBeing != nullptr)
    {
        const std::string ip = ipToString(msg.readInt32("ip address"));
        dstBeing->setIp(ip);
    }
    else
    {
        msg.readInt32("ip address");
        DEBUGLOGSTR("invisible player?");
    }

    BLOCK_END("BeingRecv::processIpResponse")
}

void BeingRecv::processPvpSet(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processPvpSet")
    const BeingId id = msg.readBeingId("being id");
    const int rank = msg.readInt32("rank");
    int teamId = 0;
    teamId = msg.readInt32("team");
    if (actorManager != nullptr)
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing != nullptr)
        {
            dstBeing->setPvpRank(rank);
            dstBeing->setTeamId(CAST_U16(teamId));
            dstBeing->addToCache();
        }
    }
    BLOCK_END("BeingRecv::processPvpSet")
}

void BeingRecv::applyPlayerAction(Net::MessageIn &msg,
                                  Being *const being,
                                  const uint8_t type)
{
    if (being == nullptr)
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
            UNIMPLEMENTEDPACKETFIELD(type);
            break;
    }
}

void BeingRecv::processSkillDamage(Net::MessageIn &msg)
{
    BLOCK_START("BeingRecv::processSkillDamage")
    if (actorManager == nullptr)
    {
        BLOCK_END("BeingRecv::processSkillDamage")
        return;
    }

    const int id = msg.readInt16("skill id");
    Being *const srcBeing = actorManager->findBeing(
        msg.readBeingId("src being id"));
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("dst being id"));
    msg.readInt32("tick");
    msg.readInt32("src speed");
    msg.readInt32("dst speed");
    const int param1 = msg.readInt32("damage");
    const int level = msg.readInt16("skill level");
    msg.readInt16("div");
    msg.readUInt8("skill hit/type?");
    if (srcBeing != nullptr)
        srcBeing->handleSkill(dstBeing, param1, id, level);
    if (dstBeing != nullptr)
        dstBeing->takeDamage(srcBeing, param1, AttackType::SKILL, id, level);
    BLOCK_END("BeingRecv::processSkillDamage")
}

void BeingRecv::setServerGender(Being *const being,
                                const uint8_t gender)
{
    if (being == nullptr)
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
