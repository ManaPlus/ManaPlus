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

#include "net/eathena/vendinghandler.h"

#include "actormanager.h"
#include "shopitem.h"

#include "enums/being/attributes.h"

#include "being/being.h"
#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/buydialog.h"

#include "listeners/vendingmodelistener.h"
#include "listeners/vendingslotslistener.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::VendingHandler *vendingHandler;

namespace EAthena
{

BuyDialog *VendingHandler::mBuyDialog = nullptr;

VendingHandler::VendingHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_VENDING_OPEN_REQ,
        SMSG_VENDING_SHOW_BOARD,
        SMSG_VENDING_HIDE_BOARD,
        SMSG_VENDING_ITEMS_LIST,
        SMSG_VENDING_BUY_ACK,
        SMSG_VENDING_OPEN,
        0
    };
    handledMessages = _messages;
    vendingHandler = this;
    mBuyDialog = nullptr;
}

void VendingHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_VENDING_OPEN_REQ:
            processOpenReq(msg);
            break;

        case SMSG_VENDING_SHOW_BOARD:
            processShowBoard(msg);
            break;

        case SMSG_VENDING_HIDE_BOARD:
            processHideBoard(msg);
            break;

        case SMSG_VENDING_ITEMS_LIST:
            processItemsList(msg);
            break;

        case SMSG_VENDING_BUY_ACK:
            processBuyAck(msg);
            break;

        case SMSG_VENDING_OPEN:
            processOpen(msg);
            break;

        case SMSG_VENDING_REPORT:
            processReport(msg);
            break;

        default:
            break;
    }
}

void VendingHandler::processOpenReq(Net::MessageIn &msg)
{
    VendingSlotsListener::distributeEvent(msg.readInt16("slots allowed"));
}

void VendingHandler::processShowBoard(Net::MessageIn &msg)
{
    const int id = msg.readInt32("owner id");
    const std::string shopName = msg.readString(80, "shop name");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing)
        dstBeing->setBoard(shopName);
}

void VendingHandler::processHideBoard(Net::MessageIn &msg)
{
    const int id = msg.readInt32("owner id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing)
        dstBeing->setBoard(std::string());
    if (dstBeing == localPlayer)
    {
        PlayerInfo::enableVending(false);
        VendingModeListener::distributeEvent(false);
    }
}

void VendingHandler::processItemsList(Net::MessageIn &msg)
{

    const int count = (msg.readInt16("len") - 12) / 22;
    const int id = msg.readInt32("id");
    Being *const being = actorManager->findBeing(id);
    if (!being)
        return;
    mBuyDialog = new BuyDialog(being->getName());
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
            msg.readInt16("card");

        const unsigned char color = 1;
        ShopItem *const item = mBuyDialog->addItem(itemId, type,
            color, amount, value);
        if (item)
            item->setInvIndex(index);
    }
    mBuyDialog->sort();
}

void VendingHandler::processBuyAck(Net::MessageIn &msg)
{
    msg.readInt16("inv index");
    msg.readInt16("amount");
    msg.readUInt8("flag");
}

void VendingHandler::processOpen(Net::MessageIn &msg)
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
    }
    PlayerInfo::enableVending(true);
    VendingModeListener::distributeEvent(true);
}

void VendingHandler::processReport(Net::MessageIn &msg)
{
    msg.readInt16("inv index");
    msg.readInt16("amount");
}

void VendingHandler::close() const
{
    createOutPacket(CMSG_VENDING_CLOSE);
    PlayerInfo::enableVending(false);
}

void VendingHandler::open(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_VENDING_LIST_REQ);
    outMsg.writeInt32(being->getId(), "account id");
}

void VendingHandler::buy(const Being *const being,
                         const int index,
                         const int amount) const
{
    if (!being)
        return;

    createOutPacket(CMSG_VENDING_BUY);
    outMsg.writeInt16(12, "len");
    outMsg.writeInt32(being->getId(), "account id");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
    outMsg.writeInt16(static_cast<int16_t>(index), "index");
}

void VendingHandler::buy2(const Being *const being,
                          const int vendId,
                          const int index,
                          const int amount) const
{
    if (!being)
        return;

    createOutPacket(CMSG_VENDING_BUY2);
    outMsg.writeInt16(16, "len");
    outMsg.writeInt32(being->getId(), "account id");
    outMsg.writeInt32(vendId, "vend id");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
    outMsg.writeInt16(static_cast<int16_t>(index), "index");
}

void VendingHandler::createShop(const std::string &name,
                                const bool flag,
                                std::vector<ShopItem*> &items) const
{
    createOutPacket(CMSG_VENDING_CREATE_SHOP);
    outMsg.writeInt16(static_cast<int16_t>(85 + items.size() * 8), "len");
    outMsg.writeString(name, 80, "shop name");
    outMsg.writeInt8(static_cast<int8_t>(flag ? 1 : 0), "flag");
    FOR_EACH (std::vector<ShopItem*>::const_iterator, it, items)
    {
        const ShopItem *const item = *it;
        outMsg.writeInt16(static_cast<int16_t>(
            item->getInvIndex() + INVENTORY_OFFSET), "index");
        outMsg.writeInt16(static_cast<int16_t>(item->getQuantity()), "amount");
        outMsg.writeInt32(item->getPrice(), "price");
    }
}

}  // namespace EAthena
