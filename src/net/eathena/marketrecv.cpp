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

#include "net/eathena/marketrecv.h"

#include "notifymanager.h"

#include "being/playerinfo.h"

#include "const/resources/currency.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/buydialog.h"

#include "gui/widgets/createwidget.h"

#include "net/messagein.h"

#include "net/eathena/npcrecv.h"

#include "resources/beinginfo.h"

#include "resources/db/npcdb.h"

#include "debug.h"

extern int itemIdLen;

namespace EAthena
{

namespace MarketRecv
{
    BuyDialog *mBuyDialog = nullptr;
}  // namespace MarketRecv


void MarketRecv::processMarketOpen(Net::MessageIn &msg)
{
    const int len = (msg.readInt16("len") - 4) / (11 + itemIdLen);

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

    CREATEWIDGETV(mBuyDialog, BuyDialog,
        fromInt(BuyDialog::Market, BeingId),
        currency);
    mBuyDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));

    for (int f = 0; f < len; f ++)
    {
        const int itemId = msg.readItemId("item id");
        const ItemTypeT type = static_cast<ItemTypeT>(msg.readUInt8("type"));
        const int value = msg.readInt32("price");
        const int amount = msg.readInt32("amount");
        msg.readInt16("weight");  // +++ compare with item weight from db?
        const ItemColor color = ItemColor_one;
        mBuyDialog->addItem(itemId, type, color, amount, value);
    }
    mBuyDialog->sort();
}

void MarketRecv::processMarketBuyAck(Net::MessageIn &msg)
{
    const int len = (msg.readInt16("len") - 5) / (6 + itemIdLen);
    const int res = msg.readUInt8("result");
    for (int f = 0; f < len; f ++)
    {
        msg.readItemId("item id");
        msg.readInt16("amount");
        msg.readInt32("price");
    }
    if (res != 0)
        NotifyManager::notify(NotifyTypes::BUY_DONE);
    else
        NotifyManager::notify(NotifyTypes::BUY_FAILED);
}

}  // namespace EAthena
