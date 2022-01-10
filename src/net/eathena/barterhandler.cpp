/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "net/eathena/barterhandler.h"

#include "net/eathena/barterrecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "utils/foreach.h"

#include "resources/item/shopitem.h"

#include "debug.h"

extern int packetVersionMain;
extern int packetVersionRe;
extern int packetVersionZero;
extern int itemIdLen;

namespace EAthena
{

BarterHandler::BarterHandler() :
    Net::BarterHandler()
{
    barterHandler = this;
    BarterRecv::mBuyDialog = nullptr;
}

BarterHandler::~BarterHandler()
{
    barterHandler = nullptr;
}

void BarterHandler::close() const
{
    if (packetVersionMain < 20190116 &&
        packetVersionRe < 20190116 &&
        packetVersionZero < 20181226)
    {
        return;
    }

    createOutPacket(CMSG_NPC_BARTER_CLOSE);
}

void BarterHandler::buyItems(const STD_VECTOR<ShopItem*> &items) const
{
    if (packetVersionMain < 20190116 &&
        packetVersionRe < 20190116 &&
        packetVersionZero < 20181226)
    {
        return;
    }
    int cnt = 0;
    const int pairSize = 10 + itemIdLen;

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
                // +++ need use player inventory index
                outMsg.writeInt16(0, "inv index");
                outMsg.writeInt32(item->getCurrentInvIndex(),
                    "inv index");
            }
        }
        else
        {
            outMsg.writeItemId(item->getId(),
                              "item id");
            outMsg.writeInt32(CAST_S16(usedQuantity), "amount");
            // +++ need use player inventory index
            outMsg.writeInt16(0, "inv index");
            outMsg.writeInt32(item->getCurrentInvIndex(),
                "inv index");
        }
    }
}

}  // namespace EAthena
