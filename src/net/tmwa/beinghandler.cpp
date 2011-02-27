/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "net/tmwa/beinghandler.h"

#include "actorspritemanager.h"
#include "being.h"
#include "client.h"
#include "effectmanager.h"
#include "guild.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "log.h"
#include "party.h"
#include "playerrelations.h"
#include "configuration.h"

#include "gui/botcheckerwindow.h"
#include "gui/outfitwindow.h"
#include "gui/socialwindow.h"
#include "gui/killstats.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "net/playerhandler.h"
#include "net/net.h"

#include "net/tmwa/protocol.h"

#include "resources/colordb.h"

#include <iostream>

extern Net::BeingHandler *beingHandler;

namespace TmwAthena
{

const int EMOTION_TIME = 500;    /**< Duration of emotion icon */

Being *createBeing(int id, short job);

BeingHandler::BeingHandler(bool enableSync):
   mSync(enableSync)
{
    static const Uint16 _messages[] =
    {
        SMSG_BEING_VISIBLE,
        SMSG_BEING_MOVE,
        SMSG_BEING_MOVE2,
        SMSG_BEING_REMOVE,
        SMSG_SKILL_DAMAGE,
        SMSG_BEING_ACTION,
        SMSG_BEING_SELFEFFECT,
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
        SMSG_BEING_STATUS_CHANGE,
        SMSG_BEING_RESURRECT,
        SMSG_SOLVE_CHAR_NAME,
        SMSG_BEING_SPAWN,
        SMSG_SKILL_CASTING,
        SMSG_SKILL_CAST_CANCEL,
        SMSG_SKILL_NO_DAMAGE,
        SMSG_BEING_IP_RESPONSE,
        SMSG_PVP_MAP_MODE,
        SMSG_PVP_SET,
        0
    };
    handledMessages = _messages;
    beingHandler = this;
}

Being *createBeing(int id, short job)
{
    if (!actorSpriteManager)
        return 0;

    ActorSprite::Type type = ActorSprite::UNKNOWN;
    if (job <= 25 || (job >= 4001 && job <= 4049))
        type = ActorSprite::PLAYER;
    else if (job >= 46 && job <= 1000)
        type = ActorSprite::NPC;
    else if (job > 1000 && job <= 2000)
        type = ActorSprite::MONSTER;
    else if (job == 45)
        type = ActorSprite::PORTAL;

    Being *being = actorSpriteManager->createBeing(id, type, job);

    if (type == ActorSprite::PLAYER || type == ActorSprite::NPC)
    {
        if (!being->updateFromCache())
        {
            MessageOut outMsg(0x0094);
            outMsg.writeInt32(id); //readLong(2));
        }
        else
        {
            if (player_node)
                player_node->checkNewName(being);
        }
    }
    if (type == Being::PLAYER)
    {
        if (botCheckerWindow)
            botCheckerWindow->updateList();
        if (socialWindow)
            socialWindow->updateActiveList();
    }
    return being;
}

void BeingHandler::requestNameById(int id)
{
    MessageOut outMsg(0x0094);
    outMsg.writeInt32(id); //readLong(2));
}

void BeingHandler::handleMessage(Net::MessageIn &msg)
{
    if (!actorSpriteManager)
        return;

    int id;
    short job, speed, gender;
    Uint16 headTop, headMid, headBottom;
    Uint16 shoes, gloves;
    Uint16 weapon, shield;
    Uint16 gmstatus;
    int param1;
    Uint16 stunMode;
    int level;
    Uint32 statusEffects;
    int type, guild;
    Uint16 status;
    Being *srcBeing, *dstBeing;
    int hairStyle, hairColor, flag;
    int hp, maxHP, oldHP;

    switch (msg.getId())
    {
        case SMSG_BEING_VISIBLE:
        case SMSG_BEING_MOVE:
            // Information about a being in range
            id = msg.readInt32();
            speed = msg.readInt16();
            stunMode = msg.readInt16();  // opt1
            statusEffects = msg.readInt16();  // opt2
            statusEffects |= ((Uint32)msg.readInt16()) << 16;  // option
            job = msg.readInt16();  // class

            dstBeing = actorSpriteManager->findBeing(id);

            if (dstBeing && dstBeing->getType() == Being::MONSTER
                && !dstBeing->isAlive())
            {
                actorSpriteManager->destroy(dstBeing);
                actorSpriteManager->erase(dstBeing);
                dstBeing = 0;
            }

            if (!dstBeing)
            {
                // Being with id >= 110000000 and job 0 are better
                // known as ghosts, so don't create those.
                if (job == 0 && id >= 110000000)
                    break;

                if (actorSpriteManager->isBlocked(id) == true)
                    break;

                dstBeing = createBeing(id, job);

                if (!dstBeing)
                    break;

                if (job == 1022 && killStats)
                    killStats->jackoAlive(dstBeing->getId());
            }
            else
            {
                // undeleting marked for deletion being
                if (dstBeing->getType() == Being::NPC)
                {
                    actorSpriteManager->undelete(dstBeing);
                }
            }

            if (dstBeing->getType() == Being::PLAYER)
                dstBeing->setMoveTime();

            if (msg.getId() == SMSG_BEING_VISIBLE)
            {
                dstBeing->clearPath();
                dstBeing->setActionTime(tick_time);
                dstBeing->setAction(Being::STAND);
            }


            // Prevent division by 0 when calculating frame
            if (speed == 0)
                speed = 150;

            dstBeing->setWalkSpeed(Vector(speed, speed, 0));
            dstBeing->setSubtype(job);
            if (dstBeing->getType() == ActorSprite::MONSTER && player_node)
                player_node->checkNewName(dstBeing);

            hairStyle = msg.readInt16();
            weapon = msg.readInt16();
            headBottom = msg.readInt16();

            if (msg.getId() == SMSG_BEING_MOVE)
                msg.readInt32(); // server tick

            shield = msg.readInt16();
            headTop = msg.readInt16();
            headMid = msg.readInt16();
            hairColor = msg.readInt16();
            shoes = msg.readInt16();  // clothes color - "abused" as shoes

            if (dstBeing->getType() == ActorSprite::MONSTER)
            {
                hp = msg.readInt32();
                maxHP = msg.readInt32();
                if (hp && maxHP)
                {
                    oldHP = dstBeing->getHP();
                    if (!oldHP || oldHP > hp)
                        dstBeing->setHP(hp);
                    dstBeing->setMaxHP(maxHP);
                }
                gloves = 0;
                guild = 0;
            }
            else
            {
                gloves = msg.readInt16();  // head dir - "abused" as gloves
                guild = msg.readInt32();  // guild
                msg.readInt16();  // guild emblem
            }
//            logger->log("being guild: " + toString(guild));
/*
            if (guild == 0)
                dstBeing->clearGuilds();
            else
                dstBeing->setGuild(Guild::getGuild(static_cast<short>(guild)));
*/

            msg.readInt16();  // manner
            dstBeing->setStatusEffectBlock(32, msg.readInt16());  // opt3
            msg.readInt8();   // karma
            gender = msg.readInt8();

            if (dstBeing->getType() == ActorSprite::PLAYER)
            {
                dstBeing->setGender((gender == 0)
                                    ? GENDER_FEMALE : GENDER_MALE);
                // Set these after the gender, as the sprites may be gender-specific
                dstBeing->setSprite(SPRITE_HAIR, hairStyle * -1,
                                    ColorDB::get(hairColor));
                dstBeing->setSprite(SPRITE_BOTTOMCLOTHES, headBottom);
                dstBeing->setSprite(SPRITE_TOPCLOTHES, headMid);
                dstBeing->setSprite(SPRITE_HAT, headTop);
                dstBeing->setSprite(SPRITE_SHOE, shoes);
                dstBeing->setSprite(SPRITE_GLOVES, gloves);
                dstBeing->setSprite(SPRITE_WEAPON, weapon, "", true);
                if (!config.getBoolValue("hideShield"))
                    dstBeing->setSprite(SPRITE_SHIELD, shield);
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
                    default:
                        dstBeing->setGender(GENDER_UNSPECIFIED);
                        break;
                }
            }

            if (msg.getId() == SMSG_BEING_MOVE)
            {
                Uint16 srcX, srcY, dstX, dstY;
                msg.readCoordinatePair(srcX, srcY, dstX, dstY);
                dstBeing->setAction(Being::STAND);
                dstBeing->setTileCoords(srcX, srcY);
                dstBeing->setDestination(dstX, dstY);

//                if (player_node && player_node->getTarget() == dstBeing)
//                    player_node->targetMoved();
            }
            else
            {
                Uint8 dir;
                Uint16 x, y;
                msg.readCoordinates(x, y, dir);
                dstBeing->setTileCoords(x, y);

                if (job == 45 && socialWindow && outfitWindow)
                {
                    int num = socialWindow->getPortalIndex(x, y);
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

                dstBeing->setDirection(getDirection(dir));
            }

            msg.readInt8();   // unknown
            msg.readInt8();   // unknown
//            msg.readInt8();   // unknown / sit
            msg.readInt16();

            dstBeing->setStunMode(stunMode);
            dstBeing->setStatusEffectBlock(0, (statusEffects >> 16) & 0xffff);
            dstBeing->setStatusEffectBlock(16, statusEffects & 0xffff);
            break;

        case SMSG_BEING_MOVE2:
            /*
             * A simplified movement packet, used by the
             * later versions of eAthena for both mobs and
             * players
             */
            dstBeing = actorSpriteManager->findBeing(msg.readInt32());

            /*
             * This packet doesn't have enough info to actually
             * create a new being, so if the being isn't found,
             * we'll just pretend the packet didn't happen
             */

            if (!dstBeing)
                break;

            Uint16 srcX, srcY, dstX, dstY;
            msg.readCoordinatePair(srcX, srcY, dstX, dstY);
            msg.readInt32();  // Server tick

            dstBeing->setAction(Being::STAND);
            dstBeing->setTileCoords(srcX, srcY);
            dstBeing->setDestination(dstX, dstY);
            if (dstBeing->getType() == Being::PLAYER)
                dstBeing->setMoveTime();

//            if (player_node && player_node->getTarget() == dstBeing)
//            {
//                logger->log("SMSG_BEING_MOVE2");
//                player_node->targetMoved();
//            }

            break;

        case SMSG_BEING_SPAWN:
            // skipping this packet
            msg.readInt32();    // id
            msg.readInt16();    // speed
            msg.readInt16();  // opt1
            msg.readInt16();  // opt2
            msg.readInt16();  // option
            msg.readInt16();    // disguise
            break;

        case SMSG_BEING_REMOVE:
            // A being should be removed or has died

            id = msg.readInt32();
            dstBeing = actorSpriteManager->findBeing(id);
            if (!dstBeing)
                break;

            player_node->followMoveTo(dstBeing, player_node->getNextDestX(),
                                      player_node->getNextDestY());

            // If this is player's current target, clear it.
            if (dstBeing == player_node->getTarget())
                player_node->stopAttack();

            if (msg.readInt8() == 1)
            {
                dstBeing->setAction(Being::DEAD);
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
                actorSpriteManager->destroy(dstBeing);
            }
            break;

        case SMSG_BEING_RESURRECT:
            // A being changed mortality status

            id = msg.readInt32();
            dstBeing = actorSpriteManager->findBeing(id);
            if (!dstBeing)
                break;

            // If this is player's current target, clear it.
            if (dstBeing == player_node->getTarget())
                player_node->stopAttack();

            if (msg.readInt8() == 1)
                dstBeing->setAction(Being::STAND);

            break;

        case SMSG_SKILL_DAMAGE:
            msg.readInt16(); // Skill Id
            srcBeing = actorSpriteManager->findBeing(msg.readInt32());
            dstBeing = actorSpriteManager->findBeing(msg.readInt32());
            msg.readInt32(); // Server tick
            msg.readInt32(); // src speed
            msg.readInt32(); // dst speed
            param1 = msg.readInt32(); // Damage
            msg.readInt16(); // Skill level
            msg.readInt16(); // Div
            msg.readInt8(); // Skill hit/type (?)
            if (dstBeing)
            {
                // Perhaps a new skill attack type should be created and used?
                dstBeing->takeDamage(srcBeing, param1, Being::HIT);
            }
            if (srcBeing)
                srcBeing->handleAttack(dstBeing, param1, Being::HIT);
            break;

        case SMSG_BEING_ACTION:
            srcBeing = actorSpriteManager->findBeing(msg.readInt32());
            dstBeing = actorSpriteManager->findBeing(msg.readInt32());

            msg.readInt32();   // server tick
            msg.readInt32();   // src speed
            msg.readInt32();   // dst speed
            param1 = msg.readInt16();
            msg.readInt16();  // param 2
            type = msg.readInt8();
            msg.readInt16();  // param 3

            switch (type)
            {
                case Being::HIT: // Damage
                case Being::CRITICAL: // Critical Damage
                case Being::MULTI: // Critical Damage
                case Being::REFLECT: // Reflected Damage
                case Being::FLEE: // Lucky Dodge
                    if (dstBeing)
                    {
                        dstBeing->takeDamage(srcBeing, param1,
                            (Being::AttackType)type);
                    }
                    if (srcBeing)
                    {
                        srcBeing->handleAttack(dstBeing, param1,
                            (Being::AttackType)type);
                        if (srcBeing->getType() == Being::PLAYER)
                            srcBeing->setAttackTime();
                    }
                    break;

                case 0x02: // Sit
                    if (srcBeing)
                    {
                        srcBeing->setAction(Being::SIT);
                        if (srcBeing->getType() == Being::PLAYER)
                        {
                            srcBeing->setMoveTime();
                            if (player_node)
                            {
                                player_node->imitateAction(
                                    srcBeing, Being::SIT);
                            }
                        }
                    }
                    break;

                case 0x03: // Stand up
                    if (srcBeing)
                    {
                        srcBeing->setAction(Being::STAND);
                        if (srcBeing->getType() == Being::PLAYER)
                        {
                            srcBeing->setMoveTime();
                            if (player_node)
                            {
                                player_node->imitateAction(
                                    srcBeing, Being::STAND);
                            }
                        }
                    }
                    break;
                default:
                    break;
/*
                    logger->log("QQQ1 SMSG_BEING_ACTION:");
                    if (srcBeing)
                        logger->log("srcBeing:" + toString(srcBeing->getId()));
                    if (dstBeing)
                        logger->log("dstBeing:" + toString(dstBeing->getId()));
                    logger->log("type: " + toString(type));
*/
            }
            break;

        case SMSG_BEING_SELFEFFECT:
        {
            if (!effectManager)
                return;

            id = (Uint32)msg.readInt32();
            Being* being = actorSpriteManager->findBeing(id);
            if (!being)
                break;

            int effectType = msg.readInt32();

            effectManager->trigger(effectType, being);

            if (being && effectType == 3
                && being->getType() == Being::PLAYER
                && socialWindow)
            {   //reset received damage
                socialWindow->resetDamage(being->getName());
            }

            break;
        }

        case SMSG_BEING_EMOTION:
            if (!player_node)
                break;

            if (!(dstBeing = actorSpriteManager->findBeing(msg.readInt32())))
                break;

            if (player_relations.hasPermission(dstBeing,
                PlayerRelation::EMOTE))
            {
                unsigned char emote = msg.readInt8();
                if (emote)
                {
                    dstBeing->setEmote(emote, EMOTION_TIME);
                    player_node->imitateEmote(dstBeing, emote);
                }
            }
            if (dstBeing->getType() == Being::PLAYER)
                dstBeing->setOtherTime();

            break;

        case SMSG_BEING_CHANGE_LOOKS:
        case SMSG_BEING_CHANGE_LOOKS2:
        {
            /*
             * SMSG_BEING_CHANGE_LOOKS (0x00c3) and
             * SMSG_BEING_CHANGE_LOOKS2 (0x01d7) do basically the same
             * thing.  The difference is that ...LOOKS carries a single
             * 8 bit value, where ...LOOKS2 carries two 16 bit values.
             *
             * If type = 2, then the first 16 bit value is the weapon ID,
             * and the second 16 bit value is the shield ID.  If no
             * shield is equipped, or type is not 2, then the second
             * 16 bit value will be 0.
             */

            if (!(dstBeing = actorSpriteManager->findBeing(msg.readInt32())))
                break;

            int type = msg.readInt8();
            int id = 0;
            int id2 = 0;

            if (msg.getId() == SMSG_BEING_CHANGE_LOOKS)
            {
                id = msg.readInt8();
            }
            else
            {        // SMSG_BEING_CHANGE_LOOKS2
                id = msg.readInt16();
                id2 = msg.readInt16();
            }

            if (dstBeing->getType() == Being::PLAYER)
                dstBeing->setOtherTime();

            if (!player_node)
                break;

            switch (type)
            {
                case 0:     // change race
                    dstBeing->setSubtype(id);
                    break;
                case 1:     // eAthena LOOK_HAIR
                    dstBeing->setSpriteID(SPRITE_HAIR, id *-1);
                    break;
                case 2:     // Weapon ID in id, Shield ID in id2
                    dstBeing->setSprite(SPRITE_WEAPON, id, "", true);
                    if (!config.getBoolValue("hideShield"))
                        dstBeing->setSprite(SPRITE_SHIELD, id2);
                    player_node->imitateOutfit(dstBeing, SPRITE_SHIELD);
                    break;
                case 3:     // Change lower headgear for eAthena, pants for us
                    dstBeing->setSprite(SPRITE_BOTTOMCLOTHES, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_BOTTOMCLOTHES);
                    break;
                case 4:     // Change upper headgear for eAthena, hat for us
                    dstBeing->setSprite(SPRITE_HAT, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_HAT);
                    break;
                case 5:     // Change middle headgear for eathena, armor for us
                    dstBeing->setSprite(SPRITE_TOPCLOTHES, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_TOPCLOTHES);
                    break;
                case 6:     // eAthena LOOK_HAIR_COLOR
                    dstBeing->setSpriteColor(SPRITE_HAIR, ColorDB::get(id));
                    break;
                case 8:     // eAthena LOOK_SHIELD
                    if (!config.getBoolValue("hideShield"))
                        dstBeing->setSprite(SPRITE_SHIELD, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_SHIELD);
                    break;
                case 9:     // eAthena LOOK_SHOES
                    dstBeing->setSprite(SPRITE_SHOE, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_SHOE);
                    break;
                case 10:   // LOOK_GLOVES
                    dstBeing->setSprite(SPRITE_GLOVES, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_GLOVES);
                    break;
                case 11:  // LOOK_CAPE
                    dstBeing->setSprite(SPRITE_CAPE, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_CAPE);
                    break;
                case 12:
                    dstBeing->setSprite(SPRITE_MISC1, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_MISC1);
                    break;
                case 13:
                    dstBeing->setSprite(SPRITE_MISC2, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_MISC2);
                    break;
                case 14:
                    dstBeing->setSprite(SPRITE_EVOL1, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_EVOL1);
                    break;
                case 15:
                    dstBeing->setSprite(SPRITE_EVOL2, id);
                    player_node->imitateOutfit(dstBeing, SPRITE_EVOL2);
                    break;
                default:
                    logger->log("QQQ3 CHANGE_LOOKS: unsupported type: "
                            "%d, id: %d", type, id);
                    if (dstBeing)
                    {
                        logger->log("ID: " + toString(dstBeing->getId()));
                        logger->log("name: " + toString(dstBeing->getName()));
                    }
                    break;
            }
        }
            break;

        case SMSG_BEING_NAME_RESPONSE:
            {
                int beingId = msg.readInt32();
                if ((dstBeing = actorSpriteManager->findBeing(beingId)))
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
                            Party *party = player_node->getParty();
                            if (party && party->isMember(dstBeing->getId()))
                            {
                                PartyMember *member = party->getMember(
                                    dstBeing->getId());

                                if (member)
                                    member->setName(dstBeing->getName());
                            }
                            player_node->checkNewName(dstBeing);
                        }
                    }
                }
            }
            break;
        case SMSG_BEING_NAME_RESPONSE2:
            {
                int len = msg.readInt16();
                int beingId = msg.readInt32();
                std::string str = msg.readString(len - 8);
                if ((dstBeing = actorSpriteManager->findBeing(beingId)))
                {
                    if (beingId == player_node->getId())
                    {
                        player_node->pingResponse();
                    }
                    else
                    {
                        dstBeing->setName(str);
                        dstBeing->updateGuild();
                        dstBeing->addToCache();

                        if (dstBeing->getType() == Being::PLAYER)
                            dstBeing->updateColors();

                        if (player_node)
                        {
                            Party *party = player_node->getParty();
                            if (party && party->isMember(dstBeing->getId()))
                            {
                                PartyMember *member = party->getMember(
                                    dstBeing->getId());

                                if (member)
                                    member->setName(dstBeing->getName());
                            }
                            player_node->checkNewName(dstBeing);
                        }
                    }
                }
            }
            break;
        case SMSG_BEING_IP_RESPONSE:
            {
                if ((dstBeing = actorSpriteManager->findBeing(
                    msg.readInt32())))
                {
                    dstBeing->setIp(ipToString(msg.readInt32()));
                }
            }
            break;
        case SMSG_SOLVE_CHAR_NAME:
            {
                logger->log1("SMSG_SOLVE_CHAR_NAME");
                logger->log(toString(msg.readInt32()));
                logger->log(msg.readString(24));
            }
            break;
        case SMSG_PLAYER_GUILD_PARTY_INFO:
            if ((dstBeing = actorSpriteManager->findBeing(msg.readInt32())))
            {
                dstBeing->setPartyName(msg.readString(24));
                dstBeing->setGuildName(msg.readString(24));
                dstBeing->setGuildPos(msg.readString(24));
                dstBeing->addToCache();
                msg.readString(24); // Discard this
            }
            break;
        case SMSG_BEING_CHANGE_DIRECTION:
        {
            if (!(dstBeing = actorSpriteManager->findBeing(msg.readInt32())))
                break;

            msg.readInt16(); // unused

            unsigned char dir = getDirection(msg.readInt8());
            dstBeing->setDirection(dir);
            if (player_node)
                player_node->imitateDirection(dstBeing, dir);
            break;
        }
        case SMSG_PLAYER_UPDATE_1:
        case SMSG_PLAYER_UPDATE_2:
        case SMSG_PLAYER_MOVE:
            if (!actorSpriteManager || !player_node)
                break;

            // An update about a player, potentially including movement.
            id = msg.readInt32();
            speed = msg.readInt16();
            stunMode = msg.readInt16(); // opt1; Aethyra use this as cape
            statusEffects = msg.readInt16(); // opt2; Aethyra use this as misc1
            statusEffects |= ((Uint32) msg.readInt16())
                << 16; // status.options; Aethyra uses this as misc2
            job = msg.readInt16();

            dstBeing = actorSpriteManager->findBeing(id);

            if (!dstBeing)
            {
                if (actorSpriteManager->isBlocked(id) == true)
                    break;

                dstBeing = createBeing(id, job);

                if (!dstBeing)
                    break;
            }

            if (Party *party = player_node->getParty())
            {
                if (party->isMember(id))
                    dstBeing->setParty(party);
            }

            dstBeing->setWalkSpeed(Vector(speed, speed, 0));
            dstBeing->setSubtype(job);
            hairStyle = msg.readInt16();
            weapon = msg.readInt16();
            shield = msg.readInt16();
            headBottom = msg.readInt16();

            if (msg.getId() == SMSG_PLAYER_MOVE)
                msg.readInt32(); // server tick

            headTop = msg.readInt16();
            headMid = msg.readInt16();
            hairColor = msg.readInt16();
            shoes = msg.readInt16();
            gloves = msg.readInt16();       //sd->head_dir
            guild = msg.readInt32();  // guild

            if (guild == 0)
                dstBeing->clearGuilds();
            else
                dstBeing->setGuild(Guild::getGuild(static_cast<short>(guild)));

            msg.readInt16();  // emblem
            msg.readInt16();  // manner
            dstBeing->setStatusEffectBlock(32, msg.readInt16());  // opt3
            msg.readInt8();   // karma
            dstBeing->setGender((msg.readInt8() == 0)
                              ? GENDER_FEMALE : GENDER_MALE);

            // Set these after the gender, as the sprites may be gender-specific
            dstBeing->setSprite(SPRITE_WEAPON, weapon, "", true);
            if (!config.getBoolValue("hideShield"))
                dstBeing->setSprite(SPRITE_SHIELD, shield);
            //dstBeing->setSprite(SPRITE_SHOE, shoes);
            dstBeing->setSprite(SPRITE_BOTTOMCLOTHES, headBottom);
            dstBeing->setSprite(SPRITE_TOPCLOTHES, headMid);
            dstBeing->setSprite(SPRITE_HAT, headTop);
            //dstBeing->setSprite(SPRITE_GLOVES, gloves);
            //dstBeing->setSprite(SPRITE_CAPE, cape);
            //dstBeing->setSprite(SPRITE_MISC1, misc1);
            //dstBeing->setSprite(SPRITE_MISC2, misc2);
            dstBeing->setSprite(SPRITE_HAIR, hairStyle * -1,
                                ColorDB::get(hairColor));

            player_node->imitateOutfit(dstBeing);

            if (msg.getId() == SMSG_PLAYER_MOVE)
            {
                Uint16 srcX, srcY, dstX, dstY;
                msg.readCoordinatePair(srcX, srcY, dstX, dstY);

                player_node->followMoveTo(dstBeing, srcX, srcY, dstX, dstY);

                dstBeing->setTileCoords(srcX, srcY);
                dstBeing->setDestination(dstX, dstY);

                if (player_node->getCurrentAction() != Being::STAND)
                    player_node->imitateAction(dstBeing, Being::STAND);
                if (player_node->getDirection() != dstBeing->getDirection())
                {
                    player_node->imitateDirection(dstBeing,
                        dstBeing->getDirection());
                }
            }
            else
            {
                Uint8 dir;
                Uint16 x, y;
                msg.readCoordinates(x, y, dir);
                dstBeing->setTileCoords(x, y);
                dir = getDirection(dir);
                dstBeing->setDirection(dir);

                player_node->imitateDirection(dstBeing, dir);
            }

            gmstatus = msg.readInt16();

            if (gmstatus & 0x80)
                dstBeing->setGM(true);

            if (msg.getId() == SMSG_PLAYER_UPDATE_1)
            {
                int type = msg.readInt8();
                switch (type)
                {
                    case 0:
                        dstBeing->setAction(Being::STAND);
                        player_node->imitateAction(dstBeing, Being::STAND);
                        break;

                    case 1:
                        dstBeing->setAction(Being::DEAD);
                        break;

                    case 2:
                        dstBeing->setAction(Being::SIT);
                        player_node->imitateAction(dstBeing, Being::SIT);
                        break;

                    default:
                        //need set stay state?
                        logger->log("QQQ2 SMSG_PLAYER_UPDATE_1:"
                            + toString(id) + " " + toString(type));
                        if (dstBeing)
                        {
                            logger->log("dstBeing id:"
                                + toString(dstBeing->getId()));
                            logger->log("dstBeing name:"
                                + dstBeing->getName());
                        }
                        break;

                }
            }
            else if (msg.getId() == SMSG_PLAYER_MOVE)
            {
                msg.readInt8(); // unknown
            }

            level = msg.readInt8();   // Lv
            if (level)
                dstBeing->setLevel(level);

            msg.readInt8();   // unknown

            dstBeing->setActionTime(tick_time);
            dstBeing->reset();

            dstBeing->setStunMode(stunMode);
            dstBeing->setStatusEffectBlock(0, (statusEffects >> 16) & 0xffff);
            dstBeing->setStatusEffectBlock(16, statusEffects & 0xffff);

            if (msg.getId() == SMSG_PLAYER_MOVE
                && dstBeing->getType() == Being::PLAYER)
            {
                dstBeing->setMoveTime();
            }

            break;
        case SMSG_PLAYER_STOP:
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

            id = msg.readInt32();

            if (mSync || id != player_node->getId())
            {
                dstBeing = actorSpriteManager->findBeing(id);
                if (dstBeing)
                {
                    Uint16 x, y;
                    x = msg.readInt16();
                    y = msg.readInt16();
                    dstBeing->setTileCoords(x, y);
                    if (dstBeing->getCurrentAction() == Being::MOVE)
                        dstBeing->setAction(Being::STAND);
                }
            }
            break;

        case SMSG_PLAYER_MOVE_TO_ATTACK:
            /*
             * This is an *advisory* message, telling the client that
             * it needs to move the character before attacking
             * a target (out of range, obstruction in line of fire).
             * We can safely ignore this...
             */
            if (player_node)
                player_node->fixAttackTarget();
            break;

        case SMSG_PLAYER_STATUS_CHANGE:
            // Change in players' flags

            id = msg.readInt32();
            dstBeing = actorSpriteManager->findBeing(id);
            if (!dstBeing)
                break;

            stunMode = msg.readInt16();
            statusEffects = msg.readInt16();
            statusEffects |= ((Uint32) msg.readInt16()) << 16;
            msg.readInt8(); // Unused?

            dstBeing->setStunMode(stunMode);
            dstBeing->setStatusEffectBlock(0, (statusEffects >> 16) & 0xffff);
            dstBeing->setStatusEffectBlock(16, statusEffects & 0xffff);
            break;

        case SMSG_BEING_STATUS_CHANGE:
            // Status change
            status = msg.readInt16();
            id = msg.readInt32();
            flag = msg.readInt8(); // 0: stop, 1: start

            dstBeing = actorSpriteManager->findBeing(id);
            if (dstBeing)
                dstBeing->setStatusEffect(status, flag);
            break;

        case SMSG_SKILL_CASTING:
            msg.readInt32();    // src id
            msg.readInt32();    // dst id
            msg.readInt16();    // dst x
            msg.readInt16();    // dst y
            msg.readInt16();    // skill num
            msg.readInt32();    // skill get pl
            msg.readInt32();    // cast time
            break;

        case SMSG_SKILL_CAST_CANCEL:
            msg.readInt32();    // id
            break;

        case SMSG_SKILL_NO_DAMAGE:
            msg.readInt16();    // skill id
            msg.readInt16();    // heal
            msg.readInt32();    // dst id
            msg.readInt32();    // src id
            msg.readInt8();     // fail
            break;

        case SMSG_PVP_MAP_MODE:
        {
            Game *game = Game::instance();
            if (!game)
                break;

            Map *map = game->getCurrentMap();
            if (map)
                map->setPvpMode(msg.readInt16());
            break;
        }

        case SMSG_PVP_SET:
        {
            int id = msg.readInt32();    // id
            int rank = msg.readInt32();  // rank
            msg.readInt32();             // num
            dstBeing = actorSpriteManager->findBeing(id);
            if (dstBeing)
                dstBeing->setPvpRank(rank);
            break;
        }

        default:
            break;
    }
}

Uint8 BeingHandler::getDirection(Uint8 dir)
{
    if (dir == 0)
        dir = 8;
    return dir;
}

void BeingHandler::undress(Being *being)
{
    being->setSprite(SPRITE_BOTTOMCLOTHES, 0);
    being->setSprite(SPRITE_TOPCLOTHES, 0);
    being->setSprite(SPRITE_HAT, 0);
    being->setSprite(SPRITE_SHOE, 0);
    being->setSprite(SPRITE_GLOVES, 0);
//    being->setSprite(SPRITE_WEAPON, 0, "", true);
}

} // namespace TmwAthena
