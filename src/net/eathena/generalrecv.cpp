/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "net/eathena/generalrecv.h"

#include "client.h"
#include "logger.h"

#include "net/messagein.h"

#include "utils/gettext.h"

#include "debug.h"

namespace EAthena
{

ServerInfo charServer;
ServerInfo mapServer;

void GeneralRecv::processConnectionProblem(Net::MessageIn &msg)
{
    const uint8_t code = msg.readUInt8("flag");
    logger->log("Connection problem: %u", static_cast<unsigned int>(code));

    switch (code)
    {
        case 0:
            errorMessage = _("Authentication failed.");
            break;
        case 1:
            errorMessage = _("No servers available.");
            break;
        case 2:
            if (client->getState() == STATE_GAME)
            {
                errorMessage = _("Someone else is trying to use "
                    "this account.");
            }
            else
            {
                errorMessage = _("This account is already logged in.");
            }
            break;
        case 3:
            errorMessage = _("Speed hack detected.");
            break;
        case 4:
            errorMessage = _("Server full.");
            break;
        case 5:
            errorMessage = _("Sorry, you are underaged.");
            break;
        case 8:
            errorMessage = _("Duplicated login.");
            break;
        case 9:
            errorMessage = _("To many connections from same ip.");
            break;
        case 10:
            errorMessage = _("Not paid for this time.");
            break;
        case 11:
            errorMessage = _("Pay suspended.");
            break;
        case 12:
            errorMessage = _("Pay changed.");
            break;
        case 13:
            errorMessage = _("Pay wrong ip.");
            break;
        case 14:
            errorMessage = _("Pay game room.");
            break;
        case 15:
            errorMessage = _("Disconnect forced by GM.");
            break;
        case 16:
        case 17:
            errorMessage = _("Ban japan refuse.");
            break;
        case 18:
            errorMessage = _("Remained other account.");
            break;
        case 100:
            errorMessage = _("Ip unfair.");
            break;
        case 101:
            errorMessage = _("Ip count all.");
            break;
        case 102:
            errorMessage = _("Ip count.");
            break;
        case 103:
        case 104:
            errorMessage = _("Memory.");
            break;
        case 105:
            errorMessage = _("Han valid.");
            break;
        case 106:
            errorMessage = _("Ip limited access.");
            break;
        case 107:
            errorMessage = _("Over characters list.");
            break;
        case 108:
            errorMessage = _("Ip blocked.");
            break;
        case 109:
            errorMessage = _("Invalid password count.");
            break;
        case 110:
            errorMessage = _("Not allowed race.");
            break;
        default:
            errorMessage = _("Unknown connection error.");
            break;
    }
    client->setState(STATE_ERROR);
}

void GeneralRecv::processMapNotFound(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 4;
    msg.readString(sz, "map name?");
    errorMessage = _("Map not found");
    client->setState(STATE_ERROR);
}

}  // namespace EAthena
