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

#include "notifymanager.h"
#include "shopitem.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h" 

#include "resources/notifytypes.h"

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
        SMSG_BUYINGSTORE_CREATE_FAILED,
        SMSG_BUYINGSTORE_OWN_ITEMS,
        SMSG_BUYINGSTORE_SHOW_BOARD,
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

        case SMSG_BUYINGSTORE_CREATE_FAILED:
            processBuyingStoreCreateFailed(msg);
            break;

        case SMSG_BUYINGSTORE_OWN_ITEMS:
            processBuyingStoreOwnItems(msg);
            break;

        case SMSG_BUYINGSTORE_SHOW_BOARD:
            processBuyingStoreShowBoard(msg);
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

void BuyingStoreHandler::processBuyingStoreCreateFailed(Net::MessageIn &msg)
{
    const int16_t result = msg.readInt16("result");
    const int weight = msg.readInt32("weight");
    switch (result)
    {
        case 0:
        default:
            NotifyManager::notify(NotifyTypes::BUYING_STORE_CREATE_FAILED);
            break;
        case 1:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_CREATE_FAILED_WEIGHT,
                weight);
            break;
        case 8:
            NotifyManager::notify(NotifyTypes::BUYING_STORE_CREATE_EMPTY);
            break;
    }
}

void BuyingStoreHandler::processBuyingStoreOwnItems(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 12) / 9;
    msg.readInt32("account id");
    msg.readInt32("money limit");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("price");
        msg.readInt16("amount");
        msg.readUInt8("item type");
        msg.readInt16("item id");
    }
}

void BuyingStoreHandler::processBuyingStoreShowBoard(Net::MessageIn &msg)
{
    msg.readInt32("account id");
    msg.readString(80, "board name");
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
