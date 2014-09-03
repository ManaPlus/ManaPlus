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

#include "net/tmwa/inventoryhandler.h"

#include "notifymanager.h"

#include "being/localplayer.h"
#include "being/pickup.h"

#include "listeners/arrowslistener.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "net/ea/eaprotocol.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern Net::InventoryHandler *inventoryHandler;
extern int serverVersion;

namespace TmwAthena
{

InventoryHandler::InventoryHandler() :
    MessageHandler(),
    Ea::InventoryHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_PLAYER_INVENTORY,
        SMSG_PLAYER_INVENTORY_ADD,
        SMSG_PLAYER_INVENTORY_REMOVE,
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
        0
    };
    handledMessages = _messages;
    inventoryHandler = this;
}

InventoryHandler::~InventoryHandler()
{
}

void InventoryHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_PLAYER_INVENTORY:
        case SMSG_PLAYER_STORAGE_ITEMS:
            processPlayerInventory(msg);
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

        default:
            break;
    }
    BLOCK_END("InventoryHandler::handleMessage")
}

void InventoryHandler::equipItem(const Item *const item) const
{
    if (!item)
        return;

    MessageOut outMsg(CMSG_PLAYER_EQUIP);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET));
    outMsg.writeInt16(0);
}

void InventoryHandler::unequipItem(const Item *const item) const
{
    if (!item)
        return;

    MessageOut outMsg(CMSG_PLAYER_UNEQUIP);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET));
}

void InventoryHandler::useItem(const Item *const item) const
{
    if (!item)
        return;

    MessageOut outMsg(CMSG_PLAYER_INVENTORY_USE);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET));
    outMsg.writeInt32(item->getId());  // unused
}

void InventoryHandler::dropItem(const Item *const item, const int amount) const
{
    if (!item)
        return;

    MessageOut outMsg(CMSG_PLAYER_INVENTORY_DROP);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET));
    outMsg.writeInt16(static_cast<int16_t>(amount));
}

void InventoryHandler::closeStorage(const int type A_UNUSED) const
{
    MessageOut outMsg(CMSG_CLOSE_STORAGE);
}

void InventoryHandler::moveItem2(const int source, const int slot,
                                 const int amount, const int destination) const
{
    if (source == Inventory::INVENTORY && destination == Inventory::STORAGE)
    {
        MessageOut outMsg(CMSG_MOVE_TO_STORAGE);
        outMsg.writeInt16(static_cast<int16_t>(slot + INVENTORY_OFFSET));
        outMsg.writeInt32(amount);
    }
    else if (source == Inventory::STORAGE
             && destination == Inventory::INVENTORY)
    {
        MessageOut outMsg(CSMG_MOVE_FROM_STORAGE);
        outMsg.writeInt16(static_cast<int16_t>(slot + STORAGE_OFFSET));
        outMsg.writeInt32(amount);
    }
}

void InventoryHandler::processPlayerEquipment(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerEquipment")
    Inventory *const inventory = localPlayer
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
        const uint8_t itemType = msg.readUInt8();  // type
        uint8_t identified = msg.readUInt8();      // identify flag

        msg.readInt16();  // equip type
        const int equipType = msg.readInt16();
        msg.readUInt8();  // attribute
        const uint8_t refine = msg.readUInt8();
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
    const int index = msg.readInt16() - INVENTORY_OFFSET;
    int amount = msg.readInt16();
    const int itemId = msg.readInt16();
    uint8_t identified = msg.readUInt8();
    msg.readUInt8();  // attribute
    const uint8_t refine = msg.readUInt8();
    for (int i = 0; i < 4; i++)
        msg.readInt16();  // cards[i]
    const int equipType = msg.readInt16();
    msg.readUInt8();  // itemType

    const ItemInfo &itemInfo = ItemDB::get(itemId);
    const unsigned char err = msg.readUInt8();
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
        if (localPlayer)
            localPlayer->pickedUp(itemInfo, 0, identified, floorId, err);
    }
    else
    {
        if (localPlayer)
        {
            localPlayer->pickedUp(itemInfo, amount,
                identified, floorId, Pickup::OKAY);
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
        ArrowsListener::distributeEvent();
    }
    BLOCK_END("InventoryHandler::processPlayerInventoryAdd")
}

void InventoryHandler::processPlayerInventory(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerInventory")
    const bool playerInvintory = msg.getId() == SMSG_PLAYER_INVENTORY;
    Inventory *const inventory = localPlayer
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
        const uint8_t itemType = msg.readUInt8();
        uint8_t identified = msg.readUInt8();
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
    BLOCK_END("InventoryHandler::processPlayerInventory")
}

void InventoryHandler::processPlayerEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerEquip")
    const int index = msg.readInt16() - INVENTORY_OFFSET;
    const int equipType = msg.readInt16();
    const uint8_t flag = msg.readUInt8();

    if (!flag)
        NotifyManager::notify(NotifyTypes::EQUIP_FAILED);
    else
        mEquips.setEquipment(getSlot(equipType), index);
    BLOCK_END("InventoryHandler::processPlayerEquip")
}

}  // namespace TmwAthena
