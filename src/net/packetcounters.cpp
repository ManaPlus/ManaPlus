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

#include "net/packetcounters.h"

#include "debug.h"

extern volatile int cur_time;
extern volatile bool runCounters;

int PacketCounters::mInCurrentSec = 0;
int PacketCounters::mInBytes = 0;
int PacketCounters::mInBytesCalc = 0;
int PacketCounters::mInPackets = 0;
int PacketCounters::mInPacketsCalc = 0;
int PacketCounters::mOutCurrentSec = 0;
int PacketCounters::mOutBytes = 0;
int PacketCounters::mOutBytesCalc = 0;
int PacketCounters::mOutPackets = 0;
int PacketCounters::mOutPacketsCalc = 0;

void PacketCounters::incInBytes(const int cnt)
{
    if (!runCounters)
        return;

    updateCounter(PacketCounters::mInCurrentSec, PacketCounters::mInBytesCalc,
                  PacketCounters::mInBytes);

    PacketCounters::mInBytes += cnt;
}

void PacketCounters::incInPackets()
{
    if (!runCounters)
        return;

    updateCounter(PacketCounters::mInCurrentSec,
                  PacketCounters::mInPacketsCalc, PacketCounters::mInPackets);

    PacketCounters::mInPackets ++;
}

int PacketCounters::getInBytes()
{
    return PacketCounters::mInBytesCalc;
}

int PacketCounters::getInPackets()
{
    return PacketCounters::mInPacketsCalc;
}

void PacketCounters::incOutBytes(const int cnt)
{
    if (!runCounters)
        return;

    updateCounter(PacketCounters::mOutCurrentSec,
                  PacketCounters::mOutBytesCalc, PacketCounters::mOutBytes);

    PacketCounters::mOutBytes += cnt;
}

void PacketCounters::incOutPackets()
{
    if (!runCounters)
        return;

    updateCounter(PacketCounters::mOutCurrentSec,
                  PacketCounters::mOutPacketsCalc,
                  PacketCounters::mOutPackets);

    PacketCounters::mOutPackets ++;
}

int PacketCounters::getOutBytes()
{
    return PacketCounters::mOutBytesCalc;
}

int PacketCounters::getOutPackets()
{
    return PacketCounters::mOutPacketsCalc;
}


void PacketCounters::updateCounter(int &restrict currentSec,
                                   int &restrict calc,
                                   int &restrict counter)
{
    const int idx = cur_time % 60;
    if (currentSec != idx)
    {
        currentSec = idx;
        calc = counter;
        counter = 0;
    }
}

void PacketCounters::update()
{
    if (!runCounters)
        return;

    BLOCK_START("PacketCounters::update")
    updateCounter(PacketCounters::mInCurrentSec, PacketCounters::mInBytesCalc,
        PacketCounters::mInBytes);
    updateCounter(PacketCounters::mInCurrentSec,
        PacketCounters::mInPacketsCalc, PacketCounters::mInPackets);
    updateCounter(PacketCounters::mOutCurrentSec,
        PacketCounters::mOutBytesCalc, PacketCounters::mOutBytes);
    updateCounter(PacketCounters::mOutCurrentSec,
        PacketCounters::mOutPacketsCalc, PacketCounters::mOutPackets);
    BLOCK_END("PacketCounters::update")
}
