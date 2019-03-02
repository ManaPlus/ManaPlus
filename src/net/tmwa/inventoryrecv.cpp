/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "net/tmwa/inventoryrecv.h"

#include "notifymanager.h"

#include "being/localplayer.h"

#include "const/net/inventory.h"

#include "enums/equipslot.h"

#include "enums/resources/notifytypes.h"

#include "listeners/arrowslistener.h"

#include "net/inventoryhandler.h"
#include "net/messagein.h"

#include "net/ea/equipbackend.h"
#include "net/ea/inventoryrecv.h"

#include "debug.h"

namespace TmwAthena
{

namespace InventoryRecv
{
    const EquipSlot::Type EQUIP_POINTS[EquipSlot::VECTOREND] =
    {
        EquipSlot::LEGS_SLOT,               // Lower Headgear
        EquipSlot::FIGHT1_SLOT,             // Weapon
        EquipSlot::GLOVES_SLOT,             // Garment
        EquipSlot::RING2_SLOT,              // Accessory 1
        EquipSlot::RING1_SLOT,              // Armor
        EquipSlot::FIGHT2_SLOT,             // Shield
        EquipSlot::FEET_SLOT,               // Footgear
        EquipSlot::NECK_SLOT,               // Accessory 2
        EquipSlot::HEAD_SLOT,               // Upper Headgear
        EquipSlot::TORSO_SLOT,              // Middle Headgear
        EquipSlot::EVOL_RING1_SLOT,         // Costume Top Headgear
        EquipSlot::EVOL_RING2_SLOT,         // Costume Mid Headgear
        EquipSlot::PROJECTILE_SLOT,         // Costume Low Headgear
        EquipSlot::COSTUME_ROBE_SLOT,       // Costume Garment/Robe
        EquipSlot::MISSING1_SLOT,           // Missing slot 1
        EquipSlot::MISSING2_SLOT,           // Missing slot 2
        EquipSlot::SHADOW_ARMOR_SLOT,       // Shadow Armor
        EquipSlot::SHADOW_WEAPON_SLOT,      // Shadow Weapon
        EquipSlot::SHADOW_SHIELD_SLOT,      // Shadow Shield
        EquipSlot::SHADOW_SHOES_SLOT,       // Shadow Shoes
        EquipSlot::SHADOW_ACCESSORY2_SLOT,  // Shadow Accessory 2
        EquipSlot::SHADOW_ACCESSORY1_SLOT,  // Shadow Accessory 1
    };
}  // namespace InventoryRecv

void InventoryRecv::processPlayerEquipment(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerEquipment")
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    msg.readInt16("len");
    Equipment *const equipment = PlayerInfo::getEquipment();
    if ((equipment != nullptr) && (equipment->getBackend() == nullptr))
    {   // look like SMSG_PLAYER_INVENTORY was not received
        Ea::InventoryRecv::mEquips.clear();
        equipment->setBackend(&Ea::InventoryRecv::mEquips);
    }
    const int number = (msg.getLength() - 4) / 20;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const uint8_t identified = msg.readUInt8("identify");
        msg.readInt16("equip type?");
        const int equipType = msg.readInt16("equip type");
        msg.readUInt8("attribute");
        const uint8_t refine = msg.readUInt8("refine");
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readUInt16("card");

        if (Ea::InventoryRecv::mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d",
                        index, itemId, CAST_S32(itemType), identified);
        }

        if (inventory != nullptr)
        {
            inventory->setItem(index,
                itemId,
                itemType,
                1,
                refine,
                ItemColor_one,
                fromBool(identified, Identified),
                Damaged_false,
                Favorite_false,
                Equipm_true,
                Equipped_false);
            inventory->setCards(index, cards, maxCards);
        }

        if (equipType != 0)
        {
            Ea::InventoryRecv::mEquips.setEquipment(
                InventoryRecv::getSlot(equipType),
                index);
        }
    }
    BLOCK_END("InventoryRecv::processPlayerEquipment")
}

void InventoryRecv::processPlayerInventoryAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventoryAdd")
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    if ((PlayerInfo::getEquipment() != nullptr)
        && (PlayerInfo::getEquipment()->getBackend() == nullptr))
    {   // look like SMSG_PLAYER_INVENTORY was not received
        Ea::InventoryRecv::mEquips.clear();
        PlayerInfo::getEquipment()->setBackend(&Ea::InventoryRecv::mEquips);
    }
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    int amount = msg.readInt16("amount");
    const int itemId = msg.readInt16("item id");
    const uint8_t identified = msg.readUInt8("identified");
    msg.readUInt8("attribute");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readUInt16("card");
    const int equipType = msg.readInt16("equip type");
    const ItemTypeT type = static_cast<ItemTypeT>(msg.readUInt8("item type"));
    const unsigned char err = msg.readUInt8("status");
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

    if (err != 0U)
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
            case 3:
                pickup = Pickup::TOO_FAR;
                break;
            case 4:
                pickup = Pickup::INV_FULL;
                break;
            case 5:
                pickup = Pickup::STACK_FULL;
                break;
            case 6:
                pickup = Pickup::DROP_STEAL;
                break;
            default:
                pickup = Pickup::UNKNOWN;
                UNIMPLEMENTEDPACKETFIELD(err);
                break;
        }
        if (localPlayer != nullptr)
        {
            if (itemId == 0)
            {
                localPlayer->pickedUp(ItemDB::getEmpty(),
                    0,
                    ItemColor_one,
                    floorId,
                    pickup);
            }
            else
            {
                localPlayer->pickedUp(ItemDB::get(itemId),
                    0,
                    ItemColor_one,
                    floorId,
                    pickup);
            }
        }
    }
    else
    {
        if (localPlayer != nullptr)
        {
            if (itemId == 0)
            {
                localPlayer->pickedUp(ItemDB::getEmpty(),
                    amount,
                    ItemColor_one,
                    floorId,
                    Pickup::OKAY);
            }
            else
            {
                localPlayer->pickedUp(ItemDB::get(itemId),
                    amount,
                    ItemColor_one,
                    floorId,
                    Pickup::OKAY);
            }
        }

        if (inventory != nullptr)
        {
            const Item *const item = inventory->getItem(index);

            if ((item != nullptr) && item->getId() == itemId)
                amount += item->getQuantity();

            inventory->setItem(index,
                itemId,
                type,
                amount,
                refine,
                ItemColor_one,
                fromBool(identified, Identified),
                Damaged_false,
                Favorite_false,
                fromBool(equipType, Equipm),
                Equipped_false);
            inventory->setCards(index, cards, maxCards);
        }
        ArrowsListener::distributeEvent();
    }
    BLOCK_END("InventoryRecv::processPlayerInventoryAdd")
}

void InventoryRecv::processPlayerInventory(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventory")
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    if (PlayerInfo::getEquipment() != nullptr)
    {
        // Clear inventory - this will be a complete refresh
        Ea::InventoryRecv::mEquips.clear();
        PlayerInfo::getEquipment()->setBackend(&Ea::InventoryRecv::mEquips);
    }

    if (inventory != nullptr)
        inventory->clear();

    msg.readInt16("len");
    const int number = (msg.getLength() - 4) / 18;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[maxCards];
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const uint8_t identified = msg.readUInt8("identified");
        const int amount = msg.readInt16("amount");
        const int arrow = msg.readInt16("arrow");
        for (int i = 0; i < maxCards; i++)
            cards[i] = msg.readUInt16("card");

        if (Ea::InventoryRecv::mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d, "
                        "Qty: %d, Cards: %d, %d, %d, %d",
                        index, itemId, CAST_S32(itemType), identified, amount,
                        cards[0], cards[1], cards[2], cards[3]);
        }

        // Trick because arrows are not considered equipment
        const bool isEquipment = (arrow & 0x8000) != 0;

        if (inventory != nullptr)
        {
            inventory->setItem(index,
                itemId,
                itemType,
                amount,
                0,
                ItemColor_one,
                fromBool(identified, Identified),
                Damaged_false,
                Favorite_false,
                fromBool(isEquipment, Equipm),
                Equipped_false);
            inventory->setCards(index, cards, maxCards);
        }
    }
    BLOCK_END("InventoryRecv::processPlayerInventory")
}

void InventoryRecv::processPlayerStorage(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventory")
    Ea::InventoryRecv::mStorageItems.clear();

    msg.readInt16("len");
    const int number = (msg.getLength() - 4) / 18;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[maxCards];
        const int index = msg.readInt16("index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const uint8_t identified = msg.readUInt8("identified");
        const int amount = msg.readInt16("amount");
        msg.readInt16("arrow");
        for (int i = 0; i < maxCards; i++)
            cards[i] = msg.readUInt16("card");

        if (Ea::InventoryRecv::mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d, "
                        "Qty: %d, Cards: %d, %d, %d, %d",
                        index, itemId, CAST_S32(itemType), identified, amount,
                        cards[0], cards[1], cards[2], cards[3]);
        }

        Ea::InventoryRecv::mStorageItems.push_back(Ea::InventoryItem(
            index,
            itemId,
            itemType,
            cards,
            nullptr,
            amount,
            0,
            ItemColor_one,
            fromBool(identified, Identified),
            Damaged_false,
            Favorite_false,
            Equipm_false,
            -1));
    }
    BLOCK_END("InventoryRecv::processPlayerInventory")
}

void InventoryRecv::processPlayerEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerEquip")
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int equipType = msg.readInt16("equip type");
    const uint8_t flag = msg.readUInt8("flag");

    if (flag == 0U)
    {
        NotifyManager::notify(NotifyTypes::EQUIP_FAILED);
    }
    else
    {
        Ea::InventoryRecv::mEquips.setEquipment(
            InventoryRecv::getSlot(equipType),
            index);
    }
    BLOCK_END("InventoryRecv::processPlayerEquip")
}

void InventoryRecv::processPlayerUnEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerUnEquip")
    msg.readInt16("index");
    const int equipType = msg.readInt16("equip type");
    const uint8_t flag = msg.readUInt8("flag");

    if (flag != 0U)
    {
        Ea::InventoryRecv::mEquips.setEquipment(
            InventoryRecv::getSlot(equipType),
            -1);
    }
    if ((equipType & 0x8000) != 0)
        ArrowsListener::distributeEvent();
    BLOCK_END("InventoryRecv::processPlayerUnEquip")
}

void InventoryRecv::processPlayerStorageEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerStorageEquip")
    msg.readInt16("len");
    const int number = (msg.getLength() - 4) / 20;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[maxCards];
        const int index = msg.readInt16("index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const uint8_t identified = msg.readUInt8("identified");
        const int amount = 1;
        msg.readInt16("equip point?");
        msg.readInt16("another equip point?");
        msg.readUInt8("attribute (broken)");
        const uint8_t refine = msg.readUInt8("refine");
        for (int i = 0; i < maxCards; i++)
            cards[i] = msg.readUInt16("card");

        if (Ea::InventoryRecv::mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %u, "
                "Qty: %d, Cards: %d, %d, %d, %d, Refine: %u",
                index, itemId, CAST_S32(itemType),
                CAST_U32(identified), amount,
                cards[0], cards[1], cards[2], cards[3],
                CAST_U32(refine));
        }

        Ea::InventoryRecv::mStorageItems.push_back(Ea::InventoryItem(
            index,
            itemId,
            itemType,
            cards,
            nullptr,
            amount,
            refine,
            ItemColor_one,
            fromBool(identified, Identified),
            Damaged_false,
            Favorite_false,
            Equipm_false,
            -1));
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
    const unsigned char identified = msg.readUInt8("identified");
    msg.readUInt8("attribute");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readUInt16("card");

    if (Item *const item = Ea::InventoryRecv::mStorage->getItem(index))
    {
        item->setId(itemId, ItemColor_one);
        item->increaseQuantity(amount);
    }
    else
    {
        if (Ea::InventoryRecv::mStorage != nullptr)
        {
            Ea::InventoryRecv::mStorage->setItem(index,
                itemId,
                ItemType::Unknown,
                amount,
                refine,
                ItemColor_one,
                fromBool(identified, Identified),
                Damaged_false,
                Favorite_false,
                Equipm_false,
                Equipped_false);
            Ea::InventoryRecv::mStorage->setCards(index, cards, maxCards);
        }
    }
    BLOCK_END("InventoryRecv::processPlayerStorageAdd")
}

void InventoryRecv::processPlayerStorageRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerStorageRemove")
    // Move an item out of storage
    const int index = msg.readInt16("index") - STORAGE_OFFSET;
    const int amount = msg.readInt32("amount");
    if (Ea::InventoryRecv::mStorage != nullptr)
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

void InventoryRecv::processPlayerInventoryRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventoryRemove")
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");
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
    BLOCK_END("InventoryRecv::processPlayerInventoryRemove")
}

int InventoryRecv::getSlot(const int eAthenaSlot)
{
    if (eAthenaSlot == 0)
        return EquipSlot::VECTOREND;

    if ((eAthenaSlot & 0x8000) != 0)
        return inventoryHandler->getProjectileSlot();

    unsigned int mask = 1;
    int position = 0;
    while ((eAthenaSlot & mask) == 0U)
    {
        mask <<= 1;
        position++;
    }
    if (position >= EquipSlot::VECTOREND)
        return EquipSlot::VECTOREND;
    return CAST_S32(EQUIP_POINTS[position]);
}

void InventoryRecv::processPlayerInventoryUse(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventoryUse")
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    msg.readItemId("item id");
    msg.readInt32("id?");
    const int amount = msg.readInt16("amount");
    msg.readUInt8("type");

    if (inventory != nullptr)
    {
        if (Item *const item = inventory->getItem(index))
        {
            if (amount != 0)
                item->setQuantity(amount);
            else
                inventory->removeItemAt(index);
        }
    }
    BLOCK_END("InventoryRecv::processPlayerInventoryUse")
}

}  // namespace TmwAthena
