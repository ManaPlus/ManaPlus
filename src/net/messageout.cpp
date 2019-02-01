/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "net/messageout.h"

#include "logger.h"

#include "net/net.h"
#include "net/packetcounters.h"

#include "utils/stringutils.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_endian.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

extern int itemIdLen;

namespace Net
{

MessageOut::MessageOut(const int16_t id) :
    mData(nullptr),
    mDataSize(0),
    mPos(0),
    mId(id),
    mIgnore(false)
{
    PacketCounters::incOutPackets();
    IGNOREDEBUGLOG;
    DEBUGLOG2("Send packet", 0, "MessageOut");
}

void MessageOut::writeInt8(const int8_t value, const char *const str)
{
    expand(1);
    mData[mPos] = value;
    DEBUGLOG2("writeInt8:  " + toStringPrint(CAST_U32(
        CAST_U8(value))),
        mPos, str);
    mPos += 1;
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
}

void MessageOut::writeItemId(const int32_t value,
                             const char *const str)
{
    if (itemIdLen == 2)
        writeInt16(CAST_S16(value), str);
    else
        writeInt32(value, str);
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
}

void MessageOut::writeBeingId(const BeingId value, const char *const str)
{
    writeInt32(toInt(value, int32_t), str);
}

void MessageOut::writeFloat(const float value, const char *const str)
{
#ifdef ENABLEDEBUGLOG
    std::string text = strprintf("writeFloat: %f", value);
    DEBUGLOG2(text, mPos, str);
#endif
    expand(4);
    memcpy(mData + CAST_SIZE(mPos), &value, sizeof(float));
    mPos += 4;
}

#define LOBYTE(w) (CAST_U8(w))
#define HIBYTE(w) (CAST_U8((CAST_U16(w)) >> 8))

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
    expand(3);
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
}

void MessageOut::writeString(const std::string &string,
                             int length,
                             const char *const str)
{
    int stringLength = CAST_S32(string.length());
    if (length < 0)
    {
        // Write the length at the start if not fixed
        writeInt16(CAST_S16(stringLength), "len");
        length = stringLength;
    }
    else if (length < stringLength)
    {
        // Make sure the length of the string is no longer than specified
        stringLength = length;
    }
    expand(length);

    // Write the actual string
    memcpy(mData + CAST_SIZE(mPos), string.c_str(), stringLength);

    // Pad remaining space with zeros
    if (length > stringLength)
    {
        memset(mData + CAST_SIZE(mPos + stringLength),
            '\0',
            length - stringLength);
    }

    DEBUGLOG2("writeString: " + string, mPos, str);
    mPos += length;
}

void MessageOut::writeStringNoLog(const std::string &string,
                                  int length,
                                  const char *const str)
{
    int stringLength = CAST_S32(string.length());
    if (length < 0)
    {
        // Write the length at the start if not fixed
        writeInt16(CAST_S16(stringLength), "len");
        length = stringLength;
    }
    else if (length < stringLength)
    {
        // Make sure the length of the string is no longer than specified
        stringLength = length;
    }
    expand(length);

    // Write the actual string
    memcpy(mData + CAST_SIZE(mPos), string.c_str(), stringLength);

    // Pad remaining space with zeros
    if (length > stringLength)
    {
        memset(mData + CAST_SIZE(mPos + stringLength),
            '\0',
            length - stringLength);
    }

    DEBUGLOG2("writeString: ***", mPos, str);
    mPos += length;
}

const char *MessageOut::getData() const
{
    return mData;
}

unsigned int MessageOut::getDataSize() const
{
    return mDataSize;
}

unsigned char MessageOut::toServerDirection(unsigned char direction)
{
    // Translate direction to eAthena format
    switch (direction)
    {
        case 1:  // DOWN
            direction = 0;
            break;
        case 3:  // DOWN | LEFT
            direction = 1;
            break;
        case 2:  // LEFT
            direction = 2;
            break;
        case 6:  // LEFT | UP
            direction = 3;
            break;
        case 4:  // UP
            direction = 4;
            break;
        case 12:  // UP | RIGHT
            direction = 5;
            break;
        case 8:  // RIGHT
            direction = 6;
            break;
        case 9:  // RIGHT + DOWN
            direction = 7;
            break;
        default:
            // OOPSIE! Impossible or unknown
            direction = CAST_U8(-1);
            break;
    }
    return direction;
}

}  // namespace Net
