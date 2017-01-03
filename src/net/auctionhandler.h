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

#ifndef NET_AUCTIONHANDLER_H
#define NET_AUCTIONHANDLER_H

#include "enums/net/auctionsearchtype.h"

#include <string>

#include "localconsts.h"

class Item;

namespace Net
{

class AuctionHandler notfinal
{
    public:
        virtual ~AuctionHandler()
        { }

        virtual void cancelReg() const = 0;

        virtual void setItem(const Item *const item,
                             const int amount) const = 0;

        virtual void reg(const int currentPrice,
                         const int maxPrice,
                         const int hours) const = 0;

        virtual void cancel(const int auctionId) const = 0;

        virtual void close(const int auctionId) const = 0;

        virtual void bid(const int auctionId,
                         const int money) const = 0;

        virtual void search(const AuctionSearchTypeT type,
                            const int auctionId,
                            const std::string &text,
                            const int page) const = 0;

        virtual void buy() const = 0;

        virtual void sell() const = 0;
};

}  // namespace Net

extern Net::AuctionHandler *auctionHandler;

#endif  // NET_AUCTIONHANDLER_H
