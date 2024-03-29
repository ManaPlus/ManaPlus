/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#include "gui/windows/changeemaildialog.h"

#include "client.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/label.h"
#include "gui/widgets/textfield.h"

#include "listeners/wrongdatanoticelistener.h"

#include "net/logindata.h"
#include "net/loginhandler.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include <sstream>

#include "debug.h"

ChangeEmailDialog::ChangeEmailDialog(LoginData &data) :
    // TRANSLATORS: change email dialog header
    Window(_("Change Email Address"), Modal_true, nullptr, "changeemail.xml"),
    ActionListener(),
    mFirstEmailField(new TextField(this, std::string(), LoseFocusOnTab_true,
        nullptr, std::string(), false)),
    mSecondEmailField(new TextField(this, std::string(), LoseFocusOnTab_true,
        nullptr, std::string(), false)),
    // TRANSLATORS: button in change email dialog
    mChangeEmailButton(new Button(this, _("Change Email Address"),
        "change_email", BUTTON_SKIN, this)),
    // TRANSLATORS: button in change email dialog
    mCancelButton(new Button(this, _("Cancel"), "cancel", BUTTON_SKIN, this)),
    mWrongDataNoticeListener(new WrongDataNoticeListener),
    mLoginData(&data)
{
    // TRANSLATORS: label in change email dialog
    Label *const accountLabel = new Label(this, strprintf(_("Account: %s"),
        mLoginData->username.c_str()));
    Label *const newEmailLabel = new Label(this,
        // TRANSLATORS: label in change email dialog
        _("Type new email address twice:"));

    const int width = 200;
    const int height = 130;
    setContentSize(width, height);

    accountLabel->setPosition(5, 5);
    accountLabel->setWidth(130);

    newEmailLabel->setPosition(
            5, accountLabel->getY() + accountLabel->getHeight() + 7);
    newEmailLabel->setWidth(width - 5);

    mFirstEmailField->setPosition(
            5, newEmailLabel->getY() + newEmailLabel->getHeight() + 7);
    mFirstEmailField->setWidth(130);

    mSecondEmailField->setPosition(
            5, mFirstEmailField->getY() + mFirstEmailField->getHeight() + 7);
    mSecondEmailField->setWidth(130);

    mCancelButton->setPosition(
            width - 5 - mCancelButton->getWidth(),
            height - 5 - mCancelButton->getHeight());
    mChangeEmailButton->setPosition(
            mCancelButton->getX() - 5 - mChangeEmailButton->getWidth(),
            mCancelButton->getY());

    add(accountLabel);
    add(newEmailLabel);
    add(mFirstEmailField);
    add(mSecondEmailField);
    add(mChangeEmailButton);
    add(mCancelButton);

    center();
    mFirstEmailField->requestFocus();

    mFirstEmailField->setActionEventId("change_email");
    mSecondEmailField->setActionEventId("change_email");
}

ChangeEmailDialog::~ChangeEmailDialog()
{
    delete2(mWrongDataNoticeListener)
}

void ChangeEmailDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "cancel")
    {
        client->setState(State::CHAR_SELECT);
    }
    else if (eventId == "change_email")
    {
        const std::string username = mLoginData->username;
        const std::string &newFirstEmail = mFirstEmailField->getText();
        const std::string &newSecondEmail = mSecondEmailField->getText();
        logger->log("ChangeEmailDialog::Email change, Username is %s",
                     username.c_str());

        std::stringstream errorMsg;
        int error = 0;

        const unsigned int min = loginHandler->getMinPasswordLength();
        const unsigned int max = loginHandler->getMaxPasswordLength();

        if (newFirstEmail.length() < min)
        {
            // First email address too short
            // TRANSLATORS: change email error
            errorMsg << strprintf(_("The new email address needs to be at "
                "least %u characters long."), min);
            error = 1;
        }
        else if (newFirstEmail.length() > max)
        {
            // First email address too long
            // TRANSLATORS: change email error
            errorMsg << strprintf(_("The new email address needs to be "
                "less than %u characters long."), max);
            error = 1;
        }
        else if (newFirstEmail != newSecondEmail)
        {
            // Second Pass mismatch
            // TRANSLATORS: change email error
            errorMsg << _("The email address entries mismatch.");
            error = 2;
        }

        if (error > 0)
        {
            if (error == 1)
                mWrongDataNoticeListener->setTarget(this->mFirstEmailField);
            else  // if (error == 2)
                mWrongDataNoticeListener->setTarget(this->mSecondEmailField);

            OkDialog *const dlg = CREATEWIDGETR(OkDialog,
                // TRANSLATORS: change email error header
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
            mChangeEmailButton->setEnabled(false);
            // Set the new email address
            mLoginData->email = newFirstEmail;
            client->setState(State::CHANGEEMAIL_ATTEMPT);
        }
    }
}
