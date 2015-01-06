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

#include "net/tmwa/auctionhandler.h"

#include "debug.h"

extern Net::AuctionHandler *auctionHandler;

namespace TmwAthena
{

AuctionHandler::AuctionHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        0
    };
    handledMessages = _messages;
    auctionHandler = this;
}

void AuctionHandler::handleMessage(Net::MessageIn &msg A_UNUSED)
{
}

void AuctionHandler::cancelReg() const
{
}

void AuctionHandler::setItem(const Item *const item A_UNUSED,
                             const int amount A_UNUSED) const
{
}

void AuctionHandler::reg(const int currentPrice A_UNUSED,
                         const int maxPrice A_UNUSED,
                         const int hours A_UNUSED) const
{
}

}  // namespace TmwAthena
