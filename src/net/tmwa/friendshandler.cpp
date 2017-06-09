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

#include "net/tmwa/friendshandler.h"

#include "debug.h"

namespace TmwAthena
{

FriendsHandler::FriendsHandler()
{
    friendsHandler = this;
}

void FriendsHandler::invite(const std::string &name A_UNUSED) const
{
}

void FriendsHandler::inviteResponse(const int accountId A_UNUSED,
                                    const int charId A_UNUSED,
                                    const bool accept A_UNUSED) const
{
}

void FriendsHandler::remove(const int accountId A_UNUSED,
                            const int charId A_UNUSED) const
{
}

}  // namespace TmwAthena
