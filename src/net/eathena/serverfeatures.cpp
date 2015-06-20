/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "net/net.h"

#include "debug.h"

extern int serverVersion;
extern Net::ServerFeatures *serverFeatures;

namespace EAthena
{

ServerFeatures::ServerFeatures()
{
    serverFeatures = this;
}

bool ServerFeatures::haveServerOnlineList() const
{
    return serverVersion >= 5;
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

bool ServerFeatures::havePlayerStatusUpdate() const
{
    return serverVersion >= 5;
}

bool ServerFeatures::haveBrokenPlayerAttackDistance() const
{
    return false;
}

bool ServerFeatures::haveNativeGuilds() const
{
    return true;
}

bool ServerFeatures::haveIncompleteChatMessages() const
{
    return false;
}

bool ServerFeatures::haveRaceSelection() const
{
    return serverVersion >= 4;
}

bool ServerFeatures::haveLookSelection() const
{
    return serverVersion >= 4;
}

bool ServerFeatures::haveChatChannels() const
{
    return true;
}

bool ServerFeatures::haveServerIgnore() const
{
    return true;
}

bool ServerFeatures::haveMove3() const
{
    return serverVersion >= 3;
}

bool ServerFeatures::haveItemColors() const
{
    return false;
}

bool ServerFeatures::haveOtherGender() const
{
    return false;
}

bool ServerFeatures::haveMonsterAttackRange() const
{
    return false;
}

bool ServerFeatures::haveEmailOnRegister() const
{
    return serverVersion >= 2;
}

bool ServerFeatures::haveEmailOnDelete() const
{
    return serverVersion >= 2;
}

bool ServerFeatures::haveEightDirections() const
{
    return true;
}

bool ServerFeatures::haveCharRename() const
{
    return true;
}

bool ServerFeatures::haveMonsterName() const
{
    return true;
}

bool ServerFeatures::haveBankApi() const
{
    return true;
}

bool ServerFeatures::haveServerVersion() const
{
    return Net::getNetworkType() == ServerType::EVOL2;
}

bool ServerFeatures::haveMapServerVersion() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveNpcGender() const
{
    return serverVersion >= 3;
}

bool ServerFeatures::haveJoinChannel() const
{
    return serverVersion >= 3;
}

bool ServerFeatures::haveNpcWhispers() const
{
    return true;
}

bool ServerFeatures::haveCreateCharGender() const
{
    return serverVersion >= 4;
}

bool ServerFeatures::haveAttackDirections() const
{
    return true;
}

bool ServerFeatures::haveVending() const
{
    return true;
}

bool ServerFeatures::haveCart() const
{
    return true;
}

bool ServerFeatures::haveTalkPet() const
{
    return serverVersion >= 5;
}

bool ServerFeatures::haveMovePet() const
{
    return serverVersion >= 5;
}

bool ServerFeatures::haveServerWarpNames() const
{
    return serverVersion == 0;
}

bool ServerFeatures::haveExpPacket() const
{
    return true;
}

bool ServerFeatures::haveMute() const
{
    return true;
}

bool ServerFeatures::haveChangePassword() const
{
    return serverVersion >= 6;
}

bool ServerFeatures::haveTeamId() const
{
    return false;
}

}  // namespace EAthena
