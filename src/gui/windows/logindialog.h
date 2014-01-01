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

#ifndef GUI_WINDOWS_LOGINDIALOG_H
#define GUI_WINDOWS_LOGINDIALOG_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>

#include <string>

class Button;
class CheckBox;
class DropDown;
class Label;
class LoginData;
class TextField;
class UpdateListModel;
class UpdateTypeModel;

/**
 * The login dialog.
 *
 * \ingroup Interface
 */
class LoginDialog final : public Window, public gcn::ActionListener,
                          public gcn::KeyListener
{
    public:
        /**
         * Constructor
         *
         * @see Window::Window
         */
        LoginDialog(LoginData *const data, std::string serverName,
                    std::string *const updateHost);

        A_DELETE_COPY(LoginDialog)

        ~LoginDialog();

        void postInit() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event) override final;

        /**
         * Called when a key is pressed in one of the text fields.
         */
        void keyPressed(gcn::KeyEvent &keyEvent) override final;

        void close() override final;

        static std::string savedPasswordKey;
        static std::string savedPassword;

    private:
        /**
         * Returns whether submit can be enabled. This is true in the login
         * state, when all necessary fields have some text.
         */
        bool canSubmit() const;

        void prepareUpdate();

        LoginData *mLoginData;

        TextField *mUserField;
        TextField *mPassField;
        CheckBox *mKeepCheck;
        Label *mUpdateTypeLabel;
        Label *mUpdateHostLabel;
        UpdateTypeModel *mUpdateTypeModel;
        DropDown *mUpdateTypeDropDown;
        Button *mServerButton;
        Button *mLoginButton;
        Button *mRegisterButton;
        CheckBox *mCustomUpdateHost;
        TextField *mUpdateHostText;
        UpdateListModel *mUpdateListModel;
        DropDown *mUpdateHostDropDown;

        std::string *mUpdateHost;
        std::string mServerName;
};

#endif  // GUI_WINDOWS_LOGINDIALOG_H
