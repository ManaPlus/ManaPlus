/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/eathena/inventoryrecv.h"

#include "notifymanager.h"
#include "itemcolormanager.h"

#include "being/localplayer.h"

#include "const/net/inventory.h"

#include "enums/resources/notifytypes.h"

#include "enums/net/deleteitemreason.h"

#include "gui/popups/itempopup.h"

#include "gui/widgets/createwidget.h"

#include "gui/windows/insertcarddialog.h"

#include "listeners/arrowslistener.h"

#include "net/inventoryhandler.h"

#include "net/messagein.h"

#include "net/eathena/itemflags.h"
#include "net/eathena/menu.h"

#include "net/ea/equipbackend.h"
#include "net/ea/inventoryrecv.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

extern int packetVersion;
extern int serverVersion;

namespace EAthena
{

namespace InventoryRecv
{
    Ea::InventoryItems mCartItems;
}

void InventoryRecv::processPlayerEquipment(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerEquipment")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    msg.readInt16("len");
    Equipment *const equipment = PlayerInfo::getEquipment();
    if (equipment && !equipment->getBackend())
    {   // look like SMSG_PLAYER_INVENTORY was not received
        Ea::InventoryRecv::mEquips.clear();
        equipment->setBackend(&Ea::InventoryRecv::mEquips);
    }
    int sz;
    if (serverVersion >= 8 && packetVersion >= 20150226)
        sz = 57;
    else
        sz = 31;

    const int number = (msg.getLength() - 4) / sz;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        msg.readInt32("location");
        const int equipType = msg.readInt32("wear state");
        const uint8_t refine = CAST_U8(msg.readInt8("refine"));
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        msg.readInt16("equip type");
        msg.readInt16("item sprite number");
        if (serverVersion >= 8 && packetVersion >= 20150226)
        {
            msg.readUInt8("rnd count");
            for (int f = 0; f < 5; f ++)
            {
                msg.readInt16("rnd index");
                msg.readInt16("rnd value");
                msg.readUInt8("rnd param");
            }
        }
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");
        if (inventory)
        {
            inventory->setItem(index,
                itemId,
                itemType,
                1,
                refine,
                ItemColorManager::getColorFromCards(&cards[0]),
                fromBool(flags.bits.isIdentified, Identified),
                fromBool(flags.bits.isDamaged, Damaged),
                fromBool(flags.bits.isFavorite, Favorite),
                Equipm_true,
                Equipped_false);
            inventory->setCards(index, cards, 4);
        }

        if (equipType)
        {
            Ea::InventoryRecv::mEquips.setEquipment(
                Ea::InventoryRecv::getSlot(equipType), index);
        }
    }
    BLOCK_END("InventoryRecv::processPlayerEquipment")
}

void InventoryRecv::processPlayerInventoryAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventoryAdd")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    if (PlayerInfo::getEquipment()
        && !PlayerInfo::getEquipment()->getBackend())
    {   // look like SMSG_PLAYER_INVENTORY was not received
        Ea::InventoryRecv::mEquips.clear();
        PlayerInfo::getEquipment()->setBackend(&Ea::InventoryRecv::mEquips);
    }
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    int amount = msg.readInt16("count");
    const int itemId = msg.readInt16("item id");
    uint8_t identified = msg.readUInt8("identified");
    const uint8_t damaged = msg.readUInt8("is damaged");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readInt16("card");
    const int equipType = msg.readInt32("location");
    const int itemType = msg.readUInt8("item type");
    const unsigned char err = msg.readUInt8("result");
    msg.readInt32("hire expire date");
    msg.readInt16("bind on equip");
    if (serverVersion >= 8 && packetVersion >= 20150226)
    {
        for (int f = 0; f < 5; f ++)
        {
            msg.readInt16("rnd index");
            msg.readInt16("rnd value");
            msg.readUInt8("rnd param");
        }
    }

    const ItemColor color = ItemColorManager::getColorFromCards(&cards[0]);
    const ItemInfo &itemInfo = ItemDB::get(itemId);
    BeingId floorId;
    if (Ea::InventoryRecv::mSentPickups.empty())
    {
        floorId = BeingId_zero;
    }
    else
    {
        floorId = Ea::InventoryRecv::mSentPickups.front();
        Ea::InventoryRecv::mSentPickups.pop();
    }

    if (err)
    {
        PickupT pickup;
        switch (err)
        {
            case 1:
                pickup = Pickup::BAD_ITEM;
                break;
            case 2:
                pickup = Pickup::TOO_HEAVY;
                break;
            case 4:
                pickup = Pickup::INV_FULL;
                break;
            case 5:
                pickup = Pickup::MAX_AMOUNT;
                break;
            case 6:
                pickup = Pickup::TOO_FAR;
                break;
            case 7:
                pickup = Pickup::STACK_AMOUNT;
                break;
            default:
                pickup = Pickup::UNKNOWN;
                UNIMPLIMENTEDPACKET;
                break;
        }
        if (localPlayer)
        {
            localPlayer->pickedUp(itemInfo,
                0,
                color,
                floorId,
                pickup);
        }
    }
    else
    {
        if (localPlayer)
        {
            localPlayer->pickedUp(itemInfo,
                amount,
                color,
                floorId,
                Pickup::OKAY);
        }

        if (inventory)
        {
            const Item *const item = inventory->getItem(index);

            if (item && item->getId() == itemId)
                amount += item->getQuantity();

            inventory->setItem(index,
                itemId,
                itemType,
                amount,
                refine,
                color,
                fromBool(identified, Identified),
                fromBool(damaged, Damaged),
                Favorite_false,
                fromBool(equipType, Equipm),
                Equipped_false);
            inventory->setCards(index, cards, 4);
        }
        ArrowsListener::distributeEvent();
    }
    BLOCK_END("InventoryRecv::processPlayerInventoryAdd")
}

void InventoryRecv::processPlayerInventory(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventory")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;
    if (PlayerInfo::getEquipment())
    {
        // Clear inventory - this will be a complete refresh
        Ea::InventoryRecv::mEquips.clear();
        PlayerInfo::getEquipment()->setBackend(&Ea::InventoryRecv::mEquips);
    }

    if (inventory)
        inventory->clear();

    msg.readInt16("len");

    const int number = (msg.getLength() - 4) / 24;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("item index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        const int amount = msg.readInt16("count");
        msg.readInt32("wear state / equip");
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        if (inventory)
        {
            inventory->setItem(index,
                itemId,
                itemType,
                amount,
                0,
                ItemColorManager::getColorFromCards(&cards[0]),
                fromBool(flags.bits.isIdentified, Identified),
                fromBool(flags.bits.isDamaged, Damaged),
                fromBool(flags.bits.isFavorite, Favorite),
                Equipm_false,
                Equipped_false);
            inventory->setCards(index, cards, 4);
        }
    }
    BLOCK_END("InventoryRecv::processPlayerInventory")
}

void InventoryRecv::processPlayerStorage(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventory")
    Ea::InventoryRecv::mInventoryItems.clear();

    msg.readInt16("len");
    msg.readString(24, "storage name");

    const int number = (msg.getLength() - 4 - 24) / 24;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("item index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        const int amount = msg.readInt16("count");
        msg.readInt32("wear state / equip");
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        Ea::InventoryRecv::mInventoryItems.push_back(Ea::InventoryItem(
            index,
            itemId,
            itemType,
            cards,
            amount,
            0,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(flags.bits.isIdentified, Identified),
            fromBool(flags.bits.isDamaged, Damaged),
            fromBool(flags.bits.isFavorite, Favorite),
            Equipm_false));
    }
    BLOCK_END("InventoryRecv::processPlayerInventory")
}

void InventoryRecv::processPlayerEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerEquip")
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int equipType = msg.readInt32("wear location");
    msg.readInt16("sprite");
    const uint8_t flag = msg.readUInt8("result");

    switch (flag)
    {
        case 0:
            Ea::InventoryRecv::mEquips.setEquipment(
                Ea::InventoryRecv::getSlot(equipType), index);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::EQUIP_FAILED_LEVEL);
            break;

        case 2:
        default:
            NotifyManager::notify(NotifyTypes::EQUIP_FAILED);
            break;
    }
    BLOCK_END("InventoryRecv::processPlayerEquip")
}

void InventoryRecv::processPlayerUnEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerUnEquip")
    msg.readInt16("index");
    const int equipType = msg.readInt32("wear location");
    const uint8_t flag = msg.readUInt8("result");

    if (flag)
    {
        NotifyManager::notify(NotifyTypes::UNEQUIP_FAILED);
    }
    else
    {
        Ea::InventoryRecv::mEquips.setEquipment(
            Ea::InventoryRecv::getSlot(equipType), -1);
    }
    if (equipType & 0x8000)
        ArrowsListener::distributeEvent();
    BLOCK_END("InventoryRecv::processPlayerUnEquip")
}

void InventoryRecv::processPlayerInventoryRemove2(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventoryRemove2")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    const DeleteItemReasonT reason = static_cast<DeleteItemReasonT>(
        msg.readInt16("reason"));
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");

    if (inventory)
    {
        if (Item *const item = inventory->getItem(index))
        {
            switch (reason)
            {
                case DeleteItemReason::Normal:
                    NotifyManager::notify(NotifyTypes::DELETE_ITEM_NORMAL,
                        item->getName());
                    break;
                case DeleteItemReason::SkillUse:
                    NotifyManager::notify(NotifyTypes::DELETE_ITEM_SKILL_USE,
                        item->getName());
                    break;
                case DeleteItemReason::FailRefine:
                    NotifyManager::notify(NotifyTypes::DELETE_ITEM_FAIL_REFINE,
                        item->getName());
                    break;
                case DeleteItemReason::MaterialChange:
                    NotifyManager::notify(
                        NotifyTypes::DELETE_ITEM_MATERIAL_CHANGE,
                        item->getName());
                    break;
                case DeleteItemReason::ToStorage:
                    NotifyManager::notify(NotifyTypes::DELETE_ITEM_TO_STORAGE,
                        item->getName());
                    break;
                case DeleteItemReason::ToCart:
                    NotifyManager::notify(NotifyTypes::DELETE_ITEM_TO_CART,
                        item->getName());
                    break;
                case DeleteItemReason::Sold:
                    NotifyManager::notify(NotifyTypes::DELETE_ITEM_SOLD,
                        item->getName());
                    break;
                case DeleteItemReason::Analysis:
                    NotifyManager::notify(NotifyTypes::DELETE_ITEM_ANALYSIS,
                        item->getName());
                    break;
                default:
                    NotifyManager::notify(NotifyTypes::DELETE_ITEM_UNKNOWN,
                        item->getName());
                    break;
            }

            item->increaseQuantity(-amount);
            if (item->getQuantity() == 0)
                inventory->removeItemAt(index);
            ArrowsListener::distributeEvent();
        }
    }
    BLOCK_END("InventoryRecv::processPlayerInventoryRemove2")
}

void InventoryRecv::processPlayerStorageEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerStorageEquip")
    msg.readInt16("len");
    int sz;
    if (serverVersion >= 8 && packetVersion >= 20150226)
        sz = 57;
    else
        sz = 31;
    const int number = (msg.getLength() - 4 - 24) / sz;

    msg.readString(24, "storage name");
    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        const int amount = 1;
        msg.readInt32("location");
        msg.readInt32("wear state");
        const uint8_t refine = msg.readUInt8("refine level");
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date");
        msg.readInt16("bind on equip");
        msg.readInt16("sprite");
        if (serverVersion >= 8 && packetVersion >= 20150226)
        {
            msg.readUInt8("rnd count");
            for (int f = 0; f < 5; f ++)
            {
                msg.readInt16("rnd index");
                msg.readInt16("rnd value");
                msg.readUInt8("rnd param");
            }
        }

        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        Ea::InventoryRecv::mInventoryItems.push_back(Ea::InventoryItem(
            index,
            itemId,
            itemType,
            cards,
            amount,
            refine,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(flags.bits.isIdentified, Identified),
            fromBool(flags.bits.isDamaged, Damaged),
            fromBool(flags.bits.isFavorite, Favorite),
            Equipm_false));
    }
    BLOCK_END("InventoryRecv::processPlayerStorageEquip")
}

void InventoryRecv::processPlayerStorageAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerStorageAdd")
    // Move an item into storage
    const int index = msg.readInt16("index") - STORAGE_OFFSET;
    const int amount = msg.readInt32("amount");
    const int itemId = msg.readInt16("item id");
    const int itemType = msg.readUInt8("type");
    const unsigned char identified = msg.readUInt8("identify");
    const Damaged damaged = fromBool(msg.readUInt8("attribute"), Damaged);
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readInt16("card");
    if (serverVersion >= 8 && packetVersion >= 20150226)
    {
        for (int f = 0; f < 5; f ++)
        {
            msg.readInt16("rnd index");
            msg.readInt16("rnd value");
            msg.readUInt8("rnd param");
        }
    }

    const ItemColor color = ItemColorManager::getColorFromCards(&cards[0]);
    if (Item *const item = Ea::InventoryRecv::mStorage->getItem(index))
    {
        item->setId(itemId, color);
        item->increaseQuantity(amount);
    }
    else
    {
        if (Ea::InventoryRecv::mStorage)
        {
            Ea::InventoryRecv::mStorage->setItem(index,
                itemId,
                itemType,
                amount,
                refine,
                color,
                fromBool(identified, Identified),
                damaged,
                Favorite_false,
                Equipm_false,
                Equipped_false);
            Ea::InventoryRecv::mStorage->setCards(index, cards, 4);
        }
    }
    BLOCK_END("InventoryRecv::processPlayerStorageAdd")
}

void InventoryRecv::processPlayerUseCard(Net::MessageIn &msg)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    const int index = inventoryHandler->getItemIndex();
    const Item *item1 = nullptr;
    if (inv)
        item1 = inv->getItem(index);
    SellDialog *const dialog = CREATEWIDGETR(InsertCardDialog,
        index, item1);

    const int count = (msg.readInt16("len") - 4) / 2;
    for (int f = 0; f < count; f ++)
    {
        const int itemIndex = msg.readInt16("item index") - INVENTORY_OFFSET;
        if (!inv)
            continue;
        const Item *const item = inv->getItem(itemIndex);
        if (!item)
            continue;
        dialog->addItem(item, 0);
    }
}

void InventoryRecv::processPlayerInsertCard(Net::MessageIn &msg)
{
    const int itemIndex = msg.readInt16("item index") - INVENTORY_OFFSET;
    const int cardIndex = msg.readInt16("card index") - INVENTORY_OFFSET;
    if (msg.readUInt8("flag"))
    {
        NotifyManager::notify(NotifyTypes::CARD_INSERT_FAILED);
    }
    else
    {
        NotifyManager::notify(NotifyTypes::CARD_INSERT_SUCCESS);
        Inventory *const inv = PlayerInfo::getInventory();
        if (!inv)
            return;
        Item *const card = inv->getItem(cardIndex);
        int cardId = 0;
        if (card)
        {
            cardId = card->getId();
            card->increaseQuantity(-1);
            if (card->getQuantity() == 0)
                inv->removeItemAt(cardIndex);
        }
        Item *const item = inv->getItem(itemIndex);
        if (item)
        {
            item->addCard(cardId);
            item->updateColor();
            itemPopup->resetPopup();
        }
    }
}

void InventoryRecv::processPlayerItemRentalTime(Net::MessageIn &msg)
{
    const int id = msg.readInt16("item id");
    const int seconds = msg.readInt32("seconds");
    const ItemInfo &info = ItemDB::get(id);
    const std::string timeStr = timeDiffToString(seconds);
    NotifyManager::notify(NotifyTypes::RENTAL_TIME_LEFT,
        // TRANSLATORS: notification message
        strprintf(_("Left %s rental time for item %s."),
        timeStr.c_str(), info.getName().c_str()));
}

void InventoryRecv::processPlayerItemRentalExpired(Net::MessageIn &msg)
{
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int id = msg.readInt16("item id");
    const ItemInfo &info = ItemDB::get(id);

    NotifyManager::notify(NotifyTypes::RENTAL_TIME_EXPIRED,
        info.getName());
    if (inventory)
    {
        if (Item *const item = inventory->getItem(index))
        {
            item->increaseQuantity(-item->getQuantity());
            inventory->removeItemAt(index);
            ArrowsListener::distributeEvent();
        }
    }
}

void InventoryRecv::processPlayerStorageRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerStorageRemove")
    // Move an item out of storage
    const int index = msg.readInt16("index") - STORAGE_OFFSET;
    const int amount = msg.readInt32("amount");
    if (Ea::InventoryRecv::mStorage)
    {
        if (Item *const item = Ea::InventoryRecv::mStorage->getItem(index))
        {
            item->increaseQuantity(-amount);
            if (item->getQuantity() == 0)
                Ea::InventoryRecv::mStorage->removeItemAt(index);
        }
    }
    BLOCK_END("InventoryRecv::processPlayerStorageRemove")
}

void InventoryRecv::processCartInfo(Net::MessageIn &msg)
{
    msg.readInt16("cart items used");
    const int size = msg.readInt16("max cart items");
    PlayerInfo::setAttribute(Attributes::CART_TOTAL_WEIGHT,
        msg.readInt32("cart weight"));
    PlayerInfo::setAttribute(Attributes::CART_MAX_WEIGHT,
        msg.readInt32("max cart weight"));
    if (mCartItems.empty())
        return;

    Inventory *const inv = PlayerInfo::getCartInventory();
    if (!inv)
        return;

    inv->resize(size);

    FOR_EACH (Ea::InventoryItems::const_iterator, it, mCartItems)
    {
        inv->setItem((*it).slot,
            (*it).id,
            (*it).type,
            (*it).quantity,
            (*it).refine,
            (*it).color,
            (*it).identified,
            (*it).damaged,
            (*it).favorite,
            (*it).equip,
            Equipped_false);
    }
    mCartItems.clear();
}

void InventoryRecv::processCartRemove(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    // +++ need close or clear cart?
}

void InventoryRecv::processPlayerCartAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerCartAdd")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getCartInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    int amount = msg.readInt32("count");
    const int itemId = msg.readInt16("item id");
    const int itemType = msg.readUInt8("item type");
    uint8_t identified = msg.readUInt8("identified");
    const Damaged damaged = fromBool(msg.readUInt8("attribute"), Damaged);
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readInt16("card");
    if (serverVersion >= 8 && packetVersion >= 20150226)
    {
        for (int f = 0; f < 5; f ++)
        {
            msg.readInt16("rnd index");
            msg.readInt16("rnd value");
            msg.readUInt8("rnd param");
        }
    }

    // check what cart was created, if not add delayed items
    if (inventory && inventory->getSize() > 0)
    {
        const Item *const item = inventory->getItem(index);

        if (item && item->getId() == itemId)
            amount += item->getQuantity();

        inventory->setItem(index,
            itemId,
            itemType,
            amount,
            refine,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(identified, Identified),
            damaged,
            Favorite_false,
            Equipm_false,
            Equipped_false);
        inventory->setCards(index, cards, 4);
    }
    else
    {
        mCartItems.push_back(Ea::InventoryItem(index,
            itemId,
            itemType,
            cards,
            amount,
            refine,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(identified, Identified),
            damaged,
            Favorite_false,
            Equipm_false));
    }
    BLOCK_END("InventoryRecv::processPlayerCartAdd")
}

void InventoryRecv::processPlayerCartEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerCartEquip")
    msg.readInt16("len");
    int sz;
    if (serverVersion >= 8 && packetVersion >= 20150226)
        sz = 57;
    else
        sz = 31;

    const int number = (msg.getLength() - 4) / sz;
    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        const int amount = 1;
        msg.readInt32("location");
        msg.readInt32("wear state");
        const uint8_t refine = msg.readUInt8("refine level");
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date");
        msg.readInt16("bind on equip");
        msg.readInt16("sprite");
        if (serverVersion >= 8 && packetVersion >= 20150226)
        {
            msg.readUInt8("rnd count");
            for (int f = 0; f < 5; f ++)
            {
                msg.readInt16("rnd index");
                msg.readInt16("rnd value");
                msg.readUInt8("rnd param");
            }
        }
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        mCartItems.push_back(Ea::InventoryItem(index,
            itemId,
            itemType,
            cards,
            amount,
            refine,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(flags.bits.isIdentified, Identified),
            fromBool(flags.bits.isDamaged, Damaged),
            fromBool(flags.bits.isFavorite, Favorite),
            Equipm_false));
    }
    BLOCK_END("InventoryRecv::processPlayerCartEquip")
}

void InventoryRecv::processPlayerCartItems(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerCartItems")
    Ea::InventoryRecv::mInventoryItems.clear();

    msg.readInt16("len");
    const int number = (msg.getLength() - 4) / 23;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("item index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        const int amount = msg.readInt16("count");
        msg.readInt32("wear state / equip");
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        mCartItems.push_back(Ea::InventoryItem(index,
            itemId,
            itemType,
            cards,
            amount,
            0,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(flags.bits.isIdentified, Identified),
            fromBool(flags.bits.isDamaged, Damaged),
            fromBool(flags.bits.isFavorite, Favorite),
            Equipm_false));
    }
    BLOCK_END("InventoryRecv::processPlayerCartItems")
}

void InventoryRecv::processPlayerCartRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerCartRemove")
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt32("amount");

    Inventory *const inv = PlayerInfo::getCartInventory();
    if (!inv)
        return;

    if (Item *const item = inv->getItem(index))
    {
        item->increaseQuantity(-amount);
        if (item->getQuantity() == 0)
            inv->removeItemAt(index);
    }
    BLOCK_END("InventoryRecv::processPlayerCartRemove")
}

void InventoryRecv::processPlayerIdentifyList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    menu = MenuType::Identify;
    const int count = msg.readInt16("len") - 4;
    for (int f = 0; f < count; f ++)
        msg.readInt16("inv index");
}

void InventoryRecv::processPlayerIdentified(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("inv index");
    msg.readUInt8("flag");
}

void InventoryRecv::processPlayerRefine(Net::MessageIn &msg)
{
    const int flag = msg.readInt16("flag");
    const int index = msg.readInt16("inv index") - INVENTORY_OFFSET;
    msg.readInt16("refine");
    const Inventory *const inv = PlayerInfo::getInventory();
    const Item *item = nullptr;
    int notifyType;
    std::string itemName;
    if (inv)
        item = inv->getItem(index);
    if (item)
    {
        itemName = item->getName();
    }
    else
    {
        // TRANSLATORS: unknown item
        itemName = _("Unknown item");
    }
    switch (flag)
    {
        case 0:
            notifyType = NotifyTypes::REFINE_SUCCESS;
            break;
        case 1:
            notifyType = NotifyTypes::REFINE_FAILURE;
            break;
        case 2:
            notifyType = NotifyTypes::REFINE_DOWNGRADE;
            break;
        default:
            UNIMPLIMENTEDPACKET;
            notifyType = NotifyTypes::REFINE_UNKNOWN;
            break;
    }
    NotifyManager::notify(notifyType, itemName);
}

void InventoryRecv::processPlayerRepairList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / 13;
    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("index");
        msg.readInt16("item id");
        msg.readUInt8("refine");
        for (int d = 0; d < maxCards; d ++)
            msg.readInt16("card");
    }
    menu = MenuType::RepairWespon;
}

void InventoryRecv::processPlayerRepairEffect(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("item index");
    msg.readUInt8("flag");
}

void InventoryRecv::processPlayerRefineList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / 13;

    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("item index");
        msg.readInt16("item id");
        msg.readUInt8("refine");
        for (int d = 0; d < maxCards; d ++)
            msg.readInt16("card");
    }
    menu = MenuType::WeaponeRefine;
}

void InventoryRecv::processPlayerStoragePassword(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("info");
}

void InventoryRecv::processPlayerStoragePasswordResult(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("result");
    msg.readInt16("error count");
}

void InventoryRecv::processPlayerCookingList(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    const int count = (msg.readInt16("len") - 6) / 2;
    msg.readInt16("list type");
    for (int f = 0; f < count; f ++)
        msg.readInt16("item id");
}

void InventoryRecv::processItemDamaged(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("position");
    msg.readBeingId("account id");
}

void InventoryRecv::processFavoriteItem(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("item index");
    msg.readUInt8("favorite (0 - favorite)");
}

void InventoryRecv::processCartAddError(Net::MessageIn &msg)
{
    switch (msg.readUInt8("flag"))
    {
        case 0:
            NotifyManager::notify(NotifyTypes::CART_ADD_WEIGHT_ERROR);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::CART_ADD_COUNT_ERROR);
            break;
        default:
            break;
    }
}

void InventoryRecv::processBindItem(Net::MessageIn &msg)
{
    const int index = msg.readInt16("item index") - INVENTORY_OFFSET;
    const Inventory *const inv = PlayerInfo::getInventory();
    if (inv)
    {
        std::string itemName;
        const Item *item = inv->getItem(index);
        if (item)
        {
            itemName = item->getName();
        }
        else
        {
            // TRANSLATORS: unknown item message
            itemName = _("Unknown item");
        }
        NotifyManager::notify(NotifyTypes::BOUND_ITEM, itemName);
    }
}

void InventoryRecv::processPlayerInventoryRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventoryRemove")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");
    if (inventory)
    {
        if (Item *const item = inventory->getItem(index))
        {
            if (amount)
            {
                NotifyManager::notify(NotifyTypes::DELETE_ITEM_DROPPED,
                    item->getName());
            }
            item->increaseQuantity(-amount);
            if (item->getQuantity() == 0)
                inventory->removeItemAt(index);
            ArrowsListener::distributeEvent();
        }
    }
    BLOCK_END("InventoryRecv::processPlayerInventoryRemove")
}

void InventoryRecv::processSelectCart(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    const int count = msg.readInt16("len") - 8;
    msg.readBeingId("account id");
    for (int f = 0; f < count; f ++)
        msg.readUInt8("cart type");
}

}  // namespace EAthena
