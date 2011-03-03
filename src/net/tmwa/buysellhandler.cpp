/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#include "net/tmwa/buysellhandler.h"

#include "actorspritemanager.h"
#include "configuration.h"
#include "event.h"
#include "inventory.h"
#include "item.h"
#include "localplayer.h"
#include "playerinfo.h"
#include "shopitem.h"

#include "gui/buy.h"
#include "gui/buysell.h"
#include "gui/sell.h"
#include "gui/shopwindow.h"

#include "gui/widgets/chattab.h"

#include "net/chathandler.h"
#include "net/messagein.h"
#include "net/net.h"

#include "net/tmwa/chathandler.h"
#include "net/tmwa/protocol.h"

#include "utils/gettext.h"

extern Net::BuySellHandler *buySellHandler;

namespace TmwAthena
{

BuySellHandler::BuySellHandler()
{
    static const Uint16 _messages[] =
    {
        SMSG_NPC_BUY_SELL_CHOICE,
        SMSG_NPC_BUY,
        SMSG_NPC_SELL,
        SMSG_NPC_BUY_RESPONSE,
        SMSG_NPC_SELL_RESPONSE,
        0
    };
    mNpcId = 0;
    handledMessages = _messages;
    buySellHandler = this;
    mBuyDialog = 0;
}

void BuySellHandler::handleMessage(Net::MessageIn &msg)
{
    int n_items;

    switch (msg.getId())
    {
        case SMSG_NPC_BUY_SELL_CHOICE:
            if (!BuySellDialog::isActive())
            {
                mNpcId = msg.readInt32();
                new BuySellDialog(mNpcId);
            }
            break;

        case SMSG_NPC_BUY:
        {
            msg.readInt16();  // length
            int sz = 11;
            if (serverVersion > 0)
                sz += 1;
            n_items = (msg.getLength() - 4) / sz;
            mBuyDialog = new BuyDialog(mNpcId);
            mBuyDialog->setMoney(PlayerInfo::getAttribute(MONEY));

            for (int k = 0; k < n_items; k++)
            {
                int value = msg.readInt32();
                msg.readInt32();  // DCvalue
                msg.readInt8();  // type
                int itemId = msg.readInt16();
                unsigned char color = 1;
                if (serverVersion > 0)
                    color = msg.readInt8();
                mBuyDialog->addItem(itemId, color, 0, value);
            }
            break;
        }

        case SMSG_NPC_SELL:
            msg.readInt16();  // length
            n_items = (msg.getLength() - 4) / 10;
            if (n_items > 0)
            {
                SellDialog *dialog = new SellDialog(mNpcId);
                dialog->setMoney(PlayerInfo::getAttribute(MONEY));

                for (int k = 0; k < n_items; k++)
                {
                    int index = msg.readInt16() - INVENTORY_OFFSET;
                    int value = msg.readInt32();
                    msg.readInt32();  // OCvalue

                    Item *item = PlayerInfo::getInventory()->getItem(index);

                    if (item && !(item->isEquipped()))
                        dialog->addItem(item, value);
                }
            }
            else
            {
                SERVER_NOTICE(_("Nothing to sell."))
            }
            break;

        case SMSG_NPC_BUY_RESPONSE:
            if (msg.readInt8() == 0)
            {
                SERVER_NOTICE(_("Thanks for buying."))
            }
            else
            {
                // Reset player money since buy dialog already assumed purchase
                // would go fine
                if (mBuyDialog)
                    mBuyDialog->setMoney(PlayerInfo::getAttribute(MONEY));
                SERVER_NOTICE(_("Unable to buy."))
            }
            break;

        case SMSG_NPC_SELL_RESPONSE:
            switch (msg.readInt8())
            {
                case 0:
                    SERVER_NOTICE(_("Thanks for selling."))
                    break;
                case 1:
                default:
                    SERVER_NOTICE(_("Unable to sell."))
                    break;
                case 2:
                    SERVER_NOTICE(_("Unable to sell while trading."))
                    break;
                case 3:
                    SERVER_NOTICE(_("Unable to sell unsellable item."))
                    break;
            }
        default:
            break;
    }

}

void BuySellHandler::requestSellList(std::string nick)
{
    if (nick.empty() != 0 || !shopWindow)
        return;

    std::string data = "!selllist " + toString(tick_time);
    shopWindow->setAcceptPlayer(nick);

    if (config.getBoolValue("hideShopMessages"))
    {
        Net::getChatHandler()->privateMessage(nick, data);
    }
    else
    {
        if (chatWindow)
            chatWindow->whisper(nick, data, BY_PLAYER);
    }
//was true
}

void BuySellHandler::requestBuyList(std::string nick)
{
    if (nick.empty() || !shopWindow)
        return;

    std::string data = "!buylist " + toString(tick_time);
    shopWindow->setAcceptPlayer(nick);

    if (config.getBoolValue("hideShopMessages"))
    {
        Net::getChatHandler()->privateMessage(nick, data);
    }
    else
    {
        if (chatWindow)
            chatWindow->whisper(nick, data, BY_PLAYER);
    }
//was true
}

void BuySellHandler::sendBuyRequest(std::string nick, ShopItem* item,
                                    int amount)
{
    if (!chatWindow || nick.empty() || !item ||
        amount < 1 || amount > item->getQuantity())
    {
        return;
    }
    std::string data = strprintf("!buyitem %d %d %d",
                        item->getId(), item->getPrice(), amount);

    if (config.getBoolValue("hideShopMessages"))
        Net::getChatHandler()->privateMessage(nick, data);
    else
        chatWindow->whisper(nick, data, BY_PLAYER);
//was true
}

void BuySellHandler::sendSellRequest(std::string nick, ShopItem* item,
                                     int amount)
{
    if (!chatWindow || nick.empty() || !item ||
        amount < 1 || amount > item->getQuantity())
    {
        return;
    }

    std::string data = strprintf("!sellitem %d %d %d",
                       item->getId(), item->getPrice(), amount);

    if (config.getBoolValue("hideShopMessages"))
        Net::getChatHandler()->privateMessage(nick, data);
    else
        chatWindow->whisper(nick, data, BY_PLAYER);
//was true
}

} // namespace TmwAthena
