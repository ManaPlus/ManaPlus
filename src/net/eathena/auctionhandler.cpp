/*
 *  The ManaPlus Client
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

#include "net/eathena/auctionhandler.h"

#include "const/net/inventory.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "resources/item/item.h"

#include "debug.h"

extern Net::AuctionHandler *auctionHandler;

namespace EAthena
{

AuctionHandler::AuctionHandler()
{
    auctionHandler = this;
}

void AuctionHandler::cancelReg() const
{
    createOutPacket(CMSG_AUCTION_CANCEL_REG);
    outMsg.writeInt16(0, "type");  // unused
}

void AuctionHandler::setItem(const Item *const item,
                             const int amount) const
{
    if (!item)
        return;
    createOutPacket(CMSG_AUCTION_SET_ITEM);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt32(amount, "amount");  // always 1
}

void AuctionHandler::reg(const int currentPrice,
                         const int maxPrice,
                         const int hours) const
{
    createOutPacket(CMSG_AUCTION_REGISTER);
    outMsg.writeInt32(currentPrice, "now money");
    outMsg.writeInt32(maxPrice, "max money");
    outMsg.writeInt32(hours, "delete hour");
}

void AuctionHandler::cancel(const int auctionId) const
{
    createOutPacket(CMSG_AUCTION_CANCEL);
    outMsg.writeInt32(auctionId, "auction id");
}

void AuctionHandler::close(const int auctionId) const
{
    createOutPacket(CMSG_AUCTION_CLOSE);
    outMsg.writeInt32(auctionId, "auction id");
}

void AuctionHandler::bid(const int auctionId,
                         const int money) const
{
    createOutPacket(CMSG_AUCTION_BID);
    outMsg.writeInt32(auctionId, "auction id");
    outMsg.writeInt32(money, "money");
}

void AuctionHandler::search(const AuctionSearchTypeT type,
                            const int auctionId,
                            const std::string &text,
                            const int page) const
{
    createOutPacket(CMSG_AUCTION_SEARCH);
    outMsg.writeInt16(CAST_S16(type), "search type");
    outMsg.writeInt32(auctionId, "auction id");
    outMsg.writeString(text, 24, "search text");
    outMsg.writeInt16(CAST_S16(page), "page");
}

void AuctionHandler::buy() const
{
    createOutPacket(CMSG_AUCTION_BUY_SELL);
    outMsg.writeInt16(1, "buy/sell type");
}

void AuctionHandler::sell() const
{
    createOutPacket(CMSG_AUCTION_BUY_SELL);
    outMsg.writeInt16(0, "buy/sell type");
}

}  // namespace EAthena
