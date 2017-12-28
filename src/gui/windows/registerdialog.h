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

#ifndef GUI_WINDOWS_REGISTERDIALOG_H
#define GUI_WINDOWS_REGISTERDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

class Button;
class LoginData;
class RadioButton;
class TextField;
class WrongDataNoticeListener;

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
        explicit RegisterDialog(LoginData &loginData);

        A_DELETE_COPY(RegisterDialog)

        /**
         * Destructor
         */
        ~RegisterDialog() override final;

        void postInit() override;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override;

        /**
         * Called when a key is pressed in one of the text fields.
         */
        void keyPressed(KeyEvent &event) override;

        void close() override;

    private:
        /**
         * Returns whether submit can be enabled. This is true in the register
         * state, when all necessary fields have some text.
         */
        bool canSubmit() const;

        LoginData *mLoginData A_NONNULLPOINTER;
        TextField *mUserField A_NONNULLPOINTER;
        TextField *mPasswordField A_NONNULLPOINTER;
        TextField *mConfirmField A_NONNULLPOINTER;
        TextField *mEmailField;
        Button *mRegisterButton A_NONNULLPOINTER;
        Button *mCancelButton A_NONNULLPOINTER;
        RadioButton *mMaleButton;
        RadioButton *mFemaleButton;
        WrongDataNoticeListener *mWrongDataNoticeListener A_NONNULLPOINTER;
};

#endif  // GUI_WINDOWS_REGISTERDIALOG_H
