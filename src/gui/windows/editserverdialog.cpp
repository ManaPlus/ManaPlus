/*
 *  The Mana Client
 *  Copyright (C) 2011-2012  The Mana Developers
 *  Copyright (C) 2012-2014  The ManaPlus Developers
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

#include "gui/windows/editserverdialog.h"

#include "events/keyevent.h"

#include "input/keydata.h"

#include "gui/models/typelistmodel.h"

#include "gui/windows/okdialog.h"
#include "gui/windows/serverdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"

EditServerDialog::EditServerDialog(ServerDialog *const parent,
                                   ServerInfo server,
                                   const int index) :
    // TRANSLATORS: edit server dialog name
    Window(_("Edit Server"), true, parent),
    ActionListener(),
    KeyListener(),
    mServerAddressField(new TextField(this, std::string())),
    mPortField(new TextField(this, std::string())),
    mNameField(new TextField(this, std::string())),
    mDescriptionField(new TextField(this, std::string())),
    mOnlineListUrlField(new TextField(this, std::string())),
    // TRANSLATORS: edit server dialog button
    mConnectButton(new Button(this, _("Connect"), "connect", this)),
    // TRANSLATORS: edit server dialog button
    mOkButton(new Button(this, _("OK"), "addServer", this)),
    // TRANSLATORS: edit server dialog button
    mCancelButton(new Button(this, _("Cancel"), "cancel", this)),
    mTypeListModel(new TypeListModel),
    mTypeField(new DropDown(this, mTypeListModel, false, true)),
    mServerDialog(parent),
    mServer(server),
    mIndex(index)
{
    setWindowName("EditServerDialog");

    // TRANSLATORS: edit server dialog label
    Label *const nameLabel = new Label(this, _("Name:"));
    // TRANSLATORS: edit server dialog label
    Label *const serverAdressLabel = new Label(this, _("Address:"));
    // TRANSLATORS: edit server dialog label
    Label *const portLabel = new Label(this, _("Port:"));
    // TRANSLATORS: edit server dialog label
    Label *const typeLabel = new Label(this, _("Server type:"));
    // TRANSLATORS: edit server dialog label
    Label *const descriptionLabel = new Label(this, _("Description:"));
    // TRANSLATORS: edit server dialog label
    Label *const onlineListUrlLabel = new Label(this, _("Online list url:"));
    mPortField->setNumeric(true);
    mPortField->setRange(1, 65535);

    mTypeField->setSelected(0);  // TmwAthena by default

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
    place(0, 5, onlineListUrlLabel);
    place(1, 5, mOnlineListUrlField, 4).setPadding(3);
    place(0, 6, mConnectButton);
    place(4, 6, mOkButton);
    place(3, 6, mCancelButton);

    // Do this manually instead of calling reflowLayout so we can enforce a
    // minimum width.
    int width = 0;
    int height = 0;
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
    mOnlineListUrlField->setText(mServer.onlineListUrl);
    mServerAddressField->setText(mServer.hostname);
    mPortField->setText(toString(mServer.port));

    switch (mServer.type)
    {
#ifdef EATHENA_SUPPORT
        case ServerInfo::EATHENA:
            mTypeField->setSelected(2);
            break;
#endif
        default:
        case ServerInfo::UNKNOWN:
        case ServerInfo::TMWATHENA:
#ifndef EATHENA_SUPPORT
        case ServerInfo::EATHENA:
#endif
            mTypeField->setSelected(0);
            break;
        case ServerInfo::EVOL:
            mTypeField->setSelected(1);
            break;
    }

    setLocationRelativeTo(getParentWindow());
}

EditServerDialog::~EditServerDialog()
{
    delete mTypeListModel;
}

void EditServerDialog::postInit()
{
    setVisible(true);
    mNameField->requestFocus();
}

void EditServerDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();

    if (eventId == "ok")
    {
        // Give focus back to the server dialog.
        mServerAddressField->requestFocus();
    }
    if (eventId == "addServer" || eventId == "connect")
    {
        // Check the given information
        if (mServerAddressField->getText().empty()
            || mPortField->getText().empty())
        {
            // TRANSLATORS: edit server dialog error header
            OkDialog *const dlg = new OkDialog(_("Error"),
                // TRANSLATORS: edit server dialog error message
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
            mServer.onlineListUrl = mOnlineListUrlField->getText();
            mServer.hostname = mServerAddressField->getText();
            mServer.port = static_cast<int16_t>(atoi(
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
#endif
                    default:
                        mServer.type = ServerInfo::UNKNOWN;
                        break;
                }
            }
            else
            {
                mServer.type = ServerInfo::TMWATHENA;
            }

            // Tell the server has to be saved
            mServer.save = true;

            // Add server
            mServerDialog->updateServer(mServer, mIndex);
            if (eventId == "connect")
                mServerDialog->connectToSelectedServer();
            scheduleDelete();
        }
    }
    else if (eventId == "cancel")
    {
        scheduleDelete();
    }
}

void EditServerDialog::keyPressed(KeyEvent &keyEvent)
{
    if (keyEvent.isConsumed())
        return;

    const int actionId = keyEvent.getActionId();

    if (actionId == static_cast<int>(Input::KEY_GUI_CANCEL))
    {
        scheduleDelete();
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_SELECT)
             || actionId == static_cast<int>(Input::KEY_GUI_SELECT2))
    {
        action(ActionEvent(nullptr, mOkButton->getActionEventId()));
    }
}
