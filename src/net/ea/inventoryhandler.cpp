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

#include "net/ea/inventoryhandler.h"

#include "notifymanager.h"

#include "being/localplayer.h"

#include "enums/equipslot.h"

#include "enums/resources/notifytypes.h"

#include "gui/widgets/createwidget.h"

#include "net/messagein.h"

#include "net/ea/eaprotocol.h"
#include "net/ea/equipbackend.h"
#include "net/ea/inventoryrecv.h"

#include "utils/delete2.h"

#include "listeners/arrowslistener.h"

#include "debug.h"

namespace Ea
{

InventoryHandler::InventoryHandler()
{
    InventoryRecv::mEquips.clear();
    InventoryRecv::mInventoryItems.clear();
    InventoryRecv::mStorage = nullptr;
    storageWindow = nullptr;
    while (!InventoryRecv::mSentPickups.empty())
        InventoryRecv::mSentPickups.pop();
    InventoryRecv::mDebugInventory = true;
}

InventoryHandler::~InventoryHandler()
{
    if (storageWindow)
    {
        storageWindow->close();
        storageWindow = nullptr;
    }

    delete2(InventoryRecv::mStorage);
}

void InventoryHandler::clear()
{
    delete2(InventoryRecv::mStorage);
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
        case InventoryType::INVENTORY:
            return 100;
        case InventoryType::STORAGE:
            return 0;  // Comes from server after items
        case InventoryType::TRADE:
            return 12;
        // GUILD_STORAGE
        case InventoryType::TYPE_END:
            return 0;  // Comes from server after items
        default:
            return 0;
    }
}
void InventoryHandler::destroyStorage()
{
    BLOCK_START("InventoryHandler::closeStorage")
    if (storageWindow)
    {
        InventoryWindow *const inv = storageWindow;
        storageWindow->close();
        inv->unsetInventory();
    }
    BLOCK_END("InventoryHandler::closeStorage")
}

void InventoryHandler::forgotStorage()
{
    storageWindow = nullptr;
}

void InventoryHandler::pushPickup(const BeingId floorId)
{
    InventoryRecv::mSentPickups.push(floorId);
}

Inventory *InventoryHandler::getStorage() const
{
    return InventoryRecv::mStorage;
}

}  // namespace Ea
