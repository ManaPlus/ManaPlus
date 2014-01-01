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

#include "net/ea/tradehandler.h"

#include "inventory.h"
#include "item.h"
#include "logger.h"
#include "notifications.h"
#include "notifymanager.h"

#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/tradewindow.h"

#include "net/net.h"

#include "net/ea/eaprotocol.h"

#include "debug.h"

extern std::string tradePartnerName;
ConfirmDialog *confirmDlg = nullptr;

/**
 * Listener for request trade dialogs
 */
namespace
{
    struct RequestTradeListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent &event)
        {
            confirmDlg = nullptr;
            const std::string &eventId = event.getId();
            if (eventId == "ignore")
                player_relations.ignoreTrade(tradePartnerName);
            Net::getTradeHandler()->respond(eventId == "yes");
        }
    } listener;
}  // namespace

namespace Ea
{

TradeHandler::TradeHandler()
{
    confirmDlg = nullptr;
}

void TradeHandler::removeItem(const int slotNum A_UNUSED,
                              const int amount A_UNUSED) const
{
}

void TradeHandler::processTradeRequest(Net::MessageIn &msg) const
{
    // If a trade window or request window is already open, send a
    // trade cancel to any other trade request.
    //
    // Note that it would be nice if the server would prevent this
    // situation, and that the requesting player would get a
    // special message about the player being occupied.
    std::string tradePartnerNameTemp = msg.readString(24);

    if (player_relations.hasPermission(tradePartnerNameTemp,
        PlayerRelation::TRADE))
    {
        if (PlayerInfo::isTrading() || confirmDlg)
        {
            respond(false);
            return;
        }

        tradePartnerName = tradePartnerNameTemp;
        PlayerInfo::setTrading(true);
        if (tradeWindow)
        {
            if (tradePartnerName.empty() || tradeWindow->getAutoTradeNick()
                != tradePartnerName)
            {
                tradeWindow->clear();
                // TRANSLATORS: trade message
                confirmDlg = new ConfirmDialog(_("Request for Trade"),
                    // TRANSLATORS: trade message
                    strprintf(_("%s wants to trade with you, do"
                    " you accept?"), tradePartnerName.c_str()),
                    SOUND_REQUEST, true);
                confirmDlg->postInit();
                confirmDlg->addActionListener(&listener);
            }
            else
            {
                respond(true);
            }
        }
    }
    else
    {
        respond(false);
        return;
    }
}

void TradeHandler::processTradeResponse(Net::MessageIn &msg) const
{
    if (confirmDlg || tradePartnerName.empty()
        || !player_relations.hasPermission(tradePartnerName,
        PlayerRelation::TRADE))
    {
        respond(false);
        return;
    }

    switch (msg.readInt8())
    {
        case 0:  // Too far away
            NotifyManager::notify(NotifyManager::TRADE_FAIL_FAR_AWAY,
                tradePartnerName);
            break;
        case 1:  // Character doesn't exist
            NotifyManager::notify(NotifyManager::TRADE_FAIL_CHAR_NOT_EXISTS,
                tradePartnerName);
            break;
        case 2:  // Invite request check failed...
            NotifyManager::notify(NotifyManager::TRADE_CANCELLED_ERROR);
            break;
        case 3:  // Trade accepted
            if (tradeWindow)
            {
                tradeWindow->reset();
                // TRANSLATORS: trade header
                tradeWindow->setCaption(strprintf(_("Trade: You and %s"),
                    tradePartnerName.c_str()));
                tradeWindow->initTrade(tradePartnerName);
                tradeWindow->setVisible(true);
            }
            break;
        case 4:  // Trade cancelled
            if (player_relations.hasPermission(tradePartnerName,
                PlayerRelation::SPEECH_LOG))
            {
                NotifyManager::notify(NotifyManager::TRADE_CANCELLED_NAME,
                    tradePartnerName);
            }
            // otherwise ignore silently

            if (tradeWindow)
            {
                tradeWindow->setVisible(false);
//                        tradeWindow->clear();
            }
            PlayerInfo::setTrading(false);
            break;
        default:  // Shouldn't happen as well, but to be sure
            NotifyManager::notify(NotifyManager::TRADE_ERROR_UNKNOWN,
                tradePartnerName);
            if (tradeWindow)
                tradeWindow->clear();
            break;
    }
}

void TradeHandler::processTradeItemAdd(Net::MessageIn &msg) const
{
    const int amount = msg.readInt32();
    const int type = msg.readInt16();
    const int identify = msg.readInt8();  // identified flag
    msg.readInt8();  // attribute
    const int refine = msg.readInt8();  // refine
    msg.skip(8);     // card (4 shorts)

    if (tradeWindow)
    {
        if (type == 0)
        {
            tradeWindow->setMoney(amount);
        }
        else
        {
            tradeWindow->addItem2(type, false, amount, refine,
                static_cast<unsigned char>(identify), false);
        }
    }
}

void TradeHandler::processTradeItemAddResponse(Net::MessageIn &msg) const
{
    // Trade: New Item add response (was 0x00ea, now 01b1)
    const int index = msg.readInt16() - INVENTORY_OFFSET;
    Item *item = nullptr;
    if (PlayerInfo::getInventory())
        item = PlayerInfo::getInventory()->getItem(index);

    if (!item)
    {
        if (tradeWindow)
            tradeWindow->receivedOk(true);
        return;
    }
    const int quantity = msg.readInt16();

    const int res = msg.readInt8();
    switch (res)
    {
        case 0:
            // Successfully added item
            if (tradeWindow)
            {
                tradeWindow->addItem2(item->getId(), true,
                    quantity, item->getRefine(), item->getColor(),
                    item->isEquipment());
            }
            item->increaseQuantity(-quantity);
            break;
        case 1:
            // Add item failed - player overweighted
            NotifyManager::notify(NotifyManager::
                TRADE_ADD_PARTNER_OVER_WEIGHT);
            break;
        case 2:
            // Add item failed - player has no free slot
            NotifyManager::notify(NotifyManager::TRADE_ADD_PARTNER_NO_SLOTS);
            break;
        case 3:
            // Add item failed - non tradable item
            NotifyManager::notify(NotifyManager::TRADE_ADD_UNTRADABLE_ITEM);
            break;
        default:
            NotifyManager::notify(NotifyManager::TRADE_ADD_ERROR);
            logger->log("QQQ SMSG_TRADE_ITEM_ADD_RESPONSE: "
                        + toString(res));
            break;
    }
}

void TradeHandler::processTradeOk(Net::MessageIn &msg) const
{
    // 0 means ok from myself, 1 means ok from other;
    if (tradeWindow)
        tradeWindow->receivedOk(msg.readInt8() == 0);
    else
        msg.readInt8();
}

void TradeHandler::processTradeCancel(Net::MessageIn &msg A_UNUSED) const
{
    NotifyManager::notify(NotifyManager::TRADE_CANCELLED);
    if (tradeWindow)
    {
        tradeWindow->setVisible(false);
        tradeWindow->reset();
    }
    PlayerInfo::setTrading(false);
}

void TradeHandler::processTradeComplete(Net::MessageIn &msg A_UNUSED) const
{
    NotifyManager::notify(NotifyManager::TRADE_COMPLETE);
    if (tradeWindow)
    {
        tradeWindow->setVisible(false);
        tradeWindow->reset();
    }
    PlayerInfo::setTrading(false);
}

}  // namespace Ea
