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

#include "net/eathena/vendingrecv.h"

#include "actormanager.h"
#include "itemcolormanager.h"
#include "shopitem.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/buydialog.h"

#include "gui/widgets/createwidget.h"

#include "listeners/vendingmodelistener.h"
#include "listeners/vendingslotslistener.h"

#include "net/messagein.h"

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
    const int count = (msg.readInt16("len") - 12) / 22;
    const BeingId id = msg.readBeingId("id");
    Being *const being = actorManager->findBeing(id);
    if (!being)
        return;
    int cards[4];
    CREATEWIDGETV(mBuyDialog, BuyDialog, being->getName());
    mBuyDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));
    msg.readInt32("vender id");
    for (int f = 0; f < count; f ++)
    {
        const int value = msg.readInt32("price");
        const int amount = msg.readInt16("amount");
        const int index = msg.readInt16("inv index");
        const int type = msg.readUInt8("item type");
        const int itemId = msg.readInt16("item id");
        msg.readUInt8("identify");
        msg.readUInt8("attribute");
        msg.readUInt8("refine");
        for (int d = 0; d < 4; d ++)
            cards[d] = msg.readInt16("card");
        if (serverVersion >= 8 && packetVersion >= 20150226)
        {
            for (int d = 0; d < 5; d ++)
            {
                msg.readInt16("rnd index");
                msg.readInt16("rnd value");
                msg.readUInt8("rnd param");
            }
        }

        const ItemColor color = ItemColorManager::getColorFromCards(&cards[0]);
        ShopItem *const item = mBuyDialog->addItem(itemId, type,
            color, amount, value);
        if (item)
            item->setInvIndex(index);
    }
    mBuyDialog->sort();
}

void VendingRecv::processBuyAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("inv index");
    msg.readInt16("amount");
    msg.readUInt8("flag");
}

void VendingRecv::processOpen(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 8) / 22;
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
        for (int d = 0; d < 4; d ++)
            msg.readInt16("card");
        if (serverVersion >= 8 && packetVersion >= 20150226)
        {
            for (int d = 0; d < 5; d ++)
            {
                msg.readInt16("rnd index");
                msg.readInt16("rnd value");
                msg.readUInt8("rnd param");
            }
        }
    }
    PlayerInfo::enableVending(true);
    VendingModeListener::distributeEvent(true);
}

void VendingRecv::processReport(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("inv index");
    msg.readInt16("amount");
}

void VendingRecv::processOpenStatus(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readUInt8("result");
}

}  // namespace EAthena
