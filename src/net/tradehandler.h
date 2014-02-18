/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#ifndef NET_TRADEHANDLER_H
#define NET_TRADEHANDLER_H

#include "being/being.h"

#include <iosfwd>

#include "localconsts.h"

class Item;

namespace Net
{
class TradeHandler
{
    public:
        virtual ~TradeHandler()
        { }

        virtual void request(const Being *const being A_UNUSED) const
        { }

        virtual void respond(const bool accept A_UNUSED) const
        { }

        virtual void addItem(const Item *const item A_UNUSED,
                             const int amount A_UNUSED) const
        { }

        virtual void removeItem(const int slotNum A_UNUSED,
                                const int amount A_UNUSED) const
        { }

        virtual void setMoney(const int amount A_UNUSED) const
        { }

        virtual void confirm() const
        { }

        virtual void finish() const
        { }

        virtual void cancel() const
        { }
};

}  // namespace Net

#endif  // NET_TRADEHANDLER_H
