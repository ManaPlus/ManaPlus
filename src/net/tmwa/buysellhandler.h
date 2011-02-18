/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef NET_TA_BUYSELLHANDLER_H
#define NET_TA_BUYSELLHANDLER_H

#include "net/buysellhandler.h"

#include "being.h"

#include "net/net.h"

#include "net/tmwa/messagehandler.h"

class BuyDialog;

namespace TmwAthena
{

class BuySellHandler : public MessageHandler, public Net::BuySellHandler
{
    public:
        BuySellHandler();

        virtual void handleMessage(Net::MessageIn &msg);

        virtual void requestSellList(std::string nick);
        virtual void requestBuyList(std::string nick);
        virtual void sendBuyRequest(std::string nick, ShopItem* item,
                                    int amount);
        virtual void sendSellRequest(std::string nick, ShopItem* item,
                                     int amount);

    private:
        int mNpcId;
        BuyDialog *mBuyDialog;
};

} // namespace TmwAthena

#endif // NET_TA_BUYSELLHANDLER_H
