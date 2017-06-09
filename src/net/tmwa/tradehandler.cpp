/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/tmwa/tradehandler.h"

#include "being/playerinfo.h"

#include "const/net/inventory.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"

#include "resources/item/item.h"

#include "debug.h"

extern std::string tradePartnerName;

namespace TmwAthena
{

TradeHandler::TradeHandler() :
    Ea::TradeHandler()
{
    tradeHandler = this;
}

void TradeHandler::request(const Being *const being) const
{
    if (being == nullptr)
        return;

    createOutPacket(CMSG_TRADE_REQUEST);
    outMsg.writeBeingId(being->getId(), "player id");
}

void TradeHandler::respond(const bool accept) const
{
    if (!accept)
        PlayerInfo::setTrading(Trading_false);

    createOutPacket(CMSG_TRADE_RESPONSE);
    outMsg.writeInt8(CAST_S8(accept ? 3 : 4), "accept");
}

void TradeHandler::addItem(const Item *const item, const int amount) const
{
    if (item == nullptr)
        return;

    createOutPacket(CMSG_TRADE_ITEM_ADD_REQUEST);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt32(amount, "amount");
}

void TradeHandler::setMoney(const int amount) const
{
    createOutPacket(CMSG_TRADE_ITEM_ADD_REQUEST);
    outMsg.writeInt16(0, "index");
    outMsg.writeInt32(amount, "amount");
}

void TradeHandler::confirm() const
{
    createOutPacket(CMSG_TRADE_ADD_COMPLETE);
}

void TradeHandler::finish() const
{
    createOutPacket(CMSG_TRADE_OK);
}

void TradeHandler::cancel() const
{
    createOutPacket(CMSG_TRADE_CANCEL_REQUEST);
}

}  // namespace TmwAthena
