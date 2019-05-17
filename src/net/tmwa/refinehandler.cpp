/*
 *  The ManaPlus Client
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

#include "net/tmwa/refinehandler.h"

#include "debug.h"

namespace TmwAthena
{

RefineHandler::RefineHandler() :
    Net::RefineHandler()
{
    refineHandler = this;
}

RefineHandler::~RefineHandler()
{
    refineHandler = nullptr;
}

void RefineHandler::addItem(const Item *const item A_UNUSED) const
{
}

void RefineHandler::refineItem(const Item *const item1 A_UNUSED,
                               const Item *const item2 A_UNUSED,
                               const int blessing A_UNUSED) const
{
}

void RefineHandler::close() const
{
}

}  // namespace TmwAthena
