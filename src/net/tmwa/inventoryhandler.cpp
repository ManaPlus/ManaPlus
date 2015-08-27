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

#include "net/tmwa/inventoryhandler.h"

#include "notifymanager.h"

#include "enums/equipslot.h"

#include "being/localplayer.h"

#include "enums/resources/notifytypes.h"

#include "listeners/arrowslistener.h"

#include "net/ea/inventoryrecv.h"

#include "net/tmwa/inventoryrecv.h"
#include "net/tmwa/messageout.h"
#include "net/tmwa/protocol.h"

#include "net/ea/eaprotocol.h"
#include "net/ea/equipbackend.h"

#include "debug.h"

extern Net::InventoryHandler *inventoryHandler;

// missing EQUIP_RING1_SLOT
const EquipSlot::Type EQUIP_CONVERT[] =
{
    EquipSlot::PROJECTILE_SLOT,    // 0    0
    EquipSlot::FEET_SLOT,          // 1    SPRITE_HAIR
    EquipSlot::LEGS_SLOT,          // 2    SPRITE_WEAPON
    EquipSlot::TORSO_SLOT,         // 3    SPRITE_HEAD_BOTTOM
    EquipSlot::PROJECTILE_SLOT,    // 4    0
    EquipSlot::NECK_SLOT,          // 5    SPRITE_RING
    EquipSlot::PROJECTILE_SLOT,    // 6    0
    EquipSlot::HEAD_SLOT,          // 7    SPRITE_CLOTHES_COLOR
    EquipSlot::RING2_SLOT,         // 8    0
    EquipSlot::GLOVES_SLOT,        // 9    SPRITE_SHOES
    EquipSlot::FIGHT1_SLOT,        // 10   SPRITE_BODY
    EquipSlot::FIGHT2_SLOT,        // 11   SPRITE_FLOOR
    EquipSlot::EVOL_RING1_SLOT,    // 12   SPRITE_ROBE
    EquipSlot::EVOL_RING2_SLOT,    // 13   SPRITE_EVOL2
};

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
            InventoryRecv::processPlayerInventory(msg);
            break;

        case SMSG_PLAYER_STORAGE_ITEMS:
            InventoryRecv::processPlayerStorage(msg);
            break;

        case SMSG_PLAYER_STORAGE_EQUIP:
            InventoryRecv::processPlayerStorageEquip(msg);
            break;

        case SMSG_PLAYER_INVENTORY_ADD:
            InventoryRecv::processPlayerInventoryAdd(msg);
            break;

        case SMSG_PLAYER_INVENTORY_REMOVE:
            Ea::InventoryRecv::processPlayerInventoryRemove(msg);
            break;

        case SMSG_PLAYER_INVENTORY_USE:
            Ea::InventoryRecv::processPlayerInventoryUse(msg);
            break;

        case SMSG_ITEM_USE_RESPONSE:
            Ea::InventoryRecv::processItemUseResponse(msg);
            break;

        case SMSG_PLAYER_STORAGE_STATUS:
            Ea::InventoryRecv::processPlayerStorageStatus(msg);
            break;

        case SMSG_PLAYER_STORAGE_ADD:
            InventoryRecv::processPlayerStorageAdd(msg);
            break;

        case SMSG_PLAYER_STORAGE_REMOVE:
            InventoryRecv::processPlayerStorageRemove(msg);
            break;

        case SMSG_PLAYER_STORAGE_CLOSE:
            Ea::InventoryRecv::processPlayerStorageClose(msg);
            break;

        case SMSG_PLAYER_EQUIPMENT:
            InventoryRecv::processPlayerEquipment(msg);
            break;

        case SMSG_PLAYER_EQUIP:
            InventoryRecv::processPlayerEquip(msg);
            break;

        case SMSG_PLAYER_UNEQUIP:
            InventoryRecv::processPlayerUnEquip(msg);
            break;

        case SMSG_PLAYER_ATTACK_RANGE:
            Ea::InventoryRecv::processPlayerAttackRange(msg);
            break;

        case SMSG_PLAYER_ARROW_EQUIP:
            Ea::InventoryRecv::processPlayerArrowEquip(msg);
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
    if (source == InventoryType::INVENTORY
        && destination == InventoryType::STORAGE)
    {
        createOutPacket(CMSG_MOVE_TO_STORAGE);
        outMsg.writeInt16(static_cast<int16_t>(slot + INVENTORY_OFFSET),
            "index");
        outMsg.writeInt32(amount, "amount");
    }
    else if (source == InventoryType::STORAGE
             && destination == InventoryType::INVENTORY)
    {
        createOutPacket(CMSG_MOVE_FROM_STORAGE);
        outMsg.writeInt16(static_cast<int16_t>(slot + STORAGE_OFFSET),
            "index");
        outMsg.writeInt32(amount, "amount");
    }
}

void InventoryHandler::useCard(const Item *const item A_UNUSED)
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

void InventoryHandler::selectEgg(const Item *const item A_UNUSED) const
{
}

int InventoryHandler::convertFromServerSlot(const int serverSlot) const
{
    if (serverSlot < 0 || serverSlot > 13)
        return 0;

    return static_cast<int>(EQUIP_CONVERT[serverSlot]);
}

}  // namespace TmwAthena
