/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2021  Andrei Karas
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

#ifndef GUI_WINDOWS_CONFIRMDIALOG_H
#define GUI_WINDOWS_CONFIRMDIALOG_H

#include "localconsts.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class TextBox;

/**
 * An option dialog.
 *
 * \ingroup GUI
 */
class ConfirmDialog notfinal : public Window,
                               public ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        ConfirmDialog(const std::string &restrict title,
                      const std::string &restrict msg,
                      const std::string &restrict soundEvent,
                      const bool ignore,
                      const Modal modal,
                      Window *const parent);

        /**
         * Constructor.
         *
         * @see Window::Window
         */
        ConfirmDialog(const std::string &restrict title,
                      const std::string &restrict msg,
                      const std::string &restrict yesMsg,
                      const std::string &restrict noMsg,
                      const std::string &restrict soundEvent,
                      const bool ignore,
                      const Modal modal,
                      Window *const parent);

        A_DELETE_COPY(ConfirmDialog)

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override;

        void postInit() override final;

    private:
        std::string mYesMsg;
        std::string mNoMsg;
        TextBox *mTextBox A_NONNULLPOINTER;
        bool mIgnore;
};

#endif  // GUI_WINDOWS_CONFIRMDIALOG_H
