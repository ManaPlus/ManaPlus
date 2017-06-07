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

#include "net/eathena/gamehandler.h"

#include "client.h"

#include "being/localplayer.h"

#include "net/serverfeatures.h"

#include "net/ea/token.h"

#include "net/ea/gamerecv.h"

#include "net/eathena/loginhandler.h"
#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/protocolout.h"

#include "debug.h"


extern int packetVersion;

namespace EAthena
{

extern ServerInfo mapServer;

GameHandler::GameHandler() :
    Ea::GameHandler()
{
    gameHandler = this;
}

void GameHandler::mapLoadedEvent() const
{
    createOutPacket(CMSG_MAP_LOADED);
}

void GameHandler::connect() const
{
    if (Network::mInstance == nullptr)
        return;

    Network::mInstance->connect(mapServer);

    const Token &token = static_cast<LoginHandler*>(loginHandler)->getToken();


    if (client->getState() == State::CONNECT_GAME)
    {
        // Change the player's ID to the account ID to match what eAthena uses
        if (localPlayer != nullptr)
        {
            Ea::GameRecv::mCharID = localPlayer->getId();
            localPlayer->setId(token.account_ID);
        }
        else
        {
            Ea::GameRecv::mCharID = BeingId_zero;
        }
    }

    // Send login infos
    createOutPacket(CMSG_MAP_SERVER_CONNECT);
    outMsg.writeBeingId(token.account_ID, "account id");
    outMsg.writeBeingId(Ea::GameRecv::mCharID, "char id");
    outMsg.writeInt32(token.session_ID1, "session key1");
    outMsg.writeInt32(0, "tick");
    outMsg.writeInt8(Being::genderToInt(token.sex), "sex");
    if (serverFeatures->haveMapServerVersion())
        loginHandler->sendVersion();

    // We get 4 useless bytes before the real answer comes in (what are these?)
//    Network::mInstance->skip(4);
}

bool GameHandler::isConnected() const
{
    if (Network::mInstance == nullptr)
        return false;
    return Network::mInstance->isConnected();
}

void GameHandler::disconnect() const
{
    if (Network::mInstance != nullptr)
        Network::mInstance->disconnect();
}

void GameHandler::quit() const
{
    createOutPacket(CMSG_CLIENT_QUIT);
}

void GameHandler::ping(const int tick) const
{
    createOutPacket(CMSG_MAP_PING);
    if (packetVersion >= 20101124)
    {
        // nothing here
    }
    else if (packetVersion >= 20080827)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20070212)
    {
        outMsg.writeInt16(0, "unknown");
    }
    else if (packetVersion >= 20070108)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20050719)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20050718)
    {
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20050628)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20050509)
    {
        outMsg.writeInt16(0, "unknown");
    }
    else if (packetVersion >= 20050110)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20041129)
    {
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20041025)
    {
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20041005)
    {
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20040920)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20040906)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20040809)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
        outMsg.writeInt8(0, "unknown");
    }
    else if (packetVersion >= 20040726)
    {
        outMsg.writeInt16(0, "unknown");
        outMsg.writeInt16(0, "unknown");
    }
    outMsg.writeInt32(tick, "tick");
}

void GameHandler::disconnect2() const
{
}

}  // namespace EAthena
