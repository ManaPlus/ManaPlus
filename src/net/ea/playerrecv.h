/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef NET_EA_PLAYERRECV_H
#define NET_EA_PLAYERRECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace Ea
{
    namespace PlayerRecv
    {
        void processPlayerWarp(Net::MessageIn &msg);
        void processPlayerStatUpdate1(Net::MessageIn &msg);
        void processPlayerStatUpdate2(Net::MessageIn &msg);
        void processPlayerStatUpdate3(Net::MessageIn &msg);
        void processPlayerStatUpdate4(Net::MessageIn &msg);
        void processPlayerStatUpdate6(Net::MessageIn &msg);
        void processPlayerArrowMessage(Net::MessageIn &msg);
        void processPlayerClientCommand(Net::MessageIn &msg);
        void processMapMusic(Net::MessageIn &msg);
        void processMapMask(Net::MessageIn &msg);
    }  // namespace PlayerRecv
}  // namespace Ea

#endif  // NET_EA_PLAYERRECV_H
