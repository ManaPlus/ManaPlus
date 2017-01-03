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

#include "net/eathena/searchstorerecv.h"

#include "logger.h"
#include "notifymanager.h"

#include "const/resources/item/cards.h"

#include "enums/resources/notifytypes.h"

#include "net/messagein.h"

#include "debug.h"

namespace EAthena
{

void SearchStoreRecv::processSearchAck(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
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
        for (int d = 0; d < maxCards; d++)
            msg.readUInt16("card");

        // +++ need use ItemColorManager for colors
    }
}

void SearchStoreRecv::processSearchFailed(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
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

void SearchStoreRecv::processSearchOpen(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("effect");
    if (msg.getVersion() >= 20100701)
        msg.readUInt8("uses");
}

void SearchStoreRecv::processSearchClickAck(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("x");
    msg.readInt16("y");
}

}  // namespace EAthena
