/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/eathena/tradehandler.h"

#include "item.h"

#include "being/playerinfo.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "net/ea/eaprotocol.h"

#include "debug.h"

extern Net::TradeHandler *tradeHandler;

namespace EAthena
{

TradeHandler::TradeHandler() :
    MessageHandler(),
    Ea::TradeHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_TRADE_REQUEST,
        SMSG_TRADE_RESPONSE,
        SMSG_TRADE_ITEM_ADD,
        SMSG_TRADE_ITEM_ADD_RESPONSE,
        SMSG_TRADE_OK,
        SMSG_TRADE_CANCEL,
        SMSG_TRADE_COMPLETE,
        0
    };
    handledMessages = _messages;
    tradeHandler = this;
}


void TradeHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_TRADE_REQUEST:
            processTradeRequest(msg);
            break;

        case SMSG_TRADE_RESPONSE:
            processTradeResponse(msg);
            break;

        case SMSG_TRADE_ITEM_ADD:
            processTradeItemAdd(msg);
            break;

        case SMSG_TRADE_ITEM_ADD_RESPONSE:
            processTradeItemAddResponse(msg);
            break;

        case SMSG_TRADE_OK:
            processTradeOk(msg);
            break;

        case SMSG_TRADE_CANCEL:
            processTradeCancel(msg);
            break;

        case SMSG_TRADE_COMPLETE:
            processTradeComplete(msg);
            break;

        default:
            break;
    }
}

void TradeHandler::request(const Being *const being) const
{
    if (!being)
        return;

    MessageOut outMsg(CMSG_TRADE_REQUEST);
    outMsg.writeInt32(being->getId());
}

void TradeHandler::respond(const bool accept) const
{
    if (!accept)
        PlayerInfo::setTrading(false);

    MessageOut outMsg(CMSG_TRADE_RESPONSE);
    outMsg.writeInt8(accept ? 3 : 4);
}

void TradeHandler::addItem(const Item *const item, const int amount) const
{
    if (!item)
        return;

    MessageOut outMsg(CMSG_TRADE_ITEM_ADD_REQUEST);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET));
    outMsg.writeInt32(amount);
}

void TradeHandler::setMoney(const int amount) const
{
    MessageOut outMsg(CMSG_TRADE_ITEM_ADD_REQUEST);
    outMsg.writeInt16(0);
    outMsg.writeInt32(amount);
}

void TradeHandler::confirm() const
{
    MessageOut outMsg(CMSG_TRADE_ADD_COMPLETE);
}

void TradeHandler::finish() const
{
    MessageOut outMsg(CMSG_TRADE_OK);
}

void TradeHandler::cancel() const
{
    MessageOut outMsg(CMSG_TRADE_CANCEL_REQUEST);
}

}  // namespace EAthena
