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

#include "logger.h"
#include "notifymanager.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern Net::SearchStoreHandler *searchStoreHandler;

namespace EAthena
{

SearchStoreHandler::SearchStoreHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_SEARCHSTORE_SEARCH_ACK,
        SMSG_SEARCHSTORE_SEARCH_FAILED,
        SMSG_SEARCHSTORE_OPEN,
        SMSG_SEARCHSTORE_CLICK_ACK,
        0
    };
    handledMessages = _messages;
    searchStoreHandler = this;
}

void SearchStoreHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_SEARCHSTORE_SEARCH_ACK:
            processSearchAck(msg);
            break;

        case SMSG_SEARCHSTORE_SEARCH_FAILED:
            processSearchFailed(msg);
            break;

        case SMSG_SEARCHSTORE_OPEN:
            processSearchOpen(msg);
            break;

        case SMSG_SEARCHSTORE_CLICK_ACK:
            processSearchClickAck(msg);
            break;

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
    outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
}

void SearchStoreHandler::nextPage() const
{
    createOutPacket(CMSG_SEARCHSTORE_NEXT_PAGE);
}

void SearchStoreHandler::close() const
{
    createOutPacket(CMSG_SEARCHSTORE_CLOSE);
}

void SearchStoreHandler::select(const int accountId,
                                const int storeId,
                                const int itemId) const
{
    createOutPacket(CMSG_SEARCHSTORE_CLICK);
    outMsg.writeInt32(accountId, "account id");
    outMsg.writeInt32(storeId, "store id");
    outMsg.writeInt16(static_cast<int16_t>(itemId), "item id");
}

void SearchStoreHandler::processSearchAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    const int count = (msg.readInt16("len") - 7) / 106;
    msg.readUInt8("is first page");
    msg.readUInt8("is next page");
    msg.readUInt8("remain uses");
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("store id");
        msg.readInt32("aoount id");
        msg.readString(80, "store name");
        msg.readInt16("item id");
        msg.readUInt8("item type");
        msg.readInt32("price");
        msg.readInt16("amount");
        msg.readUInt8("refine");
        for (int d = 0; d < 4; d++)
            msg.readInt16("card");
    }
}

void SearchStoreHandler::processSearchFailed(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    const int result = msg.readUInt8("result");
    switch (result)
    {
        case 0:
            NotifyManager::notify(
                NotifyTypes::SEARCH_STORE_FAILED_NO_STORES);
            break;
        case 1:
            NotifyManager::notify(
                NotifyTypes::SEARCH_STORE_FAILED_MANY_RESULTS);
            break;
        case 2:
            NotifyManager::notify(
                NotifyTypes::SEARCH_STORE_FAILED_CANT_SEARCH_ANYMORE);
            break;
        case 3:
            NotifyManager::notify(
                NotifyTypes::SEARCH_STORE_FAILED_CANT_SEARCH_YET);
            break;
        case 4:
            NotifyManager::notify(
                NotifyTypes::SEARCH_STORE_FAILED_NO_INFORMATION);
            break;
        default:
            NotifyManager::notify(
                NotifyTypes::SEARCH_STORE_FAILED);
            break;
    }
}

void SearchStoreHandler::processSearchOpen(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("effect");
    msg.readUInt8("uses");
}

void SearchStoreHandler::processSearchClickAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    msg.readInt16("x");
    msg.readInt16("y");
}

}  // namespace EAthena
