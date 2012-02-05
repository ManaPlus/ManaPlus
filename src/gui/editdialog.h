/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011  ManaPlus developers
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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
class EditDialog : public Window, public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        EditDialog(const std::string &title, const std::string &msg,
                   std::string eventOk = ACTION_EDIT_OK, int width = 300,
                   Window *parent = nullptr, bool modal = true);

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        std::string getMsg() const
        { return mTextField->getText(); }

    private:
        std::string mEventOk;

        TextField *mTextField;
};

#endif
