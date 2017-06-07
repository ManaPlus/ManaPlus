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

#include "net/tmwa/auctionhandler.h"

#include "debug.h"



namespace TmwAthena
{

AuctionHandler::AuctionHandler()
{
    auctionHandler = this;
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

void AuctionHandler::cancel(const int auctionId A_UNUSED) const
{
}

void AuctionHandler::close(const int auctionId A_UNUSED) const
{
}

void AuctionHandler::bid(const int auctionId A_UNUSED,
                         const int money A_UNUSED) const
{
}

void AuctionHandler::search(const AuctionSearchTypeT type A_UNUSED,
                            const int auctionId A_UNUSED,
                            const std::string &text A_UNUSED,
                            const int page A_UNUSED) const
{
}

void AuctionHandler::buy() const
{
}

void AuctionHandler::sell() const
{
}

}  // namespace TmwAthena
