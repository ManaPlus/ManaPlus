/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "net/registrationoptions.h"

#include "net/ea/token.h"

#include <string>

class LoginData;

namespace Net
{
    class MessageIn;
}

namespace Ea
{

class LoginHandler notfinal : public Net::LoginHandler
{
    public:
        A_DELETE_COPY(LoginHandler)

        virtual ~LoginHandler();

        int supportedOptionalActions() const override A_WARN_UNUSED
        { return Net::RegistrationOptions::SetGenderOnRegister; }

        virtual bool isRegistrationEnabled() const
                                           override final A_WARN_UNUSED;

        virtual void getRegistrationDetails() const override final;

        virtual unsigned int getMaxPasswordLength() const
                                                  override A_WARN_UNUSED
        { return 24; }

        virtual void loginAccount(LoginData *const loginData)
                                  const override final;

        virtual void chooseServer(const unsigned int server,
                                  const bool persistentIp) const override;

        virtual void registerAccount(const LoginData *const loginData)
                                     const override final;

        virtual const Worlds &getWorlds() const override final A_WARN_UNUSED;

        virtual void clearWorlds() override final;

        virtual ServerInfo *getCharServer() const A_WARN_UNUSED = 0;

        const Token &getToken() const A_WARN_UNUSED
        { return mToken; }

        virtual void logout() const override final;

        virtual void changeEmail(const std::string &email)
                                 const override final;

        virtual void unregisterAccount(const std::string &username,
                                       const std::string &password)
                                       const override final;

        static void processCharPasswordResponse(Net::MessageIn &msg);

        static void processUpdateHost(Net::MessageIn &msg);

        static void processLoginData(Net::MessageIn &msg);

        static void processLoginError(Net::MessageIn &msg);

        static void processUpdateHost2(Net::MessageIn &msg);

        void loginOrRegister(LoginData *const data) const override final;

    protected:
        LoginHandler();

        virtual void sendLoginRegister(const std::string &username,
                                       const std::string &password,
                                       const std::string &email) const = 0;

        static std::string mUpdateHost;
        static Worlds mWorlds;
        static Token mToken;
        static bool mVersionResponse;
        static bool mRegistrationEnabled;
};

}  // namespace Ea

#endif  // NET_EA_LOGINHANDLER_H
