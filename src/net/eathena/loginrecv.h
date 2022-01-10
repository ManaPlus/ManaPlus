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

#ifndef NET_EATHENA_LOGINRECV_H
#define NET_EATHENA_LOGINRECV_H

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace LoginRecv
    {
        void processLoginError2(Net::MessageIn &msg);
        void processUpdateHost2(Net::MessageIn &msg);
        void processServerVersion(Net::MessageIn &msg);
        void processCondingKey(Net::MessageIn &msg);
        void processCharPasswordResponse(Net::MessageIn &msg);
        void processLoginData(Net::MessageIn &msg);
        void processLoginOtp1(Net::MessageIn &msg);
        void processLoginOtp2(Net::MessageIn &msg);
        void processLoginOtp3(Net::MessageIn &msg);
        void processMobileOtp(Net::MessageIn &msg);
    }  // namespace LoginRecv
}  // namespace EAthena

#endif  // NET_EATHENA_LOGINRECV_H
