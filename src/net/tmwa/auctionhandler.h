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

#ifndef NET_TMWA_AUCTIONHANDLER_H
#define NET_TMWA_AUCTIONHANDLER_H

#ifdef EATHENA_SUPPORT

#include "net/auctionhandler.h"

namespace TmwAthena
{
class AuctionHandler final : public Net::AuctionHandler
{
    public:
        AuctionHandler();

        A_DELETE_COPY(AuctionHandler)

        void cancelReg() const final A_CONST;

        void setItem(const Item *const item,
                     const int amount) const final A_CONST;

        void reg(const int currentPrice,
                 const int maxPrice,
                 const int hours) const final A_CONST;

        void cancel(const int auctionId) const final A_CONST;

        void close(const int auctionId) const final A_CONST;

        void bid(const int auctionId,
                 const int money) const final A_CONST;

        void search(const AuctionSearchTypeT type,
                    const int auctionId,
                    const std::string &text,
                    const int page) const final A_CONST;

        void buy() const final A_CONST;

        void sell() const final A_CONST;
};

}  // namespace TmwAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_TMWA_AUCTIONHANDLER_H
