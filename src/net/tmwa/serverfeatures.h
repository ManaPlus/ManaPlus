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

        bool haveServerOnlineList() const override final A_CONST;

        bool haveServerHp() const override final;

        bool havePlayerStatusUpdate() const override final A_CONST;

        bool haveIncompleteChatMessages() const override final;

        bool haveRaceSelection() const override final A_CONST;

        bool haveLookSelection() const override final A_CONST;

        bool haveMove3() const override final;

        bool haveCharOtherGender() const override final;

        bool haveMonsterAttackRange() const override final;

        bool haveEmailOnRegister() const override final A_CONST;

        bool haveEmailOnDelete() const override final A_CONST;

        bool haveServerVersion() const override final A_CONST;

        bool haveMapServerVersion() const override final A_CONST;

        bool haveNpcGender() const override final;

        bool haveJoinChannel() const override final A_CONST;

        bool haveCreateCharGender() const override final;

        bool haveTalkPet() const override final A_CONST;

        bool haveMovePet() const override final A_CONST;

        bool haveServerWarpNames() const override final A_CONST;

        bool haveExtendedRiding() const override final A_CONST;

        bool haveExtendedDropsPosition() const override final A_CONST;

        bool haveMoveWhileSit() const override final A_CONST;

        bool haveKillerId() const override final A_CONST;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_SERVERFEATURES_H
