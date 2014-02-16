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

#ifndef GUI_WINDOWS_OKDIALOG_H
#define GUI_WINDOWS_OKDIALOG_H

#include "localconsts.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class TextBox;

enum
{
    DIALOG_OK = 0,
    DIALOG_ERROR,
    DIALOG_SILENCE
};

/**
 * An 'Ok' button dialog.
 *
 * \ingroup GUI
 */
class OkDialog final : public Window,
                       public ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        OkDialog(const std::string &restrict title,
                 const std::string &restrict msg,
                 const int soundEvent = DIALOG_OK, const bool modal = true,
                 const bool showCenter = true, Window *const parent = nullptr,
                 const int minWidth = 260);

        A_DELETE_COPY(OkDialog)

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

    private:
        TextBox *mTextBox;
};

#endif  // GUI_WINDOWS_OKDIALOG_H
