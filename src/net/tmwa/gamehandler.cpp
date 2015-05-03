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

#include "net/tmwa/gamehandler.h"

#include "client.h"

#include "being/localplayer.h"

#include "net/tmwa/loginhandler.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/network.h"
#include "net/tmwa/protocol.h"

#include "debug.h"

extern Net::GameHandler *gameHandler;

namespace TmwAthena
{

extern ServerInfo mapServer;

GameHandler::GameHandler() :
    MessageHandler(),
    Ea::GameHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_MAP_LOGIN_SUCCESS,
        SMSG_SERVER_PING,
        SMSG_WHO_ANSWER,
        SMSG_CHAR_SWITCH_RESPONSE,
        SMSG_MAP_QUIT_RESPONSE,
        0
    };
    handledMessages = _messages;
    gameHandler = this;
}

void GameHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("GameHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_MAP_LOGIN_SUCCESS:
            processMapLogin(msg);
            break;

        case SMSG_SERVER_PING:
            // We ignore this for now
            // int tick = msg.readInt32()
            break;

        case SMSG_WHO_ANSWER:
            processWhoAnswer(msg);
            break;

        case SMSG_CHAR_SWITCH_RESPONSE:
            processCharSwitchResponse(msg);
            break;

        case SMSG_MAP_QUIT_RESPONSE:
            processMapQuitResponse(msg);
            break;

        default:
            break;
    }
    BLOCK_END("GameHandler::handleMessage")
}

void GameHandler::mapLoadedEvent() const
{
    createOutPacket(CMSG_MAP_LOADED);
}

void GameHandler::connect()
{
    if (!mNetwork)
        return;

    BLOCK_START("GameHandler::connect")
    mNetwork->connect(mapServer);
    const Token &token = static_cast<LoginHandler*>(loginHandler)->getToken();

    if (client->getState() == STATE_CONNECT_GAME)
    {
        // Change the player's ID to the account ID to match what eAthena uses
        if (localPlayer)
        {
            mCharID = localPlayer->getId();
            localPlayer->setId(token.account_ID);
        }
        else
        {
            mCharID = 0;
        }
    }

    // Send login infos
    createOutPacket(CMSG_MAP_SERVER_CONNECT);
    outMsg.writeInt32(token.account_ID, "account id");
    outMsg.writeInt32(mCharID, "char id");
    outMsg.writeInt32(token.session_ID1, "session id1");
    outMsg.writeInt32(token.session_ID2, "session id2");
    outMsg.writeInt8(Being::genderToInt(token.sex), "gender");

/*
    if (localPlayer)
    {
        // Change the player's ID to the account ID to match what eAthena uses
        localPlayer->setId(token.account_ID);
    }
*/
    // We get 4 useless bytes before the real answer comes in (what are these?)
    mNetwork->skip(4);
    BLOCK_END("GameHandler::connect")
}

bool GameHandler::isConnected() const
{
    if (!mNetwork)
        return false;
    return mNetwork->isConnected();
}

void GameHandler::disconnect()
{
    BLOCK_START("GameHandler::disconnect")
    if (mNetwork)
        mNetwork->disconnect();
    BLOCK_END("GameHandler::disconnect")
}

void GameHandler::quit() const
{
    createOutPacket(CMSG_CLIENT_QUIT);
}

void GameHandler::ping(const int tick) const
{
    createOutPacket(CMSG_MAP_PING);
    outMsg.writeInt32(tick, "tick");
}

void GameHandler::disconnect2() const
{
    createOutPacket(CMSG_CLIENT_DISCONNECT);
}

void GameHandler::processMapLogin(Net::MessageIn &msg)
{
    unsigned char direction;
    uint16_t x, y;
    msg.readInt32("tick");
    msg.readCoordinates(x, y, direction, "position");
    msg.readInt16("unknown?");
    logger->log("Protocol: Player start position: (%d, %d),"
                " Direction: %d", x, y, direction);

    mLastHost &= 0xffffff;

    GameHandler *const g = static_cast<GameHandler*>(gameHandler);
    if (g)
    {
        Network *const network = g->mNetwork;
        if (network)
            network->pauseDispatch();
    }

    // Switch now or we'll have problems
    client->setState(STATE_GAME);
    if (localPlayer)
        localPlayer->setTileCoords(x, y);
}

}  // namespace TmwAthena
