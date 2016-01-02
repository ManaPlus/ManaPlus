/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_EA_BEINGRECV_H
#define NET_EA_BEINGRECV_H

#include "enums/simpletypes/beingid.h"
#include "enums/simpletypes/itemcolor.h"

#include <string>

namespace Net
{
    class MessageIn;
}

class Being;

namespace Ea
{
    namespace BeingRecv
    {
        extern bool mSync;
        extern BeingId mSpawnId;

        void processBeingRemove(Net::MessageIn &msg);
        void processSkillDamage(Net::MessageIn &msg);
        void processBeingAction(Net::MessageIn &msg);
        void processBeingEmotion(Net::MessageIn &msg);
        void processNameResponse(Net::MessageIn &msg);
        void processPlayerStop(Net::MessageIn &msg);
        void processPlayerMoveToAttack(Net::MessageIn &msg);
        void processSkillNoDamage(Net::MessageIn &msg);
        void processPvpMapMode(Net::MessageIn &msg);
        void processBeingMove3(Net::MessageIn &msg);
        Being *createBeing(const BeingId id,
                           const int job);
        void setSprite(Being *const being, const unsigned int slot,
                       const int id,
                       const std::string &color = "",
                       const ItemColor colorId = ItemColor_one,
                       const bool isWeapon = false,
                       const bool isTempSprite = false);
    }  // namespace BeingRecv
}  // namespace Ea

#endif  // NET_EA_BEINGRECV_H
