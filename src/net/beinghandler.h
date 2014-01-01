/*
 *  The ManaPlus Client
 *  Copyright (C) 2004  The Mana World Development Team
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

#ifndef NET_BEINGHANDLER_H
#define NET_BEINGHANDLER_H

#include "being/being.h"

#include "net/messagein.h"

namespace Net
{

class BeingHandler
{
    public:
        virtual ~BeingHandler()
        { }

        virtual void handleMessage(Net::MessageIn &msg) = 0;

        virtual void requestNameById(const int id) const = 0;

        virtual void undress(Being *const being) const = 0;
};

}  // namespace Net

#endif  // NET_BEINGHANDLER_H
