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

        bool haveServerOnlineList() const override final;

        bool haveServerHp() const override final A_CONST;

        bool havePlayerStatusUpdate() const override final;

        bool haveIncompleteChatMessages() const override final A_CONST;

        bool haveRaceSelection() const override final;

        bool haveLookSelection() const override final;

        bool haveMove3() const override final;

        bool haveCharOtherGender() const override final;

        bool haveMonsterAttackRange() const override final A_CONST;

        bool haveEmailOnRegister() const override final;

        bool haveEmailOnDelete() const override final;

        bool haveServerVersion() const override final;

        bool haveMapServerVersion() const override final;

        bool haveNpcGender() const override final;

        bool haveJoinChannel() const override final;

        bool haveCreateCharGender() const override final;

        bool haveTalkPet() const override final;

        bool haveMovePet() const override final;

        bool haveServerWarpNames() const override final;

        bool haveExtendedRiding() const override final;

        bool haveExtendedDropsPosition() const override final;

        bool haveServerPets() const override final;

        bool haveFamily() const override final;

        bool haveMoveWhileSit() const override final;

        bool haveKillerId() const override final;
};

}  // namespace EAthena

#endif  // NET_EATHENA_SERVERFEATURES_H
