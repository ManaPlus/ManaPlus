/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef NET_EA_LOGINRECV_H
#define NET_EA_LOGINRECV_H

#include "net/worldinfo.h"

#include "net/ea/token.h"

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace Ea
{
    namespace LoginRecv
    {
        extern std::string mUpdateHost;
        extern Worlds mWorlds;
        extern Token mToken;
        extern bool mVersionResponse;
        extern bool mRegistrationEnabled;

        void processUpdateHost(Net::MessageIn &msg);
        void processLoginError(Net::MessageIn &msg);
    }  // namespace LoginRecv
}  // namespace Ea

#endif  // NET_EA_LOGINRECV_H
