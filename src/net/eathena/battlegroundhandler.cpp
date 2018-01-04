/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "net/eathena/battlegroundhandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

BattleGroundHandler::BattleGroundHandler()
{
    battleGroundHandler = this;
}

BattleGroundHandler::~BattleGroundHandler()
{
    battleGroundHandler = nullptr;
}

void BattleGroundHandler::registerBg(const BattleGroundTypeT &type,
                                     const std::string &name) const
{
    if (packetVersion < 20111005)
        return;
    createOutPacket(CMSG_BATTLE_REGISTER);
    outMsg.writeInt16(CAST_S16(type), "type");
    outMsg.writeString(name, 24, "bg name");
}

void BattleGroundHandler::rekoveRequest(const std::string &name) const
{
    if (packetVersion < 20111005)
        return;
    createOutPacket(CMSG_BATTLE_REVOKE);
    outMsg.writeString(name, 24, "bg name");
}

void BattleGroundHandler::beginAck(const bool result,
                                   const std::string &bgName,
                                   const std::string &gameName) const
{
    if (packetVersion < 20111005)
        return;
    createOutPacket(CMSG_BATTLE_BEGIN_ACK);
    outMsg.writeInt8(CAST_S8(result ? 1 : 0), "result");
    outMsg.writeString(bgName, 24, "bg name");
    outMsg.writeString(gameName, 24, "game name");
}

void BattleGroundHandler::checkState(const std::string &name) const
{
    if (packetVersion < 20111005)
        return;
    createOutPacket(CMSG_BATTLE_CHECK_STATE);
    outMsg.writeString(name, 24, "bg name");
}

}  // namespace EAthena
