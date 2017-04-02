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

#include "net/mailhandler.h"

#include "gui/mailmessage.h"

#include "gui/windows/maileditwindow.h"
#include "gui/windows/mailwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/icon.h"
#include "gui/widgets/label.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "resources/iteminfo.h"

#include "resources/image/image.h"

#include "resources/loaders/imageloader.h"

#include "resources/db/itemdb.h"

#include "debug.h"

MailViewWindow *mailViewWindow = nullptr;

MailViewWindow::MailViewWindow(const MailMessage *const message) :
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
    // TRANSLATORS: mail view window label
    mItemLabel(nullptr),
    mIcon(nullptr)
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

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    int n = 0;
    placer(0, n++, mTimeLabel);
    placer(0, n++, mFromLabel);
    placer(0, n++, mSubjectLabel);
    if (message->money)
    {
        // TRANSLATORS: mail view window label
        mMoneyLabel = new Label(this, strprintf("%s %d", _("Money:"),
            message->money));
        placer(0, n++, mMoneyLabel);
    }
    placer(0, n++, mMessageLabel);
    if (message->itemId)
    {
        const ItemInfo &item = ItemDB::get(message->itemId);
        // +++ need use message->cards and ItemColorManager for colors
        Image *const image = Loader::getImage(combineDye2(
            pathJoin(paths.getStringValue("itemIcons"),
            item.getDisplay().image),
            item.getDyeIconColorsString(ItemColor_one)));

        mIcon = new Icon(this, image);
        if (message->itemAmount != 1)
        {
            mItemLabel = new Label(this, std::string(
                // TRANSLATORS: mail view item label
                _("Item:")).append(
                " (").append(
                toString(message->itemAmount)).append(
                ") "));
        }
        else
        {
            mItemLabel = new Label(this,
                // TRANSLATORS: mail view item label
                std::string(_("Item:")).append(" "));
        }
        placer(0, n, mItemLabel);
        placer(1, n++, mIcon);
    }
    if (message->money || message->itemId)
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
    if (mIcon)
    {
        Image *const image = mIcon->getImage();
        if (image)
            image->decRef();
    }
    delete2(mMessage);
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
        if (mGetAttachButton)
            mailHandler->getAttach(mMessage->id);
    }
    else if (eventId == "next")
    {
        if (mMessage)
            mailWindow->viewNext(mMessage->id);
    }
    else if (eventId == "prev")
    {
        if (mMessage)
            mailWindow->viewPrev(mMessage->id);
    }
    else if (eventId == "reply")
    {
        if (!mMessage)
            return;
        if (mailEditWindow)
            mailEditWindow->scheduleDelete();
        CREATEWIDGETV0(mailEditWindow, MailEditWindow);
        mailEditWindow->setTo(mMessage->sender);
        mailEditWindow->setSubject("Re:" + mMessage->title);
        mailEditWindow->setMessage(">" + mMessage->text);
        scheduleDelete();
    }
}
