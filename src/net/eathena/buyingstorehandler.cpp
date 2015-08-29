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

#include "being/being.h"
#include "being/playerinfo.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/buyingstorerecv.h"
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
            BuyingStoreRecv::processBuyingStoreOpen(msg);
            break;

        case SMSG_BUYINGSTORE_CREATE_FAILED:
            BuyingStoreRecv::processBuyingStoreCreateFailed(msg);
            break;

        case SMSG_BUYINGSTORE_OWN_ITEMS:
            BuyingStoreRecv::processBuyingStoreOwnItems(msg);
            break;

        case SMSG_BUYINGSTORE_SHOW_BOARD:
            BuyingStoreRecv::processBuyingStoreShowBoard(msg);
            break;

        case SMSG_BUYINGSTORE_HIDE_BOARD:
            BuyingStoreRecv::processBuyingStoreHideBoard(msg);
            break;

        case SMSG_BUYINGSTORE_ITEMS_LIST:
            BuyingStoreRecv::processBuyingStoreItemsList(msg);
            break;

        case SMSG_BUYINGSTORE_SELL_FAILED:
            BuyingStoreRecv::processBuyingStoreSellFailed(msg);
            break;

        case SMSG_BUYINGSTORE_REPORT:
            BuyingStoreRecv::processBuyingStoreReport(msg);
            break;

        case SMSG_BUYINGSTORE_DELETE_ITEM:
            BuyingStoreRecv::processBuyingStoreDeleteItem(msg);
            break;

        case SMSG_BUYINGSTORE_SELLER_SELL_FAILED:
            BuyingStoreRecv::processBuyingStoreSellerSellFailed(msg);
            break;

        default:
            break;
    }
}

void BuyingStoreHandler::create(const std::string &name,
                                const int maxMoney,
                                const bool flag,
                                std::vector<ShopItem*> &items) const
{
    createOutPacket(CMSG_BUYINGSTORE_CREATE);
    outMsg.writeInt16(static_cast<int16_t>(89 + items.size() * 8), "len");
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
    PlayerInfo::enableVending(false);
}

void BuyingStoreHandler::open(const Being *const being) const
{
    if (!being)
        return;
    createOutPacket(CMSG_BUYINGSTORE_OPEN);
    outMsg.writeBeingId(being->getId(), "account id");
}

void BuyingStoreHandler::sell(const Being *const being,
                              const int storeId,
                              const Item *const item,
                              const int amount) const
{
    if (!being || !item)
        return;

    createOutPacket(CMSG_BUYINGSTORE_SELL);
    outMsg.writeInt16(18, "len");
    outMsg.writeBeingId(being->getId(), "account id");
    outMsg.writeInt32(storeId, "store id");
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET),
        "index");
    outMsg.writeInt16(static_cast<int16_t>(item->getId()), "item id");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
}

}  // namespace EAthena
