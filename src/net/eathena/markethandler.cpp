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

#include "net/eathena/markethandler.h"

#include "notifymanager.h"

#include "being/attributes.h"
#include "being/playerinfo.h"

#include "gui/windows/buydialog.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern Net::MarketHandler *marketHandler;

namespace EAthena
{

BuyDialog *MarketHandler::mBuyDialog = nullptr;

MarketHandler::MarketHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_NPC_MARKET_OPEN,
        SMSG_NPC_MARKET_BUY_ACK,
        0
    };
    handledMessages = _messages;
    marketHandler = this;
    mBuyDialog = nullptr;
}

void MarketHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_NPC_MARKET_OPEN:
            processMarketOpen(msg);
            break;

        case SMSG_NPC_MARKET_BUY_ACK:
            processMarketBuyAck(msg);
            break;

        default:
            break;
    }
}

void MarketHandler::processMarketOpen(Net::MessageIn &msg)
{
    const int len = (msg.readInt16("len") - 4) / 13;

    mBuyDialog = new BuyDialog(BuyDialog::Market);
    mBuyDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));

    for (int f = 0; f < len; f ++)
    {
        const int itemId = msg.readInt16("item id");
        const int type = msg.readUInt8("type");
        const int value = msg.readInt32("price");
        const int amount = msg.readInt32("amount");
        msg.readInt16("view");
        const unsigned char color = 1;
        mBuyDialog->addItem(itemId, type, color, amount, value);
    }
    mBuyDialog->sort();
}

void MarketHandler::processMarketBuyAck(Net::MessageIn &msg)
{
    const int len = (msg.readInt16("len") - 5) / 8;
    const int res = msg.readUInt8("result");
    for (int f = 0; f < len; f ++)
    {
        msg.readInt16("item id");
        msg.readInt16("amount");
        msg.readInt32("price");
    }
    if (res)
        NotifyManager::notify(NotifyTypes::BUY_DONE);
    else
        NotifyManager::notify(NotifyTypes::BUY_FAILED);
}

void MarketHandler::close()
{
    createOutPacket(CMSG_NPC_MARKET_CLOSE);
}

void MarketHandler::buyItem(const int itemId,
                            const int type,
                            const unsigned char color A_UNUSED,
                            const int amount) const
{
    const bool nonStack = type == 4 || type == 5 || type == 7 || type == 8;
    int cnt = nonStack ? amount : 1;
    const int amount2 = nonStack ? 1 : amount;
    if (cnt > 100)
        cnt = 100;

    createOutPacket(CMSG_NPC_MARKET_BUY);
    outMsg.writeInt16(4 + 6 * cnt, "len");
    for (int f = 0; f < cnt; f ++)
    {
        outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
        outMsg.writeInt32(static_cast<int16_t>(amount2), "amount");
    }
}

}  // namespace EAthena
