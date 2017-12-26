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

#include "gui/windows/mailviewwindow.h"

#include "settings.h"

#include "net/mail2handler.h"
#include "net/mailhandler.h"

#include "gui/mailmessage.h"

#include "gui/windows/maileditwindow.h"
#include "gui/windows/mailwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/itemcontainer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/scrollarea.h"

#include "utils/delete2.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/inventory/inventory.h"
#include "debug.h"

MailViewWindow *mailViewWindow = nullptr;

MailViewWindow::MailViewWindow(MailMessage *const message,
                               const int itemsCount) :
    // TRANSLATORS: mail view window name
    Window(_("View mail"), Modal_false, nullptr, "mailview.xml"),
    ActionListener(),
    mMessage(message),
    mGetAttachButton(new Button(this,
        // TRANSLATORS: mail view attach / items button
        settings.enableNewMailSystem ? _("Get items") : _("Get attach"),
        "attach", this)),
    mGetMoneyButton(nullptr),
    // TRANSLATORS: mail view window button
    mCloseButton(new Button(this, _("Close"), "close", this)),
    mPrevButton(new Button(this, "<", "prev", this)),
    mNextButton(new Button(this, ">", "next", this)),
    // TRANSLATORS: mail view window button
    mReplyButton(new Button(this, _("Reply"), "reply", this)),
    // TRANSLATORS: mail view window label
    mTimeLabel(new Label(this, strprintf("%s %s", _("Time:"),
        message->strTime.c_str()))),
    mMoneyLabel(nullptr),
    // TRANSLATORS: mail view window label
    mFromLabel(new Label(this, strprintf("%s %s", _("From:"),
        message->sender.c_str()))),
    // TRANSLATORS: mail view window label
    mSubjectLabel(new Label(this, strprintf("%s %s", _("Subject:"),
        message->title.c_str()))),
    // TRANSLATORS: mail view window label
    mMessageLabel(new Label(this, strprintf("%s %s", _("Message:"),
        message->text.c_str()))),
    mInventory(new Inventory(InventoryType::MailView, itemsCount)),
    mItemContainer(new ItemContainer(this, mInventory, 100000,
        ShowEmptyRows_false, ForceQuantity_false)),
    mItemScrollArea(new ScrollArea(this, mItemContainer,
        fromBool(getOptionBool("showitemsbackground"), Opaque),
        "mailview_listbackground.xml")),
    mUseMail2(settings.enableNewMailSystem)
{
    setWindowName("MailView");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(false);
    setStickyButtonLock(true);
    setVisible(Visible_true);

    setDefaultSize(380, 370, ImagePosition::CENTER);
    setMinWidth(200);
    setMinHeight(100);
    center();
    mItemScrollArea->setHeight(100);

    mItemScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    ContainerPlacer placer(nullptr, nullptr);
    placer = getPlacer(0, 0);

    int n = 0;
    placer(0, n++, mTimeLabel, 1, 1);
    placer(0, n++, mFromLabel, 1, 1);
    placer(0, n++, mSubjectLabel, 1, 1);
    if (message->money != 0)
    {
        mMoneyLabel = new Label(this, strprintf("%s %u",
            // TRANSLATORS: mail view window label
            _("Money:"),
            CAST_U32(message->money)));
        placer(0, n++, mMoneyLabel, 1, 1);
    }
    placer(0, n++, mMessageLabel, 1, 1);
    placer(0, n++, mItemScrollArea, 1, 1);

    if (mUseMail2 && message->money != 0)
    {
        mGetMoneyButton = new Button(this,
            // TRANSLATORS: mail view attached money button
            _("Get money"),
            "money", this);
        placer(0, n++, mGetMoneyButton, 1, 1);
    }
    placer(0, n++, mGetAttachButton, 1, 1);
    updateAttachButton();

    ContainerPlacer placer2(nullptr, nullptr);
    placer2 = getPlacer(0, n);

    placer2(0, 0, mPrevButton, 1, 1);
    placer2(1, 0, mNextButton, 1, 1);
    placer2(3, 0, mReplyButton, 1, 1);
    placer2(4, 0, mCloseButton, 1, 1);

    loadWindowState();
    enableVisibleSound(true);
}

MailViewWindow::~MailViewWindow()
{
    if (mUseMail2)
    {
        mMessage = nullptr;
    }
    else
    {
        delete2(mMessage);
        delete2(mGetMoneyButton);
    }
    delete2(mInventory);
    mailViewWindow = nullptr;
}

void MailViewWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "close")
    {
        scheduleDelete();
    }
    else if (eventId == "attach")
    {
        if (mUseMail2)
        {
            mail2Handler->requestItems(mailWindow->getOpenType(),
                mMessage->id);
        }
        else
        {
            mailHandler->getAttach(CAST_S32(mMessage->id));
        }
    }
    else if (eventId == "money")
    {
        if (mUseMail2)
        {
            mail2Handler->requestMoney(mailWindow->getOpenType(),
                mMessage->id);
        }
    }
    else if (eventId == "next")
    {
        if (mMessage != nullptr)
            mailWindow->viewNext(mMessage->id);
    }
    else if (eventId == "prev")
    {
        if (mMessage != nullptr)
            mailWindow->viewPrev(mMessage->id);
    }
    else if (eventId == "reply")
    {
        if (mMessage == nullptr)
            return;
        if (mailEditWindow != nullptr)
            mailEditWindow->scheduleDelete();
        CREATEWIDGETV0(mailEditWindow, MailEditWindow);
        mailEditWindow->setTo(mMessage->sender);
        mailEditWindow->setSubject("Re:" + mMessage->title);
        mailEditWindow->setMessage(">" + mMessage->text);
        scheduleDelete();
    }
}

Inventory *MailViewWindow::getInventory() const
{
    return mInventory;
}

void MailViewWindow::updateAttachButton()
{
    if ((mMessage->money != 0 && !mUseMail2) ||
        mInventory->getLastUsedSlot() != -1)
    {
        mGetAttachButton->setVisible(Visible_true);
    }
    else
    {
        mGetAttachButton->setVisible(Visible_false);
    }
}

void MailViewWindow::updateItems()
{
    mItemContainer->updateMatrix();
    updateAttachButton();
}

void MailViewWindow::removeItems(const int64_t mailId)
{
    if (mailId != mMessage->id)
        return;
    mInventory->clear();
    mMessage->type = static_cast<MailMessageType::Type>(
        CAST_S32(mMessage->type) | CAST_S32(MailMessageType::Item));
    mMessage->type = static_cast<MailMessageType::Type>(
        CAST_S32(mMessage->type) ^ CAST_S32(MailMessageType::Item));
    updateAttachButton();
    if (mailWindow != nullptr)
        mailWindow->refreshMailNames();
}

void MailViewWindow::removeMoney(const int64_t mailId)
{
    if (mailId != mMessage->id)
        return;
    mMessage->type = static_cast<MailMessageType::Type>(
        CAST_S32(mMessage->type) | CAST_S32(MailMessageType::Money));
    mMessage->type = static_cast<MailMessageType::Type>(
        CAST_S32(mMessage->type) ^ CAST_S32(MailMessageType::Money));

    mMessage->money = 0;

    if (mMoneyLabel == nullptr)
        return;

    if (mGetMoneyButton != nullptr)
        mGetMoneyButton->setVisible(Visible_false);

    mMoneyLabel->setCaption(strprintf("%s %d",
        // TRANSLATORS: mail view window label
        _("Money:"),
        0));
    if (mailWindow != nullptr)
        mailWindow->refreshMailNames();
}
