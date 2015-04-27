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

#include "gui/windows/mailedit.h"

#include "net/mailhandler.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouttype.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

MailEdit::MailEdit() :
    // TRANSLATORS: mail edit window name
    Window(_("Edit mail"), true, nullptr, "mailedit.xml"),
    ActionListener(),
    // TRANSLATORS: mail edit window button
    mSendButton(new Button(this, _("Send"), "send", this)),
    // TRANSLATORS: mail edit window button
    mCloseButton(new Button(this, _("Close"), "close", this)),
    // TRANSLATORS: mail edit window label
    mToLabel(new Label(this, _("To:"))),
    mSubjectLabel(new Label(this, _("Subject:"))),
    mMessageLabel(new Label(this, _("Message:"))),
    mToField(new TextField(this)),
    mSubjectField(new TextField(this)),
    mMessageField(new TextField(this))
{
    setWindowName("MailEdit");
    setCloseButton(true);
    setResizable(true);
    setCloseButton(true);
    setSaveVisible(false);
    setStickyButtonLock(true);
    setVisible(true);

    setDefaultSize(380, 150, ImageRect::CENTER);
    setMinWidth(200);
    setMinHeight(100);
    center();

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    mToField->setWidth(100);
    mSubjectField->setWidth(100);
    mMessageField->setWidth(100);

    placer(0, 0, mToLabel);
    placer(1, 0, mToField, 2);
    placer(0, 1, mSubjectLabel);
    placer(1, 1, mSubjectField, 2);
    placer(0, 2, mMessageLabel);
    placer(1, 2, mMessageField, 2);
    placer(0, 3, mSendButton);
    placer(2, 3, mCloseButton);

    loadWindowState();
    enableVisibleSound(true);
}

MailEdit::~MailEdit()
{
}

void MailEdit::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "close")
    {
        scheduleDelete();
    }
    else if (eventId == "send")
    {
        mailHandler->send(mToField->getText(),
            mSubjectField->getText(),
            mMessageField->getText());
    }
}
