/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/ea/partyhandler.h"

#include "party.h"

#include "net/ea/partyrecv.h"

#include "gui/widgets/tabs/chat/partytab.h"

#include "utils/delete2.h"

#include "debug.h"

namespace Ea
{

PartyHandler::PartyHandler() :
    Net::PartyHandler()
{
    PartyRecv::mShareExp = PartyShare::UNKNOWN;
    PartyRecv::mShareItems = PartyShare::UNKNOWN;
    taParty = Party::getParty(1);
}

PartyHandler::~PartyHandler()
{
    delete2(partyTab)
    taParty = nullptr;
}

void PartyHandler::join(const int partyId A_UNUSED) const
{
}

void PartyHandler::reload()
{
    taParty = Party::getParty(1);
}

void PartyHandler::clear() const
{
    taParty = nullptr;
}

ChatTab *PartyHandler::getTab() const
{
    return partyTab;
}

PartyShareT PartyHandler::getShareExperience() const
{
    return PartyRecv::mShareExp;
}

PartyShareT PartyHandler::getShareItems() const
{
    return PartyRecv::mShareItems;
}

}  // namespace Ea
