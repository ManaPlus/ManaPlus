/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
 *  Copyright (C) 2019-2022  Andrei Karas
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

#ifndef GUI_WINDOWS_CONNECTIONDIALOG_H
#define GUI_WINDOWS_CONNECTIONDIALOG_H

#include "enums/state.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

/**
 * The connection dialog.
 *
 * \ingroup Interface
 */
class ConnectionDialog final : public Window,
                               private ActionListener
{
    public:
        /**
         * Constructor.
         *
         * @param text        The text to display
         * @param cancelState The state to enter when Cancel is pressed
         *
         * @see Window::Window
         */
        ConnectionDialog(const std::string &text, const StateT cancelState);

        A_DELETE_COPY(ConnectionDialog)

        void postInit() override final;

        /**
         * Called when the user presses Cancel. Restores the global state to
         * the previous one.
         */
        void action(const ActionEvent &event) override final;

        void draw(Graphics *const graphics) override final A_NONNULL(2);

        void safeDraw(Graphics *const graphics) override final A_NONNULL(2);

    private:
        StateT mCancelState;
};

#endif  // GUI_WINDOWS_CONNECTIONDIALOG_H
