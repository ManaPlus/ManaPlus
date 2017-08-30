/*
 *  The ManaPlus Client
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

#include "net/eathena/buyingstorerecv.h"

#include "actormanager.h"
#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/playerinfo.h"

#include "const/net/inventory.h"

#include "enums/resources/notifytypes.h"

#include "gui/windows/buyingstoreselldialog.h"

#include "gui/widgets/createwidget.h"

#include "listeners/arrowslistener.h"
#include "listeners/buyingstoremodelistener.h"
#include "listeners/buyingstoreslotslistener.h"

#include "net/messagein.h"

#include "resources/inventory/inventory.h"

#include "resources/item/item.h"

#include "debug.h"

namespace EAthena
{

void BuyingStoreRecv::processBuyingStoreOpen(Net::MessageIn &msg)
{
    BuyingStoreSlotsListener::distributeEvent(msg.readUInt8("slots"));
}

void BuyingStoreRecv::processBuyingStoreCreateFailed(Net::MessageIn &msg)
{
    const int16_t result = msg.readInt16("result");
    const int weight = msg.readInt32("weight");
    switch (result)
    {
        case 1:
        default:
            NotifyManager::notify(NotifyTypes::BUYING_STORE_CREATE_FAILED);
            break;
        case 2:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_CREATE_FAILED_WEIGHT,
                weight);
            break;
        case 8:
            NotifyManager::notify(NotifyTypes::BUYING_STORE_CREATE_EMPTY);
            break;
    }
}

void BuyingStoreRecv::processBuyingStoreOwnItems(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 12) / 9;
    msg.readBeingId("account id");
    msg.readInt32("money limit");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("price");
        msg.readInt16("amount");
        msg.readUInt8("item type");
        msg.readInt16("item id");
    }
    PlayerInfo::enableVending(true);
    BuyingStoreModeListener::distributeEvent(true);
}

void BuyingStoreRecv::processBuyingStoreShowBoard(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("owner id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing != nullptr)
    {
        dstBeing->setBuyBoard(msg.readString(80, "shop name"));
    }
    else
    {
        msg.readString(80, "shop name");
    }
}

void BuyingStoreRecv::processBuyingStoreHideBoard(Net::MessageIn &msg)
{
    const BeingId id = msg.readBeingId("owner id");
    Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing != nullptr)
        dstBeing->setBuyBoard(std::string());
    if (dstBeing == localPlayer)
    {
        PlayerInfo::enableVending(false);
        BuyingStoreModeListener::distributeEvent(false);
    }
}

void BuyingStoreRecv::processBuyingStoreItemsList(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 16) / 9;
    const BeingId id = msg.readBeingId("account id");
    const int storeId = msg.readInt32("store id");
    // +++ in future need use it too
    msg.readInt32("money limit");

    const Being *const dstBeing = actorManager->findBeing(id);
    if (dstBeing == nullptr)
        return;

    SellDialog *const dialog = CREATEWIDGETR(BuyingStoreSellDialog,
        dstBeing->getId(),
        storeId);
    dialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));
    const Inventory *const inv = PlayerInfo::getInventory();
    for (int f = 0; f < count; f ++)
    {
        const int price = msg.readInt32("price");
        const int amount = msg.readInt16("amount");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const int itemId = msg.readInt16("item id");

        if (inv == nullptr)
            continue;
        const Item *const item = inv->findItem(itemId, ItemColor_one);
        if (item == nullptr)
            continue;
        // +++ need add colors support
        dialog->addItem(itemId, itemType, ItemColor_one, amount, price);
    }
}

void BuyingStoreRecv::processBuyingStoreSellFailed(Net::MessageIn &msg)
{
    const int16_t result = msg.readInt16("result");
    switch (result)
    {
        case 3:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELL_FAILED_MONEY_LIMIT);
            break;
        case 4:
            NotifyManager::notify(NotifyTypes::BUYING_STORE_SELL_FAILED_EMPTY);
            break;
        default:
            NotifyManager::notify(NotifyTypes::BUYING_STORE_SELL_FAILED);
            break;
    }
}

void BuyingStoreRecv::processBuyingStoreSellerSellFailed(Net::MessageIn &msg)
{
    const int16_t result = msg.readInt16("result");
    msg.readInt16("item id");
    switch (result)
    {
        case 5:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELLER_SELL_FAILED_DEAL);
            break;
        case 6:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELLER_SELL_FAILED_AMOUNT);
            break;
        case 7:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELLER_SELL_FAILED_BALANCE);
            break;
        default:
            NotifyManager::notify(
                NotifyTypes::BUYING_STORE_SELLER_SELL_FAILED);
            break;
    }
}

void BuyingStoreRecv::processBuyingStoreReport(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("item id");
    msg.readInt16("amount");
    if (msg.getVersion() >= 20141016)
    {
        msg.readInt32("money");
        msg.readInt32("money limit");
        msg.readInt32("char id");
        msg.readInt32("date");
    }
    else
    {
        msg.readInt32("money limit");
    }
}

void BuyingStoreRecv::processBuyingStoreDeleteItem(Net::MessageIn &msg)
{
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");
    msg.readInt32("price");

    if (inventory != nullptr)
    {
        if (Item *const item = inventory->getItem(index))
        {
            item->increaseQuantity(-amount);
            if (item->getQuantity() == 0)
                inventory->removeItemAt(index);
            ArrowsListener::distributeEvent();
        }
    }
}

}  // namespace EAthena
