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

#include "net/tmwa/beinghandler.h"

#include "net/ea/beingrecv.h"

#include "net/tmwa/beingrecv.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"
#include "net/tmwa/sprite.h"

#include "debug.h"

extern Net::BeingHandler *beingHandler;
extern int serverVersion;

namespace TmwAthena
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
        SMSG_BEING_SELFEFFECT,
        SMSG_BEING_EMOTION,
        SMSG_BEING_CHANGE_LOOKS,
        SMSG_BEING_CHANGE_LOOKS2,
        SMSG_BEING_NAME_RESPONSE,
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
        SMSG_BEING_MOVE3,
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
    BLOCK_START("BeingHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_BEING_VISIBLE:
            BeingRecv::processBeingVisible(msg);
            break;

        case SMSG_BEING_MOVE:
            BeingRecv::processBeingMove(msg);
            break;

        case SMSG_BEING_MOVE2:
            BeingRecv::processBeingMove2(msg);
            break;

        case SMSG_BEING_MOVE3:
            Ea::BeingRecv::processBeingMove3(msg);
            break;

        case SMSG_BEING_SPAWN:
            BeingRecv::processBeingSpawn(msg);
            break;

        case SMSG_BEING_REMOVE:
            Ea::BeingRecv::processBeingRemove(msg);
            break;

        case SMSG_BEING_RESURRECT:
            BeingRecv::processBeingResurrect(msg);
            break;

        case SMSG_SKILL_DAMAGE:
            Ea::BeingRecv::processSkillDamage(msg);
            break;

        case SMSG_BEING_ACTION:
            Ea::BeingRecv::processBeingAction(msg);
            break;

        case SMSG_BEING_SELFEFFECT:
            BeingRecv::processBeingSelfEffect(msg);
            break;

        case SMSG_BEING_EMOTION:
            Ea::BeingRecv::processBeingEmotion(msg);
            break;

        case SMSG_BEING_CHANGE_LOOKS:
            BeingRecv::processBeingChangeLook(msg);
            break;

        case SMSG_BEING_CHANGE_LOOKS2:
            BeingRecv::processBeingChangeLook2(msg);
            break;

        case SMSG_BEING_NAME_RESPONSE:
            Ea::BeingRecv::processNameResponse(msg);
            break;

        case SMSG_BEING_IP_RESPONSE:
            BeingRecv::processIpResponse(msg);
            break;

        case SMSG_SOLVE_CHAR_NAME:
            break;

        case SMSG_PLAYER_GUILD_PARTY_INFO:
            BeingRecv::processPlayerGuilPartyInfo(msg);
            break;

        case SMSG_BEING_CHANGE_DIRECTION:
            BeingRecv::processBeingChangeDirection(msg);
            break;

        case SMSG_PLAYER_UPDATE_1:
            BeingRecv::processPlayerUpdate1(msg);
            break;

        case SMSG_PLAYER_UPDATE_2:
            BeingRecv::processPlayerUpdate2(msg);
            break;

        case SMSG_PLAYER_MOVE:
            BeingRecv::processPlayerMove(msg);
            break;

        case SMSG_PLAYER_STOP:
            Ea::BeingRecv::processPlayerStop(msg);
            break;

        case SMSG_PLAYER_MOVE_TO_ATTACK:
            Ea::BeingRecv::processPlayerMoveToAttack(msg);
            break;

        case SMSG_PLAYER_STATUS_CHANGE:
            BeingRecv::processPlaterStatusChange(msg);
            break;

        case SMSG_BEING_STATUS_CHANGE:
            BeingRecv::processBeingStatusChange(msg);
            break;

        case SMSG_SKILL_CASTING:
            BeingRecv::processSkillCasting(msg);
            break;

        case SMSG_SKILL_CAST_CANCEL:
            BeingRecv::processSkillCastCancel(msg);
            break;

        case SMSG_SKILL_NO_DAMAGE:
            Ea::BeingRecv::processSkillNoDamage(msg);
            break;

        case SMSG_PVP_MAP_MODE:
            Ea::BeingRecv::processPvpMapMode(msg);
            break;

        case SMSG_PVP_SET:
            BeingRecv::processPvpSet(msg);
            break;

        default:
            break;
    }
    BLOCK_END("BeingHandler::handleMessage")
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

#ifdef EATHENA_SUPPORT
void BeingHandler::requestRanks(const RankT rank A_UNUSED) const
{
}
#endif

void BeingHandler::viewPlayerEquipment(const Being *const being A_UNUSED)
{
}

}  // namespace TmwAthena
