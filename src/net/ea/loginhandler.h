/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#ifndef NET_EA_LOGINHANDLER_H
#define NET_EA_LOGINHANDLER_H

#include "net/loginhandler.h"
#include "net/net.h"
#include "net/messagein.h"

#include "net/ea/token.h"

#include <string>

class LoginData;

namespace Ea
{

class LoginHandler : public Net::LoginHandler
{
    public:
        LoginHandler();

        A_DELETE_COPY(LoginHandler)

        ~LoginHandler();

        int supportedOptionalActions() const A_WARN_UNUSED
        { return SetGenderOnRegister; }

        virtual bool isRegistrationEnabled() A_WARN_UNUSED;

        virtual void getRegistrationDetails();

        virtual unsigned int getMaxPasswordLength() const A_WARN_UNUSED
        { return 25; }

        virtual void loginAccount(LoginData *loginData);

        virtual void chooseServer(unsigned int server);

        virtual void registerAccount(LoginData *loginData);

        virtual Worlds getWorlds() const A_WARN_UNUSED;

        virtual void clearWorlds();

        virtual ServerInfo *getCharServer() A_WARN_UNUSED = 0;

        const Token &getToken() const A_WARN_UNUSED
        { return mToken; }

        virtual void logout();

        virtual void changeEmail(const std::string &email);

        virtual void unregisterAccount(const std::string &username,
                                       const std::string &password);

        virtual void procecessCharPasswordResponse(Net::MessageIn &msg);

        virtual void processUpdateHost(Net::MessageIn &msg);

        virtual void processUpdateHost2(Net::MessageIn &msg) = 0;

        virtual void processLoginData(Net::MessageIn &msg);

        virtual void processLoginError(Net::MessageIn &msg);

        virtual void processServerVersion(Net::MessageIn &msg) = 0;

    protected:
        virtual void sendLoginRegister(const std::string &username,
                                       const std::string &password,
                                       const std::string &email) = 0;

        bool mVersionResponse;
        bool mRegistrationEnabled;
        std::string mUpdateHost;
        Worlds mWorlds;
        Token mToken;
};

} // namespace Ea

#endif // NET_TA_LOGINHANDLER_H
