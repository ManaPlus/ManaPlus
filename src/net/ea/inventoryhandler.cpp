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

#include "net/ea/inventoryhandler.h"

#include "notifymanager.h"

#include "being/localplayer.h"

#include "enums/equipslot.h"

#include "enums/being/attributes.h"

#include "net/messagein.h"

#include "net/ea/eaprotocol.h"

#include "utils/delete2.h"

#include "listeners/arrowslistener.h"

#include "resources/notifytypes.h"

#include "debug.h"

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

namespace Ea
{

EquipBackend InventoryHandler::mEquips;
InventoryItems InventoryHandler::mInventoryItems;
Inventory *InventoryHandler::mStorage = nullptr;
PickupQueue InventoryHandler::mSentPickups;
bool InventoryHandler::mDebugInventory = true;

InventoryHandler::InventoryHandler()
{
    mEquips.clear();
    mInventoryItems.clear();
    mStorage = nullptr;
    storageWindow = nullptr;
    while (!mSentPickups.empty())
        mSentPickups.pop();
    mDebugInventory = true;
}

InventoryHandler::~InventoryHandler()
{
    if (storageWindow)
    {
        storageWindow->close();
        storageWindow = nullptr;
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
        case InventoryType::INVENTORY:
            return 100;
        case InventoryType::STORAGE:
            return 0;  // Comes from server after items
        case InventoryType::TRADE:
            return 12;
        // GUILD_STORAGE
        case InventoryType::TYPE_END:
            return 0;  // Comes from server after items
        default:
            return 0;
    }
}
int InventoryHandler::getSlot(const int eAthenaSlot)
{
    if (eAthenaSlot == 0)
        return EquipSlot::VECTOREND;

    if (eAthenaSlot & 0x8000)
        return inventoryHandler->getProjectileSlot();

    unsigned int mask = 1;
    int position = 0;
    while (!(eAthenaSlot & mask))
    {
        mask <<= 1;
        position++;
    }
    return static_cast<int>(EQUIP_POINTS[position]);
}

void InventoryHandler::processPlayerInventoryRemove(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventoryRemove")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

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
    BLOCK_END("InventoryHandler::processPlayerInventoryRemove")
}

void InventoryHandler::processPlayerInventoryUse(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventoryUse")
    Inventory *const inventory = localPlayer
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    msg.readInt16("item id");
    msg.readInt32("id?");
    const int amount = msg.readInt16("amount");
    msg.readUInt8("type");

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

    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");

    if (msg.readUInt8("result") == 0)
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
    msg.readInt16("used count");
    const int size = msg.readInt16("max size");

    if (!mStorage)
        mStorage = new Inventory(InventoryType::STORAGE, size);

    FOR_EACH (Ea::InventoryItems::const_iterator, it, mInventoryItems)
    {
        mStorage->setItem((*it).slot,
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
    mInventoryItems.clear();

    if (!storageWindow)
    {
        storageWindow = new InventoryWindow(mStorage);
        storageWindow->postInit();
    }
    BLOCK_END("InventoryHandler::processPlayerStorageStatus")
}

void InventoryHandler::processPlayerStorageClose(Net::MessageIn &msg A_UNUSED)
{
    BLOCK_START("InventoryHandler::processPlayerStorageClose")
    // Storage access has been closed
    // Storage window deletes itself
    if (storageWindow)
    {
        storageWindow->unsetInventory();
        storageWindow->close();
    }
    storageWindow = nullptr;

    if (mStorage)
        mStorage->clear();

    delete2(mStorage);
    BLOCK_END("InventoryHandler::processPlayerStorageClose")
}

void InventoryHandler::processPlayerAttackRange(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerAttackRange")
    const int range = msg.readInt16("range");
    if (localPlayer)
        localPlayer->setAttackRange(range);
    PlayerInfo::setStatBase(Attributes::ATTACK_RANGE, range);
    PlayerInfo::setStatMod(Attributes::ATTACK_RANGE, 0);
    BLOCK_END("InventoryHandler::processPlayerAttackRange")
}

void InventoryHandler::processPlayerArrowEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerArrowEquip")
    int index = msg.readInt16("index");
    if (index <= 1)
        return;

    index -= INVENTORY_OFFSET;
    mEquips.setEquipment(inventoryHandler->getProjectileSlot(), index);
    ArrowsListener::distributeEvent();
    BLOCK_END("InventoryHandler::processPlayerArrowEquip")
}

void InventoryHandler::destroyStorage()
{
    BLOCK_START("InventoryHandler::closeStorage")
    if (storageWindow)
    {
        storageWindow->unsetInventory();
        storageWindow->close();
        storageWindow = nullptr;
    }
    BLOCK_END("InventoryHandler::closeStorage")
}

void InventoryHandler::forgotStorage()
{
    storageWindow = nullptr;
}

}  // namespace Ea
