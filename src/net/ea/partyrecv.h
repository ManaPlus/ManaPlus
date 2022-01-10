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

#ifndef NET_EA_PARTYRECV_H
#define NET_EA_PARTYRECV_H

#include "enums/net/partyshare.h"

class Party;

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace Ea
{
    namespace PartyRecv
    {
        extern PartyShareT mShareExp;
        extern PartyShareT mShareItems;

        void processPartyCreate(Net::MessageIn &msg);
        void processPartyLeave(Net::MessageIn &msg);
        void processPartyUpdateCoords(Net::MessageIn &msg);
        void processPartyExpSettingsContinue(Net::MessageIn &msg,
                                             const PartyShareT exp);
        void processPartyItemSettingsContinue(Net::MessageIn &msg,
                                              const PartyShareT item);

        void createTab();
    }  // namespace PartyRecv
    extern Party *taParty;
}  // namespace Ea

#endif  // NET_EA_PARTYRECV_H
