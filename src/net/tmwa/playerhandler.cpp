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

#include "net/tmwa/inventoryhandler.h"
#include "net/tmwa/messageout.h"
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

        case SMSG_MAP_MUSIC:
            processMapMusic(msg);
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

void PlayerHandler::processPlayerStatUpdate5(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processPlayerStatUpdate5")
    PlayerInfo::setAttribute(Attributes::CHAR_POINTS,
        msg.readInt16("char points"));

    unsigned int val = msg.readUInt8("str");
    PlayerInfo::setStatBase(Attributes::STR, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::STR,
            msg.readUInt8("str cost"));
    }
    else
    {
        msg.readUInt8("str cost");
    }

    val = msg.readUInt8("agi");
    PlayerInfo::setStatBase(Attributes::AGI, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::AGI,
            msg.readUInt8("agi cost"));
    }
    else
    {
        msg.readUInt8("agi cost");
    }

    val = msg.readUInt8("vit");
    PlayerInfo::setStatBase(Attributes::VIT, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::VIT,
            msg.readUInt8("vit cost"));
    }
    else
    {
        msg.readUInt8("vit cost");
    }

    val = msg.readUInt8("int");
    PlayerInfo::setStatBase(Attributes::INT, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::INT,
            msg.readUInt8("int cost"));
    }
    else
    {
        msg.readUInt8("int cost");
    }

    val = msg.readUInt8("dex");
    PlayerInfo::setStatBase(Attributes::DEX, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::DEX,
            msg.readUInt8("dex cost"));
    }
    else
    {
        msg.readUInt8("dex cost");
    }

    val = msg.readUInt8("luk");
    PlayerInfo::setStatBase(Attributes::LUK, val);
    if (statusWindow)
    {
        statusWindow->setPointsNeeded(Attributes::LUK,
            msg.readUInt8("luk cost"));
    }
    else
    {
        msg.readUInt8("luk cost");
    }

    PlayerInfo::setStatBase(Attributes::ATK,
        msg.readInt16("atk"), Notify_false);
    PlayerInfo::setStatMod(Attributes::ATK, msg.readInt16("atk+"));
    PlayerInfo::updateAttrs();

    val = msg.readInt16("matk");
    PlayerInfo::setStatBase(Attributes::MATK, val, Notify_false);

    val = msg.readInt16("matk+");
    PlayerInfo::setStatMod(Attributes::MATK, val);

    PlayerInfo::setStatBase(Attributes::DEF,
        msg.readInt16("def"), Notify_false);
    PlayerInfo::setStatMod(Attributes::DEF, msg.readInt16("def+"));

    PlayerInfo::setStatBase(Attributes::MDEF,
        msg.readInt16("mdef"), Notify_false);
    PlayerInfo::setStatMod(Attributes::MDEF, msg.readInt16("mdef+"));

    PlayerInfo::setStatBase(Attributes::HIT, msg.readInt16("hit"));

    PlayerInfo::setStatBase(Attributes::FLEE,
        msg.readInt16("flee"), Notify_false);
    PlayerInfo::setStatMod(Attributes::FLEE, msg.readInt16("flee+"));

    PlayerInfo::setStatBase(Attributes::CRIT, msg.readInt16("crit"));

    PlayerInfo::setStatBase(Attributes::MANNER, msg.readInt16("manner"));
    msg.readInt16("unused?");
    BLOCK_END("PlayerHandler::processPlayerStatUpdate5")
}

void PlayerHandler::processWalkResponse(Net::MessageIn &msg)
{
    BLOCK_START("PlayerHandler::processWalkResponse")
    /*
      * This client assumes that all walk messages succeed,
      * and that the server will send a correction notice
      * otherwise.
      */
    uint16_t srcX, srcY, dstX, dstY;
    msg.readInt32("tick");
    msg.readCoordinatePair(srcX, srcY, dstX, dstY, "move path");
    msg.readUInt8("unused");
    if (localPlayer)
        localPlayer->setRealPos(dstX, dstY);
    BLOCK_END("PlayerHandler::processWalkResponse")
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
