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

#ifndef NET_REFINEHANDLER_H
#define NET_REFINEHANDLER_H

#include "resources/item/item.h"

#include "localconsts.h"

namespace Net
{

class RefineHandler notfinal
{
    public:
        RefineHandler()
        { }

        A_DELETE_COPY(RefineHandler)

        virtual ~RefineHandler()
        { }

        virtual void addItem(const Item *const item) const = 0;

        virtual void refineItem(const Item *const item1,
                                const Item *const item2,
                                int blessing) const = 0;
};

}  // namespace Net

extern Net::RefineHandler *refineHandler;

#endif  // NET_REFINEHANDLER_H
