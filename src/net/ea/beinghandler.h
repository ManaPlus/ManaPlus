/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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
#include "net/net.h"

namespace Ea
{

class BeingHandler : public Net::BeingHandler
{
    public:
        BeingHandler(bool enableSync);

        A_DELETE_COPY(BeingHandler);

    protected:
        virtual void requestNameById(int id) = 0;

        virtual Being *createBeing(int id, short job);

        virtual void setSprite(Being *being, unsigned int slot, int id,
                               std::string color = "",
                               unsigned char colorId = 1,
                               bool isWeapon = false,
                               bool isTempSprite = false);

        virtual void processBeingVisibleOrMove(Net::MessageIn &msg,
                                               bool visible);

        virtual void processBeingMove2(Net::MessageIn &msg);

        virtual void processBeingSpawn(Net::MessageIn &msg);

        virtual void processBeingRemove(Net::MessageIn &msg);

        virtual void processBeingResurrect(Net::MessageIn &msg);

        virtual void processSkillDamage(Net::MessageIn &msg);

        virtual void processBeingAction(Net::MessageIn &msg);

        virtual void processBeingSelfEffect(Net::MessageIn &msg);

        virtual void processBeingEmotion(Net::MessageIn &msg);

        virtual void processBeingChangeLook(Net::MessageIn &msg,
                                            bool look2) = 0;

        virtual void processNameResponse(Net::MessageIn &msg);

        virtual void processIpResponse(Net::MessageIn &msg);

        virtual void processPlayerGuilPartyInfo(Net::MessageIn &msg);

        virtual void processBeingChangeDirection(Net::MessageIn &msg);

        virtual void processPlayerMoveUpdate(Net::MessageIn &msg,
                                             int type) = 0;

        virtual void processPlayerStop(Net::MessageIn &msg);

        virtual void processPlayerMoveToAttack(Net::MessageIn &msg);

        virtual void processPlaterStatusChange(Net::MessageIn &msg);

        virtual void processBeingStatusChange(Net::MessageIn &msg);

        virtual void processSkilCasting(Net::MessageIn &msg);

        virtual void processSkillNoDamage(Net::MessageIn &msg);

        virtual void processPvpMapMode(Net::MessageIn &msg);

        virtual void processPvpSet(Net::MessageIn &msg);

    protected:
        // Should we honor server "Stop Walking" packets
        bool mSync;
        int mSpawnId;
};

} // namespace Ea

#endif // NET_EA_BEINGHANDLER_H
