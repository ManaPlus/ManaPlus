/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#ifndef NET_EA_PARTYHANDLER_H
#define NET_EA_PARTYHANDLER_H

#include "net/partyhandler.h"

class Party;

namespace Ea
{
class PartyHandler notfinal : public Net::PartyHandler
{
    public:
        PartyHandler();

        A_DELETE_COPY(PartyHandler)

        ~PartyHandler() override;

        void join(const int partyId) const override final;

        PartyShareT getShareExperience() const override final A_WARN_UNUSED;

        PartyShareT getShareItems() const override final A_WARN_UNUSED;

        static void reload();

        void clear() const override final;

        ChatTab *getTab() const override final;
};

extern Party *taParty;

}  // namespace Ea

#endif  // NET_EA_PARTYHANDLER_H
