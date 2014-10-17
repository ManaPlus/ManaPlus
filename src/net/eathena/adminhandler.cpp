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

#include "net/eathena/adminhandler.h"

#include "notifymanager.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include <string>

#include "debug.h"

extern Net::AdminHandler *adminHandler;

namespace EAthena
{

std::string AdminHandler::mStatsName;

AdminHandler::AdminHandler() :
    MessageHandler(),
    Ea::AdminHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_ADMIN_KICK_ACK,
        SMSG_ADMIN_GET_LOGIN_ACK,
        SMSG_ADMIN_SET_TILE_TYPE,
        SMSG_ADMIN_ACCOUNT_STATS,
        0
    };
    handledMessages = _messages;
    adminHandler = this;
}

void AdminHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_ADMIN_KICK_ACK:
            if (msg.readInt32() == 0)
                NotifyManager::notify(NotifyTypes::KICK_FAIL);
            else
                NotifyManager::notify(NotifyTypes::KICK_SUCCEED);
            break;
        case SMSG_ADMIN_GET_LOGIN_ACK:
            processAdminGetLoginAck(msg);
            break;
        case SMSG_ADMIN_SET_TILE_TYPE:
            processSetTileType(msg);
            break;
        case SMSG_ADMIN_ACCOUNT_STATS:
            processAccountStats(msg);
            break;
        default:
            break;
    }
}

void AdminHandler::announce(const std::string &text) const
{
    createOutPacket(CMSG_ADMIN_ANNOUNCE);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 4), "len");
    outMsg.writeString(text, static_cast<int>(text.length()), "message");
}

void AdminHandler::localAnnounce(const std::string &text) const
{
    createOutPacket(CMSG_ADMIN_LOCAL_ANNOUNCE);
    outMsg.writeInt16(static_cast<int16_t>(text.length() + 4), "len");
    outMsg.writeString(text, static_cast<int>(text.length()), "message");
}

void AdminHandler::hide(const bool h A_UNUSED) const
{
    createOutPacket(CMSG_ADMIN_HIDE);
    outMsg.writeInt32(0, "unused");
}

void AdminHandler::kick(const int playerId) const
{
    createOutPacket(CMSG_ADMIN_KICK);
    outMsg.writeInt32(playerId, "account id");
}

void AdminHandler::kickAll() const
{
    createOutPacket(CMSG_ADMIN_KICK_ALL);
}

void AdminHandler::warp(const std::string &map, const int x, const int y) const
{
    createOutPacket(CMSG_PLAYER_MAPMOVE);
    outMsg.writeString(map, 16, "map");
    outMsg.writeInt16(static_cast<int16_t>(x), "x");
    outMsg.writeInt16(static_cast<int16_t>(y), "y");
}

void AdminHandler::resetStats() const
{
    createOutPacket(CMSG_ADMIN_RESET_PLAYER);
    outMsg.writeInt16(0, "flag");
}

void AdminHandler::resetSkills() const
{
    createOutPacket(CMSG_ADMIN_RESET_PLAYER);
    outMsg.writeInt16(1, "flag");
}

void AdminHandler::gotoName(const std::string &name) const
{
    createOutPacket(CMSG_ADMIN_GOTO);
    outMsg.writeString(name, 24, "name");
}

void AdminHandler::recallName(const std::string &name) const
{
    createOutPacket(CMSG_ADMIN_RECALL);
    outMsg.writeString(name, 24, "name");
}

void AdminHandler::mute(const Being *const being,
                        const int type,
                        const int limit) const
{
    if (!being)
        return;

    createOutPacket(CMSG_ADMIN_MUTE);
    outMsg.writeInt32(being->getId(), "account id");
    outMsg.writeInt8(static_cast<int8_t>(type), "type");
    outMsg.writeInt16(static_cast<int16_t>(limit), "value");
}

void AdminHandler::muteName(const std::string &name) const
{
    createOutPacket(CMSG_ADMIN_MUTE_NAME);
    outMsg.writeString(name, 24, "name");
}

void AdminHandler::requestLogin(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_ADMIN_ID_TO_LOGIN);
    outMsg.writeInt32(being->getId(), "account id");
}

void AdminHandler::setTileType(const int x, const int y,
                               const int type) const
{
    createOutPacket(CMSG_ADMIN_SET_TILE_TYPE);
    outMsg.writeInt16(static_cast<int16_t>(x), "x");
    outMsg.writeInt16(static_cast<int16_t>(y), "y");
    outMsg.writeInt16(static_cast<int16_t>(type), "type");
}

void AdminHandler::unequipAll(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_ADMIN_UNEQUIP_ALL);
    outMsg.writeInt32(being->getId(), "account id");
}

void AdminHandler::processAdminGetLoginAck(Net::MessageIn &msg)
{
    msg.readInt32("account id");
    msg.readString(24, "login");
}

void AdminHandler::processSetTileType(Net::MessageIn &msg)
{
    // +++ here need set collision tile for map
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readInt16("type");
    msg.readString(16, "map name");
}

void AdminHandler::requestStats(const std::string &name)
{
    mStatsName = name;
    createOutPacket(CMSG_ADMIN_REQUEST_STATS);
    outMsg.writeString(name, 24);
}

void AdminHandler::processAccountStats(Net::MessageIn &msg)
{
    // +++ need show in other players stats window, nick in mStatsName
    msg.readUInt8("str");
    msg.readUInt8("need str");
    msg.readUInt8("agi");
    msg.readUInt8("need agi");
    msg.readUInt8("vit");
    msg.readUInt8("need vit");
    msg.readUInt8("int");
    msg.readUInt8("need int");
    msg.readUInt8("dex");
    msg.readUInt8("need dex");
    msg.readUInt8("luk");
    msg.readUInt8("need luk");
    msg.readInt16("attack");
    msg.readInt16("refine");
    msg.readInt16("matk max");
    msg.readInt16("matk min");
    msg.readInt16("item def");
    msg.readInt16("plus def");
    msg.readInt16("mdef");
    msg.readInt16("plus mdef");
    msg.readInt16("hit");
    msg.readInt16("flee");
    msg.readInt16("flee2/10");
    msg.readInt16("cri/10");
    msg.readInt16("speed");
    msg.readInt16("zero");
}

}  // namespace EAthena
