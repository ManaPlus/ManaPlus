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

#include "gui/windows/registerdialog.h"

#include "client.h"

#include "events/keyevent.h"

#include "input/keydata.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/passwordfield.h"
#include "gui/widgets/radiobutton.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

WrongDataNoticeListener::WrongDataNoticeListener():
    ActionListener(),
    mTarget(nullptr)
{
}

void WrongDataNoticeListener::setTarget(TextField *const textField)
{
    mTarget = textField;
}

void WrongDataNoticeListener::action(const ActionEvent &event)
{
    if (event.getId() == "ok" && mTarget)
        mTarget->requestFocus();
}

RegisterDialog::RegisterDialog(LoginData *const data) :
    // TRANSLATORS: register dialog name
    Window(_("Register"), false, nullptr, "register.xml"),
    ActionListener(),
    KeyListener(),
    mLoginData(data),
    mUserField(new TextField(this, mLoginData->username)),
    mPasswordField(new PasswordField(this, mLoginData->password)),
    mConfirmField(new PasswordField(this)),
    mEmailField(nullptr),
    // TRANSLATORS: register dialog. button.
    mRegisterButton(new Button(this, _("Register"), "register", this)),
    // TRANSLATORS: register dialog. button.
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    mMaleButton(nullptr),
    mFemaleButton(nullptr),
    mOtherButton(nullptr),
    mWrongDataNoticeListener(new WrongDataNoticeListener)
{
    setCloseButton(true);

    const int optionalActions = Net::getLoginHandler()->
        supportedOptionalActions();

    // TRANSLATORS: register dialog. label.
    Label *const userLabel = new Label(this, _("Name:"));
    // TRANSLATORS: register dialog. label.
    Label *const passwordLabel = new Label(this, _("Password:"));
    // TRANSLATORS: register dialog. label.
    Label *const confirmLabel = new Label(this, _("Confirm:"));

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
        // TRANSLATORS: register dialog. button.
        mMaleButton = new RadioButton(this, _("Male"), "sex", true);
        // TRANSLATORS: register dialog. button.
        mFemaleButton = new RadioButton(this, _("Female"), "sex", false);
        if (serverVersion >= 5)
        {
            // TRANSLATORS: register dialog. button.
            mOtherButton = new RadioButton(this, _("Other"), "sex", false);
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
        // TRANSLATORS: register dialog. label.
        Label *const emailLabel = new Label(this, _("Email:"));
        mEmailField = new TextField(this);
        placer(0, row, emailLabel);
        placer(1, row, mEmailField, 3).setPadding(2);
//        row++;
    }

    placer = getPlacer(0, 2);
    placer(1, 0, mRegisterButton);
    placer(2, 0, mCancelButton);
    reflowLayout(250, 0);

    mUserField->addKeyListener(this);
    mPasswordField->addKeyListener(this);
    mConfirmField->addKeyListener(this);

    mUserField->setActionEventId("register");
    mPasswordField->setActionEventId("register");
    mConfirmField->setActionEventId("register");

    mUserField->addActionListener(this);
    mPasswordField->addActionListener(this);
    mConfirmField->addActionListener(this);

    center();
}

void RegisterDialog::postInit()
{
    setVisible(true);
    mUserField->requestFocus();
    mUserField->setCaretPosition(static_cast<unsigned>(
                                 mUserField->getText().length()));

    mRegisterButton->setEnabled(canSubmit());
}

RegisterDialog::~RegisterDialog()
{
    delete2(mWrongDataNoticeListener);
}

void RegisterDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "cancel")
    {
        close();
    }
    else if (eventId == "register" && canSubmit())
    {
        const std::string &user = mUserField->getText();
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
                // TRANSLATORS: error message
                (_("The username needs to be at least %u characters long."),
                 minUser);
            error = 1;
        }
        else if (user.length() > maxUser - 1)
        {
            // Name too long
            errorMsg = strprintf
                // TRANSLATORS: error message
                (_("The username needs to be less than %u characters long."),
                 maxUser);
            error = 1;
        }
        else if (mPasswordField->getText().length() < minPass)
        {
            // Pass too short
            errorMsg = strprintf
                // TRANSLATORS: error message
                (_("The password needs to be at least %u characters long."),
                 minPass);
            error = 2;
        }
        else if (mPasswordField->getText().length() > maxPass)
        {
            // Pass too long
            errorMsg = strprintf
                // TRANSLATORS: error message
                (_("The password needs to be less than %u characters long."),
                 maxPass);
            error = 2;
        }
        else if (mPasswordField->getText() != mConfirmField->getText())
        {
            // Password does not match with the confirmation one
            // TRANSLATORS: error message
            errorMsg = _("Passwords do not match.");
            error = 2;
        }

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
                // TRANSLATORS: error message
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

            client->setState(STATE_REGISTER_ATTEMPT);
        }
    }
}

void RegisterDialog::keyPressed(KeyEvent &event)
{
    if (event.isConsumed())
    {
        mRegisterButton->setEnabled(canSubmit());
        return;
    }
    const int actionId = event.getActionId();
    if (actionId == static_cast<int>(Input::KEY_GUI_CANCEL))
    {
        action(ActionEvent(nullptr, mCancelButton->getActionEventId()));
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_SELECT)
             || actionId == static_cast<int>(Input::KEY_GUI_SELECT2))
    {
        action(ActionEvent(nullptr, mRegisterButton->getActionEventId()));
    }
    else
    {
        mRegisterButton->setEnabled(canSubmit());
    }
}

bool RegisterDialog::canSubmit() const
{
    return !mUserField->getText().empty() &&
           !mPasswordField->getText().empty() &&
           !mConfirmField->getText().empty() &&
           client->getState() == STATE_REGISTER;
}

void RegisterDialog::close()
{
    client->setState(STATE_LOGIN);
    Window::close();
}
