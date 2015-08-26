/*
 *  The ManaPlus Client
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

#ifndef NET_EATHENA_HOMUNCULUSRECV_H
#define NET_EATHENA_HOMUNCULUSRECV_H

#ifdef EATHENA_SUPPORT

#include "net/homunculushandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{
    namespace HomunculusRecv
    {
        void processHomunculusSkills(Net::MessageIn &msg);
        void processHomunculusData(Net::MessageIn &msg);
        void processHomunculusInfo(Net::MessageIn &msg);
        void processHomunculusSkillUp(Net::MessageIn &msg);
        void processHomunculusFood(Net::MessageIn &msg);
    }  // namespace HomunculusRecv
}  // namespace EAthena

#endif  // EATHENA_SUPPORT
#endif  // NET_EATHENA_HOMUNCULUSRECV_H
