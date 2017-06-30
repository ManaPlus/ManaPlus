/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/packetlimiter.h"

#include "configuration.h"
#include "settings.h"

#include "utils/cast.h"
#include "utils/checkutils.h"
#include "utils/timer.h"

#include <sys/stat.h>

#include "debug.h"

struct PacketLimit final
{
    A_DEFAULT_COPY(PacketLimit)

    int lastTime;
    int timeLimit;
    int cnt;
    int cntLimit;
};

PacketLimit mPacketLimits[CAST_SIZE(PacketType::PACKET_SIZE) + 1];

void PacketLimiter::initPacketLimiter()
{
    // here i setting packet limits. but current server is broken,
    // and this limits may not help.

    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_CHAT)].timeLimit = 10 + 5;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_CHAT)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_CHAT)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_CHAT)].cnt = 0;

    // 10
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_PICKUP)].timeLimit = 10 + 5;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_PICKUP)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_PICKUP)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_PICKUP)].cnt = 0;

    // 10 5
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_DROP)].timeLimit = 5;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_DROP)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_DROP)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_DROP)].cnt = 0;

    // 100
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_NEXT)].timeLimit = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_NEXT)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_NEXT)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_NEXT)].cnt = 0;

    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_INPUT)].timeLimit = 100;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_INPUT)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_INPUT)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_INPUT)].cnt = 0;

    // 50
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_TALK)].timeLimit = 60;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_TALK)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_TALK)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_NPC_TALK)].cnt = 0;

    // 10
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_EMOTE)].timeLimit = 10 + 5;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_EMOTE)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_EMOTE)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_EMOTE)].cnt = 0;

    // 100
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_SIT)].timeLimit = 100;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_SIT)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_SIT)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_SIT)].cnt = 0;

    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_DIRECTION)].timeLimit = 50;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_DIRECTION)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_DIRECTION)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_DIRECTION)].cnt = 0;

    // 2+
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_ATTACK)].timeLimit = 2 + 10;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_ATTACK)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_ATTACK)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_ATTACK)].cnt = 0;

    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_STOPATTACK)].timeLimit = 2 + 10;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_STOPATTACK)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_STOPATTACK)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_STOPATTACK)].cnt = 0;

    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_ONLINELIST)].timeLimit = 1800;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_ONLINELIST)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_ONLINELIST)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_ONLINELIST)].cnt = 0;

    // 300ms + 50 fix
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_WHISPER)].timeLimit = 30 + 5;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_WHISPER)].lastTime = 0;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_WHISPER)].cntLimit = 1;
    mPacketLimits[CAST_SIZE(
        PacketType::PACKET_WHISPER)].cnt = 0;

    if (!settings.serverConfigDir.empty())
    {
        const std::string packetLimitsName = settings.serverConfigDir
            + "/packetlimiter.txt";

        std::ifstream inPacketFile;
        struct stat statbuf;

        if ((stat(packetLimitsName.c_str(), &statbuf) != 0)
            || !S_ISREG(statbuf.st_mode))
        {
            // wtiting new file
            writePacketLimits(packetLimitsName);
        }
        else
        {   // reading existent file
            inPacketFile.open(packetLimitsName.c_str(), std::ios::in);
            char line[101];

            if (!inPacketFile.is_open() || !inPacketFile.getline(line, 100))
            {
                inPacketFile.close();
                return;
            }

            const int ver = atoi(line);

            for (int f = 0;
                 f < CAST_S32(PacketType::PACKET_SIZE);
                 f ++)
            {
                if (!inPacketFile.getline(line, 100))
                    break;

                if (!(ver == 1 &&
                    (static_cast<PacketTypeT>(f) == PacketType::PACKET_DROP ||
                    static_cast<PacketTypeT>(f)
                    == PacketType::PACKET_NPC_NEXT)))
                {
                    mPacketLimits[f].timeLimit = atoi(line);
                }
            }
            inPacketFile.close();
            if (ver < 5)
                writePacketLimits(packetLimitsName);
        }
    }
}

void PacketLimiter::writePacketLimits(const std::string &packetLimitsName)
{
    std::ofstream outPacketFile;
    outPacketFile.open(packetLimitsName.c_str(), std::ios::out);
    if (!outPacketFile.is_open())
    {
        reportAlways("Error opening file for writing: %s",
            packetLimitsName.c_str());
        outPacketFile.close();
        return;
    }
    outPacketFile << "4" << std::endl;
    for (int f = 0; f < CAST_S32(PacketType::PACKET_SIZE); f ++)
    {
        outPacketFile << toString(mPacketLimits[f].timeLimit)
                      << std::endl;
    }

    outPacketFile.close();
}

bool PacketLimiter::checkPackets(const PacketTypeT type)
{
    if (type > PacketType::PACKET_SIZE)
        return false;

    if (!serverConfig.getValueBool("enableBuggyServers", true))
        return true;

    const PacketLimit &limit = mPacketLimits[CAST_SIZE(type)];
    const int timeLimit = limit.timeLimit;

    if (timeLimit == 0)
        return true;

    const int time = tick_time;
    const int lastTime = limit.lastTime;
    const int cnt = limit.cnt;
    const int cntLimit = limit.cntLimit;

    if (lastTime > tick_time)
    {
//        instance()->mPacketLimits[type].lastTime = time;
//        instance()->mPacketLimits[type].cnt = 0;

        return true;
    }
    else if (lastTime + timeLimit > time)
    {
        if (cnt >= cntLimit)
        {
            return false;
        }
        else
        {
//            instance()->mPacketLimits[type].cnt ++;
            return true;
        }
    }
//    instance()->mPacketLimits[type].lastTime = time;
//    instance()->mPacketLimits[type].cnt = 1;
    return true;
}

bool PacketLimiter::limitPackets(const PacketTypeT type)
{
    if (CAST_S32(type) < 0 || type > PacketType::PACKET_SIZE)
        return false;

    if (!serverConfig.getValueBool("enableBuggyServers", true))
        return true;

    PacketLimit &pack = mPacketLimits[CAST_SIZE(type)];
    const int timeLimit = pack.timeLimit;

    if (timeLimit == 0)
        return true;

    const int time = tick_time;
    const int lastTime = pack.lastTime;
    const int cnt = pack.cnt;
    const int cntLimit = pack.cntLimit;

    if (lastTime > tick_time)
    {
        pack.lastTime = time;
        pack.cnt = 0;
        return true;
    }
    else if (lastTime + timeLimit > time)
    {
        if (cnt >= cntLimit)
        {
            return false;
        }
        else
        {
            pack.cnt ++;
            return true;
        }
    }
    pack.lastTime = time;
    pack.cnt = 1;
    return true;
}
