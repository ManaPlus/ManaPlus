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

#include "net/eathena/buyingstorehandler.h"

#include "shopitem.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h" 

#include "debug.h"

extern Net::BuyingStoreHandler *buyingStoreHandler;

namespace EAthena
{

BuyingStoreHandler::BuyingStoreHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_BUYINGSTORE_OPEN,
        0
    };
    handledMessages = _messages;
    buyingStoreHandler = this;
}

void BuyingStoreHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_BUYINGSTORE_OPEN:
            processBuyingStoreOpen(msg);
            break;

        default:
            break;
    }
}

void BuyingStoreHandler::processBuyingStoreOpen(Net::MessageIn &msg)
{
    // +++ need create store dialog
    msg.readUInt8("slots");
}

void BuyingStoreHandler::create(const std::string &name,
                                const int maxMoney,
                                const bool flag,
                                std::vector<ShopItem*> &items) const
{
    createOutPacket(CMSG_BUYINGSTORE_CREATE);
    outMsg.writeInt32(maxMoney, "limit money");
    outMsg.writeInt8(flag, "flag");
    outMsg.writeString(name, 80, "store name");
    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *it;
        outMsg.writeInt16(static_cast<int16_t>(item->getId()), "item id");
        outMsg.writeInt16(static_cast<int16_t>(item->getQuantity()), "amount");
        outMsg.writeInt32(item->getPrice(), "price");
    }
}

}  // namespace EAthena
