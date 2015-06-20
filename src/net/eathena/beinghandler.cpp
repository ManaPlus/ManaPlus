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

#include "net/eathena/beinghandler.h"

#include "actormanager.h"
#include "effectmanager.h"
#include "game.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"
#include "being/mercenaryinfo.h"
#include "being/playerinfo.h"

#include "particle/particle.h"

#include "input/keyboardconfig.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/outfitwindow.h"

#include "net/serverfeatures.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/maptypeproperty2.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"
#include "net/eathena/sprite.h"

#include "resources/iteminfo.h"
#include "resources/notifytypes.h"

#include "resources/db/itemdb.h"

#include "resources/map/map.h"

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
        SMSG_BEING_REMOVE_SKILL,
        SMSG_SKILL_DAMAGE,
        SMSG_BEING_ACTION,
        SMSG_BEING_ACTION2,
        SMSG_BEING_SELFEFFECT,
        SMSG_BEING_SPECIAL_EFFECT,
        SMSG_BEING_SPECIAL_EFFECT_NUM,
        SMSG_BEING_SOUND_EFFECT,
        SMSG_BEING_EMOTION,
        SMSG_BEING_CHANGE_LOOKS2,
        SMSG_BEING_NAME_RESPONSE,
        SMSG_BEING_NAME_RESPONSE2,
        SMSG_PLAYER_GUILD_PARTY_INFO,
        SMSG_BEING_CHANGE_DIRECTION,
        SMSG_PLAYER_STOP,
        SMSG_PLAYER_MOVE_TO_ATTACK,
        SMSG_PLAYER_STATUS_CHANGE,
        SMSG_PLAYER_STATUS_CHANGE2,
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
        SMSG_PVP_MAP_MODE,
        SMSG_PVP_SET,
        SMSG_MAP_TYPE_PROPERTY2,
        SMSG_MAP_TYPE,
        SMSG_MONSTER_HP,
        SMSG_PLAYER_HP,
        SMSG_SKILL_AUTO_CAST,
        SMSG_RANKS_LIST,
        SMSG_BEING_FAKE_NAME,
        SMSG_BEING_STAT_UPDATE_1,
        SMSG_MOB_INFO,
        SMSG_BEING_MOVE3,
        SMSG_BEING_ATTRS,
        SMSG_MONSTER_INFO,
        SMSG_CLASS_CHANGE,
        SMSG_SPIRIT_BALLS,
        SMSG_SPIRIT_BALL_SINGLE,
        SMSG_BLADE_STOP,
        SMSG_COMBO_DELAY,
        SMSG_WEDDING_EFFECT,
        SMSG_BEING_SLIDE,
        SMSG_STARS_KILL,
        SMSG_BLACKSMITH_RANKS_LIST,
        SMSG_ALCHEMIST_RANKS_LIST,
        SMSG_TAEKWON_RANKS_LIST,
        SMSG_PK_RANKS_LIST,
        SMSG_GLADIATOR_FEEL_REQUEST,
        SMSG_BOSS_MAP_INFO,
        SMSG_BEING_FONT,
        SMSG_BEING_MILLENIUM_SHIELD,
        SMSG_BEING_CHARM,
        SMSG_BEING_VIEW_EQUIPMENT,
        0
    };
    handledMessages = _messages;
    beingHandler = this;
}

void BeingHandler::requestNameById(const BeingId id) const
{
    createOutPacket(CMSG_NAME_REQUEST);
    outMsg.writeBeingId(id, "being id");
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

        case SMSG_BEING_REMOVE_SKILL:
            processBeingRemoveSkil(msg);
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

        case SMSG_BEING_SPECIAL_EFFECT_NUM:
            processBeingSpecialEffectNum(msg);
            break;

        case SMSG_BEING_SOUND_EFFECT:
            processBeingSoundEffect(msg);
            break;

        case SMSG_BEING_EMOTION:
            processBeingEmotion(msg);
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

        case SMSG_SOLVE_CHAR_NAME:
            break;

        case SMSG_PLAYER_GUILD_PARTY_INFO:
            processPlayerGuilPartyInfo(msg);
            break;

        case SMSG_BEING_CHANGE_DIRECTION:
            processBeingChangeDirection(msg);
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

        case SMSG_PLAYER_STATUS_CHANGE2:
            processPlaterStatusChange2(msg);
            break;

        case SMSG_PLAYER_STATUS_CHANGE_NO_TICK:
            processPlaterStatusChangeNoTick(msg);
            break;

        case SMSG_BEING_STATUS_CHANGE:
            processBeingStatusChange(msg);
            break;

        case SMSG_BEING_STATUS_CHANGE2:
            processBeingStatusChange2(msg);
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

        case SMSG_MAP_TYPE:
            processMapType(msg);
            break;

        case SMSG_MONSTER_HP:
        case SMSG_PLAYER_HP:
            processMonsterHp(msg);
            break;

        case SMSG_RANKS_LIST:
            processRanksList(msg);
            break;

        case SMSG_BEING_FAKE_NAME:
            processBeingFakeName(msg);
            break;

        case SMSG_BEING_STAT_UPDATE_1:
            processBeingStatUpdate1(msg);
            break;

        case SMSG_MOB_INFO:
            processMobInfo(msg);
            break;

        case SMSG_BEING_MOVE3:
            processBeingMove3(msg);
            break;

        case SMSG_BEING_ATTRS:
            processBeingAttrs(msg);
            break;

        case SMSG_MONSTER_INFO:
            processMonsterInfo(msg);
            break;

        case SMSG_CLASS_CHANGE:
            processClassChange(msg);
            break;

        case SMSG_SPIRIT_BALLS:
            processSpiritBalls(msg);
            break;

        case SMSG_SPIRIT_BALL_SINGLE:
            processSpiritBallSingle(msg);
            break;

        case SMSG_BLADE_STOP:
            processBladeStop(msg);
            break;

        case SMSG_COMBO_DELAY:
            processComboDelay(msg);
            break;

        case SMSG_WEDDING_EFFECT:
            processWddingEffect(msg);
            break;

        case SMSG_BEING_SLIDE:
            processBeingSlide(msg);
            break;

        case SMSG_STARS_KILL:
            processStarsKill(msg);
            break;

        case SMSG_BLACKSMITH_RANKS_LIST:
            processBlacksmithRanksList(msg);
            break;

        case SMSG_ALCHEMIST_RANKS_LIST:
            processAlchemistRanksList(msg);
            break;

        case SMSG_TAEKWON_RANKS_LIST:
            processTaekwonRanksList(msg);
            break;

        case SMSG_PK_RANKS_LIST:
            processPkRanksList(msg);
            break;

        case SMSG_GLADIATOR_FEEL_REQUEST:
            processGladiatorFeelRequest(msg);
            break;

        case SMSG_BOSS_MAP_INFO:
            processBossMapInfo(msg);
            break;

        case SMSG_BEING_FONT:
            processBeingFont(msg);
            break;

        case SMSG_BEING_MILLENIUM_SHIELD:
            processBeingMilleniumShield(msg);
            break;

        case SMSG_BEING_CHARM:
            processBeingCharm(msg);
            break;

        case SMSG_BEING_VIEW_EQUIPMENT:
            processBeingViewEquipment(msg);
            break;

        default:
            break;
    }
}

Being *BeingHandler::createBeing2(Net::MessageIn &msg,
                                  const BeingId id,
                                  const int16_t job,
                                  const BeingType::BeingType beingType)
{
    if (!actorManager)
        return nullptr;

    ActorTypeT type = ActorType::Unknown;
    switch (beingType)
    {
        case BeingType::PC:
            type = ActorType::Player;
            break;
        case BeingType::NPC:
        case BeingType::NPC_EVENT:
            type = ActorType::Npc;
            break;
        case BeingType::MONSTER:
            type = ActorType::Monster;
            break;
        case BeingType::MERSOL:
            type = ActorType::Mercenary;
            break;
        case BeingType::PET:
            type = ActorType::Pet;
            break;
        case BeingType::HOMUN:
            type = ActorType::Homunculus;
            break;
        case BeingType::ITEM:
        case BeingType::SKILL:
        case BeingType::ELEMENTAL:
            logger->log("not supported object type: %d, job: %d",
                static_cast<int>(beingType), static_cast<int>(job));
            break;
        case BeingType::CHAT:
        default:
            UNIMPLIMENTEDPACKET;
            type = ActorType::Monster;
            logger->log("not supported object type: %d, job: %d",
                static_cast<int>(beingType), static_cast<int>(job));
            break;
    }
    if (job == 45 && beingType == BeingType::NPC_EVENT)
        type = ActorType::Portal;

    Being *const being = actorManager->createBeing(
        id, type, fromInt(job, BeingTypeId));
    if (beingType == BeingType::MERSOL)
    {
        MercenaryInfo *const info = PlayerInfo::getMercenary();
        if (info && info->id == id)
            PlayerInfo::setMercenaryBeing(being);
    }
    else if (beingType == BeingType::PET)
    {
        if (PlayerInfo::getPetBeingId() == id)
            PlayerInfo::setPetBeing(being);
    }
    return being;
}

void BeingHandler::undress(Being *const being) const
{
    being->setSprite(SPRITE_WEAPON, 0);
    being->setSprite(SPRITE_HEAD_BOTTOM, 0);
    being->setSprite(SPRITE_CLOTHES_COLOR, 0);
    being->setSprite(SPRITE_HAIR, 0);
    being->setSprite(SPRITE_SHOES, 0);
//    being->setSprite(SPRITE_BODY, 0, "", true);
}

void BeingHandler::requestRanks(const RankT rank) const
{
    createOutPacket(CMSG_REQUEST_RANKS);
    outMsg.writeInt16(static_cast<int16_t>(rank), "type");
}

void BeingHandler::processBeingChangeLook2(Net::MessageIn &msg)
{
    if (!actorManager)
        return;

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));
    const uint8_t type = msg.readUInt8("type");

    const int id = msg.readInt16("id1");
    unsigned int id2 = msg.readInt16("id2");
    if (type != 2)
        id2 = 1;

    if (!localPlayer || !dstBeing)
        return;

    processBeingChangeLookContinue(msg, dstBeing, type, id, id2);
}

void BeingHandler::processBeingChangeLookContinue(Net::MessageIn &msg,
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
        case 0:  // change race
            dstBeing->setSubtype(fromInt(id, BeingTypeId),
                dstBeing->getLook());
            break;
        case 1:  // eAthena LOOK_HAIR
            dstBeing->setHairColor(id);
            dstBeing->setSpriteID(SPRITE_HAIR_COLOR, id * -1);
            break;
        case 2:  // LOOK_WEAPON Weapon ID in id, Shield ID in id2
            dstBeing->setSprite(SPRITE_BODY, id, "", 1, true);
            dstBeing->setSprite(SPRITE_FLOOR, id2);
            localPlayer->imitateOutfit(dstBeing, SPRITE_FLOOR);
            break;
        case 3:  // LOOK_HEAD_BOTTOM
            dstBeing->setSprite(SPRITE_WEAPON, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_WEAPON);
            break;
        case 4:  // LOOK_HEAD_TOP Change upper headgear for eAthena, hat for us
            dstBeing->setSprite(SPRITE_CLOTHES_COLOR, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_CLOTHES_COLOR);
            break;
        case 5:  // LOOK_HEAD_MID Change middle headgear for eathena,
                 // armor for us
            dstBeing->setSprite(SPRITE_HEAD_BOTTOM, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_HEAD_BOTTOM);
            break;
        case 6:  // eAthena LOOK_HAIR_COLOR
            dstBeing->setHairColor(id);
            dstBeing->setSpriteColor(SPRITE_HAIR_COLOR,
                ItemDB::get(dstBeing->getSpriteID(
                SPRITE_HAIR_COLOR)).getDyeColorsString(id));
            break;
        case 7:  // Clothes color. Now used as look
            dstBeing->setLook(static_cast<uint8_t>(id));
            break;
        case 8:  // eAthena LOOK_SHIELD
            dstBeing->setSprite(SPRITE_FLOOR, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_FLOOR);
            break;
        case 9:  // eAthena LOOK_SHOES
            dstBeing->setSprite(SPRITE_HAIR, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_HAIR);
            break;
        case 10:  // LOOK_GLOVES
            dstBeing->setSprite(SPRITE_SHOES, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHOES);
            break;
        case 11:  // LOOK_FLOOR
            dstBeing->setSprite(SPRITE_SHIELD, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_SHIELD);
            break;
        case 12:  // LOOK_ROBE
            dstBeing->setSprite(SPRITE_HEAD_TOP, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_HEAD_TOP);
            break;
        case 13:  // COSTUME_HEAD_TOP
            dstBeing->setSprite(SPRITE_HEAD_MID, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_HEAD_MID);
            break;
        case 14:  // COSTUME_HEAD_MID
            dstBeing->setSprite(SPRITE_ROBE, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_ROBE);
            break;
        case 15:  // COSTUME_HEAD_LOW
            dstBeing->setSprite(SPRITE_EVOL2, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_EVOL2);
            break;
        case 16:  // COSTUME_GARMENT
            dstBeing->setSprite(SPRITE_EVOL3, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_EVOL3);
            break;
        case 17:  // ARMOR
            dstBeing->setSprite(SPRITE_EVOL4, id, color,
                static_cast<unsigned char>(id2));
            localPlayer->imitateOutfit(dstBeing, SPRITE_EVOL4);
            break;
        default:
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void BeingHandler::processBeingVisible(Net::MessageIn &msg)
{
    if (!actorManager)
        return;

    msg.readInt16("len");
    const BeingType::BeingType type = static_cast<BeingType::BeingType>(
        msg.readUInt8("object type"));

    // Information about a being in range
    const BeingId id = msg.readBeingId("being id");
    BeingId spawnId;
    if (id == mSpawnId)
        spawnId = mSpawnId;
    else
        spawnId = BeingId_zero;
    mSpawnId = BeingId_zero;

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
        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing2(msg, id, job, type);

        if (!dstBeing)
            return;
    }
    else
    {
        // undeleting marked for deletion being
        if (dstBeing->getType() == ActorType::Npc)
            actorManager->undelete(dstBeing);
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

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    dstBeing->setSubtype(fromInt(job, BeingTypeId), 0);
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
    dstBeing->setManner(msg.readInt16("manner"));
    dstBeing->setStatusEffectBlock(32, static_cast<uint16_t>(
        msg.readInt32("opt3")));
    dstBeing->setKarma(msg.readUInt8("karma"));
    uint8_t gender = static_cast<uint8_t>(msg.readUInt8("gender") & 3);

    if (dstBeing->getType() == ActorType::Player)
    {
        dstBeing->setGender(Being::intToGender(gender));
        dstBeing->setHairColor(hairColor);
        // Set these after the gender, as the sprites may be gender-specific
        setSprite(dstBeing, SPRITE_HAIR_COLOR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        setSprite(dstBeing, SPRITE_WEAPON, headBottom);
        setSprite(dstBeing, SPRITE_HEAD_BOTTOM, headMid);
        setSprite(dstBeing, SPRITE_CLOTHES_COLOR, headTop);
        setSprite(dstBeing, SPRITE_HAIR, shoes);
        setSprite(dstBeing, SPRITE_SHOES, gloves);
        setSprite(dstBeing, SPRITE_BODY, weapon, "", 1, true);
//        setSprite(dstBeing, SPRITE_FLOOR, shield);
    }
    else if (dstBeing->getType() == ActorType::Npc
             && serverFeatures->haveNpcGender())
    {
        dstBeing->setGender(Being::intToGender(gender));
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
    const BeingType::BeingType type = static_cast<BeingType::BeingType>(
        msg.readUInt8("object type"));

    // Information about a being in range
    const BeingId id = msg.readBeingId("being id");
    BeingId spawnId;
    if (id == mSpawnId)
        spawnId = mSpawnId;
    else
        spawnId = BeingId_zero;
    mSpawnId = BeingId_zero;
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
        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing2(msg, id, job, type);

        if (!dstBeing)
            return;
    }
    else
    {
        // undeleting marked for deletion being
        if (dstBeing->getType() == ActorType::Npc)
            actorManager->undelete(dstBeing);
    }

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();

    if (spawnId != BeingId_zero)
        dstBeing->setAction(BeingAction::SPAWN, 0);

    // Prevent division by 0 when calculating frame
    if (speed == 0)
        speed = 150;

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    dstBeing->setSubtype(fromInt(job, BeingTypeId), 0);
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
    dstBeing->setManner(msg.readInt16("manner"));
    dstBeing->setStatusEffectBlock(32, static_cast<uint16_t>(
        msg.readInt32("opt3")));
    dstBeing->setKarma(msg.readUInt8("karma"));
    uint8_t gender = static_cast<uint8_t>(msg.readUInt8("gender") & 3);

    if (dstBeing->getType() == ActorType::Player)
    {
        dstBeing->setGender(Being::intToGender(gender));
        dstBeing->setHairColor(hairColor);
        // Set these after the gender, as the sprites may be gender-specific
        setSprite(dstBeing, SPRITE_HAIR_COLOR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        if (!serverFeatures->haveMove3())
        {
            setSprite(dstBeing, SPRITE_WEAPON, headBottom);
            setSprite(dstBeing, SPRITE_HEAD_BOTTOM, headMid);
            setSprite(dstBeing, SPRITE_CLOTHES_COLOR, headTop);
            setSprite(dstBeing, SPRITE_HAIR, shoes);
            setSprite(dstBeing, SPRITE_SHOES, gloves);
            setSprite(dstBeing, SPRITE_BODY, weapon, "", 1, true);
        }
//        setSprite(dstBeing, SPRITE_FLOOR, shield);
    }
    else if (dstBeing->getType() == ActorType::Npc
             && serverFeatures->haveNpcGender())
    {
        dstBeing->setGender(Being::intToGender(gender));
    }

    uint16_t srcX, srcY, dstX, dstY;
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    msg.readUInt8("(sx<<4) | (sy&0x0f)");
    msg.readInt8("xs");
    msg.readInt8("ys");
    dstBeing->setAction(BeingAction::STAND, 0);
    dstBeing->setTileCoords(srcX, srcY);
    if (localPlayer)
        localPlayer->followMoveTo(dstBeing, srcX, srcY, dstX, dstY);
    if (!serverFeatures->haveMove3())
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
    const BeingType::BeingType type = static_cast<BeingType::BeingType>(
        msg.readUInt8("object type"));

    // Information about a being in range
    const BeingId id = msg.readBeingId("being id");
    mSpawnId = id;
    const BeingId spawnId = id;
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
        if (actorManager->isBlocked(id) == true)
            return;

        dstBeing = createBeing2(msg, id, job, type);

        if (!dstBeing)
            return;
    }
    else
    {
        // undeleting marked for deletion being
        if (dstBeing->getType() == ActorType::Npc)
            actorManager->undelete(dstBeing);
    }

    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();

    if (spawnId != BeingId_zero)
        dstBeing->setAction(BeingAction::SPAWN, 0);

    // Prevent division by 0 when calculating frame
    if (speed == 0)
        speed = 150;

    dstBeing->setWalkSpeed(Vector(speed, speed, 0));
    dstBeing->setSubtype(fromInt(job, BeingTypeId), 0);
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
    dstBeing->setManner(msg.readInt16("manner"));
    dstBeing->setStatusEffectBlock(32, static_cast<uint16_t>(
        msg.readInt32("opt3")));
    dstBeing->setKarma(msg.readUInt8("karma"));
    uint8_t gender = static_cast<uint8_t>(msg.readUInt8("gender") & 3);

    if (dstBeing->getType() == ActorType::Player)
    {
        dstBeing->setGender(Being::intToGender(gender));
        dstBeing->setHairColor(hairColor);
        // Set these after the gender, as the sprites may be gender-specific
        setSprite(dstBeing, SPRITE_HAIR_COLOR, hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(hairColor));
        setSprite(dstBeing, SPRITE_WEAPON, headBottom);
        setSprite(dstBeing, SPRITE_HEAD_BOTTOM, headMid);
        setSprite(dstBeing, SPRITE_CLOTHES_COLOR, headTop);
        setSprite(dstBeing, SPRITE_HAIR, shoes);
        setSprite(dstBeing, SPRITE_SHOES, gloves);
        setSprite(dstBeing, SPRITE_BODY, weapon, "", 1, true);
//        setSprite(dstBeing, SPRITE_FLOOR, shield);
    }
    else if (dstBeing->getType() == ActorType::Npc
             && serverFeatures->haveNpcGender())
    {
        dstBeing->setGender(Being::intToGender(gender));
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

void BeingHandler::processMapTypeProperty(Net::MessageIn &msg)
{
    const int16_t type = msg.readInt16("type");
    const int flags = msg.readInt32("flags");
    if (type == 0x28)
    {
        // +++ need get other flags from here
        MapTypeProperty2 props;
        props.data = static_cast<uint32_t>(flags);
        Game *const game = Game::instance();
        Map *const map = game->getCurrentMap();
        if (!map)
            return;
        map->setPvpMode(props.bits.party | (props.bits.guild * 2));
    }
}

void BeingHandler::processMapType(Net::MessageIn &msg)
{
    const int16_t type = msg.readInt16("type");
    if (type == 19)
        NotifyManager::notify(NotifyTypes::MAP_TYPE_BATTLEFIELD);
    else
        UNIMPLIMENTEDPACKET;
}

void BeingHandler::processSkillCasting(Net::MessageIn &msg)
{
    // +++ need use other parameters

    const BeingId srcId = msg.readBeingId("src id");
    const BeingId dstId = msg.readBeingId("dst id");
    const int dstX = msg.readInt16("dst x");
    const int dstY = msg.readInt16("dst y");
    const int skillId = msg.readInt16("skill id");
    msg.readInt32("property");  // can be used to trigger effect
    msg.readInt32("cast time");
    msg.readInt8("dispossable");

    if (!effectManager)
        return;

    if (srcId == BeingId_zero)
    {
        UNIMPLIMENTEDPACKET;
        return;
    }
    else if (dstId != BeingId_zero)
    {   // being to being
        Being *const srcBeing = actorManager->findBeing(srcId);
        Being *const dstBeing = actorManager->findBeing(dstId);
        skillDialog->playCastingSrcEffect(skillId, srcBeing);
        skillDialog->playCastingDstEffect(skillId, dstBeing);
    }
    else if (dstX != 0 || dstY != 0)
    {   // being to position
        UNIMPLIMENTEDPACKET;
    }
}

void BeingHandler::processBeingStatusChange(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingStatusChange")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingStatusChange")
        return;
    }

    // Status change
    const uint16_t status = msg.readInt16("status");
    const BeingId id = msg.readBeingId("being id");
    const Enable flag = fromBool(
        msg.readUInt8("flag: 0: stop, 1: start"), Enable);
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

void BeingHandler::processBeingStatusChange2(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingStatusChange")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingStatusChange")
        return;
    }

    // Status change
    const uint16_t status = msg.readInt16("status");
    const BeingId id = msg.readBeingId("being id");
    const Enable flag = fromBool(
        msg.readUInt8("flag: 0: stop, 1: start"), Enable);
    msg.readInt32("left");
    msg.readInt32("val1");
    msg.readInt32("val2");
    msg.readInt32("val3");

    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing)
        dstBeing->setStatusEffect(status, flag);
    BLOCK_END("BeingHandler::processBeingStatusChange")
}

void BeingHandler::processBeingMove2(Net::MessageIn &msg)
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
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

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

    dstBeing->setTileCoords(srcX, srcY);
    if (localPlayer)
        localPlayer->followMoveTo(dstBeing, srcX, srcY, dstX, dstY);
    if (!serverFeatures->haveMove3())
        dstBeing->setDestination(dstX, dstY);
    if (dstBeing->getType() == ActorType::Player)
        dstBeing->setMoveTime();
    BLOCK_END("BeingHandler::processBeingMove2")
}

void BeingHandler::processBeingAction2(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingAction2")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingAction2")
        return;
    }

    Being *const srcBeing = actorManager->findBeing(
        msg.readBeingId("src being id"));
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("dst being id"));

    msg.readInt32("tick");
    const int srcSpeed = msg.readInt32("src speed");
    msg.readInt32("dst speed");
    const int param1 = msg.readInt32("damage");
    msg.readInt16("count");
    const AttackTypeT type = static_cast<AttackTypeT>(
        msg.readUInt8("action"));
    msg.readInt32("left damage");

    switch (type)
    {
        case AttackType::HIT:  // Damage
        case AttackType::CRITICAL:  // Critical Damage
        case AttackType::MULTI:  // Critical Damage
        case AttackType::MULTI_REFLECT:
        case AttackType::REFLECT:  // Reflected Damage
        case AttackType::FLEE:  // Lucky Dodge
        case AttackType::SPLASH:
        case AttackType::SKILL:
        case AttackType::REPEATE:
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
                // level not present, using 1
                dstBeing->takeDamage(srcBeing, param1,
                    static_cast<AttackTypeT>(type), 1);
            }
            break;

        case AttackType::PICKUP:
            break;

        case AttackType::TOUCH_SKILL:
            break;

        case AttackType::SIT:
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

        case AttackType::STAND:
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
        case AttackType::MISS:
        case AttackType::SKILLMISS:
            UNIMPLIMENTEDPACKET;
            break;
    }
    BLOCK_END("BeingHandler::processBeingAction2")
}

void BeingHandler::processMonsterHp(Net::MessageIn &msg)
{
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("monster id"));
    const int hp = msg.readInt32("hp");
    const int maxHP = msg.readInt32("max hp");
    if (dstBeing)
    {
        dstBeing->setHP(hp);
        dstBeing->setMaxHP(maxHP);
    }
}

void BeingHandler::processSkillAutoCast(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("skill id");
    msg.readInt16("inf");
    msg.readInt16("unused");
    msg.readInt16("skill level");
    msg.readInt16("sp");
    msg.readInt16("range");
    msg.readString(24, "skill name");
    msg.readInt8("unused");
}

void BeingHandler::processRanksList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ here need window with rank tables.
    msg.readInt16("rank type");
    for (int f = 0; f < 10; f ++)
        msg.readString(24, "name");
    for (int f = 0; f < 10; f ++)
        msg.readInt32("points");
    msg.readInt32("my points");
}

void BeingHandler::processBlacksmithRanksList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ here need window with rank tables.
    for (int f = 0; f < 10; f ++)
        msg.readString(24, "name");
    for (int f = 0; f < 10; f ++)
        msg.readInt32("points");
}

void BeingHandler::processAlchemistRanksList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ here need window with rank tables.
    for (int f = 0; f < 10; f ++)
        msg.readString(24, "name");
    for (int f = 0; f < 10; f ++)
        msg.readInt32("points");
}

void BeingHandler::processTaekwonRanksList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ here need window with rank tables.
    for (int f = 0; f < 10; f ++)
        msg.readString(24, "name");
    for (int f = 0; f < 10; f ++)
        msg.readInt32("points");
}

void BeingHandler::processPkRanksList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ here need window with rank tables.
    for (int f = 0; f < 10; f ++)
        msg.readString(24, "name");
    for (int f = 0; f < 10; f ++)
        msg.readInt32("points");
}

void BeingHandler::processBeingChangeDirection(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingChangeDirection")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processBeingChangeDirection")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    msg.readInt16("head direction");

    const uint8_t dir = Net::MessageIn::fromServerDirection(
        static_cast<uint8_t>(msg.readUInt8("player direction") & 0x0FU));

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

void BeingHandler::processBeingSpecialEffect(Net::MessageIn &msg)
{
    if (!effectManager || !actorManager)
        return;

    const BeingId id = msg.readBeingId("being id");
    Being *const being = actorManager->findBeing(id);
    if (!being)
        return;

    const int effectType = msg.readInt32("effect type");

    if (Particle::enabled)
        effectManager->trigger(effectType, being);

    // +++ need dehard code effectType == 3
    if (effectType == 3 && being->getType() == ActorType::Player
        && socialWindow)
    {   // reset received damage
        socialWindow->resetDamage(being->getName());
    }
}

void BeingHandler::processBeingSpecialEffectNum(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ need somhow show this effects.
    // type is not same with self/misc effect.
    msg.readBeingId("account id");
    msg.readInt32("effect type");
    msg.readInt32("num");  // effect variable
}

void BeingHandler::processBeingSoundEffect(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ need play this effect.
    msg.readString(24, "sound effect name");
    msg.readUInt8("type");
    msg.readInt32("unused");
    msg.readInt32("source being id");
}

void BeingHandler::applyPlayerAction(Net::MessageIn &msg,
                                     Being *const being,
                                     const uint8_t type)
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
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void BeingHandler::viewPlayerEquipment(const Being *const being)
{
    if (!being)
        return;

    createOutPacket(CMSG_PLAYER_VIEW_EQUIPMENT);
    outMsg.writeBeingId(being->getId(), "account id");
}

void BeingHandler::processSkillGroundNoDamage(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("skill id");
    msg.readInt32("src id");
    msg.readInt16("val");
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readInt32("tick");
}

void BeingHandler::processSkillEntry(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
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

void BeingHandler::processPlaterStatusChange(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processPlayerStop")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processPlayerStop")
        return;
    }

    // Change in players' flags
    const BeingId id = msg.readBeingId("account id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    const uint16_t stunMode = msg.readInt16("stun mode");
    uint32_t statusEffects = msg.readInt16("status effect");
    statusEffects |= (static_cast<uint32_t>(msg.readInt32("opt?"))) << 16;
    dstBeing->setKarma(msg.readUInt8("karma"));

    dstBeing->setStunMode(stunMode);
    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));
    BLOCK_END("BeingHandler::processPlayerStop")
}

void BeingHandler::processPlaterStatusChange2(Net::MessageIn &msg)
{
    if (!actorManager)
        return;

    const BeingId id = msg.readBeingId("account id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    uint32_t statusEffects = msg.readInt32("status effect");
    dstBeing->setLevel(msg.readInt32("level"));
    msg.readInt32("showEFST");

    dstBeing->setStatusEffectBlock(0, static_cast<uint16_t>(
        (statusEffects >> 16) & 0xffff));
    dstBeing->setStatusEffectBlock(16, static_cast<uint16_t>(
        statusEffects & 0xffff));
}

void BeingHandler::processPlaterStatusChangeNoTick(Net::MessageIn &msg)
{
    const uint16_t status = msg.readInt16("index");
    const BeingId id = msg.readBeingId("account id");
    const Enable flag = fromBool(msg.readUInt8("state")
        ? true : false, Enable);

    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    dstBeing->setStatusEffect(status, flag);
}

void BeingHandler::processBeingResurrect(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingResurrect")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingHandler::processBeingResurrect")
        return;
    }

    // A being changed mortality status

    const BeingId id = msg.readBeingId("being id");
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

void BeingHandler::processPlayerGuilPartyInfo(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processPlayerGuilPartyInfo")
    if (!actorManager)
    {
        BLOCK_END("BeingHandler::processPlayerGuilPartyInfo")
        return;
    }

    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("being id"));

    if (dstBeing)
    {
        dstBeing->setName(msg.readString(24, "char name"));
        dstBeing->setPartyName(msg.readString(24, "party name"));
        dstBeing->setGuildName(msg.readString(24, "guild name"));
        dstBeing->setGuildPos(msg.readString(24, "guild pos"));
        dstBeing->addToCache();
    }
    BLOCK_END("BeingHandler::processPlayerGuilPartyInfo")
}

void BeingHandler::processBeingRemoveSkil(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ if skill unit was added, here need remove it from actors
    msg.readInt32("skill unit id");
}

void BeingHandler::processBeingFakeName(Net::MessageIn &msg)
{
    const BeingType::BeingType type = static_cast<BeingType::BeingType>(
        msg.readUInt8("object type"));
    const BeingId id = msg.readBeingId("npc id");
    msg.skip(8, "unused");
    const uint16_t job = msg.readInt16("class?");  // 111
    msg.skip(30, "unused");
    uint16_t x, y;
    uint8_t dir;
    msg.readCoordinates(x, y, dir, "position");
    msg.readUInt8("sx");
    msg.readUInt8("sy");
    msg.skip(4, "unsued");

    Being *const dstBeing = createBeing2(msg, id, job, type);
    dstBeing->setSubtype(fromInt(job, BeingTypeId), 0);
    dstBeing->setTileCoords(x, y);
    dstBeing->setDirection(dir);
}

void BeingHandler::processBeingStatUpdate1(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    const int type = msg.readInt16("type");
    const int value = msg.readInt32("value");

    Being *const dstBeing = actorManager->findBeing(id);
    if (!dstBeing)
        return;

    if (type != Ea::MANNER)
    {
        UNIMPLIMENTEDPACKET;
        return;
    }
    dstBeing->setManner(value);
}

void BeingHandler::processBeingSelfEffect(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processBeingSelfEffect")
    if (!effectManager || !actorManager)
    {
        BLOCK_END("BeingHandler::processBeingSelfEffect")
        return;
    }

    const BeingId id = msg.readBeingId("being id");
    Being *const being = actorManager->findBeing(id);
    if (!being)
    {
        BLOCK_END("BeingHandler::processBeingSelfEffect")
        return;
    }

    const int effectType = msg.readInt32("effect type");
    if (Particle::enabled)
        effectManager->trigger(effectType, being);

    BLOCK_END("BeingHandler::processBeingSelfEffect")
}

void BeingHandler::processMobInfo(Net::MessageIn &msg)
{
    const int len = msg.readInt16("len");
    if (len < 12)
        return;
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("monster id"));
    const int attackRange = msg.readInt32("range");
    if (dstBeing)
        dstBeing->setAttackRange(attackRange);
}

void BeingHandler::processBeingAttrs(Net::MessageIn &msg)
{
    const int len = msg.readInt16("len");
    if (len < 12)
        return;
    Being *const dstBeing = actorManager->findBeing(
        msg.readBeingId("player id"));
    const int gmLevel = msg.readInt32("gm level");
    if (dstBeing && gmLevel)
    {
        if (dstBeing == localPlayer)
            localPlayer->setGMLevel(gmLevel);
        dstBeing->setGM(true);
    }
    else
    {
        if (dstBeing == localPlayer)
            localPlayer->setGMLevel(0);
        if (dstBeing)
            dstBeing->setGM(false);
    }
}

void BeingHandler::processMonsterInfo(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("class");
    msg.readInt16("level");
    msg.readInt16("size");
    msg.readInt32("hp");
    msg.readInt16("def");
    msg.readInt16("race");
    msg.readInt16("mdef");
    msg.readInt16("ele");
}

void BeingHandler::processClassChange(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("being id");
    msg.readUInt8("type");
    msg.readInt32("class");
}

void BeingHandler::processSpiritBalls(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("being id");
    msg.readInt16("spirits amount");
}

void BeingHandler::processSpiritBallSingle(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("being id");
    msg.readInt16("spirits amount");
}

void BeingHandler::processBladeStop(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt32("src being id");
    msg.readInt32("dst being id");
    msg.readInt32("flag");
}

void BeingHandler::processComboDelay(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("being id");
    msg.readInt32("wait");
}

void BeingHandler::processWddingEffect(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("being id");
}

void BeingHandler::processBeingSlide(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("being id");
    msg.readInt16("x");
    msg.readInt16("y");
}

void BeingHandler::processStarsKill(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readString(24, "map name");
    msg.readInt32("monster id");
    msg.readUInt8("start");
    msg.readUInt8("result");
}

void BeingHandler::processGladiatorFeelRequest(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readUInt8("which");
}

void BeingHandler::processBossMapInfo(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readUInt8("info type");
    msg.readInt32("x");
    msg.readInt32("y");
    msg.readInt16("min hours");
    msg.readInt16("min minutes");
    msg.readInt16("max hours");
    msg.readInt16("max minutes");
    msg.readString(24, "monster name");  // really can be used 51 byte?
}

void BeingHandler::processBeingFont(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("account id");
    msg.readInt16("font");
}

void BeingHandler::processBeingMilleniumShield(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("account id");
    msg.readInt16("shields");
    msg.readInt16("unused");
}

void BeingHandler::processBeingCharm(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readBeingId("account id");
    msg.readInt16("charm type");
    msg.readInt16("charm count");
}

void BeingHandler::processBeingViewEquipment(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    const int count = (msg.readInt16("len") - 45) / 31;
    msg.readString(24, "name");
    msg.readInt16("job");
    msg.readInt16("head");
    msg.readInt16("accessory");
    msg.readInt16("accessory2");
    msg.readInt16("accessory3");
    msg.readInt16("robe");
    msg.readInt16("hair color");
    msg.readInt16("body color");
    msg.readUInt8("gender");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("index");
        msg.readInt16("item id");
        msg.readUInt8("item type");
        msg.readInt32("location");
        msg.readInt32("wear state");
        msg.readInt8("refine");
        for (int d = 0; d < 4; d ++)
            msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        msg.readInt16("equip type");
        msg.readInt16("item sprite number");
        msg.readUInt8("flags");
    }
}

void BeingHandler::processPvpSet(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processPvpSet")
    const BeingId id = msg.readBeingId("being id");
    const int rank = msg.readInt32("rank");
    msg.readInt32("num");
    if (actorManager)
    {
        Being *const dstBeing = actorManager->findBeing(id);
        if (dstBeing)
            dstBeing->setPvpRank(rank);
    }
    BLOCK_END("BeingHandler::processPvpSet")
}

void BeingHandler::processNameResponse2(Net::MessageIn &msg)
{
    BLOCK_START("BeingHandler::processNameResponse2")
    if (!actorManager || !localPlayer)
    {
        BLOCK_END("BeingHandler::processNameResponse2")
        return;
    }

    const int len = msg.readInt16("len");
    const BeingId beingId = msg.readBeingId("account ic");
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
    BLOCK_END("BeingHandler::processNameResponse2")
}

}  // namespace EAthena
