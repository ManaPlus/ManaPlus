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

#ifndef NET_TMWA_SERVERFEATURES_H
#define NET_TMWA_SERVERFEATURES_H

#include "net/serverfeatures.h"

#include "localconsts.h"

namespace TmwAthena
{
class ServerFeatures final : public Net::ServerFeatures
{
    public:
        ServerFeatures();

        A_DELETE_COPY(ServerFeatures)

        bool haveServerOnlineList() const final A_CONST;

        bool haveOnlineList() const final A_CONST;

        bool havePartyNickInvite() const final A_CONST;

        bool haveChangePartyLeader() const final A_CONST;

        bool haveServerHp() const final;

        bool havePlayerStatusUpdate() const final A_CONST;

        bool haveBrokenPlayerAttackDistance() const final A_CONST;

        bool haveNativeGuilds() const final;

        bool haveIncompleteChatMessages() const final;

        bool haveRaceSelection() const final A_CONST;

        bool haveLookSelection() const final A_CONST;

        bool haveChatChannels() const final A_CONST;

        bool haveServerIgnore() const final;

        bool haveMove3() const final;

        bool haveItemColors() const final A_CONST;

        bool haveAccountOtherGender() const final A_CONST;

        bool haveCharOtherGender() const final;

        bool haveMonsterAttackRange() const final;

        bool haveMonsterName() const final A_CONST;

        bool haveEmailOnRegister() const final A_CONST;

        bool haveEmailOnDelete() const final A_CONST;

        bool haveEightDirections() const final A_CONST;

        bool haveCharRename() const final A_CONST;

        bool haveBankApi() const final A_CONST;

        bool haveServerVersion() const final A_CONST;

        bool haveMapServerVersion() const final A_CONST;

        bool haveNpcGender() const final;

        bool haveJoinChannel() const final A_CONST;

        bool haveNpcWhispers() const final A_CONST;

        bool haveCreateCharGender() const final;

        bool haveAttackDirections() const final A_CONST;

        bool haveVending() const final A_CONST;

        bool haveCart() const final A_CONST;

        bool haveTalkPet() const final A_CONST;

        bool haveMovePet() const final A_CONST;

        bool haveServerWarpNames() const final A_CONST;

        bool haveExpPacket() const final A_CONST;

        bool haveMute() const final A_CONST;

        bool haveChangePassword() const final A_CONST;

        bool haveTeamId() const final;

        bool haveNewGuild() const final;

        bool haveAdvancedSprites() const final A_CONST;

        bool haveExtendedRiding() const final A_CONST;

        bool haveAdvancedBuySell() const final A_CONST;

        bool haveSlide() const final A_CONST;

        bool haveExtendedDropsPosition() const final A_CONST;

        bool haveSecureTrades() const final A_CONST;

        bool haveMultyStatusUp() const final A_CONST;

        bool haveMail() const final A_CONST;

        bool haveServerPets() const final A_CONST;

        bool haveFamily() const final A_CONST;

        bool haveMoveWhileSit() const final A_CONST;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_SERVERFEATURES_H
