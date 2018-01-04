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

#ifndef NET_BUYINGSTOREHANDLER_H
#define NET_BUYINGSTOREHANDLER_H

#include "utils/vector.h"

#include <string>

#include "localconsts.h"

class Being;
class Item;
class ShopItem;

namespace Net
{

class BuyingStoreHandler notfinal
{
    public:
        BuyingStoreHandler()
        { }

        A_DELETE_COPY(BuyingStoreHandler)

        virtual ~BuyingStoreHandler()
        { }

        virtual void open(const Being *const being) const = 0;

        virtual void create(const std::string &name,
                            const int maxMoney,
                            const bool flag,
                            const STD_VECTOR<ShopItem*> &items) const = 0;

        virtual void close() const = 0;

        virtual void sell(const Being *const being,
                          const int storeId,
                          const Item *const item,
                          const int amount) const = 0;
};

}  // namespace Net

extern Net::BuyingStoreHandler *buyingStoreHandler;

#endif  // NET_BUYINGSTOREHANDLER_H
