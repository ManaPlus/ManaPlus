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

#ifndef NET_EATHENA_CASHSHOPHANDLER_H
#define NET_EATHENA_CASHSHOPHANDLER_H

#include "net/cashshophandler.h"

namespace EAthena
{
class CashShopHandler final : public Net::CashShopHandler
{
    public:
        CashShopHandler();

        A_DELETE_COPY(CashShopHandler)

        ~CashShopHandler() override final;

        void buyItem(const int points,
                     const int itemId,
                     const ItemColor color,
                     const int amount) const override final;

        void buyItems(const int points,
                      const STD_VECTOR<ShopItem*> &items) const
                      override final;

        void close() const override final;

        void requestPoints() const override final;

        void requestTab(const int tab) const override final;

        void schedule() const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_CASHSHOPHANDLER_H
