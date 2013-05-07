/*
 *  The Mana Client
 *  Copyright (C) 2011-2012  The Mana Developers
 *  Copyright (C) 2012-2013  The ManaPlus Developers
 *
 *  This file is part of The Mana Client.
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

#ifndef EDITSERVERDIALOG_H
#define EDITSERVERDIALOG_H

class Button;
class Label;
class TextField;
class DropDown;
class ServerDialog;
class TypeListModel;

#include "gui/widgets/window.h"

#include "net/serverinfo.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>
#include <guichan/listmodel.hpp>

/**
 * Server Type List Model
 */
class TypeListModel : public gcn::ListModel
{
    public:
        TypeListModel()
        { }

        /**
         * Used to get number of line in the list
         */
        int getNumberOfElements() override A_WARN_UNUSED
#ifdef EATHENA_SUPPORT
#ifdef MANASERV_SUPPORT
        { return 4; }
#else
        { return 3; }
#endif
#else
#ifdef MANASERV_SUPPORT
        { return 3; }
#else
        { return 2; }
#endif
#endif

        /**
         * Used to get an element from the list
         */
        std::string getElementAt(int elementIndex) override A_WARN_UNUSED;
};

/**
 * The custom server addition dialog.
 *
 * \ingroup Interface
 */
class EditServerDialog final : public Window,
                               public gcn::ActionListener,
                               public gcn::KeyListener
{
    public:
        EditServerDialog(ServerDialog *const parent, ServerInfo server,
                         const int index);

        A_DELETE_COPY(EditServerDialog)

        ~EditServerDialog();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event) override;

        void keyPressed(gcn::KeyEvent &keyEvent) override;

    private:
        TextField *mServerAddressField;
        TextField *mPortField;
        TextField *mNameField;
        TextField *mDescriptionField;
        TextField *mOnlineListUrlField;
        Button *mConnectButton;
        Button *mOkButton;
        Button *mCancelButton;

        TypeListModel *mTypeListModel;
        DropDown *mTypeField;

        ServerDialog *mServerDialog;
        ServerInfo mServer;
        int mIndex;
};

#endif  // EDITSERVERDIALOG_H
