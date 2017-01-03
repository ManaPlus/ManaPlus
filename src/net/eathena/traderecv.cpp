/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/eathena/traderecv.h"

#include "itemcolormanager.h"
#include "notifymanager.h"

#include "being/playerinfo.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/tradewindow.h"

#include "net/messagein.h"

#include "net/ea/traderecv.h"

#include "resources/inventory/inventory.h"

#include "resources/item/item.h"

#include "debug.h"

extern int serverVersion;

namespace EAthena
{

namespace TradeRecv
{
    int mQuantity = 0;
    int mItemIndex = -1;
}  // namespace TradeRecv

void TradeRecv::processTradeRequest(Net::MessageIn &msg)
{
    const std::string &partner = msg.readString(24, "name");
    if (msg.getVersion() >= 6)
    {
        msg.readInt32("char id");
        msg.readInt16("base level");
    }
    Ea::TradeRecv::processTradeRequestContinue(partner);
}

void TradeRecv::processTradeResponse(Net::MessageIn &msg)
{
    const uint8_t type = msg.readUInt8("type");
    msg.readInt32("char id");
    msg.readInt16("base level");
    Ea::TradeRecv::processTradeResponseContinue(type);
}

void TradeRecv::processTradeItemAdd(Net::MessageIn &msg)
{
    const int type = msg.readInt16("type");
    ItemTypeT itemType = ItemType::Unknown;
    if (msg.getVersion() >= 20100223)
    {
        itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
    }
    const int amount = msg.readInt32("amount");
    const uint8_t identify = msg.readUInt8("identify");
    const Damaged damaged = fromBool(msg.readUInt8("attribute"), Damaged);
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readUInt16("card");
    if ((serverVersion >= 8 || serverVersion == 0) &&
        msg.getVersion() >= 20150226)
    {
        for (int f = 0; f < 5; f ++)
        {
            msg.readInt16("rnd index");
            msg.readInt16("rnd value");
            msg.readUInt8("rnd param");
        }
    }

    if (tradeWindow)
    {
        if (type == 0)
        {
            tradeWindow->setMoney(amount);
        }
        else
        {
            tradeWindow->addItem2(type,
                itemType,
                cards,
                4,
                false,
                amount,
                refine,
                ItemColorManager::getColorFromCards(&cards[0]),
                fromBool(identify, Identified),
                damaged,
                Favorite_false,
                Equipm_false);
        }
    }
}

void TradeRecv::processTradeItemAddResponse(Net::MessageIn &msg)
{
    msg.readInt16("index");
    const uint8_t res = msg.readUInt8("fail");
    switch (res)
    {
        case 0:  // Successfully added item
        case 9:  // silent added item
        {
            Item *const item = PlayerInfo::getInventory()->getItem(
                mItemIndex);
            if (!item)
                return;
            if (tradeWindow)
            {
                tradeWindow->addItem2(item->getId(),
                    item->getType(),
                    item->getCards(),
                    4,
                    true,
                    mQuantity,
                    item->getRefine(),
                    item->getColor(),
                    item->getIdentified(),
                    item->getDamaged(),
                    item->getFavorite(),
                    item->isEquipment());
            }
            item->increaseQuantity(-mQuantity);
            mItemIndex = -1;
            mQuantity = 0;
            break;
        }
        case 1:
            // Add item failed - player overweighted
            NotifyManager::notify(NotifyTypes::
                TRADE_ADD_PARTNER_OVER_WEIGHT);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::TRADE_ADD_ERROR);
            break;
        default:
            NotifyManager::notify(NotifyTypes::TRADE_ADD_ERROR);
            UNIMPLEMENTEDPACKETFIELD(res);
            break;
    }
}

void TradeRecv::processTradeUndo(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    // +++ here need clear trade window from partner side?
}

}  // namespace EAthena
