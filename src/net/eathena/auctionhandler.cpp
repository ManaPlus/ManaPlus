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

        default:
            break;
    }
}

void AuctionHandler::processOpenWindow(Net::MessageIn &msg)
{
    msg.readInt32("flag");  // 0 - open, 1 - close
}

void AuctionHandler::processAuctionResults(Net::MessageIn &msg)
{
    const int count = msg.readInt16("len");
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
        for (int f = 0; f < 4; f++)
            msg.readInt16("card");
        msg.readInt32("price");
        msg.readInt32("buy now");
        msg.readString(24, "buyer name");
        msg.readInt32("timestamp");
    }
}

void AuctionHandler::processAuctionSetItem(Net::MessageIn &msg)
{
    msg.readInt16("index");
    msg.readUInt8("flag");
}

}  // namespace EAthena
