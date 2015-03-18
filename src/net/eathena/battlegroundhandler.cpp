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
            processBattleEmblem(msg);
            break;

        case SMSG_BATTLE_UPDATE_SCORE:
            processBattleUpdateScore(msg);
            break;

        case SMSG_BATTLE_UPDATE_COORDS:
            processBattleUpdateCoords(msg);
            break;

        case SMSG_BATTLE_PLAY:
            processBattlePlay(msg);
            break;

        case SMSG_BATTLE_QUEUE_ACK:
            processBattleQueueAck(msg);
            break;

        default:
            break;
    }
}

void BattleGroundHandler::processBattleEmblem(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("account id");
    msg.readString(24, "name");
    msg.readInt16("camp");
}

void BattleGroundHandler::processBattleUpdateScore(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("camp a points");
    msg.readInt16("camp b points");
}

void BattleGroundHandler::processBattleUpdateCoords(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("account id");
    msg.readString(24, "name");
    msg.readInt16("class");
    msg.readInt16("x");
    msg.readInt16("y");
}

void BattleGroundHandler::processBattlePlay(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readString(24, "battle ground name");
}

void BattleGroundHandler::processBattleQueueAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readUInt8("type");
    msg.readString(24, "bg name");
}

}  // namespace EAthena
