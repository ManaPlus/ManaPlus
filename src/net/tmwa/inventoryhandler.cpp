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

#include "net/tmwa/inventoryhandler.h"

#include "const/net/inventory.h"

#include "enums/equipslot.h"

#include "net/tmwa/messageout.h"
#include "net/tmwa/protocolout.h"

#include "resources/item/item.h"

#include "debug.h"

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
    Ea::InventoryHandler()
{
    inventoryHandler = this;
}

InventoryHandler::~InventoryHandler()
{
}

void InventoryHandler::equipItem(const Item *const item) const
{
    if (item == nullptr)
        return;

    createOutPacket(CMSG_PLAYER_EQUIP);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(0, "unused");
}

void InventoryHandler::unequipItem(const Item *const item) const
{
    if (item == nullptr)
        return;

    createOutPacket(CMSG_PLAYER_UNEQUIP);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
}

void InventoryHandler::useItem(const Item *const item) const
{
    if (item == nullptr)
        return;

    createOutPacket(CMSG_PLAYER_INVENTORY_USE);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt32(item->getId(), "item id");
}

void InventoryHandler::dropItem(const Item *const item, const int amount) const
{
    if (item == nullptr)
        return;

    createOutPacket(CMSG_PLAYER_INVENTORY_DROP);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(CAST_S16(amount), "amount");
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
    if (source == InventoryType::Inventory &&
        destination == InventoryType::Storage)
    {
        createOutPacket(CMSG_MOVE_TO_STORAGE);
        outMsg.writeInt16(CAST_S16(slot + INVENTORY_OFFSET),
            "index");
        outMsg.writeInt32(amount, "amount");
    }
    else if (source == InventoryType::Storage &&
             destination == InventoryType::Inventory)
    {
        createOutPacket(CMSG_MOVE_FROM_STORAGE);
        outMsg.writeInt16(CAST_S16(slot + STORAGE_OFFSET),
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

    return CAST_S32(EQUIP_CONVERT[serverSlot]);
}

void InventoryHandler::selectCart(const BeingId accountId A_UNUSED,
                                  const int type A_UNUSED) const
{
}

void InventoryHandler::identifyItem(const Item *const item A_UNUSED) const
{
}

void InventoryHandler::mergeItemsAck(const std::vector<Item*> &items A_UNUSED)
                                     const
{
}

void InventoryHandler::mergetItemsCancel() const
{
}

}  // namespace TmwAthena
