/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "net/manaserv/inventoryhandler.h"

#include "equipment.h"
#include "inventory.h"
#include "item.h"
#include "itemshortcut.h"
#include "localplayer.h"
#include "playerinfo.h"

#include "gui/chatwindow.h"

#include "net/manaserv/connection.h"
#include "net/manaserv/messagein.h"
#include "net/manaserv/messageout.h"
#include "net/manaserv/protocol.h"

#include "resources/iteminfo.h"

#include "logger.h" // <<< REMOVE ME!

extern Net::InventoryHandler *inventoryHandler;

namespace ManaServ
{

extern Connection *gameServerConnection;

InventoryHandler::InventoryHandler()
{
    static const uint16_t _messages[] =
    {
        GPMSG_INVENTORY_FULL,
        GPMSG_INVENTORY,
        GPMSG_EQUIP,
        0
    };
    handledMessages = _messages;
    inventoryHandler = this;
}

void InventoryHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case GPMSG_INVENTORY_FULL:
            {
                PlayerInfo::clearInventory();
                PlayerInfo::getEquipment()->setBackend(&mEquips);
                int count = msg.readInt16();
                while (count--)
                {
                    unsigned int slot = msg.readInt16();
                    int id = msg.readInt16();
                    unsigned int amount = msg.readInt16();
                    PlayerInfo::setInventoryItem(slot, id, amount, 0);
                }
                while (msg.getUnreadLength())
                {
                    unsigned int slot = msg.readInt8();
                    unsigned int ref = msg.readInt16();

                    mEquips.addEquipment(slot, ref);
                }
            }
            break;

        case GPMSG_INVENTORY:
            while (msg.getUnreadLength())
            {
                unsigned int slot = msg.readInt16();
                int id = msg.readInt16();
                unsigned int amount = id ? msg.readInt16() : 0;
                PlayerInfo::setInventoryItem(slot, id, amount, 0);
            }
            break;

        case GPMSG_EQUIP:
            while (msg.getUnreadLength())
            {
                unsigned int ref = msg.readInt16();
                int count = msg.readInt8();
                while (count--)
                {
                    unsigned int slot = msg.readInt8();
                    unsigned int used = msg.readInt8();

                    mEquips.setEquipment(slot, used, ref);
                }
            }
            break;
        default:
            break;
    }
}

void InventoryHandler::equipItem(const Item *item)
{
    MessageOut msg(PGMSG_EQUIP);
    msg.writeInt8(item->getInvIndex());
    gameServerConnection->send(msg);
}

void InventoryHandler::unequipItem(const Item *item)
{
    MessageOut msg(PGMSG_UNEQUIP);
    msg.writeInt8(item->getInvIndex());
    gameServerConnection->send(msg);

/*
    // Tidy equipment directly to avoid weapon still shown bug, for instance
    int equipSlot = item->getInvIndex();
    logger->log("Unequipping %d", equipSlot);
    mEquips.setEquipment(equipSlot, 0);
*/
}

void InventoryHandler::useItem(const Item *item)
{
    MessageOut msg(PGMSG_USE_ITEM);
    msg.writeInt8(item->getInvIndex());
    gameServerConnection->send(msg);
}

void InventoryHandler::dropItem(const Item *item, int amount)
{
    MessageOut msg(PGMSG_DROP);
    msg.writeInt8(item->getInvIndex());
    msg.writeInt8(amount);
    gameServerConnection->send(msg);
}

bool InventoryHandler::canSplit(const Item *item) const
{
    return item && !item->isEquipment() && item->getQuantity() > 1;
}

void InventoryHandler::splitItem(const Item *item, int amount)
{
    int newIndex = PlayerInfo::getInventory()->getFreeSlot();
    if (newIndex > Inventory::NO_SLOT_INDEX)
    {
        MessageOut msg(PGMSG_MOVE_ITEM);
        msg.writeInt8(item->getInvIndex());
        msg.writeInt8(newIndex);
        msg.writeInt8(amount);
        gameServerConnection->send(msg);
    }
}

void InventoryHandler::moveItem(int oldIndex, int newIndex)
{
    if (oldIndex == newIndex)
        return;

    MessageOut msg(PGMSG_MOVE_ITEM);
    msg.writeInt8(oldIndex);
    msg.writeInt8(newIndex);
    msg.writeInt8(PlayerInfo::getInventory()->getItem(oldIndex)
                  ->getQuantity());
    gameServerConnection->send(msg);
}

void InventoryHandler::openStorage(int type A_UNUSED)
{
    // TODO
}

void InventoryHandler::closeStorage(int type A_UNUSED)
{
    // TODO
}

void InventoryHandler::moveItem2(int source A_UNUSED, int slot A_UNUSED,
                                 int amount A_UNUSED, int destination A_UNUSED)
{
    // TODO
}

size_t InventoryHandler::getSize(int type) const
{
    switch (type)
    {
        case Inventory::INVENTORY:
        case Inventory::TRADE:
            return 50;
        case Inventory::STORAGE:
            return 300;
        default:
            return 0;
    }
}

int InventoryHandler::convertFromServerSlot(int eAthenaSlot) const
{
    return eAthenaSlot;
}

} // namespace ManaServ
