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
#include "itemcolormanager.h"
#include "notifymanager.h"
#include "party.h"

#include "being/mercenaryinfo.h"

#include "enums/resources/notifytypes.h"

#include "particle/particle.h"

#include "input/keyboardconfig.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/outfitwindow.h"

#include "net/serverfeatures.h"

#include "net/ea/eaprotocol.h"

#include "net/charserverhandler.h"

#include "net/character.h"

#include "net/ea/beingnet.h"

#include "net/eathena/beingnet.h"
#include "net/eathena/maptypeproperty2.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"
#include "net/eathena/sprite.h"

#include "resources/iteminfo.h"
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
        SMSG_BEING_CHANGE_LOOKS_CARDS,
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
            BeingNet::processBeingVisible(msg);
            break;

        case SMSG_BEING_MOVE:
            BeingNet::processBeingMove(msg);
            break;

        case SMSG_BEING_SPAWN:
            BeingNet::processBeingSpawn(msg);
            break;

        case SMSG_BEING_MOVE2:
            BeingNet::processBeingMove2(msg);
            break;

        case SMSG_BEING_REMOVE:
            Ea::BeingNet::processBeingRemove(msg);
            break;

        case SMSG_BEING_REMOVE_SKILL:
            BeingNet::processBeingRemoveSkil(msg);
            break;

        case SMSG_BEING_RESURRECT:
            BeingNet::processBeingResurrect(msg);
            break;

        case SMSG_SKILL_DAMAGE:
            Ea::BeingNet::processSkillDamage(msg);
            break;

        case SMSG_SKILL_AUTO_CAST:
            BeingNet::processSkillAutoCast(msg);
            break;

        case SMSG_BEING_ACTION:
            Ea::BeingNet::processBeingAction(msg);
            break;

        case SMSG_BEING_ACTION2:
            BeingNet::processBeingAction2(msg);
            break;

        case SMSG_BEING_SELFEFFECT:
            BeingNet::processBeingSelfEffect(msg);
            break;

        case SMSG_BEING_SPECIAL_EFFECT:
            BeingNet::processBeingSpecialEffect(msg);
            break;

        case SMSG_BEING_SPECIAL_EFFECT_NUM:
            BeingNet::processBeingSpecialEffectNum(msg);
            break;

        case SMSG_BEING_SOUND_EFFECT:
            BeingNet::processBeingSoundEffect(msg);
            break;

        case SMSG_BEING_EMOTION:
            Ea::BeingNet::processBeingEmotion(msg);
            break;

        case SMSG_BEING_CHANGE_LOOKS2:
            BeingNet::processBeingChangeLook2(msg);
            break;

        case SMSG_BEING_CHANGE_LOOKS_CARDS:
            BeingNet::processBeingChangeLookCards(msg);
            break;

        case SMSG_BEING_NAME_RESPONSE:
            Ea::BeingNet::processNameResponse(msg);
            break;

        case SMSG_BEING_NAME_RESPONSE2:
            BeingNet::processNameResponse2(msg);
            break;

        case SMSG_SOLVE_CHAR_NAME:
            break;

        case SMSG_PLAYER_GUILD_PARTY_INFO:
            BeingNet::processPlayerGuilPartyInfo(msg);
            break;

        case SMSG_BEING_CHANGE_DIRECTION:
            BeingNet::processBeingChangeDirection(msg);
            break;

        case SMSG_PLAYER_STOP:
            Ea::BeingNet::processPlayerStop(msg);
            break;

        case SMSG_PLAYER_MOVE_TO_ATTACK:
            Ea::BeingNet::processPlayerMoveToAttack(msg);
            break;

        case SMSG_PLAYER_STATUS_CHANGE:
            BeingNet::processPlaterStatusChange(msg);
            break;

        case SMSG_PLAYER_STATUS_CHANGE2:
            BeingNet::processPlaterStatusChange2(msg);
            break;

        case SMSG_PLAYER_STATUS_CHANGE_NO_TICK:
            BeingNet::processPlaterStatusChangeNoTick(msg);
            break;

        case SMSG_BEING_STATUS_CHANGE:
            BeingNet::processBeingStatusChange(msg);
            break;

        case SMSG_BEING_STATUS_CHANGE2:
            BeingNet::processBeingStatusChange2(msg);
            break;

        case SMSG_SKILL_CASTING:
            BeingNet::processSkillCasting(msg);
            break;

        case SMSG_SKILL_CAST_CANCEL:
            msg.readInt32("id?");
            break;

        case SMSG_SKILL_NO_DAMAGE:
            Ea::BeingNet::processSkillNoDamage(msg);
            break;

        case SMSG_SKILL_GROUND_NO_DAMAGE:
            BeingNet::processSkillGroundNoDamage(msg);
            break;

        case SMSG_SKILL_ENTRY:
            BeingNet::processSkillEntry(msg);
            break;

        case SMSG_PVP_MAP_MODE:
            Ea::BeingNet::processPvpMapMode(msg);
            break;

        case SMSG_PVP_SET:
            BeingNet::processPvpSet(msg);
            break;

        case SMSG_MAP_TYPE_PROPERTY2:
            BeingNet::processMapTypeProperty(msg);
            break;

        case SMSG_MAP_TYPE:
            BeingNet::processMapType(msg);
            break;

        case SMSG_MONSTER_HP:
        case SMSG_PLAYER_HP:
            BeingNet::processMonsterHp(msg);
            break;

        case SMSG_RANKS_LIST:
            BeingNet::processRanksList(msg);
            break;

        case SMSG_BEING_FAKE_NAME:
            BeingNet::processBeingFakeName(msg);
            break;

        case SMSG_BEING_STAT_UPDATE_1:
            BeingNet::processBeingStatUpdate1(msg);
            break;

        case SMSG_MOB_INFO:
            BeingNet::processMobInfo(msg);
            break;

        case SMSG_BEING_MOVE3:
            Ea::BeingNet::processBeingMove3(msg);
            break;

        case SMSG_BEING_ATTRS:
            BeingNet::processBeingAttrs(msg);
            break;

        case SMSG_MONSTER_INFO:
            BeingNet::processMonsterInfo(msg);
            break;

        case SMSG_CLASS_CHANGE:
            BeingNet::processClassChange(msg);
            break;

        case SMSG_SPIRIT_BALLS:
            BeingNet::processSpiritBalls(msg);
            break;

        case SMSG_SPIRIT_BALL_SINGLE:
            BeingNet::processSpiritBallSingle(msg);
            break;

        case SMSG_BLADE_STOP:
            BeingNet::processBladeStop(msg);
            break;

        case SMSG_COMBO_DELAY:
            BeingNet::processComboDelay(msg);
            break;

        case SMSG_WEDDING_EFFECT:
            BeingNet::processWddingEffect(msg);
            break;

        case SMSG_BEING_SLIDE:
            BeingNet::processBeingSlide(msg);
            break;

        case SMSG_STARS_KILL:
            BeingNet::processStarsKill(msg);
            break;

        case SMSG_BLACKSMITH_RANKS_LIST:
            BeingNet::processBlacksmithRanksList(msg);
            break;

        case SMSG_ALCHEMIST_RANKS_LIST:
            BeingNet::processAlchemistRanksList(msg);
            break;

        case SMSG_TAEKWON_RANKS_LIST:
            BeingNet::processTaekwonRanksList(msg);
            break;

        case SMSG_PK_RANKS_LIST:
            BeingNet::processPkRanksList(msg);
            break;

        case SMSG_GLADIATOR_FEEL_REQUEST:
            BeingNet::processGladiatorFeelRequest(msg);
            break;

        case SMSG_BOSS_MAP_INFO:
            BeingNet::processBossMapInfo(msg);
            break;

        case SMSG_BEING_FONT:
            BeingNet::processBeingFont(msg);
            break;

        case SMSG_BEING_MILLENIUM_SHIELD:
            BeingNet::processBeingMilleniumShield(msg);
            break;

        case SMSG_BEING_CHARM:
            BeingNet::processBeingCharm(msg);
            break;

        case SMSG_BEING_VIEW_EQUIPMENT:
            BeingNet::processBeingViewEquipment(msg);
            break;

        default:
            break;
    }
}

void BeingHandler::undress(Being *const being) const
{
    if (!being)
        return;
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

void BeingHandler::viewPlayerEquipment(const Being *const being)
{
    if (!being)
        return;

    createOutPacket(CMSG_PLAYER_VIEW_EQUIPMENT);
    outMsg.writeBeingId(being->getId(), "account id");
}

}  // namespace EAthena
