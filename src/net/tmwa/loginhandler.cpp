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

#include "net/tmwa/loginhandler.h"

#include "client.h"
#include "logger.h"

#include "net/serverfeatures.h"

#include "net/ea/loginrecv.h"

#include "net/tmwa/loginrecv.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"

#include "utils/gettext.h"

#include "debug.h"

extern Net::LoginHandler *loginHandler;

namespace TmwAthena
{

extern ServerInfo charServer;

LoginHandler::LoginHandler() :
    MessageHandler(),
    Ea::LoginHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_UPDATE_HOST,
        SMSG_LOGIN_DATA,
        SMSG_LOGIN_ERROR,
        SMSG_CHAR_PASSWORD_RESPONSE,
        SMSG_SERVER_VERSION_RESPONSE,
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
    BLOCK_START("LoginHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_CHAR_PASSWORD_RESPONSE:
            LoginRecv::processCharPasswordResponse(msg);
            break;

        case SMSG_UPDATE_HOST:
            Ea::LoginRecv::processUpdateHost(msg);
            break;

        case SMSG_LOGIN_DATA:
            Ea::LoginRecv::processLoginData(msg);
            break;

        case SMSG_LOGIN_ERROR:
            Ea::LoginRecv::processLoginError(msg);
            break;

        case SMSG_SERVER_VERSION_RESPONSE:
            LoginRecv::processServerVersion(msg);
            break;

        default:
            break;
    }
    BLOCK_END("LoginHandler::handleMessage")
}

void LoginHandler::connect()
{
    if (!mNetwork)
        return;

    mNetwork->connect(mServer);
    if (serverFeatures->haveServerVersion())
        createOutPacket(CMSG_SERVER_VERSION_REQUEST);
}

bool LoginHandler::isConnected() const
{
    if (!mNetwork)
        return false;

    return Ea::LoginRecv::mVersionResponse && mNetwork->isConnected();
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
    if (serverVersion > 0)
    {
        outMsg.writeInt32(CLIENT_PROTOCOL_VERSION,
            "client protocol version");
    }
    else
    {
        outMsg.writeInt32(CLIENT_TMW_PROTOCOL_VERSION,
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

int LoginHandler::supportedOptionalActions() const
{
    return Net::RegistrationOptions::SetGenderOnRegister;
}

void LoginHandler::sendVersion() const
{
}

void LoginHandler::ping() const
{
}

}  // namespace TmwAthena
