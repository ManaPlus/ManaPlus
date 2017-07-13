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

#ifndef NET_CASHSHOPHANDLER_H
#define NET_CASHSHOPHANDLER_H

#include "enums/simpletypes/itemcolor.h"

#include "utils/vector.h"

#include "localconsts.h"

class ShopItem;

namespace Net
{

class CashShopHandler notfinal
{
    public:
        CashShopHandler()
        { }

        A_DELETE_COPY(CashShopHandler)

        virtual ~CashShopHandler()
        { }

        virtual void buyItem(const int points,
                             const int itemId,
                             const ItemColor color,
                             const int amount) const = 0;

        virtual void buyItems(const int points,
                              const std::vector<ShopItem*> &items) const = 0;

        virtual void close() const = 0;

        virtual void requestPoints() const = 0;

        virtual void requestTab(const int tab) const = 0;

        virtual void schedule() const = 0;
};

}  // namespace Net

extern Net::CashShopHandler *cashShopHandler;

#endif  // NET_CASHSHOPHANDLER_H
