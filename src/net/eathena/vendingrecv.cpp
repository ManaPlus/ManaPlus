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

#include "net/eathena/vendingrecv.h"

#include "actormanager.h"
#include "itemcolormanager.h"
#include "notifymanager.h"

#include "const/resources/currency.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/net/inventory.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/buydialog.h"

#include "gui/widgets/createwidget.h"

#include "listeners/vendingmodelistener.h"
#include "listeners/vendingslotslistener.h"

#include "net/messagein.h"

#include "resources/iteminfo.h"

#include "resources/db/unitsdb.h"

#include "resources/inventory/inventory.h"

#include "resources/item/itemoptionslist.h"
#include "resources/item/shopitem.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

extern int packetVersion;
extern int serverVersion;

namespace EAthena
{

namespace VendingRecv
{
    BuyDialog *mBuyDialog = nullptr;
}  // namespace VendingRecv

void VendingRecv::processOpenReq(Net::MessageIn &msg)
{
    VendingSlotsListener::distributeEvent(msg.readInt16("slots allowed"));
}

void VendingRecv::processShowBoard(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("owner id");
    const std::string shopName = msg.readString(80, "shop name");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing)
        dstBeing->setSellBoard(shopName);
}

void VendingRecv::processHideBoard(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("owner id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing)
        dstBeing->setSellBoard(std::string());
    if (dstBeing == localPlayer)
    {
        PlayerInfo::enableVending(false);
        VendingModeListener::distributeEvent(false);
    }
}

void VendingRecv::processItemsList(Net::MessageIn &msg)
{
    int packetLen = 22;
    if (msg.getVersion() >= 20150226)
        packetLen += 25;
    int offset = 8;
    if (msg.getVersion() >= 20100105)
        offset += 4;

    const int count = (msg.readInt16("len") - offset) / packetLen;
    const BeingId id = msg.readBeingId("id");
    const Being *const being = actorManager->findBeing(id);
    if (!being)
        return;
    int cards[maxCards];
    CREATEWIDGETV(mBuyDialog, BuyDialog, being, DEFAULT_CURRENCY);
    mBuyDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));
    if (msg.getVersion() >= 20100105)
        msg.readInt32("vender id");
    for (int f = 0; f < count; f ++)
    {
        const int value = msg.readInt32("price");
        const int amount = msg.readInt16("amount");
        const int index = msg.readInt16("inv index");
        const ItemTypeT type = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const int itemId = msg.readInt16("item id");
        msg.readUInt8("identify");
        msg.readUInt8("attribute");
        msg.readUInt8("refine");
        for (int d = 0; d < maxCards; d ++)
            cards[d] = msg.readUInt16("card");
        ItemOptionsList *options = nullptr;
        if (msg.getVersion() >= 20150226)
        {
            options = new ItemOptionsList;
            for (int d = 0; d < 5; d ++)
            {
                const uint16_t idx = msg.readInt16("option index");
                const uint16_t val = msg.readInt16("option value");
                msg.readUInt8("option param");
                options->add(idx, val);
            }
        }

        const ItemColor color = ItemColorManager::getColorFromCards(&cards[0]);
        ShopItem *const item = mBuyDialog->addItem(itemId, type,
            color, amount, value);
        if (item)
        {
            item->setInvIndex(index);
            item->setOptions(options);
        }
        delete options;
    }
    mBuyDialog->sort();
}

void VendingRecv::processBuyAck(Net::MessageIn &msg)
{
    msg.readInt16("inv index");
    msg.readInt16("amount");
    const int flag = msg.readUInt8("flag");
    switch (flag)
    {
        case 0:
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_NO_MONEY);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_OVERWEIGHT);
            break;
        case 4:
            NotifyManager::notify(NotifyTypes::BUY_FAILED_TOO_MANY_ITEMS);
            break;
        case 5:
            NotifyManager::notify(NotifyTypes::BUY_TRADE_FAILED);
            break;
        case 6:  // +++ probably need show exact error messages?
        case 7:
            NotifyManager::notify(NotifyTypes::BUY_FAILED);
            break;
        default:
            NotifyManager::notify(NotifyTypes::BUY_FAILED);
            UNIMPLEMENTEDPACKETFIELD(flag);
            break;
    }
}

void VendingRecv::processOpen(Net::MessageIn &msg)
{
    int packetLen = 22;
    if (msg.getVersion() >= 20150226)
        packetLen += 25;

    const int count = (msg.readInt16("len") - 8) / packetLen;
    msg.readInt32("id");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("price");
        msg.readInt16("inv index");
        msg.readInt16("amount");
        msg.readUInt8("item type");
        msg.readInt16("item id");
        msg.readUInt8("identify");
        msg.readUInt8("attribute");
        msg.readUInt8("refine");
        for (int d = 0; d < maxCards; d ++)
            msg.readUInt16("card");
        if (msg.getVersion() >= 20150226)
        {
            for (int d = 0; d < 5; d ++)
            {
                msg.readInt16("option index");
                msg.readInt16("option value");
                msg.readUInt8("option param");
            }
        }
    }
    PlayerInfo::enableVending(true);
    VendingModeListener::distributeEvent(true);
}

void VendingRecv::processReport(Net::MessageIn &msg)
{
    const int index = msg.readInt16("inv index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");
    int money = 0;
    if (msg.getVersion() >= 20141016)
    {
        msg.readInt32("char id");
        msg.readInt32("time");
        money = msg.readInt32("zeny");
    }
    const Inventory *const inventory = PlayerInfo::getCartInventory();
    if (!inventory)
        return;
    const Item *const item = inventory->getItem(index);
    if (!item)
        return;

    const ItemInfo &info = item->getInfo();
    std::string str;
    if (money != 0)
    {
        // TRANSLATORS: vending sold item message
        str = strprintf(_("Sold item %s amount %d. You got: %s"),
            info.getLink().c_str(),
            amount,
            UnitsDb::formatCurrency(money).c_str());
    }
    else
    {
        // TRANSLATORS: vending sold item message
        str = strprintf(_("Sold item %s amount %d"),
            info.getLink().c_str(),
            amount);
    }
    NotifyManager::notify(NotifyTypes::VENDING_SOLD_ITEM, str);
}

void VendingRecv::processOpenStatus(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readUInt8("result");
}

}  // namespace EAthena
