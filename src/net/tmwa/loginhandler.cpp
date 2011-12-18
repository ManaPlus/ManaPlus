/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "configuration.h"

#include "net/messagein.h"

#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"

#include "utils/gettext.h"

#include "debug.h"

extern Net::LoginHandler *loginHandler;

namespace TmwAthena
{

extern ServerInfo charServer;

LoginHandler::LoginHandler()
{
    static const Uint16 _messages[] =
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
    switch (msg.getId())
    {
        case SMSG_CHAR_PASSWORD_RESPONSE:
            procecessCharPasswordResponse(msg);
            break;

        case SMSG_UPDATE_HOST:
            processUpdateHost(msg);
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

void LoginHandler::processServerVersion(Net::MessageIn &msg)
{
    char b1 = msg.readInt8(); // -1
    char b2 = msg.readInt8(); // E
    char b3 = msg.readInt8(); // V
    char b4 = msg.readInt8(); // L
    if (b1 == -1 && b2 == 'E' && b3 == 'V' && b4 == 'L')
    {
        unsigned int options = msg.readInt8();
        mRegistrationEnabled = options;
        msg.skip(2);    // 0 unused
        serverVersion = msg.readInt8();
    }
    else
    {
        unsigned int options = msg.readInt32();
        mRegistrationEnabled = options;
        serverVersion = 0;
    }

    // Leave this last
    mVersionResponse = true;
}

} // namespace TmwAthena
