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

#include "net/tmwa/playerhandler.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/statuswindow.h"

#include "net/ea/playerrecv.h"

#include "net/tmwa/inventoryhandler.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/playerrecv.h"
#include "net/tmwa/protocol.h"

#include "debug.h"

extern Net::PlayerHandler *playerHandler;
extern int serverVersion;

namespace TmwAthena
{

PlayerHandler::PlayerHandler() :
    MessageHandler(),
    Ea::PlayerHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_WALK_RESPONSE,
        SMSG_PLAYER_WARP,
        SMSG_PLAYER_STAT_UPDATE_1,
        SMSG_PLAYER_STAT_UPDATE_2,
        SMSG_PLAYER_STAT_UPDATE_3,
        SMSG_PLAYER_STAT_UPDATE_4,
        SMSG_PLAYER_STAT_UPDATE_5,
        SMSG_PLAYER_STAT_UPDATE_6,
        SMSG_PLAYER_ARROW_MESSAGE,
        SMSG_MAP_MUSIC,
        0
    };
    handledMessages = _messages;
    playerHandler = this;
}

void PlayerHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_WALK_RESPONSE:
            PlayerRecv::processWalkResponse(msg);
            break;

        case SMSG_PLAYER_WARP:
            Ea::PlayerRecv::processPlayerWarp(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_1:
            Ea::PlayerRecv::processPlayerStatUpdate1(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_2:
            Ea::PlayerRecv::processPlayerStatUpdate2(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_3:  // Update a base attribute
            Ea::PlayerRecv::processPlayerStatUpdate3(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_4:  // Attribute increase ack
            Ea::PlayerRecv::processPlayerStatUpdate4(msg);
            break;

        // Updates stats and status points
        case SMSG_PLAYER_STAT_UPDATE_5:
            PlayerRecv::processPlayerStatUpdate5(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_6:
            Ea::PlayerRecv::processPlayerStatUpdate6(msg);
            break;

        case SMSG_PLAYER_ARROW_MESSAGE:
            Ea::PlayerRecv::processPlayerArrowMessage(msg);
            break;

        case SMSG_MAP_MUSIC:
            Ea::PlayerRecv::processMapMusic(msg);
            break;

        default:
            break;
    }
    BLOCK_END("PlayerHandler::handleMessage")
}

void PlayerHandler::attack(const BeingId id,
                           const Keep keep) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeBeingId(id, "target id");
    if (keep == Keep_true)
        outMsg.writeInt8(7, "action");
    else
        outMsg.writeInt8(0, "action");
}

void PlayerHandler::stopAttack() const
{
    createOutPacket(CMSG_PLAYER_STOP_ATTACK);
}

void PlayerHandler::emote(const uint8_t emoteId) const
{
    createOutPacket(CMSG_PLAYER_EMOTE);
    outMsg.writeInt8(emoteId, "emote id");
}

void PlayerHandler::increaseAttribute(const AttributesT attr) const
{
    if (attr >= Attributes::STR && attr <= Attributes::LUK)
    {
        createOutPacket(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(static_cast<int16_t>(attr), "attribute id");
        outMsg.writeInt8(1, "increment");
    }
}

void PlayerHandler::increaseSkill(const uint16_t skillId) const
{
    if (PlayerInfo::getAttribute(Attributes::SKILL_POINTS) <= 0)
        return;

    createOutPacket(CMSG_SKILL_LEVELUP_REQUEST);
    outMsg.writeInt16(skillId, "skill id");
}

void PlayerHandler::pickUp(const FloorItem *const floorItem) const
{
    if (!floorItem)
        return;

    createOutPacket(CMSG_ITEM_PICKUP);
    outMsg.writeBeingId(floorItem->getId(), "object id");
    TmwAthena::InventoryHandler *const handler =
        static_cast<TmwAthena::InventoryHandler*>(inventoryHandler);
    if (handler)
        handler->pushPickup(floorItem->getId());
}

void PlayerHandler::setDirection(const unsigned char direction) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_DIR);
    outMsg.writeInt16(0, "unused");
    outMsg.writeInt8(direction, "direction");
}

void PlayerHandler::setDestination(const int x, const int y,
                                   const int direction) const
{
    createOutPacket(CMSG_PLAYER_CHANGE_DEST);
    outMsg.writeCoordinates(static_cast<uint16_t>(x),
        static_cast<uint16_t>(y),
        static_cast<unsigned char>(direction), "destination");
}

void PlayerHandler::changeAction(const BeingActionT &action) const
{
    char type;
    switch (action)
    {
        case BeingAction::SIT:
            type = 2;
            break;
        case BeingAction::STAND:
        case BeingAction::PRESTAND:
            type = 3;
            break;
        default:
        case BeingAction::MOVE:
        case BeingAction::ATTACK:
        case BeingAction::DEAD:
        case BeingAction::HURT:
        case BeingAction::SPAWN:
            return;
    }

    createOutPacket(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt8(type, "action");
}

void PlayerHandler::respawn() const
{
    createOutPacket(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(0, "action");
}

void PlayerHandler::requestOnlineList() const
{
}

void PlayerHandler::removeOption() const
{
}

void PlayerHandler::changeCart(const int type A_UNUSED) const
{
}

void PlayerHandler::setMemo() const
{
}

void PlayerHandler::updateStatus(const uint8_t status A_UNUSED) const
{
}

void PlayerHandler::setShortcut(const int idx A_UNUSED,
                                const uint8_t type A_UNUSED,
                                const int id A_UNUSED,
                                const int level A_UNUSED) const
{
}

void PlayerHandler::doriDori() const
{
}

void PlayerHandler::explosionSpirits() const
{
}

void PlayerHandler::requestPvpInfo() const
{
}

void PlayerHandler::revive() const
{
}

void PlayerHandler::setViewEquipment(const bool allow A_UNUSED) const
{
}

void PlayerHandler::setStat(Net::MessageIn &msg,
                            const int type,
                            const int base,
                            const int mod,
                            const Notify notify) const
{
    if (type == 500)
    {
        localPlayer->setGMLevel(base);
        return;
    }
    Ea::PlayerHandler::setStat(msg, type, base, mod, notify);
}

}  // namespace TmwAthena
