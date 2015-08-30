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
#include "itemcolormanager.h"
#include "shopitem.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "gui/windows/buydialog.h"

#include "gui/widgets/createwidget.h"

#include "listeners/vendingmodelistener.h"
#include "listeners/vendingslotslistener.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"
#include "net/eathena/vendingrecv.h"

#include "debug.h"

extern Net::VendingHandler *vendingHandler;

namespace EAthena
{

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
        SMSG_VENDING_REPORT,
        SMSG_VENDING_OPEN_STATUS,
        0
    };
    handledMessages = _messages;
    vendingHandler = this;
    VendingRecv::mBuyDialog = nullptr;
}

void VendingHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_VENDING_OPEN_REQ:
            VendingRecv::processOpenReq(msg);
            break;

        case SMSG_VENDING_SHOW_BOARD:
            VendingRecv::processShowBoard(msg);
            break;

        case SMSG_VENDING_HIDE_BOARD:
            VendingRecv::processHideBoard(msg);
            break;

        case SMSG_VENDING_ITEMS_LIST:
            VendingRecv::processItemsList(msg);
            break;

        case SMSG_VENDING_BUY_ACK:
            VendingRecv::processBuyAck(msg);
            break;

        case SMSG_VENDING_OPEN:
            VendingRecv::processOpen(msg);
            break;

        case SMSG_VENDING_REPORT:
            VendingRecv::processReport(msg);
            break;

        case SMSG_VENDING_OPEN_STATUS:
            VendingRecv::processOpenStatus(msg);
            break;

        default:
            break;
    }
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
    outMsg.writeBeingId(being->getId(), "account id");
}

void VendingHandler::buy(const Being *const being,
                         const int index,
                         const int amount) const
{
    if (!being)
        return;

    createOutPacket(CMSG_VENDING_BUY);
    outMsg.writeInt16(12, "len");
    outMsg.writeBeingId(being->getId(), "account id");
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
    outMsg.writeBeingId(being->getId(), "account id");
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
