/*
 *  The ManaPlus Client
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

#include "net/eathena/friendshandler.h"

#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::FriendsHandler *friendsHandler;

namespace EAthena
{

FriendsHandler::FriendsHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_FRIENDS_PLAYER_ONLINE,
        SMSG_FRIENDS_LIST,
        0
    };
    handledMessages = _messages;
    friendsHandler = this;
}

void FriendsHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_FRIENDS_PLAYER_ONLINE:
            processPlayerOnline(msg);
            break;

        case SMSG_FRIENDS_LIST:
            processFriendsList(msg);
            break;

        default:
            break;
    }
}

void FriendsHandler::processPlayerOnline(Net::MessageIn &msg)
{
    msg.readInt32("account id");
    msg.readInt32("char id");
    msg.readUInt8("flag");  // 0 - online, 1 - offline
}

void FriendsHandler::processFriendsList(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("size") - 4) / 32;
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("account id");
        msg.readInt32("char id");
        msg.readString(24, "name");
    }
}

}  // namespace EAthena
