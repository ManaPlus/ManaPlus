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

#include "net/eathena/loginrecv.h"

#include "client.h"

#include "gui/windows/logindialog.h"

#include "net/logindata.h"

#include "net/messagein.h"

#include "net/eathena/updateprotocol.h"

#include "utils/gettext.h"
#include "utils/paths.h"

#include "debug.h"

extern int packetVersion;

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
    msg.readInt16("len");
    msg.readInt32("unused");
    serverVersion = msg.readInt32("server version");
    if (serverVersion > 0)
    {
        logger->log("Evol2 server version: %d", serverVersion);
        if (serverVersion >= 8)
        {
            packetVersion = msg.readInt32("packet version");
            logger->log("Hercules packet version: %d", packetVersion);
        }
        else
        {
            packetVersion = 20150000;
            logger->log("Possible hercules packet version: %d", packetVersion);
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

}  // namespace EAthena
