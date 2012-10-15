/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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
#include "configuration.h"
#include "logger.h"

#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"

#include "utils/gettext.h"
#include "utils/paths.h"

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
}

void LoginHandler::connect()
{
    if (!mNetwork)
        return;

    mNetwork->connect(mServer);
    MessageOut outMsg(CMSG_SERVER_VERSION_REQUEST);
}

bool LoginHandler::isConnected()
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

void LoginHandler::changePassword(const std::string &username A_UNUSED,
                                  const std::string &oldPassword,
                                  const std::string &newPassword)
{
    MessageOut outMsg(CMSG_CHAR_PASSWORD_CHANGE);
    outMsg.writeStringNoLog(oldPassword, 24);
    outMsg.writeStringNoLog(newPassword, 24);
}

void LoginHandler::sendLoginRegister(const std::string &username,
                                     const std::string &password)
{
    MessageOut outMsg(0x0064);
    outMsg.writeInt32(0); // client version
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

ServerInfo *LoginHandler::getCharServer()
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
    const uint8_t b1 = msg.readInt8(); // -1
    const uint8_t b2 = msg.readInt8(); // E
    const uint8_t b3 = msg.readInt8(); // V
    const uint8_t b4 = msg.readInt8(); // L
    if (b1 == 255 && b2 == 'E' && b3 == 'V' && b4 == 'L')
    {
        const unsigned int options = msg.readInt8();
        mRegistrationEnabled = options;
        msg.skip(2);    // 0 unused
        serverVersion = msg.readInt8();
        if (serverVersion >= 5)
            requestUpdateHosts();
    }
    else
    {
        const unsigned int options = msg.readInt32();
        mRegistrationEnabled = options;
        serverVersion = 0;
    }
    logger->log("Server version: %d", serverVersion);
    if (serverVersion < 5)
    {
        if (Client::getState() != STATE_LOGIN)
            Client::setState(STATE_LOGIN);
    }

    // Leave this last
    mVersionResponse = true;
}

void LoginHandler::processUpdateHost2(Net::MessageIn &msg)
{
    int len;

    len = msg.readInt16() - 4;
    std::string updateHost = msg.readString(len);

    splitToStringVector(loginData.updateHosts, updateHost, '|');
    for (StringVectIter it = loginData.updateHosts.begin(),
         it_end = loginData.updateHosts.end(); it != it_end; ++ it)
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

    if (Client::getState() == STATE_PRE_LOGIN)
        Client::setState(STATE_LOGIN);
}

} // namespace TmwAthena
