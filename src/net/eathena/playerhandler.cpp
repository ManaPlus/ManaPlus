/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "net/eathena/playerhandler.h"

#include "configuration.h"
#include "client.h"
#include "logger.h"

#include "net/messagein.h"

#include "net/eathena/protocol.h"
#include "net/eathena/npchandler.h"
#include "net/eathena/inventoryhandler.h"

#include "gui/whoisonline.h"

#include "debug.h"

extern Net::PlayerHandler *playerHandler;

namespace EAthena
{

PlayerHandler::PlayerHandler()
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
        SMSG_PLAYER_SHORTCUTS,
        SMSG_PLAYER_SHOW_EQUIP,
        0
    };
    handledMessages = _messages;
    playerHandler = this;
}

void PlayerHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_WALK_RESPONSE:
            processWalkResponse(msg);
            break;

        case SMSG_PLAYER_WARP:
            processPlayerWarp(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_1:
            processPlayerStatUpdate1(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_2:
            processPlayerStatUpdate2(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_3: // Update a base attribute
            processPlayerStatUpdate3(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_4: // Attribute increase ack
            processPlayerStatUpdate4(msg);
            break;

        // Updates stats and status points
        case SMSG_PLAYER_STAT_UPDATE_5:
            processPlayerStatUpdate5(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_6:
            processPlayerStatUpdate6(msg);
            break;

        case SMSG_PLAYER_ARROW_MESSAGE:
            processPlayerArrowMessage(msg);
            break;

        case SMSG_PLAYER_SHORTCUTS:
            processPlayerShortcuts(msg);
            break;

        case SMSG_PLAYER_SHOW_EQUIP:
            processPlayerShowEquip(msg);
            break;

        default:
            break;
    }
}

void PlayerHandler::attack(int id, bool keep)
{
    MessageOut outMsg(CMSG_PLAYER_ATTACK);
    outMsg.writeInt32(id);
    if (keep)
        outMsg.writeInt8(7);
    else
        outMsg.writeInt8(0);
}

void PlayerHandler::stopAttack()
{
    MessageOut outMsg(CMSG_PLAYER_STOP_ATTACK);
}

void PlayerHandler::emote(uint8_t emoteId)
{
    MessageOut outMsg(CMSG_PLAYER_EMOTE);
    outMsg.writeInt8(emoteId);
}

void PlayerHandler::increaseAttribute(int attr)
{
    if (attr >= STR && attr <= LUK)
    {
        MessageOut outMsg(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(static_cast<int16_t>(attr));
        outMsg.writeInt8(1);
    }
}

void PlayerHandler::increaseSkill(unsigned short skillId)
{
    if (PlayerInfo::getAttribute(SKILL_POINTS) <= 0)
        return;

    MessageOut outMsg(CMSG_SKILL_LEVELUP_REQUEST);
    outMsg.writeInt16(skillId);
}

void PlayerHandler::pickUp(FloorItem *floorItem)
{
    if (!floorItem)
        return;

    MessageOut outMsg(CMSG_ITEM_PICKUP);
    outMsg.writeInt32(floorItem->getId());
    EAthena::InventoryHandler *handler =
        static_cast<EAthena::InventoryHandler*>(Net::getInventoryHandler());
    if (handler)
        handler->pushPickup(floorItem->getId());
}

void PlayerHandler::setDirection(char direction)
{
    MessageOut outMsg(CMSG_PLAYER_CHANGE_DIR);
    outMsg.writeInt16(0);
    outMsg.writeInt8(direction);
}

void PlayerHandler::setDestination(int x, int y, int direction)
{
    MessageOut outMsg(CMSG_PLAYER_CHANGE_DEST);
    outMsg.writeCoordinates(static_cast<short unsigned int>(x),
        static_cast<short unsigned int>(y),
        static_cast<unsigned char>(direction));
}

void PlayerHandler::changeAction(Being::Action action)
{
    char type;
    switch (action)
    {
        case Being::SIT:
            type = 2;
            break;
        case Being::STAND:
            type = 3;
            break;
        default:
        case Being::MOVE:
        case Being::ATTACK:
        case Being::DEAD:
        case Being::HURT:
        case Being::SPAWN:
            return;
    }

    MessageOut outMsg(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeInt32(0);
    outMsg.writeInt8(type);
}

void PlayerHandler::respawn()
{
    MessageOut outMsg(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(0);
}

void PlayerHandler::requestOnlineList()
{
}

void PlayerHandler::updateStatus(uint8_t status)
{
    MessageOut outMsg(CMSG_SET_STATUS);
    outMsg.writeInt8(status);
    outMsg.writeInt8(0);
}

void PlayerHandler::processPlayerShortcuts(Net::MessageIn &msg)
{
    for (int f = 0; f < 27; f ++)
    {
        msg.readInt8();     // type 0: item, 1: skill
        msg.readInt32();    // item or skill id
        msg.readInt16();    // lvl
    }
}

void PlayerHandler::processPlayerShowEquip(Net::MessageIn &msg)
{
    msg.readInt8();     // show equip
}

} // namespace EAthena
