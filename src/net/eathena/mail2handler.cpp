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

#include "net/eathena/mail2handler.h"

#include "const/net/inventory.h"

#include "net/eathena/messageout.h"
#include "net/eathena/updateprotocol.h"
#include "net/eathena/protocolout.h"

#include "resources/item/item.h"

#include "debug.h"

namespace EAthena
{

Mail2Handler::Mail2Handler()
{
    mail2Handler = this;
}

Mail2Handler::~Mail2Handler()
{
    mail2Handler = nullptr;
}

void Mail2Handler::openWriteMail(const std::string &receiver) const
{
    createOutPacket(CMSG_MAIL2_OPEN_WRITE_MAIL);
    outMsg.writeString(receiver, 24, "receiver name");
}

void Mail2Handler::addItem(const Item *const item,
                           const int amount) const
{
    if (item == nullptr)
        return;

    createOutPacket(CMSG_MAIL2_ADD_ITEM_TO_MAIL);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(CAST_S16(amount), "amount");
}

}  // namespace EAthena
