/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/ea/partyhandler.h"

namespace EAthena
{

class PartyHandler final : public Ea::PartyHandler
{
    public:
        PartyHandler();

        A_DELETE_COPY(PartyHandler)

        ~PartyHandler();

        void create(const std::string &name) const final;

        void invite(const std::string &name) const final;

        void inviteResponse(const int partyId,
                            const bool accept) const final;

        void leave() const final;

        void kick(const Being *const being) const final;

        void kick(const std::string &name) const final;

        void chat(const std::string &text) const final;

        void setShareExperience(const PartyShareT share) const final;

        void setShareItems(const PartyShareT share) const final;

        void changeLeader(const std::string &name) const final;

        void allowInvite(const bool allow) const final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_PARTYHANDLER_H
