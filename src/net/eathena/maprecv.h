/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef NET_EATHENA_MAPRECV_H
#define NET_EATHENA_MAPRECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace MapRecv
    {
        void processInstanceStart(Net::MessageIn &msg);
        void processInstanceCreate(Net::MessageIn &msg);
        void processInstanceInfo(Net::MessageIn &msg);
        void processInstanceDelete(Net::MessageIn &msg);
        void processAddMapMarker(Net::MessageIn &msg);
        void processCameraInfo(Net::MessageIn &msg);
    }  // namespace MapRecv
}  // namespace EAthena

#endif  // NET_EATHENA_MAPRECV_H
