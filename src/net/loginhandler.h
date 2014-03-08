/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_LOGINHANDLER_H
#define NET_LOGINHANDLER_H

#include "net/logindata.h"
#include "net/serverinfo.h"
#include "net/worldinfo.h"

#include <iosfwd>
#include <vector>

namespace Net
{

class LoginHandler
{
    public:
        enum OptionalAction
        {
            Unregister          = 0x1,
            ChangeEmail         = 0x2,
            SetEmailOnRegister  = 0x4,
            SetGenderOnRegister = 0x8
        };

        void setServer(const ServerInfo &server)
        { mServer = server; }

        const ServerInfo &getServer() const A_WARN_UNUSED
        { return mServer; }

        virtual void connect() = 0;

        virtual bool isConnected() const A_WARN_UNUSED = 0;

        virtual void disconnect() = 0;

        /**
         * @see OptionalAction
         */
        virtual int supportedOptionalActions() const A_WARN_UNUSED = 0;

        virtual bool isRegistrationEnabled() const A_WARN_UNUSED = 0;

        virtual void getRegistrationDetails() const = 0;

        virtual unsigned int getMinUserNameLength() const A_WARN_UNUSED
        { return 4; }

        virtual unsigned int getMaxUserNameLength() const A_WARN_UNUSED
        { return 25; }

        virtual unsigned int getMinPasswordLength() const A_WARN_UNUSED
        { return 4; }

        virtual unsigned int getMaxPasswordLength() const A_WARN_UNUSED
        { return 255; }

        virtual void loginAccount(LoginData *const loginData) const = 0;

        virtual void logout() const = 0;

        virtual void changeEmail(const std::string &email) const = 0;

        virtual void changePassword(const std::string &restrict username,
                                    const std::string &restrict oldPassword,
                                    const std::string &restrict newPassword)
                                    const = 0;

        virtual void chooseServer(unsigned int server) const = 0;

        virtual void registerAccount(LoginData *const loginData) const = 0;

        virtual void unregisterAccount(const std::string &username,
                                       const std::string &password) const = 0;

        virtual const Worlds &getWorlds() const A_WARN_UNUSED = 0;

        virtual void clearWorlds() = 0;

        virtual void loginOrRegister(LoginData *const data) const = 0;

        virtual ~LoginHandler()
        { }

    protected:
        LoginHandler() :
            mServer()
        {
        }

        ServerInfo mServer;
};

}  // namespace Net

#endif  // NET_LOGINHANDLER_H
