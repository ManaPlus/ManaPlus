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

#include "net/eathena/buysellrecv.h"

#include "notifymanager.h"

#include "being/playerinfo.h"

#include "const/resources/currency.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/buydialog.h"

#include "gui/widgets/createwidget.h"

#include "net/messagein.h"

#include "net/ea/buysellrecv.h"

#include "net/eathena/npcrecv.h"

#include "resources/beinginfo.h"

#include "resources/db/npcdb.h"

#include "debug.h"

extern int itemIdLen;

namespace EAthena
{

void BuySellRecv::processNpcBuy(Net::MessageIn &msg)
{
    msg.readInt16("len");
    const int sz = 9 + itemIdLen;
    const int n_items = (msg.getLength() - 4) / sz;

    const BeingTypeId npcId = NpcRecv::mNpcTypeId;
    std::string currency;

    if (npcId != BeingTypeId_zero)
    {
        const BeingInfo *const info = NPCDB::get(npcId);
        if (info != nullptr)
            currency = info->getCurrency();
        else
            currency = DEFAULT_CURRENCY;
    }
    else
    {
        currency = DEFAULT_CURRENCY;
    }

    CREATEWIDGETV(Ea::BuySellRecv::mBuyDialog, BuyDialog,
        Ea::BuySellRecv::mNpcId,
        currency);
    Ea::BuySellRecv::mBuyDialog->setMoney(
        PlayerInfo::getAttribute(Attributes::MONEY));

    for (int k = 0; k < n_items; k++)
    {
        const int value = msg.readInt32("price");
        msg.readInt32("dc value?");
        const ItemTypeT type = static_cast<ItemTypeT>(
            msg.readUInt8("type"));
        const int itemId = msg.readItemId("item id");
        const ItemColor color = ItemColor_one;
        Ea::BuySellRecv::mBuyDialog->addItem(itemId, type, color, 0, value);
    }
    Ea::BuySellRecv::mBuyDialog->sort();
}

void BuySellRecv::processNpcSellResponse(Net::MessageIn &msg)
{
    switch (msg.readUInt8("result"))
    {
        case 0:
            NotifyManager::notify(NotifyTypes::SOLD);
            break;
        case 1:
        default:
            NotifyManager::notify(NotifyTypes::SELL_FAILED);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::SELL_TRADE_FAILED);
            break;
        case 3:
            NotifyManager::notify(NotifyTypes::SELL_UNSELLABLE_FAILED);
            break;
    }
}

void BuySellRecv::processNpcBuyResponse(Net::MessageIn &msg)
{
    const uint8_t response = msg.readUInt8("response");
    if (response == 0U)
    {
        NotifyManager::notify(NotifyTypes::BUY_DONE);
        return;
    }
    switch (response)
    {
        case 1:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_NO_MONEY);
            break;

        case 2:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_OVERWEIGHT);
            break;

        case 3:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_TOO_MANY_ITEMS);
            break;

        default:
            NotifyManager::notify(NotifyTypes::BUY_FAILED);
            break;
    }
}

}  // namespace EAthena
