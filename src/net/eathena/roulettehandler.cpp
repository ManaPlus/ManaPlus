/*
 *  The ManaPlus Client
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

#include "net/eathena/roulettehandler.h"

#include "logger.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::RouletteHandler *rouletteHandler;

namespace EAthena
{

RouletteHandler::RouletteHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_ROULETTE_INFO_ACK_TYPE,
        SMSG_ROULETTE_RECV_ITEM_ACK,
        0
    };
    handledMessages = _messages;
    rouletteHandler = this;
}

void RouletteHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_ROULETTE_INFO_ACK_TYPE:
            processRouletteInfoAckType(msg);
            break;

        case SMSG_ROULETTE_RECV_ITEM_ACK:
            processRouletteItemAck(msg);
            break;

        default:
            break;
    }
}

void RouletteHandler::processRouletteInfoAckType(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    const int count = (msg.readInt16("len") - 8) / 8;
    msg.readInt32("serial");

    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("row");
        msg.readInt16("position");
        msg.readInt16("item id");
        msg.readInt16("count");
    }
}

void RouletteHandler::processRouletteItemAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readUInt8("result");
    msg.readInt16("item id");
}

}  // namespace EAthena
