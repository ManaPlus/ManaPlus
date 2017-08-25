/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/eathena/messageout.h"

#include "net/packetcounters.h"

#include "net/eathena/network.h"

#include "logger.h"

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

namespace EAthena
{

MessageOut::MessageOut(const int16_t id) :
    Net::MessageOut(id),
    mNetwork(EAthena::Network::instance())
{
    mNetwork->fixSendBuffer();
    mData = mNetwork->mOutBuffer + CAST_SIZE(mNetwork->mOutSize);
}

MessageOut::~MessageOut()
{
    DEBUGLOG2("writeEnd: ", mPos, "position after end of packet");
}

void MessageOut::expand(const size_t bytes) const
{
    mNetwork->mOutSize += CAST_U32(bytes);
    PacketCounters::incOutBytes(CAST_S32(bytes));
}

void MessageOut::writeInt16(const int16_t value, const char *const str)
{
    expand(2);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int16_t swap = SDL_Swap16(value);
    memcpy(mData + CAST_SIZE(mPos), &swap, sizeof(int16_t));
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    memcpy(mData + CAST_SIZE(mPos), &value, sizeof(int16_t));
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    DEBUGLOG2("writeInt16: " + toStringPrint(CAST_U32(
        CAST_U16(value))),
        mPos, str);
    mPos += 2;
    PacketCounters::incOutBytes(2);
}

void MessageOut::writeInt32(const int32_t value, const char *const str)
{
    DEBUGLOG2("writeInt32: " + toStringPrint(CAST_U32(value)),
        mPos, str);
    expand(4);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int32_t swap = SDL_Swap32(value);
    memcpy(mData + CAST_SIZE(mPos), &swap, sizeof(int32_t));
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    memcpy(mData + CAST_SIZE(mPos), &value, sizeof(int32_t));
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    mPos += 4;
    PacketCounters::incOutBytes(4);
}

void MessageOut::writeInt64(const int64_t value, const char *const str)
{
    DEBUGLOG2("writeInt64: " + toStringPrint(CAST_U32(value)),
        mPos, str);
    expand(8);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int32_t swap = SDL_Swap64(value);
    memcpy(mData + CAST_SIZE(mPos), &swap, sizeof(int64_t));
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    memcpy(mData + CAST_SIZE(mPos), &value, sizeof(int64_t));
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

    mPos += 8;
    PacketCounters::incOutBytes(8);
}

void MessageOut::writeBeingId(const BeingId value, const char *const str)
{
    writeInt32(toInt(value, int32_t), str);
}

#define LOBYTE(w)  (CAST_U8(w))
#define HIBYTE(w)  (CAST_U8(( \
CAST_U16(w)) >> 8))

void MessageOut::writeCoordinates(const uint16_t x,
                                  const uint16_t y,
                                  unsigned char direction,
                                  const char *const str)
{
    DEBUGLOG2(strprintf("writeCoordinates: %u,%u %u",
        CAST_U32(x),
        CAST_U32(y),
        CAST_U32(direction)), mPos, str);
    unsigned char *const data = reinterpret_cast<unsigned char*>(mData)
        + CAST_SIZE(mPos);
    mNetwork->mOutSize += 3;
    mPos += 3;

    uint16_t temp = x;
    temp <<= 6;
    data[0] = 0;
    data[1] = 1;
    data[2] = 2;
    data[0] = HIBYTE(temp);
    data[1] = CAST_U8(temp);
    temp = y;
    temp <<= 4;
    data[1] |= HIBYTE(temp);
    data[2] = LOBYTE(temp);
    direction = toServerDirection(direction);
    data[2] |= direction;
    PacketCounters::incOutBytes(3);
}

}  // namespace EAthena
