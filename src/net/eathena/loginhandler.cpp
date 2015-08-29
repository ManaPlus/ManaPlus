/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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
#include "net/serverfeatures.h"

#include "net/ea/loginrecv.h"

#include "net/eathena/loginrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::LoginHandler *loginHandler;

namespace EAthena
{

extern ServerInfo charServer;

LoginHandler::LoginHandler() :
    MessageHandler(),
    Ea::LoginHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_LOGIN_DATA,
        SMSG_LOGIN_ERROR,
        SMSG_LOGIN_ERROR2,
        SMSG_SERVER_VERSION_RESPONSE,
        SMSG_UPDATE_HOST,
        SMSG_LOGIN_CODING_KEY,
        SMSG_CHAR_PASSWORD_RESPONSE,
        0
    };
    handledMessages = _messages;
    loginHandler = this;
}

LoginHandler::~LoginHandler()
{
}

void LoginHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_LOGIN_DATA:
            Ea::LoginRecv::processLoginData(msg);
            break;

        case SMSG_LOGIN_ERROR:
            Ea::LoginRecv::processLoginError(msg);
            break;

        case SMSG_LOGIN_ERROR2:
            LoginRecv::processLoginError2(msg);
            break;

        case SMSG_SERVER_VERSION_RESPONSE:
            LoginRecv::processServerVersion(msg);
            break;

        case SMSG_UPDATE_HOST:
            Ea::LoginRecv::processUpdateHost(msg);
            break;

        case SMSG_LOGIN_CODING_KEY:
            LoginRecv::processCondingKey(msg);
            break;

        case SMSG_CHAR_PASSWORD_RESPONSE:
            LoginRecv::processCharPasswordResponse(msg);
            break;

        default:
            break;
    }
}

void LoginHandler::connect()
{
    if (!mNetwork)
        return;

    mNetwork->connect(mServer);
    if (serverFeatures->haveServerVersion())
    {
        sendVersion();
    }
    else
    {
        if (client->getState() != STATE_LOGIN)
            client->setState(STATE_LOGIN);
    }
}

bool LoginHandler::isConnected() const
{
    if (!mNetwork)
        return false;

    return mNetwork->isConnected();
}

void LoginHandler::disconnect()
{
    if (mNetwork && mNetwork->getServer() == mServer)
        mNetwork->disconnect();
}

void LoginHandler::changePassword(const std::string &restrict oldPassword,
                                  const std::string &restrict newPassword)
                                  const
{
    if (!serverFeatures->haveChangePassword())
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
        createOutPacket(CMSG_LOGIN_REGISTER);
        outMsg.writeInt32(20, "client version");
        outMsg.writeString(username, 24, "login");
        outMsg.writeStringNoLog(password, 24, "password");
        outMsg.writeInt8(0x03, "client type");
    }
    else
    {
        createOutPacket(CMSG_LOGIN_REGISTER2);
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

int LoginHandler::supportedOptionalActions() const
{
    return serverFeatures->haveEmailOnRegister()
        ? Net::RegistrationOptions::SetEmailOnRegister
        | Net::RegistrationOptions::SetGenderOnRegister
        : Net::RegistrationOptions::SetGenderOnRegister;
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

}  // namespace EAthena
