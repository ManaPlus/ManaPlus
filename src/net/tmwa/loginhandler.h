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

#ifndef NET_TMWA_LOGINHANDLER_H
#define NET_TMWA_LOGINHANDLER_H

#include "net/loginhandler.h"

#include "net/ea/loginhandler.h"

#include "net/tmwa/messagehandler.h"

#include <string>

class LoginData;

namespace TmwAthena
{

class LoginHandler final : public MessageHandler, public Ea::LoginHandler
{
    public:
        LoginHandler();

        A_DELETE_COPY(LoginHandler)

        ~LoginHandler();

        void handleMessage(Net::MessageIn &msg);

        void connect();

        bool isConnected() A_WARN_UNUSED;

        void disconnect();

        int supportedOptionalActions() const A_WARN_UNUSED;

        unsigned int getMaxPasswordLength() const A_WARN_UNUSED
        { return 25; }

        void changePassword(const std::string &username,
                            const std::string &oldPassword,
                            const std::string &newPassword);

        ServerInfo *getCharServer() A_WARN_UNUSED;

        void processServerVersion(Net::MessageIn &msg);

        void requestUpdateHosts();

        void processUpdateHost2(Net::MessageIn &msg);

    private:
        void sendLoginRegister(const std::string &username,
                               const std::string &password,
                               const std::string &email);
};

} // namespace TmwAthena

#endif // NET_TA_LOGINHANDLER_H
