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

#include "shopitem.h"

#include "being/being.h"
#include "being/playerinfo.h"

#include "const/net/inventory.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"
#include "net/eathena/vendingrecv.h"

#include "debug.h"

extern Net::VendingHandler *vendingHandler;

namespace EAthena
{

VendingHandler::VendingHandler()
{
    vendingHandler = this;
    VendingRecv::mBuyDialog = nullptr;
}

void VendingHandler::close() const
{
    createOutPacket(CMSG_VENDING_CLOSE);
    PlayerInfo::enableVending(false);
}

void VendingHandler::open(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_VENDING_LIST_REQ);
    outMsg.writeBeingId(being->getId(), "account id");
}

void VendingHandler::buy(const Being *const being,
                         const int index,
                         const int amount) const
{
    if (!being)
        return;

    createOutPacket(CMSG_VENDING_BUY);
    outMsg.writeInt16(12, "len");
    outMsg.writeBeingId(being->getId(), "account id");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
    outMsg.writeInt16(static_cast<int16_t>(index), "index");
}

void VendingHandler::buy2(const Being *const being,
                          const int vendId,
                          const int index,
                          const int amount) const
{
    if (!being)
        return;

    createOutPacket(CMSG_VENDING_BUY2);
    outMsg.writeInt16(16, "len");
    outMsg.writeBeingId(being->getId(), "account id");
    outMsg.writeInt32(vendId, "vend id");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
    outMsg.writeInt16(static_cast<int16_t>(index), "index");
}

void VendingHandler::createShop(const std::string &name,
                                const bool flag,
                                std::vector<ShopItem*> &items) const
{
    createOutPacket(CMSG_VENDING_CREATE_SHOP);
    outMsg.writeInt16(static_cast<int16_t>(85 + items.size() * 8), "len");
    outMsg.writeString(name, 80, "shop name");
    outMsg.writeInt8(static_cast<int8_t>(flag ? 1 : 0), "flag");
    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *it;
        outMsg.writeInt16(static_cast<int16_t>(
            item->getInvIndex() + INVENTORY_OFFSET), "index");
        outMsg.writeInt16(static_cast<int16_t>(item->getQuantity()), "amount");
        outMsg.writeInt32(item->getPrice(), "price");
    }
}

}  // namespace EAthena
