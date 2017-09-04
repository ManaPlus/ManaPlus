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

#include "net/eathena/mail2recv.h"

#include "itemcolormanager.h"
#include "notifymanager.h"

#include "const/net/inventory.h"

#include "being/playerinfo.h"

#include "enums/resources/notifytypes.h"

#include "gui/mailmessage.h"

#include "gui/windows/maileditwindow.h"
#include "gui/windows/mailviewwindow.h"
#include "gui/windows/mailwindow.h"

#include "net/mail2handler.h"
#include "net/messagein.h"

#include "resources/mailqueue.h"

#include "resources/inventory/inventory.h"

#include "resources/item/item.h"
#include "resources/item/itemoptionslist.h"

#include "utils/checkutils.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

namespace EAthena
{

namespace Mail2Recv
{
    std::queue<MailQueue*> mMailQueue;
    std::string mCheckedName;
}  // namespace Mail2Recv

void Mail2Recv::processMailIcon(Net::MessageIn &msg)
{
    // ignored, because if has new mail, server send chat message already.
    msg.readUInt8("has new mail");
}

void Mail2Recv::processOpenNewMailWindow(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readString(24, "receiver");
    msg.readUInt8("result");
}

void Mail2Recv::processAddItemResult(Net::MessageIn &msg)
{
    const int res = msg.readUInt8("result");
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("amount");
    const int itemId = msg.readInt16("item id");
    const ItemTypeT itemType = static_cast<ItemTypeT>(
        msg.readUInt8("item type"));
    const uint8_t identify = msg.readUInt8("identify");
    const Damaged damaged = fromBool(msg.readUInt8("attribute"), Damaged);
    const uint8_t refine = msg.readUInt8("refine");
    int cards[maxCards];
    for (int f = 0; f < maxCards; f++)
        cards[f] = msg.readUInt16("card");
    ItemOptionsList *options = new ItemOptionsList(5);
    for (int f = 0; f < 5; f ++)
    {
        const uint16_t idx = msg.readInt16("option index");
        const uint16_t val = msg.readInt16("option value");
        msg.readUInt8("option param");
        options->add(idx, val);
    }
    msg.readInt16("weight");
    msg.readUInt8("unknown 1");
    msg.readUInt8("unknown 2");
    msg.readUInt8("unknown 3");
    msg.readUInt8("unknown 4");
    msg.readUInt8("unknown 5");

    if (mailEditWindow == nullptr)
    {
        reportAlways("Mail edit window not created");
        delete options;
        return;
    }
    Inventory *const inventory = mailEditWindow->getInventory();
    if (inventory == nullptr)
    {
        reportAlways("Mail edit window inventory not exists");
        delete options;
        return;
    }

    if (res != 0)
    {
        switch (res)
        {
            case 1:
                NotifyManager::notify(
                    NotifyTypes::MAIL_ATTACH_ITEM_WEIGHT_ERROR);
                break;
            case 2:
                NotifyManager::notify(
                    NotifyTypes::MAIL_ATTACH_ITEM_FATAL_ERROR);
                break;
            case 3:
                NotifyManager::notify(
                    NotifyTypes::MAIL_ATTACH_ITEM_NO_SPACE);
                break;
            case 4:
                NotifyManager::notify(
                    NotifyTypes::MAIL_ATTACH_ITEM_NOT_TRADEABLE);
                break;
            default:
                NotifyManager::notify(
                    NotifyTypes::MAIL_ATTACH_ITEM_UNKNOWN_ERROR);
                UNIMPLEMENTEDPACKETFIELD(res);
                break;
        }
        delete options;
        return;
    }

    Item *const item = inventory->findItemByTag(index);
    if (item == nullptr)
    {
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
        if (slot == -1)
        {
            delete options;
            return;
        }
        inventory->setCards(slot, cards, maxCards);
        inventory->setOptions(slot, options);
        inventory->setTag(slot, index);
    }
    else
    {
        item->increaseQuantity(amount);
    }

    mailEditWindow->updateItems();
    delete options;
}

void Mail2Recv::processRemoveItemResult(Net::MessageIn &msg)
{
    const int result = msg.readUInt8("result");
    const int index = msg.readInt16("index") - INVENTORY_OFFSET;
    const int amount = msg.readInt16("count");
    msg.readInt16("weight");

    if (result == 0)
    {
        const Inventory *const inv = PlayerInfo::getInventory();
        if (inv == nullptr)
        {
            reportAlways("Player inventory not exists");
            return;
        }
        std::string itemName;
        const Item *const item = inv->getItem(index);
        if (item != nullptr)
        {
            itemName = item->getName();
        }
        else
        {
            // TRANSLATORS: unknown item name
            itemName = _("Unknown item");
        }

        NotifyManager::notify(
            NotifyTypes::MAIL_REMOVE_ITEM_ERROR,
            itemName);
        return;
    }
    if (mailEditWindow == nullptr)
    {
        reportAlways("Mail edit window not created");
        return;
    }
    Inventory *const inventory = mailEditWindow->getInventory();
    if (inventory == nullptr)
    {
        reportAlways("Mail edit window inventory not exists");
        return;
    }
    const int index2 = inventory->findIndexByTag(index);
    if (index2 == -1)
    {
        reportAlways("Item not exists in mail edit window.");
        return;
    }
    Item *const item = inventory->getItem(index2);
    if (item == nullptr)
    {
        reportAlways("Item not exists.");
        return;
    }

    item->increaseQuantity(-amount);
    mailEditWindow->updateItems();
}

void Mail2Recv::processCheckNameResult(Net::MessageIn &msg)
{
    const int charId = msg.readInt32("char id");
    msg.readInt16("class");
    msg.readInt16("level");
    if (msg.getVersion() >= 20160316)
        msg.readString(24, "name");
    // +++ in future if name received, need use it in map
    if (mMailQueue.empty())
    {
        reportAlways("Mail2Recv::processCheckNameResult no names in queue."
            "Char id: %d", charId);
        return;
    }
    MailQueue *const mail = mMailQueue.front();
    mMailQueue.pop();
    if (charId == 0)
    {
        NotifyManager::notify(NotifyTypes::MAIL_NAME_VALIDATION_ERROR,
            mail->to);
        delete mail;
        return;
    }
    mCheckedName = mail->to;
    switch (mail->type)
    {
        case MailQueueType::SendMail:
            mail2Handler->sendMail(mail->to,
                mail->title,
                mail->body,
                mail->money);
            break;
        case MailQueueType::EditMail:
            if (mailWindow == nullptr)
            {
                reportAlways("Mail window not created");
            }
            else
            {
                mailWindow->createMail(mail->to);
            }
            break;
        case MailQueueType::ValidateTo:
            if (mailEditWindow == nullptr)
            {
                reportAlways("Mail edit window not created");
            }
            else
            {
                mailEditWindow->validatedTo();
            }
            break;
        case MailQueueType::Unknown:
        default:
            reportAlways("Not implemented yet.");
            break;
    }
    delete mail;
}

void Mail2Recv::processSendResult(Net::MessageIn &msg)
{
    const int res = msg.readUInt8("result");
    switch (res)
    {
        case 0:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_OK);
            if (mailEditWindow != nullptr)
                mailEditWindow->close();
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_FATAL_ERROR);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_COUNT_ERROR);
            break;
        case 3:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_ITEM_ERROR);
            break;
        case 4:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_RECEIVER_ERROR);
            break;
        default:
            NotifyManager::notify(NotifyTypes::MAIL_SEND_ERROR);
            break;
    }
}

void Mail2Recv::processMailListPage(Net::MessageIn &msg)
{
    if (mailWindow == nullptr)
    {
        reportAlways("mail window not created");
        return;
    }
    msg.readInt16("len");
    mailWindow->setOpenType(fromInt(msg.readUInt8("open type"),
        MailOpenTypeT));
    const int cnt = msg.readUInt8("cnt");
    const bool isEnd = msg.readUInt8("isEnd") != 0;
    for (int f = 0; f < cnt; f ++)
    {
        MailMessage *const mail = new MailMessage;
        mail->id = msg.readInt64("mail id");
        mail->read = msg.readUInt8("is read") != 0U ? true : false;
        mail->type = static_cast<MailMessageType::Type>(
            msg.readUInt8("type"));
        mail->sender = msg.readString(24, "sender name");
        mail->time = msg.readInt32("reg time");
        mail->strTime = timeToStr(mail->time);
        mail->expireTime = msg.readInt32("expire time");
        mail->title = msg.readString(-1, "title");
        mailWindow->addMail(mail);
    }
    if (isEnd)
        mailWindow->setLastPage();
}

void Mail2Recv::processReadMail(Net::MessageIn &msg)
{
    msg.readInt16("len");
    const MailOpenTypeT openType = static_cast<MailOpenTypeT>(
        msg.readUInt8("open type"));
    const int64_t mailId = msg.readInt64("mail id");
    const int textLen = msg.readInt16("text len");
    const int64_t money = msg.readInt64("money");
    const int itemsCount = msg.readUInt8("item count");
    const std::string text = msg.readString(textLen, "text message");
    MailMessage *mail = nullptr;

    if (mailWindow != nullptr &&
        openType == mailWindow->getOpenType())
    {
        mail = mailWindow->findMail(mailId);
    }

    if (mail == nullptr)
    {
        reportAlways("Mail message not found");
        for (int f = 0; f < itemsCount; f ++)
        {
            msg.readInt16("amount");
            msg.readInt16("item id");
            msg.readUInt8("identify");
            msg.readUInt8("damaged");
            msg.readUInt8("refine");
            for (int d = 0; d < maxCards; d ++)
                msg.readUInt16("card");
            msg.readInt32("unknown");
            msg.readUInt8("type");
            msg.readInt32("unknown");
            for (int d = 0; d < 5; d ++)
            {
                msg.readInt16("option index");
                msg.readInt16("option value");
                msg.readUInt8("option param");
            }
        }
        return;
    }

    mail->money = money;
    mail->text = text;
    mailWindow->showMessage(mail, itemsCount);

    Inventory *const inventory = mailViewWindow->getInventory();

    for (int f = 0; f < itemsCount; f ++)
    {
        const int amount = msg.readInt16("amount");
        const int itemId = msg.readInt16("item id");
        const uint8_t identify = msg.readUInt8("identify");
        const Damaged damaged = fromBool(msg.readUInt8("attribute"), Damaged);
        const uint8_t refine = msg.readUInt8("refine");
        int cards[maxCards];
        for (int d = 0; d < maxCards; d ++)
            cards[d] = msg.readUInt16("card");
        msg.readInt32("unknown");
        const ItemTypeT itemType = static_cast<ItemTypeT>(
            msg.readUInt8("item type"));
        msg.readInt32("unknown");
        ItemOptionsList *options = new ItemOptionsList(5);
        for (int d = 0; d < 5; d ++)
        {
            const uint16_t idx = msg.readInt16("option index");
            const uint16_t val = msg.readInt16("option value");
            msg.readUInt8("option param");
            options->add(idx, val);
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
        if (slot == -1)
        {
            delete options;
            continue;
        }
        inventory->setCards(slot, cards, maxCards);
        inventory->setOptions(slot, options);
        delete options;
    }
    mailViewWindow->updateItems();
}

void Mail2Recv::processMailDelete(Net::MessageIn &msg)
{
    msg.readUInt8("open type");
    const int64_t mailId = msg.readInt64("mail id");
    if (mailWindow == nullptr)
    {
        reportAlways("Mail window not created.");
        return;
    }
    mailWindow->removeMail(mailId);
}

void Mail2Recv::processRequestMoney(Net::MessageIn &msg)
{
    const int64_t mailId = msg.readInt64("mail id");
    msg.readUInt8("open type");
    const int res = msg.readUInt8("result");
    switch (res)
    {
        case 0:
            NotifyManager::notify(
                NotifyTypes::MAIL_GET_MONEY_OK);
            if (mailViewWindow != nullptr)
                mailViewWindow->removeMoney(mailId);
            break;
        case 1:
            NotifyManager::notify(
                NotifyTypes::MAIL_GET_MONEY_ERROR);
            break;
        case 2:
            NotifyManager::notify(
                NotifyTypes::MAIL_GET_MONEY_LIMIT_ERROR);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(res);
            NotifyManager::notify(
                NotifyTypes::MAIL_GET_MONEY_ERROR);
            break;
    }
}

void Mail2Recv::processRequestItems(Net::MessageIn &msg)
{
    const int64_t mailId = msg.readInt64("mail id");
    msg.readUInt8("open type");
    const int res = msg.readUInt8("result");
    switch (res)
    {
        case 0:
            NotifyManager::notify(
                NotifyTypes::MAIL_GET_ATTACH_OK);
            if (mailViewWindow != nullptr)
                mailViewWindow->removeItems(mailId);
            break;
        case 1:
            NotifyManager::notify(
                NotifyTypes::MAIL_GET_ATTACH_ERROR);
            break;
        case 2:
            NotifyManager::notify(
                NotifyTypes::MAIL_GET_ATTACH_FULL_ERROR);
            break;
        default:
            UNIMPLEMENTEDPACKETFIELD(res);
            NotifyManager::notify(
                NotifyTypes::MAIL_GET_ATTACH_ERROR);
            break;
    }
}

}  // namespace EAthena
