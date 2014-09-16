/*
 *  The ManaPlus Client
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

#include "net/eathena/serverfeatures.h"

#include "debug.h"

extern Net::ServerFeatures *serverFeatures;

namespace EAthena
{

ServerFeatures::ServerFeatures()
{
    serverFeatures = this;
}

bool ServerFeatures::haveOnlineList() const
{
    return false;
}

bool ServerFeatures::havePartyNickInvite() const
{
    return true;
}

bool ServerFeatures::haveChangePartyLeader() const
{
    return true;
}

bool ServerFeatures::haveServerHp() const
{
    return true;
}

bool ServerFeatures::haveLangTab() const
{
    return false;
}

bool ServerFeatures::havePlayerStatusUpdate() const
{
    return false;
}

bool ServerFeatures::haveBrokenPlayerAttackDistance() const
{
    return false;
}

}  // namespace EAthena
