/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

namespace TmwAthena
{

ServerFeatures::ServerFeatures() :
    Net::ServerFeatures()
{
    serverFeatures = this;
}

bool ServerFeatures::haveServerOnlineList() const
{
    return tmwServerVersion >= 0x120b0b;
}

bool ServerFeatures::haveServerHp() const
{
    return tmwServerVersion > 0x0f0513;
}

bool ServerFeatures::havePlayerStatusUpdate() const
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

bool ServerFeatures::haveMove3() const
{
    return tmwServerVersion >= 0x0f0512 && tmwServerVersion <= 0xf0706;
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

bool ServerFeatures::haveCreateCharGender() const
{
    return tmwServerVersion > 0x0f0b17;
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

bool ServerFeatures::haveExtendedRiding() const
{
    return false;
}

bool ServerFeatures::haveExtendedDropsPosition() const
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

bool ServerFeatures::havePincode() const
{
    return false;
}

}  // namespace TmwAthena
