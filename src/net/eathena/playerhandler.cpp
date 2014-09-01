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

#include "net/eathena/playerhandler.h"

#include "being/attributes.h"
#include "being/localplayer.h"

#include "gui/windows/statuswindow.h"

#include "net/net.h"

#include "net/eathena/attrs.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"
#include "net/eathena/inventoryhandler.h"

#include "debug.h"

extern Net::PlayerHandler *playerHandler;

namespace EAthena
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

        case SMSG_PLAYER_STAT_UPDATE_3:  // Update a base attribute
            processPlayerStatUpdate3(msg);
            break;

        case SMSG_PLAYER_STAT_UPDATE_4:  // Attribute increase ack
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

void PlayerHandler::attack(const int id, const bool keep) const
{
    MessageOut outMsg(CMSG_PLAYER_ATTACK);
    outMsg.writeInt32(id);
    if (keep)
        outMsg.writeInt8(7);
    else
        outMsg.writeInt8(0);
}

void PlayerHandler::stopAttack() const
{
    MessageOut outMsg(CMSG_PLAYER_STOP_ATTACK);
}

void PlayerHandler::emote(const uint8_t emoteId) const
{
    MessageOut outMsg(CMSG_PLAYER_EMOTE);
    outMsg.writeInt8(emoteId);
}

void PlayerHandler::increaseAttribute(const int attr) const
{
    if (attr >= STR && attr <= LUK)
    {
        MessageOut outMsg(CMSG_STAT_UPDATE_REQUEST);
        outMsg.writeInt16(static_cast<int16_t>(attr));
        outMsg.writeInt8(1);
    }
}

void PlayerHandler::increaseSkill(const uint16_t skillId) const
{
    if (PlayerInfo::getAttribute(Attributes::SKILL_POINTS) <= 0)
        return;

    MessageOut outMsg(CMSG_SKILL_LEVELUP_REQUEST);
    outMsg.writeInt16(skillId);
}

void PlayerHandler::pickUp(const FloorItem *const floorItem) const
{
    if (!floorItem)
        return;

    MessageOut outMsg(CMSG_ITEM_PICKUP);
    outMsg.writeInt32(floorItem->getId());
    EAthena::InventoryHandler *const handler =
        static_cast<EAthena::InventoryHandler*>(Net::getInventoryHandler());
    if (handler)
        handler->pushPickup(floorItem->getId());
}

void PlayerHandler::setDirection(const unsigned char direction) const
{
    MessageOut outMsg(CMSG_PLAYER_CHANGE_DIR);
    outMsg.writeInt16(0);
    outMsg.writeInt8(direction);
}

void PlayerHandler::setDestination(const int x, const int y,
                                   const int direction) const
{
    MessageOut outMsg(CMSG_PLAYER_CHANGE_DEST);
    outMsg.writeCoordinates(static_cast<uint16_t>(x),
        static_cast<uint16_t>(y),
        static_cast<unsigned char>(direction));
}

void PlayerHandler::changeAction(const BeingAction::Action &action) const
{
    unsigned char type;
    switch (action)
    {
        case BeingAction::SIT:
            type = 2;
            break;
        case BeingAction::STAND:
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

    MessageOut outMsg(CMSG_PLAYER_CHANGE_ACT);
    outMsg.writeInt32(0);
    outMsg.writeInt8(type);
}

void PlayerHandler::respawn() const
{
    MessageOut outMsg(CMSG_PLAYER_RESTART);
    outMsg.writeInt8(0);
}

void PlayerHandler::requestOnlineList() const
{
}

void PlayerHandler::updateStatus(const uint8_t status) const
{
    MessageOut outMsg(CMSG_SET_STATUS);
    outMsg.writeInt8(status);
    outMsg.writeInt8(0);
}

void PlayerHandler::processPlayerShortcuts(Net::MessageIn &msg)
{
    for (int f = 0; f < 27; f ++)
    {
        msg.readUInt8("type 0: item, 1: skill");
        msg.readInt32("item or skill id");
        msg.readInt16("skill level");
    }
}

void PlayerHandler::processPlayerShowEquip(Net::MessageIn &msg)
{
    msg.readUInt8();  // show equip
}

void PlayerHandler::processPlayerStatUpdate5(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate5")
    PlayerInfo::setAttribute(Attributes::CHAR_POINTS,
        msg.readInt16("char points"));

    unsigned int val = msg.readUInt8("str");
    PlayerInfo::setStatBase(STR, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(STR, msg.readUInt8("str cost"));
    else
        msg.readUInt8("str need");

    val = msg.readUInt8("agi");
    PlayerInfo::setStatBase(AGI, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(AGI, msg.readUInt8("agi cost"));
    else
        msg.readUInt8("agi cost");

    val = msg.readUInt8("vit");
    PlayerInfo::setStatBase(VIT, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(VIT, msg.readUInt8("vit cost"));
    else
        msg.readUInt8("vit cost");

    val = msg.readUInt8("int");
    PlayerInfo::setStatBase(INT, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(INT, msg.readUInt8("int cost"));
    else
        msg.readUInt8("int cost");

    val = msg.readUInt8("dex");
    PlayerInfo::setStatBase(DEX, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(DEX, msg.readUInt8("dex cost"));
    else
        msg.readUInt8("dex cost");

    val = msg.readUInt8("luk");
    PlayerInfo::setStatBase(LUK, val);
    if (statusWindow)
        statusWindow->setPointsNeeded(LUK, msg.readUInt8("luk cost"));
    else
        msg.readUInt8("luk cost");

    PlayerInfo::setStatBase(ATK, msg.readInt16("left atk"), false);
    PlayerInfo::setStatMod(ATK, msg.readInt16("right atk"));
    PlayerInfo::updateAttrs();

    val = msg.readInt16("right matk");
    PlayerInfo::setStatBase(MATK, val, false);

    val = msg.readInt16("left matk");
    PlayerInfo::setStatMod(MATK, val);

    PlayerInfo::setStatBase(DEF, msg.readInt16("left def"), false);
    PlayerInfo::setStatMod(DEF, msg.readInt16("right def"));

    PlayerInfo::setStatBase(MDEF, msg.readInt16("left mdef"), false);
    PlayerInfo::setStatMod(MDEF, msg.readInt16("right mdef"));

    PlayerInfo::setStatBase(HIT, msg.readInt16("hit"));

    PlayerInfo::setStatBase(FLEE, msg.readInt16("flee"), false);
    PlayerInfo::setStatMod(FLEE, msg.readInt16("flee2/10"));

    PlayerInfo::setStatBase(CRIT, msg.readInt16("crit/10"));

    const int speed = msg.readInt16("speed");
    localPlayer->setWalkSpeed(Vector(static_cast<float>(speed),
        static_cast<float>(speed), 0));
    PlayerInfo::setStatBase(Attributes::WALK_SPEED, speed);
    PlayerInfo::setStatMod(Attributes::WALK_SPEED,
        msg.readInt16("plus speed = 0"));

    BLOCK_END("PlayerHandler::processPlayerStatUpdate5")
}

}  // namespace EAthena
