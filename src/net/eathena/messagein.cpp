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

#include "net/eathena/messagein.h"

#include "net/packetcounters.h"

#include "logger.h"

#include "utils/stringutils.h"

#include <SDL_endian.h>

#include "debug.h"

namespace EAthena
{

MessageIn::MessageIn(const char *const data, const unsigned int length) :
    Net::MessageIn(data, length)
{
}

void MessageIn::postInit()
{
    // Read the message ID
    mId = readInt16();
}

int16_t MessageIn::readInt16()
{
    int16_t value = -1;
    if (mPos + 2 <= mLength)
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        int16_t swap;
        memcpy(&swap, mData + mPos, sizeof(int16_t));
        value = SDL_Swap16(swap);
#else
        memcpy(&value, mData + mPos, sizeof(int16_t));
#endif
    }
    mPos += 2;
    PacketCounters::incInBytes(2);
    DEBUGLOG("readInt16: " + toStringPrint(static_cast<int>(value)));
    return value;
}

int32_t MessageIn::readInt32()
{
    int32_t value = -1;
    if (mPos + 4 <= mLength)
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        int32_t swap;
        memcpy(&swap, mData + mPos, sizeof(int32_t));
        value = SDL_Swap32(swap);
#else
        memcpy(&value, mData + mPos, sizeof(int32_t));
#endif
    }
    mPos += 4;
    PacketCounters::incInBytes(4);
    DEBUGLOG("readInt32: " + toStringPrint(value));
    return value;
}

}  // namespace EAthena
