/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef NET_VENDINGHANDLER_H
#define NET_VENDINGHANDLER_H

#include <string>

#include "localconsts.h"

class Being;

namespace Net
{

class VendingHandler notfinal
{
    public:
        virtual ~VendingHandler()
        { }

        virtual void close() const = 0;

        virtual void open(const Being *const being) const = 0;

        virtual void buy(const Being *const being,
                         const int index,
                         const int amount) const = 0;
};

}  // namespace Net

extern Net::VendingHandler *vendingHandler;

#endif  // NET_VENDINGHANDLER_H
