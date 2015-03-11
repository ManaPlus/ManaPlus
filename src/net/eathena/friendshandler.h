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

#ifndef NET_EATHENA_FRIENDSHANDLER_H
#define NET_EATHENA_FRIENDSHANDLER_H

#ifdef EATHENA_SUPPORT

#include "net/friendshandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{
class FriendsHandler final : public MessageHandler,
                             public Net::FriendsHandler
{
    public:
        FriendsHandler();

        A_DELETE_COPY(FriendsHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void invite(const std::string &name) const override final;

        void inviteResponse(const int accountId,
                            const int charId,
                            const bool accept) const override final;

        void remove(const int accountId,
                    const int charId) const override final;

    protected:
        static void processPlayerOnline(Net::MessageIn &msg);

        static void processFriendsList(Net::MessageIn &msg);

        static void processRequestAck(Net::MessageIn &msg);

        static void processRequest(Net::MessageIn &msg);

        static void processDeletePlayer(Net::MessageIn &msg);
};

}  // namespace EAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_EATHENA_FRIENDSHANDLER_H
