/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/registerdialog.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"

#include "gui/logindialog.h"
#include "gui/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/passwordfield.h"
#include "gui/widgets/radiobutton.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/gettext.h"

#include "debug.h"

WrongDataNoticeListener::WrongDataNoticeListener():
    ActionListener(),
    mTarget(nullptr)
{
}

void WrongDataNoticeListener::setTarget(gcn::TextField *const textField)
{
    mTarget = textField;
}

void WrongDataNoticeListener::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok" && mTarget)
        mTarget->requestFocus();
}

RegisterDialog::RegisterDialog(LoginData *const data):
    Window(_("Register"), false, nullptr, "register.xml"),
    ActionListener(),
    KeyListener(),
    mLoginData(data),
    mUserField(new TextField(mLoginData->username)),
    mPasswordField(new PasswordField(mLoginData->password)),
    mConfirmField(new PasswordField),
    mEmailField(nullptr),
    mRegisterButton(new Button(_("Register"), "register", this)),
    mCancelButton(new Button(_("Cancel"), "cancel", this)),
    mMaleButton(nullptr),
    mFemaleButton(nullptr),
    mOtherButton(nullptr),
    mWrongDataNoticeListener(new WrongDataNoticeListener)
{
    const int optionalActions = Net::getLoginHandler()->
        supportedOptionalActions();

    Label *const userLabel = new Label(_("Name:"));
    Label *const passwordLabel = new Label(_("Password:"));
    Label *const confirmLabel = new Label(_("Confirm:"));

    ContainerPlacer placer;
    placer = getPlacer(0, 0);
    placer(0, 0, userLabel);
    placer(0, 1, passwordLabel);
    placer(0, 2, confirmLabel);

    placer(1, 0, mUserField, 3).setPadding(2);
    placer(1, 1, mPasswordField, 3).setPadding(2);
    placer(1, 2, mConfirmField, 3).setPadding(2);

    int row = 3;

    if (optionalActions & Net::LoginHandler::SetGenderOnRegister)
    {
        mMaleButton = new RadioButton(_("Male"), "sex", true);
        mFemaleButton = new RadioButton(_("Female"), "sex", false);
        if (serverVersion >= 5)
        {
            mOtherButton = new RadioButton(_("Other"), "sex", false);
            placer(0, row, mMaleButton);
            placer(1, row, mFemaleButton);
            placer(2, row, mOtherButton);
        }
        else
        {
            placer(1, row, mMaleButton);
            placer(2, row, mFemaleButton);
        }

        row++;
    }

    if (optionalActions & Net::LoginHandler::SetEmailOnRegister)
    {
        Label *const emailLabel = new Label(_("Email:"));
        mEmailField = new TextField;
        placer(0, row, emailLabel);
        placer(1, row, mEmailField, 3).setPadding(2);

        row++;
    }

    placer = getPlacer(0, 2);
    placer(1, 0, mRegisterButton);
    placer(2, 0, mCancelButton);
    reflowLayout(250, 0);

    mUserField->addKeyListener(this);
    mPasswordField->addKeyListener(this);
    mConfirmField->addKeyListener(this);

    /* TODO:
     * This is a quick and dirty way to respond to the ENTER key, regardless of
     * which text field is selected. There may be a better way now with the new
     * input system of Guichan 0.6.0. See also the login dialog.
     */
    mUserField->setActionEventId("register");
    mPasswordField->setActionEventId("register");
    mConfirmField->setActionEventId("register");

    mUserField->addActionListener(this);
    mPasswordField->addActionListener(this);
    mConfirmField->addActionListener(this);

    center();
    setVisible(true);
    mUserField->requestFocus();
    mUserField->setCaretPosition(static_cast<unsigned>(
                                 mUserField->getText().length()));

    mRegisterButton->setEnabled(canSubmit());
}

RegisterDialog::~RegisterDialog()
{
    delete mWrongDataNoticeListener;
    mWrongDataNoticeListener = nullptr;
}

void RegisterDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "cancel")
    {
        Client::setState(STATE_LOGIN);
    }
    else if (event.getId() == "register" && canSubmit())
    {
        const std::string user = mUserField->getText();
        logger->log("RegisterDialog::register Username is %s", user.c_str());

        std::string errorMsg;
        int error = 0;

        const unsigned int minUser = Net::getLoginHandler()
            ->getMinUserNameLength();
        const unsigned int maxUser = Net::getLoginHandler()
            ->getMaxUserNameLength();
        const unsigned int minPass = Net::getLoginHandler()
            ->getMinPasswordLength();
        const unsigned int maxPass = Net::getLoginHandler()
            ->getMaxPasswordLength();

        if (user.length() < minUser)
        {
            // Name too short
            errorMsg = strprintf
                (_("The username needs to be at least %u characters long."),
                 minUser);
            error = 1;
        }
        else if (user.length() > maxUser - 1 )
        {
            // Name too long
            errorMsg = strprintf
                (_("The username needs to be less than %u characters long."),
                 maxUser);
            error = 1;
        }
        else if (mPasswordField->getText().length() < minPass)
        {
            // Pass too short
            errorMsg = strprintf
                (_("The password needs to be at least %u characters long."),
                 minPass);
            error = 2;
        }
        else if (mPasswordField->getText().length() > maxPass - 1 )
        {
            // Pass too long
            errorMsg = strprintf
                (_("The password needs to be less than %u characters long."),
                 maxPass);
            error = 2;
        }
        else if (mPasswordField->getText() != mConfirmField->getText())
        {
            // Password does not match with the confirmation one
            errorMsg = _("Passwords do not match.");
            error = 2;
        }

        // TODO: Check if a valid email address was given

        if (error > 0)
        {
            if (error == 1)
            {
                mWrongDataNoticeListener->setTarget(this->mUserField);
            }
            else if (error == 2)
            {
                // Reset password confirmation
                mPasswordField->setText("");
                mConfirmField->setText("");

                mWrongDataNoticeListener->setTarget(this->mPasswordField);
            }

            OkDialog *const dlg = new OkDialog(
                _("Error"), errorMsg, DIALOG_ERROR);
            dlg->addActionListener(mWrongDataNoticeListener);
        }
        else
        {
            // No errors detected, register the new user.
            mRegisterButton->setEnabled(false);

            mLoginData->username = mUserField->getText();
            mLoginData->password = mPasswordField->getText();
            if (mFemaleButton && mFemaleButton->isSelected())
                mLoginData->gender = GENDER_FEMALE;
            else if (mOtherButton && mOtherButton->isSelected())
                mLoginData->gender = GENDER_OTHER;
            else
                mLoginData->gender = GENDER_MALE;

            if (mEmailField)
                mLoginData->email = mEmailField->getText();
            mLoginData->registerLogin = true;

            Client::setState(STATE_REGISTER_ATTEMPT);
        }
    }
}

void RegisterDialog::keyPressed(gcn::KeyEvent &keyEvent A_UNUSED)
{
    mRegisterButton->setEnabled(canSubmit());
}

bool RegisterDialog::canSubmit() const
{
    return !mUserField->getText().empty() &&
           !mPasswordField->getText().empty() &&
           !mConfirmField->getText().empty() &&
           Client::getState() == STATE_REGISTER;
}
