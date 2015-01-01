/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/tmwa/tradehandler.h"

#include "inventory.h"
#include "item.h"
#include "notifymanager.h"

#include "being/playerinfo.h"
#include "being/playerrelations.h"

#include "gui/windows/confirmdialog.h"
#include "gui/windows/tradewindow.h"

#include "net/serverfeatures.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "net/ea/eaprotocol.h"

#include "resources/notifytypes.h"

#include "utils/stringutils.h"

#include "debug.h"

extern Net::TradeHandler *tradeHandler;

extern std::string tradePartnerName;
extern ConfirmDialog *confirmDlg;

namespace TmwAthena
{

TradeHandler::TradeHandler() :
    MessageHandler(),
    Ea::TradeHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_TRADE_REQUEST,
        SMSG_TRADE_RESPONSE,
        SMSG_TRADE_ITEM_ADD,
        SMSG_TRADE_ITEM_ADD_RESPONSE,
        SMSG_TRADE_OK,
        SMSG_TRADE_CANCEL,
        SMSG_TRADE_COMPLETE,
        0
    };
    handledMessages = _messages;
    tradeHandler = this;
}


void TradeHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("TradeHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_TRADE_REQUEST:
            processTradeRequest(msg);
            break;

        case SMSG_TRADE_RESPONSE:
            processTradeResponse(msg);
            break;

        case SMSG_TRADE_ITEM_ADD:
            processTradeItemAdd(msg);
            break;

        case SMSG_TRADE_ITEM_ADD_RESPONSE:
            processTradeItemAddResponse(msg);
            break;

        case SMSG_TRADE_OK:
            processTradeOk(msg);
            break;

        case SMSG_TRADE_CANCEL:
            processTradeCancel(msg);
            break;

        case SMSG_TRADE_COMPLETE:
            processTradeComplete(msg);
            break;

        default:
            break;
    }
    BLOCK_END("TradeHandler::handleMessage")
}

void TradeHandler::request(const Being *const being) const
{
    if (!being)
        return;

    createOutPacket(CMSG_TRADE_REQUEST);
    outMsg.writeInt32(being->getId(), "player id");
}

void TradeHandler::respond(const bool accept) const
{
    if (!accept)
        PlayerInfo::setTrading(false);

    createOutPacket(CMSG_TRADE_RESPONSE);
    outMsg.writeInt8(static_cast<int8_t>(accept ? 3 : 4), "accept");
}

void TradeHandler::addItem(const Item *const item, const int amount) const
{
    if (!item)
        return;

    createOutPacket(CMSG_TRADE_ITEM_ADD_REQUEST);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt32(amount, "amount");
}

void TradeHandler::setMoney(const int amount) const
{
    createOutPacket(CMSG_TRADE_ITEM_ADD_REQUEST);
    outMsg.writeInt16(0, "index");
    outMsg.writeInt32(amount, "amount");
}

void TradeHandler::confirm() const
{
    createOutPacket(CMSG_TRADE_ADD_COMPLETE);
}

void TradeHandler::finish() const
{
    createOutPacket(CMSG_TRADE_OK);
}

void TradeHandler::cancel() const
{
    createOutPacket(CMSG_TRADE_CANCEL_REQUEST);
}

void TradeHandler::processTradeRequest(Net::MessageIn &msg)
{
    processTradeRequestContinue(msg.readString(24, "name"));
}

void TradeHandler::processTradeItemAdd(Net::MessageIn &msg)
{
    const int amount = msg.readInt32("amount");
    const int type = msg.readInt16("type");
    const uint8_t identify = msg.readUInt8("identify");
    msg.readUInt8("attribute");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[4];
    for (int f = 0; f < 4; f++)
        cards[f] = msg.readInt16("card");

    if (tradeWindow)
    {
        if (type == 0)
        {
            tradeWindow->setMoney(amount);
        }
        else
        {
            if (serverFeatures->haveItemColors())
            {
                tradeWindow->addItem2(type, 0,
                    cards, 4,
                    false, amount,
                    refine, identify, true, false, false, false);
            }
            else
            {
                tradeWindow->addItem2(type, 0,
                    cards, 4,
                    false, amount,
                    refine, 1, identify != 0, false, false, false);
            }
        }
    }
}

void TradeHandler::processTradeItemAddResponse(Net::MessageIn &msg)
{
    // Trade: New Item add response (was 0x00ea, now 01b1)
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    Item *item = nullptr;
    if (PlayerInfo::getInventory())
        item = PlayerInfo::getInventory()->getItem(index);

    if (!item)
    {
        if (tradeWindow)
            tradeWindow->receivedOk(true);
        return;
    }
    const int quantity = msg.readInt16("amount");

    const uint8_t res = msg.readUInt8("status");
    switch (res)
    {
        case 0:
            // Successfully added item
            if (tradeWindow)
            {
                tradeWindow->addItem2(item->getId(), item->getType(),
                    item->getCards(), 4,
                    true, quantity, item->getRefine(), item->getColor(),
                    item->getIdentified(), item->getDamaged(),
                    item->getFavorite(), item->isEquipment());
            }
            item->increaseQuantity(-quantity);
            break;
        case 1:
            // Add item failed - player overweighted
            NotifyManager::notify(NotifyTypes::
                TRADE_ADD_PARTNER_OVER_WEIGHT);
            break;
        case 2:
            // Add item failed - player has no free slot
            NotifyManager::notify(NotifyTypes::TRADE_ADD_PARTNER_NO_SLOTS);
            break;
        case 3:
            // Add item failed - non tradable item
            NotifyManager::notify(NotifyTypes::TRADE_ADD_UNTRADABLE_ITEM);
            break;
        default:
            NotifyManager::notify(NotifyTypes::TRADE_ADD_ERROR);
            logger->log("QQQ SMSG_TRADE_ITEM_ADD_RESPONSE: "
                        + toString(res));
            break;
    }
}

void TradeHandler::processTradeResponse(Net::MessageIn &msg)
{
    if (confirmDlg || tradePartnerName.empty()
        || !player_relations.hasPermission(tradePartnerName,
        PlayerRelation::TRADE))
    {
        tradeHandler->respond(false);
        return;
    }
    const uint8_t type = msg.readUInt8("type");
    processTradeResponseContinue(type);
}

}  // namespace TmwAthena
