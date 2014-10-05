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

        bool haveServerOnlineList() const override final;

        bool haveOnlineList() const override final;

        bool havePartyNickInvite() const override final;

        bool haveChangePartyLeader() const override final;

        bool haveServerHp() const override final;

        bool haveLangTab() const override final;

        bool havePlayerStatusUpdate() const override final;

        bool haveBrokenPlayerAttackDistance() const override final;

        bool haveNativeGuilds() const override final;

        bool haveIncompleteChatMessages() const override final;

        bool haveRaceSelection() const override final;

        bool haveLookSelection() const override final;

        bool haveChatChannels() const override final;

        bool haveServerIgnore() const override final;

        bool haveMove3() const override final;

        bool haveItemColors() const override final;

        bool haveOtherGender() const override final;

        bool haveMonsterAttackRange() const override final;

        bool haveEmailOnRegister() const override final;

        bool haveEightDirections() const override final;

        bool haveCharRename() const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_SERVERFEATURES_H
