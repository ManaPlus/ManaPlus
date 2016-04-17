/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#ifndef NET_EATHENA_AUCTIONHANDLER_H
#define NET_EATHENA_AUCTIONHANDLER_H

#ifdef EATHENA_SUPPORT

#include "net/auctionhandler.h"

namespace EAthena
{
class AuctionHandler final : public Net::AuctionHandler
{
    public:
        AuctionHandler();

        A_DELETE_COPY(AuctionHandler)

        void cancelReg() const final;

        void setItem(const Item *const item,
                     const int amount) const final;

        void reg(const int currentPrice,
                 const int maxPrice,
                 const int hours) const final;

        void cancel(const int auctionId) const final;

        void close(const int auctionId) const final;

        void bid(const int auctionId,
                 const int money) const final;

        void search(const AuctionSearchTypeT type,
                    const int auctionId,
                    const std::string &text,
                    const int page) const final;

        void buy() const final;

        void sell() const final;
};

}  // namespace EAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_EATHENA_AUCTIONHANDLER_H
