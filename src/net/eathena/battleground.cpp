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

#include "net/eathena/battleground.h"

#include "logger.h"

#include "debug.h"

namespace EAthena
{

void BattleGround::processBattleEmblem(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readString(24, "name");
    msg.readInt16("camp");
}

void BattleGround::processBattleUpdateScore(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("camp a points");
    msg.readInt16("camp b points");
}

void BattleGround::processBattleUpdateCoords(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readString(24, "name");
    msg.readInt16("class");
    msg.readInt16("x");
    msg.readInt16("y");
}

void BattleGround::processBattlePlay(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readString(24, "battle ground name");
}

void BattleGround::processBattleQueueAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readUInt8("type");
    msg.readString(24, "bg name");
}

void BattleGround::processBattleBegins(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readString(24, "bg name");
    msg.readString(24, "game name");
}

void BattleGround::processBattleNoticeDelete(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readUInt8("type");
    msg.readString(24, "bg name");
}

void BattleGround::processBattleJoined(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readString(24, "name");
    msg.readInt32("position");
}

}  // namespace EAthena
