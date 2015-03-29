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

#ifndef NET_EATHENA_CASHSHOPHANDLER_H
#define NET_EATHENA_CASHSHOPHANDLER_H

#ifdef EATHENA_SUPPORT

#include "net/cashshophandler.h"

#include "net/eathena/messagehandler.h"

class BuyDialog;

namespace EAthena
{
class CashShopHandler final : public MessageHandler,
                              public Net::CashShopHandler
{
    public:
        CashShopHandler();

        A_DELETE_COPY(CashShopHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void buyItem(const int points,
                     const int itemId,
                     const unsigned char color,
                     const int amount) const override final;

        void close() const override final;

        void requestPoints() const override final;

    protected:
        static void processCashShopOpen(Net::MessageIn &msg);

        static void processCashShopBuyAck(Net::MessageIn &msg);

        static void processCashShopPoints(Net::MessageIn &msg);

        static void processCashShopBuy(Net::MessageIn &msg);

        static void processCashShopTabPriceList(Net::MessageIn &msg);

        static void processCashShopSchedule(Net::MessageIn &msg);

        static BuyDialog *mBuyDialog;
};

}  // namespace EAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_EATHENA_CASHSHOPHANDLER_H
