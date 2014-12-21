/*
 *  The ManaPlus Client
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

#include "net/eathena/markethandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::MarketHandler *marketHandler;

namespace EAthena
{

MarketHandler::MarketHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_NPC_MARKET_OPEN,
        SMSG_NPC_MARKET_BUY_ACK,
        0
    };
    handledMessages = _messages;
    marketHandler = this;
}

void MarketHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_NPC_MARKET_OPEN:
            processMarketOpen(msg);
            break;

        case SMSG_NPC_MARKET_BUY_ACK:
            processMarketBuyAck(msg);
            break;

        default:
            break;
    }
}

void MarketHandler::processMarketOpen(Net::MessageIn &msg)
{
    const int len = (msg.readInt16("len") - 4) / 13;
    for (int f = 0; f < len; f ++)
    {
        msg.readInt16("item id");
        msg.readUInt8("type");
        msg.readInt32("price");
        msg.readInt32("amount");
        msg.readInt16("view");
    }
}

void MarketHandler::processMarketBuyAck(Net::MessageIn &msg)
{
    const int len = (msg.readInt16("len") - 4) / 8;
    for (int f = 0; f < len; f ++)
    {
        msg.readInt16("item id");
        msg.readInt16("amount");
        msg.readInt32("price");
    }
}

void MarketHandler::close()
{
    createOutPacket(CMSG_NPC_MARKET_CLOSE);
}

void MarketHandler::buyItem(const int itemId,
                            const unsigned char color A_UNUSED,
                            const int amount) const
{
    createOutPacket(CMSG_NPC_MARKET_BUY);
    outMsg.writeInt16(8, "len");
    outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
}

}  // namespace EAthena
