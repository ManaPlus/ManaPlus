/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "const/net/inventory.h"

#include "enums/equipslot.h"

#include "net/eathena/inventoryrecv.h"
#include "net/eathena/menu.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "resources/item/item.h"

#include "debug.h"

extern Net::InventoryHandler *inventoryHandler;

// missing EQUIP_RING1_SLOT
const EquipSlot::Type EQUIP_CONVERT[] =
{
    EquipSlot::PROJECTILE_SLOT,    // 0    0
    EquipSlot::FEET_SLOT,          // 1    SPRITE_HAIR
    EquipSlot::LEGS_SLOT,          // 2    SPRITE_WEAPON
    EquipSlot::TORSO_SLOT,         // 3    SPRITE_HEAD_BOTTOM
    EquipSlot::GLOVES_SLOT,        // 4    0
    EquipSlot::EVOL_RING1_SLOT,    // 5
    EquipSlot::PROJECTILE_SLOT,    // 6    0
    EquipSlot::HEAD_SLOT,          // 7    SPRITE_CLOTHES_COLOR
    EquipSlot::RING2_SLOT,         // 8    0
    EquipSlot::PROJECTILE_SLOT,    // 9    SPRITE_SHOES
    EquipSlot::FIGHT1_SLOT,        // 10   SPRITE_BODY
    EquipSlot::FIGHT2_SLOT,        // 11   SPRITE_FLOOR
    EquipSlot::EVOL_RING2_SLOT,    // 12
    EquipSlot::PROJECTILE_SLOT,    // 13   SPRITE_EVOL2
    EquipSlot::COSTUME_ROBE_SLOT,  // 14   SPRITE_EVOL3
    EquipSlot::RING1_SLOT,         // 15   SPRITE_EVOL4
};

namespace EAthena
{

InventoryHandler::InventoryHandler() :
    Ea::InventoryHandler(),
    mItemIndex(0)
{
    inventoryHandler = this;

    InventoryRecv::mCartItems.clear();
}

InventoryHandler::~InventoryHandler()
{
}

void InventoryHandler::equipItem(const Item *const item) const
{
    if (!item)
        return;

    createOutPacket(CMSG_PLAYER_EQUIP);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    // here we set flag for any slots,
    // probably better set to slot from item properties
    outMsg.writeInt32(0xFFFFFFFFU, "wear location");
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
    outMsg.writeInt32(item->getId(), "unused");
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

void InventoryHandler::closeStorage() const
{
    createOutPacket(CMSG_CLOSE_STORAGE);
}

void InventoryHandler::moveItem2(const InventoryTypeT source,
                                 const int slot,
                                 const int amount,
                                 const InventoryTypeT destination) const
{
    int packet = 0;
    int offset = INVENTORY_OFFSET;
    if (source == InventoryType::Inventory)
    {
        if (destination == InventoryType::Storage)
            packet = CMSG_MOVE_TO_STORAGE;
        else if (destination == InventoryType::Cart)
            packet = CMSG_MOVE_TO_CART;
    }
    else if (source == InventoryType::Storage)
    {
        offset = STORAGE_OFFSET;
        if (destination == InventoryType::Inventory)
            packet = CMSG_MOVE_FROM_STORAGE;
        else if (destination == InventoryType::Cart)
            packet = CMSG_MOVE_FROM_STORAGE_TO_CART;
    }
    else if (source == InventoryType::Cart)
    {
        if (destination == InventoryType::Inventory)
            packet = CMSG_MOVE_FROM_CART;
        else if (destination == InventoryType::Storage)
            packet = CMSG_MOVE_FROM_CART_TO_STORAGE;
    }

    if (packet)
    {
        createOutPacket(packet);
        outMsg.writeInt16(static_cast<int16_t>(slot + offset), "index");
        outMsg.writeInt32(amount, "amount");
    }
}

void InventoryHandler::useCard(const Item *const item)
{
    if (!item)
        return;

    mItemIndex = item->getInvIndex();
    createOutPacket(CMSG_PLAYER_USE_CARD);
    outMsg.writeInt16(static_cast<int16_t>(
        mItemIndex + INVENTORY_OFFSET), "index");
}

void InventoryHandler::insertCard(const int cardIndex,
                                  const int itemIndex) const
{
    createOutPacket(CMSG_PLAYER_INSERT_CARD);
    outMsg.writeInt16(static_cast<int16_t>(cardIndex + INVENTORY_OFFSET),
        "card index");
    outMsg.writeInt16(static_cast<int16_t>(itemIndex + INVENTORY_OFFSET),
        "item index");
}

void InventoryHandler::favoriteItem(const Item *const item,
                                    const bool favorite) const
{
    if (!item)
        return;
    createOutPacket(CMSG_PLAYER_FAVORITE_ITEM);
    outMsg.writeInt16(static_cast<int16_t>(item->getInvIndex()
        + INVENTORY_OFFSET),
        "item index");
    outMsg.writeInt8(favorite, "favorite flag");
}

void InventoryHandler::selectEgg(const Item *const item) const
{
    if (!item)
        return;
    createOutPacket(CMSG_PET_SELECT_EGG);
    outMsg.writeInt16(static_cast<int16_t>(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    menu = MenuType::Unknown;
}

int InventoryHandler::convertFromServerSlot(const int serverSlot) const
{
    if (serverSlot < 0 || serverSlot > 15)
        return 0;

    return static_cast<int>(EQUIP_CONVERT[serverSlot]);
}

void InventoryHandler::selectCart(const BeingId accountId,
                                  const int type) const
{
    createOutPacket(CMSG_SELECT_CART);
    outMsg.writeBeingId(accountId, "account id");
    outMsg.writeInt8(type, "type");
}

}  // namespace EAthena
