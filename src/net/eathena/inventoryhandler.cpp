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

#include "net/eathena/inventoryhandler.h"

#include "notifymanager.h"
#include "itemcolormanager.h"

#include "being/localplayer.h"

#include "enums/equipslot.h"

#include "enums/resources/notifytypes.h"

#include "gui/popups/itempopup.h"

#include "gui/widgets/createwidget.h"

#include "gui/windows/insertcarddialog.h"

#include "listeners/arrowslistener.h"

#include "net/ea/inventoryrecv.h"

#include "net/eathena/inventoryrecv.h"
#include "net/eathena/itemflags.h"
#include "net/eathena/menu.h"
#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "net/ea/eaprotocol.h"
#include "net/ea/equipbackend.h"

#include "resources/iteminfo.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

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
    MessageHandler(),
    Ea::InventoryHandler(),
    mItemIndex(0)
{
    static const uint16_t _messages[] =
    {
        SMSG_PLAYER_INVENTORY,
        SMSG_PLAYER_INVENTORY_ADD,
        SMSG_PLAYER_INVENTORY_REMOVE,
        SMSG_PLAYER_INVENTORY_REMOVE2,
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
        SMSG_PLAYER_USE_CARD,
        SMSG_PLAYER_INSERT_CARD,
        SMSG_PLAYER_ITEM_RENTAL_TIME,
        SMSG_PLAYER_ITEM_RENTAL_EXPIRED,
        SMSG_CART_INFO,
        SMSG_CART_REMOVE,
        SMSG_PLAYER_CART_ADD,
        SMSG_PLAYER_CART_EQUIP,
        SMSG_PLAYER_CART_ITEMS,
        SMSG_PLAYER_CART_REMOVE,
        SMSG_PLAYER_IDENTIFY_LIST,
        SMSG_PLAYER_IDENTIFIED,
        SMSG_PLAYER_REFINE,
        SMSG_PLAYER_REPAIR_LIST,
        SMSG_PLAYER_REPAIR_EFFECT,
        SMSG_PLAYER_REFINE_LIST,
        SMSG_PLAYER_STORAGE_PASSWORD,
        SMSG_PLAYER_STORAGE_PASSWORD_RESULT,
        SMSG_PLAYER_COOKING_LIST,
        SMSG_ITEM_DAMAGED,
        SMSG_PLAYER_FAVORITE_ITEM,
        SMSG_PLAYER_CART_ADD_ERROR,
        SMSG_BIND_ITEM,
        0
    };
    handledMessages = _messages;
    inventoryHandler = this;

    InventoryRecv::mCartItems.clear();
}

InventoryHandler::~InventoryHandler()
{
}

void InventoryHandler::handleMessage(Net::MessageIn &msg)
{
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

        case SMSG_PLAYER_INVENTORY_REMOVE2:
            InventoryRecv::processPlayerInventoryRemove2(msg);
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

        case SMSG_PLAYER_USE_CARD:
            InventoryRecv::processPlayerUseCard(msg);
            break;

        case SMSG_PLAYER_INSERT_CARD:
            InventoryRecv::processPlayerInsertCard(msg);
            break;

        case SMSG_PLAYER_ITEM_RENTAL_TIME:
            InventoryRecv::processPlayerItemRentalTime(msg);
            break;

        case SMSG_PLAYER_ITEM_RENTAL_EXPIRED:
            InventoryRecv::processPlayerItemRentalExpired(msg);
            break;

        case SMSG_CART_INFO:
            InventoryRecv::processCartInfo(msg);
            break;

        case SMSG_CART_REMOVE:
            InventoryRecv::processCartRemove(msg);
            break;

        case SMSG_PLAYER_CART_ADD:
            InventoryRecv::processPlayerCartAdd(msg);
            break;

        case SMSG_PLAYER_CART_EQUIP:
            InventoryRecv::processPlayerCartEquip(msg);
            break;

        case SMSG_PLAYER_CART_ITEMS:
            InventoryRecv::processPlayerCartItems(msg);
            break;

        case SMSG_PLAYER_CART_REMOVE:
            InventoryRecv::processPlayerCartRemove(msg);
            break;

        case SMSG_PLAYER_IDENTIFY_LIST:
            InventoryRecv::processPlayerIdentifyList(msg);
            break;

        case SMSG_PLAYER_IDENTIFIED:
            InventoryRecv::processPlayerIdentified(msg);
            break;

        case SMSG_PLAYER_REFINE:
            InventoryRecv::processPlayerRefine(msg);
            break;

        case SMSG_PLAYER_REPAIR_LIST:
            InventoryRecv::processPlayerRepairList(msg);
            break;

        case SMSG_PLAYER_REPAIR_EFFECT:
            InventoryRecv::processPlayerRepairEffect(msg);
            break;

        case SMSG_PLAYER_REFINE_LIST:
            InventoryRecv::processPlayerRefineList(msg);
            break;

        case SMSG_PLAYER_STORAGE_PASSWORD:
            InventoryRecv::processPlayerStoragePassword(msg);
            break;

        case SMSG_PLAYER_STORAGE_PASSWORD_RESULT:
            InventoryRecv::processPlayerStoragePasswordResult(msg);
            break;

        case SMSG_PLAYER_COOKING_LIST:
            InventoryRecv::processPlayerCookingList(msg);
            break;

        case SMSG_ITEM_DAMAGED:
            InventoryRecv::processItemDamaged(msg);
            break;

        case SMSG_PLAYER_FAVORITE_ITEM:
            InventoryRecv::processFavoriteItem(msg);
            break;

        case SMSG_PLAYER_CART_ADD_ERROR:
            InventoryRecv::processCartAddError(msg);
            break;

        case SMSG_BIND_ITEM:
            InventoryRecv::processBindItem(msg);
            break;

        default:
            break;
    }
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

void InventoryHandler::closeStorage(const int type A_UNUSED) const
{
    createOutPacket(CMSG_CLOSE_STORAGE);
}

void InventoryHandler::moveItem2(const int source,
                                 const int slot,
                                 const int amount,
                                 const int destination) const
{
    int packet = 0;
    int offset = INVENTORY_OFFSET;
    if (source == InventoryType::INVENTORY)
    {
        if (destination == InventoryType::STORAGE)
            packet = CMSG_MOVE_TO_STORAGE;
        else if (destination == InventoryType::CART)
            packet = CMSG_MOVE_TO_CART;
    }
    else if (source == InventoryType::STORAGE)
    {
        offset = STORAGE_OFFSET;
        if (destination == InventoryType::INVENTORY)
            packet = CMSG_MOVE_FROM_STORAGE;
        else if (destination == InventoryType::CART)
            packet = CMSG_MOVE_FROM_STORAGE_TO_CART;
    }
    else if (source == InventoryType::CART)
    {
        if (destination == InventoryType::INVENTORY)
            packet = CMSG_MOVE_FROM_CART;
        else if (destination == InventoryType::STORAGE)
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

}  // namespace EAthena
