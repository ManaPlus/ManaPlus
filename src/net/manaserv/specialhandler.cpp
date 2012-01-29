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

#include "net/manaserv/specialhandler.h"

#include "net/manaserv/connection.h"
#include "net/manaserv/messagein.h"
#include "net/manaserv/messageout.h"
#include "net/manaserv/protocol.h"

#include "debug.h"

extern Net::SpecialHandler *specialHandler;

namespace ManaServ
{

extern Connection *gameServerConnection;

SpecialHandler::SpecialHandler()
{
    specialHandler = this;
}

void SpecialHandler::handleMessage(Net::MessageIn &msg A_UNUSED)
{
    // TODO
}

void SpecialHandler::use(int id)
{
    MessageOut msg(PGMSG_USE_SPECIAL);
    msg.writeInt8(id);
    gameServerConnection->send(msg);
}

void SpecialHandler::useBeing(int id A_UNUSED, int level A_UNUSED,
                              int beingId A_UNUSED)
{
    // TODO
}

void SpecialHandler::usePos(int id A_UNUSED, int level A_UNUSED, int x A_UNUSED,
                            int y A_UNUSED)
{
    // TODO
}

void SpecialHandler::useMap(int id A_UNUSED, const std::string &map A_UNUSED)
{
    // TODO
}

} // namespace ManaServ
