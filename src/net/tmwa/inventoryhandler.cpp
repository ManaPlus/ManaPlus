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

#include "listeners/arrowslistener.h"

#include "net/serverfeatures.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "net/ea/eaprotocol.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern Net::InventoryHandler *inventoryHandler;

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

    createOutPacket(CMSG_PLAYER_EQUIP);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(0, "unused");
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
    outMsg.writeInt32(item->getId(), "item id");
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

void InventoryHandler::moveItem2(const int source, const int slot,
                                 const int amount, const int destination) const
{
    if (source == Inventory::INVENTORY && destination == Inventory::STORAGE)
    {
        createOutPacket(CMSG_MOVE_TO_STORAGE);
        outMsg.writeInt16(static_cast<int16_t>(slot + INVENTORY_OFFSET),
            "index");
        outMsg.writeInt32(amount, "amount");
    }
    else if (source == Inventory::STORAGE
             && destination == Inventory::INVENTORY)
    {
        createOutPacket(CSMG_MOVE_FROM_STORAGE);
        outMsg.writeInt16(static_cast<int16_t>(slot + STORAGE_OFFSET),
            "index");
        outMsg.writeInt32(amount, "amount");
    }
}

void InventoryHandler::useCard(const int index A_UNUSED) const
{
}

void InventoryHandler::insertCard(const int cardIndex A_UNUSED,
                                  const int itemIndex A_UNUSED) const
{
}

void InventoryHandler::favoriteItem(const Item *const item A_UNUSED,
                                    const bool favorite A_UNUSED) const
{
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
    const int number = (msg.getLength() - 4) / 20;

    for (int loop = 0; loop < number; loop++)
    {
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const uint8_t itemType = msg.readUInt8("item type");
        const uint8_t identified = msg.readUInt8("identify");

        msg.readInt16("equip type?");
        const int equipType = msg.readInt16("equip type");
        msg.readUInt8("attribute");
        const uint8_t refine = msg.readUInt8("refine");
        int cards[4];
        for (int f = 0; f < 4; f++)
            cards[f] = msg.readInt16("card");

        if (mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d",
                        index, itemId, itemType, identified);
        }

        if (inventory)
        {
            if (serverFeatures->haveItemColors())
            {
                inventory->setItem(index, itemId, 1, refine,
                    identified, true, false, false, true, false);
            }
            else
            {
                inventory->setItem(index, itemId, 1, refine,
                    1, identified != 0, false, false, true, false);
            }
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
    int amount = msg.readInt16("amount");
    const int itemId = msg.readInt16("item id");
    const uint8_t identified = msg.readUInt8("identified");
    msg.readUInt8("attribute");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[4];
    for (int f = 0; f < 4; f++)
        cards[f] = msg.readInt16("card");
    const int equipType = msg.readInt16("equip type");
    msg.readUInt8("item type");

    const ItemInfo &itemInfo = ItemDB::get(itemId);
    const unsigned char err = msg.readUInt8("status");
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
                logger->log("unknown pickup type: %d", err);
                break;
        }
        if (localPlayer)
            localPlayer->pickedUp(itemInfo, 0, identified, floorId, pickup);
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

            if (serverFeatures->haveItemColors())
            {
                inventory->setItem(index, itemId, amount, refine,
                    identified, true, false, false, equipType != 0, false);
            }
            else
            {
                inventory->setItem(index, itemId, amount, refine,
                    1, identified != 0, false, false, equipType != 0, false);
            }
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
    const int number = (msg.getLength() - 4) / 18;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[4];
        const int index = msg.readInt16("index") - INVENTORY_OFFSET;
        const int itemId = msg.readInt16("item id");
        const uint8_t itemType = msg.readUInt8("item type");
        const uint8_t identified = msg.readUInt8("identified");
        const int amount = msg.readInt16("amount");
        const int arrow = msg.readInt16("arrow");
        for (int i = 0; i < 4; i++)
            cards[i] = msg.readInt16("card");

        if (mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d, "
                        "Qty: %d, Cards: %d, %d, %d, %d",
                        index, itemId, itemType, identified, amount,
                        cards[0], cards[1], cards[2], cards[3]);
        }

        // Trick because arrows are not considered equipment
        const bool isEquipment = arrow & 0x8000;

        if (inventory)
        {
            if (serverFeatures->haveItemColors())
            {
                inventory->setItem(index, itemId, amount,
                    0, identified, true, false, false, isEquipment, false);
            }
            else
            {
                inventory->setItem(index, itemId, amount,
                    0, 1, identified != 0, false, false, isEquipment, false);
            }
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
    const int number = (msg.getLength() - 4) / 18;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[4];
        const int index = msg.readInt16("index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const uint8_t itemType = msg.readUInt8("item type");
        const uint8_t identified = msg.readUInt8("identified");
        const int amount = msg.readInt16("amount");
        msg.readInt16("arrow");
        for (int i = 0; i < 4; i++)
            cards[i] = msg.readInt16("card");

        if (mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %d, "
                        "Qty: %d, Cards: %d, %d, %d, %d",
                        index, itemId, itemType, identified, amount,
                        cards[0], cards[1], cards[2], cards[3]);
        }

        if (serverFeatures->haveItemColors())
        {
            mInventoryItems.push_back(Ea::InventoryItem(index, itemId,
                cards, amount, 0, identified, true, false, false, false));
        }
        else
        {
            mInventoryItems.push_back(Ea::InventoryItem(index, itemId,
                cards, amount, 0, 1, identified != 0, false, false, false));
        }
    }
    BLOCK_END("InventoryHandler::processPlayerInventory")
}

void InventoryHandler::processPlayerEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerEquip")
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int equipType = msg.readInt16("equip type");
    const uint8_t flag = msg.readUInt8("flag");

    if (!flag)
        NotifyManager::notify(NotifyTypes::EQUIP_FAILED);
    else
        mEquips.setEquipment(getSlot(equipType), index);
    BLOCK_END("InventoryHandler::processPlayerEquip")
}

void InventoryHandler::processPlayerUnEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerUnEquip")
    msg.readInt16("index");
    const int equipType = msg.readInt16("equip type");
    const uint8_t flag = msg.readUInt8("flag");

    if (flag)
        mEquips.setEquipment(getSlot(equipType), -1);
    if (equipType & 0x8000)
        ArrowsListener::distributeEvent();
    BLOCK_END("InventoryHandler::processPlayerUnEquip")
}

void InventoryHandler::processPlayerStorageEquip(Net::MessageIn &msg)
{
    BLOCK_START("InventoryHandler::processPlayerStorageEquip")
    msg.readInt16("len");
    const int number = (msg.getLength() - 4) / 20;

    for (int loop = 0; loop < number; loop++)
    {
        int cards[4];
        const int index = msg.readInt16("index") - STORAGE_OFFSET;
        const int itemId = msg.readInt16("item id");
        const uint8_t itemType = msg.readUInt8("item type");
        const uint8_t identified = msg.readUInt8("identified");
        const int amount = 1;
        msg.readInt16("equip point?");
        msg.readInt16("another equip point?");
        msg.readUInt8("attribute (broken)");
        const uint8_t refine = msg.readUInt8("refine");
        for (int i = 0; i < 4; i++)
            cards[i] = msg.readInt16("card");

        if (mDebugInventory)
        {
            logger->log("Index: %d, ID: %d, Type: %d, Identified: %u, "
                "Qty: %d, Cards: %d, %d, %d, %d, Refine: %u",
                index, itemId, itemType,
                static_cast<unsigned int>(identified), amount,
                cards[0], cards[1], cards[2], cards[3],
                static_cast<unsigned int>(refine));
        }

        if (serverFeatures->haveItemColors())
        {
            mInventoryItems.push_back(Ea::InventoryItem(index, itemId,
                cards, amount, refine, identified, true,
                false, false, false));
        }
        else
        {
            mInventoryItems.push_back(Ea::InventoryItem(index, itemId,
                cards, amount, refine, 1, identified != 0,
                false, false, false));
        }
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
    const unsigned char identified = msg.readUInt8("identified");
    msg.readUInt8("attribute");
    const uint8_t refine = msg.readUInt8("refine");
    int cards[4];
    for (int f = 0; f < 4; f++)
        cards[f] = msg.readInt16("card");

    if (Item *const item = mStorage->getItem(index))
    {
        item->setId(itemId, identified);
        item->increaseQuantity(amount);
    }
    else
    {
        if (mStorage)
        {
            if (serverFeatures->haveItemColors())
            {
                mStorage->setItem(index, itemId, amount,
                    refine, identified, true, false, false, false, false);
            }
            else
            {
                mStorage->setItem(index, itemId, amount,
                    refine, 1, identified != 0, false, false, false, false);
            }
            mStorage->setCards(index, cards, 4);
        }
    }
    BLOCK_END("InventoryHandler::processPlayerStorageAdd")
}

void InventoryHandler::selectEgg(const Item *const item A_UNUSED) const
{
}

}  // namespace TmwAthena
