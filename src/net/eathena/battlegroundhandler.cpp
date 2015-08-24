/*
 *  The ManaPlus Client
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

#include "net/eathena/battlegroundhandler.h"

#include "logger.h"

#include "net/eathena/battlegroundrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::BattleGroundHandler *battleGroundHandler;

namespace EAthena
{

BattleGroundHandler::BattleGroundHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_BATTLE_EMBLEM,
        SMSG_BATTLE_UPDATE_SCORE,
        SMSG_BATTLE_UPDATE_COORDS,
        SMSG_BATTLE_PLAY,
        SMSG_BATTLE_QUEUE_ACK,
        SMSG_BATTLE_BEGINS,
        SMSG_BATTLE_NOTICE_DELETE,
        SMSG_BATTLE_JOINED,
        0
    };
    handledMessages = _messages;
    battleGroundHandler = this;
}

void BattleGroundHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_BATTLE_EMBLEM:
            BattleGroundRecv::processBattleEmblem(msg);
            break;

        case SMSG_BATTLE_UPDATE_SCORE:
            BattleGroundRecv::processBattleUpdateScore(msg);
            break;

        case SMSG_BATTLE_UPDATE_COORDS:
            BattleGroundRecv::processBattleUpdateCoords(msg);
            break;

        case SMSG_BATTLE_PLAY:
            BattleGroundRecv::processBattlePlay(msg);
            break;

        case SMSG_BATTLE_QUEUE_ACK:
            BattleGroundRecv::processBattleQueueAck(msg);
            break;

        case SMSG_BATTLE_BEGINS:
            BattleGroundRecv::processBattleBegins(msg);
            break;

        case SMSG_BATTLE_NOTICE_DELETE:
            BattleGroundRecv::processBattleNoticeDelete(msg);
            break;

        case SMSG_BATTLE_JOINED:
            BattleGroundRecv::processBattleJoined(msg);
            break;

        default:
            break;
    }
}

void BattleGroundHandler::registerBg(const BattleGroundTypeT &type,
                                     const std::string &name) const
{
    createOutPacket(CMSG_BATTLE_REGISTER);
    outMsg.writeInt16(static_cast<int16_t>(type), "type");
    outMsg.writeString(name, 24, "bg name");
}

void BattleGroundHandler::rekoveRequest(const std::string &name) const
{
    createOutPacket(CMSG_BATTLE_REVOKE);
    outMsg.writeString(name, 24, "bg name");
}

void BattleGroundHandler::beginAck(const bool result,
                                   const std::string &bgName,
                                   const std::string &gameName) const
{
    createOutPacket(CMSG_BATTLE_BEGIN_ACK);
    outMsg.writeInt8(static_cast<int8_t>(result ? 1 : 0), "result");
    outMsg.writeString(bgName, 24, "bg name");
    outMsg.writeString(gameName, 24, "game name");
}

void BattleGroundHandler::checkState(const std::string &name) const
{
    createOutPacket(CMSG_BATTLE_CHECK_STATE);
    outMsg.writeString(name, 24, "bg name");
}

}  // namespace EAthena
