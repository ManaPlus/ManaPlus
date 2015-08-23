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

#include "net/tmwa/beingnet.h"
#include "net/tmwa/guildmanager.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"
#include "net/tmwa/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/stringutils.h"
#include "utils/timer.h"

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
            BeingNet::processBeingVisible(msg);
            break;

        case SMSG_BEING_MOVE:
            BeingNet::processBeingMove(msg);
            break;

        case SMSG_BEING_MOVE2:
            BeingNet::processBeingMove2(msg);
            break;

        case SMSG_BEING_MOVE3:
            Ea::BeingNet::processBeingMove3(msg);
            break;

        case SMSG_BEING_SPAWN:
            BeingNet::processBeingSpawn(msg);
            break;

        case SMSG_BEING_REMOVE:
            Ea::BeingNet::processBeingRemove(msg);
            break;

        case SMSG_BEING_RESURRECT:
            BeingNet::processBeingResurrect(msg);
            break;

        case SMSG_SKILL_DAMAGE:
            Ea::BeingNet::processSkillDamage(msg);
            break;

        case SMSG_BEING_ACTION:
            Ea::BeingNet::processBeingAction(msg);
            break;

        case SMSG_BEING_SELFEFFECT:
            BeingNet::processBeingSelfEffect(msg);
            break;

        case SMSG_BEING_EMOTION:
            Ea::BeingNet::processBeingEmotion(msg);
            break;

        case SMSG_BEING_CHANGE_LOOKS:
            BeingNet::processBeingChangeLook(msg);
            break;

        case SMSG_BEING_CHANGE_LOOKS2:
            BeingNet::processBeingChangeLook2(msg);
            break;

        case SMSG_BEING_NAME_RESPONSE:
            Ea::BeingNet::processNameResponse(msg);
            break;

        case SMSG_BEING_IP_RESPONSE:
            BeingNet::processIpResponse(msg);
            break;

        case SMSG_SOLVE_CHAR_NAME:
            break;

        case SMSG_PLAYER_GUILD_PARTY_INFO:
            BeingNet::processPlayerGuilPartyInfo(msg);
            break;

        case SMSG_BEING_CHANGE_DIRECTION:
            BeingNet::processBeingChangeDirection(msg);
            break;

        case SMSG_PLAYER_UPDATE_1:
            BeingNet::processPlayerUpdate1(msg);
            break;

        case SMSG_PLAYER_UPDATE_2:
            BeingNet::processPlayerUpdate2(msg);
            break;

        case SMSG_PLAYER_MOVE:
            BeingNet::processPlayerMove(msg);
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

        case SMSG_BEING_STATUS_CHANGE:
            BeingNet::processBeingStatusChange(msg);
            break;

        case SMSG_SKILL_CASTING:
            BeingNet::processSkillCasting(msg);
            break;

        case SMSG_SKILL_CAST_CANCEL:
            BeingNet::processSkillCastCancel(msg);
            break;

        case SMSG_SKILL_NO_DAMAGE:
            Ea::BeingNet::processSkillNoDamage(msg);
            break;

        case SMSG_PVP_MAP_MODE:
            Ea::BeingNet::processPvpMapMode(msg);
            break;

        case SMSG_PVP_SET:
            BeingNet::processPvpSet(msg);
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
