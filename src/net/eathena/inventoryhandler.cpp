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

#include "net/eathena/inventoryhandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "net/ea/eaprotocol.h"

#include "debug.h"

extern Net::InventoryHandler *inventoryHandler;

namespace EAthena
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
    switch (msg.getId())
    {
        case SMSG_PLAYER_INVENTORY:
        case SMSG_PLAYER_STORAGE_ITEMS:
            processPlayerInventory(msg, msg.getId() == SMSG_PLAYER_INVENTORY);
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

}  // namespace EAthena
