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

#include "being/being.h"

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
        SMSG_BUYINGSTORE_HIDE_BOARD,
        SMSG_BUYINGSTORE_ITEMS_LIST,
        SMSG_BUYINGSTORE_SELL_FAILED,
        SMSG_BUYINGSTORE_REPORT,
        SMSG_BUYINGSTORE_DELETE_ITEM,
        SMSG_BUYINGSTORE_SELLER_SELL_FAILED,
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

        case SMSG_BUYINGSTORE_HIDE_BOARD:
            processBuyingStoreHideBoard(msg);
            break;

        case SMSG_BUYINGSTORE_ITEMS_LIST:
            processBuyingStoreItemsList(msg);
            break;

        case SMSG_BUYINGSTORE_SELL_FAILED:
            processBuyingStoreSellFailed(msg);
            break;

        case SMSG_BUYINGSTORE_REPORT:
            processBuyingStoreReport(msg);
            break;

        case SMSG_BUYINGSTORE_DELETE_ITEM:
            processBuyingStoreDeleteItem(msg);
            break;

        case SMSG_BUYINGSTORE_SELLER_SELL_FAILED:
            processBuyingStoreSellerSellFailed(msg);
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

void BuyingStoreHandler::processBuyingStoreHideBoard(Net::MessageIn &msg)
{
    msg.readInt32("account id");
}

void BuyingStoreHandler::processBuyingStoreItemsList(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 16) / 9;
    msg.readInt32("account id");
    msg.readInt32("store id");
    msg.readInt32("money limit");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("price");
        msg.readInt16("amount");
        msg.readUInt8("item type");
        msg.readInt16("item id");
    }
}

void BuyingStoreHandler::processBuyingStoreSellFailed(Net::MessageIn &msg)
{
    const int16_t result = msg.readInt16("result");
    switch (result)
    {
        case 3:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELL_FAILED_MONEY_LIMIT);
            break;
        case 4:
            NotifyManager::notify(NotifyTypes::BUYING_STORE_SELL_FAILED_EMPTY);
            break;
        default:
            NotifyManager::notify(NotifyTypes::BUYING_STORE_SELL_FAILED);
            break;
    }
}

void BuyingStoreHandler::processBuyingStoreSellerSellFailed(Net::MessageIn
                                                            &msg)
{
    const int16_t result = msg.readInt16("result");
    msg.readInt16("item id");
    switch (result)
    {
        case 5:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELLER_SELL_FAILED_DEAL);
            break;
        case 6:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELLER_SELL_FAILED_AMOUNT);
            break;
        case 7:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELLER_SELL_FAILED_BALANCE);
            break;
        default:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELLER_SELL_FAILED);
            break;
    }
}

void BuyingStoreHandler::processBuyingStoreReport(Net::MessageIn &msg)
{
    msg.readInt16("item id");
    msg.readInt16("amount");
    msg.readInt32("money limit");
}

void BuyingStoreHandler::processBuyingStoreDeleteItem(Net::MessageIn &msg)
{
    msg.readInt16("item index");
    msg.readInt16("amount");
    msg.readInt32("price");
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

void BuyingStoreHandler::close() const
{
    createOutPacket(CMSG_BUYINGSTORE_CLOSE);
}

void BuyingStoreHandler::open(const Being *const being) const
{
    if (!being)
        return;
    createOutPacket(CMSG_BUYINGSTORE_OPEN);
    outMsg.writeInt32(being->getId(), "account id");
}

void BuyingStoreHandler::sell(const Being *const being,
                              const int storeId,
                              const int index,
                              const int itemId,
                              const int amount) const
{
    if (!being)
        return;

    createOutPacket(CMSG_BUYINGSTORE_SELL);
    outMsg.writeInt16(16, "len");
    outMsg.writeInt32(being->getId(), "account id");
    outMsg.writeInt32(storeId, "store id");
    outMsg.writeInt16(static_cast<int16_t>(index), "index");
    outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
}

}  // namespace EAthena
