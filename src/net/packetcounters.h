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

#ifndef NET_PACKETCOUNTERS_H
#define NET_PACKETCOUNTERS_H

#include "localconsts.h"

class PacketCounters final
{
public:
    static void incInBytes(const int cnt);

    static void incInPackets();

    static int getInBytes() A_WARN_UNUSED;

    static int getInPackets() A_WARN_UNUSED;

    static void incOutBytes(const int cnt);

    static void incOutPackets();

    static int getOutBytes() A_WARN_UNUSED;

    static int getOutPackets() A_WARN_UNUSED;

    static void update();

    static int mInCurrentSec;
    static int mInBytes;
    static int mInBytesCalc;
    static int mInPackets;
    static int mInPacketsCalc;
    static int mOutCurrentSec;
    static int mOutBytes;
    static int mOutBytesCalc;
    static int mOutPackets;
    static int mOutPacketsCalc;

private:
    static void updateCounter(int &restrict currentSec,
                              int &restrict calc,
                              int &restrict counter);
};

#endif  // NET_PACKETCOUNTERS_H
