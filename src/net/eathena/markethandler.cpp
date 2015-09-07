/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/eathena/markethandler.h"

#include "net/eathena/marketrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern Net::MarketHandler *marketHandler;

namespace EAthena
{

MarketHandler::MarketHandler()
{
    marketHandler = this;
    MarketRecv::mBuyDialog = nullptr;
}

void MarketHandler::close()
{
    createOutPacket(CMSG_NPC_MARKET_CLOSE);
}

void MarketHandler::buyItem(const int itemId,
                            const int type,
                            const ItemColor color A_UNUSED,
                            const int amount) const
{
    const bool nonStack = type == 4 || type == 5 || type == 7 || type == 8;
    int cnt = nonStack ? amount : 1;
    const int amount2 = nonStack ? 1 : amount;
    if (cnt > 100)
        cnt = 100;

    createOutPacket(CMSG_NPC_MARKET_BUY);
    outMsg.writeInt16(static_cast<int16_t>(4 + 6 * cnt), "len");
    for (int f = 0; f < cnt; f ++)
    {
        outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
        outMsg.writeInt32(static_cast<int16_t>(amount2), "amount");
    }
}

}  // namespace EAthena
