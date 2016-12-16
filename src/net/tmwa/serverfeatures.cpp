/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

extern unsigned int tmwServerVersion;
extern Net::ServerFeatures *serverFeatures;

namespace TmwAthena
{

ServerFeatures::ServerFeatures()
{
    serverFeatures = this;
}

bool ServerFeatures::haveServerOnlineList() const
{
    return false;
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
    return tmwServerVersion > 0x0f0513;
}

bool ServerFeatures::havePlayerStatusUpdate() const
{
    return false;
}

bool ServerFeatures::haveBrokenPlayerAttackDistance() const
{
    return true;
}

bool ServerFeatures::haveNativeGuilds() const
{
    return false;
}

bool ServerFeatures::haveIncompleteChatMessages() const
{
    return tmwServerVersion <= 0x100209;
}

bool ServerFeatures::haveRaceSelection() const
{
    return false;
}

bool ServerFeatures::haveLookSelection() const
{
    return false;
}

bool ServerFeatures::haveChatChannels() const
{
    return false;
}

bool ServerFeatures::haveServerIgnore() const
{
    return false;
}

bool ServerFeatures::haveMove3() const
{
    return tmwServerVersion >= 0x0f0512 && tmwServerVersion <= 0xf0706;
}

bool ServerFeatures::haveItemColors() const
{
    return false;
}

bool ServerFeatures::haveAccountOtherGender() const
{
    return false;
}

bool ServerFeatures::haveCharOtherGender() const
{
    return tmwServerVersion > 0x0f0b17;
}

bool ServerFeatures::haveMonsterAttackRange() const
{
    return tmwServerVersion >= 0x0f0513;
}

bool ServerFeatures::haveEmailOnRegister() const
{
    return false;
}

bool ServerFeatures::haveEmailOnDelete() const
{
    return false;
}

bool ServerFeatures::haveEightDirections() const
{
    return false;
}

bool ServerFeatures::haveCharRename() const
{
    return false;
}

bool ServerFeatures::haveMonsterName() const
{
    return false;
}

bool ServerFeatures::haveBankApi() const
{
    return false;
}

bool ServerFeatures::haveServerVersion() const
{
    return true;
}

bool ServerFeatures::haveMapServerVersion() const
{
    return false;
}

bool ServerFeatures::haveNpcGender() const
{
    return tmwServerVersion >= 0x0f0513;
}

bool ServerFeatures::haveJoinChannel() const
{
    return false;
}

bool ServerFeatures::haveNpcWhispers() const
{
    return false;
}

bool ServerFeatures::haveCreateCharGender() const
{
    return tmwServerVersion > 0x0f0b17;
}

bool ServerFeatures::haveAttackDirections() const
{
    return false;
}

bool ServerFeatures::haveVending() const
{
    return false;
}

bool ServerFeatures::haveCart() const
{
    return false;
}

bool ServerFeatures::haveCards() const
{
    return false;
}

bool ServerFeatures::haveTalkPet() const
{
    return true;
}

bool ServerFeatures::haveMovePet() const
{
    return true;
}

bool ServerFeatures::haveServerWarpNames() const
{
    return false;
}

bool ServerFeatures::haveExpPacket() const
{
    return false;
}

bool ServerFeatures::haveMute() const
{
    return false;
}

bool ServerFeatures::haveTeamId() const
{
    return true;
}

bool ServerFeatures::haveAdvancedSprites() const
{
    return false;
}

bool ServerFeatures::haveExtendedRiding() const
{
    return false;
}

bool ServerFeatures::haveAdvancedBuySell() const
{
    return false;
}

bool ServerFeatures::haveSlide() const
{
    return false;
}

bool ServerFeatures::haveExtendedDropsPosition() const
{
    return false;
}

bool ServerFeatures::haveSecureTrades() const
{
    return false;
}

bool ServerFeatures::haveMultyStatusUp() const
{
    return false;
}

bool ServerFeatures::haveMail() const
{
    return false;
}

bool ServerFeatures::haveServerPets() const
{
    return false;
}

bool ServerFeatures::haveFamily() const
{
    return false;
}

bool ServerFeatures::haveMoveWhileSit() const
{
    return true;
}

bool ServerFeatures::haveKillerId() const
{
    return false;
}

}  // namespace TmwAthena
