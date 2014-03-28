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

#include "gui/windows/logindialog.h"

#include "client.h"
#include "configuration.h"

#include "events/keyevent.h"

#include "input/keydata.h"

#include "gui/models/updatelistmodel.h"
#include "gui/models/updatetypemodel.h"

#include "gui/windows/confirmdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/passwordfield.h"

#include "net/charserverhandler.h"
#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/gettext.h"
#include "utils/paths.h"
#include "utils/process.h"

#include "debug.h"

std::string LoginDialog::savedPassword("");
std::string LoginDialog::savedPasswordKey("");

namespace
{
    struct OpenUrlListener : public ActionListener
    {
        OpenUrlListener() :
            ActionListener(),
            url()
        {
        }

        A_DELETE_COPY(OpenUrlListener)

        void action(const ActionEvent &event) override final
        {
            if (event.getId() == "yes")
                openBrowser(url);
        }

        std::string url;
    } urlListener;
}  // namespace

LoginDialog::LoginDialog(LoginData *const data, std::string serverName,
                         std::string *const updateHost):
    // TRANSLATORS: login dialog name
    Window(_("Login"), false, nullptr, "login.xml"),
    ActionListener(),
    KeyListener(),
    mLoginData(data),
    mUserField(new TextField(this, mLoginData->username)),
    mPassField(new PasswordField(this, mLoginData->password)),
    // TRANSLATORS: login dialog label
    mKeepCheck(new CheckBox(this, _("Remember username"),
        mLoginData->remember)),
    // TRANSLATORS: login dialog label
    mUpdateTypeLabel(new Label(this, _("Update:"))),
    mUpdateHostLabel(nullptr),
    mUpdateTypeModel(new UpdateTypeModel),
    mUpdateTypeDropDown(new DropDown(this, mUpdateTypeModel)),
    // TRANSLATORS: login dialog button
    mServerButton(new Button(this, _("Change Server"), "server", this)),
    // TRANSLATORS: login dialog button
    mLoginButton(new Button(this, _("Login"), "login", this)),
    // TRANSLATORS: login dialog button
    mRegisterButton(new Button(this, _("Register"), "register", this)),
    // TRANSLATORS: login dialog checkbox
    mCustomUpdateHost(new CheckBox(this, _("Custom update host"),
        mLoginData->updateType & LoginData::Upd_Custom, this, "customhost")),
    mUpdateHostText(new TextField(this, serverConfig.getValue(
        "customUpdateHost", ""))),
    mUpdateListModel(nullptr),
    mUpdateHostDropDown(nullptr),
    mUpdateHost(updateHost),
    mServerName(serverName)
{
    setCloseButton(true);

    Net::getCharServerHandler()->clear();

    // TRANSLATORS: login dialog label
    Label *const serverLabel1 = new Label(this, _("Server:"));
    Label *const serverLabel2 = new Label(this, serverName);
    serverLabel2->adjustSize();
    // TRANSLATORS: login dialog label
    Label *const userLabel = new Label(this, _("Name:"));
    // TRANSLATORS: login dialog label
    Label *const passLabel = new Label(this, _("Password:"));
    if (mLoginData && mLoginData->updateHosts.size() > 1)
    {
        // TRANSLATORS: login dialog label
        mUpdateHostLabel = new Label(this, strprintf(_("Update host: %s"),
            mLoginData->updateHost.c_str()));
        mUpdateListModel = new UpdateListModel(mLoginData);
        mUpdateHostDropDown = new DropDown(this, mUpdateListModel,
            false, false, this, "updateselect");
        const std::string str = serverConfig.getValue("updateHost2", "");
        if (!str.empty())
            mUpdateHostDropDown->setSelectedString(str);
    }
    else
    {
        mUpdateHostLabel = nullptr;
        mUpdateListModel = nullptr;
        mUpdateHostDropDown = nullptr;
    }
    mUpdateHostText->adjustSize();

    if (mPassField->getText().empty() && LoginDialog::savedPassword != "")
        mPassField->setText(LoginDialog::savedPassword);

    mUpdateTypeDropDown->setActionEventId("updatetype");
    mUpdateTypeDropDown->setSelected((mLoginData->updateType
        | LoginData::Upd_Custom) ^ LoginData::Upd_Custom);

    if (!mCustomUpdateHost->isSelected())
        mUpdateHostText->setVisible(false);

    mUserField->setActionEventId("login");
    mPassField->setActionEventId("login");

    mUserField->addKeyListener(this);
    mPassField->addKeyListener(this);
    mUserField->addActionListener(this);
    mPassField->addActionListener(this);

    place(0, 0, serverLabel1);
    place(1, 0, serverLabel2, 8);
    place(0, 1, userLabel);
    place(1, 1, mUserField, 8);
    place(0, 2, passLabel);
    place(1, 2, mPassField, 8);
    place(0, 6, mUpdateTypeLabel, 1);
    place(1, 6, mUpdateTypeDropDown, 8);
    int n = 7;
    if (mUpdateHostLabel)
    {
        place(0, 7, mUpdateHostLabel, 9);
        place(0, 8, mUpdateHostDropDown, 9);
        n += 2;
    }
    place(0, n, mCustomUpdateHost, 9);
    place(0, n + 1, mUpdateHostText, 9);
    place(0, n + 2, mKeepCheck, 9);
    place(0, n + 3, mRegisterButton).setHAlign(LayoutCell::LEFT);
    place(2, n + 3, mServerButton);
    place(3, n + 3, mLoginButton);

    addKeyListener(this);
    if (mUpdateHostLabel)
        setContentSize(310, 250);
    else
        setContentSize(310, 200);

    reflowLayout();
    center();
}

void LoginDialog::postInit()
{
    setVisible(true);

    if (mUserField->getText().empty())
        mUserField->requestFocus();
    else
        mPassField->requestFocus();

    mLoginButton->setEnabled(canSubmit());
    mRegisterButton->setEnabled(Net::getLoginHandler()->isRegistrationEnabled()
        || !mLoginData->registerUrl.empty());
}

LoginDialog::~LoginDialog()
{
    delete2(mUpdateTypeModel);
    delete2(mUpdateListModel);
}

void LoginDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "login" && canSubmit())
    {
        prepareUpdate();
        mLoginData->registerLogin = false;
        client->setState(STATE_LOGIN_ATTEMPT);
    }
    else if (eventId == "server")
    {
        close();
    }
    else if (eventId == "register")
    {
        if (Net::getLoginHandler()->isRegistrationEnabled())
        {
            prepareUpdate();
            client->setState(STATE_REGISTER_PREP);
        }
        else if (!mLoginData->registerUrl.empty())
        {
            const std::string &url = mLoginData->registerUrl;
            urlListener.url = url;
            // TRANSLATORS: question dialog
            ConfirmDialog *const confirmDlg = new ConfirmDialog(
                _("Open register url"), url, SOUND_REQUEST, false, true);
            confirmDlg->postInit();
            confirmDlg->addActionListener(&urlListener);
        }
    }
    else if (eventId == "customhost")
    {
        mUpdateHostText->setVisible(mCustomUpdateHost->isSelected());
    }
    else if (eventId == "updateselect")
    {
        mCustomUpdateHost->setSelected(false);
        mUpdateHostText->setVisible(false);
    }
}

void LoginDialog::keyPressed(KeyEvent &event)
{
    if (event.isConsumed())
    {
        mLoginButton->setEnabled(canSubmit());
        return;
    }

    const int actionId = event.getActionId();
    if (actionId == static_cast<int>(Input::KEY_GUI_CANCEL))
    {
        action(ActionEvent(nullptr, mServerButton->getActionEventId()));
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_SELECT)
             || actionId == static_cast<int>(Input::KEY_GUI_SELECT2))
    {
        action(ActionEvent(nullptr, mLoginButton->getActionEventId()));
    }
    else
    {
        mLoginButton->setEnabled(canSubmit());
    }
}

bool LoginDialog::canSubmit() const
{
    return !mUserField->getText().empty() &&
        !mPassField->getText().empty() &&
        client->getState() == STATE_LOGIN;
}

void LoginDialog::prepareUpdate()
{
    mLoginData->username = mUserField->getText();
    mLoginData->password = mPassField->getText();
    mLoginData->remember = mKeepCheck->isSelected();
    int updateType = mUpdateTypeDropDown->getSelected();

    if (mCustomUpdateHost->isSelected()
        && !mUpdateHostText->getText().empty())
    {
        updateType |= LoginData::Upd_Custom;
        serverConfig.setValue("customUpdateHost",
            mUpdateHostText->getText());

        if (checkPath(mUpdateHostText->getText()))
        {
            mLoginData->updateHost = mUpdateHostText->getText();
            *mUpdateHost = mUpdateHostText->getText();
        }
        else
        {
            mLoginData->updateHost.clear();
            (*mUpdateHost).clear();
        }
    }
    else
    {
        std::string str;
        if (mUpdateHostDropDown)
        {
            str = mUpdateHostDropDown->getSelectedString();
        }
        else if (mLoginData->updateHost.empty()
                 && !mLoginData->updateHosts.empty())
        {
            str = mLoginData->updateHosts[0];
        }
        serverConfig.setValue("updateHost2", str);
        if (!str.empty() && checkPath(str))
        {
            mLoginData->updateHost = str;
            *mUpdateHost = str;
        }
        else
        {
            mLoginData->updateHost.clear();
            (*mUpdateHost).clear();
        }
    }

    mLoginData->updateType = updateType;
    serverConfig.setValue("updateType", updateType);

    mRegisterButton->setEnabled(false);
    mServerButton->setEnabled(false);
    mLoginButton->setEnabled(false);

    LoginDialog::savedPassword = mPassField->getText();
    if (mLoginData->remember)
        LoginDialog::savedPasswordKey = mServerName;
    else
        LoginDialog::savedPasswordKey = "-";
}

void LoginDialog::close()
{
    client->setState(STATE_SWITCH_SERVER);
    Window::close();
}
