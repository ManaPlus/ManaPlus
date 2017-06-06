/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/tmwa/loginrecv.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"

#include "being/being.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/messagein.h"

#include "net/ea/loginrecv.h"

#include "net/tmwa/updateprotocol.h"

#include "utils/gettext.h"

#include "debug.h"

extern int packetVersion;

namespace TmwAthena
{

extern ServerInfo charServer;

enum ServerFlags
{
    FLAG_REGISTRATION = 1
};

void LoginRecv::processServerVersion(Net::MessageIn &msg)
{
    packetVersion = 0;
    const uint8_t b1 = msg.readUInt8("b1");  // -1
    const uint8_t b2 = msg.readUInt8("b2");
    const uint8_t b3 = msg.readUInt8("b3");
    msg.readUInt8("b4");
    if (b1 == 255)
    {   // old TMWA
        const unsigned int options = msg.readInt32("options");
        Ea::LoginRecv::mRegistrationEnabled =
            ((options & FLAG_REGISTRATION) != 0u);
        serverVersion = 0;
        tmwServerVersion = 0;
    }
    else if (b1 >= 0x0d)
    {   // new TMWA
        const unsigned int options = msg.readInt32("options");
        Ea::LoginRecv::mRegistrationEnabled =
            ((options & FLAG_REGISTRATION) != 0u);
        serverVersion = 0;
        tmwServerVersion = (b1 << 16) | (b2 << 8) | b3;
    }
    else
    {   // eAthena
        const unsigned int options = msg.readInt32("options");
        Ea::LoginRecv::mRegistrationEnabled =
            ((options & FLAG_REGISTRATION) != 0u);
        serverVersion = 0;
        tmwServerVersion = 0;
    }
    if (tmwServerVersion > 0)
        logger->log("Tmw server version: x%06x", tmwServerVersion);
    else
        logger->log("Server without version");
    updateProtocol();

    if (client->getState() != State::LOGIN)
        client->setState(State::LOGIN);

    // Leave this last
    Ea::LoginRecv::mVersionResponse = true;
}

void LoginRecv::processCharPasswordResponse(Net::MessageIn &msg)
{
    // 0: acc not found, 1: success, 2: password mismatch, 3: pass too short
    const uint8_t errMsg = msg.readUInt8("result code");
    // Successful pass change
    if (errMsg == 1)
    {
        client->setState(State::CHANGEPASSWORD_SUCCESS);
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
        client->setState(State::ACCOUNTCHANGE_ERROR);
    }
}

void LoginRecv::processLoginData(Net::MessageIn &msg)
{
    msg.readInt16("len");

    loginHandler->clearWorlds();

    const int worldCount = (msg.getLength() - 47) / 32;

    Ea::LoginRecv::mToken.session_ID1 = msg.readInt32("session id1");
    Ea::LoginRecv::mToken.account_ID = msg.readBeingId("accound id");
    Ea::LoginRecv::mToken.session_ID2 = msg.readInt32("session id2");
    msg.readInt32("old ip");
    loginData.lastLogin = msg.readString(24, "last login");
    msg.readInt16("unused");

    // reserve bits for future usage
    Ea::LoginRecv::mToken.sex = Being::intToGender(CAST_U8(
        msg.readUInt8("gender") & 3U));

    for (int i = 0; i < worldCount; i++)
    {
        WorldInfo *const world = new WorldInfo;

        world->address = msg.readInt32("ip address");
        world->port = msg.readInt16("port");
        world->name = msg.readString(20, "name");
        world->online_users = msg.readInt16("online number");
        config.setValue("updatehost", Ea::LoginRecv::mUpdateHost);
        world->updateHost = Ea::LoginRecv::mUpdateHost;
        msg.readInt16("maintenance");
        msg.readInt16("new");

        logger->log("Network: Server: %s (%s:%d)", world->name.c_str(),
            ipToString(world->address), world->port);

        Ea::LoginRecv::mWorlds.push_back(world);
    }
    client->setState(State::WORLD_SELECT);
}

}  // namespace TmwAthena
