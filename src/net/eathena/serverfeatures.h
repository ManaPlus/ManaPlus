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

#ifndef NET_EATHENA_SERVERFEATURES_H
#define NET_EATHENA_SERVERFEATURES_H

#include "net/serverfeatures.h"

#include "localconsts.h"

namespace EAthena
{
class ServerFeatures final : public Net::ServerFeatures
{
    public:
        ServerFeatures();

        A_DELETE_COPY(ServerFeatures)

        bool haveServerOnlineList() const final;

        bool haveOnlineList() const final A_CONST;

        bool havePartyNickInvite() const final A_CONST;

        bool haveChangePartyLeader() const final A_CONST;

        bool haveServerHp() const final A_CONST;

        bool havePlayerStatusUpdate() const final;

        bool haveBrokenPlayerAttackDistance() const final A_CONST;

        bool haveNativeGuilds() const final A_CONST;

        bool haveIncompleteChatMessages() const final A_CONST;

        bool haveRaceSelection() const final;

        bool haveLookSelection() const final;

        bool haveChatChannels() const final A_CONST;

        bool haveServerIgnore() const final A_CONST;

        bool haveMove3() const final;

        bool haveItemColors() const final A_CONST;

        bool haveAccountOtherGender() const final A_CONST;

        bool haveCharOtherGender() const final;

        bool haveMonsterAttackRange() const final A_CONST;

        bool haveMonsterName() const final A_CONST;

        bool haveEmailOnRegister() const final;

        bool haveEmailOnDelete() const final;

        bool haveEightDirections() const final A_CONST;

        bool haveCharRename() const final A_CONST;

        bool haveBankApi() const final A_CONST;

        bool haveServerVersion() const final;

        bool haveMapServerVersion() const final;

        bool haveNpcGender() const final;

        bool haveJoinChannel() const final;

        bool haveNpcWhispers() const final A_CONST;

        bool haveCreateCharGender() const final;

        bool haveAttackDirections() const final A_CONST;

        bool haveVending() const final A_CONST;

        bool haveCart() const final A_CONST;

        bool haveTalkPet() const final;

        bool haveMovePet() const final;

        bool haveServerWarpNames() const final;

        bool haveExpPacket() const final A_CONST;

        bool haveMute() const final A_CONST;

        bool haveChangePassword() const final;

        bool haveTeamId() const final A_CONST;

        bool haveNewGuild() const final A_CONST;

        bool haveAdvancedSprites() const final;

        bool haveExtendedRiding() const final;

        bool haveAdvancedBuySell() const final A_CONST;

        bool haveSlide() const final;

        bool haveExtendedDropsPosition() const final;

        bool haveSecureTrades() const final;

        bool haveMultyStatusUp() const final;

        bool haveMail() const final;

        bool haveServerPets() const final;

        bool haveFamily() const final;

        bool haveMoveWhileSit() const final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_SERVERFEATURES_H
