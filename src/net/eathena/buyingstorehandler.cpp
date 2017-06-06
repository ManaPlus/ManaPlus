/*
 *  The ManaPlus Client
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

#include "net/eathena/buyingstorehandler.h"

#include "being/being.h"
#include "being/playerinfo.h"

#include "const/net/inventory.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "resources/item/shopitem.h"

#include "debug.h"

extern Net::BuyingStoreHandler *buyingStoreHandler;
extern int packetVersion;

namespace EAthena
{

BuyingStoreHandler::BuyingStoreHandler()
{
    buyingStoreHandler = this;
}

void BuyingStoreHandler::create(const std::string &name,
                                const int maxMoney,
                                const bool flag,
                                const std::vector<ShopItem*> &items) const
{
    if (packetVersion < 20100303)
        return;
    createOutPacket(CMSG_BUYINGSTORE_CREATE);
    outMsg.writeInt16(CAST_S16(89 + items.size() * 8), "len");
    outMsg.writeInt32(maxMoney, "limit money");
    outMsg.writeInt8(static_cast<int8_t>(flag), "flag");
    outMsg.writeString(name, 80, "store name");
    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *it;
        outMsg.writeInt16(CAST_S16(item->getId()), "item id");
        outMsg.writeInt16(CAST_S16(item->getQuantity()), "amount");
        outMsg.writeInt32(item->getPrice(), "price");
    }
}

void BuyingStoreHandler::close() const
{
    if (packetVersion < 20100420)
        return;
    createOutPacket(CMSG_BUYINGSTORE_CLOSE);
    PlayerInfo::enableVending(false);
}

void BuyingStoreHandler::open(const Being *const being) const
{
    if (being == nullptr)
        return;
    if (packetVersion < 20100420)
        return;
    createOutPacket(CMSG_BUYINGSTORE_OPEN);
    outMsg.writeBeingId(being->getId(), "account id");
}

void BuyingStoreHandler::sell(const Being *const being,
                              const int storeId,
                              const Item *const item,
                              const int amount) const
{
    if ((being == nullptr) || (item == nullptr))
        return;
    if (packetVersion < 20100420)
        return;

    createOutPacket(CMSG_BUYINGSTORE_SELL);
    outMsg.writeInt16(18, "len");
    outMsg.writeBeingId(being->getId(), "account id");
    outMsg.writeInt32(storeId, "store id");
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET),
        "index");
    outMsg.writeInt16(CAST_S16(item->getId()), "item id");
    outMsg.writeInt16(CAST_S16(amount), "amount");
}

}  // namespace EAthena
