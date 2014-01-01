/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef NET_EA_BUYSELLHANDLER_H
#define NET_EA_BUYSELLHANDLER_H

#include "net/buysellhandler.h"

class BuyDialog;

namespace Ea
{

class BuySellHandler : public Net::BuySellHandler
{
    public:
        BuySellHandler();

        A_DELETE_COPY(BuySellHandler)

        virtual void requestSellList(const std::string &nick)
                                     const override final;

        virtual void requestBuyList(const std::string &nick)
                                    const override final;

        virtual void sendBuyRequest(const std::string &nick,
                                    const ShopItem *const item,
                                    const int amount) const override final;

        virtual void sendSellRequest(const std::string &nick,
                                     const ShopItem *const item,
                                     const int amount) const override final;

        virtual void processNpcBuySellChoice(Net::MessageIn &msg);

        virtual void processNpcSell(Net::MessageIn &msg,
                                    const int offset) const;

        virtual void processNpcBuyResponse(Net::MessageIn &msg) const;

    protected:
        int mNpcId;
        BuyDialog *mBuyDialog;
};

}  // namespace Ea

#endif  // NET_EA_BUYSELLHANDLER_H
