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

#ifndef NET_TMWA_LOGINHANDLER_H
#define NET_TMWA_LOGINHANDLER_H

#include "net/ea/loginhandler.h"

namespace TmwAthena
{

class LoginHandler final : public Ea::LoginHandler
{
    public:
        LoginHandler();

        A_DELETE_COPY(LoginHandler)

        ~LoginHandler() override final;

        void connect() const override final;

        bool isConnected() const override final A_WARN_UNUSED;

        void disconnect() const override final;

        unsigned int getMaxPasswordLength() const override final A_WARN_UNUSED
        { return 24; }

        void changePassword(const std::string &restrict oldPassword,
                            const std::string &restrict newPassword)
                            const override final;

        ServerInfo *getCharServer() const override final A_CONST A_WARN_UNUSED;

        void sendVersion() const override final A_CONST;

        void ping() const override final A_CONST;

        void updatePacketVersion() const override final A_CONST;

        static void requestUpdateHosts() A_CONST;

    private:
        void sendLoginRegister(const std::string &restrict username,
                               const std::string &restrict password,
                               const std::string &restrict email)
                               const override final;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_LOGINHANDLER_H
