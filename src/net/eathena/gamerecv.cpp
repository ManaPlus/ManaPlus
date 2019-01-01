/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "net/eathena/gamerecv.h"

#include "client.h"
#include "logger.h"

#include "being/localplayer.h"

#include "net/messagein.h"

#include "net/eathena/network.h"

#include "debug.h"

namespace EAthena
{

extern ServerInfo mapServer;

void GameRecv::processMapAccountId(Net::MessageIn &msg)
{
    // ignored, because we already know local player account id.
    msg.readBeingId("account id");
}

void GameRecv::processMapLogin(Net::MessageIn &msg)
{
    unsigned char direction;
    uint16_t x;
    uint16_t y;
    msg.readInt32("start time");
    msg.readCoordinates(x, y, direction, "position");
    msg.readInt8("x size");
    msg.readInt8("y size");
    logger->log("Protocol: Player start position: "
        "(%d, %d), Direction: %d",
        x, y, direction);
    if (msg.getVersion() >= 20080102)
        msg.readInt16("font");
    if (msg.getVersion() >= 20141022 && msg.getVersion() < 20160330)
        msg.readUInt8("sex");

    mLastHost &= 0xffffff;

    Network *const network = Network::mInstance;
    if (network != nullptr)
        network->pauseDispatch();

    // Switch now or we'll have problems
    client->setState(State::GAME);
    if (localPlayer != nullptr)
        localPlayer->setTileCoords(x, y);
}

void GameRecv::processServerTick(Net::MessageIn &msg)
{
    // ignoring
    msg.readInt32("tick");
}

void GameRecv::processMapAuthRefuse(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("error");
}
}  // namespace EAthena
