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

#ifndef GUI_WINDOWS_QUITDIALOG_H
#define GUI_WINDOWS_QUITDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

#include <vector>

class Button;
class RadioButton;

/**
 * The quit dialog.
 *
 * \ingroup Interface
 */
class QuitDialog final : public Window,
                         public ActionListener,
                         public KeyListener
{
    public:
        /**
         * Constructor
         *
         * @pointerToMe  will be set to NULL when the QuitDialog is destroyed
         */
        explicit QuitDialog(QuitDialog **const pointerToMe);

        A_DELETE_COPY(QuitDialog)

        /**
         * Destructor
         */
        ~QuitDialog();

        void postInit() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        void keyPressed(KeyEvent &keyEvent) override final;

    private:
        void placeOption(ContainerPlacer &placer,
                         RadioButton *const option);
        std::vector<RadioButton*> mOptions;

        RadioButton *mLogoutQuit;
        RadioButton *mForceQuit;
        RadioButton *mSwitchAccountServer;
        RadioButton *mSwitchCharacter;
        RadioButton *mRate;
        Button *mOkButton;
        Button *mCancelButton;

        QuitDialog **mMyPointer;
        bool mNeedForceQuit;
};

#endif  // GUI_WINDOWS_QUITDIALOG_H
