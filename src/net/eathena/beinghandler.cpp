/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "net/eathena/beinghandler.h"

#include "actormanager.h"
#include "guild.h"
#include "guildmanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "input/keyboardconfig.h"

#include "gui/windows/socialwindow.h"
#include "gui/windows/killstats.h"

#include "gui/windows/outfitwindow.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"
#include "net/eathena/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/stringutils.h"
#include "utils/timer.h"

#include "debug.h"

extern Net::BeingHandler *beingHandler;
extern int serverVersion;

namespace EAthena
{

BeingHandler::BeingHandler(const bool enableSync) :
    MessageHandler(),
    Ea::BeingHandler(enableSync)
{
    static const uint16_t _messages[] =
    {
        SMSG_BEING_VISIBLE,
        SMSG_BEING_MOVE,
        SMSG_BEING_MOVE2,
        SMSG_BEING_REMOVE,
        SMSG_SKILL_DAMAGE,
        SMSG_BEING_ACTION,
        SMSG_BEING_ACTION2,
        SMSG_BEING_SELFEFFECT,
        SMSG_BEING_SPECIAL_EFFECT,
        SMSG_BEING_EMOTION,
        SMSG_BEING_CHANGE_LOOKS,
        SMSG_BEING_CHANGE_LOOKS2,
        SMSG_BEING_NAME_RESPONSE,
        SMSG_BEING_NAME_RESPONSE2,
        SMSG_PLAYER_GUILD_PARTY_INFO,
        SMSG_BEING_CHANGE_DIRECTION,
        SMSG_PLAYER_UPDATE_1,
        SMSG_PLAYER_UPDATE_2,
        SMSG_PLAYER_MOVE,
        SMSG_PLAYER_STOP,
        SMSG_PLAYER_MOVE_TO_ATTACK,
        SMSG_PLAYER_STATUS_CHANGE,
        SMSG_PLAYER_STATUS_CHANGE_NO_TICK,
        SMSG_BEING_STATUS_CHANGE,
        SMSG_BEING_STATUS_CHANGE2,
        SMSG_BEING_RESURRECT,
        SMSG_SOLVE_CHAR_NAME,
        SMSG_BEING_SPAWN,
        SMSG_SKILL_CASTING,
        SMSG_SKILL_CAST_CANCEL,
        SMSG_SKILL_NO_DAMAGE,
        SMSG_SKILL_GROUND_NO_DAMAGE,
        SMSG_SKILL_ENTRY,
        SMSG_BEING_IP_RESPONSE,
        SMSG_PVP_MAP_MODE,
        SMSG_PVP_SET,
        SMSG_MAP_TYPE_PROPERTY2,
        SMSG_MONSTER_HP,
        SMSG_PLAYER_HP,
        SMSG_SKILL_AUTO_CAST,
        SMSG_RANKS_LIST,
        0
    };
    handledMessages = _messages;
    beingHandler = this;
}

void BeingHandler::requestNameById(const int id) const
{
    MessageOut outMsg(CMSG_NAME_REQUEST);
    outMsg.writeInt32(id);  // readLong(2));
}

void BeingHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_BEING_VISIBLE:
            processBeingVisible(msg);
            break;

        case SMSG_BEING_MOVE:
            processBeingMove(msg);
            break;

        case SMSG_BEING_SPAWN:
            processBeingSpawn(msg);
            break;

        case SMSG_BEING_MOVE2:
            processBeingMove2(msg);
            break;

        case SMSG_BEING_REMOVE:
            processBeingRemove(msg);
            break;

        case SMSG_BEING_RESURRECT:
            processBeingResurrect(msg);
            break;

        case SMSG_SKILL_DAMAGE:
            processSkillDamage(msg);
            break;

        case SMSG_SKILL_AUTO_CAST:
            processSkillAutoCast(msg);
            break;

        case SMSG_BEING_ACTION:
            processBeingAction(msg);
            break;

        case SMSG_BEING_ACTION2:
            processBeingAction2(msg);
            break;

        case SMSG_BEING_SELFEFFECT:
            processBeingSelfEffect(msg);
            break;

        case SMSG_BEING_SPECIAL_EFFECT:
            processBeingSpecialEffect(msg);
            break;

        case SMSG_BEING_EMOTION:
            processBeingEmotion(msg);
            break;

        case SMSG_BEING_CHANGE_LOOKS:
            processBeingChangeLook(msg);
            break;

        case SMSG_BEING_CHANGE_LOOKS2:
            processBeingChangeLook2(msg);
            break;

        case SMSG_BEING_NAME_RESPONSE:
            processNameResponse(msg);
            break;

        case SMSG_BEING_NAME_RESPONSE2:
            processNameResponse2(msg);
            break;

        case SMSG_BEING_IP_RESPONSE:
            processIpResponse(msg);
            break;

        case SMSG_SOLVE_CHAR_NAME:
            break;

        case SMSG_PLAYER_GUILD_PARTY_INFO:
            processPlayerGuilPartyInfo(msg);
            break;

        case SMSG_BEING_CHANGE_DIRECTION:
            processBeingChangeDirection(msg);
            break;

        case SMSG_PLAYER_UPDATE_1:
            processPlayerUpdate1(msg);
            break;

        case SMSG_PLAYER_UPDATE_2:
            processPlayerUpdate2(msg);
            break;

        case SMSG_PLAYER_MOVE:
            processPlayerMove(msg);
            break;

        case SMSG_PLAYER_STOP:
            processPlayerStop(msg);
            break;

        case SMSG_PLAYER_MOVE_TO_ATTACK:
            processPlayerMoveToAttack(msg);
            break;

        case SMSG_PLAYER_STATUS_CHANGE:
            processPlaterStatusChange(msg);
            break;

        case SMSG_PLAYER_STATUS_CHANGE_NO_TICK:
            processPlaterStatusChangeNoTick(msg);
            break;

        case SMSG_BEING_STATUS_CHANGE:
        case SMSG_BEING_STATUS_CHANGE2:
            processBeingStatusChange(msg);
            break;

        case SMSG_SKILL_CASTING:
            processSkillCasting(msg);
            break;

        case SMSG_SKILL_CAST_CANCEL:
            msg.readInt32("id?");
            break;

        case SMSG_SKILL_NO_DAMAGE:
            processSkillNoDamage(msg);
            break;

        case SMSG_SKILL_GROUND_NO_DAMAGE:
            processSkillGroundNoDamage(msg);
            break;

        case SMSG_SKILL_ENTRY:
            processSkillEntry(msg);
            break;

        case SMSG_PVP_MAP_MODE:
            processPvpMapMode(msg);
            break;

        case SMSG_PVP_SET:
            processPvpSet(msg);
            break;

        case SMSG_MAP_TYPE_PROPERTY2:
            processMapTypeProperty(msg);
            break;

        case SMSG_MONSTER_HP:
        case SMSG_PLAYER_HP:
            processMonsterHp(msg);
            break;

        case SMSG_RANKS_LIST:
            processRanksList(msg);
            break;

        default:
            break;
    }
}

void BeingHandler::undress(Being *const being) const
{
    being->setSprite(SPRITE_BOTTOMCLOTHES, 0);
    being->setSprite(SPRITE_TOPCLOTHES, 0);
    being->setSprite(SPRITE_HAT, 0);
    being->setSprite(SPRITE_SHOE, 0);
    being->setSprite(SPRITE_GLOVES, 0);
//    being->setSprite(SPRITE_WEAPON, 0, "", true);
}

void BeingHandler::requestRanks(const Rank::Rank rank) const
{
    MessageOut outMsg(CMSG_REQUEST_RANKS);
    outMsg.writeInt16(static_cast<int16_t>(rank), "type");
}

void BeingHandler::processBeingChangeLook(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    Being *const dstBeing = actorManager->findBeing(
        msg.readInt32("being id"));
    const uint8_t type = msg.readUInt8("type");

    const int id = static_cast<int>(msg.readUInt8("id"));
    const unsigned int id2 = 1U;

    if (!localPlayer || !dstBeing)
        return;

    processBeingChangeLookContinue(dstBeing, type, id, id2);
}

void BeingHandler::processBeingChangeLook2(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    Being *const dstBeing = actorManager->findBeing(
        msg.readInt32("being id"));
    const uint8_t type = msg.readUInt8("type");

    const int id = msg.readInt16("id1");
    unsigned int id2 = msg.readInt16("id2");
    if (type != 2)
        id2 = 1;

    if (!localPlayer || !dstBeing)
        return;

    processBeingChangeLookContinue(dstBeing, type, id, id2);
}

void BeingHandler::processBeingChangeLookContinue(Being *const dstBeing,
                                                  const uint8_t type,
                                                  const int id,
                                                  const int id2) const
{
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setOtherTime();

    const std::string color;
    switch (type)
    {
        case 0:     // change race
            dstBeing->setSubtype(static_cast<uint16_t>(id), 0);
            break;
        case 1:     // eAthena LOOK_HAIR
            dstBeing->setSpriteID(SPRITE_HAIR, id *-1);
            break;
        case 2:     // Weapon ID in id, Shield ID in id2
            dstBeing->setSprite(SPRITE_WEAPON, id, "", 1, true);
            if (!mHideShield)
                dstBeing->setSprite(SPRITE_SHIELD, id2);
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHIELD);
            break;
        case 3:     // Change lower headgear for eAthena, pants for us
            dstBeing->setSprite(SPRITE_BOTTOMCLOTHES, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_BOTTOMCLOTHES);
            break;
        case 4:     // Change upper headgear for eAthena, hat for us
            dstBeing->setSprite(SPRITE_HAT, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_HAT);
            break;
        case 5:     // Change middle headgear for eathena, armor for us
            dstBeing->setSprite(SPRITE_TOPCLOTHES, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_TOPCLOTHES);
            break;
        case 6:     // eAthena LOOK_HAIR_COLOR
            dstBeing->setSpriteColor(SPRITE_HAIR, ItemDB::get(
                dstBeing->getSpriteID(SPRITE_HAIR)).getDyeColorsString(id));
            break;
        case 7:     // Clothes color
            // ignoring it
            break;
        case 8:     // eAthena LOOK_SHIELD
            if (!mHideShield)
            {
                dstBeing->setSprite(SPRITE_SHIELD, id, color,
                    static_cast<unsigned char>(id2));
            }
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHIELD);
            break;
        case 9:     // eAthena LOOK_SHOES
            dstBeing->setSprite(SPRITE_SHOE, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHOE);
            break;
        case 10:   // LOOK_GLOVES
            dstBeing->setSprite(SPRITE_GLOVES, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_GLOVES);
            break;
        case 11:  // LOOK_CAPE
            dstBeing->setSprite(SPRITE_CAPE, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_CAPE);
            break;
        case 12:
            dstBeing->setSprite(SPRITE_MISC1, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_MISC1);
            break;
        case 13:
            dstBeing->setSprite(SPRITE_MISC2, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_MISC2);
            break;
        case 14:
            dstBeing->setSprite(SPRITE_EVOL1, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_EVOL1);
            break;
        case 15:
            dstBeing->setSprite(SPRITE_EVOL2, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_EVOL2);
            break;
        default:
            logger->log("QQQ3 CHANGE_LOOKS: unsupported type: "
                    "%d, id: %d", type, id);
            logger->log("ID: " + toString(dstBeing->getId()));
            logger->log("name: " + toString(dstBeing->getName()));
            break;
    }
}

void BeingHandler::processNameResponse2(Net::MessageIn &msg)
{
    if (!actorManager || !localPlayer)
        return;

    const int len = msg.readInt16("len");
    const int beingId = msg.readInt32("being id");
    const std::string str = msg.readString(len - 8, "name");
    Being *const dstBeing = actorManager->findBeing(beingId);
    if (dstBeing)
    {
        if (beingId == localPlayer->getId())
        {
            localPlayer->pingResponse();
        }
        else
        {
            dstBeing->setName(str);
            dstBeing->updateGuild();
            dstBeing->addToCache();

            if (dstBeing->getType() == ActorType::Player)
                dstBeing->updateColors();

            if (localPlayer)
            {
                const Party *const party = localPlayer->getParty();
                if (party && party->isMember(dstBeing->getId()))
                {
                    PartyMember *const member = party->getMember(
                        dstBeing->getId());

                    if (member)
                        member->setName(dstBeing->getName());
                }
                localPlayer->checkNewName(dstBeing);
            }
        }
    }
}

void BeingHandler::processPlayerUpdate1(Net::MessageIn &msg) const
{
    if (!actorManager || !localPlayer)
        return;

    // An update about a player, potentially including movement.
    const int id = msg.readInt32("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    uint32_t statusEffects = msg.readInt16("opt2");
    statusEffects |= (static_cast<uint16_t>(msg.readInt16("options"))) << 16U;
    const int16_t job = msg.readInt16("class");
    int disguiseId = 0;
    if (id < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);

    if (!dstBeing)
    {
        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing(id, job);

        if (!dstBeing)
            return;
    }
    else if (disguiseId)
    {
        actorManager->undelete(dstBeing);
        if (serverVersion < 1)
            requestNameById(id);
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
    dstBeing->setSubtype(job, 0);

    const int hairStyle = msg.readInt16("hair style");
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headBottom = msg.readInt16("head bottom");
    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const int hairColor = msg.readInt16("hair color");

    msg.readUInt8("unused?");
    msg.readUInt8("unused?");
    msg.readUInt8("unused?");
    msg.readUInt8("unused?");

    const int guild = msg.readInt32("guild");

    if (!guildManager || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(static_cast<int16_t>(guild)));
    }

    msg.readInt16("emblem");
    msg.readInt16("manner");
    dstBeing->setStatusEffectBlock(32, msg.readInt16("opt3"));
    msg.readUInt8("karma");
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(msg.readUInt8("gender")));

    if (!disguiseId)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_WEAPON, weapon, "", 1, true);
        if (!mHideShield)
            dstBeing->updateSprite(SPRITE_SHIELD, shield);
        dstBeing->updateSprite(SPRITE_BOTTOMCLOTHES, headBottom);
        dstBeing->updateSprite(SPRITE_TOPCLOTHES, headMid);
        dstBeing->updateSprite(SPRITE_HAT, headTop);
        dstBeing->updateSprite(SPRITE_HAIR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
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

    applyPlayerAction(dstBeing, msg.readUInt8("action type"));

    const int level = static_cast<int>(msg.readUInt8("level"));

    if (level)
        dstBeing->setLevel(level);

    msg.readUInt8("unknown");

    dstBeing->setActionTime(tick_time);

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffffU));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffffU));
}

void BeingHandler::processPlayerUpdate2(Net::MessageIn &msg) const
{
    if (!actorManager || !localPlayer)
        return;

    // An update about a player, potentially including movement.
    const int id = msg.readInt32("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    uint32_t statusEffects = msg.readInt16("opt2");
    statusEffects |= (static_cast<uint16_t>(msg.readInt16("options"))) << 16U;
    const int16_t job = msg.readInt16("class");
    int disguiseId = 0;
    if (id < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);

    if (!dstBeing)
    {
        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing(id, job);

        if (!dstBeing)
            return;
    }
    else if (disguiseId)
    {
        actorManager->undelete(dstBeing);
        if (serverVersion < 1)
            requestNameById(id);
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
    dstBeing->setSubtype(job, 0);

    const int hairStyle = msg.readInt16("hair style");
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headBottom = msg.readInt16("head bottom");

    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const int hairColor = msg.readInt16("hair color");

    msg.readUInt8("unused?");
    msg.readUInt8("unused?");
    msg.readUInt8("unused?");
    msg.readUInt8("unused?");

    const int guild = msg.readInt32("guild");

    if (!guildManager || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(static_cast<int16_t>(guild)));
    }

    msg.readInt16("emblem");
    msg.readInt16("manner");
    dstBeing->setStatusEffectBlock(32, msg.readInt16("opt3"));
    msg.readUInt8("karma");
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(msg.readUInt8("gender")));

    if (!disguiseId)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_WEAPON, weapon, "", 1, true);
        if (!mHideShield)
            dstBeing->updateSprite(SPRITE_SHIELD, shield);
        dstBeing->updateSprite(SPRITE_BOTTOMCLOTHES, headBottom);
        dstBeing->updateSprite(SPRITE_TOPCLOTHES, headMid);
        dstBeing->updateSprite(SPRITE_HAT, headTop);
        dstBeing->updateSprite(SPRITE_HAIR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
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

    applyPlayerAction(dstBeing, msg.readUInt8("action type"));
    const int level = static_cast<int>(msg.readUInt8("level"));

    if (level)
        dstBeing->setLevel(level);

    msg.readUInt8("unknown");

    dstBeing->setActionTime(tick_time);
    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffffU));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffffU));
}

void BeingHandler::processPlayerMove(Net::MessageIn &msg) const
{
    if (!actorManager || !localPlayer)
        return;

    // An update about a player, potentially including movement.
    const int id = msg.readInt32("account id");
    const int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    uint32_t statusEffects = msg.readInt16("opt2");
    statusEffects |= (static_cast<uint16_t>(msg.readInt16("options"))) << 16U;
    const int16_t job = msg.readInt16("class");
    int disguiseId = 0;
    if (id < 110000000 && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);

    if (!dstBeing)
    {
        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing(id, job);

        if (!dstBeing)
            return;
    }
    else if (disguiseId)
    {
        actorManager->undelete(dstBeing);
        if (serverVersion < 1)
            requestNameById(id);
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
    dstBeing->setSubtype(job, 0);

    const int hairStyle = msg.readInt16("hair style");
    const uint16_t weapon = msg.readInt16("weapon");
    const uint16_t shield = msg.readInt16("shield");
    const uint16_t headBottom = msg.readInt16("head bottom");

    msg.readInt32("tick");

    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const int hairColor = msg.readInt16("hair color");

    msg.readUInt8("unused?");
    msg.readUInt8("unused?");
    msg.readUInt8("unused?");
    msg.readUInt8("unused?");

    const int guild = msg.readInt32("guild");

    if (!guildManager || !GuildManager::getEnableGuildBot())
    {
        if (guild == 0)
            dstBeing->clearGuilds();
        else
            dstBeing->setGuild(Guild::getGuild(static_cast<int16_t>(guild)));
    }

    msg.readInt16("emblem");
    msg.readInt16("manner");
    dstBeing->setStatusEffectBlock(32, msg.readInt16("opt3"));
    msg.readUInt8("karma");
    // reserving bit for future usage
    dstBeing->setGender(Being::intToGender(msg.readUInt8("gender")));

    if (!disguiseId)
    {
        // Set these after the gender, as the sprites may be gender-specific
        dstBeing->updateSprite(SPRITE_WEAPON, weapon, "", 1, true);
        if (!mHideShield)
            dstBeing->updateSprite(SPRITE_SHIELD, shield);
        dstBeing->updateSprite(SPRITE_BOTTOMCLOTHES, headBottom);
        dstBeing->updateSprite(SPRITE_TOPCLOTHES, headMid);
        dstBeing->updateSprite(SPRITE_HAT, headTop);
        dstBeing->updateSprite(SPRITE_HAIR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
    }
    localPlayer->imitateOutfit(dstBeing);

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    msg.readUInt8("(sx<<4) | (sy&0x0f)");

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

    msg.readUInt8("unknown?");

    const int level = static_cast<int>(msg.readUInt8("level"));

    if (level)
        dstBeing->setLevel(level);

    msg.readUInt8("unknown");

    if (dstBeing->getType() != ActorType::Player)
        dstBeing->setActionTime(tick_time);

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffffU));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffffU));

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();
}

void BeingHandler::processBeingVisible(Net::MessageIn &msg)
{
    if (!actorManager)
        return;

    msg.readInt16("len");
    msg.readUInt8("object type");

    // Information about a being in range
    const int id = msg.readInt32("being id");
    int spawnId;
    if (id == mSpawnId)
        spawnId = mSpawnId;
    else
        spawnId = 0;
    mSpawnId = 0;

    int16_t speed = msg.readInt16("speed");
    const uint16_t stunMode = msg.readInt16("opt1");
    // probably wrong effect usage
    const uint32_t statusEffects = msg.readInt16("opt2");
    msg.readInt32("option");

    const int16_t job = msg.readInt16("class");

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
        if (job == 0 && id >= 110000000)
            return;

        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing(id, job);

        if (!dstBeing)
            return;

        if (job == 1022 && killStats)
            killStats->jackoAlive(dstBeing->getId());
    }
    else
    {
        // undeleting marked for deletion being
        if (dstBeing->getType() == ActorType::Npc)
            actorManager->undelete(dstBeing);
    }

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();

    if (spawnId)
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

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    dstBeing->setSubtype(job, 0);
    if (dstBeing->getType() == ActorType::Monster && localPlayer)
        localPlayer->checkNewName(dstBeing);

    const int hairStyle = msg.readInt16("hair style");
    const uint32_t weapon = static_cast<uint32_t>(msg.readInt32("weapon"));
    const uint16_t headBottom = msg.readInt16("head bottom");

    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const int hairColor = msg.readInt16("hair color");
    const uint16_t shoes = msg.readInt16("shoes or clothes color?");

    const uint16_t gloves = msg.readInt16("head dir / gloves");
    // may be use robe as gloves?
    msg.readInt16("robe");
    msg.readInt32("guild id");
    msg.readInt16("guild emblem");
    msg.readInt16("manner");
    dstBeing->setStatusEffectBlock(32, static_cast<uint16_t>(
        msg.readInt32("opt3")));
    msg.readUInt8("karma");
    uint8_t gender = msg.readUInt8("gender");

    if (dstBeing->getType() == ActorType::Player)
    {
        gender &= 3;
        dstBeing->setGender(Being::intToGender(gender));
        // Set these after the gender, as the sprites may be gender-specific
        setSprite(dstBeing, SPRITE_HAIR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        setSprite(dstBeing, SPRITE_BOTTOMCLOTHES, headBottom);
        setSprite(dstBeing, SPRITE_TOPCLOTHES, headMid);
        setSprite(dstBeing, SPRITE_HAT, headTop);
        setSprite(dstBeing, SPRITE_SHOE, shoes);
        setSprite(dstBeing, SPRITE_GLOVES, gloves);
        setSprite(dstBeing, SPRITE_WEAPON, weapon, "", 1, true);
//        if (!mHideShield)
//            setSprite(dstBeing, SPRITE_SHIELD, shield);
    }
    else if (dstBeing->getType() == ActorType::Npc)
    {
        setServerGender(dstBeing, gender);
    }

    uint8_t dir;
    uint16_t x, y;
    msg.readCoordinates(x, y, dir, "position");
    msg.readInt8("xs");
    msg.readInt8("ys");
    msg.readUInt8("action type");
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

    const int level = static_cast<int>(msg.readInt16("level"));
    if (level)
        dstBeing->setLevel(level);
    msg.readInt16("font");

    const int maxHP = msg.readInt32("max hp");
    const int hp = msg.readInt32("hp");
    dstBeing->setMaxHP(maxHP);
    dstBeing->setHP(hp);

    msg.readInt8("is boss");

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffffU));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffffU));
}

void BeingHandler::processBeingMove(Net::MessageIn &msg)
{
    if (!actorManager)
        return;

    msg.readInt16("len");
    msg.readUInt8("object type");

    // Information about a being in range
    const int id = msg.readInt32("being id");
    int spawnId;
    if (id == mSpawnId)
        spawnId = mSpawnId;
    else
        spawnId = 0;
    mSpawnId = 0;
    int16_t speed = msg.readInt16("speed");
//    if (visible)
//    {
        const uint16_t stunMode = msg.readInt16("opt1");
        // probably wrong effect usage
        const uint32_t statusEffects = msg.readInt16("opt2");
//    }
//    else
//    {
// commented for now, probably it can be removed after testing
//        msg.readInt16("body state");
//        msg.readInt16("health state");
//    }
    msg.readInt32("effect state");

    const int16_t job = msg.readInt16("class");

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
        if (job == 0 && id >= 110000000)
            return;

        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing(id, job);

        if (!dstBeing)
            return;

        if (job == 1022 && killStats)
            killStats->jackoAlive(dstBeing->getId());
    }
    else
    {
        // undeleting marked for deletion being
        if (dstBeing->getType() == ActorType::Npc)
            actorManager->undelete(dstBeing);
    }

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();

    if (spawnId)
        dstBeing->setAction(BeingAction::SPAWN, 0);

    // Prevent division by 0 when calculating frame
    if (speed == 0)
        speed = 150;

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    dstBeing->setSubtype(job, 0);
    if (dstBeing->getType() == ActorType::Monster && localPlayer)
        localPlayer->checkNewName(dstBeing);

    const int hairStyle = msg.readInt16("hair style");
    const uint32_t weapon = static_cast<uint32_t>(msg.readInt32("weapon"));
    const uint16_t headBottom = msg.readInt16("head bottom");

    msg.readInt32("tick");

    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const int hairColor = msg.readInt16("hair color");
    const uint16_t shoes = msg.readInt16("shoes or clothes color?");

    const uint16_t gloves = msg.readInt16("head dir / gloves");
    // may be use robe as gloves?
    msg.readInt16("robe");
    msg.readInt32("guild id");
    msg.readInt16("guild emblem");
    msg.readInt16("manner");
    dstBeing->setStatusEffectBlock(32, static_cast<uint16_t>(
        msg.readInt32("opt3")));
    msg.readUInt8("karma");
    uint8_t gender = msg.readUInt8("gender");

    if (dstBeing->getType() == ActorType::Player)
    {
        gender &= 3;
        dstBeing->setGender(Being::intToGender(gender));
        // Set these after the gender, as the sprites may be gender-specific
        setSprite(dstBeing, SPRITE_HAIR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        setSprite(dstBeing, SPRITE_BOTTOMCLOTHES, headBottom);
        setSprite(dstBeing, SPRITE_TOPCLOTHES, headMid);
        setSprite(dstBeing, SPRITE_HAT, headTop);
        setSprite(dstBeing, SPRITE_SHOE, shoes);
        setSprite(dstBeing, SPRITE_GLOVES, gloves);
        setSprite(dstBeing, SPRITE_WEAPON, weapon, "", 1, true);
//        if (!mHideShield)
//            setSprite(dstBeing, SPRITE_SHIELD, shield);
    }
    else if (dstBeing->getType() == ActorType::Npc)
    {
        setServerGender(dstBeing, gender);
    }

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    msg.readUInt8("(sx<<4) | (sy&0x0f)");
    msg.readInt8("xs");
    msg.readInt8("ys");
    dstBeing->setAction(BeingAction::STAND, 0);
    dstBeing->setTileCoords(srcX, srcY);
    dstBeing->setDestination(dstX, dstY);

    // because server don't send direction in move packet, we fixing it

    uint8_t d = 0;
    if (srcX == dstX && srcY == dstY)
    {   // if player did one step from invisible area to visible,
        // move path is broken
        int x2 = localPlayer->getTileX();
        int y2 = localPlayer->getTileY();
        if (abs(x2 - srcX) > abs(y2 - srcY))
            y2 = srcY;
        else
            x2 = srcX;
        d = dstBeing->calcDirection(x2, y2);
    }
    else
    {
        d = dstBeing->calcDirection(dstX, dstY);
    }
    if (d && dstBeing->getDirection() != d)
        dstBeing->setDirection(d);

    const int level = static_cast<int>(msg.readInt16("level"));
    if (level)
        dstBeing->setLevel(level);
    msg.readInt16("font");

    const int maxHP = msg.readInt32("max hp");
    const int hp = msg.readInt32("hp");
    dstBeing->setMaxHP(maxHP);
    dstBeing->setHP(hp);

    msg.readInt8("is boss");

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffffU));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffffU));
}

void BeingHandler::processBeingSpawn(Net::MessageIn &msg)
{
    if (!actorManager)
        return;

    msg.readInt16("len");
    msg.readUInt8("object type");

    // Information about a being in range
    const int id = msg.readInt32("being id");
    mSpawnId = id;
    const int spawnId = id;
    int16_t speed = msg.readInt16("speed");
//    if (visible)
//    {
        const uint16_t stunMode = msg.readInt16("opt1");
        // probably wrong effect usage
        const uint32_t statusEffects = msg.readInt16("opt2");
//    }
//    else
//    {
// commented for now, probably it can be removed after testing
//        msg.readInt16("body state");
//        msg.readInt16("health state");
//    }
    msg.readInt32("effect state");

    const int16_t job = msg.readInt16("class");

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
        if (job == 0 && id >= 110000000)
            return;

        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing(id, job);

        if (!dstBeing)
            return;

        if (job == 1022 && killStats)
            killStats->jackoAlive(dstBeing->getId());
    }
    else
    {
        // undeleting marked for deletion being
        if (dstBeing->getType() == ActorType::Npc)
            actorManager->undelete(dstBeing);
    }

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();

    if (spawnId)
    {
        dstBeing->setAction(BeingAction::SPAWN, 0);
    }

    // Prevent division by 0 when calculating frame
    if (speed == 0)
        speed = 150;

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    dstBeing->setSubtype(job, 0);
    if (dstBeing->getType() == ActorType::Monster && localPlayer)
        localPlayer->checkNewName(dstBeing);

    const int hairStyle = msg.readInt16("hair style");
    const uint32_t weapon = static_cast<uint32_t>(msg.readInt32("weapon"));
    const uint16_t headBottom = msg.readInt16("head bottom");

    const uint16_t headTop = msg.readInt16("head top");
    const uint16_t headMid = msg.readInt16("head mid");
    const int hairColor = msg.readInt16("hair color");
    const uint16_t shoes = msg.readInt16("shoes or clothes color?");

    const uint16_t gloves = msg.readInt16("head dir / gloves");
    // may be use robe as gloves?
    msg.readInt16("robe");
    msg.readInt32("guild id");
    msg.readInt16("guild emblem");
    msg.readInt16("manner");
    dstBeing->setStatusEffectBlock(32, static_cast<uint16_t>(
        msg.readInt32("opt3")));
    msg.readUInt8("karma");
    uint8_t gender = msg.readUInt8("gender");

    if (dstBeing->getType() == ActorType::Player)
    {
        gender &= 3;
        dstBeing->setGender(Being::intToGender(gender));
        // Set these after the gender, as the sprites may be gender-specific
        setSprite(dstBeing, SPRITE_HAIR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        setSprite(dstBeing, SPRITE_BOTTOMCLOTHES, headBottom);
        setSprite(dstBeing, SPRITE_TOPCLOTHES, headMid);
        setSprite(dstBeing, SPRITE_HAT, headTop);
        setSprite(dstBeing, SPRITE_SHOE, shoes);
        setSprite(dstBeing, SPRITE_GLOVES, gloves);
        setSprite(dstBeing, SPRITE_WEAPON, weapon, "", 1, true);
//        if (!mHideShield)
//            setSprite(dstBeing, SPRITE_SHIELD, shield);
    }
    else if (dstBeing->getType() == ActorType::Npc)
    {
        setServerGender(dstBeing, gender);
    }

    uint8_t dir;
    uint16_t x, y;
    msg.readCoordinates(x, y, dir, "position");
    msg.readInt8("xs");
    msg.readInt8("ys");
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

    const int level = static_cast<int>(msg.readInt16("level"));
    if (level)
        dstBeing->setLevel(level);
    msg.readInt16("font");

    const int maxHP = msg.readInt32("max hp");
    const int hp = msg.readInt32("hp");
    dstBeing->setMaxHP(maxHP);
    dstBeing->setHP(hp);

    msg.readInt8("is boss");

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffffU));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffffU));
}

void BeingHandler::processMapTypeProperty(Net::MessageIn &msg) const
{
    msg.readInt16("type");
    // need get pvp and other flags from here
    msg.readInt32("flags");
}

void BeingHandler::processSkillCasting(Net::MessageIn &msg) const
{
    msg.readInt32("src id");
    msg.readInt32("dst id");
    msg.readInt16("dst x");
    msg.readInt16("dst y");
    msg.readInt16("skill id");
    msg.readInt32("property");  // can be used to trigger effect
    msg.readInt32("cast time");
    msg.readInt8("dispossable");
}

void BeingHandler::processBeingStatusChange(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingStatusChange")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingStatusChange")
        return;
    }

    const bool status1 = msg.getId() == SMSG_BEING_STATUS_CHANGE;

    // Status change
    const uint16_t status = msg.readInt16("status");
    const int id = msg.readInt32("being id");
    const bool flag = msg.readUInt8("flag: 0: stop, 1: start");
    if (status1)
        msg.readInt32("total");
    msg.readInt32("left");
    msg.readInt32("val1");
    msg.readInt32("val2");
    msg.readInt32("val3");

    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing)
        dstBeing->setStatusEffect(status, flag);
    BLOCK_END("BeingHandler::processBeingStatusChange")
}

void BeingHandler::processBeingMove2(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingMove2")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingMove2")
        return;
    }

    /*
      * A simplified movement packet, used by the
      * later versions of eAthena for both mobs and
      * players
      */
    Being *const dstBeing = actorManager->findBeing(msg.readInt32("being id"));

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    msg.readUInt8("(sx<<4) | (sy&0x0f)");
    msg.readInt32("tick");

    /*
      * This packet doesn't have enough info to actually
      * create a new being, so if the being isn't found,
      * we'll just pretend the packet didn't happen
      */

    if (!dstBeing)
    {
        BLOCK_END("BeingHandler::processBeingMove2")
        return;
    }

    dstBeing->setAction(BeingAction::STAND, 0);
    dstBeing->setTileCoords(srcX, srcY);
    dstBeing->setDestination(dstX, dstY);
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();
    BLOCK_END("BeingHandler::processBeingMove2")
}

void BeingHandler::processBeingAction2(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingAction")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingAction")
        return;
    }

    Being *const srcBeing = actorManager->findBeing(
        msg.readInt32("src being id"));
    Being *const dstBeing = actorManager->findBeing(
        msg.readInt32("dst being id"));

    msg.readInt32("tick");
    const int srcSpeed = msg.readInt32("src speed");
    msg.readInt32("dst speed");
    const int param1 = msg.readInt32("damage");
    msg.readInt16("count");
    const uint8_t type = msg.readUInt8("action");
    msg.readInt32("left damage");

    switch (type)
    {
        case Being::HIT:  // Damage
        case Being::CRITICAL:  // Critical Damage
        case Being::MULTI:  // Critical Damage
        case Being::REFLECT:  // Reflected Damage
        case Being::FLEE:  // Lucky Dodge
            if (srcBeing)
            {
                if (srcSpeed && srcBeing->getType() == ActorType::Player)
                    srcBeing->setAttackDelay(srcSpeed);
                // attackid=1, type
                srcBeing->handleAttack(dstBeing, param1, 1);
                if (srcBeing->getType() == ActorType::Player)
                    srcBeing->setAttackTime();
            }
            if (dstBeing)
            {
                dstBeing->takeDamage(srcBeing, param1,
                    static_cast<Being::AttackType>(type));
            }
            break;

        case 0x01:  // dead or pickup?
            break;
            // tmw server can send here garbage?
//            if (srcBeing)
//                srcBeing->setAction(BeingAction::DEAD, 0);

        case 0x02:  // Sit
            if (srcBeing)
            {
                srcBeing->setAction(BeingAction::SIT, 0);
                if (srcBeing->getType() == ActorType::Player)
                {
                    srcBeing->setMoveTime();
                    if (localPlayer)
                        localPlayer->imitateAction(srcBeing, BeingAction::SIT);
                }
            }
            break;

        case 0x03:  // Stand up
            if (srcBeing)
            {
                srcBeing->setAction(BeingAction::STAND, 0);
                if (srcBeing->getType() == ActorType::Player)
                {
                    srcBeing->setMoveTime();
                    if (localPlayer)
                    {
                        localPlayer->imitateAction(srcBeing,
                            BeingAction::STAND);
                    }
                }
            }
            break;
        default:
            logger->log("QQQ1 SMSG_BEING_ACTION:");
            if (srcBeing)
                logger->log("srcBeing:" + toString(srcBeing->getId()));
            if (dstBeing)
                logger->log("dstBeing:" + toString(dstBeing->getId()));
            logger->log("type: " + toString(type));
            break;
    }
    BLOCK_END("BeingHandler::processBeingAction")
}

void BeingHandler::processMonsterHp(Net::MessageIn &msg) const
{
    Being *const dstBeing = actorManager->findBeing(
        msg.readInt32("monster id"));
    const int hp = msg.readInt32("hp");
    const int maxHP = msg.readInt32("max hp");
    if (dstBeing)
    {
        dstBeing->setHP(hp);
        dstBeing->setMaxHP(maxHP);
    }
}

void BeingHandler::processSkillAutoCast(Net::MessageIn &msg) const
{
    msg.readInt16("skill id");
    msg.readInt16("inf");
    msg.readInt16("unused");
    msg.readInt16("skill level");
    msg.readInt16("sp");
    msg.readInt16("range");
    msg.readString(24, "skill name");
    msg.readInt8("unused");
}

void BeingHandler::processRanksList(Net::MessageIn &msg) const
{
    // +++ here need window with rank tables.
    msg.readInt16("rank type");
    for (int f = 0; f < 10; f ++)
    {
        msg.readString(24, "name");
        msg.readInt32("points");
    }
    msg.readInt32("my points");
}

void BeingHandler::processBeingChangeDirection(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingChangeDirection")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingChangeDirection")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(msg.readInt32("being id"));

    msg.readInt16("head direction");

    const uint8_t dir = static_cast<uint8_t>(
        msg.readUInt8("player direction") & 0x0FU);

    if (!dstBeing)
    {
        BLOCK_END("BeingHandler::processBeingChangeDirection")
        return;
    }

    dstBeing->setDirection(dir);
    if (localPlayer)
        localPlayer->imitateDirection(dstBeing, dir);
    BLOCK_END("BeingHandler::processBeingChangeDirection")
}

void BeingHandler::processBeingSpecialEffect(Net::MessageIn &msg) const
{
    // +++ need somhow show this effects.
    // type is not same with self/misc effect.
    msg.readInt32("account id");
    msg.readInt32("effect type");
}

void BeingHandler::setServerGender(Being *const being, const uint8_t gender)
{
    switch (gender)
    {
        case 2:
            being->setGender(Gender::FEMALE);
            break;
        case 3:
            being->setGender(Gender::MALE);
            break;
        default:
            being->setGender(Gender::UNSPECIFIED);
            break;
    }
}

void BeingHandler::applyPlayerAction(Being *const being, const uint8_t type)
{
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
            // need set stand state?
            logger->log("QQQ2 SMSG_PLAYER_UPDATE_1:" + toString(type));
            logger->log("being id:" + toString(being->getId()));
            logger->log("being name:" + being->getName());
            break;
    }
}

void BeingHandler::viewPlayerEquipment(const Being *const being)
{
    if (!being)
        return;

    MessageOut outMsg(CMSG_PLAYER_VIEW_EQUIPMENT);
    outMsg.writeInt32(being->getId(), "account id");
}

void BeingHandler::processSkillGroundNoDamage(Net::MessageIn &msg) const
{
    msg.readInt16("skill id");
    msg.readInt32("src id");
    msg.readInt16("val");
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readInt32("tick");
}

void BeingHandler::processSkillEntry(Net::MessageIn &msg) const
{
    msg.readInt16("len");
    msg.readInt32("accound id");
    msg.readInt32("creator accound id");
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readInt32("job");
    msg.readUInt8("radius");
    msg.readUInt8("visible");
    msg.readUInt8("level");
}

void BeingHandler::processPlaterStatusChange(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processPlayerStop")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processPlayerStop")
        return;
    }

    // Change in players' flags
    const int id = msg.readInt32("account id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    const uint16_t stunMode = msg.readInt16("stun mode");
    uint32_t statusEffects = msg.readInt16("status effect");
    statusEffects |= (static_cast<uint32_t>(msg.readInt32("opt?"))) << 16;
    msg.readUInt8("karma");

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));
    BLOCK_END("BeingHandler::processPlayerStop")
}

void BeingHandler::processPlaterStatusChangeNoTick(Net::MessageIn &msg) const
{
    // +++ probably need show some effect?
    msg.readInt16("index");
    msg.readInt32("account id");
    msg.readUInt8("state");
}

void BeingHandler::processBeingResurrect(Net::MessageIn &msg) const
{
    BLOCK_START("BeingHandler::processBeingResurrect")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingHandler::processBeingResurrect")
        return;
    }

    // A being changed mortality status

    const int id = msg.readInt32("being id");
    msg.readInt16("unused");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
    {
        BLOCK_END("BeingHandler::processBeingResurrect")
        return;
    }

    // If this is player's current target, clear it.
    if (dstBeing == localPlayer->getTarget())
        localPlayer->stopAttack();

    dstBeing->setAction(BeingAction::STAND, 0);
    BLOCK_END("BeingHandler::processBeingResurrect")
}

}  // namespace EAthena
