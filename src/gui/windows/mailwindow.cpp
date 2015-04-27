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

#include "gui/windows/mailwindow.h"

#include "net/mailhandler.h"

#include "gui/models/extendednamesmodel.h"

#include "gui/mailmessage.h"

#include "gui/windows/mailedit.h"
#include "gui/windows/mailview.h"
#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/extendedlistbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouttype.h"

#include "utils/delete2.h"
#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

MailWindow *mailWindow = nullptr;

MailWindow::MailWindow() :
    // TRANSLATORS: mail window name
    Window(_("Mail"), false, nullptr, "mail.xml"),
    ActionListener(),
    mMessages(),
    mMailModel(new ExtendedNamesModel),
    mListBox(new ExtendedListBox(this, mMailModel, "extendedlistbox.xml")),
    // TRANSLATORS: mail window button
    mRefreshButton(new Button(this, _("Refresh"), "refresh", this)),
    // TRANSLATORS: mail window button
    mNewButton(new Button(this, _("New"), "new", this)),
    // TRANSLATORS: mail window button
    mDeleteButton(new Button(this, _("Delete"), "delete", this)),
    // TRANSLATORS: mail window button
    mOpenButton(new Button(this, _("Open"), "open", this))
{
    mListBox->postInit();
    setWindowName("Mail");
    setCloseButton(true);
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(true);
    setStickyButtonLock(true);

    if (setupWindow)
        setupWindow->registerWindowForReset(this);

    setDefaultSize(387, 307, ImageRect::CENTER);
    setMinWidth(310);
    setMinHeight(179);
    center();

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    placer(0, 0, mListBox, 4, 4).setPadding(3);
    placer(4, 0, mRefreshButton);
    placer(4, 1, mOpenButton);
    placer(4, 2, mNewButton);
    placer(4, 3, mDeleteButton);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    loadWindowState();
    enableVisibleSound(true);
}

MailWindow::~MailWindow()
{
    delete_all(mMessages);
    delete2(mMailModel);
}

void MailWindow::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "refresh")
    {
        mailHandler->refresh();
    }
    else if (eventId == "new")
    {
        new MailEdit();
    }
    else if (eventId == "open")
    {
        const int sel = mListBox->getSelected();
        if (sel < 0)
            return;
        const MailMessage *const mail = mMessages[sel];
        mailHandler->readMessage(mail->id);
    }
}

void MailWindow::clear()
{
    delete_all(mMessages);
    mMailModel->clear();
    mListBox->setSelected(-1);
}

void MailWindow::addMail(MailMessage *const message)
{
    mMessages.push_back(message);
    mMailModel->add(strprintf("%s %s",
        message->unread ? " " : "U",
        message->title.c_str()));
}

void MailWindow::showMessage(const MailMessage *const mail)
{
    new MailView(mail);
}
