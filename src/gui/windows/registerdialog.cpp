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

#include "gui/windows/registerdialog.h"

#include "client.h"
#include "configuration.h"

#include "being/being.h"

#include "gui/windows/okdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/containerplacer.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layoutcell.h"
#include "gui/widgets/passwordfield.h"
#include "gui/widgets/radiobutton.h"

#include "listeners/wrongdatanoticelistener.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/serverfeatures.h"

#include "utils/delete2.h"
#include "utils/gettext.h"

#include "debug.h"

RegisterDialog::RegisterDialog(LoginData &data) :
    // TRANSLATORS: register dialog name
    Window(_("Register"), Modal_false, nullptr, "register.xml"),
    ActionListener(),
    KeyListener(),
    mLoginData(&data),
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

    if (features.getIntValue("forceAccountGender") == -1)
    {
        // TRANSLATORS: register dialog. button.
        mMaleButton = new RadioButton(this, _("Male"), "sex", true);
        // TRANSLATORS: register dialog. button.
        mFemaleButton = new RadioButton(this, _("Female"), "sex", false);
        if (serverFeatures->haveAccountOtherGender())
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

    if (serverFeatures->haveEmailOnRegister())
    {
        // TRANSLATORS: register dialog. label.
        Label *const emailLabel = new Label(this, _("Email:"));
        mEmailField = new TextField(this);
        placer(0, row, emailLabel);
        placer(1, row, mEmailField, 3).setPadding(2);
        mEmailField->addKeyListener(this);
        mEmailField->setActionEventId("register");
        mEmailField->addActionListener(this);
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
    Window::postInit();
    setVisible(Visible_true);
    mUserField->requestFocus();
    mUserField->setCaretPosition(CAST_U32(
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

        const unsigned int minUser = loginHandler->getMinUserNameLength();
        const unsigned int maxUser = loginHandler->getMaxUserNameLength();
        const unsigned int minPass = loginHandler->getMinPasswordLength();
        const unsigned int maxPass = loginHandler->getMaxPasswordLength();

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
        else if (mEmailField &&
                 mEmailField->getText().find('@') == std::string::npos)
        {
            // TRANSLATORS: error message
            errorMsg = _("Incorrect email.");
            error = 1;
        }
        else if (mEmailField && mEmailField->getText().size() > 40)
        {
            // TRANSLATORS: error message
            errorMsg = _("Email too long.");
            error = 1;
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

            OkDialog *const dlg = CREATEWIDGETR(OkDialog,
                // TRANSLATORS: error message
                _("Error"), errorMsg, _("OK"),
                DialogType::ERROR,
                Modal_true,
                ShowCenter_true,
                nullptr,
                260);
            dlg->addActionListener(mWrongDataNoticeListener);
        }
        else
        {
            // No errors detected, register the new user.
            mRegisterButton->setEnabled(false);
            mLoginData->username = mUserField->getText();
            mLoginData->password = mPasswordField->getText();
            if (features.getIntValue("forceAccountGender") == -1)
            {
                if (mFemaleButton && mFemaleButton->isSelected())
                    mLoginData->gender = Gender::FEMALE;
                else if (mOtherButton && mOtherButton->isSelected())
                    mLoginData->gender = Gender::OTHER;
                else
                    mLoginData->gender = Gender::MALE;
            }
            else
            {
                mLoginData->gender = Being::intToGender(
                    CAST_U8(features.getIntValue("forceAccountGender")));
            }

            if (mEmailField)
                mLoginData->email = mEmailField->getText();
            mLoginData->registerLogin = true;

            client->setState(State::REGISTER_ATTEMPT);
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
    const InputActionT actionId = event.getActionId();
    if (actionId == InputAction::GUI_CANCEL)
    {
        action(ActionEvent(nullptr, mCancelButton->getActionEventId()));
    }
    else if (actionId == InputAction::GUI_SELECT ||
             actionId == InputAction::GUI_SELECT2)
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
           client->getState() == State::REGISTER &&
           (!mEmailField || !mEmailField->getText().empty());
}

void RegisterDialog::close()
{
    client->setState(State::LOGIN);
    Window::close();
}
