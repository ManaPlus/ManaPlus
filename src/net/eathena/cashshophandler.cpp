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

#include "net/eathena/cashshophandler.h"

#include "logger.h"

#include "being/playerinfo.h"

#include "enums/being/attributes.h"

#include "gui/windows/buydialog.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::CashShopHandler *cashShopHandler;

namespace EAthena
{

BuyDialog *CashShopHandler::mBuyDialog = nullptr;

CashShopHandler::CashShopHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_NPC_CASH_SHOP_OPEN,
        SMSG_NPC_CASH_BUY_ACK,
        SMSG_NPC_CASH_POINTS,
        SMSG_NPC_CASH_BUY,
        SMSG_NPC_CASH_TAB_PRICE_LIST,
        SMSG_NPC_CASH_SCHEDULE,
        0
    };
    handledMessages = _messages;
    cashShopHandler = this;
    mBuyDialog = nullptr;
}

void CashShopHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_NPC_CASH_SHOP_OPEN:
            processCashShopOpen(msg);
            break;

        case SMSG_NPC_CASH_BUY_ACK:
            processCashShopBuyAck(msg);
            break;

        case SMSG_NPC_CASH_POINTS:
            processCashShopPoints(msg);
            break;

        case SMSG_NPC_CASH_BUY:
            processCashShopBuy(msg);
            break;

        case SMSG_NPC_CASH_TAB_PRICE_LIST:
            processCashShopTabPriceList(msg);
            break;

        case SMSG_NPC_CASH_SCHEDULE:
            processCashShopSchedule(msg);
            break;

        default:
            break;
    }
}

void CashShopHandler::processCashShopOpen(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 12) / 11;

    mBuyDialog = new BuyDialog(BuyDialog::Cash);
    mBuyDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));

    msg.readInt32("cash points");
    msg.readInt32("kafra points");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("price");
        const int value = msg.readInt32("discount price");
        const int type = msg.readUInt8("item type");
        const int itemId = msg.readInt16("item id");
        const int color = 1;
        mBuyDialog->addItem(itemId, type, color, 0, value);
    }
    mBuyDialog->sort();
}

void CashShopHandler::processCashShopBuyAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("cash points");
    msg.readInt32("kafra points");
    msg.readInt16("error");
}

void CashShopHandler::processCashShopPoints(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("cash points");
    msg.readInt32("kafra points");
}

void CashShopHandler::processCashShopBuy(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt32("id");
    msg.readInt16("result");
    msg.readInt32("cash points");
    msg.readInt32("kafra points");
}

void CashShopHandler::processCashShopTabPriceList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    const int count = (msg.readInt16("len") - 10) / 6;
    msg.readInt32("tab");
    const int itemsCount = msg.readInt16("count");
    if (count != itemsCount)
        logger->log("error: wrong list count");

    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("item id");
        msg.readInt32("price");
    }
}

void CashShopHandler::processCashShopSchedule(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    const int count = (msg.readInt16("len") - 8) / 6;
    const int itemsCount = msg.readInt16("count");
    msg.readInt16("tab");
    if (count != itemsCount)
        logger->log("error: wrong list count");

    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("item id");
        msg.readInt32("price");
    }
}

void CashShopHandler::buyItem(const int points,
                              const int itemId,
                              const unsigned char color A_UNUSED,
                              const int amount) const
{
    createOutPacket(CMSG_NPC_CASH_SHOP_BUY);
    outMsg.writeInt16(10 + 4, "len");
    outMsg.writeInt32(points, "points");
    outMsg.writeInt16(1, "count");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
    outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
}

void CashShopHandler::close() const
{
    createOutPacket(CMSG_NPC_CASH_SHOP_CLOSE);
}

void CashShopHandler::requestPoints() const
{
    createOutPacket(CMSG_NPC_CASH_SHOP_OPEN);
}

}  // namespace EAthena
