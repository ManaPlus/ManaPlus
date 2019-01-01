/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef NET_BUYSELLHANDLER_H
#define NET_BUYSELLHANDLER_H

#include "resources/item/shopitem.h"

class BuyDialog;

namespace Net
{

class BuySellHandler notfinal
{
    public:
        BuySellHandler()
        { }

        A_DELETE_COPY(BuySellHandler)

        virtual ~BuySellHandler()
        { }

        virtual void requestSellList(const std::string &nick) const = 0;

        virtual void requestBuyList(const std::string &nick) const = 0;

        virtual void sendBuyRequest(const std::string &nick,
                                    const ShopItem *const item,
                                    const int amount) const = 0;

        virtual void sendSellRequest(const std::string &nick,
                                     const ShopItem *const item,
                                     const int amount) const = 0;

        virtual void close() const = 0;

        virtual void cleanDialogReference(const BuyDialog *const dialog)
                                          const = 0;
};

}  // namespace Net

extern Net::BuySellHandler *buySellHandler;

#endif  // NET_BUYSELLHANDLER_H
