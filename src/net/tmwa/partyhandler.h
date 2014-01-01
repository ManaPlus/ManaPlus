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

#ifndef NET_TMWA_PARTYHANDLER_H
#define NET_TMWA_PARTYHANDLER_H

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

        void handleMessage(Net::MessageIn &msg) override final;

        void create(const std::string &name) const override final;

        void invite(Being *const being) const override final;

        void invite(const std::string &name) const override final;

        void inviteResponse(const std::string &inviter,
                            const bool accept) const override final;

        void leave() const override final;

        void kick(Being *const being) const override final;

        void kick(const std::string &name) const override final;

        void chat(const std::string &text) const override final;

        void setShareExperience(const PartyShare share) const override final;

        void setShareItems(const PartyShare share) const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_PARTYHANDLER_H
