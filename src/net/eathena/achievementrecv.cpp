/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/eathena/achievementrecv.h"

#include "logger.h"

#include "net/messagein.h"

#include "debug.h"

namespace EAthena
{

void AchievementRecv::processAchievementList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int count = (msg.readInt16("len") - 22) / 50;
    msg.readInt32("total achievements");
    msg.readInt32("total points");
    msg.readInt16("rank level");
    msg.readInt32("rank points");
    msg.readInt32("rank points need");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("ach id");
        msg.readUInt8("completed");
        for (int d = 0; d < 10; d ++)
            msg.readInt32("objective");
        msg.readInt32("completed at");
        msg.readUInt8("reward");
    }
}

void AchievementRecv::processAchievementUpdate(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("total points");
    msg.readInt16("rank level");
    msg.readInt32("rank points");
    msg.readInt32("rank points need");

    msg.readInt32("ach id");
    msg.readUInt8("completed");
    for (int d = 0; d < 10; d ++)
        msg.readInt32("objective");
    msg.readInt32("completed at");
    msg.readUInt8("reward");
}

void AchievementRecv::processAchievementGetReward(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("received");
    msg.readInt32("ach id");
}

}  // namespace EAthena
