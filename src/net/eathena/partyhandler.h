/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#ifndef NET_EATHENA_PARTYHANDLER_H
#define NET_EATHENA_PARTYHANDLER_H

#include "net/eathena/messagehandler.h"

#include "net/ea/partyhandler.h"

namespace EAthena
{

class PartyHandler final : public MessageHandler, public Ea::PartyHandler
{
    public:
        PartyHandler();

        A_DELETE_COPY(PartyHandler)

        ~PartyHandler();

        void handleMessage(Net::MessageIn &msg) override final;

        void create(const std::string &name) const override final;

        void invite(const std::string &name) const override final;

        void inviteResponse(const std::string &inviter,
                            const bool accept) const override final;

        void leave() const override final;

        void kick(const Being *const being) const override final;

        void kick(const std::string &name) const override final;

        void chat(const std::string &text) const override final;

        void setShareExperience(const Net::PartyShare::Type share)
                                const override final;

        void setShareItems(const Net::PartyShare::Type share)
                           const override final;

        void changeLeader(const std::string &name) const override final;

        void processPartySettings(Net::MessageIn &msg);

        void allowInvite(const bool allow) const override final;

    protected:
        void processPartyInvitationStats(Net::MessageIn &msg);

        void processPartyMemberInfo(Net::MessageIn &msg);

        void processPartyInfo(Net::MessageIn &msg) const;

        void processPartyMessage(Net::MessageIn &msg) const;

        void processPartyInviteResponse(Net::MessageIn &msg) const;

        void processPartyItemPickup(Net::MessageIn &msg) const;
};

}  // namespace EAthena

#endif  // NET_EATHENA_PARTYHANDLER_H
