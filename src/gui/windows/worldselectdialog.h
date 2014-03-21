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

#ifndef GUI_WINDOWS_WORLDSELECTDIALOG_H
#define GUI_WINDOWS_WORLDSELECTDIALOG_H

#include "gui/widgets/window.h"

#include "net/worldinfo.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

class Button;
class ListBox;
class WorldListModel;

/**
 * The server select dialog.
 *
 * \ingroup Interface
 */
class WorldSelectDialog final : public Window,
                                public ActionListener,
                                public KeyListener
{
    public:
        /**
         * Constructor
         *
         * @see Window::Window
         */
        explicit WorldSelectDialog(Worlds worlds);

        A_DELETE_COPY(WorldSelectDialog)

        void postInit() override final;

        /**
         * Destructor.
         */
        ~WorldSelectDialog();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        void keyPressed(KeyEvent &event) override final;

    private:
        WorldListModel *mWorldListModel;
        ListBox *mWorldList;
        Button *mChangeLoginButton;
        Button *mChooseWorld;
};

#endif  // GUI_WINDOWS_WORLDSELECTDIALOG_H
