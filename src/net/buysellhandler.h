/*
 *  The Mana Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *
 *  This file is part of The Mana Client.
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

#ifndef BUYSELLHANDLER_H
#define BUYSELLHANDLER_H

#include "net/messagein.h"

#include "being.h"
#include "shopitem.h"

namespace Net
{

class BuySellHandler
{
    public:
        virtual void handleMessage(Net::MessageIn &msg) = 0;
        virtual void requestSellList(std::string nick) = 0;
        virtual void requestBuyList(std::string nick) = 0;
        virtual void sendBuyRequest(std::string nick, ShopItem* item,
                                    int amount) = 0;
        virtual void sendSellRequest(std::string nick, ShopItem* item,
                                     int amount) = 0;
};

} // namespace Net

#endif // BUYSELLHANDLER_H
