/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "actorspritemanager.h"
#include "client.h"
#include "configuration.h"
#include "depricatedevent.h"
#include "inventory.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "gui/buydialog.h"
#include "gui/buyselldialog.h"
#include "gui/selldialog.h"
#include "gui/shopwindow.h"

#include "gui/widgets/chattab.h"

#include "net/chathandler.h"

#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{

BuySellHandler::BuySellHandler() :
    mNpcId(0),
    mBuyDialog(nullptr)
{
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
            chatWindow->addWhisper(nick, data, BY_PLAYER);
    }
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
            chatWindow->addWhisper(nick, data, BY_PLAYER);
    }
}

void BuySellHandler::sendBuyRequest(const std::string &nick,
                                    const ShopItem *const item,
                                    const int amount)
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
        chatWindow->addWhisper(nick, data, BY_PLAYER);
}

void BuySellHandler::sendSellRequest(const std::string &nick,
                                     const ShopItem *const item,
                                     const int amount)
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

void BuySellHandler::processNpcSell(Net::MessageIn &msg, int offset)
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

            const Item *const item = PlayerInfo::getInventory()->getItem(index);

            if (item && !(item->isEquipped()))
                dialog->addItem(item, value);
        }
    }
    else
    {
        SERVER_NOTICE(_("Nothing to sell."))
    }
}

void BuySellHandler::processNpcBuyResponse(Net::MessageIn &msg)
{
    if (msg.readInt8() == 0)
    {
        SERVER_NOTICE(_("Thanks for buying."))
    }
    else
    {
        // Reset player money since buy dialog already assumed purchase
        // would go fine
        if (mBuyDialog)
            mBuyDialog->setMoney(PlayerInfo::getAttribute(PlayerInfo::MONEY));
        SERVER_NOTICE(_("Unable to buy."))
    }
}

} // namespace Ea
