/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#ifndef NET_TMWA_BEINGRECV_H
#define NET_TMWA_BEINGRECV_H

#if defined(__GXX_EXPERIMENTAL_CXX0X__)
#if defined(__APPLE__)
#include <tr1/cstdint>
#endif  // defined(__APPLE__)
#endif  // defined(__GXX_EXPERIMENTAL_CXX0X__)

#include "localconsts.h"

namespace Net
{
    class MessageIn;
}  // namespace Net

class Being;

namespace TmwAthena
{
    namespace BeingRecv
    {
        void processBeingChangeLook(Net::MessageIn &msg);
        void processBeingChangeLook2(Net::MessageIn &msg);
        void processBeingVisible(Net::MessageIn &msg);
        void processBeingMove(Net::MessageIn &msg);
        void processPlayerUpdate1(Net::MessageIn &msg);
        void processPlayerUpdate2(Net::MessageIn &msg);
        void processPlayerMove(Net::MessageIn &msg);
        void processBeingSpawn(Net::MessageIn &msg);
        void processSkillCasting(Net::MessageIn &msg);
        void processBeingStatusChange(Net::MessageIn &msg);
        void processBeingMove2(Net::MessageIn &msg);
        void processBeingChangeDirection(Net::MessageIn &msg);
        void processBeingChangeLookContinue(const Net::MessageIn &msg,
                                            Being *const dstBeing,
                                            const uint8_t type,
                                            const int id,
                                            const int id2) A_NONNULL(2);
        void processPlayerStatusChange(Net::MessageIn &msg);
        void processBeingResurrect(Net::MessageIn &msg);
        void processPlayerGuilPartyInfo(Net::MessageIn &msg);
        void processBeingSelfEffect(Net::MessageIn &msg);
        void processSkillCastCancel(Net::MessageIn &msg);
        void processIpResponse(Net::MessageIn &msg);
        void processPvpSet(Net::MessageIn &msg);
        void processSkillDamage(Net::MessageIn &msg);
        void applyPlayerAction(Net::MessageIn &msg,
                               Being *const being,
                               const uint8_t type);
        void setServerGender(Being *const being,
                             const uint8_t gender);
    }  // namespace BeingRecv
}  // namespace TmwAthena

#endif  // NET_TMWA_BEINGRECV_H
