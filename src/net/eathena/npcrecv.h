/*
 *  The ManaPlus Client
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

#ifndef NET_EATHENA_NPCRECV_H
#define NET_EATHENA_NPCRECV_H

#include "enums/simpletypes/beingtypeid.h"

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace NpcRecv
    {
        extern BeingTypeId mNpcTypeId;

        void processNpcCutin(Net::MessageIn &msg);
        void processNpcViewPoint(Net::MessageIn &msg);
        void processNpcShowProgressBar(Net::MessageIn &msg);
        void processNpcCloseTimeout(Net::MessageIn &msg);
        void processArea(Net::MessageIn &msg);
        void processShowDigit(Net::MessageIn &msg);
        void processProgressBarAbort(Net::MessageIn &msg);
        void processShowProgressBarFree(Net::MessageIn &msg);
        void processNpcSkin(Net::MessageIn &msg);
        void processPrivateAirShipResponse(Net::MessageIn &msg);
    }  // namespace NpcRecv
}  // namespace EAthena

#endif  // NET_EATHENA_NPCRECV_H
