/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef NET_EATHENA_BUYINGSTOREHANDLER_H
#define NET_EATHENA_BUYINGSTOREHANDLER_H

#include "net/buyingstorehandler.h"

namespace EAthena
{
class BuyingStoreHandler final : public Net::BuyingStoreHandler
{
    public:
        BuyingStoreHandler();

        A_DELETE_COPY(BuyingStoreHandler)

        ~BuyingStoreHandler() override final;

        void create(const std::string &name,
                    const int maxMoney,
                    const bool flag,
                    const STD_VECTOR<ShopItem*> &items) const override final;

        void close() const override final;

        void open(const Being *const being) const override final;

        void sell(const Being *const being,
                  const int storeId,
                  const Item *const item,
                  const int amount) const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_BUYINGSTOREHANDLER_H
