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

#include "net/tmwa/messageout.h"
#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"

#include "utils/gettext.h"

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
            processCharPasswordResponse(msg);
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
    if (serverFeatures->haveServerVersion())
        createOutPacket(CMSG_SERVER_VERSION_REQUEST);
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
                                     const std::string &restrict email) const
{
    if (email.empty())
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
    else
    {
        createOutPacket(CMSG_LOGIN_REGISTER2);
        outMsg.writeInt32(0, "client version");
        outMsg.writeString(username, 24, "login");
        outMsg.writeStringNoLog(password, 24, "password");

        /*
         * eAthena calls the last byte "client version 2", but it isn't used at
         * at all. We're retasking it, as a bit mask:
         *  0 - can handle the 0x63 "update host" packet
         *  1 - defaults to the first char-server (instead of the last)
         */
        outMsg.writeInt8(0x03, "flags");
        outMsg.writeString(email, 24, "email");
    }
}

ServerInfo *LoginHandler::getCharServer() const
{
    return &charServer;
}

void LoginHandler::requestUpdateHosts()
{
    createOutPacket(CMSG_SEND_CLIENT_INFO);
    outMsg.writeInt8(CLIENT_PROTOCOL_VERSION, "protocol version");
    outMsg.writeInt8(0, "unused");
}

void LoginHandler::processServerVersion(Net::MessageIn &msg)
{
    const uint8_t b1 = msg.readUInt8("b1");  // -1
    const uint8_t b2 = msg.readUInt8("b2");  // E
    const uint8_t b3 = msg.readUInt8("b3");  // V
    const uint8_t b4 = msg.readUInt8("b4");  // L
    if (b1 == 255 && b2 == 'E' && b3 == 'V' && b4 == 'L')
    {
        const unsigned int options = msg.readUInt8("options");
        mRegistrationEnabled = options & FLAG_REGISTRATION;
        msg.readInt16("unused");
        serverVersion = msg.readUInt8("server version");
        tmwServerVersion = 0;
        if (serverVersion >= 5)
            requestUpdateHosts();
    }
    else if (b1 == 255)
    {   // old TMWA
        const unsigned int options = msg.readInt32("options");
        mRegistrationEnabled = options & FLAG_REGISTRATION;
        serverVersion = 0;
        tmwServerVersion = 0;
    }
    else if (b1 >= 0x0d)
    {   // new TMWA
        const unsigned int options = msg.readInt32("options");
        mRegistrationEnabled = options & FLAG_REGISTRATION;
        serverVersion = 0;
        tmwServerVersion = (b1 << 16) | (b2 << 8) | b3;
    }
    else
    {   // eAthena
        const unsigned int options = msg.readInt32("options");
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

int LoginHandler::supportedOptionalActions() const
{
    return serverFeatures->haveEmailOnRegister()
        ? Net::RegistrationOptions::SetEmailOnRegister
        | Net::RegistrationOptions::SetGenderOnRegister
        : Net::RegistrationOptions::SetGenderOnRegister;
}

void LoginHandler::sendVersion() const
{
}

void LoginHandler::processCharPasswordResponse(Net::MessageIn &msg)
{
    // 0: acc not found, 1: success, 2: password mismatch, 3: pass too short
    const uint8_t errMsg = msg.readUInt8("result code");
    // Successful pass change
    if (errMsg == 1)
    {
        client->setState(STATE_CHANGEPASSWORD_SUCCESS);
    }
    // pass change failed
    else
    {
        switch (errMsg)
        {
            case 0:
                errorMessage =
                    // TRANSLATORS: error message
                    _("Account was not found. Please re-login.");
                break;
            case 2:
                // TRANSLATORS: error message
                errorMessage = _("Old password incorrect.");
                break;
            case 3:
                // TRANSLATORS: error message
                errorMessage = _("New password too short.");
                break;
            default:
                // TRANSLATORS: error message
                errorMessage = _("Unknown error.");
                break;
        }
        client->setState(STATE_ACCOUNTCHANGE_ERROR);
    }
}

void LoginHandler::ping() const
{
}

}  // namespace TmwAthena
