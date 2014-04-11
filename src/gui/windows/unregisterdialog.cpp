/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "gui/windows/unregisterdialog.h"

#include "client.h"

#include "gui/windows/okdialog.h"
#include "gui/windows/registerdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/passwordfield.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include <string>
#include <sstream>

#include "debug.h"

UnRegisterDialog::UnRegisterDialog(LoginData *const data) :
    // TRANSLATORS: unregister dialog name
    Window(_("Unregister"), true, nullptr, "unregister.xml"),
    ActionListener(),
    mLoginData(data),
    mPasswordField(new PasswordField(this, mLoginData->password)),
    // TRANSLATORS: unregister dialog. button.
    mUnRegisterButton(new Button(this, _("Unregister"), "unregister", this)),
    // TRANSLATORS: unregister dialog. button.
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    mWrongDataNoticeListener(new WrongDataNoticeListener)
{
}

void UnRegisterDialog::postInit()
{
    // TRANSLATORS: unregister dialog. label.
    Label *const userLabel = new Label(this, strprintf(_("Name: %s"),
        mLoginData->username.c_str()));
    // TRANSLATORS: unregister dialog. label.
    Label *const passwordLabel = new Label(this, _("Password:"));

    const int width = 210;
    const int height = 80;
    setContentSize(width, height);

    userLabel->setPosition(5, 5);
    userLabel->setWidth(width - 5);
    mPasswordField->setPosition(
            68, userLabel->getY() + userLabel->getHeight() + 7);
    mPasswordField->setWidth(130);

    passwordLabel->setPosition(5, mPasswordField->getY() + 1);

    mCancelButton->setPosition(
            width - 5 - mCancelButton->getWidth(),
            height - 5 - mCancelButton->getHeight());
    mUnRegisterButton->setPosition(
            mCancelButton->getX() - 5 - mUnRegisterButton->getWidth(),
            mCancelButton->getY());

    add(userLabel);
    add(passwordLabel);
    add(mPasswordField);
    add(mUnRegisterButton);
    add(mCancelButton);

    center();
    setVisible(true);
    mPasswordField->requestFocus();
    mPasswordField->setActionEventId("cancel");
}

UnRegisterDialog::~UnRegisterDialog()
{
    delete2(mWrongDataNoticeListener);
}

void UnRegisterDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "cancel")
    {
        client->setState(STATE_CHAR_SELECT);
    }
    else if (eventId == "unregister")
    {
        const std::string username = mLoginData->username.c_str();
        const std::string &password = mPasswordField->getText();
        logger->log("UnregisterDialog::unregistered, Username is %s",
                     username.c_str());

        std::stringstream errorMsg;
        bool error = false;

        const unsigned int min = Net::getLoginHandler()
            ->getMinPasswordLength();
        const unsigned int max = Net::getLoginHandler()
            ->getMaxPasswordLength();

        if (password.length() < min)
        {
            // TRANSLATORS: unregister dialog. error message.
            errorMsg << strprintf(_("The password needs to be at least %u "
                "characters long."), min);
            error = true;
        }
        else if (password.length() > max)
        {
            // TRANSLATORS: unregister dialog. error message.
            errorMsg << strprintf(_("The password needs to be less than "
                "%u characters long."), max);
            error = true;
        }

        if (error)
        {
            mWrongDataNoticeListener->setTarget(this->mPasswordField);

            // TRANSLATORS: unregister dialog. error message.
            OkDialog *const dlg = new OkDialog(_("Error"),
                errorMsg.str(), DIALOG_ERROR);
            dlg->addActionListener(mWrongDataNoticeListener);
        }
        else
        {
            // No errors detected, unregister the new user.
            mUnRegisterButton->setEnabled(false);
            mLoginData->password = password;
            client->setState(STATE_UNREGISTER_ATTEMPT);
        }
    }
}
