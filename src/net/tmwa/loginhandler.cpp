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

#include "net/tmwa/loginhandler.h"

#include "client.h"
#include "logger.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"

#include "utils/paths.h"

#include "debug.h"

extern Net::LoginHandler *loginHandler;

namespace TmwAthena
{

extern ServerInfo charServer;

enum ServerFlags
{
    FLAG_REGISTRATION = 1
};

LoginHandler::LoginHandler() :
    MessageHandler(),
    Ea::LoginHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_UPDATE_HOST,
        SMSG_UPDATE_HOST2,
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
            procecessCharPasswordResponse(msg);
            break;

        case SMSG_UPDATE_HOST:
            processUpdateHost(msg);
            break;

        case SMSG_UPDATE_HOST2:
            processUpdateHost2(msg);
            break;

        case SMSG_LOGIN_DATA:
            processLoginData(msg);
            break;

        case SMSG_LOGIN_ERROR:
            processLoginError(msg);
            break;

        case SMSG_SERVER_VERSION_RESPONSE:
            processServerVersion(msg);
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
    MessageOut outMsg(CMSG_SERVER_VERSION_REQUEST);
}

bool LoginHandler::isConnected() const
{
    if (!mNetwork)
        return false;

    return mVersionResponse && mNetwork->isConnected();
}

void LoginHandler::disconnect()
{
    if (mNetwork && mNetwork->getServer() == mServer)
        mNetwork->disconnect();
}

void LoginHandler::changePassword(const std::string &restrict username
                                  A_UNUSED,
                                  const std::string &restrict oldPassword,
                                  const std::string &restrict newPassword)
                                  const
{
    MessageOut outMsg(CMSG_CHAR_PASSWORD_CHANGE);
    outMsg.writeStringNoLog(oldPassword, 24);
    outMsg.writeStringNoLog(newPassword, 24);
}

void LoginHandler::sendLoginRegister(const std::string &restrict username,
                                     const std::string &restrict password,
                                     const std::string &restrict email) const
{
    if (email.empty())
    {
        MessageOut outMsg(CMSG_LOGIN_REGISTER);
        outMsg.writeInt32(0);  // client version
        outMsg.writeString(username, 24);
        outMsg.writeStringNoLog(password, 24);

        /*
         * eAthena calls the last byte "client version 2", but it isn't used at
         * at all. We're retasking it, as a bit mask:
         *  0 - can handle the 0x63 "update host" packet
         *  1 - defaults to the first char-server (instead of the last)
         */
        outMsg.writeInt8(0x03);
    }
    else
    {
        MessageOut outMsg(CMSG_LOGIN_REGISTER2);
        outMsg.writeInt32(0);  // client version
        outMsg.writeString(username, 24);
        outMsg.writeStringNoLog(password, 24);

        /*
         * eAthena calls the last byte "client version 2", but it isn't used at
         * at all. We're retasking it, as a bit mask:
         *  0 - can handle the 0x63 "update host" packet
         *  1 - defaults to the first char-server (instead of the last)
         */
        outMsg.writeInt8(0x03);
        outMsg.writeString(email, 24);
    }
}

ServerInfo *LoginHandler::getCharServer() const
{
    return &charServer;
}

void LoginHandler::requestUpdateHosts()
{
    MessageOut outMsg(CMSG_SEND_CLIENT_INFO);
    outMsg.writeInt8(CLIENT_PROTOCOL_VERSION);
    outMsg.writeInt8(0);    // unused
}

void LoginHandler::processServerVersion(Net::MessageIn &msg)
{
    const uint8_t b1 = msg.readInt8();  // -1
    const uint8_t b2 = msg.readInt8();  // E
    const uint8_t b3 = msg.readInt8();  // V
    const uint8_t b4 = msg.readInt8();  // L
    if (b1 == 255 && b2 == 'E' && b3 == 'V' && b4 == 'L')
    {
        const unsigned int options = msg.readInt8();
        mRegistrationEnabled = options & FLAG_REGISTRATION;
        msg.skip(2);  // 0 unused
        serverVersion = msg.readInt8();
        tmwServerVersion = 0;
        if (serverVersion >= 5)
            requestUpdateHosts();
    }
    else if (b1 == 255)
    {   // old TMWA
        const unsigned int options = msg.readInt32();
        mRegistrationEnabled = options & FLAG_REGISTRATION;
        serverVersion = 0;
        tmwServerVersion = 0;
    }
    else if (b1 >= 0x0d)
    {   // new TMWA
        const unsigned int options = msg.readInt32();
        mRegistrationEnabled = options & FLAG_REGISTRATION;
        serverVersion = 0;
        tmwServerVersion = (b1 << 16) | (b2 << 8) | b3;
    }
    else
    {   // eAthena
        const unsigned int options = msg.readInt32();
        mRegistrationEnabled = options & FLAG_REGISTRATION;
        serverVersion = 0;
        tmwServerVersion = 0;
    }
    if (serverVersion > 0)
        logger->log("Evol server version: %d", serverVersion);
    else if (tmwServerVersion > 0)
        logger->log("Tmw server version: x%06x", tmwServerVersion);
    else
        logger->log("Server witout version");

    if (serverVersion < 5)
    {
        if (client->getState() != STATE_LOGIN)
            client->setState(STATE_LOGIN);
    }

    // Leave this last
    mVersionResponse = true;
}

void LoginHandler::processUpdateHost2(Net::MessageIn &msg) const
{
    const int len = msg.readInt16() - 4;
    const std::string updateHost = msg.readString(len);

    splitToStringVector(loginData.updateHosts, updateHost, '|');
    FOR_EACH (StringVectIter, it, loginData.updateHosts)
    {
        if (!checkPath(*it))
        {
            logger->log1("Warning: incorrect update server name");
            loginData.updateHosts.clear();
            break;
        }
    }

    logger->log("Received update hosts \"%s\" from login server.",
        updateHost.c_str());

    if (client->getState() == STATE_PRE_LOGIN)
        client->setState(STATE_LOGIN);
}

int LoginHandler::supportedOptionalActions() const
{
    return serverVersion >= 7 ? SetEmailOnRegister | SetGenderOnRegister
        : SetGenderOnRegister;
}

}  // namespace TmwAthena
