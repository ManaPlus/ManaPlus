/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef NET_MANASERV_PARTYHANDLER_H
#define NET_MANASERV_PARTYHANDLER_H

#include "net/partyhandler.h"

#include "net/manaserv/messagehandler.h"

#include "party.h"

#include <string>

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#else
#define A_UNUSED
#endif

namespace ManaServ
{

class PartyHandler : public MessageHandler, public Net::PartyHandler
{
public:
    PartyHandler();

    void handleMessage(Net::MessageIn &msg);

    void create(const std::string &name = "");

    void join(int partyId);

    void invite(Being *being);

    void invite(const std::string &name);

    void inviteResponse(const std::string &inviter, bool accept);

    void leave();

    void kick(Being *being);

    void kick(const std::string &name);

    void chat(const std::string &text);

    void requestPartyMembers() const;

    PartyShare getShareExperience() const
    { return PARTY_SHARE_NO; }

    void setShareExperience(PartyShare share A_UNUSED)
    { }

    PartyShare getShareItems() const
    { return PARTY_SHARE_NO; }

    void setShareItems(PartyShare share A_UNUSED) {}
private:
    Party *mParty;
};

} // namespace ManaServ

#endif // NET_MANASERV_PARTYHANDLER_H
