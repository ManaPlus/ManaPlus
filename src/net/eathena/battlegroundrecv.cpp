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

#include "net/eathena/battlegroundrecv.h"

#include "actormanager.h"
#include "logger.h"

#include "being/being.h"

#include "net/messagein.h"

#include "debug.h"

namespace EAthena
{

void BattleGroundRecv::processBattleEmblem(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readString(24, "name");
    msg.readInt16("bg id");
}

void BattleGroundRecv::processBattleEmblem2(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("account id");
    msg.readString(24, "name");
    msg.readInt16("bg id");
    const int teamId = msg.readInt16("team id");

    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing != nullptr)
        dstBeing->setTeamId(CAST_U16(teamId));
}

void BattleGroundRecv::processBattleUpdateScore(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("camp a points");
    msg.readInt16("camp b points");
}

void BattleGroundRecv::processBattleUpdateCoords(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("account id");
    msg.readString(24, "name");
    msg.readInt16("class");
    msg.readInt16("x");
    msg.readInt16("y");
}

void BattleGroundRecv::processBattlePlay(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readString(24, "battle ground name");
}

void BattleGroundRecv::processBattleQueueAck(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("type");
    msg.readString(24, "bg name");
}

void BattleGroundRecv::processBattleBegins(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readString(24, "bg name");
    msg.readString(24, "game name");
}

void BattleGroundRecv::processBattleNoticeDelete(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("type");
    msg.readString(24, "bg name");
}

void BattleGroundRecv::processBattleJoined(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readString(24, "name");
    msg.readInt32("position");
}

void BattleGroundRecv::processBattleUpdateHp(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("account id");
    if (msg.getVersion() >= 20140613)
    {
        msg.readInt32("hp");
        msg.readInt32("max hp");
    }
    else
    {
        msg.readString(24, "name");
        msg.readInt16("hp");
        msg.readInt16("max hp");
    }
}

}  // namespace EAthena
