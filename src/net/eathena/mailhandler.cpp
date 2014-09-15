/*
 *  The ManaPlus Client
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

#include "net/eathena/mailhandler.h"

#include "logger.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "debug.h"

extern Net::MailHandler *mailHandler;

namespace EAthena
{

MailHandler::MailHandler() :
    MessageHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_MAIL_OPEN_WINDOW,
        0
    };
    handledMessages = _messages;
    mailHandler = this;
}

void MailHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_MAIL_OPEN_WINDOW:
            processMailOpen(msg);
            break;

        default:
            break;
    }
}

void MailHandler::processMailOpen(Net::MessageIn &msg) const
{
    const int flag = msg.readInt32("flag");
    switch (flag)
    {
        case 0:  // open window
            break;

        case 1:  // close window
            break;

        default:
            logger->log("unknown mail window open flag: %u", flag);
            break;
    }
}

void MailHandler::refresh()
{
    MessageOut outMsg(CMSG_MAIL_REFRESH_INBOX);
}

void MailHandler::readMessage(const int msgId)
{
    MessageOut outMsg(CMSG_MAIL_READ_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::getAttach(const int msgId)
{
    MessageOut outMsg(CMSG_MAIL_GET_ATTACH);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::deleteMessage(const int msgId)
{
    MessageOut outMsg(CMSG_MAIL_DELETE_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::returnMessage(const int msgId)
{
    MessageOut outMsg(CMSG_MAIL_RETURN_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
    outMsg.writeString("", 24, "unused");
}

void MailHandler::setAttach(const int index, const int amount)
{
    MessageOut outMsg(CMSG_MAIL_SET_ATTACH);
    outMsg.writeInt16(static_cast<int16_t>(index + INVENTORY_OFFSET), "index");
    outMsg.writeInt32(amount, "amount");
}

}  // namespace EAthena
