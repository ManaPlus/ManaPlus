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

#include "net/messageout.h"

#include "net/packetcounters.h"

#include "logger.h"

#include "utils/stringutils.h"

#include <cstring>
#include <string>

#include "debug.h"

namespace Net
{

MessageOut::MessageOut(const int16_t id A_UNUSED):
    mData(nullptr),
    mDataSize(0),
    mPos(0)
{
    PacketCounters::incOutPackets();
    DEBUGLOG("MessageOut");
}

void MessageOut::writeInt8(const int8_t value)
{
    DEBUGLOG("writeInt8: " + toStringPrint(static_cast<int>(value)));
    expand(1);
    mData[mPos] = value;
    mPos += 1;
    PacketCounters::incOutBytes(1);
}

void MessageOut::writeString(const std::string &string, int length)
{
    int stringLength = static_cast<int>(string.length());
    if (length < 0)
    {
        // Write the length at the start if not fixed
        writeInt16(static_cast<int16_t>(stringLength));
        length = stringLength;
    }
    else if (length < stringLength)
    {
        // Make sure the length of the string is no longer than specified
        stringLength = length;
    }
    expand(length);

    // Write the actual string
    memcpy(mData + mPos, string.c_str(), stringLength);

    // Pad remaining space with zeros
    if (length > stringLength)
        memset(mData + mPos + stringLength, '\0', length - stringLength);

    mPos += length;
    DEBUGLOG("writeString: " + string);
    PacketCounters::incOutBytes(length);
}

void MessageOut::writeStringNoLog(const std::string &string, int length)
{
    int stringLength = static_cast<int>(string.length());
    if (length < 0)
    {
        // Write the length at the start if not fixed
        writeInt16(static_cast<int16_t>(stringLength));
        length = stringLength;
    }
    else if (length < stringLength)
    {
        // Make sure the length of the string is no longer than specified
        stringLength = length;
    }
    expand(length);

    // Write the actual string
    memcpy(mData + mPos, string.c_str(), stringLength);

    // Pad remaining space with zeros
    if (length > stringLength)
        memset(mData + mPos + stringLength, '\0', length - stringLength);

    mPos += length;
    DEBUGLOG("writeString: ***");
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

}  // namespace Net
