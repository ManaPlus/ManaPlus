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

#include "net/ea/beinghandler.h"

#include "net/ea/eaprotocol.h"

#include "actormanager.h"
#include "configuration.h"
#include "effectmanager.h"
#include "game.h"
#include "guildmanager.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/playerrelations.h"

#include "particle/particle.h"

#include "input/keyboardconfig.h"

#include "gui/windows/botcheckerwindow.h"
#include "gui/windows/outfitwindow.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/killstats.h"
#include "gui/windows/questswindow.h"

#include "utils/timer.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "debug.h"

extern int serverVersion;

namespace Ea
{
BeingHandler::BeingHandler(const bool enableSync) :
    mSync(enableSync),
    mSpawnId(0),
    mHideShield(config.getBoolValue("hideShield"))
{
}

Being *BeingHandler::createBeing(const int id, const int16_t job) const
{
    if (!actorManager)
        return nullptr;

    ActorSprite::Type type = ActorSprite::UNKNOWN;
    if (job <= 25 || (job >= 4001 && job <= 4049))
        type = ActorSprite::PLAYER;
    else if (job >= 46 && job <= 1000)
        type = ActorSprite::NPC;
    else if (job > 1000 && job <= 2000)
        type = ActorSprite::MONSTER;
    else if (job == 45)
        type = ActorSprite::PORTAL;

    Being *const being = actorManager->createBeing(id, type, job);

    if (type == ActorSprite::PLAYER || type == ActorSprite::NPC)
    {
        being->updateFromCache();
        requestNameById(id);
        if (player_node)
            player_node->checkNewName(being);
    }
    if (type == Being::PLAYER)
    {
        if (botCheckerWindow)
            botCheckerWindow->updateList();
        if (socialWindow)
            socialWindow->updateActiveList();
    }
    else if (type == Being::NPC)
    {
        if (questsWindow)
            questsWindow->addEffect(being);
    }
    return being;
}

void BeingHandler::setSprite(Being *const being, const unsigned int slot,
                             const int id, const std::string &color,
                             const unsigned char colorId,
                             const bool isWeapon,
                             const bool isTempSprite) const
{
    if (!being)
        return;
    being->updateSprite(slot, id, color, colorId, isWeapon, isTempSprite);
}

void BeingHandler::processBeingVisibleOrMove(Net::MessageIn &msg,
                                             const bool visible)
{
    if (!actorManager)
        return;

    int spawnId;

    // Information about a being in range
    const int id = msg.readInt32();
    if (id == mSpawnId)
        spawnId = mSpawnId;
    else
        spawnId = 0;
    mSpawnId = 0;
    int16_t speed = msg.readInt16();
    const uint16_t stunMode = msg.readInt16();  // opt1
    uint32_t statusEffects = msg.readInt16();  // opt2
    statusEffects |= (static_cast<uint32_t>(msg.readInt16())) << 16;  // option
    const int16_t job = msg.readInt16();  // class
    int disguiseId = 0;
    if (id == player_node->getId() && job >= 1000)
        disguiseId = job;

    Being *dstBeing = actorManager->findBeing(id);

    if (dstBeing && dstBeing->getType() == Being::MONSTER
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
        if (dstBeing->getType() == Being::NPC)
        {
            actorManager->undelete(dstBeing);
            if (serverVersion < 1)
                requestNameById(id);
        }
    }

    if (dstBeing->getType() == Being::PLAYER)
        dstBeing->setMoveTime();

    if (spawnId)
    {
        dstBeing->setAction(Being::SPAWN, 0);
    }
    else if (visible)
    {
        dstBeing->clearPath();
        dstBeing->setActionTime(tick_time);
        dstBeing->setAction(Being::STAND, 0);
    }

    // Prevent division by 0 when calculating frame
    if (speed == 0)
        speed = 150;

    const int hairStyle = msg.readInt8();
    const int look = msg.readInt8();
    dstBeing->setSubtype(job, look);
    if (dstBeing->getType() == ActorSprite::MONSTER && player_node)
        player_node->checkNewName(dstBeing);
    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    const uint16_t weapon = msg.readInt16();
    const uint16_t headBottom = msg.readInt16();

    if (!visible)
        msg.readInt32();  // server tick

    const uint16_t shield = msg.readInt16();
    const uint16_t headTop = msg.readInt16();
    const uint16_t headMid = msg.readInt16();
    const int hairColor = msg.readInt8();
    msg.readInt8();  // free
    const uint16_t shoes = msg.readInt16();  // clothes color

    uint16_t gloves;
    if (dstBeing->getType() == ActorSprite::MONSTER)
    {
        if (serverVersion > 0)
        {
            const int hp = msg.readInt32();
            const int maxHP = msg.readInt32();
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
            msg.readInt32();
            msg.readInt32();
        }
        gloves = 0;
    }
    else
    {
        gloves = msg.readInt16();  // head dir - "abused" as gloves
        msg.readInt32();  // guild
        msg.readInt16();  // guild emblem
    }
//            logger->log("being guild: " + toString(guild));

    msg.readInt16();  // manner
    dstBeing->setStatusEffectBlock(32, msg.readInt16());  // opt3
    if (serverVersion > 0 && dstBeing->getType() == ActorSprite::MONSTER)
    {
        const int attackRange = msg.readInt8();   // karma
        dstBeing->setAttackRange(attackRange);
    }
    else
    {
        msg.readInt8();   // karma
    }
    uint8_t gender = msg.readInt8();

    if (!disguiseId && dstBeing->getType() == ActorSprite::PLAYER)
    {
        // reserving bits for future usage
        gender &= 3;
        dstBeing->setGender(Being::intToGender(gender));
        // Set these after the gender, as the sprites may be gender-specific
        setSprite(dstBeing, EA_SPRITE_HAIR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        dstBeing->setHairColor(hairColor);
        setSprite(dstBeing, EA_SPRITE_BOTTOMCLOTHES, headBottom);
        setSprite(dstBeing, EA_SPRITE_TOPCLOTHES, headMid);
        setSprite(dstBeing, EA_SPRITE_HAT, headTop);
        setSprite(dstBeing, EA_SPRITE_SHOE, shoes);
        setSprite(dstBeing, EA_SPRITE_GLOVES, gloves);
        setSprite(dstBeing, EA_SPRITE_WEAPON, weapon, "", 1, true);
        if (!mHideShield)
            setSprite(dstBeing, EA_SPRITE_SHIELD, shield);
    }
    else if (dstBeing->getType() == ActorSprite::NPC)
    {
        switch (gender)
        {
            case 2:
                dstBeing->setGender(GENDER_FEMALE);
                break;
            case 3:
                dstBeing->setGender(GENDER_MALE);
                break;
            case 4:
                dstBeing->setGender(GENDER_OTHER);
                break;
            default:
                dstBeing->setGender(GENDER_UNSPECIFIED);
                break;
        }
    }

    if (!visible)
    {
        uint16_t srcX, srcY, dstX, dstY;
        msg.readCoordinatePair(srcX, srcY, dstX, dstY);
        if (!disguiseId)
        {
            dstBeing->setAction(Being::STAND, 0);
            dstBeing->setTileCoords(srcX, srcY);
            if (serverVersion < 10)
                dstBeing->setDestination(dstX, dstY);
        }
    }
    else
    {
        uint8_t dir;
        uint16_t x, y;
        msg.readCoordinates(x, y, dir);
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
    }

    msg.readInt8();   // unknown
    msg.readInt8();   // unknown
    msg.readInt16();

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));
}

void BeingHandler::processBeingMove2(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    /*
      * A simplified movement packet, used by the
      * later versions of eAthena for both mobs and
      * players
      */
    Being *const dstBeing = actorManager->findBeing(msg.readInt32());

    /*
      * This packet doesn't have enough info to actually
      * create a new being, so if the being isn't found,
      * we'll just pretend the packet didn't happen
      */

    if (!dstBeing)
        return;

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY);
    msg.readInt32();  // Server tick

    dstBeing->setAction(Being::STAND, 0);
    dstBeing->setTileCoords(srcX, srcY);
    dstBeing->setDestination(dstX, dstY);
    if (dstBeing->getType() == Being::PLAYER)
        dstBeing->setMoveTime();
}

void BeingHandler::processBeingSpawn(Net::MessageIn &msg)
{
    // skipping this packet
    mSpawnId = msg.readInt32();    // id
    msg.readInt16();    // speed
    msg.readInt16();  // opt1
    msg.readInt16();  // opt2
    msg.readInt16();  // option
    msg.readInt16();    // disguise
}

void BeingHandler::processBeingRemove(Net::MessageIn &msg) const
{
    if (!actorManager || !player_node)
        return;

    // A being should be removed or has died

    const int id = msg.readInt32();
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    player_node->followMoveTo(dstBeing, player_node->getNextDestX(),
        player_node->getNextDestY());

    // If this is player's current target, clear it.
    if (dstBeing == player_node->getTarget())
        player_node->stopAttack(true);

    if (msg.readInt8() == 1)
    {
        if (dstBeing->getCurrentAction() != Being::DEAD)
        {
            dstBeing->setAction(Being::DEAD, 0);
            dstBeing->recalcSpritesOrder();
        }
        if (dstBeing->getName() == "Jack O" && killStats)
            killStats->jackoDead(id);
    }
    else
    {
        if (dstBeing->getType() == Being::PLAYER)
        {
            if (botCheckerWindow)
                botCheckerWindow->updateList();
            if (socialWindow)
                socialWindow->updateActiveList();
        }
        actorManager->destroy(dstBeing);
    }
}

void BeingHandler::processBeingResurrect(Net::MessageIn &msg) const
{
    if (!actorManager || !player_node)
        return;

    // A being changed mortality status

    const int id = msg.readInt32();
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    // If this is player's current target, clear it.
    if (dstBeing == player_node->getTarget())
        player_node->stopAttack();

    if (msg.readInt8() == 1)
        dstBeing->setAction(Being::STAND, 0);
}


void BeingHandler::processSkillDamage(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    const int id = msg.readInt16();  // Skill Id
    Being *const srcBeing = actorManager->findBeing(msg.readInt32());
    Being *const dstBeing = actorManager->findBeing(msg.readInt32());
    msg.readInt32();  // Server tick
    msg.readInt32();  // src speed
    msg.readInt32();  // dst speed
    const int param1 = msg.readInt32();  // Damage
    const int level = msg.readInt16();  // Skill level
    msg.readInt16();  // Div
    msg.readInt8();  // Skill hit/type (?)
    if (srcBeing)
        srcBeing->handleSkill(dstBeing, param1, id, level);
    if (dstBeing)
        dstBeing->takeDamage(srcBeing, param1, Being::SKILL, id);
}

void BeingHandler::processBeingAction(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    Being *const srcBeing = actorManager->findBeing(msg.readInt32());
    Being *const dstBeing = actorManager->findBeing(msg.readInt32());

    msg.readInt32();   // server tick
    const int srcSpeed = msg.readInt32();   // src speed
    msg.readInt32();   // dst speed
    const int param1 = msg.readInt16();
    msg.readInt16();  // param 2
    const int type = msg.readInt8();
    msg.readInt16();  // param 3

    switch (type)
    {
        case Being::HIT:  // Damage
        case Being::CRITICAL:  // Critical Damage
        case Being::MULTI:  // Critical Damage
        case Being::REFLECT:  // Reflected Damage
        case Being::FLEE:  // Lucky Dodge
            if (srcBeing)
            {
                if (srcSpeed && srcBeing->getType() == Being::PLAYER)
                    srcBeing->setAttackDelay(srcSpeed);
                // attackid=1, type
                srcBeing->handleAttack(dstBeing, param1, 1);
                if (srcBeing->getType() == Being::PLAYER)
                    srcBeing->setAttackTime();
            }
            if (dstBeing)
            {
                dstBeing->takeDamage(srcBeing, param1,
                    static_cast<Being::AttackType>(type));
            }
            break;

        case 0x01:  // dead
            break;
            // tmw server can send here garbage?
//            if (srcBeing)
//                srcBeing->setAction(Being::DEAD, 0);

        case 0x02:  // Sit
            if (srcBeing)
            {
                srcBeing->setAction(Being::SIT, 0);
                if (srcBeing->getType() == Being::PLAYER)
                {
                    srcBeing->setMoveTime();
                    if (player_node)
                        player_node->imitateAction(srcBeing, Being::SIT);
                }
            }
            break;

        case 0x03:  // Stand up
            if (srcBeing)
            {
                srcBeing->setAction(Being::STAND, 0);
                if (srcBeing->getType() == Being::PLAYER)
                {
                    srcBeing->setMoveTime();
                    if (player_node)
                        player_node->imitateAction(srcBeing, Being::STAND);
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
}

void BeingHandler::processBeingSelfEffect(Net::MessageIn &msg) const
{
    if (!effectManager || !actorManager)
        return;

    const int id = static_cast<uint32_t>(msg.readInt32());
    Being *const being = actorManager->findBeing(id);
    if (!being)
        return;

    const int effectType = msg.readInt32();

    if (Particle::enabled)
        effectManager->trigger(effectType, being);

    // +++ need dehard code effectType == 3
    if (effectType == 3 && being->getType() == Being::PLAYER
        && socialWindow)
    {   // reset received damage
        socialWindow->resetDamage(being->getName());
    }
}

void BeingHandler::processBeingEmotion(Net::MessageIn &msg) const
{
    if (!player_node || !actorManager)
        return;

    Being *const dstBeing = actorManager->findBeing(msg.readInt32());
    if (!dstBeing)
        return;

    if (player_relations.hasPermission(dstBeing, PlayerRelation::EMOTE))
    {
        const unsigned char emote = msg.readInt8();
        if (emote)
        {
            dstBeing->setEmote(emote, 0);
            player_node->imitateEmote(dstBeing, emote);
        }
    }
    if (dstBeing->getType() == Being::PLAYER)
        dstBeing->setOtherTime();
}

void BeingHandler::processNameResponse(Net::MessageIn &msg) const
{
    if (!player_node || !actorManager)
        return;

    const int beingId = msg.readInt32();
    Being *const dstBeing = actorManager->findBeing(beingId);

    if (dstBeing)
    {
        if (beingId == player_node->getId())
        {
            player_node->pingResponse();
        }
        else
        {
            dstBeing->setName(msg.readString(24));
            dstBeing->updateGuild();
            dstBeing->addToCache();

            if (dstBeing->getType() == Being::PLAYER)
                dstBeing->updateColors();

            if (player_node)
            {
                const Party *const party = player_node->getParty();
                if (party && party->isMember(dstBeing->getId()))
                {
                    PartyMember *const member = party->getMember(
                        dstBeing->getId());

                    if (member)
                        member->setName(dstBeing->getName());
                }
                player_node->checkNewName(dstBeing);
            }
        }
    }
}

void BeingHandler::processIpResponse(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    Being *const dstBeing = actorManager->findBeing(msg.readInt32());
    if (dstBeing)
        dstBeing->setIp(ipToString(msg.readInt32()));
}

void BeingHandler::processPlayerGuilPartyInfo(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    Being *const dstBeing = actorManager->findBeing(msg.readInt32());

    if (dstBeing)
    {
        dstBeing->setPartyName(msg.readString(24));
        if (!guildManager || !GuildManager::getEnableGuildBot())
        {
            dstBeing->setGuildName(msg.readString(24));
            dstBeing->setGuildPos(msg.readString(24));
        }
        else
        {
            msg.skip(48);
        }
        dstBeing->addToCache();
        msg.readString(24);  // Discard this
    }
}

void BeingHandler::processBeingChangeDirection(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    Being *const dstBeing = actorManager->findBeing(msg.readInt32());

    if (!dstBeing)
        return;

    msg.readInt16();  // unused

    const unsigned char dir = msg.readInt8() & 0x0f;
    dstBeing->setDirection(dir);
    if (player_node)
        player_node->imitateDirection(dstBeing, dir);
}

void BeingHandler::processPlayerStop(Net::MessageIn &msg) const
{
    if (!actorManager || !player_node)
        return;

    /*
      *  Instruction from server to stop walking at x, y.
      *
      *  Some people like having this enabled.  Others absolutely
      *  despise it.  So I'm setting to so that it only affects the
      *  local player if the person has set a key "EnableSync" to "1"
      *  in their config.xml file.
      *
      *  This packet will be honored for all other beings, regardless
      *  of the config setting.
      */

    const int id = msg.readInt32();

    if (mSync || id != player_node->getId())
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing)
        {
            const uint16_t x = msg.readInt16();
            const uint16_t y = msg.readInt16();
            dstBeing->setTileCoords(x, y);
            if (dstBeing->getCurrentAction() == Being::MOVE)
                dstBeing->setAction(Being::STAND, 0);
        }
    }
}

void BeingHandler::processPlayerMoveToAttack(Net::MessageIn &msg A_UNUSED)
                                             const
{
    /*
      * This is an *advisory* message, telling the client that
      * it needs to move the character before attacking
      * a target (out of range, obstruction in line of fire).
      * We can safely ignore this...
      */
    if (player_node)
        player_node->fixAttackTarget();
}

void BeingHandler::processPlaterStatusChange(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    // Change in players' flags
    const int id = msg.readInt32();
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    const uint16_t stunMode = msg.readInt16();
    uint32_t statusEffects = msg.readInt16();
    statusEffects |= (static_cast<uint32_t>(msg.readInt16())) << 16;
    msg.readInt8();  // Unused?

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));
}

void BeingHandler::processBeingStatusChange(Net::MessageIn &msg) const
{
    if (!actorManager)
        return;

    // Status change
    const uint16_t status = msg.readInt16();
    const int id = msg.readInt32();
    const int flag = msg.readInt8();  // 0: stop, 1: start

    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing)
        dstBeing->setStatusEffect(status, flag);
}

void BeingHandler::processSkilCasting(Net::MessageIn &msg) const
{
    msg.readInt32();    // src id
    msg.readInt32();    // dst id
    msg.readInt16();    // dst x
    msg.readInt16();    // dst y
    msg.readInt16();    // skill num
    msg.readInt32();    // skill get pl
    msg.readInt32();    // cast time
}

void BeingHandler::processSkillNoDamage(Net::MessageIn &msg) const
{
    msg.readInt16();    // skill id
    msg.readInt16();    // heal
    msg.readInt32();    // dst id
    msg.readInt32();    // src id
    msg.readInt8();     // fail
}

void BeingHandler::processPvpMapMode(Net::MessageIn &msg) const
{
    const Game *const game = Game::instance();
    if (!game)
        return;

    Map *const map = game->getCurrentMap();
    if (map)
        map->setPvpMode(msg.readInt16());
}

void BeingHandler::processPvpSet(Net::MessageIn &msg) const
{
    const int id = msg.readInt32();    // id
    const int rank = msg.readInt32();  // rank
    msg.readInt32();             // num
    if (actorManager)
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing)
            dstBeing->setPvpRank(rank);
    }
}

}  // namespace Ea
