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

#include "net/ea/inventoryhandler.h"

#include "notifymanager.h"

#include "being/attributes.h"
#include "being/localplayer.h"
#include "being/pickup.h"

#include "net/messagein.h"

#include "net/ea/eaprotocol.h"

#include "utils/delete2.h"

#include "listeners/arrowslistener.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern int serverVersion;

const Equipment::Slot EQUIP_POINTS[Equipment::EQUIP_VECTOREND] =
{
    Equipment::EQUIP_LEGS_SLOT,               // Lower Headgear
    Equipment::EQUIP_FIGHT1_SLOT,             // Weapon
    Equipment::EQUIP_GLOVES_SLOT,             // Garment
    Equipment::EQUIP_RING2_SLOT,              // Accessory 1
    Equipment::EQUIP_RING1_SLOT,              // Armor
    Equipment::EQUIP_FIGHT2_SLOT,             // Shield
    Equipment::EQUIP_FEET_SLOT,               // Footgear
    Equipment::EQUIP_NECK_SLOT,               // Accessory 2
    Equipment::EQUIP_HEAD_SLOT,               // Upper Headgear
    Equipment::EQUIP_TORSO_SLOT,              // Middle Headgear
    Equipment::EQUIP_EVOL_RING1_SLOT,         // Costume Top Headgear
    Equipment::EQUIP_EVOL_RING2_SLOT,         // Costume Mid Headgear
    Equipment::EQUIP_PROJECTILE_SLOT,         // Costume Low Headgear
    Equipment::EQUIP_COSTUME_ROBE_SLOT,       // Costume Garment/Robe
    Equipment::EQUIP_SHADOW_ARMOR_SLOT,       // Shadow Armor
    Equipment::EQUIP_SHADOW_WEAPON_SLOT,      // Shadow Weapon
    Equipment::EQUIP_SHADOW_SHIELD_SLOT,      // Shadow Shield
    Equipment::EQUIP_SHADOW_SHOES_SLOT,       // Shadow Shoes
    Equipment::EQUIP_SHADOW_ACCESSORY2_SLOT,  // Shadow Accessory 2
    Equipment::EQUIP_SHADOW_ACCESSORY1_SLOT,  // Shadow Accessory 1
};

// missing EQUIP_RING1_SLOT
const Equipment::Slot EQUIP_CONVERT[] =
{
    Equipment::EQUIP_PROJECTILE_SLOT,   // 0
    Equipment::EQUIP_FEET_SLOT,         // SPRITE_SHOE
    Equipment::EQUIP_LEGS_SLOT,         // SPRITE_BOTTOMCLOTHES
    Equipment::EQUIP_TORSO_SLOT,        // SPRITE_TOPCLOTHES
    Equipment::EQUIP_PROJECTILE_SLOT,   // 0
    Equipment::EQUIP_NECK_SLOT,         // SPRITE_RING
    Equipment::EQUIP_PROJECTILE_SLOT,   // 0
    Equipment::EQUIP_HEAD_SLOT,         // SPRITE_HAT
    Equipment::EQUIP_RING2_SLOT,        // 0
    Equipment::EQUIP_GLOVES_SLOT,       // SPRITE_GLOVES
    Equipment::EQUIP_FIGHT1_SLOT,       // SPRITE_WEAPON
    Equipment::EQUIP_FIGHT2_SLOT,       // SPRITE_SHIELD
    Equipment::EQUIP_EVOL_RING1_SLOT,   // SPRITE_EVOL1
    Equipment::EQUIP_EVOL_RING2_SLOT,   // SPRITE_EVOL2
    Equipment::EQUIP_PROJECTILE_SLOT,   // 0
};

namespace Ea
{

InventoryHandler::InventoryHandler() :
    mEquips(),
    mInventoryItems(),
    mStorage(nullptr),
    mStorageWindow(nullptr),
    mDebugInventory(true),
    mSentPickups()
{
}

InventoryHandler::~InventoryHandler()
{
    if (mStorageWindow)
    {
        mStorageWindow->close();
        mStorageWindow = nullptr;
    }

    delete2(mStorage);
}

void InventoryHandler::clear()
{
    delete2(mStorage);
}

bool InventoryHandler::canSplit(const Item *const item A_UNUSED) const
{
    return false;
}

void InventoryHandler::splitItem(const Item *const item A_UNUSED,
                                 const int amount A_UNUSED) const
{
    // Not implemented for eAthena (possible?)
}

void InventoryHandler::moveItem(const int oldIndex A_UNUSED,
                                const int newIndex A_UNUSED) const
{
    // Not implemented for eAthena (possible?)
}

void InventoryHandler::openStorage(const int type A_UNUSED) const
{
    // Doesn't apply to eAthena, since opening happens through NPCs?
}

size_t InventoryHandler::getSize(const int type) const
{
    switch (type)
    {
        case Inventory::INVENTORY:
            return 100;
        case Inventory::STORAGE:
            return 0;  // Comes from server after items
        case Inventory::TRADE:
            return 12;
        // GUILD_STORAGE
        case Inventory::TYPE_END:
            return 0;  // Comes from server after items
        default:
            return 0;
    }
}
int InventoryHandler::convertFromServerSlot(const int serverSlot) const
{
    if (serverSlot < 0 || serverSlot > 13)
        return 0;

    return static_cast<int>(EQUIP_CONVERT[serverSlot]);
}

int InventoryHandler::getSlot(const int eAthenaSlot)
{
    if (eAthenaSlot == 0)
        return Equipment::EQUIP_VECTOREND;

    if (eAthenaSlot & 0x8000)
        return Equipment::EQUIP_PROJECTILE_SLOT;

    unsigned int mask = 1;
    int position = 0;
    while (!(eAthenaSlot & mask))
    {
        mask <<= 1;
        position++;
    }
    return static_cast<int>(EQUIP_POINTS[position]);
}

void InventoryHandler::processPlayerStorageEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerStorageEquip")
    msg.readInt16();  // length
    const int number = (msg.getLength() - 4) / 20;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[4];
        const int index = msg.readInt16() - STORAGE_OFFSET;
        const int itemId = msg.readInt16();
        const uint8_t itemType = msg.readUInt8();
        uint8_t identified = msg.readUInt8();
        const int amount = 1;
        msg.readInt16();    // Equip Point?
        msg.readInt16();    // Another Equip Point?
        msg.readUInt8();   // Attribute (broken)
        const uint8_t refine = msg.readUInt8();
        for (int i = 0; i < 4; i++)
            cards[i] = msg.readInt16();

        if (mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %u, "
                "Qty: %d, Cards: %d, %d, %d, %d, Refine: %u",
                index, itemId, itemType,
                static_cast<unsigned int>(identified), amount,
                cards[0], cards[1], cards[2], cards[3],
                static_cast<unsigned int>(refine));
        }

        if (serverVersion < 1 && identified > 1U)
            identified = 1U;

        mInventoryItems.push_back(Ea::InventoryItem(index,
            itemId, amount, refine, identified, false));
    }
    BLOCK_END("InventoryHandler::processPlayerStorageEquip")
}

void InventoryHandler::processPlayerInventoryRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventoryRemove")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16() - INVENTORY_OFFSET;
    const int amount = msg.readInt16();
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
    BLOCK_END("InventoryHandler::processPlayerInventoryRemove")
}

void InventoryHandler::processPlayerInventoryUse(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventoryUse")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16() - INVENTORY_OFFSET;
    msg.readInt16();  // item id
    msg.readInt32();  // id
    const int amount = msg.readInt16();
    msg.readUInt8();  // type

    if (inventory)
    {
        if (Item *const item = inventory->getItem(index))
        {
            if (amount)
                item->setQuantity(amount);
            else
                inventory->removeItemAt(index);
        }
    }
    BLOCK_END("InventoryHandler::processPlayerInventoryUse")
}

void InventoryHandler::processItemUseResponse(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processItemUseResponse")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16() - INVENTORY_OFFSET;
    const int amount = msg.readInt16();

    if (msg.readUInt8() == 0)
    {
        NotifyManager::notify(NotifyTypes::USE_FAILED);
    }
    else
    {
        if (inventory)
        {
            if (Item *const item = inventory->getItem(index))
            {
                if (amount)
                    item->setQuantity(amount);
                else
                    inventory->removeItemAt(index);
            }
        }
    }
    BLOCK_END("InventoryHandler::processItemUseResponse")
}

void InventoryHandler::processPlayerStorageStatus(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerStorageStatus")
    /*
      * This is the closest we get to an "Open Storage" packet from the
      * server. It always comes after the two SMSG_PLAYER_STORAGE_...
      * packets that update storage contents.
      */
    msg.readInt16();  // Used count
    const int size = msg.readInt16();  // Max size

    if (!mStorage)
        mStorage = new Inventory(Inventory::STORAGE, size);

    FOR_EACH (Ea::InventoryItems::const_iterator, it, mInventoryItems)
    {
        mStorage->setItem((*it).slot, (*it).id, (*it).quantity,
            (*it).refine, (*it).color, (*it).equip);
    }
    mInventoryItems.clear();

    if (!mStorageWindow)
    {
        mStorageWindow = new InventoryWindow(mStorage);
        mStorageWindow->postInit();
    }
    BLOCK_END("InventoryHandler::processPlayerStorageStatus")
}

void InventoryHandler::processPlayerStorageAdd(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerStorageAdd")
    // Move an item into storage
    const int index = msg.readInt16() - STORAGE_OFFSET;
    const int amount = msg.readInt32();
    const int itemId = msg.readInt16();
    unsigned char identified = msg.readUInt8();
    msg.readUInt8();  // attribute
    const uint8_t refine = msg.readUInt8();
    for (int i = 0; i < 4; i++)
        msg.readInt16();  // card i

    if (Item *const item = mStorage->getItem(index))
    {
        item->setId(itemId, identified);
        item->increaseQuantity(amount);
    }
    else
    {
        if (mStorage)
        {
            if (serverVersion < 1 && identified > 1)
                identified = 1;

            mStorage->setItem(index, itemId, amount,
                refine, identified, false);
        }
    }
    BLOCK_END("InventoryHandler::processPlayerStorageAdd")
}

void InventoryHandler::processPlayerStorageRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerStorageRemove")
    // Move an item out of storage
    const int index = msg.readInt16() - STORAGE_OFFSET;
    const int amount = msg.readInt16();
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

void InventoryHandler::processPlayerStorageClose(Net::MessageIn &msg A_UNUSED)
{
    BLOCK_START("InventoryHandler::processPlayerStorageClose")
    // Storage access has been closed
    // Storage window deletes itself
    if (mStorageWindow)
    {
        mStorageWindow->unsetInventory();
        mStorageWindow->close();
    }
    mStorageWindow = nullptr;

    if (mStorage)
        mStorage->clear();

    delete2(mStorage);
    BLOCK_END("InventoryHandler::processPlayerStorageClose")
}

void InventoryHandler::processPlayerAttackRange(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerAttackRange")
    const int range = msg.readInt16();
    if (localPlayer)
        localPlayer->setAttackRange(range);
    PlayerInfo::setStatBase(Attributes::ATTACK_RANGE, range);
    PlayerInfo::setStatMod(Attributes::ATTACK_RANGE, 0);
    BLOCK_END("InventoryHandler::processPlayerAttackRange")
}

void InventoryHandler::processPlayerArrowEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerArrowEquip")
    int index = msg.readInt16();
    if (index <= 1)
        return;

    index -= INVENTORY_OFFSET;
    mEquips.setEquipment(Equipment::EQUIP_PROJECTILE_SLOT, index);
    ArrowsListener::distributeEvent();
    BLOCK_END("InventoryHandler::processPlayerArrowEquip")
}

void InventoryHandler::closeStorage()
{
    BLOCK_START("InventoryHandler::closeStorage")
    if (mStorageWindow)
    {
        mStorageWindow->unsetInventory();
        mStorageWindow->close();
        mStorageWindow = nullptr;
    }
    BLOCK_END("InventoryHandler::closeStorage")
}

void InventoryHandler::forgotStorage()
{
    mStorageWindow = nullptr;
}

}  // namespace Ea
