/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_EA_BEINGHANDLER_H
#define NET_EA_BEINGHANDLER_H

#include "net/beinghandler.h"

namespace Ea
{

class BeingHandler : public Net::BeingHandler
{
    public:
        A_DELETE_COPY(BeingHandler)

    protected:
        explicit BeingHandler(const bool enableSync);

        Being *createBeing(const int id,
                           const int16_t job) const A_WARN_UNUSED;

        virtual void setSprite(Being *const being, const unsigned int slot,
                               const int id,
                               const std::string &color = "",
                               const unsigned char colorId = 1,
                               const bool isWeapon = false,
                               const bool isTempSprite = false) const;

        virtual void processBeingVisibleOrMove(Net::MessageIn &msg,
                                               const bool visible);

        virtual void processBeingMove2(Net::MessageIn &msg) const;

        virtual void processBeingSpawn(Net::MessageIn &msg);

        virtual void processBeingRemove(Net::MessageIn &msg) const;

        virtual void processBeingResurrect(Net::MessageIn &msg) const;

        virtual void processSkillDamage(Net::MessageIn &msg) const;

        virtual void processBeingAction(Net::MessageIn &msg) const;

        virtual void processBeingSelfEffect(Net::MessageIn &msg) const;

        virtual void processBeingEmotion(Net::MessageIn &msg) const;

        virtual void processBeingChangeLook(Net::MessageIn &msg,
                                            const bool look2) const = 0;

        virtual void processNameResponse(Net::MessageIn &msg) const;

        virtual void processIpResponse(Net::MessageIn &msg) const;

        virtual void processPlayerGuilPartyInfo(Net::MessageIn &msg) const;

        virtual void processBeingChangeDirection(Net::MessageIn &msg) const;

        virtual void processPlayerMoveUpdate(Net::MessageIn &msg,
                                             const int type) const = 0;

        virtual void processPlayerStop(Net::MessageIn &msg) const;

        virtual void processPlayerMoveToAttack(Net::MessageIn &msg) const;

        virtual void processPlaterStatusChange(Net::MessageIn &msg) const;

        virtual void processBeingStatusChange(Net::MessageIn &msg) const;

        virtual void processSkilCasting(Net::MessageIn &msg) const;

        virtual void processSkillNoDamage(Net::MessageIn &msg) const;

        virtual void processPvpMapMode(Net::MessageIn &msg) const;

        virtual void processPvpSet(Net::MessageIn &msg) const;

        // Should we honor server "Stop Walking" packets
        bool mSync;
        int mSpawnId;
        bool mHideShield;
};

}  // namespace Ea

#endif  // NET_EA_BEINGHANDLER_H
