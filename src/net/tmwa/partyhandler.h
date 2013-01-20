/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef NET_TA_PARTYHANDLER_H
#define NET_TA_PARTYHANDLER_H

#include "net/net.h"
#include "net/partyhandler.h"

#include "net/tmwa/messagehandler.h"

#include "net/ea/partyhandler.h"

namespace TmwAthena
{

class PartyHandler final : public MessageHandler, public Ea::PartyHandler
{
    public:
        PartyHandler();

        A_DELETE_COPY(PartyHandler)

        ~PartyHandler();

        void handleMessage(Net::MessageIn &msg);

        void create(const std::string &name = "");

        void invite(Being *being);

        void invite(const std::string &name);

        void inviteResponse(const std::string &inviter, bool accept);

        void leave();

        void kick(Being *being);

        void kick(const std::string &name);

        void chat(const std::string &text);

        void setShareExperience(PartyShare share);

        void setShareItems(PartyShare share);
};

} // namespace TmwAthena

#endif // NET_TA_PARTYHANDLER_H
