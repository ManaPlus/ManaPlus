/*
 *  The Mana Client
 *  Copyright (C) 2011-2012  The Mana Developers
 *  Copyright (C) 2012  The ManaPlus Developers
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

#include "gui/editserverdialog.h"

#include "configuration.h"
#include "keydata.h"
#include "keyevent.h"

#include "gui/okdialog.h"
#include "gui/sdlinput.h"
#include "gui/serverdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"

std::string TypeListModel::getElementAt(int elementIndex)
{
    if (elementIndex == 0)
        return "TmwAthena";
    else if (elementIndex == 1)
        return "Evol";
#ifdef EATHENA_SUPPORT
    else if (elementIndex == 2)
        return "eAthena";
#ifdef MANASERV_SUPPORT
    else if (elementIndex == 3)
        return "ManaServ";
#endif
#else
#ifdef MANASERV_SUPPORT
    else if (elementIndex == 2)
        return "ManaServ";
#endif
#endif
    else
        return "Unknown";
}

EditServerDialog::EditServerDialog(ServerDialog *parent, ServerInfo server,
                                   int index) :
    Window(_("Edit Server"), true, parent),
    mServerDialog(parent),
    mServer(server),
    mIndex(index)
{
    setWindowName("EditServerDialog");

    Label *nameLabel = new Label(_("Name:"));
    Label *serverAdressLabel = new Label(_("Address:"));
    Label *portLabel = new Label(_("Port:"));
    Label *typeLabel = new Label(_("Server type:"));
    Label *descriptionLabel = new Label(_("Description:"));
    mServerAddressField = new TextField(std::string());
    mPortField = new TextField(std::string());
    mPortField->setNumeric(true);
    mPortField->setRange(1, 65535);

    mTypeListModel = new TypeListModel();
    mTypeField = new DropDown(mTypeListModel);
    mTypeField->setSelected(0); // TmwAthena by default for now.

    mNameField = new TextField(std::string());
    mDescriptionField = new TextField(std::string());

    mOkButton = new Button(_("OK"), "addServer", this);
    mCancelButton = new Button(_("Cancel"), "cancel", this);

    mServerAddressField->addActionListener(this);
    mPortField->addActionListener(this);

    place(0, 0, nameLabel);
    place(1, 0, mNameField, 4).setPadding(3);
    place(0, 1, serverAdressLabel);
    place(1, 1, mServerAddressField, 4).setPadding(3);
    place(0, 2, portLabel);
    place(1, 2, mPortField, 4).setPadding(3);
    place(0, 3, typeLabel);
    place(1, 3, mTypeField).setPadding(3);
    place(0, 4, descriptionLabel);
    place(1, 4, mDescriptionField, 4).setPadding(3);
    place(4, 5, mOkButton);
    place(3, 5, mCancelButton);

    // Do this manually instead of calling reflowLayout so we can enforce a
    // minimum width.
    int width = 0, height = 0;
    getLayout().reflow(width, height);
    if (width < 300)
    {
        width = 300;
        getLayout().reflow(width, height);
    }
    if (height < 120)
    {
        height = 120;
        getLayout().reflow(width, height);
    }

    setContentSize(width, height);

    setMinWidth(getWidth());
    setMinHeight(getHeight());
    setDefaultSize(getWidth(), getHeight(), ImageRect::CENTER);

    setResizable(false);
    addKeyListener(this);

    loadWindowState();

    mNameField->setText(mServer.name);
    mDescriptionField->setText(mServer.description);
    mServerAddressField->setText(mServer.hostname);
    mPortField->setText(toString(mServer.port));

    switch (mServer.type)
    {
#ifdef EATHENA_SUPPORT
        case ServerInfo::EATHENA:
            mTypeField->setSelected(2);
            break;
        case ServerInfo::MANASERV:
#ifdef MANASERV_SUPPORT
            mTypeField->setSelected(3);
            break;
#endif
#else
        case ServerInfo::MANASERV:
#ifdef MANASERV_SUPPORT
            mTypeField->setSelected(2);
            break;
#endif
#endif
        default:
        case ServerInfo::UNKNOWN:
        case ServerInfo::TMWATHENA:
            mTypeField->setSelected(0);
            break;
        case ServerInfo::EVOL:
            mTypeField->setSelected(1);
            break;
    }

    setLocationRelativeTo(getParentWindow());
    setVisible(true);

    mNameField->requestFocus();
}

EditServerDialog::~EditServerDialog()
{
    delete mTypeListModel;
}

void EditServerDialog::logic()
{
    Window::logic();
}

void EditServerDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok")
    {
        // Give focus back to the server dialog.
        mServerAddressField->requestFocus();
    }
    if (event.getId() == "addServer")
    {
        // Check the given information
        if (mServerAddressField->getText().empty()
            || mPortField->getText().empty())
        {
            OkDialog *dlg = new OkDialog(_("Error"),
                _("Please at least type both the address and the port "
                  "of the server."), DIALOG_ERROR);
            dlg->addActionListener(this);
        }
        else
        {
            mCancelButton->setEnabled(false);
            mOkButton->setEnabled(false);

            mServer.name = mNameField->getText();
            mServer.description = mDescriptionField->getText();
            mServer.hostname = mServerAddressField->getText();
            mServer.port = static_cast<short>(atoi(
                mPortField->getText().c_str()));

            if (mTypeField)
            {
                switch (mTypeField->getSelected())
                {
                    case 0:
                        mServer.type = ServerInfo::TMWATHENA;
                        break;
                    case 1:
                        mServer.type = ServerInfo::EVOL;
                        break;
#ifdef EATHENA_SUPPORT
                    case 2:
                        mServer.type = ServerInfo::EATHENA;
                        break;
#ifdef MANASERV_SUPPORT
                    case 3:
                        mServer.type = ServerInfo::MANASERV;
                        break;
#endif
#else
#ifdef MANASERV_SUPPORT
                    case 2:
                        mServer.type = ServerInfo::MANASERV;
                        break;
#endif
#endif
                    default:
                        mServer.type = ServerInfo::UNKNOWN;
                }
            }
            else
            {
                mServer.type = ServerInfo::TMWATHENA;
            }

            // Tell the server has to be saved
            mServer.save = true;

            //Add server
            mServerDialog->updateServer(mServer, mIndex);
            scheduleDelete();
        }
    }
    else if (event.getId() == "cancel")
    {
        scheduleDelete();
    }
}

void EditServerDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    int actionId = static_cast<KeyEvent*>(&keyEvent)->getActionId();

    if (actionId == Input::KEY_GUI_CANCEL)
    {
        scheduleDelete();
    }
    else if (actionId == Input::KEY_GUI_SELECT
             || actionId == Input::KEY_GUI_SELECT2)
    {
        action(gcn::ActionEvent(nullptr, mOkButton->getActionEventId()));
    }
}
