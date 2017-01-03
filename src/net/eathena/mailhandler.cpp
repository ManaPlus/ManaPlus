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

#include "net/eathena/mailhandler.h"

#include "const/net/inventory.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern Net::MailHandler *mailHandler;

namespace EAthena
{

MailHandler::MailHandler()
{
    mailHandler = this;
}

void MailHandler::refresh() const
{
    createOutPacket(CMSG_MAIL_REFRESH_INBOX);
}

void MailHandler::readMessage(const int msgId) const
{
    createOutPacket(CMSG_MAIL_READ_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::getAttach(const int msgId) const
{
    createOutPacket(CMSG_MAIL_GET_ATTACH);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::deleteMessage(const int msgId) const
{
    createOutPacket(CMSG_MAIL_DELETE_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::returnMessage(const int msgId) const
{
    createOutPacket(CMSG_MAIL_RETURN_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
    outMsg.writeString("", 24, "unused");
}

void MailHandler::setAttach(const int index, const int amount) const
{
    createOutPacket(CMSG_MAIL_SET_ATTACH);
    outMsg.writeInt16(CAST_S16(index + INVENTORY_OFFSET), "index");
    outMsg.writeInt32(amount, "amount");
}

void MailHandler::setAttachMoney(const int money) const
{
    createOutPacket(CMSG_MAIL_SET_ATTACH);
    outMsg.writeInt16(CAST_S16(0), "index");
    outMsg.writeInt32(money, "money");
}

void MailHandler::resetAttach(const int flag) const
{
    createOutPacket(CMSG_MAIL_RESET_ATTACH);
    outMsg.writeInt16(CAST_S16(flag), "flag");
}

void MailHandler::send(const std::string &name,
                       const std::string &title,
                       std::string message) const
{
    if (message.size() > 255)
        message = message.substr(0, 255);
    const int sz = CAST_S32(message.size());

    createOutPacket(CMSG_MAIL_SEND);
    outMsg.writeInt16(CAST_S16(69 + sz), "len");
    outMsg.writeString(name, 24, "name");
    outMsg.writeString(title, 40, "title");
    outMsg.writeInt8(CAST_S8(sz), "message size");
    outMsg.writeString(message, sz, "message");
}

}  // namespace EAthena
