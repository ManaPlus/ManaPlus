/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef NET_EATHENA_PARTYRECV_H
#define NET_EATHENA_PARTYRECV_H

#include "enums/net/partyshare.h"

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace PartyRecv
    {
        extern PartyShareT mShareAutoItems;

        void processPartySettings(Net::MessageIn &msg);
        void processPartyInvitationStats(Net::MessageIn &msg);
        void processPartyMemberInfo(Net::MessageIn &msg);
        void processPartyInfo(Net::MessageIn &msg);
        void processPartyMessage(Net::MessageIn &msg);
        void processPartyInviteResponse(Net::MessageIn &msg);
        void processPartyItemPickup(Net::MessageIn &msg);
        void processPartyLeader(Net::MessageIn &msg);
        void processPartyInvited(Net::MessageIn &msg);
        void processPartyMemberJobLevel(Net::MessageIn &msg);
        void processPartyMemberDead(Net::MessageIn &msg);

        void processPartyAutoItemSettingsContinue(Net::MessageIn &msg,
                                                  const PartyShareT item);
    }  // namespace PartyRecv
}  // namespace EAthena

#endif  // NET_EATHENA_PARTYRECV_H
