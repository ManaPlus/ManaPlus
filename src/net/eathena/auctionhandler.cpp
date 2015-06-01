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

#include "net/eathena/auctionhandler.h"

#include "item.h"
#include "logger.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::AuctionHandler *auctionHandler;

namespace EAthena
{

AuctionHandler::AuctionHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_AUCTION_OPEN_WINDOW,
        SMSG_AUCTION_RESULTS,
        SMSG_AUCTION_SET_ITEM,
        SMSG_AUCTION_MESSAGE,
        SMSG_AUCTION_CLOSE,
        0
    };
    handledMessages = _messages;
    auctionHandler = this;
}

void AuctionHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_AUCTION_OPEN_WINDOW:
            processOpenWindow(msg);
            break;

        case SMSG_AUCTION_RESULTS:
            processAuctionResults(msg);
            break;

        case SMSG_AUCTION_SET_ITEM:
            processAuctionSetItem(msg);
            break;

        case SMSG_AUCTION_MESSAGE:
            processAuctionMessage(msg);
            break;

        case SMSG_AUCTION_CLOSE:
            processAuctionClose(msg);
            break;

        default:
            break;
    }
}

void AuctionHandler::processOpenWindow(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("flag");  // 0 - open, 1 - close
}

void AuctionHandler::processAuctionResults(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("len");
    msg.readInt32("pages");
    const int itemCount = msg.readInt32("items count");
    for (int f = 0; f < itemCount; f ++)
    {
        msg.readInt32("auction id");
        msg.readString(24, "seller name");
        msg.readInt32("item id");
        msg.readInt32("auction type");
        msg.readInt16("item amount");  // always 1
        msg.readUInt8("identify");
        msg.readUInt8("attribute");
        msg.readUInt8("refine");
        for (int d = 0; d < 4; d ++)
            msg.readInt16("card");
        msg.readInt32("price");
        msg.readInt32("buy now");
        msg.readString(24, "buyer name");
        msg.readInt32("timestamp");
    }
}

void AuctionHandler::processAuctionSetItem(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("index");
    msg.readUInt8("flag");
}

void AuctionHandler::processAuctionMessage(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readUInt8("message");
}

void AuctionHandler::processAuctionClose(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("flag");
}

void AuctionHandler::cancelReg() const
{
    createOutPacket(CMSG_AUCTION_CANCEL_REG);
    outMsg.writeInt16(0, "type");  // unused
}

void AuctionHandler::setItem(const Item *const item,
                             const int amount) const
{
    createOutPacket(CMSG_AUCTION_SET_ITEM);
    outMsg.writeInt16(static_cast<int16_t>(
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
    outMsg.writeInt16(static_cast<int16_t>(type), "search type");
    outMsg.writeInt32(auctionId, "auction id");
    outMsg.writeString(text, 24, "search text");
    outMsg.writeInt16(static_cast<int16_t>(page), "page");
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
