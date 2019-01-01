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

#include "net/tmwa/messageout.h"

#include "net/packetcounters.h"

#include "net/tmwa/network.h"

#include "logger.h"

#include "debug.h"

namespace TmwAthena
{

MessageOut::MessageOut(const int16_t id) :
    Net::MessageOut(id),
    mNetwork(TmwAthena::Network::instance())
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

}  // namespace TmwAthena
