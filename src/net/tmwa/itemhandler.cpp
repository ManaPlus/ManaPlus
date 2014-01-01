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

#include "net/tmwa/itemhandler.h"

#include "net/tmwa/protocol.h"

#include "debug.h"

namespace TmwAthena
{

ItemHandler::ItemHandler() :
    MessageHandler(),
    Ea::ItemHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_ITEM_VISIBLE,
        SMSG_ITEM_DROPPED,
        SMSG_ITEM_REMOVE,
        0
    };
    handledMessages = _messages;
}

void ItemHandler::handleMessage(Net::MessageIn &msg)
{
    BLOCK_START("ItemHandler::handleMessage")
    switch (msg.getId())
    {
        case SMSG_ITEM_VISIBLE:
            processItemVisible(msg);
            break;

        case SMSG_ITEM_DROPPED:
            processItemDropped(msg);
            break;

        case SMSG_ITEM_REMOVE:
            processItemRemove(msg);
            break;

        default:
            break;
    }
    BLOCK_END("ItemHandler::handleMessage")
}

}  // namespace TmwAthena
