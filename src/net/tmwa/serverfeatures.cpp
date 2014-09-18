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

#include "net/tmwa/serverfeatures.h"

#include "debug.h"

extern int serverVersion;
extern int tmwServerVersion;
extern Net::ServerFeatures *serverFeatures;

namespace TmwAthena
{

ServerFeatures::ServerFeatures()
{
    serverFeatures = this;
}

bool ServerFeatures::haveServerOnlineList() const
{
    return serverVersion >= 3;
}

bool ServerFeatures::haveOnlineList() const
{
    return true;
}

bool ServerFeatures::havePartyNickInvite() const
{
    return false;
}

bool ServerFeatures::haveChangePartyLeader() const
{
    return false;
}

bool ServerFeatures::haveServerHp() const
{
    return serverVersion > 0 || tmwServerVersion > 0x0E081d;
}

bool ServerFeatures::haveLangTab() const
{
    return serverVersion >= 8;
}

bool ServerFeatures::havePlayerStatusUpdate() const
{
    return serverVersion >= 4;
}

bool ServerFeatures::haveBrokenPlayerAttackDistance() const
{
    return serverVersion < 1;
}

bool ServerFeatures::haveNativeGuilds() const
{
    return tmwServerVersion <= 0;
}

bool ServerFeatures::haveIncompleteChatMessages() const
{
    return tmwServerVersion > 0;
}

bool ServerFeatures::haveRaceSelection() const
{
    return serverVersion >= 2;
}

bool ServerFeatures::haveLookSelection() const
{
    return serverVersion >= 9;
}

bool ServerFeatures::haveChatChannels() const
{
    return serverVersion >= 8;
}

bool ServerFeatures::haveServerIgnore() const
{
    return tmwServerVersion == 0;
}

bool ServerFeatures::haveMove3() const
{
    return serverVersion >= 10;
}

bool ServerFeatures::haveItemColors() const
{
    return serverVersion >= 1;
}

bool ServerFeatures::haveOtherGender() const
{
    return serverVersion >= 5;
}

bool ServerFeatures::haveMonsterAttackRange() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveEmailOnRegister() const
{
    return serverVersion >= 7;
}

bool ServerFeatures::haveEightDirections() const
{
    return false;
}

}  // namespace TmwAthena
