/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#include "net/eathena/cashshoprecv.h"

#include "notifymanager.h"

#include "const/resources/currency.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/buydialog.h"

#include "gui/widgets/createwidget.h"

#include "net/messagein.h"

#include "net/eathena/npcrecv.h"

#include "resources/beinginfo.h"

#include "resources/db/npcdb.h"

#include "debug.h"

extern int packetVersion;
extern int itemIdLen;

namespace EAthena
{

namespace CashShopRecv
{
    BuyDialog *mBuyDialog;
}  // namespace CashShopRecv

void CashShopRecv::processCashShopOpen(Net::MessageIn &msg)
{
    int count;
    int blockSize = 11;
    if (itemIdLen == 4)
        blockSize += 2;
    if (packetVersion >= 20070711)
        count = (msg.readInt16("len") - 12) / blockSize;
    else
        count = (msg.readInt16("len") - 8) / blockSize;

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
        fromInt(BuyDialog::Cash, BeingId),
        currency);
    const int points = msg.readInt32("cash points");

    mBuyDialog->setMoney(points);

    if (packetVersion >= 20070711)
        msg.readInt32("kafra points");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("price");
        const int value = msg.readInt32("discount price");
        const ItemTypeT type = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const int itemId = msg.readItemId("item id");
        const ItemColor color = ItemColor_one;
        mBuyDialog->addItem(itemId, type, color, 0, value);
    }
    mBuyDialog->sort();
}

void CashShopRecv::processCashShopBuyAck(Net::MessageIn &msg)
{
    msg.readInt32("cash points");
    msg.readInt32("kafra points");
    const uint16_t res = msg.readInt16("error");
    switch (res)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::BUY_DONE);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_NPC_NOT_FOUND);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_SYSTEM_ERROR);
            break;
        case 3:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_OVERWEIGHT);
            break;
        case 4:
            NotifyManager::notify(NotifyTypes::BUY_TRADE_FAILED);
            break;
        case 5:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_WRONG_ITEM);
            break;
        case 6:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_NO_MONEY);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(res);
            break;
    }
}

void CashShopRecv::processCashShopPoints(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("cash points");
    msg.readInt32("kafra points");
}

void CashShopRecv::processCashShopBuy(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("id");
    msg.readInt16("result");
    msg.readInt32("cash points");
    msg.readInt32("kafra points");
}

void CashShopRecv::processCashShopTabPriceList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int count = (msg.readInt16("len") - 10) / 6;
    msg.readInt32("tab");
    const int itemsCount = msg.readInt16("count");
    if (count != itemsCount)
        logger->log("error: wrong list count");

    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("item id");  // item id size always 16 bit
        msg.readInt32("price");
    }
}

void CashShopRecv::processCashShopSchedule(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    int blockSize = 6;
    if (itemIdLen == 4)
        blockSize += 2;
    const int count = (msg.readInt16("len") - 8) / blockSize;
    const int itemsCount = msg.readInt16("count");
    msg.readInt16("tab");
    if (count != itemsCount)
        logger->log("error: wrong list count");

    for (int f = 0; f < count; f ++)
    {
        msg.readItemId("item id");
        msg.readInt32("price");
    }
}

}  // namespace EAthena
