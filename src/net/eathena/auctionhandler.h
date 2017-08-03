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

#ifndef NET_EATHENA_AUCTIONHANDLER_H
#define NET_EATHENA_AUCTIONHANDLER_H

#include "net/auctionhandler.h"

namespace EAthena
{
class AuctionHandler final : public Net::AuctionHandler
{
    public:
        AuctionHandler();

        A_DELETE_COPY(AuctionHandler)

        ~AuctionHandler();

        void cancelReg() const override final;

        void setItem(const Item *const item,
                     const int amount) const override final;

        void reg(const int currentPrice,
                 const int maxPrice,
                 const int hours) const override final;

        void cancel(const int auctionId) const override final;

        void close(const int auctionId) const override final;

        void bid(const int auctionId,
                 const int money) const override final;

        void search(const AuctionSearchTypeT type,
                    const int auctionId,
                    const std::string &text,
                    const int page) const override final;

        void buy() const override final;

        void sell() const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_AUCTIONHANDLER_H
