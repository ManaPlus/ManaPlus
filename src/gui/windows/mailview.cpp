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

#include "gui/windows/mailview.h"

#include "configuration.h"

#include "net/mailhandler.h"

#include "gui/mailmessage.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/icon.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/layouttype.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "resources/image.h"
#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "resources/db/itemdb.h"

#include "debug.h"

MailView::MailView(const MailMessage *const message) :
    // TRANSLATORS: mail view window name
    Window(_("View mail"), true, nullptr, "mailview.xml"),
    ActionListener(),
    mMessage(message),
    // TRANSLATORS: mail view window button
    mGetAttachButton(new Button(this, _("Get attach"), "attach", this)),
    // TRANSLATORS: mail view window button
    mCloseButton(new Button(this, _("Close"), "close", this)),
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
    mItemLabel(new Label(this, std::string(_("Item:")).append(" "))),
    mMessageField(new TextField(this)),
    mIcon(new Icon(this, nullptr))
{
    setWindowName("MailView");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(false);
    setStickyButtonLock(true);
    setVisible(true);

    setDefaultSize(380, 150, ImageRect::CENTER);
    setMinWidth(200);
    setMinHeight(100);
    center();

    ContainerPlacer placer;
    placer = getPlacer(0, 0);

    mMessageField->setWidth(100);

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
        ResourceManager *const resman = ResourceManager::getInstance();
        const ItemInfo &item = ItemDB::get(message->itemId);
        Image *const image = resman->getImage(combineDye2(
            paths.getStringValue("itemIcons").append(
            item.getDisplay().image), item.getDyeColorsString(1)));

        mIcon->setImage(image);
        placer(0, n, mItemLabel);
        placer(1, n++, mIcon);
    }

    placer(0, n, mGetAttachButton);
    placer(2, n, mCloseButton);

    loadWindowState();
    enableVisibleSound(true);
}

MailView::~MailView()
{
}

void MailView::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "close")
    {
        scheduleDelete();
    }
}
