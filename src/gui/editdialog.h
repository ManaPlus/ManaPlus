/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2013  The ManaPlus developers
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

#ifndef EDIT_DIALOG_H
#define EDIT_DIALOG_H

#include "localconsts.h"

#include "gui/widgets/window.h"
#include "gui/widgets/textfield.h"

#include <guichan/actionlistener.hpp>

#define ACTION_EDIT_OK "edit ok"

class TextField;

/**
 * An 'Ok' button dialog.
 *
 * \ingroup GUI
 */
class EditDialog final : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        EditDialog(const std::string &title, const std::string &msg,
                   const std::string &eventOk = ACTION_EDIT_OK,
                   const int width = 300, Window *const parent = nullptr,
                   const bool modal = true);

        A_DELETE_COPY(EditDialog)

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event) override;

        std::string getMsg() const A_WARN_UNUSED
        { return mTextField->getText(); }

    private:
        std::string mEventOk;

        TextField *mTextField;
};

#endif
