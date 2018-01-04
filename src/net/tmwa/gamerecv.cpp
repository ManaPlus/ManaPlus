/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "net/tmwa/gamerecv.h"

#include "client.h"
#include "logger.h"

#include "being/localplayer.h"

#include "net/messagein.h"

#include "net/tmwa/network.h"

#include "debug.h"

namespace TmwAthena
{

extern ServerInfo mapServer;

void GameRecv::processMapLogin(Net::MessageIn &msg)
{
    unsigned char direction;
    uint16_t x, y;
    msg.readInt32("tick");
    msg.readCoordinates(x, y, direction, "position");
    msg.readInt16("unknown?");
    logger->log("Protocol: Player start position: (%d, %d),"
                " Direction: %d", x, y, direction);

    mLastHost &= 0xffffff;

    Network *const network = Network::mInstance;
    if (network != nullptr)
        network->pauseDispatch();

    // Switch now or we'll have problems
    client->setState(State::GAME);
    if (localPlayer != nullptr)
        localPlayer->setTileCoords(x, y);
}

}  // namespace TmwAthena
