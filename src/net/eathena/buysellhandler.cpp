/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/eathena/buysellhandler.h"

#include "net/ea/buysellrecv.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

BuySellHandler::BuySellHandler() :
    Ea::BuySellHandler()
{
    buySellHandler = this;
    Ea::BuySellRecv::mBuyDialog = nullptr;
}

BuySellHandler::~BuySellHandler()
{
    buySellHandler = nullptr;
}

void BuySellHandler::requestSellList(const std::string &nick A_UNUSED) const
{
}

void BuySellHandler::requestBuyList(const std::string &nick A_UNUSED) const
{
}

void BuySellHandler::sendBuyRequest(const std::string &nick A_UNUSED,
                                    const ShopItem *const item A_UNUSED,
                                    const int amount A_UNUSED) const
{
}

void BuySellHandler::sendSellRequest(const std::string &nick A_UNUSED,
                                     const ShopItem *const item A_UNUSED,
                                     const int amount A_UNUSED) const
{
}

void BuySellHandler::close() const
{
    if (packetVersion < 20131218)
        return;

    createOutPacket(CMSG_NPC_SHOP_CLOSE);
}

}  // namespace EAthena
