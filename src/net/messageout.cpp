/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/messageout.h"

#include "logger.h"

#include "net/net.h"
#include "net/packetcounters.h"

#include "utils/stringutils.h"

#include "debug.h"

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
    PacketCounters::incOutBytes(1);
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
    PacketCounters::incOutBytes(length);
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
    PacketCounters::incOutBytes(length);
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
