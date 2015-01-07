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

#include "net/eathena/vendinghandler.h"

#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::VendingHandler *vendingHandler;

namespace EAthena
{

VendingHandler::VendingHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_VENDING_OPEN_REQ,
        SMSG_VENDING_SHOW_BOARD,
        SMSG_VENDING_HIDE_BOARD,
        0
    };
    handledMessages = _messages;
    vendingHandler = this;
}

void VendingHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_VENDING_OPEN_REQ:
            processOpenReq(msg);
            break;

        case SMSG_VENDING_SHOW_BOARD:
            processShowBoard(msg);
            break;

        case SMSG_VENDING_HIDE_BOARD:
            processHideBoard(msg);
            break;

        default:
            break;
    }
}

void VendingHandler::processOpenReq(Net::MessageIn &msg)
{
    msg.readInt16("slots allowed");
}

void VendingHandler::processShowBoard(Net::MessageIn &msg)
{
    msg.readInt32("owner id");
    msg.readString(80, "shop name");
}

void VendingHandler::processHideBoard(Net::MessageIn &msg)
{
    msg.readInt32("owner id");
}

}  // namespace EAthena
