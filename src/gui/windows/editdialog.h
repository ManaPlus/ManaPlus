/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2009-2022  Andrei Karas
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

#ifndef GUI_WINDOWS_EDITDIALOG_H
#define GUI_WINDOWS_EDITDIALOG_H

#include "localconsts.h"

#include "gui/widgets/window.h"
#include "gui/widgets/textfield.h"

#include "listeners/actionlistener.h"

#define ACTION_EDIT_OK "edit ok"

/**
 * An 'Ok' button dialog.
 *
 * \ingroup GUI
 */
class EditDialog final : public Window,
                         public ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        EditDialog(const std::string &restrict title,
                   const std::string &restrict msg,
                   const std::string &restrict eventOk,
                   const int width,
                   Window *const parent,
                   const Modal modal);

        A_DELETE_COPY(EditDialog)

        void postInit() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        std::string getMsg() const A_WARN_UNUSED
        { return mTextField->getText(); }

    private:
        std::string mEventOk;
        TextField *mTextField A_NONNULLPOINTER;
};

#endif  // GUI_WINDOWS_EDITDIALOG_H
