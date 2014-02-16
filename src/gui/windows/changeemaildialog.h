/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef GUI_WINDOWS_CHANGEEMAILDIALOG_H
#define GUI_WINDOWS_CHANGEEMAILDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class LoginData;
class TextField;
class WrongDataNoticeListener;

/**
 * The Change email dialog.
 *
 * \ingroup Interface
 */
class ChangeEmailDialog final : public Window,
                                public ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        explicit ChangeEmailDialog(LoginData *const data);

        A_DELETE_COPY(ChangeEmailDialog)

        /**
         * Destructor.
         */
        ~ChangeEmailDialog();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        /**
         * This is used to pass the pointer to where the new email should be
         * put when the dialog finishes.
         */
        static void setEmail(std::string *email);

    private:
        TextField *mFirstEmailField;
        TextField *mSecondEmailField;

        Button *mChangeEmailButton;
        Button *mCancelButton;

        WrongDataNoticeListener *mWrongDataNoticeListener;

        LoginData *mLoginData;
};

#endif  // GUI_WINDOWS_CHANGEEMAILDIALOG_H
