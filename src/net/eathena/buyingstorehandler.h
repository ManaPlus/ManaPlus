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

#ifndef NET_EATHENA_BUYINGSTOREHANDLER_H
#define NET_EATHENA_BUYINGSTOREHANDLER_H

#include "net/buyingstorehandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{
class BuyingStoreHandler final : public MessageHandler,
                                 public Net::BuyingStoreHandler
{
    public:
        BuyingStoreHandler();

        A_DELETE_COPY(BuyingStoreHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void create(const std::string &name,
                    const int maxMoney,
                    const bool flag,
                    std::vector<ShopItem*> &items) const override final;

        void close() const override final;

        void open(const Being *const being) const override final;

        void sell(const Being *const being,
                  const int storeId,
                  const Item *const item,
                  const int amount) const override final;

    protected:
        static void processBuyingStoreOpen(Net::MessageIn &msg);

        static void processBuyingStoreCreateFailed(Net::MessageIn &msg);

        static void processBuyingStoreOwnItems(Net::MessageIn &msg);

        static void processBuyingStoreShowBoard(Net::MessageIn &msg);

        static void processBuyingStoreHideBoard(Net::MessageIn &msg);

        static void processBuyingStoreItemsList(Net::MessageIn &msg);

        static void processBuyingStoreSellFailed(Net::MessageIn &msg);

        static void processBuyingStoreReport(Net::MessageIn &msg);

        static void processBuyingStoreDeleteItem(Net::MessageIn &msg);

        static void processBuyingStoreSellerSellFailed(Net::MessageIn &msg);
};

}  // namespace EAthena

#endif  // NET_EATHENA_BUYINGSTOREHANDLER_H
