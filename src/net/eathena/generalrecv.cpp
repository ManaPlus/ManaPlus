/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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
    logger->log("Connection problem: %u", CAST_U32(code));

    switch (code)
    {
        case 0:
            // TRANSLATORS: error message
            errorMessage = _("Authentication failed.");
            break;
        case 1:
            // TRANSLATORS: error message
            errorMessage = _("No servers available.");
            break;
        case 2:
            if (client->getState() == State::GAME)
            {
                // TRANSLATORS: error message
                errorMessage = _("Someone else is trying to use "
                    "this account.");
            }
            else
            {
                // TRANSLATORS: error message
                errorMessage = _("This account is already logged in.");
            }
            break;
        case 3:
            // TRANSLATORS: error message
            errorMessage = _("Speed hack detected.");
            break;
        case 4:
            // TRANSLATORS: error message
            errorMessage = _("Server full.");
            break;
        case 5:
            // TRANSLATORS: error message
            errorMessage = _("Sorry, you are underaged.");
            break;
        case 8:
            // TRANSLATORS: error message
            errorMessage = _("Duplicated login.");
            break;
        case 9:
            // TRANSLATORS: error message
            errorMessage = _("To many connections from same ip.");
            break;
        case 10:
            // TRANSLATORS: error message
            errorMessage = _("Not paid for this time.");
            break;
        case 11:
            // TRANSLATORS: error message
            errorMessage = _("Pay suspended.");
            break;
        case 12:
            // TRANSLATORS: error message
            errorMessage = _("Pay changed.");
            break;
        case 13:
            // TRANSLATORS: error message
            errorMessage = _("Pay wrong ip.");
            break;
        case 14:
            // TRANSLATORS: error message
            errorMessage = _("Pay game room.");
            break;
        case 15:
            // TRANSLATORS: error message
            errorMessage = _("Disconnect forced by GM.");
            break;
        case 16:
        case 17:
            // TRANSLATORS: error message
            errorMessage = _("Ban japan refuse.");
            break;
        case 18:
            // TRANSLATORS: error message
            errorMessage = _("Remained other account.");
            break;
        case 100:
            // TRANSLATORS: error message
            errorMessage = _("Ip unfair.");
            break;
        case 101:
            // TRANSLATORS: error message
            errorMessage = _("Ip count all.");
            break;
        case 102:
            // TRANSLATORS: error message
            errorMessage = _("Ip count.");
            break;
        case 103:
        case 104:
            // TRANSLATORS: error message
            errorMessage = _("Memory.");
            break;
        case 105:
            // TRANSLATORS: error message
            errorMessage = _("Han valid.");
            break;
        case 106:
            // TRANSLATORS: error message
            errorMessage = _("Ip limited access.");
            break;
        case 107:
            // TRANSLATORS: error message
            errorMessage = _("Over characters list.");
            break;
        case 108:
            // TRANSLATORS: error message
            errorMessage = _("Ip blocked.");
            break;
        case 109:
            // TRANSLATORS: error message
            errorMessage = _("Invalid password count.");
            break;
        case 110:
            // TRANSLATORS: error message
            errorMessage = _("Not allowed race.");
            break;
        default:
            // TRANSLATORS: error message
            errorMessage = _("Unknown connection error.");
            break;
    }
    client->setState(State::ERROR);
}

void GeneralRecv::processMapNotFound(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 4;
    msg.readString(sz, "map name?");
    // TRANSLATORS: error message
    errorMessage = _("Map not found");
    client->setState(State::ERROR);
}

}  // namespace EAthena
