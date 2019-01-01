/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "net/eathena/loginhandler.h"

#include "client.h"

#include "net/generalhandler.h"
#include "net/net.h"
#include "net/serverfeatures.h"

#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/updateprotocol.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

namespace EAthena
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
        sendVersion();
    }
    else
    {
        if (client->getState() != State::LOGIN)
            client->setState(State::LOGIN);
    }
}

bool LoginHandler::isConnected() const
{
    if (Network::mInstance == nullptr)
        return false;

    return Network::mInstance->isConnected();
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
    if (serverVersion == 0)
        return;
    createOutPacket(CMSG_CHAR_PASSWORD_CHANGE);
    outMsg.writeStringNoLog(oldPassword, 24, "old password");
    outMsg.writeStringNoLog(newPassword, 24, "new password");
}

void LoginHandler::sendLoginRegister(const std::string &restrict username,
                                     const std::string &restrict password,
                                     const std::string &restrict email) const
{
    if (email.empty())
    {
        if (Net::getNetworkType() == ServerType::EATHENA)
        {
            createOutPacket(CMSG_LOGIN_REGISTER_HAN);
            outMsg.writeInt32(20, "client version");
            outMsg.writeString(username, 24, "login");
            outMsg.writeStringNoLog(password, 24, "password");
            outMsg.writeInt8(0x03, "client type");
            outMsg.writeString("127.0.0.1", 16, "ip address");
            outMsg.writeString("001122334455", 13, "mac address");
            outMsg.writeInt8(0, "is gravity id");
        }
        else
        {
            createOutPacket(CMSG_LOGIN_REGISTER);
            outMsg.writeInt32(20, "client version");
            outMsg.writeString(username, 24, "login");
            outMsg.writeStringNoLog(password, 24, "password");
            outMsg.writeInt8(0x03, "client type");
        }
    }
    else
    {
        createOutPacket(CMSG_LOGIN_REGISTER4);
        outMsg.writeString(username, 24, "login");
        outMsg.writeStringNoLog(password, 24, "password");
        outMsg.writeInt8(0x03, "client type");
        outMsg.writeString(email, 40, "email");
    }
}

ServerInfo *LoginHandler::getCharServer() const
{
    return &charServer;
}

void LoginHandler::sendVersion() const
{
    createOutPacket(CMSG_SERVER_VERSION_REQUEST);
    outMsg.writeInt32(CLIENT_PROTOCOL_VERSION, "protocol version");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    generalHandler->flushSend();
}

void LoginHandler::ping() const
{
    createOutPacket(CMSG_LOGIN_PING);
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
}

void LoginHandler::updatePacketVersion() const
{
    updateProtocol();
}

}  // namespace EAthena
