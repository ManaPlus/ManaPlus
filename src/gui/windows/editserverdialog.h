/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The Mana Developers
 *  Copyright (C) 2012-2016  The ManaPlus Developers
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

#ifndef GUI_WINDOWS_EDITSERVERDIALOG_H
#define GUI_WINDOWS_EDITSERVERDIALOG_H

class Button;
class CheckBox;
class DropDown;
class IntTextField;
class ServerDialog;
class TextField;
class TypeListModel;

#include "gui/widgets/window.h"

#include "net/serverinfo.h"

#include "listeners/actionlistener.h"
#include "listeners/keylistener.h"

/**
 * The custom server addition dialog.
 *
 * \ingroup Interface
 */
class EditServerDialog final : public Window,
                               public ActionListener,
                               public KeyListener
{
    public:
        EditServerDialog(ServerDialog *const parent,
                         const ServerInfo &server,
                         const int index);

        A_DELETE_COPY(EditServerDialog)

        ~EditServerDialog();

        void postInit() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        void keyPressed(KeyEvent &event) override final;

    private:
        TextField *mServerAddressField A_NONNULLPOINTER;
        TextField *mPortField A_NONNULLPOINTER;
        TextField *mNameField A_NONNULLPOINTER;
        TextField *mDescriptionField A_NONNULLPOINTER;
        TextField *mOnlineListUrlField A_NONNULLPOINTER;
        IntTextField *mPacketVersionField A_NONNULLPOINTER;
        Button *mConnectButton A_NONNULLPOINTER;
        Button *mOkButton A_NONNULLPOINTER;
        Button *mCancelButton A_NONNULLPOINTER;
        CheckBox *mPersistentIp A_NONNULLPOINTER;
        TypeListModel *mTypeListModel A_NONNULLPOINTER;
        DropDown *mTypeField A_NONNULLPOINTER;

        ServerDialog *mServerDialog;
        ServerInfo mServer;
        int mIndex;
};

#endif  // GUI_WINDOWS_EDITSERVERDIALOG_H
