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

#include "net/tmwa/loginhandler.h"

#include "net/serverfeatures.h"

#include "net/ea/loginrecv.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/network.h"
#include "net/tmwa/protocolout.h"

#include "debug.h"

extern unsigned int tmwServerVersion;
extern int itemIdLen;

namespace TmwAthena
{

extern ServerInfo charServer;

LoginHandler::LoginHandler() :
    Ea::LoginHandler()
{
    loginHandler = this;
}

LoginHandler::~LoginHandler()
{
    loginHandler = nullptr;
}

void LoginHandler::connect() const
{
    if (Network::mInstance == nullptr)
        return;

    Network::mInstance->connect(mServer);
    if (serverFeatures->haveServerVersion())
    {
        createOutPacket(CMSG_SERVER_VERSION_REQUEST);
    }
}

bool LoginHandler::isConnected() const
{
    if (Network::mInstance == nullptr)
        return false;

    return Ea::LoginRecv::mVersionResponse &&
        Network::mInstance->isConnected();
}

void LoginHandler::disconnect() const
{
    if (Network::mInstance != nullptr &&
        Network::mInstance->getServer() == mServer)
    {
        Network::mInstance->disconnect();
    }
}

void LoginHandler::changePassword(const std::string &restrict oldPassword,
                                  const std::string &restrict newPassword)
                                  const
{
    createOutPacket(CMSG_CHAR_PASSWORD_CHANGE);
    outMsg.writeStringNoLog(oldPassword, 24, "old password");
    outMsg.writeStringNoLog(newPassword, 24, "new password");
}

void LoginHandler::sendLoginRegister(const std::string &restrict username,
                                     const std::string &restrict password,
                                     const std::string &restrict email
                                     A_UNUSED) const
{
    createOutPacket(CMSG_LOGIN_REGISTER);
    if (tmwServerVersion < 0x100408)
    {
        // hack to avoid bug in tmwa...
        outMsg.writeInt32(5,
            "client protocol version");
    }
    else
    {
        outMsg.writeInt32(CLIENT_PROTOCOL_VERSION,
            "client protocol version");
    }
    outMsg.writeString(username, 24, "login");
    outMsg.writeStringNoLog(password, 24, "password");

    /*
     * eAthena calls the last byte "client version 2", but it isn't used at
     * at all. We're retasking it, as a bit mask:
     *  0 - can handle the 0x63 "update host" packet
     *  1 - defaults to the first char-server (instead of the last)
     */
    outMsg.writeInt8(0x03, "flags");
}

ServerInfo *LoginHandler::getCharServer() const
{
    return &charServer;
}

void LoginHandler::requestUpdateHosts()
{
}

void LoginHandler::sendVersion() const
{
}

void LoginHandler::ping() const
{
}

void LoginHandler::updatePacketVersion() const
{
    itemIdLen = 2;
}

void LoginHandler::sendMobileCode(const BeingId accountId A_UNUSED,
                                  const std::string &code A_UNUSED) const
{
}

void LoginHandler::sendOtpCode(const std::string &code A_UNUSED) const
{
}

}  // namespace TmwAthena
