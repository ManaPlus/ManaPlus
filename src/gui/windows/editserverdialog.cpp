/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2012  The Mana Developers
 *  Copyright (C) 2012-2019  The ManaPlus Developers
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

#include "const/net/maxpacketversion.h"

#include "gui/models/typelistmodel.h"

#include "gui/windows/okdialog.h"
#include "gui/windows/serverdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/inttextfield.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"

#include "utils/gettext.h"

#include "debug.h"

EditServerDialog::EditServerDialog(ServerDialog *const parent,
                                   const ServerInfo &server,
                                   const int index) :
    // TRANSLATORS: edit server dialog name
    Window(_("Edit Server"), Modal_true, parent, "editserverdialog.xml"),
    ActionListener(),
    KeyListener(),
    mServerAddressField(new TextField(this, std::string(),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    mPortField(new TextField(this, std::string(),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    mNameField(new TextField(this, std::string(),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    mDescriptionField(new TextField(this, std::string(),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    mOnlineListUrlField(new TextField(this, std::string(),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    mPacketVersionField(new IntTextField(this, 0, 0, maxPacketVersion,
        Enable_true, 0)),
    // TRANSLATORS: edit server dialog button
    mConnectButton(new Button(this, _("Connect"), "connect",
        BUTTON_SKIN, this)),
    // TRANSLATORS: edit server dialog button
    mOkButton(new Button(this, _("OK"), "addServer", BUTTON_SKIN, this)),
    // TRANSLATORS: edit server dialog button
    mCancelButton(new Button(this, _("Cancel"), "cancel", BUTTON_SKIN, this)),
    // TRANSLATORS: edit server dialog label
    mPersistentIp(new CheckBox(this, _("Use same ip"),
        true, this, "persistentIp")),
    mTypeListModel(new TypeListModel),
    mTypeField(new DropDown(this, mTypeListModel,
        false, Modal_true, nullptr, std::string())),
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
    // TRANSLATORS: edit server dialog label
    Label *const packetVersionLabel = new Label(this, _("Packet version:"));
    mPortField->setNumeric(true);
    mPortField->setRange(1, 65535);

    mTypeField->setSelected(0);  // TmwAthena by default

    mServerAddressField->addActionListener(this);
    mPortField->addActionListener(this);

    place(0, 0, nameLabel, 1, 1);
    place(1, 0, mNameField, 4, 1).setPadding(3);
    place(0, 1, serverAdressLabel, 1, 1);
    place(1, 1, mServerAddressField, 4, 1).setPadding(3);
    place(0, 2, portLabel, 1, 1);
    place(1, 2, mPortField, 4, 1).setPadding(3);
    place(0, 3, typeLabel, 1, 1);
    place(1, 3, mTypeField, 1, 1).setPadding(3);
    place(0, 4, descriptionLabel, 1, 1);
    place(1, 4, mDescriptionField, 4, 1).setPadding(3);
    place(0, 5, onlineListUrlLabel, 1, 1);
    place(1, 5, mOnlineListUrlField, 4, 1).setPadding(3);
    place(0, 6, packetVersionLabel, 1, 1);
    place(1, 6, mPacketVersionField, 4, 1).setPadding(3);
    place(0, 7, mPersistentIp, 4, 1).setPadding(3);
    place(0, 8, mConnectButton, 1, 1);
    place(4, 8, mOkButton, 1, 1);
    place(3, 8, mCancelButton, 1, 1);

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
    setDefaultSize(getWidth(), getHeight(), ImagePosition::CENTER, 0, 0);

    setResizable(false);
    addKeyListener(this);

    loadWindowState();

    mNameField->setText(mServer.name);
    mDescriptionField->setText(mServer.description);
    mOnlineListUrlField->setText(mServer.onlineListUrl);
    mServerAddressField->setText(mServer.hostname);
    mPacketVersionField->setValue(mServer.packetVersion);
    mPortField->setText(toString(mServer.port));
    mPersistentIp->setSelected(mServer.persistentIp);

    switch (mServer.type)
    {
        case ServerType::EATHENA:
#ifdef TMWA_SUPPORT
            mTypeField->setSelected(1);
#else   // TMWA_SUPPORT
            mTypeField->setSelected(0);
#endif  // TMWA_SUPPORT
            break;
        default:
        case ServerType::UNKNOWN:
        case ServerType::TMWATHENA:
            mTypeField->setSelected(0);
            break;
        case ServerType::EVOL2:
#ifdef TMWA_SUPPORT
            mTypeField->setSelected(2);
#else   // TMWA_SUPPORT
            mTypeField->setSelected(1);
#endif  // TMWA_SUPPORT
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
    Window::postInit();
    setVisible(Visible_true);
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
            OkDialog *const dlg = CREATEWIDGETR(OkDialog,
                // TRANSLATORS: edit server dialog error header
                _("Error"),
                // TRANSLATORS: edit server dialog error message
                _("Please at least type both the address and the port "
                  "of the server."),
                // TRANSLATORS: ok dialog button
                _("OK"),
                DialogType::ERROR,
                Modal_true,
                ShowCenter_true,
                nullptr,
                260);
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
            mServer.packetVersion = mPacketVersionField->getValue();
            mServer.port = CAST_S16(atoi(
                mPortField->getText().c_str()));
            mServer.persistentIp = mPersistentIp->isSelected();

            if (mTypeField != nullptr)
            {
                switch (mTypeField->getSelected())
                {
#ifdef TMWA_SUPPORT
                    case 0:
                        mServer.type = ServerType::TMWATHENA;
                        break;
                    case 1:
                        mServer.type = ServerType::EATHENA;
                        break;
                    case 2:
                        mServer.type = ServerType::EVOL2;
                        break;
#else  // TMWA_SUPPORT

                    case 0:
                        mServer.type = ServerType::EATHENA;
                        break;
                    case 1:
                        mServer.type = ServerType::EVOL2;
                        break;
#endif  // TMWA_SUPPORT

                    default:
                        mServer.type = ServerType::UNKNOWN;
                        break;
                }
            }
            else
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

    const InputActionT actionId = event.getActionId();

    if (actionId == InputAction::GUI_CANCEL)
    {
        scheduleDelete();
    }
    else if (actionId == InputAction::GUI_SELECT ||
             actionId == InputAction::GUI_SELECT2)
    {
        action(ActionEvent(nullptr, mOkButton->getActionEventId()));
    }
}
