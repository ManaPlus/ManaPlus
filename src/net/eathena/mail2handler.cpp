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

#include "being/localplayer.h"

#include "net/eathena/messageout.h"
#include "net/eathena/updateprotocol.h"
#include "net/eathena/protocolout.h"

#include "resources/item/item.h"

#include "debug.h"

extern int packetVersion;
extern int serverVersion;

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
    if (packetVersion < 20140416 ||
        serverVersion < 19)
    {
        return;
    }
    createOutPacket(CMSG_MAIL2_OPEN_WRITE_MAIL);
    outMsg.writeString(receiver, 24, "receiver name");
}

void Mail2Handler::addItem(const Item *const item,
                           const int amount) const
{
    if (item == nullptr)
        return;
    if (packetVersion < 20140416 ||
        serverVersion < 19)
    {
        return;
    }

    createOutPacket(CMSG_MAIL2_ADD_ITEM_TO_MAIL);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(CAST_S16(amount), "amount");
}

void Mail2Handler::removeItem(const Item *const item,
                              const int amount) const
{
    if (item == nullptr)
        return;
    if (packetVersion < 20140416 ||
        serverVersion < 19)
    {
        return;
    }

    createOutPacket(CMSG_MAIL2_REMOVE_ITEM_MAIL);
    outMsg.writeInt16(CAST_S16(
        item->getInvIndex() + INVENTORY_OFFSET), "index");
    outMsg.writeInt16(CAST_S16(amount), "amount");
}

void Mail2Handler::sendMail(const std::string &to,
                            const std::string &title,
                            const std::string &body,
                            const int64_t &money) const
{
    if (packetVersion < 20131230 ||
        serverVersion < 19)
    {
        return;
    }
    if (localPlayer == nullptr)
        return;

    const std::string from = localPlayer->getName();
    const int titleSz = title.size();
    const int bodySz = body.size();
    int16_t sz = 2 + 2 + 24 + 24 + 8 + 2 + 2 + titleSz + bodySz;
    if (packetVersion >= 20160600)
        sz += 4;

    createOutPacket(CMSG_MAIL2_SEND_MAIL);
    outMsg.writeInt16(sz, "len");
    outMsg.writeString(to, 24, "to");
    outMsg.writeString(from, 24, "from");
    outMsg.writeInt64(money, "money");
    outMsg.writeInt16(CAST_S16(title.size()), "title len");
    outMsg.writeInt16(CAST_S16(body.size()), "body len");
    if (packetVersion >= 20160600)
        outMsg.writeInt32(0, "to char id");
    outMsg.writeString(title, titleSz, "title");
    outMsg.writeString(body, bodySz, "body");
}

void Mail2Handler::nextPage(const int openType,
                            const int64_t mailId) const
{
    if (packetVersion < 20131218 ||
        serverVersion < 19)
    {
        return;
    }
    createOutPacket(CMSG_MAIL2_NEXT_PAGE);
    outMsg.writeInt8(openType, "open type");
    outMsg.writeInt64(mailId, "mail id");
}

void Mail2Handler::readMail(const int openType,
                            const int64_t mailId) const
{
    if (packetVersion < 20131223 ||
        serverVersion < 19)
    {
        return;
    }
    createOutPacket(CMSG_MAIL2_READ_MAIL);
    outMsg.writeInt8(openType, "open type");
    outMsg.writeInt64(mailId, "mail id");
}

void Mail2Handler::deleteMail(const int openType,
                              const int64_t mailId) const
{
    if (packetVersion < 20131218 ||
        serverVersion < 19)
    {
        return;
    }
    createOutPacket(CMSG_MAIL2_DELETE_MAIL);
    outMsg.writeInt8(openType, "open type");
    outMsg.writeInt64(mailId, "mail id");
}

void Mail2Handler::requestMoney(const int openType,
                                const int64_t mailId) const
{
    if (packetVersion < 20140326 ||
        serverVersion < 19)
    {
        return;
    }
    createOutPacket(CMSG_MAIL2_REQUEST_MONEY);
    outMsg.writeInt64(mailId, "mail id");
    outMsg.writeInt8(openType, "open type");
}

}  // namespace EAthena
