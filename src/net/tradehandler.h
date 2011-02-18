/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef TRADEHANDLER_H
#define TRADEHANDLER_H

#include "being.h"

#include <iosfwd>

#ifdef __GNUC__
#define _UNUSED_  __attribute__ ((unused))
#else
#define _UNUSED_
#endif

namespace Net
{
class TradeHandler
{
    public:
        virtual void request(Being *being _UNUSED_)
        { }
//        virtual ~TradeHandler() {}

        virtual void respond(bool accept _UNUSED_)
        { }

        virtual void addItem(Item *item _UNUSED_, int amount _UNUSED_)
        { }

        virtual void removeItem(int slotNum _UNUSED_, int amount _UNUSED_)
        { }

        virtual void setMoney(int amount _UNUSED_)
        { }

        virtual void confirm()
        { }

        virtual void finish()
        { }

        virtual void cancel()
        { }
};
}

#endif // TRADEHANDLER_H
