/*
 *  The ManaPlus Client
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

#include "net/eathena/searchstorehandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

SearchStoreHandler::SearchStoreHandler() :
    Net::SearchStoreHandler()
{
    searchStoreHandler = this;
}

SearchStoreHandler::~SearchStoreHandler()
{
    searchStoreHandler = nullptr;
}

void SearchStoreHandler::search(const StoreSearchTypeT type,
                                const int minPrice,
                                const int maxPrice,
                                const int itemId) const
{
    if (packetVersion < 20100601)
        return;

    createOutPacket(CMSG_SEARCHSTORE_SEARCH);
    outMsg.writeInt16(23, "len");
    outMsg.writeInt8(CAST_U8(type), "search type");
    outMsg.writeInt32(maxPrice, "max price");
    outMsg.writeInt32(minPrice, "min price");
    outMsg.writeInt32(1, "items count");
    outMsg.writeInt32(0, "cards count");
    outMsg.writeInt16(CAST_S16(itemId), "item id");
}

void SearchStoreHandler::nextPage() const
{
    if (packetVersion < 20100608)
        return;
    createOutPacket(CMSG_SEARCHSTORE_NEXT_PAGE);
}

void SearchStoreHandler::close() const
{
    if (packetVersion < 20100608)
        return;
    createOutPacket(CMSG_SEARCHSTORE_CLOSE);
}

void SearchStoreHandler::select(const int accountId,
                                const int storeId,
                                const int itemId) const
{
    if (packetVersion < 20100608)
        return;
    createOutPacket(CMSG_SEARCHSTORE_CLICK);
    outMsg.writeInt32(accountId, "account id");
    outMsg.writeInt32(storeId, "store id");
    outMsg.writeInt16(CAST_S16(itemId), "item id");
}

}  // namespace EAthena
