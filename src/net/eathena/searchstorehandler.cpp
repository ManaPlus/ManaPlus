/*
 *  The ManaPlus Client
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

#include "net/eathena/searchstorehandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::SearchStoreHandler *searchStoreHandler;

namespace EAthena
{

SearchStoreHandler::SearchStoreHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        0
    };
    handledMessages = _messages;
    searchStoreHandler = this;
}

void SearchStoreHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        default:
            break;
    }
}

void SearchStoreHandler::search(const StoreSearchType::Type type,
                                const int minPrice,
                                const int maxPrice,
                                const int itemId) const
{
    createOutPacket(CMSG_SEARCHSTORE_SEARCH);
    outMsg.writeInt16(23, "len");
    outMsg.writeInt8(static_cast<uint8_t>(type), "search type");
    outMsg.writeInt32(maxPrice, "max price");
    outMsg.writeInt32(minPrice, "min price");
    outMsg.writeInt32(1, "items count");
    outMsg.writeInt32(0, "cards count");
    outMsg.writeInt16(itemId, "item id");
}

}  // namespace EAthena
