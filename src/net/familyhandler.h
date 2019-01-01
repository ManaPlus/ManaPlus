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

#ifndef NET_FAMILYHANDLER_H
#define NET_FAMILYHANDLER_H

#include "localconsts.h"

class Being;

namespace Net
{

class FamilyHandler notfinal
{
    public:
        FamilyHandler()
        { }

        A_DELETE_COPY(FamilyHandler)

        virtual ~FamilyHandler()
        { }

        virtual void askForChild(const Being *const being) const = 0;

        virtual void askForChildReply(const bool accept) const = 0;
};

}  // namespace Net

extern Net::FamilyHandler *familyHandler;

#endif  // NET_FAMILYHANDLER_H
