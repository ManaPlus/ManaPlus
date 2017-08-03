/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/tmwa/buysellhandler.h"

#include "configuration.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/shopwindow.h"

#include "net/chathandler.h"

#include "net/ea/buysellrecv.h"

#include "utils/timer.h"

#include "debug.h"

namespace TmwAthena
{

BuySellHandler::BuySellHandler() :
    Ea::BuySellHandler()
{
    buySellHandler = this;
    Ea::BuySellRecv::mBuyDialog = nullptr;
}

BuySellHandler::~BuySellHandler()
{
    buySellHandler = nullptr;
}

void BuySellHandler::requestSellList(const std::string &nick) const
{
    if (nick.empty() || shopWindow == nullptr)
        return;

    const std::string data("!selllist " + toString(tick_time));
    shopWindow->setAcceptPlayer(nick);
    if (config.getBoolValue("hideShopMessages"))
    {
        chatHandler->privateMessage(nick, data);
    }
    else
    {
        if (chatWindow != nullptr)
            chatWindow->addWhisper(nick, data, ChatMsgType::BY_PLAYER);
    }
}

void BuySellHandler::requestBuyList(const std::string &nick) const
{
    if (nick.empty() || (shopWindow == nullptr))
        return;

    const std::string data("!buylist " + toString(tick_time));
    shopWindow->setAcceptPlayer(nick);

    if (config.getBoolValue("hideShopMessages"))
    {
        chatHandler->privateMessage(nick, data);
    }
    else
    {
        if (chatWindow != nullptr)
            chatWindow->addWhisper(nick, data, ChatMsgType::BY_PLAYER);
    }
}

void BuySellHandler::sendBuyRequest(const std::string &nick,
                                    const ShopItem *const item,
                                    const int amount) const
{
    if ((chatWindow == nullptr) || nick.empty() || (item == nullptr) ||
        amount < 1 || amount > item->getQuantity())
    {
        return;
    }
    const std::string data = strprintf("!buyitem %d %d %d",
        item->getId(), item->getPrice(), amount);

    if (config.getBoolValue("hideShopMessages"))
        chatHandler->privateMessage(nick, data);
    else
        chatWindow->addWhisper(nick, data, ChatMsgType::BY_PLAYER);
}

void BuySellHandler::sendSellRequest(const std::string &nick,
                                     const ShopItem *const item,
                                     const int amount) const
{
    if ((chatWindow == nullptr) || nick.empty() || (item == nullptr) ||
        amount < 1 || amount > item->getQuantity())
    {
        return;
    }

    const std::string data = strprintf("!sellitem %d %d %d",
        item->getId(), item->getPrice(), amount);

    if (config.getBoolValue("hideShopMessages"))
        chatHandler->privateMessage(nick, data);
    else
        chatWindow->addWhisper(nick, data, ChatMsgType::BY_PLAYER);
}

void BuySellHandler::close() const
{
}

}  // namespace TmwAthena
