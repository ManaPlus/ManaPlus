/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "gui/windows/changepassworddialog.h"

#include "client.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/passwordfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layoutcell.h"

#include "listeners/wrongdatanoticelistener.h"

#include "net/logindata.h"
#include "net/loginhandler.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include <sstream>

#include "debug.h"

ChangePasswordDialog::ChangePasswordDialog(LoginData &data) :
    // TRANSLATORS: change password window name
    Window(_("Change Password"), Modal_true, nullptr, "changepassword.xml"),
    ActionListener(),
    mOldPassField(new PasswordField(this, std::string())),
    mFirstPassField(new PasswordField(this, std::string())),
    mSecondPassField(new PasswordField(this, std::string())),
    // TRANSLATORS: change password dialog button
    mChangePassButton(new Button(this, _("Change Password"),
        "change_password", this)),
    // TRANSLATORS: change password dialog button
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    mWrongDataNoticeListener(new WrongDataNoticeListener),
    mLoginData(&data)
{
    Label *const accountLabel = new Label(this,
        // TRANSLATORS: change password dialog label
        strprintf(_("Account: %s"), mLoginData->username.c_str()));

    place(0, 0, accountLabel, 3, 1);
    // TRANSLATORS: change password dialog label
    place(0, 1, new Label(this, _("Password:")), 3, 1);
    place(0, 2, mOldPassField, 3, 1).setPadding(1);
    // TRANSLATORS: change password dialog label
    place(0, 3, new Label(this, _("Type new password twice:")), 3, 1);
    place(0, 4, mFirstPassField, 3, 1).setPadding(1);
    place(0, 5, mSecondPassField, 3, 1).setPadding(1);
    place(1, 6, mCancelButton, 1, 1);
    place(2, 6, mChangePassButton, 1, 1);
    reflowLayout(200, 0);

    center();
    mOldPassField->requestFocus();

    mOldPassField->setActionEventId("change_password");
    mFirstPassField->setActionEventId("change_password");
    mSecondPassField->setActionEventId("change_password");
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete2(mWrongDataNoticeListener);
}

void ChangePasswordDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "cancel")
    {
        client->setState(State::CHAR_SELECT);
    }
    else if (eventId == "change_password")
    {
        const std::string username = mLoginData->username;
        const std::string &oldPassword = mOldPassField->getText();
        const std::string &newFirstPass = mFirstPassField->getText();
        const std::string &newSecondPass = mSecondPassField->getText();
        logger->log("ChangePasswordDialog::Password change, Username is %s",
                     username.c_str());

        std::stringstream errorMsg;
        int error = 0;

        const unsigned int min = loginHandler->getMinPasswordLength();
        const unsigned int max = loginHandler->getMaxPasswordLength();

        // Check old Password
        if (oldPassword.empty())
        {
            // No old password
            // TRANSLATORS: change password error
            errorMsg << _("Enter the old password first.");
            error = 1;
        }
        else if (newFirstPass.length() < min)
        {
            // First password too short
            // TRANSLATORS: change password error
            errorMsg << strprintf(_("The new password needs to be at least"
                " %u characters long."), min);
            error = 2;
        }
        else if (newFirstPass.length() > max)
        {
            // First password too long
            // TRANSLATORS: change password error
            errorMsg << strprintf(_("The new password needs to be less "
                "than %u characters long."), max);
            error = 2;
        }
        else if (newFirstPass != newSecondPass)
        {
            // Second Pass mismatch
            // TRANSLATORS: change password error
            errorMsg << _("The new password entries mismatch.");
            error = 3;
        }

        if (error > 0)
        {
            if (error == 1)
                mWrongDataNoticeListener->setTarget(this->mOldPassField);
            else if (error == 2)
                mWrongDataNoticeListener->setTarget(this->mFirstPassField);
            else  // if (error == 3)
                mWrongDataNoticeListener->setTarget(this->mSecondPassField);

            OkDialog *const dlg = CREATEWIDGETR(OkDialog,
                // TRANSLATORS: change password error header
                _("Error"),
                errorMsg.str(),
                // TRANSLATORS: ok dialog button
                _("OK"),
                DialogType::ERROR,
                Modal_true,
                ShowCenter_true,
                nullptr,
                260);
            dlg->addActionListener(mWrongDataNoticeListener);
        }
        else
        {
            // No errors detected, change account password.
            mChangePassButton->setEnabled(false);
            // Set the new password
            mLoginData->password = oldPassword;
            mLoginData->newPassword = newFirstPass;
            client->setState(State::CHANGEPASSWORD_ATTEMPT);
        }
    }
}
