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

#include "net/tmwa/generalrecv.h"

#include "client.h"
#include "configuration.h"

#include "gui/windows/skilldialog.h"
#include "gui/windows/socialwindow.h"
#include "gui/windows/statuswindow.h"

#include "gui/widgets/tabs/chat/guildtab.h"
#include "gui/widgets/tabs/chat/partytab.h"

#include "net/tmwa/adminhandler.h"
#include "net/tmwa/beinghandler.h"
#include "net/tmwa/buysellhandler.h"
#include "net/tmwa/chathandler.h"
#include "net/tmwa/charserverhandler.h"
#include "net/tmwa/gamehandler.h"
#include "net/tmwa/guildhandler.h"
#include "net/tmwa/inventoryhandler.h"
#include "net/tmwa/itemhandler.h"
#include "net/tmwa/loginhandler.h"
#include "net/tmwa/network.h"
#include "net/tmwa/npchandler.h"
#include "net/tmwa/partyhandler.h"
#include "net/tmwa/pethandler.h"
#include "net/tmwa/playerhandler.h"
#include "net/tmwa/protocol.h"
#include "net/tmwa/serverfeatures.h"
#include "net/tmwa/tradehandler.h"
#include "net/tmwa/skillhandler.h"
#include "net/tmwa/questhandler.h"

#ifdef EATHENA_SUPPORT
#include "net/tmwa/auctionhandler.h"
#include "net/tmwa/bankhandler.h"
#include "net/tmwa/battlegroundhandler.h"
#include "net/tmwa/buyingstorehandler.h"
#include "net/tmwa/cashshophandler.h"
#include "net/tmwa/elementalhandler.h"
#include "net/tmwa/familyhandler.h"
#include "net/tmwa/friendshandler.h"
#include "net/tmwa/homunculushandler.h"
#include "net/tmwa/mailhandler.h"
#include "net/tmwa/maphandler.h"
#include "net/tmwa/markethandler.h"
#include "net/tmwa/mercenaryhandler.h"
#include "net/tmwa/roulettehandler.h"
#include "net/tmwa/searchstorehandler.h"
#include "net/tmwa/vendinghandler.h"
#endif

#include "resources/db/itemdbstat.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

namespace TmwAthena
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
            // TRANSLATORS: error message
            errorMessage = _("Authentication failed.");
            break;
        case 1:
            // TRANSLATORS: error message
            errorMessage = _("No servers available.");
            break;
        case 2:
            if (client->getState() == STATE_GAME)
            {
                // TRANSLATORS: error message
                errorMessage = _("Someone else is trying to use this "
                         "account.");
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
        case 8:
            // TRANSLATORS: error message
            errorMessage = _("Duplicated login.");
            break;
        default:
            // TRANSLATORS: error message
            errorMessage = _("Unknown connection error.");
            break;
    }
    client->setState(STATE_ERROR);
}

}  // namespace TmwAthena
