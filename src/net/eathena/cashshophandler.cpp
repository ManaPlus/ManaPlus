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

#include "being/playerinfo.h"

#include "gui/windows/buydialog.h"

#include "gui/widgets/createwidget.h"

#include "net/eathena/cashshoprecv.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::CashShopHandler *cashShopHandler;

namespace EAthena
{

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
    CashShopRecv::mBuyDialog = nullptr;
}

void CashShopHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_NPC_CASH_SHOP_OPEN:
            CashShopRecv::processCashShopOpen(msg);
            break;

        case SMSG_NPC_CASH_BUY_ACK:
            CashShopRecv::processCashShopBuyAck(msg);
            break;

        case SMSG_NPC_CASH_POINTS:
            CashShopRecv::processCashShopPoints(msg);
            break;

        case SMSG_NPC_CASH_BUY:
            CashShopRecv::processCashShopBuy(msg);
            break;

        case SMSG_NPC_CASH_TAB_PRICE_LIST:
            CashShopRecv::processCashShopTabPriceList(msg);
            break;

        case SMSG_NPC_CASH_SCHEDULE:
            CashShopRecv::processCashShopSchedule(msg);
            break;

        default:
            break;
    }
}

void CashShopHandler::buyItem(const int points,
                              const int itemId,
                              const ItemColor color A_UNUSED,
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

void CashShopHandler::requestTab(const int tab) const
{
    createOutPacket(CMSG_NPC_CASH_SHOP_REQUEST_TAB);
    outMsg.writeInt16(static_cast<int16_t>(tab), "tab");
}

void CashShopHandler::schedule() const
{
    createOutPacket(CMSG_NPC_CASH_SHOP_SCHEDULE);
}

}  // namespace EAthena
