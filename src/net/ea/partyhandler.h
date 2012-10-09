/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "net/messagein.h"
#include "net/net.h"
#include "net/partyhandler.h"

#include "net/ea/gui/partytab.h"

#include "party.h"

namespace Ea
{

class PartyHandler : public Net::PartyHandler
{
    public:
        PartyHandler();

        A_DELETE_COPY(PartyHandler);

        ~PartyHandler();

        void join(int partyId);

        void requestPartyMembers() const;

        PartyShare getShareExperience() const
        { return mShareExp; }

        PartyShare getShareItems() const
        { return mShareItems; }

        void reload();

        void clear();

        virtual void processPartyCreate(Net::MessageIn &msg);

        virtual void processPartyInfo(Net::MessageIn &msg);

        virtual void processPartyInviteResponse(Net::MessageIn &msg);

        virtual void processPartyInvited(Net::MessageIn &msg);

        virtual void processPartySettings(Net::MessageIn &msg);

        virtual void processPartyMove(Net::MessageIn &msg);

        virtual void processPartyLeave(Net::MessageIn &msg);

        virtual void processPartyUpdateHp(Net::MessageIn &msg);

        virtual void processPartyUpdateCoords(Net::MessageIn &msg);

        virtual void processPartyMessage(Net::MessageIn &msg);

    protected:
        PartyShare mShareExp, mShareItems;
};

extern PartyTab *partyTab;
extern Party *taParty;

} // namespace Ea

#endif // NET_EA_PARTYHANDLER_H
