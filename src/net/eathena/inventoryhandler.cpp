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

#include "net/eathena/inventoryhandler.h"

#include "notifymanager.h"

#include "enums/equipslot.h"

#include "being/localplayer.h"

#include "listeners/arrowslistener.h"

#include "net/eathena/itemflags.h"
#include "net/eathena/menu.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "net/ea/eaprotocol.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern Net::InventoryHandler *inventoryHandler;

// missing EQUIP_RING1_SLOT
const EquipSlot::Type EQUIP_CONVERT[] =
{
    EquipSlot::PROJECTILE_SLOT,    // 0    0
    EquipSlot::FEET_SLOT,          // 1    SPRITE_HAIR
    EquipSlot::LEGS_SLOT,          // 2    SPRITE_WEAPON
    EquipSlot::TORSO_SLOT,         // 3    SPRITE_HEAD_BOTTOM
    EquipSlot::GLOVES_SLOT,        // 4    0
    EquipSlot::EVOL_RING1_SLOT,    // 5
    EquipSlot::PROJECTILE_SLOT,    // 6    0
    EquipSlot::HEAD_SLOT,          // 7    SPRITE_CLOTHES_COLOR
    EquipSlot::RING2_SLOT,         // 8    0
    EquipSlot::PROJECTILE_SLOT,    // 9    SPRITE_SHOES
    EquipSlot::FIGHT1_SLOT,        // 10   SPRITE_BODY
    EquipSlot::FIGHT2_SLOT,        // 11   SPRITE_FLOOR
    EquipSlot::EVOL_RING2_SLOT,    // 12
    EquipSlot::PROJECTILE_SLOT,    // 13   SPRITE_EVOL2
    EquipSlot::COSTUME_ROBE_SLOT,  // 14   SPRITE_EVOL3
    EquipSlot::RING1_SLOT,         // 15   SPRITE_EVOL4
};

namespace EAthena
{

Ea::InventoryItems InventoryHandler::mCartItems;

InventoryHandler::InventoryHandler() :
    MessageHandler(),
    Ea::InventoryHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_PLAYER_INVENTORY,
        SMSG_PLAYER_INVENTORY_ADD,
        SMSG_PLAYER_INVENTORY_REMOVE,
        SMSG_PLAYER_INVENTORY_REMOVE2,
        SMSG_PLAYER_INVENTORY_USE,
        SMSG_ITEM_USE_RESPONSE,
        SMSG_PLAYER_STORAGE_ITEMS,
        SMSG_PLAYER_STORAGE_EQUIP,
        SMSG_PLAYER_STORAGE_STATUS,
        SMSG_PLAYER_STORAGE_ADD,
        SMSG_PLAYER_STORAGE_REMOVE,
        SMSG_PLAYER_STORAGE_CLOSE,
        SMSG_PLAYER_EQUIPMENT,
        SMSG_PLAYER_EQUIP,
        SMSG_PLAYER_UNEQUIP,
        SMSG_PLAYER_ARROW_EQUIP,
        SMSG_PLAYER_ATTACK_RANGE,
        SMSG_PLAYER_UNE_CARD,
        SMSG_PLAYER_INSERT_CARD,
        SMSG_PLAYER_ITEM_RENTAL_TIME,
        SMSG_PLAYER_ITEM_RENTAL_EXPIRED,
        SMSG_CART_INFO,
        SMSG_CART_REMOVE,
        SMSG_PLAYER_CART_ADD,
        SMSG_PLAYER_CART_EQUIP,
        SMSG_PLAYER_CART_ITEMS,
        SMSG_PLAYER_CART_REMOVE,
        0
    };
    handledMessages = _messages;
    inventoryHandler = this;

    mCartItems.clear();
}

InventoryHandler::~InventoryHandler()
{
}

void InventoryHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_PLAYER_INVENTORY:
            processPlayerInventory(msg);
            break;

        case SMSG_PLAYER_STORAGE_ITEMS:
            processPlayerStorage(msg);
            break;

        case SMSG_PLAYER_STORAGE_EQUIP:
            processPlayerStorageEquip(msg);
            break;

        case SMSG_PLAYER_INVENTORY_ADD:
            processPlayerInventoryAdd(msg);
            break;

        case SMSG_PLAYER_INVENTORY_REMOVE:
            processPlayerInventoryRemove(msg);
            break;

        case SMSG_PLAYER_INVENTORY_REMOVE2:
            processPlayerInventoryRemove2(msg);
            break;

        case SMSG_PLAYER_INVENTORY_USE:
            processPlayerInventoryUse(msg);
            break;

        case SMSG_ITEM_USE_RESPONSE:
            processItemUseResponse(msg);
            break;

        case SMSG_PLAYER_STORAGE_STATUS:
            processPlayerStorageStatus(msg);
            break;

        case SMSG_PLAYER_STORAGE_ADD:
            processPlayerStorageAdd(msg);
            break;

        case SMSG_PLAYER_STORAGE_REMOVE:
            processPlayerStorageRemove(msg);
            break;

        case SMSG_PLAYER_STORAGE_CLOSE:
            processPlayerStorageClose(msg);
            break;

        case SMSG_PLAYER_EQUIPMENT:
            processPlayerEquipment(msg);
            break;

        case SMSG_PLAYER_EQUIP:
            processPlayerEquip(msg);
            break;

        case SMSG_PLAYER_UNEQUIP:
            processPlayerUnEquip(msg);
            break;

        case SMSG_PLAYER_ATTACK_RANGE:
            processPlayerAttackRange(msg);
            break;

        case SMSG_PLAYER_ARROW_EQUIP:
            processPlayerArrowEquip(msg);
            break;

        case SMSG_PLAYER_UNE_CARD:
            processPlayerUseCard(msg);
            break;

        case SMSG_PLAYER_INSERT_CARD:
            processPlayerInsertCard(msg);
            break;

        case SMSG_PLAYER_ITEM_RENTAL_TIME:
            processPlayerItemRentalTime(msg);
            break;

        case SMSG_PLAYER_ITEM_RENTAL_EXPIRED:
            processPlayerItemRentalExpired(msg);
            break;

        case SMSG_CART_INFO:
            processCartInfo(msg);
            break;

        case SMSG_CART_REMOVE:
            processCartRemove(msg);
            break;

        case SMSG_PLAYER_CART_ADD:
            processPlayerCartAdd(msg);
            break;

        case SMSG_PLAYER_CART_EQUIP:
            processPlayerCartEquip(msg);
            break;

        case SMSG_PLAYER_CART_ITEMS:
            processPlayerCartItems(msg);
            break;

        case SMSG_PLAYER_CART_REMOVE:
            processPlayerCartRemove(msg);
            break;

        default:
            break;
    }
}

void InventoryHandler::equipItem(const Item *const item) const
{
    if (!item)
        return;

    createOutPacket(CMSG_PLAYER_EQUIP);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    // here we set flag for any slots,
    // probably better set to slot from item properties
    outMsg.writeInt32(0xFFFFFFFFU, "wear location");
}

void InventoryHandler::unequipItem(const Item *const item) const
{
    if (!item)
        return;

    createOutPacket(CMSG_PLAYER_UNEQUIP);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
}

void InventoryHandler::useItem(const Item *const item) const
{
    if (!item)
        return;

    createOutPacket(CMSG_PLAYER_INVENTORY_USE);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt32(item->getId(), "unused");
}

void InventoryHandler::dropItem(const Item *const item, const int amount) const
{
    if (!item)
        return;

    createOutPacket(CMSG_PLAYER_INVENTORY_DROP);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(static_cast<int16_t>(amount), "amount");
}

void InventoryHandler::closeStorage(const int type A_UNUSED) const
{
    createOutPacket(CMSG_CLOSE_STORAGE);
}

void InventoryHandler::moveItem2(const int source,
                                 const int slot,
                                 const int amount,
                                 const int destination) const
{
    int packet = 0;
    int offset = INVENTORY_OFFSET;
    if (source == InventoryType::INVENTORY)
    {
        if (destination == InventoryType::STORAGE)
            packet = CMSG_MOVE_TO_STORAGE;
        else if (destination == InventoryType::CART)
            packet = CMSG_MOVE_TO_CART;
    }
    else if (source == InventoryType::STORAGE)
    {
        offset = STORAGE_OFFSET;
        if (destination == InventoryType::INVENTORY)
            packet = CSMG_MOVE_FROM_STORAGE;
        else if (destination == InventoryType::CART)
            packet = CMSG_MOVE_FROM_STORAGE_TO_CART;
    }
    else if (source == InventoryType::CART)
    {
        if (destination == InventoryType::INVENTORY)
            packet = CMSG_MOVE_FROM_CART;
        else if (destination == InventoryType::STORAGE)
            packet = CMSG_MOVE_FROM_CART_TO_STORAGE;
    }

    if (packet)
    {
        createOutPacket(packet);
        outMsg.writeInt16(static_cast<int16_t>(slot + offset), "index");
        outMsg.writeInt32(amount, "amount");
    }
}

void InventoryHandler::useCard(const int index) const
{
    createOutPacket(CMSG_PLAYER_USE_CARD);
    outMsg.writeInt16(static_cast<int16_t>(index + INVENTORY_OFFSET), "index");
}

void InventoryHandler::insertCard(const int cardIndex,
                                  const int itemIndex) const
{
    createOutPacket(CMSG_PLAYER_INSERT_CARD);
    outMsg.writeInt16(static_cast<int16_t>(cardIndex + INVENTORY_OFFSET),
        "card index");
    outMsg.writeInt16(static_cast<int16_t>(itemIndex + INVENTORY_OFFSET),
        "item index");
}

void InventoryHandler::favoriteItem(const Item *const item,
                                    const bool favorite) const
{
    createOutPacket(CMSG_PLAYER_FAVORITE_ITEM);
    outMsg.writeInt16(static_cast<int16_t>(item->getInvIndex()
        + INVENTORY_OFFSET),
        "item index");
    outMsg.writeInt8(favorite, "favorite flag");
}

void InventoryHandler::processPlayerEquipment(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerEquipment")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    msg.readInt16("len");
    Equipment *const equipment = PlayerInfo::getEquipment();
    if (equipment && !equipment->getBackend())
    {   // look like SMSG_PLAYER_INVENTORY was not received
        mEquips.clear();
        equipment->setBackend(&mEquips);
    }
    const int number = (msg.getLength() - 4) / 31;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        msg.readInt32("location");
        const int equipType = msg.readInt32("wear state");
        const uint8_t refine = static_cast<uint8_t>(msg.readInt8("refine"));
        int cards[4];
        for (int f = 0; f < 4; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        msg.readInt16("equip type");
        msg.readInt16("item sprite number");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");
        if (inventory)
        {
            inventory->setItem(index, itemId, itemType, 1, refine,
                1, flags.bits.isIdentified, flags.bits.isDamaged,
                flags.bits.isFavorite,
                true, false);
            inventory->setCards(index, cards, 4);
        }

        if (equipType)
            mEquips.setEquipment(getSlot(equipType), index);
    }
    BLOCK_END("InventoryHandler::processPlayerEquipment")
}

void InventoryHandler::processPlayerInventoryAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventoryAdd")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    if (PlayerInfo::getEquipment()
        && !PlayerInfo::getEquipment()->getBackend())
    {   // look like SMSG_PLAYER_INVENTORY was not received
        mEquips.clear();
        PlayerInfo::getEquipment()->setBackend(&mEquips);
    }
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    int amount = msg.readInt16("count");
    const int itemId = msg.readInt16("item id");
    uint8_t identified = msg.readUInt8("identified");
    const uint8_t damaged = msg.readUInt8("is damaged");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[4];
    for (int f = 0; f < 4; f++)
        cards[f] = msg.readInt16("card");
    const int equipType = msg.readInt32("location");
    const int itemType = msg.readUInt8("item type");
    const unsigned char err = msg.readUInt8("result");
    msg.readInt32("hire expire date");
    msg.readInt16("bind on equip");

    const ItemInfo &itemInfo = ItemDB::get(itemId);
    int floorId;
    if (mSentPickups.empty())
    {
        floorId = 0;
    }
    else
    {
        floorId = mSentPickups.front();
        mSentPickups.pop();
    }

    if (err)
    {
        Pickup::Type pickup;
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
                logger->log("unknown pickup type: %d", err);
                break;
        }
        if (localPlayer)
            localPlayer->pickedUp(itemInfo, 0, 1, floorId, pickup);
    }
    else
    {
        if (localPlayer)
        {
            localPlayer->pickedUp(itemInfo, amount,
                1, floorId, Pickup::OKAY);
        }

        if (inventory)
        {
            const Item *const item = inventory->getItem(index);

            if (item && item->getId() == itemId)
                amount += item->getQuantity();

            inventory->setItem(index, itemId, itemType, amount, refine,
                1, identified != 0, damaged != 0, false,
                equipType != 0, false);
            inventory->setCards(index, cards, 4);
        }
        ArrowsListener::distributeEvent();
    }
    BLOCK_END("InventoryHandler::processPlayerInventoryAdd")
}

void InventoryHandler::processPlayerInventory(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventory")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;
    if (PlayerInfo::getEquipment())
    {
        // Clear inventory - this will be a complete refresh
        mEquips.clear();
        PlayerInfo::getEquipment()->setBackend(&mEquips);
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
        int cards[4];
        for (int f = 0; f < 4; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        if (inventory)
        {
            inventory->setItem(index, itemId, itemType, amount,
                0, 1, flags.bits.isIdentified,
                flags.bits.isDamaged, flags.bits.isFavorite,
                false, false);
            inventory->setCards(index, cards, 4);
        }
    }
    BLOCK_END("InventoryHandler::processPlayerInventory")
}

void InventoryHandler::processPlayerStorage(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventory")
    mInventoryItems.clear();

    msg.readInt16("len");
    msg.readString(24, "storage name");

    const int number = (msg.getLength() - 4 - 24) / 23;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("item index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        const int amount = msg.readInt16("count");
        msg.readInt32("wear state / equip");
        int cards[4];
        for (int f = 0; f < 4; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        mInventoryItems.push_back(Ea::InventoryItem(index, itemId, itemType,
            cards, amount, 0, 1, flags.bits.isIdentified,
            flags.bits.isDamaged, flags.bits.isFavorite, false));
    }
    BLOCK_END("InventoryHandler::processPlayerInventory")
}

void InventoryHandler::processPlayerEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerEquip")
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int equipType = msg.readInt32("wear location");
    msg.readInt16("sprite");
    const uint8_t flag = msg.readUInt8("result");

    switch (flag)
    {
        case 0:
            mEquips.setEquipment(getSlot(equipType), index);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::EQUIP_FAILED_LEVEL);
            break;

        case 2:
        default:
            NotifyManager::notify(NotifyTypes::EQUIP_FAILED);
            break;
    }
    BLOCK_END("InventoryHandler::processPlayerEquip")
}

void InventoryHandler::processPlayerUnEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerUnEquip")
    msg.readInt16("index");
    const int equipType = msg.readInt32("wear location");
    const uint8_t flag = msg.readUInt8("result");

    // +++ need use UNEQUIP_FAILED event
    if (flag)
        NotifyManager::notify(NotifyTypes::EQUIP_FAILED);
    else
        mEquips.setEquipment(getSlot(equipType), -1);
    if (equipType & 0x8000)
        ArrowsListener::distributeEvent();
    BLOCK_END("InventoryHandler::processPlayerUnEquip")
}

void InventoryHandler::processPlayerInventoryRemove2(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventoryRemove2")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    // +++ here possible use particle or text/sound effects
    // for different reasons
    msg.readInt16("reason");
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");
    if (inventory)
    {
        if (Item *const item = inventory->getItem(index))
        {
            item->increaseQuantity(-amount);
            if (item->getQuantity() == 0)
                inventory->removeItemAt(index);
            ArrowsListener::distributeEvent();
        }
    }
    BLOCK_END("InventoryHandler::processPlayerInventoryRemove2")
}

void InventoryHandler::processPlayerStorageEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerStorageEquip")
    msg.readInt16("len");
    const int number = (msg.getLength() - 4 - 24) / 31;

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
        int cards[4];
        for (int f = 0; f < 4; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date");
        msg.readInt16("bind on equip");
        msg.readInt16("sprite");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        mInventoryItems.push_back(Ea::InventoryItem(index, itemId, itemType,
            cards, amount, refine, 1, flags.bits.isIdentified,
            flags.bits.isDamaged, flags.bits.isFavorite, false));
    }
    BLOCK_END("InventoryHandler::processPlayerStorageEquip")
}

void InventoryHandler::processPlayerStorageAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerStorageAdd")
    // Move an item into storage
    const int index = msg.readInt16("index") - STORAGE_OFFSET;
    const int amount = msg.readInt32("amount");
    const int itemId = msg.readInt16("item id");
    const int itemType = msg.readUInt8("type");
    const unsigned char identified = msg.readUInt8("identify");
    msg.readUInt8("attribute");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[4];
    for (int f = 0; f < 4; f++)
        cards[f] = msg.readInt16("card");

    if (Item *const item = mStorage->getItem(index))
    {
        item->setId(itemId, 1);
        item->increaseQuantity(amount);
    }
    else
    {
        if (mStorage)
        {
            mStorage->setItem(index, itemId, itemType, amount,
                refine, 1, identified != 0, false, false, false, false);
            mStorage->setCards(index, cards, 4);
        }
    }
    BLOCK_END("InventoryHandler::processPlayerStorageAdd")
}

void InventoryHandler::processPlayerUseCard(Net::MessageIn &msg)
{
    // +++ here need show dialog with item selection for card.
    const int count = (msg.readInt16("len") - 4) / 2;
    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("item id");
    }
}

void InventoryHandler::processPlayerInsertCard(Net::MessageIn &msg)
{
    msg.readInt16("item index");
    msg.readInt16("card index");
    if (msg.readUInt8("flag"))
        NotifyManager::notify(NotifyTypes::CARD_INSERT_FAILED);
    else
        NotifyManager::notify(NotifyTypes::CARD_INSERT_SUCCESS);
}

void InventoryHandler::selectEgg(const Item *const item) const
{
    createOutPacket(CMSG_PET_SELECT_EGG);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    menu = MenuType::Unknown;
}

void InventoryHandler::processPlayerItemRentalTime(Net::MessageIn &msg)
{
    // +++ need update item rental time
    msg.readInt16("item id");
    msg.readInt32("seconds");
}

void InventoryHandler::processPlayerItemRentalExpired(Net::MessageIn &msg)
{
    // ++ need remove item from inventory
    msg.readInt16("index");
    msg.readInt16("item id");
}

int InventoryHandler::convertFromServerSlot(const int serverSlot) const
{
    if (serverSlot < 0 || serverSlot > 15)
        return 0;

    return static_cast<int>(EQUIP_CONVERT[serverSlot]);
}

void InventoryHandler::processPlayerStorageRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerStorageRemove")
    // Move an item out of storage
    const int index = msg.readInt16("index") - STORAGE_OFFSET;
    const int amount = msg.readInt32("amount");
    if (mStorage)
    {
        if (Item *const item = mStorage->getItem(index))
        {
            item->increaseQuantity(-amount);
            if (item->getQuantity() == 0)
                mStorage->removeItemAt(index);
        }
    }
    BLOCK_END("InventoryHandler::processPlayerStorageRemove")
}

void InventoryHandler::processCartInfo(Net::MessageIn &msg)
{
    msg.readInt16("cart items used");
    const int size = msg.readInt16("max cart items");
    msg.readInt32("cart weight");
    msg.readInt32("max cart weight");
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
            false);
    }
    mCartItems.clear();
}

void InventoryHandler::processCartRemove(Net::MessageIn &msg A_UNUSED)
{
    //+++ need close or clear cart?
}

void InventoryHandler::processPlayerCartAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerCartAdd")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getCartInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    int amount = msg.readInt32("count");
    const int itemId = msg.readInt16("item id");
    const int itemType = msg.readUInt8("item type");
    uint8_t identified = msg.readUInt8("identified");
    msg.readUInt8("attribute");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[4];
    for (int f = 0; f < 4; f++)
        cards[f] = msg.readInt16("card");

    const ItemInfo &itemInfo = ItemDB::get(itemId);
    int floorId;
    if (mSentPickups.empty())
    {
        floorId = 0;
    }
    else
    {
        floorId = mSentPickups.front();
        mSentPickups.pop();
    }

    if (inventory)
    {
        const Item *const item = inventory->getItem(index);

        if (item && item->getId() == itemId)
            amount += item->getQuantity();

        inventory->setItem(index, itemId, itemType, amount, refine,
            1, identified != 0, false, false, false, false);
        inventory->setCards(index, cards, 4);
    }
    BLOCK_END("InventoryHandler::processPlayerCartAdd")
}

void InventoryHandler::processPlayerCartEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerCartEquip")
    msg.readInt16("len");
    const int number = (msg.getLength() - 4) / 31;
    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        const int amount = 1;
        msg.readInt32("location");
        msg.readInt32("wear state");
        const uint8_t refine = msg.readUInt8("refine level");
        int cards[4];
        for (int f = 0; f < 4; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date");
        msg.readInt16("bind on equip");
        msg.readInt16("sprite");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        mCartItems.push_back(Ea::InventoryItem(index, itemId, itemType,
            cards, amount, refine, 1, flags.bits.isIdentified,
            flags.bits.isDamaged, flags.bits.isFavorite, false));
    }
    BLOCK_END("InventoryHandler::processPlayerCartEquip")
}

void InventoryHandler::processPlayerCartItems(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerCartItems")
    mInventoryItems.clear();

    msg.readInt16("len");
    const int number = (msg.getLength() - 4) / 23;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("item index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const int itemType = msg.readUInt8("item type");
        const int amount = msg.readInt16("count");
        msg.readInt32("wear state / equip");
        int cards[4];
        for (int f = 0; f < 4; f++)
            cards[f] = msg.readInt16("card");
        msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        flags.byte = msg.readUInt8("flags");

        mCartItems.push_back(Ea::InventoryItem(index, itemId, itemType,
            cards, amount, 0, 1, flags.bits.isIdentified,
            flags.bits.isDamaged, flags.bits.isFavorite, false));
    }
    BLOCK_END("InventoryHandler::processPlayerCartItems")
}

void InventoryHandler::processPlayerCartRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerCartRemove")
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
    BLOCK_END("InventoryHandler::processPlayerCartRemove")
}

}  // namespace EAthena
