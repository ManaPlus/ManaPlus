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

#include "net/eathena/mailrecv.h"

#include "itemcolormanager.h"
#include "notifymanager.h"

#include "const/resources/item/cards.h"

#include "enums/resources/notifytypes.h"

#include "gui/mailmessage.h"

#include "gui/windows/mailviewwindow.h"
#include "gui/windows/mailwindow.h"

#include "net/mailhandler.h"
#include "net/messagein.h"

#include "utils/checkutils.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/inventory/inventory.h"

#include "debug.h"

namespace EAthena
{

void MailRecv::processMailOpen(Net::MessageIn &msg)
{
    const int flag = msg.readInt32("flag");
    switch (flag)
    {
        case 0:  // open window
            if (mailWindow != nullptr)
            {
                if (!mailWindow->isWindowVisible())
                    mailWindow->setVisible(Visible_true);
                mailWindow->requestMoveToTop();
            }
            break;

        case 1:  // close window
            if (mailWindow != nullptr)
            {
                if (mailWindow->isWindowVisible())
                    mailWindow->setVisible(Visible_false);
            }
            break;

        default:
            UNIMPLEMENTEDPACKETFIELD(flag);
            break;
    }
}

void MailRecv::processMailList(Net::MessageIn &msg)
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
        mail->read = msg.readUInt8("unread flag") != 0u ? true : false;
        mail->sender = msg.readString(24, "sender name");
        mail->time = msg.readInt32("time stamp");
        mail->strTime = timeToStr(mail->time);
        mailWindow->addMail(mail);
    }
}

void MailRecv::processReadMail(Net::MessageIn &msg)
{
    const int sz = msg.readInt16("len") - 101;
    MailMessage *const mail = new MailMessage;
    mail->id = msg.readInt32("message id");
    mail->title = msg.readString(40, "title");
    mail->sender = msg.readString(24, "sender name");
    msg.readInt32("unused");
    mail->money = msg.readInt32("money");

    const int amount = msg.readInt32("item amount");
    const int itemId = msg.readInt16("item id");
    const ItemTypeT itemType = static_cast<ItemTypeT>(
        msg.readInt16("item type"));
    const uint8_t identify = msg.readUInt8("identify");
    const Damaged damaged = fromBool(msg.readUInt8("attribute"), Damaged);
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int d = 0; d < maxCards; d ++)
        cards[d] = msg.readUInt16("card");
    const int msgLen = msg.readUInt8("msg len");
    if (msgLen != sz)
        logger->log("error: wrong message size");
    mail->text = msg.readString(sz, "message");
    msg.readUInt8("zero");
    mail->strTime = timeToStr(mail->time);
    if (!mailWindow)
    {
        reportAlways("Mail window not created");
        return;
    }
    mailWindow->showMessage(mail, itemId != 0 ? 1 : 0);
    if (!mailViewWindow)
    {
        reportAlways("Mail view window not created");
        return;
    }

    Inventory *const inventory = mailViewWindow->getInventory();
    if (!inventory)
    {
        reportAlways("Mail view window missing inventory");
        return;
    }

    const int slot = inventory->addItem(itemId,
        itemType,
        amount,
        refine,
        ItemColorManager::getColorFromCards(&cards[0]),
        fromBool(identify, Identified),
        damaged,
        Favorite_false,
        Equipm_false,
        Equipped_false);
    if (slot != -1)
        inventory->setCards(slot, cards, maxCards);

    mailViewWindow->updateItems();
}

void MailRecv::processGetAttachment(Net::MessageIn &msg)
{
    const uint8_t flag = msg.readUInt8("flag");
    switch (flag)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::MAIL_GET_ATTACH_OK);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::MAIL_GET_ATTACH_ERROR);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::MAIL_GET_ATTACH_TOO_MANY_ITEMS);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(flag);
            break;
    }
}

void MailRecv::processSendMailAck(Net::MessageIn &msg)
{
    const uint8_t flag = msg.readUInt8("fail flag");
    switch (flag)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_OK);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_ERROR);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(flag);
            break;
    }
}

void MailRecv::processNewMail(Net::MessageIn &msg)
{
    msg.readInt32("message id");
    const std::string subj = msg.readString(40, "title");
    const std::string sender = msg.readString(24, "sender name");
    NotifyManager::notify(NotifyTypes::NEW_MAIL,
        // TRANSLATORS: mail message notification
        strprintf(_("You have new mail from %s with subject %s"),
        sender.c_str(), subj.c_str()));
    mailHandler->refresh();
}

void MailRecv::processSetAttachmentAck(Net::MessageIn &msg)
{
    const int index = msg.readInt16("index");
    const int flag = msg.readUInt8("flag");
    if (flag != 0)
    {
        if (index != 0)
            NotifyManager::notify(NotifyTypes::MAIL_ATTACH_ITEM_ERROR);
        else
            NotifyManager::notify(NotifyTypes::MAIL_ATTACH_MONEY_ERROR);
    }
}

void MailRecv::processDeleteAck(Net::MessageIn &msg)
{
    const int mail = msg.readInt32("message id");
    const int flag = msg.readInt16("fail flag");
    if (flag != 0)
    {
        NotifyManager::notify(NotifyTypes::MAIL_DELETE_ERROR);
    }
    else
    {
        NotifyManager::notify(NotifyTypes::MAIL_DELETE_OK);
        mailWindow->removeMail(mail);
    }
}

void MailRecv::processMailReturn(Net::MessageIn &msg)
{
    const int mail = msg.readInt32("message id");
    const int flag = msg.readInt16("fail flag");
    if (flag != 0)
    {
        NotifyManager::notify(NotifyTypes::MAIL_RETURN_ERROR);
    }
    else
    {
        NotifyManager::notify(NotifyTypes::MAIL_RETURN_OK);
        mailWindow->removeMail(mail);
    }
}

}  // namespace EAthena
