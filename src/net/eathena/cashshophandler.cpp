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

#include "net/eathena/cashshophandler.h"

#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::CashShopHandler *cashShopHandler;

namespace EAthena
{

CashShopHandler::CashShopHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_NPC_CASH_SHOP_OPEN,
        0
    };
    handledMessages = _messages;
    cashShopHandler = this;
}

void CashShopHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_NPC_CASH_SHOP_OPEN:
            processCashShopOpen(msg);
            break;

        default:
            break;
    }
}

void CashShopHandler::processCashShopOpen(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 12) / 11;
    msg.readInt32("cash points");
    msg.readInt32("kafra points");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("price");
        msg.readInt32("discount price");
        msg.readUInt8("item type");
        msg.readInt16("item id");
    }
}

}  // namespace EAthena
