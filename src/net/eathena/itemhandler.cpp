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

#include "net/eathena/itemhandler.h"

#include "actormanager.h"
#include "itemcolormanager.h"
#include "logger.h"

#include "enums/simpletypes/damaged.h"

#include "net/ea/itemrecv.h"

#include "net/eathena/itemrecv.h"
#include "net/eathena/protocol.h"

#include "debug.h"

namespace EAthena
{

ItemHandler::ItemHandler() :
    MessageHandler(),
    Ea::ItemHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_ITEM_VISIBLE,
        SMSG_ITEM_VISIBLE2,
        SMSG_ITEM_DROPPED,
        SMSG_ITEM_DROPPED2,
        SMSG_ITEM_REMOVE,
        SMSG_GRAFFITI_VISIBLE,
        SMSG_ITEM_MVP_DROPPED,
        0
    };
    handledMessages = _messages;
}

void ItemHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_ITEM_VISIBLE:
            ItemRecv::processItemVisible(msg);
            break;

        case SMSG_ITEM_DROPPED:
            ItemRecv::processItemDropped(msg);
            break;

        case SMSG_ITEM_REMOVE:
            Ea::ItemRecv::processItemRemove(msg);
            break;

        case SMSG_GRAFFITI_VISIBLE:
            ItemRecv::processGraffiti(msg);
            break;

        case SMSG_ITEM_MVP_DROPPED:
            ItemRecv::processItemMvpDropped(msg);
            break;

        case SMSG_ITEM_VISIBLE2:
            ItemRecv::processItemVisible2(msg);
            break;

        case SMSG_ITEM_DROPPED2:
            ItemRecv::processItemDropped2(msg);
            break;

        default:
            break;
    }
}

}  // namespace EAthena
