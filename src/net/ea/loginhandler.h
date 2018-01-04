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

#ifndef NET_EA_LOGINHANDLER_H
#define NET_EA_LOGINHANDLER_H

#include "net/loginhandler.h"

class LoginData;

struct Token;

namespace Ea
{

class LoginHandler notfinal : public Net::LoginHandler
{
    public:
        A_DELETE_COPY(LoginHandler)

        ~LoginHandler() override;

        bool isRegistrationEnabled() const override final A_WARN_UNUSED;

        void getRegistrationDetails() const override final;

        unsigned int getMaxPasswordLength() const override A_WARN_UNUSED
        { return 24; }

        void loginAccount(LoginData *const loginData) const override final;

        void chooseServer(const unsigned int server,
                          const bool persistentIp) const override;

        void registerAccount(const LoginData *const loginData)
                             const override final;

        const Worlds &getWorlds() const override final A_CONST A_WARN_UNUSED;

        void clearWorlds() const override final;

        virtual ServerInfo *getCharServer() const A_WARN_UNUSED = 0;

        const Token &getToken() const A_CONST A_WARN_UNUSED;

        void logout() const override final A_CONST;

        void changeEmail(const std::string &email) const
                         override final A_CONST;

        void unregisterAccount(const std::string &username,
                               const std::string &password)
                               const override final A_CONST;

        void loginOrRegister(LoginData *const data) const override final;

    protected:
        LoginHandler();

        virtual void sendLoginRegister(const std::string &username,
                                       const std::string &password,
                                       const std::string &email) const = 0;
};

}  // namespace Ea

#endif  // NET_EA_LOGINHANDLER_H
