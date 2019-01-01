/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "net/tmwa/traderecv.h"

#include "notifymanager.h"

#include "being/playerinfo.h"
#include "being/playerrelation.h"
#include "being/playerrelations.h"

#include "const/net/inventory.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/tradewindow.h"

#include "net/messagein.h"
#include "net/tradehandler.h"

#include "net/ea/traderecv.h"

#include "resources/inventory/inventory.h"

#include "resources/item/item.h"

#include "utils/stringutils.h"

#include "debug.h"

extern std::string tradePartnerName;

namespace TmwAthena
{

void TradeRecv::processTradeRequest(Net::MessageIn &msg)
{
    Ea::TradeRecv::processTradeRequestContinue(msg.readString(24, "name"));
}

void TradeRecv::processTradeItemAdd(Net::MessageIn &msg)
{
    const int amount = msg.readInt32("amount");
    const int type = msg.readInt16("type");
    const uint8_t identify = msg.readUInt8("identify");
    msg.readUInt8("attribute");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readUInt16("card");

    if (tradeWindow != nullptr)
    {
        if (type == 0)
        {
            tradeWindow->setMoney(amount);
        }
        else
        {
            tradeWindow->addItem2(type,
                ItemType::Unknown,
                cards,
                nullptr,
                4,
                false,
                amount,
                refine,
                ItemColor_one,
                fromBool(identify, Identified),
                Damaged_false,
                Favorite_false,
                Equipm_false);
        }
    }
}

void TradeRecv::processTradeItemAddResponse(Net::MessageIn &msg)
{
    // Trade: New Item add response (was 0x00ea, now 01b1)
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    Item *item = nullptr;
    if (PlayerInfo::getInventory() != nullptr)
        item = PlayerInfo::getInventory()->getItem(index);

    if (item == nullptr)
    {
        if (tradeWindow != nullptr)
            tradeWindow->receivedOk(true);
        return;
    }
    const int quantity = msg.readInt16("amount");

    const uint8_t res = msg.readUInt8("status");
    switch (res)
    {
        case 0:
            // Successfully added item
            if (tradeWindow != nullptr)
            {
                tradeWindow->addItem2(item->getId(),
                    item->getType(),
                    item->getCards(),
                    nullptr,
                    4,
                    true,
                    quantity,
                    item->getRefine(),
                    item->getColor(),
                    item->getIdentified(),
                    item->getDamaged(),
                    item->getFavorite(),
                    item->isEquipment());
            }
            item->increaseQuantity(-quantity);
            break;
        case 1:
            // Add item failed - player overweighted
            NotifyManager::notify(NotifyTypes::
                TRADE_ADD_PARTNER_OVER_WEIGHT);
            break;
        case 2:
            // Add item failed - player has no free slot
            NotifyManager::notify(NotifyTypes::TRADE_ADD_PARTNER_NO_SLOTS);
            break;
        case 3:
            // Add item failed - non tradable item
            NotifyManager::notify(NotifyTypes::TRADE_ADD_UNTRADABLE_ITEM);
            break;
        default:
            NotifyManager::notify(NotifyTypes::TRADE_ADD_ERROR);
            UNIMPLEMENTEDPACKETFIELD(res);
            logger->log("QQQ SMSG_TRADE_ITEM_ADD_RESPONSE: "
                        + toString(res));
            break;
    }
}

void TradeRecv::processTradeResponse(Net::MessageIn &msg)
{
    if (tradePartnerName.empty() ||
        !playerRelations.hasPermission(tradePartnerName,
        PlayerRelation::TRADE))
    {
        tradeHandler->respond(false);
        return;
    }
    const uint8_t type = msg.readUInt8("type");
    Ea::TradeRecv::processTradeResponseContinue(type);
}

}  // namespace TmwAthena
