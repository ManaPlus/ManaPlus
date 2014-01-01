/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "net/net.h"

#include "net/eathena/loginhandler.h"
#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::GameHandler *gameHandler;

namespace EAthena
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
        SMSG_MAP_CHAR_ID,
        0
    };
    handledMessages = _messages;
    gameHandler = this;
}

void GameHandler::handleMessage(Net::MessageIn &msg)
{
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

        case SMSG_MAP_CHAR_ID:
            processMapCharId(msg);
            break;

        default:
            break;
    }
}

void GameHandler::mapLoadedEvent() const
{
    MessageOut outMsg(CMSG_MAP_LOADED);
}

void GameHandler::connect()
{
    if (!mNetwork)
        return;

    mNetwork->connect(mapServer);

    const Token &token =
            static_cast<LoginHandler*>(Net::getLoginHandler())->getToken();


    if (client->getState() == STATE_CONNECT_GAME)
    {
        // Change the player's ID to the account ID to match what eAthena uses
        if (player_node)
        {
            mCharID = player_node->getId();
            player_node->setId(token.account_ID);
        }
        else
        {
            mCharID = 0;
        }
    }

    // Send login infos
    MessageOut outMsg(CMSG_MAP_SERVER_CONNECT);
    outMsg.writeInt32(token.account_ID);
    outMsg.writeInt32(mCharID);
    outMsg.writeInt32(token.session_ID1);
    outMsg.writeInt32(token.session_ID2);
    outMsg.writeInt8(Being::genderToInt(token.sex));

/*
    if (player_node)
    {
        // Change the player's ID to the account ID to match what eAthena uses
        player_node->setId(token.account_ID);
    }
*/
    // We get 4 useless bytes before the real answer comes in (what are these?)
//    mNetwork->skip(4);
}

bool GameHandler::isConnected() const
{
    if (!mNetwork)
        return false;
    return mNetwork->isConnected();
}

void GameHandler::disconnect()
{
    if (mNetwork)
        mNetwork->disconnect();
}

void GameHandler::quit() const
{
    MessageOut outMsg(CMSG_CLIENT_QUIT);
}

void GameHandler::ping(const int tick) const
{
    MessageOut msg(CMSG_CLIENT_PING);
    msg.writeInt32(tick);
}

void GameHandler::disconnect2() const
{
    MessageOut outMsg(CMSG_CLIENT_DISCONNECT);
}

void GameHandler::processMapCharId(Net::MessageIn &msg) const
{
    msg.readInt32();  // char id
}

}  // namespace EAthena
