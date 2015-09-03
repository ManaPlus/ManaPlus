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

#include "net/eathena/adminhandler.h"

#include "being/being.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::AdminHandler *adminHandler;

namespace EAthena
{

std::string AdminHandler::mStatsName;

AdminHandler::AdminHandler() :
    Ea::AdminHandler()
{
    adminHandler = this;
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

void AdminHandler::kick(const BeingId playerId) const
{
    createOutPacket(CMSG_ADMIN_KICK);
    outMsg.writeBeingId(playerId, "account id");
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
    outMsg.writeBeingId(being->getId(), "account id");
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
    outMsg.writeBeingId(being->getId(), "account id");
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
    outMsg.writeBeingId(being->getId(), "account id");
}

void AdminHandler::requestStats(const std::string &name)
{
    mStatsName = name;
    createOutPacket(CMSG_ADMIN_REQUEST_STATS);
    outMsg.writeString(name, 24, "name");
}

}  // namespace EAthena
