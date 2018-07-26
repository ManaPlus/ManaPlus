/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2018  The ManaPlus Developers
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

#include "net/eathena/markethandler.h"

#include "net/eathena/marketrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "utils/foreach.h"

#include "resources/item/shopitem.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

MarketHandler::MarketHandler() :
    Net::MarketHandler()
{
    marketHandler = this;
    MarketRecv::mBuyDialog = nullptr;
}

MarketHandler::~MarketHandler()
{
    marketHandler = nullptr;
}

void MarketHandler::close() const
{
    if (packetVersion < 20131218)
        return;

    createOutPacket(CMSG_NPC_MARKET_CLOSE);
}

void MarketHandler::buyItem(const int itemId,
                            const ItemTypeT type,
                            const ItemColor color A_UNUSED,
                            const int amount) const
{
    if (packetVersion < 20131218)
        return;
    const bool nonStack = type == ItemType::Weapon ||
        type == ItemType::Armor ||
        type == ItemType::PetEgg ||
        type == ItemType::PetArmor;
    int cnt = nonStack ? amount : 1;
    const int amount2 = nonStack ? 1 : amount;
    if (cnt > 100)
        cnt = 100;

    createOutPacket(CMSG_NPC_MARKET_BUY);
    outMsg.writeInt16(CAST_S16(4 + 6 * cnt), "len");
    for (int f = 0; f < cnt; f ++)
    {
        outMsg.writeItemId(itemId, "item id");
        outMsg.writeInt32(CAST_S16(amount2), "amount");
    }
}

void MarketHandler::buyItems(const STD_VECTOR<ShopItem*> &items) const
{
    if (packetVersion < 20131218)
        return;
    int cnt = 0;
    const int pairSize = 6;

    FOR_EACH (STD_VECTOR<ShopItem*>::const_iterator, it, items)
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

    createOutPacket(CMSG_NPC_MARKET_BUY);
    outMsg.writeInt16(CAST_S16(4 + pairSize * cnt), "len");
    FOR_EACH (STD_VECTOR<ShopItem*>::const_iterator, it, items)
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
                outMsg.writeItemId(item->getId(),
                    "item id");
                outMsg.writeInt32(CAST_S16(1), "amount");
            }
        }
        else
        {
            outMsg.writeItemId(item->getId(),
                              "item id");
            outMsg.writeInt32(CAST_S16(usedQuantity), "amount");
        }
    }
}

}  // namespace EAthena
