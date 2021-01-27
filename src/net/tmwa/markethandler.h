/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_TMWA_MARKETHANDLER_H
#define NET_TMWA_MARKETHANDLER_H

#include "net/markethandler.h"

namespace TmwAthena
{
class MarketHandler final : public Net::MarketHandler
{
    public:
        MarketHandler();

        A_DELETE_COPY(MarketHandler)

        ~MarketHandler() override final;

        void close() const override final;

        void buyItem(const int itemId,
                     const ItemTypeT type,
                     const ItemColor color,
                     const int amount) const override final;

        void buyItems(const STD_VECTOR<ShopItem*> &items) const
                      override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_MARKETHANDLER_H
