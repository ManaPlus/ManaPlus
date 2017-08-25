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

#include "configuration.h"
#include "settings.h"

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

#include "resources/iteminfo.h"

#include "resources/image/image.h"

#include "resources/inventory/inventory.h"

#include "resources/loaders/imageloader.h"

#include "resources/db/itemdb.h"

#include "debug.h"

MailViewWindow *mailViewWindow = nullptr;

MailViewWindow::MailViewWindow(const MailMessage *const message,
                               const int itemsCount) :
    // TRANSLATORS: mail view window name
    Window(_("View mail"), Modal_false, nullptr, "mailview.xml"),
    ActionListener(),
    mMessage(message),
    // TRANSLATORS: mail view window button
    mGetAttachButton(nullptr),
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
    mInventory(new Inventory(InventoryType::Mail, itemsCount)),
    mItemContainer(new ItemContainer(this, mInventory)),
    mItemScrollArea(new ScrollArea(this, mItemContainer,
        fromBool(getOptionBool("showitemsbackground"), Opaque),
        "mailedit_listbackground.xml")),
    mUseMail2(settings.enableNewMailSystem)
{
    setWindowName("MailView");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(false);
    setStickyButtonLock(true);
    setVisible(Visible_true);

    setDefaultSize(380, 230, ImagePosition::CENTER);
    setMinWidth(200);
    setMinHeight(100);
    center();
    mItemScrollArea->setHeight(100);
    mItemScrollArea->setWidth(100);

    mItemScrollArea->setHorizontalScrollPolicy(ScrollArea::SHOW_NEVER);

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    int n = 0;
    placer(0, n++, mTimeLabel);
    placer(0, n++, mFromLabel);
    placer(0, n++, mSubjectLabel);
    if (message->money != 0)
    {
        // TRANSLATORS: mail view window label
        mMoneyLabel = new Label(this, strprintf("%s %d", _("Money:"),
            message->money));
        placer(0, n++, mMoneyLabel);
    }
    placer(0, n++, mMessageLabel);
    placer(0, n++, mItemScrollArea);

    logger->log("sizes: %d, %d",
        mItemContainer->getWidth(),
        mItemContainer->getHeight());

    if (message->money != 0 ||
        message->itemId != 0)
    {
        mGetAttachButton = new Button(this,
            // TRANSLATORS: mail view attach button
            _("Get attach"),
             "attach",
            this);
        placer(0, n++, mGetAttachButton);
    }
    ContainerPlacer placer2;
    placer2 = getPlacer(0, n);

    placer2(0, 0, mPrevButton);
    placer2(1, 0, mNextButton);
    placer2(3, 0, mReplyButton);
    placer2(4, 0, mCloseButton);

    loadWindowState();
    enableVisibleSound(true);
}

MailViewWindow::~MailViewWindow()
{
    if (mUseMail2)
        mMessage = nullptr;
    else
        delete2(mMessage);
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
        if (mGetAttachButton != nullptr)
            mailHandler->getAttach(mMessage->id);
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

void MailViewWindow::updateItems()
{
    mItemContainer->updateMatrix();
}
