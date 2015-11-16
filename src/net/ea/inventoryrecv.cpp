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

#include "net/ea/inventoryrecv.h"

#include "notifymanager.h"

#include "being/localplayer.h"

#include "enums/equipslot.h"

#include "enums/resources/notifytypes.h"

#include "gui/widgets/createwidget.h"

#include "net/inventoryhandler.h"
#include "net/messagein.h"

#include "net/ea/eaprotocol.h"
#include "net/ea/equipbackend.h"

#include "utils/delete2.h"

#include "listeners/arrowslistener.h"

#include "debug.h"

namespace Ea
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

    EquipBackend mEquips;
    InventoryItems mInventoryItems;
    Inventory *mStorage = nullptr;
    PickupQueue mSentPickups;
    bool mDebugInventory = true;
}  // namespace InventoryRecv

void InventoryRecv::processPlayerInventoryUse(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerInventoryUse")
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
    BLOCK_END("InventoryRecv::processPlayerInventoryUse")
}

void InventoryRecv::processItemUseResponse(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processItemUseResponse")
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
    BLOCK_END("InventoryRecv::processItemUseResponse")
}

void InventoryRecv::processPlayerStorageStatus(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerStorageStatus")
    /*
      * This is the closest we get to an "Open Storage" packet from the
      * server. It always comes after the two SMSG_PLAYER_STORAGE_...
      * packets that update storage contents.
      */
    msg.readInt16("used count");
    const int size = msg.readInt16("max size");

    if (!mStorage)
        mStorage = new Inventory(InventoryType::Storage, size);

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
        CREATEWIDGETV(storageWindow, InventoryWindow, mStorage);
    }
    BLOCK_END("InventoryRecv::processPlayerStorageStatus")
}

void InventoryRecv::processPlayerStorageClose(Net::MessageIn &msg A_UNUSED)
{
    BLOCK_START("InventoryRecv::processPlayerStorageClose")
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
    BLOCK_END("InventoryRecv::processPlayerStorageClose")
}

void InventoryRecv::processPlayerAttackRange(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerAttackRange")
    const int range = msg.readInt16("range");
    if (localPlayer)
        localPlayer->setAttackRange(range);
    PlayerInfo::setStatBase(Attributes::ATTACK_RANGE, range);
    PlayerInfo::setStatMod(Attributes::ATTACK_RANGE, 0);
    BLOCK_END("InventoryRecv::processPlayerAttackRange")
}

void InventoryRecv::processPlayerArrowEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryRecv::processPlayerArrowEquip")
    int index = msg.readInt16("index");
    if (index <= 1)
        return;

    index -= INVENTORY_OFFSET;
    mEquips.setEquipment(inventoryHandler->getProjectileSlot(), index);
    ArrowsListener::distributeEvent();
    BLOCK_END("InventoryRecv::processPlayerArrowEquip")
}

int InventoryRecv::getSlot(const int eAthenaSlot)
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

}  // namespace Ea
