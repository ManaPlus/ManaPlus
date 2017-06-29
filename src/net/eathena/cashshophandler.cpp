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

#include "net/eathena/cashshophandler.h"

#include "net/eathena/cashshoprecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "utils/foreach.h"

#include "resources/item/shopitem.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

CashShopHandler::CashShopHandler() :
    Net::CashShopHandler()
{
    cashShopHandler = this;
    CashShopRecv::mBuyDialog = nullptr;
}

void CashShopHandler::buyItem(const int points,
                              const int itemId,
                              const ItemColor color A_UNUSED,
                              const int amount) const
{
    if (packetVersion < 20101124)
        return;
    createOutPacket(CMSG_NPC_CASH_SHOP_BUY);
    outMsg.writeInt16(10 + 4, "len");
    outMsg.writeInt32(points, "points");
    outMsg.writeInt16(1, "count");
    outMsg.writeInt16(CAST_S16(amount), "amount");
    outMsg.writeInt16(CAST_S16(itemId), "item id");
}

void CashShopHandler::buyItems(const int points,
                               const std::vector<ShopItem*> &items) const
{
    if (packetVersion < 20101124)
        return;

    int cnt = 0;
    const int pairSize = 4;

    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *it;
        const int usedQuantity = item->getUsedQuantity();
        const ItemTypeT type = item->getType();
        if (usedQuantity == 0)
            continue;
        if (type == ItemType::Weapon ||
            type == ItemType::Armor ||
            type == ItemType::PetEgg ||
            type == ItemType::PetArmor)
        {
            cnt += item->getUsedQuantity();
        }
        else
        {
            cnt ++;
        }
    }

    if (cnt > 100)
        return;

    createOutPacket(CMSG_NPC_CASH_SHOP_BUY);
    outMsg.writeInt16(CAST_S16(10 + pairSize * cnt), "len");
    outMsg.writeInt32(points, "points");
    outMsg.writeInt16(CAST_S16(cnt), "count");
    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        ShopItem *const item = *it;
        const int usedQuantity = item->getUsedQuantity();
        if (usedQuantity == 0)
            continue;
        item->increaseQuantity(usedQuantity);
        item->increaseUsedQuantity(-usedQuantity);
        item->update();
        const ItemTypeT type = item->getType();
        if (type == ItemType::Weapon ||
            type == ItemType::Armor ||
            type == ItemType::PetEgg ||
            type == ItemType::PetArmor)
        {
            for (int f = 0; f < usedQuantity; f ++)
            {
                outMsg.writeInt16(CAST_S16(1), "amount");
                outMsg.writeInt16(CAST_S16(item->getId()),
                    "item id");
            }
        }
        else
        {
            outMsg.writeInt16(CAST_S16(usedQuantity), "amount");
            outMsg.writeInt16(CAST_S16(item->getId()), "item id");
        }
    }
}

void CashShopHandler::close() const
{
    if (packetVersion < 20110718)
        return;
    createOutPacket(CMSG_NPC_CASH_SHOP_CLOSE);
}

void CashShopHandler::requestPoints() const
{
    if (packetVersion < 20110718)
        return;
    createOutPacket(CMSG_NPC_CASH_SHOP_OPEN);
}

void CashShopHandler::requestTab(const int tab) const
{
    if (packetVersion < 20110718)
        return;
    createOutPacket(CMSG_NPC_CASH_SHOP_REQUEST_TAB);
    outMsg.writeInt16(CAST_S16(tab), "tab");
}

void CashShopHandler::schedule() const
{
    if (packetVersion < 20110614)
        return;
    createOutPacket(CMSG_NPC_CASH_SHOP_SCHEDULE);
}

}  // namespace EAthena
