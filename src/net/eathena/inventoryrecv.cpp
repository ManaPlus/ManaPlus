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

#include "net/eathena/inventoryrecv.h"

#include "notifymanager.h"
#include "itemcolormanager.h"

#include "being/localplayer.h"

#include "const/net/inventory.h"

#include "enums/equipslot.h"

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

extern int serverVersion;
extern int packetVersion;

namespace EAthena
{

namespace InventoryRecv
{
    // EQP_* to EquipSlot
    const EquipSlot::Type EQUIP_POINTS[EquipSlot::VECTOREND] =
    {
        EquipSlot::LEGS_SLOT,               // 0  1       EQP_HEAD_LOW
        EquipSlot::FIGHT1_SLOT,             // 1  2       EQP_HAND_R
        EquipSlot::GLOVES_SLOT,             // 2  4       EQP_GARMENT
        EquipSlot::RING2_SLOT,              // 3  8       EQP_ACC_L
        EquipSlot::RING1_SLOT,              // 4  16      EQP_ARMOR
        EquipSlot::FIGHT2_SLOT,             // 5  32      EQP_HAND_L
        EquipSlot::FEET_SLOT,               // 6  64      EQP_SHOES
        EquipSlot::NECK_SLOT,               // 7  128     EQP_ACC_R
        EquipSlot::HEAD_SLOT,               // 8  256     EQP_HEAD_TOP
        EquipSlot::TORSO_SLOT,              // 9  512     EQP_HEAD_MID
        EquipSlot::EVOL_RING1_SLOT,         // 10 1024    EQP_COSTUME_HEAD_TOP
        EquipSlot::EVOL_RING2_SLOT,         // 11 2048    EQP_COSTUME_HEAD_MID
        EquipSlot::PROJECTILE_SLOT,         // 12 4096    EQP_COSTUME_HEAD_LOW
        EquipSlot::COSTUME_ROBE_SLOT,       // 13 8192    EQP_COSTUME_GARMENT
        EquipSlot::PROJECTILE_SLOT,         // 14 16384   UNUSED_COSTUME_FLOOR
        EquipSlot::PROJECTILE_SLOT,         // 15 32768   EQP_AMMO
        EquipSlot::SHADOW_ARMOR_SLOT,       // 16 65536   EQP_SHADOW_ARMOR
        EquipSlot::SHADOW_WEAPON_SLOT,      // 17 131072  EQP_SHADOW_WEAPON
        EquipSlot::SHADOW_SHIELD_SLOT,      // 18 262144  EQP_SHADOW_SHIELD
        EquipSlot::SHADOW_SHOES_SLOT,       // 19 524288  EQP_SHADOW_SHOES
        EquipSlot::SHADOW_ACCESSORY2_SLOT,  // 20 1048576 EQP_SHADOW_ACC_R
        EquipSlot::SHADOW_ACCESSORY1_SLOT,  // 21 2097152 EQP_SHADOW_ACC_L
    };

    Ea::InventoryItems mCartItems;
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

    int packetLen = 2 + 2 + 1 + 1 + 8;
    if (msg.getVersion() >= 20120925)
        packetLen += 4 + 4 + 1;
    else
        packetLen += 1 + 2 + 2 + 1;
    if (msg.getVersion() >= 20071002)
        packetLen += 4;
    if (msg.getVersion() >= 20080102)
        packetLen += 2;
    if (msg.getVersion() >= 20100629)
        packetLen += 2;
    if (msg.getVersion() >= 20150226)
        packetLen += 26;

    const int number = (msg.getLength() - 4) / packetLen;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        int equipType;
        if (msg.getVersion() >= 20120925)
        {
            msg.readInt32("location");
            equipType = msg.readInt32("wear state");
        }
        else
        {
            msg.readUInt8("identified");
            msg.readInt16("location");
            equipType = msg.readInt16("wear state");
            msg.readUInt8("is damaged");
        }
        const uint8_t refine = CAST_U8(msg.readInt8("refine"));
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readUInt16("card");
        if (msg.getVersion() >= 20071002)
            msg.readInt32("hire expire date (?)");
        if (msg.getVersion() >= 20080102)
            msg.readInt16("equip type");
        if (msg.getVersion() >= 20100629)
            msg.readInt16("item sprite number");
        ItemOptionsList *options = nullptr;
        if (msg.getVersion() >= 20150226)
        {
            options = new ItemOptionsList(msg.readUInt8("option count"));
            for (int f = 0; f < 5; f ++)
            {
                const uint16_t idx = msg.readInt16("option index");
                const uint16_t val = msg.readInt16("option value");
                msg.readUInt8("option param");
                options->add(idx, val);
            }
        }
        ItemFlags flags;
        if (msg.getVersion() >= 20120925)
            flags.byte = msg.readUInt8("flags");
        else
            flags.byte = 0;
        if (inventory != nullptr)
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
            inventory->setOptions(index, options);
        }
        delete options;

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
    int amount = msg.readInt16("count");
    const int itemId = msg.readInt16("item id");
    const uint8_t identified = msg.readUInt8("identified");
    const uint8_t damaged = msg.readUInt8("is damaged");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readUInt16("card");
    int equipType;
    if (msg.getVersion() >= 20120925)
        equipType = msg.readInt32("location");
    else
        equipType = msg.readInt16("location");
    const ItemTypeT itemType = static_cast<ItemTypeT>(
        msg.readUInt8("item type"));
    const unsigned char err = msg.readUInt8("result");
    if (msg.getVersion() >= 20061218)
        msg.readInt32("hire expire date");
    if (msg.getVersion() >= 20071002)
        msg.readInt16("bind on equip");
    ItemOptionsList *options = nullptr;
    if (msg.getVersion() >= 20150226)
    {
        options = new ItemOptionsList;
        for (int f = 0; f < 5; f ++)
        {
            const uint16_t idx = msg.readInt16("option index");
            const uint16_t val = msg.readInt16("option value");
            msg.readUInt8("option param");
            options->add(idx, val);
        }
    }

    const ItemColor color = ItemColorManager::getColorFromCards(&cards[0]);
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

    if (err != 0u)
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
                UNIMPLEMENTEDPACKETFIELD(err);
                break;
        }
        if (localPlayer != nullptr)
        {
            if (itemId == 0)
            {
                localPlayer->pickedUp(ItemDB::getEmpty(),
                    0,
                    color,
                    floorId,
                    pickup);
            }
            else
            {
                localPlayer->pickedUp(ItemDB::get(itemId),
                    0,
                    color,
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
                    color,
                    floorId,
                    Pickup::OKAY);
            }
            else
            {
                localPlayer->pickedUp(ItemDB::get(itemId),
                    amount,
                    color,
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
            inventory->setOptions(index, options);
        }
        ArrowsListener::distributeEvent();
    }
    delete options;
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

    int packetLen = 7;
    if (msg.getVersion() >= 20120925)
        packetLen += 4 + 1;
    else
        packetLen += 1 + 2;
    if (packetVersion >= 5)
        packetLen += 8;
    if (msg.getVersion() >= 20080102)
        packetLen += 4;

    const int number = (msg.getLength() - 4) / packetLen;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("item index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        if (msg.getVersion() < 20120925)
            msg.readUInt8("identified");
        const int amount = msg.readInt16("count");
        if (msg.getVersion() >= 20120925)
            msg.readInt32("wear state / equip");
        else
            msg.readInt16("wear state / equip");
        int cards[maxCards];
        if (packetVersion >= 5)
        {
            for (int f = 0; f < maxCards; f++)
                cards[f] = msg.readUInt16("card");
        }
        else
        {
            for (int f = 0; f < maxCards; f++)
                cards[f] = 0;
        }
        if (msg.getVersion() >= 20080102)
            msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        if (msg.getVersion() >= 20120925)
            flags.byte = msg.readUInt8("flags");
        else
            flags.byte = 0;

        if (inventory != nullptr)
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

    int packetLen = 7;
    if (msg.getVersion() >= 20120925)
        packetLen += 4 + 1;
    else
        packetLen += 1 + 2;
    if (packetVersion >= 5)
        packetLen += 8;
    if (msg.getVersion() >= 20080102)
        packetLen += 4;

    int number;
    if (msg.getVersion() >= 20120925)
    {
        msg.readString(24, "storage name");
        number = (msg.getLength() - 4 - 24) / packetLen;
    }
    else
    {
        number = (msg.getLength() - 4) / packetLen;
    }

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("item index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        if (msg.getVersion() < 20120925)
            msg.readUInt8("identified");
        const int amount = msg.readInt16("count");
        if (msg.getVersion() >= 20120925)
            msg.readInt32("wear state / equip");
        else
            msg.readInt16("wear state / equip");
        int cards[maxCards];
        if (msg.getVersion() >= 5)
        {
            for (int f = 0; f < maxCards; f++)
                cards[f] = msg.readUInt16("card");
        }
        else
        {
            for (int f = 0; f < maxCards; f++)
                cards[f] = 0;
        }
        if (msg.getVersion() >= 20080102)
            msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        if (msg.getVersion() >= 20120925)
            flags.byte = msg.readUInt8("flags");
        else
            flags.byte = 0;

        Ea::InventoryRecv::mInventoryItems.push_back(Ea::InventoryItem(
            index,
            itemId,
            itemType,
            cards,
            nullptr,
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
    int equipType;
    if (msg.getVersion() >= 20120925)
        equipType = msg.readInt32("wear location");
    else
        equipType = msg.readInt16("wear location");
    if (msg.getVersion() >= 20100629)
        msg.readInt16("sprite");
    const uint8_t flag = msg.readUInt8("result");

    switch (flag)
    {
        case 0:
            Ea::InventoryRecv::mEquips.setEquipment(
                InventoryRecv::getSlot(equipType),
                index);
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
    int equipType;
    if (msg.getVersion() >= 20120925)
        equipType = msg.readInt32("wear location");
    else
        equipType = msg.readInt16("wear location");
    const uint8_t flag = msg.readUInt8("result");

    if (flag != 0u)
    {
        NotifyManager::notify(NotifyTypes::UNEQUIP_FAILED);
    }
    else
    {
        Ea::InventoryRecv::mEquips.setEquipment(
            InventoryRecv::getSlot(equipType),
            -1);
    }
    if ((equipType & 0x8000) != 0)
        ArrowsListener::distributeEvent();
    BLOCK_END("InventoryRecv::processPlayerUnEquip")
}

void InventoryRecv::processPlayerInventoryRemove2(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventoryRemove2")
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    const DeleteItemReasonT reason = static_cast<DeleteItemReasonT>(
        msg.readInt16("reason"));
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");

    if (inventory != nullptr)
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

    int packetLen = 2 + 2 + 1 + 1 + 8;
    if (msg.getVersion() >= 20120925)
        packetLen += 4 + 4 + 1;
    else
        packetLen += 1 + 2 + 2 + 1;
    if (msg.getVersion() >= 20071002)
        packetLen += 4;
    if (msg.getVersion() >= 20080102)
        packetLen += 2;
    if (msg.getVersion() >= 20100629)
        packetLen += 2;
    if (msg.getVersion() >= 20150226)
        packetLen += 26;

    int number;
    if (msg.getVersion() >= 20120925)
    {
        msg.readString(24, "storage name");
        number = (msg.getLength() - 4 - 24) / packetLen;
    }
    else
    {
        number = (msg.getLength() - 4) / packetLen;
    }

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const int amount = 1;
        if (msg.getVersion() >= 20120925)
        {
            msg.readInt32("location");
            msg.readInt32("wear state");
        }
        else
        {
            msg.readUInt8("identified");
            msg.readInt16("location");
            msg.readInt16("wear state");
            msg.readUInt8("is damaged");
        }
        const uint8_t refine = msg.readUInt8("refine level");
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readUInt16("card");
        if (msg.getVersion() >= 20071002)
            msg.readInt32("hire expire date");
        if (msg.getVersion() >= 20080102)
            msg.readInt16("bind on equip");
        if (msg.getVersion() >= 20100629)
            msg.readInt16("sprite");
        ItemOptionsList *options = nullptr;
        if (msg.getVersion() >= 20150226)
        {
            options = new ItemOptionsList(msg.readUInt8("option count"));
            for (int f = 0; f < 5; f ++)
            {
                const uint16_t idx = msg.readInt16("option index");
                const uint16_t val = msg.readInt16("option value");
                msg.readUInt8("option param");
                options->add(idx, val);
            }
        }

        ItemFlags flags;
        if (msg.getVersion() >= 20120925)
            flags.byte = msg.readUInt8("flags");
        else
            flags.byte = 0;

        Ea::InventoryRecv::mInventoryItems.push_back(Ea::InventoryItem(
            index,
            itemId,
            itemType,
            cards,
            options,
            amount,
            refine,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(flags.bits.isIdentified, Identified),
            fromBool(flags.bits.isDamaged, Damaged),
            fromBool(flags.bits.isFavorite, Favorite),
            Equipm_false));
        delete options;
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
    ItemTypeT itemType;
    if (msg.getVersion() >= 5)
        itemType = static_cast<ItemTypeT>(msg.readUInt8("type"));
    else
        itemType = ItemType::Unknown;
    const unsigned char identified = msg.readUInt8("identify");
    const Damaged damaged = fromBool(msg.readUInt8("attribute"), Damaged);
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readUInt16("card");
    ItemOptionsList *options = nullptr;
    if (msg.getVersion() >= 20150226)
    {
        options = new ItemOptionsList;
        for (int f = 0; f < 5; f ++)
        {
            const uint16_t idx = msg.readInt16("option index");
            const uint16_t val = msg.readInt16("option value");
            msg.readUInt8("option param");
            options->add(idx, val);
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
        if (Ea::InventoryRecv::mStorage != nullptr)
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
            Ea::InventoryRecv::mStorage->setOptions(index, options);
        }
    }
    delete options;
    BLOCK_END("InventoryRecv::processPlayerStorageAdd")
}

void InventoryRecv::processPlayerUseCard(Net::MessageIn &msg)
{
    const Inventory *const inv = PlayerInfo::getInventory();
    const int index = inventoryHandler->getItemIndex();
    const Item *item1 = nullptr;
    if (inv != nullptr)
        item1 = inv->getItem(index);
    SellDialog *const dialog = CREATEWIDGETR(InsertCardDialog,
        index, item1);

    const int count = (msg.readInt16("len") - 4) / 2;
    for (int f = 0; f < count; f ++)
    {
        const int itemIndex = msg.readInt16("item index") - INVENTORY_OFFSET;
        if (inv == nullptr)
            continue;
        const Item *const item = inv->getItem(itemIndex);
        if (item == nullptr)
            continue;
        dialog->addItem(item, 0);
    }
}

void InventoryRecv::processPlayerInsertCard(Net::MessageIn &msg)
{
    const int itemIndex = msg.readInt16("item index") - INVENTORY_OFFSET;
    const int cardIndex = msg.readInt16("card index") - INVENTORY_OFFSET;
    if (msg.readUInt8("flag") != 0u)
    {
        NotifyManager::notify(NotifyTypes::CARD_INSERT_FAILED);
    }
    else
    {
        NotifyManager::notify(NotifyTypes::CARD_INSERT_SUCCESS);
        Inventory *const inv = PlayerInfo::getInventory();
        if (inv == nullptr)
            return;
        Item *const card = inv->getItem(cardIndex);
        int cardId = 0;
        if (card != nullptr)
        {
            cardId = card->getId();
            card->increaseQuantity(-1);
            if (card->getQuantity() == 0)
                inv->removeItemAt(cardIndex);
        }
        Item *const item = inv->getItem(itemIndex);
        if (item != nullptr)
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
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int id = msg.readInt16("item id");
    const ItemInfo &info = ItemDB::get(id);

    NotifyManager::notify(NotifyTypes::RENTAL_TIME_EXPIRED,
        info.getName());
    if (inventory != nullptr)
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
    if (inv == nullptr)
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
    UNIMPLEMENTEDPACKET;
    // +++ need close or clear cart?
}

void InventoryRecv::processPlayerCartAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerCartAdd")
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getCartInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    int amount = msg.readInt32("count");
    const int itemId = msg.readInt16("item id");
    ItemTypeT itemType = ItemType::Unknown;
    if (msg.getVersion() >= 5)
    {
        itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
    }
    const uint8_t identified = msg.readUInt8("identified");
    const Damaged damaged = fromBool(msg.readUInt8("attribute"), Damaged);
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readUInt16("card");
    ItemOptionsList *options = nullptr;
    if (msg.getVersion() >= 20150226)
    {
        options = new ItemOptionsList;
        for (int f = 0; f < 5; f ++)
        {
            const uint16_t idx = msg.readInt16("option index");
            const uint16_t val = msg.readInt16("option value");
            msg.readUInt8("option param");
            options->add(idx, val);
        }
    }

    // check what cart was created, if not add delayed items
    if ((inventory != nullptr) && inventory->getSize() > 0)
    {
        const Item *const item = inventory->getItem(index);

        if ((item != nullptr) && item->getId() == itemId)
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
        inventory->setOptions(index, options);
    }
    else
    {
        mCartItems.push_back(Ea::InventoryItem(index,
            itemId,
            itemType,
            cards,
            options,
            amount,
            refine,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(identified, Identified),
            damaged,
            Favorite_false,
            Equipm_false));
    }
    delete options;
    BLOCK_END("InventoryRecv::processPlayerCartAdd")
}

void InventoryRecv::processPlayerCartEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerCartEquip")
    msg.readInt16("len");

    int packetLen = 2 + 2 + 1 + 1 + 8;
    if (msg.getVersion() >= 20120925)
        packetLen += 4 + 4 + 1;
    else
        packetLen += 1 + 2 + 2 + 1;
    if (msg.getVersion() >= 20071002)
        packetLen += 4;
    if (msg.getVersion() >= 20080102)
        packetLen += 2;
    if (msg.getVersion() >= 20100629)
        packetLen += 2;
    if (msg.getVersion() >= 20150226)
        packetLen += 26;

    const int number = (msg.getLength() - 4) / packetLen;
    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        const int amount = 1;
        if (msg.getVersion() >= 20120925)
        {
            msg.readInt32("location");
            msg.readInt32("wear state");
        }
        else
        {
            msg.readUInt8("identified");
            msg.readInt16("location");
            msg.readInt16("wear state");
            msg.readUInt8("is damaged");
        }
        const uint8_t refine = msg.readUInt8("refine level");
        int cards[maxCards];
        for (int f = 0; f < maxCards; f++)
            cards[f] = msg.readUInt16("card");
        if (msg.getVersion() >= 20071002)
            msg.readInt32("hire expire date");
        if (msg.getVersion() >= 20080102)
            msg.readInt16("bind on equip");
        if (msg.getVersion() >= 20100629)
            msg.readInt16("sprite");
        ItemOptionsList *options = nullptr;
        if (msg.getVersion() >= 20150226)
        {
            options = new ItemOptionsList(msg.readUInt8("option count"));
            for (int f = 0; f < 5; f ++)
            {
                const uint16_t idx = msg.readInt16("option index");
                const uint16_t val = msg.readInt16("option value");
                msg.readUInt8("option param");
                options->add(idx, val);
            }
        }
        ItemFlags flags;
        if (msg.getVersion() >= 20120925)
            flags.byte = msg.readUInt8("flags");
        else
            flags.byte = 0;

        mCartItems.push_back(Ea::InventoryItem(index,
            itemId,
            itemType,
            cards,
            options,
            amount,
            refine,
            ItemColorManager::getColorFromCards(&cards[0]),
            fromBool(flags.bits.isIdentified, Identified),
            fromBool(flags.bits.isDamaged, Damaged),
            fromBool(flags.bits.isFavorite, Favorite),
            Equipm_false));
        delete options;
    }
    BLOCK_END("InventoryRecv::processPlayerCartEquip")
}

void InventoryRecv::processPlayerCartItems(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerCartItems")
    Ea::InventoryRecv::mInventoryItems.clear();

    msg.readInt16("len");

    int packetLen = 7;
    if (msg.getVersion() >= 20120925)
        packetLen += 4 + 1;
    else
        packetLen += 1 + 2;
    if (packetVersion >= 5)
        packetLen += 8;
    if (msg.getVersion() >= 20080102)
        packetLen += 4;

    const int number = (msg.getLength() - 4) / packetLen;
    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("item index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        if (msg.getVersion() < 20120925)
            msg.readUInt8("identified");
        const int amount = msg.readInt16("count");
        if (msg.getVersion() >= 20120925)
            msg.readInt32("wear state / equip");
        int cards[maxCards];
        if (msg.getVersion() >= 5)
        {
            for (int f = 0; f < maxCards; f++)
                cards[f] = msg.readUInt16("card");
        }
        else
        {
            for (int f = 0; f < maxCards; f++)
                cards[f] = 0;
        }
        if (msg.getVersion() >= 20080102)
            msg.readInt32("hire expire date (?)");
        ItemFlags flags;
        if (msg.getVersion() >= 20120925)
            flags.byte = msg.readUInt8("flags");
        else
            flags.byte = 0;

        mCartItems.push_back(Ea::InventoryItem(index,
            itemId,
            itemType,
            cards,
            nullptr,
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
    if (inv == nullptr)
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
    UNIMPLEMENTEDPACKET;

    menu = MenuType::Identify;
    const int count = msg.readInt16("len") - 4;
    for (int f = 0; f < count; f ++)
        msg.readInt16("inv index");
}

void InventoryRecv::processPlayerIdentified(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

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
    if (inv != nullptr)
        item = inv->getItem(index);
    if (item != nullptr)
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
            UNIMPLEMENTEDPACKETFIELD(flag);
            notifyType = NotifyTypes::REFINE_UNKNOWN;
            break;
    }
    NotifyManager::notify(notifyType, itemName);
}

void InventoryRecv::processPlayerRepairList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / 13;
    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("index");
        msg.readInt16("item id");
        msg.readUInt8("refine");
        for (int d = 0; d < maxCards; d ++)
            msg.readUInt16("card");
    }
    menu = MenuType::RepairWespon;
}

void InventoryRecv::processPlayerRepairEffect(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readInt16("item index");
    msg.readUInt8("flag");
}

void InventoryRecv::processPlayerRefineList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / 13;

    for (int f = 0; f < count; f ++)
    {
        msg.readInt16("item index");
        msg.readInt16("item id");
        msg.readUInt8("refine");
        for (int d = 0; d < maxCards; d ++)
            msg.readUInt16("card");
    }
    menu = MenuType::WeaponeRefine;
}

void InventoryRecv::processPlayerStoragePassword(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readInt16("info");
}

void InventoryRecv::processPlayerStoragePasswordResult(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readInt16("result");
    msg.readInt16("error count");
}

void InventoryRecv::processPlayerCookingList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    const int count = (msg.readInt16("len") - 6) / 2;
    msg.readInt16("list type");
    for (int f = 0; f < count; f ++)
        msg.readInt16("item id");
}

void InventoryRecv::processItemDamaged(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readInt16("position");
    msg.readBeingId("account id");
}

void InventoryRecv::processFavoriteItem(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

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
    if (inv != nullptr)
    {
        std::string itemName;
        const Item *const item = inv->getItem(index);
        if (item != nullptr)
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
    Inventory *const inventory = localPlayer != nullptr
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");
    if (inventory != nullptr)
    {
        if (Item *const item = inventory->getItem(index))
        {
            if (amount != 0)
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
    UNIMPLEMENTEDPACKET;

    const int count = msg.readInt16("len") - 8;
    msg.readBeingId("account id");
    for (int f = 0; f < count; f ++)
        msg.readUInt8("cart type");
}

int InventoryRecv::getSlot(const int eAthenaSlot)
{
    if (eAthenaSlot == 0)
        return EquipSlot::VECTOREND;

    if ((eAthenaSlot & 0x8000) != 0)
        return inventoryHandler->getProjectileSlot();

    unsigned int mask = 1;
    int position = 0;
    while ((eAthenaSlot & mask) == 0u)
    {
        mask <<= 1;
        position++;
    }
    if (position >= EquipSlot::VECTOREND)
        return EquipSlot::VECTOREND;
    return CAST_S32(EQUIP_POINTS[position]);
}

void InventoryRecv::processMergeItem(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    const int count = (msg.readInt16("len") - 4) / 2;
    for (int f = 0; f < count; f ++)
        msg.readInt16("inv index");
}

void InventoryRecv::processMergeItemResponse(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;

    msg.readInt16("inv index");
    msg.readInt16("amount");
    msg.readUInt8("result");
}

}  // namespace EAthena
