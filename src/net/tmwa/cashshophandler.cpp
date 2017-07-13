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

#include "net/tmwa/cashshophandler.h"

#include "debug.h"

namespace TmwAthena
{

CashShopHandler::CashShopHandler() :
    Net::CashShopHandler()
{
    cashShopHandler = this;
}

void CashShopHandler::buyItem(const int points A_UNUSED,
                              const int itemId A_UNUSED,
                              const ItemColor color A_UNUSED,
                              const int amount A_UNUSED) const
{
}

void CashShopHandler::buyItems(const int points A_UNUSED,
                               const STD_VECTOR<ShopItem*> &items A_UNUSED)
                               const
{
}

void CashShopHandler::close() const
{
}

void CashShopHandler::requestPoints() const
{
}

void CashShopHandler::requestTab(const int tab A_UNUSED) const
{
}

void CashShopHandler::schedule() const
{
}

}  // namespace TmwAthena
