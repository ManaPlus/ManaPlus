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

#ifndef NET_SERVERFEATURES_H
#define NET_SERVERFEATURES_H

#include "localconsts.h"

namespace Net
{
class ServerFeatures notfinal
{
    public:
        virtual ~ServerFeatures()
        { }

        virtual bool haveOnlineList() const = 0;

        virtual bool havePartyNickInvite() const = 0;

        virtual bool haveChangePartyLeader() const = 0;

        virtual bool haveServerHp() const = 0;

        virtual bool haveLangTab() const = 0;

        virtual bool havePlayerStatusUpdate() const = 0;

        virtual bool haveBrokenPlayerAttackDistance() const = 0;

        virtual bool haveNativeGuilds() const = 0;

        virtual bool haveIncompleteChatMessages() const = 0;

        virtual bool haveRaceSelection() const = 0;

        virtual bool haveLookSelection() const = 0;

        virtual bool haveChatChannels() const = 0;

        virtual bool haveServerIgnore() const = 0;

        virtual bool haveMove3() const = 0;

        virtual bool haveItemColors() const = 0;

        virtual bool haveOtherGender() const = 0;
};

}  // namespace Net

#endif  // NET_SERVERFEATURES_H
