/*
 *  The ManaPlus Client
 *  Copyright (C) 2004  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_BEINGHANDLER_H
#define NET_BEINGHANDLER_H

#include "being/being.h"

#include "enums/being/rank.h"

namespace Net
{

class BeingHandler notfinal
{
    public:
        BeingHandler()
        { }

        A_DELETE_COPY(BeingHandler)

        virtual ~BeingHandler()
        { }

        virtual void requestNameById(const BeingId id) const = 0;

        virtual void requestNameByCharId(const int id) const = 0;

        virtual void undress(Being *const being) const = 0;

        virtual void requestRanks(const RankT rank) const = 0;

        virtual void viewPlayerEquipment(const Being *const being) const = 0;
};

}  // namespace Net

extern Net::BeingHandler *beingHandler;

#endif  // NET_BEINGHANDLER_H
