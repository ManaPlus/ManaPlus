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

#ifndef GUI_WINDOWS_REGISTERDIALOG_H
#define GUI_WINDOWS_REGISTERDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

class Button;
class LoginData;
class RadioButton;
class TextField;

/**
 * Listener used while dealing with wrong data. It is used to direct the focus
 * to the field which contained wrong data when the Ok button was pressed on
 * the error notice.
 */
class WrongDataNoticeListener final : public ActionListener
{
    public:
        WrongDataNoticeListener();

        A_DELETE_COPY(WrongDataNoticeListener)

        void setTarget(TextField *const textField);

        void action(const ActionEvent &event) override final;
    private:
        TextField *mTarget;
};

/**
 * The registration dialog.
 *
 * \ingroup Interface
 */
class RegisterDialog final : public Window,
                             public ActionListener,
                             public KeyListener
{
    public:
        /**
         * Constructor. Name, password and server fields will be initialized to
         * the information already present in the LoginData instance.
         *
         * @see Window::Window
         */
        explicit RegisterDialog(LoginData *const loginData);

        A_DELETE_COPY(RegisterDialog)

        /**
         * Destructor
         */
        ~RegisterDialog();

        void postInit() override;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override;

        /**
         * Called when a key is pressed in one of the text fields.
         */
        void keyPressed(KeyEvent &keyEvent) override;

        void close() override;

    private:
        /**
         * Returns whether submit can be enabled. This is true in the register
         * state, when all necessary fields have some text.
         */
        bool canSubmit() const;

        LoginData *mLoginData;
        TextField *mUserField;
        TextField *mPasswordField;
        TextField *mConfirmField;
        TextField *mEmailField;
        Button *mRegisterButton;
        Button *mCancelButton;
        RadioButton *mMaleButton;
        RadioButton *mFemaleButton;
        RadioButton *mOtherButton;
        WrongDataNoticeListener *mWrongDataNoticeListener;
};

#endif  // GUI_WINDOWS_REGISTERDIALOG_H
