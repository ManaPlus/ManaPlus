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

#ifndef NET_FRIENDSHANDLER_H
#define NET_FRIENDSHANDLER_H

#include <string>

#include "localconsts.h"

namespace Net
{

class FriendsHandler notfinal
{
    public:
        FriendsHandler()
        { }

        A_DELETE_COPY(FriendsHandler)

        virtual void invite(const std::string &name) const = 0;

        virtual void inviteResponse(const int accountId,
                                    const int charId,
                                    const bool accept) const = 0;

        virtual void remove(const int accountId, const int charId) const = 0;

        virtual ~FriendsHandler()
        { }
};

}  // namespace Net

extern Net::FriendsHandler *friendsHandler;

#endif  // NET_FRIENDSHANDLER_H
