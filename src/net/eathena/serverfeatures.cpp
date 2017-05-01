/*
 *  The ManaPlus Client
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

#include "net/eathena/serverfeatures.h"

#include "net/net.h"

#include "debug.h"

extern int serverVersion;
extern Net::ServerFeatures *serverFeatures;

namespace EAthena
{

ServerFeatures::ServerFeatures() :
    Net::ServerFeatures()
{
    serverFeatures = this;
}

bool ServerFeatures::haveServerOnlineList() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveServerHp() const
{
    return true;
}

bool ServerFeatures::havePlayerStatusUpdate() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveIncompleteChatMessages() const
{
    return false;
}

bool ServerFeatures::haveRaceSelection() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveLookSelection() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveMove3() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveCharOtherGender() const
{
    return true;
}

bool ServerFeatures::haveMonsterAttackRange() const
{
    return false;
}

bool ServerFeatures::haveEmailOnRegister() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveEmailOnDelete() const
{
    return serverVersion > 0;
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
    return serverVersion > 0;
}

bool ServerFeatures::haveJoinChannel() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveCreateCharGender() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveCards() const
{
    return true;
}

bool ServerFeatures::haveTalkPet() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveMovePet() const
{
    return serverVersion > 0;
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

bool ServerFeatures::haveTeamId() const
{
    return false;
}

bool ServerFeatures::haveExtendedRiding() const
{
    return true;
}

bool ServerFeatures::haveAdvancedBuySell() const
{
    return true;
}

bool ServerFeatures::haveSlide() const
{
    return true;
}

bool ServerFeatures::haveExtendedDropsPosition() const
{
    return serverVersion >= 12;
}

bool ServerFeatures::haveSecureTrades() const
{
    return true;
}

bool ServerFeatures::haveMultyStatusUp() const
{
    return true;
}

bool ServerFeatures::haveMail() const
{
    return true;
}

bool ServerFeatures::haveServerPets() const
{
    return true;
}

bool ServerFeatures::haveFamily() const
{
    return true;
}

bool ServerFeatures::haveMoveWhileSit() const
{
    return serverVersion > 0;
}

bool ServerFeatures::haveKillerId() const
{
    return serverVersion >= 14;
}

}  // namespace EAthena
