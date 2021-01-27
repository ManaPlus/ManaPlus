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

#ifndef NET_MARKETHANDLER_H
#define NET_MARKETHANDLER_H

#include "enums/resources/item/itemtype.h"

#include "enums/simpletypes/itemcolor.h"

#include "utils/vector.h"

#include "localconsts.h"

class ShopItem;

namespace Net
{

class MarketHandler notfinal
{
    public:
        MarketHandler()
        { }

        A_DELETE_COPY(MarketHandler)

        virtual ~MarketHandler()
        { }

        virtual void close() const = 0;

        virtual void buyItem(const int itemId,
                             const ItemTypeT type,
                             const ItemColor color,
                             const int amount) const = 0;

        virtual void buyItems(const STD_VECTOR<ShopItem*> &items) const = 0;
};

}  // namespace Net

extern Net::MarketHandler *marketHandler;

#endif  // NET_MARKETHANDLER_H
