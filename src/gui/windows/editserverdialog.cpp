/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The Mana Developers
 *  Copyright (C) 2012-2015  The ManaPlus Developers
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

#include "input/inputaction.h"

#include "enums/gui/dialogtype.h"

#include "gui/models/typelistmodel.h"

#include "gui/windows/okdialog.h"
#include "gui/windows/serverdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/textfield.h"

#include "utils/gettext.h"

#include "debug.h"

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
    // TRANSLATORS: edit server dialog label
    mPersistentIp(new CheckBox(this, _("Use same ip"),
                  true, this, "persistentIp")),
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
    place(0, 6, mPersistentIp, 4).setPadding(3);
    place(0, 7, mConnectButton);
    place(4, 7, mOkButton);
    place(3, 7, mCancelButton);

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
    mPersistentIp->setSelected(mServer.persistentIp);

    switch (mServer.type)
    {
        case ServerType::EATHENA:
#ifdef EATHENA_SUPPORT
#ifdef TMWA_SUPPORT
            mTypeField->setSelected(2);
#else   // TMWA_SUPPORT
            mTypeField->setSelected(0);
#endif  // TMWA_SUPPORT
#else   // EATHENA_SUPPORT
            mTypeField->setSelected(0);
#endif  // EATHENA_SUPPORT
            break;
        default:
        case ServerType::UNKNOWN:
        case ServerType::TMWATHENA:
            mTypeField->setSelected(0);
            break;
        case ServerType::EVOL:
#ifdef TMWA_SUPPORT
            mTypeField->setSelected(1);
#else   // TMWA_SUPPORT
            mTypeField->setSelected(0);
#endif  // TMWA_SUPPORT
            break;
        case ServerType::EVOL2:
#ifdef EATHENA_SUPPORT
#ifdef TMWA_SUPPORT
            mTypeField->setSelected(3);
#else   // TMWA_SUPPORT
            mTypeField->setSelected(1);
#endif  // TMWA_SUPPORT
#else   // EATHENA_SUPPORT
            mTypeField->setSelected(0);
#endif  // EATHENA_SUPPORT
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
                  "of the server."),
                // TRANSLATORS: ok dialog button
                _("OK"),
                DialogType::ERROR,
                true, true, nullptr, 260);
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
            mServer.persistentIp = mPersistentIp->isSelected();

#if defined(TMWA_SUPPORT) || defined(EATHENA_SUPPORT)
            if (mTypeField)
            {
                switch (mTypeField->getSelected())
                {
#ifdef TMWA_SUPPORT
                    case 0:
                        mServer.type = ServerType::TMWATHENA;
                        break;
                    case 1:
                        mServer.type = ServerType::EVOL;
                        break;
#ifdef EATHENA_SUPPORT
                    case 2:
                        mServer.type = ServerType::EATHENA;
                        break;
                    case 3:
                        mServer.type = ServerType::EVOL2;
                        break;
#endif
#else
#ifdef EATHENA_SUPPORT
                    case 0:
                        mServer.type = ServerType::EATHENA;
                        break;
                    case 1:
                        mServer.type = ServerType::EVOL2;
                        break;
#endif
#endif
                    default:
                        mServer.type = ServerType::UNKNOWN;
                        break;
                }
            }
            else
#endif  // defined(TMWA_SUPPORT) || defined(EATHENA_SUPPORT)
            {
                mServer.type = ServerType::TMWATHENA;
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

void EditServerDialog::keyPressed(KeyEvent &event)
{
    if (event.isConsumed())
        return;

    const int actionId = event.getActionId();

    if (actionId == static_cast<int>(InputAction::GUI_CANCEL))
    {
        scheduleDelete();
    }
    else if (actionId == static_cast<int>(InputAction::GUI_SELECT)
             || actionId == static_cast<int>(InputAction::GUI_SELECT2))
    {
        action(ActionEvent(nullptr, mOkButton->getActionEventId()));
    }
}
