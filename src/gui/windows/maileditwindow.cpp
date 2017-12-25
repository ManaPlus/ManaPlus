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

#include "gui/windows/maileditwindow.h"

#include "settings.h"

#include "being/playerinfo.h"

#include "gui/windows/inventorywindow.h"
#include "gui/windows/itemamountwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/scrollarea.h"

#include "resources/item/item.h"

#include "net/mail2handler.h"
#include "net/mailhandler.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include <climits>

#include "debug.h"

MailEditWindow *mailEditWindow = nullptr;

MailEditWindow::MailEditWindow() :
    // TRANSLATORS: mail edit window name
    Window(_("Edit mail"), Modal_false, nullptr, "mailedit.xml"),
    ActionListener(),
    FocusListener(),
    // TRANSLATORS: mail edit window button
    mSendButton(new Button(this, _("Send"), "send", this)),
    // TRANSLATORS: mail edit window button
    mCloseButton(new Button(this, _("Close"), "close", this)),
    // TRANSLATORS: mail edit window button
    mAddButton(new Button(this, _("Add"), "add", this)),
    // TRANSLATORS: mail edit window label
    mToLabel(new Label(this, _("To:"))),
    // TRANSLATORS: mail edit window label
    mSubjectLabel(new Label(this, _("Subject:"))),
    // TRANSLATORS: mail edit window label
    mMoneyLabel(new Label(this, _("Money:"))),
    // TRANSLATORS: mail edit window label
    mItemLabel(new Label(this, _("Item:"))),
    // TRANSLATORS: mail edit window label
    mMessageLabel(new Label(this, _("Message:"))),
    mToField(new TextField(this)),
    mSubjectField(new TextField(this)),
    mMoneyField(new IntTextField(this, 0, 0,
        settings.enableNewMailSystem ? INT_MAX : 10000000)),
    mMessageField(new TextField(this)),
    mInventory(new Inventory(InventoryType::MailEdit,
        settings.enableNewMailSystem ? -1 : 1)),
    mItemContainer(new ItemContainer(this, mInventory)),
    mItemScrollArea(new ScrollArea(this, mItemContainer,
        fromBool(getOptionBool("showitemsbackground"), Opaque),
        "mailedit_listbackground.xml")),
    mUseMail2(settings.enableNewMailSystem)
{
    setWindowName("MailEdit");
    setCloseButton(true);
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(false);
    setStickyButtonLock(true);
    setVisible(Visible_true);

    setDefaultSize(380, 200, ImagePosition::CENTER);
    setMinWidth(200);
    setMinHeight(200);
    center();

    ContainerPlacer placer(nullptr, nullptr);
    placer = getPlacer(0, 0);

    mToField->setWidth(100);
    mSubjectField->setWidth(100);
    mMessageField->setWidth(100);
    mItemScrollArea->setHeight(100);
    mItemScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);
    mToField->addFocusListener(this);

    placer(0, 0, mToLabel, 1, 1);
    placer(1, 0, mToField, 3, 1);
    placer(0, 1, mSubjectLabel, 1, 1);
    placer(1, 1, mSubjectField, 3, 1);
    placer(0, 2, mMoneyLabel, 1, 1);
    placer(1, 2, mMoneyField, 3, 1);
    placer(0, 3, mItemLabel, 1, 1);
    placer(1, 3, mItemScrollArea, 2, 2);
    placer(3, 4, mAddButton, 1, 1);

    placer(0, 5, mMessageLabel, 1, 1);
    placer(1, 5, mMessageField, 3, 1);
    placer(0, 6, mSendButton, 1, 1);
    placer(3, 6, mCloseButton, 1, 1);

    loadWindowState();
    if (mUseMail2)
        mSendButton->setEnabled(false);
    enableVisibleSound(true);
}

MailEditWindow::~MailEditWindow()
{
    mailEditWindow = nullptr;
    delete2(mInventory);
}

void MailEditWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "close")
    {
        close();
    }
    else if (eventId == "send")
    {
        sendMail();
    }
    else if (eventId == "add")
    {
        Item *const item = inventoryWindow->getSelectedItem();

        if (item == nullptr)
            return;

        ItemAmountWindow::showWindow(ItemAmountWindowUsage::MailAdd,
            this,
            item,
            0,
            0);
    }
}

void MailEditWindow::addItem(const Item *const item,
                             const int amount)
{
    if (item == nullptr)
        return;
    mInventory->addItem(item->getId(),
        item->getType(),
        amount,
        item->getRefine(),
        item->getColor(),
        item->getIdentified(),
        item->getDamaged(),
        item->getFavorite(),
        Equipm_false,
        Equipped_false);
}

void MailEditWindow::setSubject(const std::string &str)
{
    mSubjectField->setText(str);
}

void MailEditWindow::setTo(const std::string &str)
{
    mToField->setText(str);
    mSendButton->setEnabled(true);
}

void MailEditWindow::setMessage(const std::string &str)
{
    mMessageField->setText(str);
}

void MailEditWindow::close()
{
    if (mUseMail2)
        mail2Handler->cancelWriteMail();
    mailEditWindow = nullptr;
    scheduleDelete();
}

Inventory *MailEditWindow::getInventory() const
{
    return mInventory;
}

void MailEditWindow::sendMail()
{
    const int money = mMoneyField->getValue();
    std::string subject = mSubjectField->getText();
    if (subject.empty())
    {
        // TRANSLATORS: empty mail message subject
        subject.append(_("empty subject"));
    }
    if (mUseMail2)
    {
        mail2Handler->sendMail(mToField->getText(),
            subject,
            mMessageField->getText(),
            money);
    }
    else
    {
        if (money != 0)
            mailHandler->setAttachMoney(money);
        const Item *const tempItem = mInventory->getItem(0);
        if (tempItem != nullptr)
        {
            const Inventory *const inv = PlayerInfo::getInventory();
            if (inv != nullptr)
            {
                const Item *const item = inv->findItem(
                    tempItem->getId(), ItemColor_one);
                if (item != nullptr)
                {
                    mailHandler->setAttach(item->getInvIndex(),
                        tempItem->getQuantity());
                }
            }
        }

        mailHandler->send(mToField->getText(),
            subject,
            mMessageField->getText());
    }
}

void MailEditWindow::updateItems()
{
    mItemContainer->updateMatrix();
}

void MailEditWindow::focusLost(const Event &event)
{
    if (!mUseMail2)
        return;

    if (event.getSource() == mToField)
    {
        const std::string to = mToField->getText();
        if (to != mail2Handler->getCheckedName())
        {
            mail2Handler->queueCheckName(MailQueueType::ValidateTo,
                to,
                std::string(),
                std::string(),
                0);
            mSendButton->setEnabled(false);
        }
        else
        {
            mSendButton->setEnabled(true);
        }
    }
}

void MailEditWindow::validatedTo()
{
    mSendButton->setEnabled(true);
}
