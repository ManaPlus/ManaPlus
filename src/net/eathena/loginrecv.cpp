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

#include "net/eathena/loginrecv.h"

#include "client.h"
#include "configuration.h"

#include "being/being.h"

#include "fs/paths.h"

#include "gui/windows/logindialog.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/messagein.h"

#include "net/ea/loginrecv.h"

#include "net/eathena/updateprotocol.h"

#include "utils/gettext.h"
#include "utils/foreach.h"

#include "debug.h"

extern int packetVersion;
extern int packetsType;
extern bool packets_main;
extern bool packets_re;
extern bool packets_zero;
extern int evolPacketOffset;

namespace EAthena
{

extern ServerInfo charServer;

void LoginRecv::processLoginError2(Net::MessageIn &msg)
{
    const uint32_t code = msg.readInt32("error");
    msg.readString(20, "error message");
    logger->log("Login::error code: %u", code);

    switch (code)
    {
        case 0:
            // TRANSLATORS: error message
            errorMessage = _("Unregistered ID.");
            break;
        case 1:
            // TRANSLATORS: error message
            errorMessage = _("Wrong password.");
            LoginDialog::savedPassword.clear();
            break;
        case 2:
            // TRANSLATORS: error message
            errorMessage = _("Account expired.");
            break;
        case 3:
            // TRANSLATORS: error message
            errorMessage = _("Rejected from server.");
            break;
        case 4:
            // TRANSLATORS: error message
            errorMessage = _("You have been permanently banned from "
                              "the game. Please contact the GM team.");
            break;
        case 5:
            // TRANSLATORS: error message
            errorMessage = _("Client too old or wrong server type.\n"
                "Please update client on http://manaplus.org");
            break;
        case 6:
            // TRANSLATORS: error message
            errorMessage = strprintf(_("You have been temporarily "
                                        "banned from the game until "
                                        "%s.\nPlease contact the GM "
                                        "team via the forums."),
                                        msg.readString(20, "date").c_str());
            break;
        case 7:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("Server overpopulated.");
            break;
        case 9:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("This user name is already taken.");
            break;
        case 10:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("Wrong name.");
            break;
        case 11:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("Incorrect email.");
            break;
        case 99:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("Username permanently erased.");
            break;
        default:
            // TRANSLATORS: error message
            errorMessage = _("Unknown error.");
            UNIMPLEMENTEDPACKETFIELD(code);
            break;
    }
    client->setState(State::ERROR);
}

void LoginRecv::processUpdateHost2(Net::MessageIn &msg)
{
    const int len = msg.readInt16("len") - 4;
    const std::string updateHost = msg.readString(len, "host");

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

    if (client->getState() == State::PRE_LOGIN)
        client->setState(State::LOGIN);
}

void LoginRecv::processServerVersion(Net::MessageIn &msg)
{
    const int len = msg.readInt16("len");
    msg.readInt32("unused");
    serverVersion = msg.readInt32("server version");
    if (serverVersion > 0)
    {
        logger->log("Evol2 server version: %d", serverVersion);
        packetVersion = msg.readInt32("packet version");
        logger->log("Hercules packet version: %d", packetVersion);
        if (packetVersion == 20150000)
        {
            packetVersion = 20141022;
            logger->log("autofix Hercules packet version to: %d",
                packetVersion);
        }
        if (len >= 18)
        {
            evolPacketOffset = msg.readInt16("evol packet offset");
        }
        if (len >= 20)
        {
            packetsType = msg.readInt16("server packets type");
            packets_re = (packetsType & 1) != 0;
            packets_zero = (packetsType & 2) != 0;
            if (packets_re == false &&
                packets_zero == false)
            {
                packets_main = true;
            }
        }
    }
    else
    {
        logger->log("Hercules without version");
    }
    updateProtocol();
    client->setState(State::LOGIN);
}

void LoginRecv::processCondingKey(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int sz = msg.readInt16("len") - 4;
    msg.readString(sz, "coding key");
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

    int offset = 0;
    int serverLen = 0;
    if (msg.getVersion() >= 20170315)
    {
        offset = 47 + 17;
        serverLen = 32 + 128;
    }
    else
    {
        offset = 47;
        serverLen = 32;
    }

    const int worldCount = (msg.getLength() - offset) / serverLen;

    Ea::LoginRecv::mToken.session_ID1 = msg.readInt32("session id1");
    Ea::LoginRecv::mToken.account_ID = msg.readBeingId("accound id");
    Ea::LoginRecv::mToken.session_ID2 = msg.readInt32("session id2");
    msg.readInt32("old ip");
    loginData.lastLogin = msg.readString(24, "last login");
    msg.readInt16("unused");

    // reserve bits for future usage
    Ea::LoginRecv::mToken.sex = Being::intToGender(CAST_U8(
        msg.readUInt8("gender") & 3U));

    if (msg.getVersion() >= 20170315)
    {
        msg.readString(16, "twitter auth token");
        msg.readUInt8("twitter flag");
    }

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
        if (msg.getVersion() >= 20170315)
        {
            for (int f = 0; f < 32; f ++)
                msg.readInt32("unused2");
        }

        logger->log("Network: Server: %s (%s:%d)", world->name.c_str(),
            ipToString(world->address), world->port);

        Ea::LoginRecv::mWorlds.push_back(world);
    }
    client->setState(State::WORLD_SELECT);
}

void LoginRecv::processLoginOtp1(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int tokenLen = msg.readInt16("len") - 8;
    msg.readInt32("login flag");
    msg.readString(tokenLen, "token");
}

void LoginRecv::processLoginOtp2(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int tokenLen = msg.readInt16("len") - 14;
    msg.readInt32("login flag");
    msg.readString(6, "login flag2");
    msg.readString(tokenLen, "token");
}

void LoginRecv::processLoginOtp3(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int tokenLen = msg.readInt16("len") - 28;
    msg.readInt32("login flag");
    msg.readString(20, "login flag2");
    msg.readString(tokenLen, "token");
}

void LoginRecv::processMobileOtp(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readBeingId("accound id");
}

}  // namespace EAthena
