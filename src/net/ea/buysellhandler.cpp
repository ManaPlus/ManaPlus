/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "net/ea/buysellhandler.h"

#include "configuration.h"
#include "inventory.h"
#include "notifications.h"
#include "notifymanager.h"

#include "being/playerinfo.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/buyselldialog.h"
#include "gui/windows/selldialog.h"

#include "gui/windows/buydialog.h"
#include "gui/windows/shopwindow.h"

#include "net/chathandler.h"
#include "net/net.h"

#include "net/ea/eaprotocol.h"

#include "utils/timer.h"

#include "debug.h"

namespace Ea
{

BuySellHandler::BuySellHandler() :
    mNpcId(0),
    mBuyDialog(nullptr)
{
}

void BuySellHandler::requestSellList(const std::string &nick) const
{
    if (nick.empty() != 0 || !shopWindow)
        return;

    const std::string data("!selllist " + toString(tick_time));
    shopWindow->setAcceptPlayer(nick);

    if (config.getBoolValue("hideShopMessages"))
    {
        Net::getChatHandler()->privateMessage(nick, data);
    }
    else
    {
        if (chatWindow)
            chatWindow->addWhisper(nick, data, BY_PLAYER);
    }
}

void BuySellHandler::requestBuyList(const std::string &nick) const
{
    if (nick.empty() || !shopWindow)
        return;

    const std::string data("!buylist " + toString(tick_time));
    shopWindow->setAcceptPlayer(nick);

    if (config.getBoolValue("hideShopMessages"))
    {
        Net::getChatHandler()->privateMessage(nick, data);
    }
    else
    {
        if (chatWindow)
            chatWindow->addWhisper(nick, data, BY_PLAYER);
    }
}

void BuySellHandler::sendBuyRequest(const std::string &nick,
                                    const ShopItem *const item,
                                    const int amount) const
{
    if (!chatWindow || nick.empty() || !item ||
        amount < 1 || amount > item->getQuantity())
    {
        return;
    }
    const std::string data = strprintf("!buyitem %d %d %d",
        item->getId(), item->getPrice(), amount);

    if (config.getBoolValue("hideShopMessages"))
        Net::getChatHandler()->privateMessage(nick, data);
    else
        chatWindow->addWhisper(nick, data, BY_PLAYER);
}

void BuySellHandler::sendSellRequest(const std::string &nick,
                                     const ShopItem *const item,
                                     const int amount) const
{
    if (!chatWindow || nick.empty() || !item ||
        amount < 1 || amount > item->getQuantity())
    {
        return;
    }

    const std::string data = strprintf("!sellitem %d %d %d",
        item->getId(), item->getPrice(), amount);

    if (config.getBoolValue("hideShopMessages"))
        Net::getChatHandler()->privateMessage(nick, data);
    else
        chatWindow->addWhisper(nick, data, BY_PLAYER);
}

void BuySellHandler::processNpcBuySellChoice(Net::MessageIn &msg)
{
    if (!BuySellDialog::isActive())
    {
        mNpcId = msg.readInt32();
        new BuySellDialog(mNpcId);
    }
}

void BuySellHandler::processNpcSell(Net::MessageIn &msg,
                                    const int offset) const
{
    msg.readInt16();  // length
    const int n_items = (msg.getLength() - 4) / 10;
    if (n_items > 0)
    {
        SellDialog *const dialog = new SellDialog(mNpcId);
        dialog->setMoney(PlayerInfo::getAttribute(PlayerInfo::MONEY));

        for (int k = 0; k < n_items; k++)
        {
            const int index = msg.readInt16() - offset;
            const int value = msg.readInt32();
            msg.readInt32();  // value

            const Item *const item = PlayerInfo::getInventory()
                ->getItem(index);

            if (item && !(item->isEquipped()))
                dialog->addItem(item, value);
        }
    }
    else
    {
        NotifyManager::notify(NotifyManager::SELL_LIST_EMPTY);
    }
}

void BuySellHandler::processNpcBuyResponse(Net::MessageIn &msg) const
{
    if (msg.readInt8() == 0)
    {
        NotifyManager::notify(NotifyManager::BUY_DONE);
    }
    else
    {
        // Reset player money since buy dialog already assumed purchase
        // would go fine
        if (mBuyDialog)
            mBuyDialog->setMoney(PlayerInfo::getAttribute(PlayerInfo::MONEY));
        NotifyManager::notify(NotifyManager::BUY_FAILED);
    }
}

}  // namespace Ea
