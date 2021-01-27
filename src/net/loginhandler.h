/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_LOGINHANDLER_H
#define NET_LOGINHANDLER_H

#include "net/serverinfo.h"

#include "enums/simpletypes/beingid.h"

#include "net/worldinfo.h"

class LoginData;

namespace Net
{

class LoginHandler notfinal
{
    public:
        A_DELETE_COPY(LoginHandler)

        void setServer(const ServerInfo &server)
        { mServer = server; }

        const ServerInfo &getServer() const A_WARN_UNUSED
        { return mServer; }

        virtual void connect() const = 0;

        virtual bool isConnected() const A_WARN_UNUSED = 0;

        virtual void disconnect() const = 0;

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

        virtual void changePassword(const std::string &restrict oldPassword,
                                    const std::string &restrict newPassword)
                                    const = 0;

        virtual void chooseServer(unsigned int server,
                                  const bool persistentIp) const = 0;

        virtual void registerAccount(const LoginData *const loginData)
                                     const = 0;

        virtual void unregisterAccount(const std::string &username,
                                       const std::string &password) const = 0;

        virtual const Worlds &getWorlds() const A_WARN_UNUSED = 0;

        virtual void clearWorlds() const = 0;

        virtual void loginOrRegister(LoginData *const data) const = 0;

        virtual void sendVersion() const = 0;

        virtual void ping() const = 0;

        virtual void updatePacketVersion() const = 0;

        virtual void sendMobileCode(const BeingId accountId,
                                    const std::string &code) const = 0;

        virtual void sendOtpCode(const std::string &code) const = 0;

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

extern Net::LoginHandler *loginHandler;

#endif  // NET_LOGINHANDLER_H
