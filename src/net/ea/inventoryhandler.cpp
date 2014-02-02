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

#include "notifications.h"
#include "notifymanager.h"

#include "being/localplayer.h"

#include "gui/windows/ministatuswindow.h"

#include "net/ea/eaprotocol.h"

#include "debug.h"

extern int serverVersion;

const Equipment::Slot EQUIP_POINTS[Equipment::EQUIP_VECTOREND] =
{
    Equipment::EQUIP_LEGS_SLOT,
    Equipment::EQUIP_FIGHT1_SLOT,
    Equipment::EQUIP_GLOVES_SLOT,
    Equipment::EQUIP_RING2_SLOT,
    Equipment::EQUIP_RING1_SLOT,
    Equipment::EQUIP_FIGHT2_SLOT,
    Equipment::EQUIP_FEET_SLOT,
    Equipment::EQUIP_NECK_SLOT,
    Equipment::EQUIP_HEAD_SLOT,
    Equipment::EQUIP_TORSO_SLOT,
    Equipment::EQUIP_EVOL_RING1_SLOT,
    Equipment::EQUIP_EVOL_RING2_SLOT,
    Equipment::EQUIP_PROJECTILE_SLOT,
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

    delete mStorage;
    mStorage = nullptr;
}

void InventoryHandler::clear()
{
    delete mStorage;
    mStorage = nullptr;
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

    return EQUIP_CONVERT[serverSlot];
}

int InventoryHandler::getSlot(const int eAthenaSlot) const
{
    if (eAthenaSlot == 0)
        return Equipment::EQUIP_VECTOREND;

    if (eAthenaSlot & 0x8000)
        return Equipment::EQUIP_PROJECTILE_SLOT;

    int mask = 1;
    int position = 0;
    while (!(eAthenaSlot & mask))
    {
        mask <<= 1;
        position++;
    }
    return EQUIP_POINTS[position];
}

void InventoryHandler::processPlayerInventory(Net::MessageIn &msg,
                                              const bool playerInvintory)
{
    Inventory *const inventory = player_node
        ? PlayerInfo::getInventory() : nullptr;
    if (playerInvintory)
    {
        if (PlayerInfo::getEquipment())
        {
            // Clear inventory - this will be a complete refresh
            mEquips.clear();
            PlayerInfo::getEquipment()->setBackend(&mEquips);
        }

        if (inventory)
            inventory->clear();
    }
    else
    {
        mInventoryItems.clear();
    }

    msg.readInt16();  // length
    const int number = (msg.getLength() - 4) / 18;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[4];
        const int index = msg.readInt16() - (playerInvintory
            ? INVENTORY_OFFSET : STORAGE_OFFSET);
        const int itemId = msg.readInt16();
        const int itemType = msg.readInt8();
        unsigned char identified = msg.readInt8();
        const int amount = msg.readInt16();
        const int arrow = msg.readInt16();
        for (int i = 0; i < 4; i++)
            cards[i] = msg.readInt16();

        if (mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d, "
                        "Qty: %d, Cards: %d, %d, %d, %d",
                        index, itemId, itemType, identified, amount,
                        cards[0], cards[1], cards[2], cards[3]);
        }

        if (serverVersion < 1 && identified > 1)
            identified = 1;

        if (playerInvintory)
        {
            // Trick because arrows are not considered equipment
            const bool isEquipment = arrow & 0x8000;

            if (inventory)
            {
                inventory->setItem(index, itemId, amount,
                                   0, identified, isEquipment);
            }
        }
        else
        {
            mInventoryItems.push_back(Ea::InventoryItem(index, itemId,
                amount, 0, identified, false));
        }
    }
}

void InventoryHandler::processPlayerStorageEquip(Net::MessageIn &msg)
{
    msg.readInt16();  // length
    const int number = (msg.getLength() - 4) / 20;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[4];
        const int index = msg.readInt16() - STORAGE_OFFSET;
        const int itemId = msg.readInt16();
        const int itemType = msg.readInt8();
        unsigned char identified = msg.readInt8();
        const int amount = 1;
        msg.readInt16();    // Equip Point?
        msg.readInt16();    // Another Equip Point?
        msg.readInt8();   // Attribute (broken)
        const int refine = msg.readInt8();
        for (int i = 0; i < 4; i++)
            cards[i] = msg.readInt16();

        if (mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d, "
                        "Qty: %d, Cards: %d, %d, %d, %d, Refine: %d",
                        index, itemId, itemType, identified, amount,
                        cards[0], cards[1], cards[2], cards[3],
                        refine);
        }

        if (serverVersion < 1 && identified > 1)
            identified = 1;

        mInventoryItems.push_back(Ea::InventoryItem(index,
            itemId, amount, refine, identified, false));
    }
}

void InventoryHandler::processPlayerInventoryAdd(Net::MessageIn &msg)
{
    Inventory *const inventory = player_node
        ? PlayerInfo::getInventory() : nullptr;

    if (PlayerInfo::getEquipment()
        && !PlayerInfo::getEquipment()->getBackend())
    {   // look like SMSG_PLAYER_INVENTORY was not received
        mEquips.clear();
        PlayerInfo::getEquipment()->setBackend(&mEquips);
    }
    const int index = msg.readInt16() - INVENTORY_OFFSET;
    int amount = msg.readInt16();
    const int itemId = msg.readInt16();
    unsigned char identified = msg.readInt8();
    msg.readInt8();  // attribute
    const int refine = msg.readInt8();
    for (int i = 0; i < 4; i++)
        msg.readInt16();  // cards[i]
    const int equipType = msg.readInt16();
    msg.readInt8();  // itemType

    const ItemInfo &itemInfo = ItemDB::get(itemId);
    const unsigned char err = msg.readInt8();
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
        if (player_node)
            player_node->pickedUp(itemInfo, 0, identified, floorId, err);
    }
    else
    {
        if (player_node)
        {
            player_node->pickedUp(itemInfo, amount,
                identified, floorId, PICKUP_OKAY);
        }

        if (inventory)
        {
            const Item *const item = inventory->getItem(index);

            if (item && item->getId() == itemId)
                amount += item->getQuantity();

            if (serverVersion < 1 && identified > 1)
                identified = 1;

            inventory->setItem(index, itemId, amount, refine,
                identified, equipType != 0);
        }
    }
}

void InventoryHandler::processPlayerInventoryRemove(Net::MessageIn &msg)
{
    Inventory *const inventory = player_node
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
            if (miniStatusWindow)
                miniStatusWindow->updateArrows();
        }
    }
}

void InventoryHandler::processPlayerInventoryUse(Net::MessageIn &msg)
{
    Inventory *const inventory = player_node
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16() - INVENTORY_OFFSET;
    msg.readInt16();  // item id
    msg.readInt32();  // id
    const int amount = msg.readInt16();
    msg.readInt8();  // type

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

void InventoryHandler::processItemUseResponse(Net::MessageIn &msg)
{
    Inventory *const inventory = player_node
        ? PlayerInfo::getInventory() : nullptr;

    const int index = msg.readInt16() - INVENTORY_OFFSET;
    const int amount = msg.readInt16();

    if (msg.readInt8() == 0)
    {
        NotifyManager::notify(NotifyManager::USE_FAILED);
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
}

void InventoryHandler::processPlayerStorageStatus(Net::MessageIn &msg)
{
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
}

void InventoryHandler::processPlayerStorageAdd(Net::MessageIn &msg)
{
    // Move an item into storage
    const int index = msg.readInt16() - STORAGE_OFFSET;
    const int amount = msg.readInt32();
    const int itemId = msg.readInt16();
    unsigned char identified = msg.readInt8();
    msg.readInt8();  // attribute
    const int refine = msg.readInt8();
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
}

void InventoryHandler::processPlayerStorageRemove(Net::MessageIn &msg)
{
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
}

void InventoryHandler::processPlayerStorageClose(Net::MessageIn &msg A_UNUSED)
{
    // Storage access has been closed
    // Storage window deletes itself
    mStorageWindow = nullptr;

    if (mStorage)
        mStorage->clear();

    delete mStorage;
    mStorage = nullptr;
}

void InventoryHandler::processPlayerEquipment(Net::MessageIn &msg)
{
    Inventory *const inventory = player_node
        ? PlayerInfo::getInventory() : nullptr;

    msg.readInt16();  // length
    Equipment *const equipment = PlayerInfo::getEquipment();
    if (equipment && !equipment->getBackend())
    {   // look like SMSG_PLAYER_INVENTORY was not received
        mEquips.clear();
        equipment->setBackend(&mEquips);
    }
    const int number = (msg.getLength() - 4) / 20;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16() - INVENTORY_OFFSET;
        const int itemId = msg.readInt16();
        const int itemType = msg.readInt8();  // type
        unsigned char identified = msg.readInt8();  // identify flag

        msg.readInt16();  // equip type
        const int equipType = msg.readInt16();
        msg.readInt8();  // attribute
        const int refine = msg.readInt8();
        msg.skip(8);  // card

        if (mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d",
                        index, itemId, itemType, identified);
        }

        if (serverVersion < 1 && identified > 1)
            identified = 1;

        if (inventory)
        {
            inventory->setItem(index, itemId, 1, refine,
                identified, true);
        }

        if (equipType)
            mEquips.setEquipment(getSlot(equipType), index);
    }
}

void InventoryHandler::processPlayerEquip(Net::MessageIn &msg)
{
    const int index = msg.readInt16() - INVENTORY_OFFSET;
    const int equipType = msg.readInt16();
    const int flag = msg.readInt8();

    if (!flag)
        NotifyManager::notify(NotifyManager::EQUIP_FAILED);
    else
        mEquips.setEquipment(getSlot(equipType), index);
}

void InventoryHandler::processPlayerUnEquip(Net::MessageIn &msg)
{
    msg.readInt16();  // inder val - INVENTORY_OFFSET;
    const int equipType = msg.readInt16();
    const int flag = msg.readInt8();

    if (flag)
        mEquips.setEquipment(getSlot(equipType), -1);
    if (miniStatusWindow && equipType & 0x8000)
        miniStatusWindow->updateArrows();
}

void InventoryHandler::processPlayerAttackRange(Net::MessageIn &msg) const
{
    const int range = msg.readInt16();
    if (player_node)
        player_node->setAttackRange(range);
    PlayerInfo::setStatBase(PlayerInfo::ATTACK_RANGE, range);
    PlayerInfo::setStatMod(PlayerInfo::ATTACK_RANGE, 0);
}

void InventoryHandler::processPlayerArrowEquip(Net::MessageIn &msg)
{
    int index = msg.readInt16();
    if (index <= 1)
        return;

    index -= INVENTORY_OFFSET;

    mEquips.setEquipment(Equipment::EQUIP_PROJECTILE_SLOT, index);

    if (miniStatusWindow)
        miniStatusWindow->updateArrows();
}

void InventoryHandler::closeStorage()
{
    if (mStorageWindow)
    {
        mStorageWindow->close();
        mStorageWindow = nullptr;
    }
}

}  // namespace Ea
