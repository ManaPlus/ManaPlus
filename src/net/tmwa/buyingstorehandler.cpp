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

#include "net/tmwa/buyingstorehandler.h"

#include "debug.h"

namespace TmwAthena
{

BuyingStoreHandler::BuyingStoreHandler()
{
    buyingStoreHandler = this;
}

BuyingStoreHandler::~BuyingStoreHandler()
{
    buyingStoreHandler = nullptr;
}

void BuyingStoreHandler::create(const std::string &name A_UNUSED,
                                const int maxMoney A_UNUSED,
                                const bool flag A_UNUSED,
                                const STD_VECTOR<ShopItem*> &items A_UNUSED)
                                const
{
}

void BuyingStoreHandler::close() const
{
}

void BuyingStoreHandler::open(const Being *const being A_UNUSED) const
{
}

void BuyingStoreHandler::sell(const Being *const being A_UNUSED,
                              const int storeId A_UNUSED,
                              const Item *const item A_UNUSED,
                              const int amount A_UNUSED) const
{
}

}  // namespace TmwAthena
