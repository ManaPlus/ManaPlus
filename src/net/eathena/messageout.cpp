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

#include "net/eathena/messageout.h"

#include "net/packetcounters.h"

#include "net/eathena/network.h"

#include "logger.h"

#include <SDL_endian.h>

#include <cstring>
#include <string>

#include "debug.h"

namespace EAthena
{

MessageOut::MessageOut(const int16_t id):
    Net::MessageOut(id),
    mNetwork(EAthena::Network::instance())
{
    mNetwork->fixSendBuffer();
    mData = mNetwork->mOutBuffer + mNetwork->mOutSize;

    // +++ can be issue. call to virtual member
    writeInt16(id);
}

void MessageOut::expand(const size_t bytes)
{
    mNetwork->mOutSize += static_cast<unsigned>(bytes);
    PacketCounters::incOutBytes(static_cast<int>(bytes));
}

void MessageOut::writeInt16(const int16_t value)
{
    DEBUGLOG("writeInt16: " + toStringPrint(static_cast<int>(value)));
    expand(2);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int16_t swap = SDL_Swap16(value);
    memcpy(mData + mPos, &swap, sizeof(int16_t));
#else
    memcpy(mData + mPos, &value, sizeof(int16_t));
#endif
    mPos += 2;
    PacketCounters::incOutBytes(2);
}

void MessageOut::writeInt32(const int32_t value)
{
    DEBUGLOG("writeInt32: " + toStringPrint(value));
    expand(4);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int32_t swap = SDL_Swap32(value);
    memcpy(mData + mPos, &swap, sizeof(int32_t));
#else
    memcpy(mData + mPos, &value, sizeof(int32_t));
#endif
    mPos += 4;
    PacketCounters::incOutBytes(4);
}

#define LOBYTE(w)  (static_cast<unsigned char>(w))
#define HIBYTE(w)  (static_cast<unsigned char>(( \
static_cast<uint16_t>(w)) >> 8))

void MessageOut::writeCoordinates(const uint16_t x,
                                  const uint16_t y,
                                  unsigned char direction)
{
    DEBUGLOG(strprintf("writeCoordinates: %u,%u %u", x, y, direction));
    char *const data = mData + mPos;
    mNetwork->mOutSize += 3;
    mPos += 3;

    uint16_t temp = x;
    temp <<= 6;
    data[0] = 0;
    data[1] = 1;
    data[2] = 2;
    data[0] = HIBYTE(temp);
    data[1] = static_cast<unsigned char>(temp);
    temp = y;
    temp <<= 4;
    data[1] |= HIBYTE(temp);
    data[2] = LOBYTE(temp);

    // Translate direction to eAthena format
    switch (direction)
    {
        case 1:
            direction = 0;
            break;
        case 3:
            direction = 1;
            break;
        case 2:
            direction = 2;
            break;
        case 6:
            direction = 3;
            break;
        case 4:
            direction = 4;
            break;
        case 12:
            direction = 5;
            break;
        case 8:
            direction = 6;
            break;
        case 9:
            direction = 7;
            break;
        default:
            // OOPSIE! Impossible or unknown
            direction = static_cast<unsigned char>(-1);
            break;
    }
    data[2] |= direction;
    PacketCounters::incOutBytes(3);
}

}  // namespace EAthena
