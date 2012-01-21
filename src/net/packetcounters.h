/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef PACKETCOUNTERS_H
#define PACKETCOUNTERS_H

class PacketCounters
{
public:
    static void incInBytes(int cnt);

    static void incInPackets();

    static int getInBytes();

    static int getInPackets();

    static void incOutBytes(int cnt);

    static void incOutPackets();

    static int getOutBytes();

    static int getOutPackets();

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
    static void updateCounter(int &currentSec, int &calc, int &counter);

};

#endif
