/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_PARTYHANDLER_H
#define NET_PARTYHANDLER_H

#include "localconsts.h"

#include "enums/net/partyshare.h"

#include "gui/widgets/tabs/chat/chattab.h"

class Being;

namespace Net
{

class PartyHandler notfinal
{
    public:
        PartyHandler()
        { }

        A_DELETE_COPY(PartyHandler)

        virtual ~PartyHandler()
        { }

        virtual void create(const std::string &name) const = 0;

        virtual void join(const int partyId) const = 0;

        virtual void invite(const std::string &name) const = 0;

        virtual void leave() const = 0;

        virtual void kick(const Being *const player) const = 0;

        virtual void kick(const std::string &name) const = 0;

        virtual void chat(const std::string &text) const = 0;

        virtual PartyShareT getShareExperience() const A_WARN_UNUSED = 0;

        virtual void setShareExperience(const PartyShareT share) const = 0;

        virtual PartyShareT getShareItems() const A_WARN_UNUSED = 0;

        virtual void setShareItems(const PartyShareT share) const = 0;

        virtual PartyShareT getShareAutoItems() const A_WARN_UNUSED = 0;

        virtual void setShareAutoItems(const PartyShareT share) const = 0;

        virtual void changeLeader(const std::string &name) const = 0;

        virtual void allowInvite(const bool allow) const = 0;

        virtual void clear() const = 0;

        virtual void inviteResponse(const int partyId,
                                    const bool accept) const = 0;

        virtual ChatTab *getTab() const = 0;
};

}  // namespace Net

extern Net::PartyHandler *partyHandler;

#endif  // NET_PARTYHANDLER_H
