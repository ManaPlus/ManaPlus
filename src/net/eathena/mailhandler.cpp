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

#include "net/eathena/mailhandler.h"

#include "logger.h"
#include "notifymanager.h"

#include "gui/mailmessage.h"

#include "gui/windows/mailwindow.h"

#include "net/ea/eaprotocol.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include "utils/stringutils.h"

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
        SMSG_MAIL_MAILS_LIST,
        SMSG_MAIL_READ_MAIL,
        SMSG_MAIL_GET_ATTACHMENT,
        SMSG_MAIL_SEND_MAIL_ACK,
        SMSG_MAIL_NEW_MAIL,
        SMSG_MAIL_SET_ATTACHMENT_ACK,
        SMSG_MAIL_DELETE_MAIL_ACK,
        SMSG_MAIL_RETURN,
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

        case SMSG_MAIL_MAILS_LIST:
            processMailList(msg);
            break;

        case SMSG_MAIL_READ_MAIL:
            processReadMail(msg);
            break;

        case SMSG_MAIL_GET_ATTACHMENT:
            processGetAttachment(msg);
            break;

        case SMSG_MAIL_SEND_MAIL_ACK:
            processSendMailAck(msg);
            break;

        case SMSG_MAIL_NEW_MAIL:
            processNewMail(msg);
            break;

        case SMSG_MAIL_SET_ATTACHMENT_ACK:
            processSetAttachmentAck(msg);
            break;

        case SMSG_MAIL_DELETE_MAIL_ACK:
            processDeleteAck(msg);
            break;

        case SMSG_MAIL_RETURN:
            processMailReturn(msg);
            break;

        default:
            break;
    }
}

void MailHandler::processMailOpen(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;
    const int flag = msg.readInt32("flag");
    switch (flag)
    {
        case 0:  // open window
            break;

        case 1:  // close window
            break;

        default:
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void MailHandler::processMailList(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 8) / 73;
    const int amount = msg.readInt32("amount");
    if (count != amount)
        logger->log("error: wrong mails count");
    mailWindow->clear();
    for (int f = 0; f < count; f ++)
    {
        MailMessage *const mail = new MailMessage;
        mail->id = msg.readInt32("message id");
        mail->title = msg.readString(40, "title");
        mail->unread = msg.readUInt8("unread flag") ? true : false;
        mail->sender = msg.readString(24, "sender name");
        mail->time = msg.readInt32("time stamp");
        mail->strTime = timeToStr(mail->time);
        mailWindow->addMail(mail);
    }
}

void MailHandler::processReadMail(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 101;
    MailMessage *mail = new MailMessage;
    mail->id = msg.readInt32("message id");
    mail->title = msg.readString(40, "title");
    mail->sender = msg.readString(24, "sender name");
    msg.readInt32("unused");
    mail->money = msg.readInt32("money");
    mail->itemAmount = msg.readInt32("item amount");
    mail->itemId = msg.readInt16("item id");
    mail->itemType = msg.readInt16("item type");
    mail->itemIdentify = msg.readUInt8("identify");
    mail->itemAttribute = msg.readUInt8("attribute");
    mail->itemRefine = msg.readUInt8("refine");
    for (int f = 0; f < 4; f ++)
        mail->card[f] = msg.readInt16("card");
    const int msgLen = msg.readUInt8("msg len");
    if (msgLen != sz)
        logger->log("error: wrong message size");
    mail->text = msg.readString(sz, "message");
    msg.readUInt8("zero");
    mail->strTime = timeToStr(mail->time);
    mailWindow->showMessage(mail);
}

void MailHandler::processGetAttachment(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readUInt8("flag");
}

void MailHandler::processSendMailAck(Net::MessageIn &msg)
{
    switch (msg.readUInt8("fail flag"))
    {
        case 0:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_OK);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_ERROR);
            break;
        default:
            UNIMPLIMENTEDPACKET;
            break;
    }
}

void MailHandler::processNewMail(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt32("message id");
    msg.readString(40, "title");
    msg.readString(24, "sender name");
}

void MailHandler::processSetAttachmentAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt16("index");
    msg.readUInt8("flag");
}

void MailHandler::processDeleteAck(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt32("message id");
    msg.readInt16("fail flag");
}

void MailHandler::processMailReturn(Net::MessageIn &msg)
{
    UNIMPLIMENTEDPACKET;

    msg.readInt32("message id");
    msg.readInt16("fail flag");
}

void MailHandler::refresh()
{
    createOutPacket(CMSG_MAIL_REFRESH_INBOX);
}

void MailHandler::readMessage(const int msgId)
{
    createOutPacket(CMSG_MAIL_READ_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::getAttach(const int msgId)
{
    createOutPacket(CMSG_MAIL_GET_ATTACH);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::deleteMessage(const int msgId)
{
    createOutPacket(CMSG_MAIL_DELETE_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
}

void MailHandler::returnMessage(const int msgId)
{
    createOutPacket(CMSG_MAIL_RETURN_MESSAGE);
    outMsg.writeInt32(msgId, "message id");
    outMsg.writeString("", 24, "unused");
}

void MailHandler::setAttach(const int index, const int amount)
{
    createOutPacket(CMSG_MAIL_SET_ATTACH);
    outMsg.writeInt16(static_cast<int16_t>(index + INVENTORY_OFFSET), "index");
    outMsg.writeInt32(amount, "amount");
}

void MailHandler::setAttachMoney(const int money)
{
    createOutPacket(CMSG_MAIL_SET_ATTACH);
    outMsg.writeInt16(static_cast<int16_t>(0), "index");
    outMsg.writeInt32(money, "money");
}

void MailHandler::resetAttach(const int flag)
{
    createOutPacket(CMSG_MAIL_RESET_ATTACH);
    outMsg.writeInt16(static_cast<int16_t>(flag), "flag");
}

void MailHandler::send(const std::string &name,
                       const std::string &title,
                       std::string message)
{
    if (message.size() > 255)
        message = message.substr(0, 255);
    const int sz = static_cast<int>(message.size());

    createOutPacket(CMSG_MAIL_SEND);
    outMsg.writeInt16(static_cast<int16_t>(69 + sz), "len");
    outMsg.writeString(name, 24, "name");
    outMsg.writeString(title, 40, "title");
    outMsg.writeInt8(static_cast<int8_t>(sz), "message size");
    outMsg.writeString(message, sz, "message");
}

}  // namespace EAthena
