/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

class BeingHandler notfinal : public Net::BeingHandler
{
    public:
        A_DELETE_COPY(BeingHandler)

    protected:
        explicit BeingHandler(const bool enableSync);

        static Being *createBeing(const BeingId id,
                                  const int job) A_WARN_UNUSED;

        static void setSprite(Being *const being, const unsigned int slot,
                              const int id,
                              const std::string &color = "",
                              const unsigned char colorId = 1,
                              const bool isWeapon = false,
                              const bool isTempSprite = false);

        static void processBeingRemove(Net::MessageIn &msg);

        static void processSkillDamage(Net::MessageIn &msg);

        static void processBeingAction(Net::MessageIn &msg);

        static void processBeingEmotion(Net::MessageIn &msg);

        static void processNameResponse(Net::MessageIn &msg);

        static void processPlayerStop(Net::MessageIn &msg);

        static void processPlayerMoveToAttack(Net::MessageIn &msg);

        static void processSkillNoDamage(Net::MessageIn &msg);

        static void processPvpMapMode(Net::MessageIn &msg);

        static void processPvpSet(Net::MessageIn &msg);

        static void processNameResponse2(Net::MessageIn &msg);

        static void processBeingMove3(Net::MessageIn &msg);

        // Should we honor server "Stop Walking" packets
        static BeingId mSpawnId;
        static bool mSync;
};

}  // namespace Ea

#endif  // NET_EA_BEINGHANDLER_H
