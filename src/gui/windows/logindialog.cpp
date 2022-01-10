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

#include "gui/windows/logindialog.h"

#include "client.h"
#include "configuration.h"

#include "const/sound.h"

#include "fs/paths.h"

#include "gui/models/updatelistmodel.h"
#include "gui/models/updatetypemodel.h"

#include "gui/windows/confirmdialog.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/createwidget.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/passwordfield.h"
#include "gui/widgets/layoutcell.h"

#include "listeners/openurllistener.h"

#include "net/charserverhandler.h"
#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/updatetypeoperators.h"

#include "utils/delete2.h"
#include "utils/stdmove.h"

#include "debug.h"

std::string LoginDialog::savedPassword;
std::string LoginDialog::savedPasswordKey;

namespace
{
    OpenUrlListener urlListener;
}  // namespace

LoginDialog::LoginDialog(LoginData &data,
                         ServerInfo *const server,
                         std::string *const updateHost) :
    // TRANSLATORS: login dialog name
    Window(_("Login"), Modal_false, nullptr, "login.xml"),
    ActionListener(),
    KeyListener(),
    mLoginData(&data),
    mServer(server),
    mUserField(new TextField(this, mLoginData->username,
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    mPassField(new PasswordField(this, mLoginData->password)),
#ifdef SAVE_PASSWORD
    // TRANSLATORS: login dialog label
    mKeepCheck(new CheckBox(this, _("Remember user and password"),
#else
    // TRANSLATORS: login dialog label
    mKeepCheck(new CheckBox(this, _("Remember username"),
#endif
    mLoginData->remember, nullptr, std::string())),
    // TRANSLATORS: login dialog label
    mUpdateTypeLabel(new Label(this, _("Update:"))),
    mUpdateTypeModel(new UpdateTypeModel),
    mUpdateTypeDropDown(new DropDown(this, mUpdateTypeModel,
        false, Modal_false, nullptr, std::string())),
    // TRANSLATORS: login dialog button
    mServerButton(new Button(this, _("Change Server"), "server",
        BUTTON_SKIN, this)),
    // TRANSLATORS: login dialog button
    mLoginButton(new Button(this, _("Login"), "login",
        BUTTON_SKIN, this)),
    // TRANSLATORS: login dialog button
    mRegisterButton(new Button(this, _("Register"), "register",
        BUTTON_SKIN, this)),
    // TRANSLATORS: login dialog checkbox
    mCustomUpdateHost(new CheckBox(this, _("Custom update host"),
        (mLoginData->updateType & UpdateType::Custom) != 0,
        this, "customhost")),
    mUpdateHostText(new TextField(this, serverConfig.getValue(
        "customUpdateHost", ""),
        LoseFocusOnTab_true, nullptr, std::string(), false)),
    mUpdateListModel(nullptr),
    mUpdateHostDropDown(nullptr),
    mUpdateHost(updateHost),
    mServerName(server->hostname)
{
    setCloseButton(true);
    setWindowName("Login");

    if (charServerHandler != nullptr)
        charServerHandler->clear();

    mergeUpdateHosts();

    // TRANSLATORS: login dialog label
    Label *const serverLabel1 = new Label(this, _("Server:"));
    Label *const serverLabel2 = new Label(this, mServerName);
    serverLabel2->adjustSize();
    // TRANSLATORS: login dialog label
    Label *const userLabel = new Label(this, _("Name:"));
    // TRANSLATORS: login dialog label
    Label *const passLabel = new Label(this, _("Password:"));
    if (mServer->updateHosts.size() > 1)
    {
        mUpdateListModel = new UpdateListModel(mServer);
        mUpdateHostDropDown = new DropDown(this, mUpdateListModel,
            false, Modal_false, this, "updateselect");
        const std::string str = serverConfig.getValue("updateHost2", "");
        if (!str.empty())
            mUpdateHostDropDown->setSelectedString(str);
    }
    else
    {
        mUpdateListModel = nullptr;
        mUpdateHostDropDown = nullptr;
    }
    mUpdateHostText->adjustSize();

    if (mPassField->getText().empty() &&
        !LoginDialog::savedPassword.empty())
    {
        mPassField->setText(LoginDialog::savedPassword);
    }

    mUpdateTypeDropDown->setActionEventId("updatetype");
    mUpdateTypeDropDown->setSelected((mLoginData->updateType
        | UpdateType::Custom) ^ CAST_S32(UpdateType::Custom));

    if (!mCustomUpdateHost->isSelected())
        mUpdateHostText->setVisible(Visible_false);

    mUserField->setActionEventId("login");
    mPassField->setActionEventId("login");

    mUserField->addKeyListener(this);
    mPassField->addKeyListener(this);
    mUserField->addActionListener(this);
    mPassField->addActionListener(this);

    place(0, 0, serverLabel1, 1, 1);
    place(1, 0, serverLabel2, 8, 1);
    place(0, 1, userLabel, 1, 1);
    place(1, 1, mUserField, 8, 1);
    place(0, 2, passLabel, 1, 1);
    place(1, 2, mPassField, 8, 1);
    place(0, 6, mUpdateTypeLabel, 1, 1);
    place(1, 6, mUpdateTypeDropDown, 8, 1);
    int n = 7;
    if (mUpdateHostDropDown != nullptr)
    {
        place(0, 7, mUpdateHostDropDown, 9, 1);
        n += 1;
    }
    place(0, n, mCustomUpdateHost, 9, 1);
    place(0, n + 1, mUpdateHostText, 9, 1);
    place(0, n + 2, mKeepCheck, 9, 1);
    place(0, n + 3, mRegisterButton, 1, 1).setHAlign(LayoutCell::LEFT);
    place(2, n + 3, mServerButton, 1, 1);
    place(3, n + 3, mLoginButton, 1, 1);

    addKeyListener(this);
}

void LoginDialog::postInit()
{
    Window::postInit();
    setVisible(Visible_true);

    const int h = 200;
    if (mUpdateHostDropDown != nullptr)
        setContentSize(310, 250);
    setContentSize(310, h);
#ifdef ANDROID
    setDefaultSize(310, h, ImagePosition::UPPER_CENTER, 0, 0);
#else  // ANDROID

    setDefaultSize(310, h, ImagePosition::CENTER, 0, 0);
#endif  // ANDROID

    center();
    loadWindowState();
    reflowLayout(0, 0);

#ifdef SAVE_PASSWORD
    mPassField->setText(LoginDialog::savedPassword);
#else
    if (mUserField->getText().empty())
        mUserField->requestFocus();
    else
        mPassField->requestFocus();
#endif

    mLoginButton->setEnabled(canSubmit());
    if (loginHandler != nullptr)
    {
        mRegisterButton->setEnabled(loginHandler->isRegistrationEnabled()
            || !mLoginData->registerUrl.empty());
    }
    else
    {
        mRegisterButton->setEnabled(false);
    }
}

LoginDialog::~LoginDialog()
{
    if (mUpdateTypeDropDown != nullptr)
        mUpdateTypeDropDown->hideDrop(false);
    if (mUpdateHostDropDown != nullptr)
        mUpdateHostDropDown->hideDrop(false);

    delete2(mUpdateTypeModel)
    delete2(mUpdateListModel)
}

void LoginDialog::action(const ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "login" && canSubmit())
    {
        prepareUpdate();
        mLoginData->registerLogin = false;
        client->setState(State::LOGIN_ATTEMPT);
    }
    else if (eventId == "server")
    {
        close();
    }
    else if (eventId == "register")
    {
        if (loginHandler->isRegistrationEnabled())
        {
            prepareUpdate();
            client->setState(State::REGISTER_PREP);
        }
        else if (!mLoginData->registerUrl.empty())
        {
            const std::string &url = mLoginData->registerUrl;
            urlListener.url = url;
            ConfirmDialog *const confirmDlg = CREATEWIDGETR(ConfirmDialog,
                // TRANSLATORS: question dialog
                _("Open register url"),
                url,
                SOUND_REQUEST,
                false,
                Modal_true,
                nullptr);
            confirmDlg->addActionListener(&urlListener);
        }
    }
    else if (eventId == "customhost")
    {
        mUpdateHostText->setVisible(fromBool(
            mCustomUpdateHost->isSelected(), Visible));
    }
    else if (eventId == "updateselect")
    {
        mCustomUpdateHost->setSelected(false);
        mUpdateHostText->setVisible(Visible_false);
    }
}

void LoginDialog::keyPressed(KeyEvent &event)
{
    if (event.isConsumed())
    {
        mLoginButton->setEnabled(canSubmit());
        return;
    }

    const InputActionT actionId = event.getActionId();
    if (actionId == InputAction::GUI_CANCEL)
    {
        action(ActionEvent(nullptr, mServerButton->getActionEventId()));
    }
#ifdef USE_SDL2
    else if (actionId == InputAction::GUI_SELECT2)
#else  // USE_SDL2
    else if (actionId == InputAction::GUI_SELECT ||
             actionId == InputAction::GUI_SELECT2)
#endif  // USE_SDL2
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
        client->getState() == State::LOGIN;
}

void LoginDialog::prepareUpdate()
{
    mLoginData->username = mUserField->getText();
    mLoginData->password = mPassField->getText();
    mLoginData->remember = mKeepCheck->isSelected();
    UpdateTypeT updateType = static_cast<UpdateTypeT>(
        mUpdateTypeDropDown->getSelected());

    if (mCustomUpdateHost->isSelected()
        && !mUpdateHostText->getText().empty())
    {
        updateType = static_cast<UpdateTypeT>(
            updateType | UpdateType::Custom);
        serverConfig.setValue("customUpdateHost",
            mUpdateHostText->getText());

        if (checkPath(mUpdateHostText->getText()))
        {
            mLoginData->updateHost = mUpdateHostText->getText();
            *mUpdateHost = mLoginData->updateHost;
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
        if (mUpdateHostDropDown != nullptr)
        {
            const int sel = mUpdateHostDropDown->getSelected();
            if (sel >= 0)
            {
                const HostsGroup &group = mServer->updateHosts[sel];
                if (!group.hosts.empty())
                {
                    str = group.hosts[0];
                    mLoginData->updateHosts = group.hosts;
                    serverConfig.setValue("updateHost2", group.name);
                }
                else
                {
                    serverConfig.setValue("updateHost2", "");
                }
            }
        }
        else if (mLoginData->updateHost.empty()
                 && !mLoginData->updateHosts.empty())
        {
            str = mLoginData->updateHosts[0];
            serverConfig.setValue("updateHost2", str);
        }
        if (!str.empty() && checkPath(str))
        {
            mLoginData->updateHost = str;
            *mUpdateHost = STD_MOVE(str);
        }
        else
        {
            mLoginData->updateHost.clear();
            (*mUpdateHost).clear();
        }
    }

    mLoginData->updateType = updateType;
    serverConfig.setValue("updateType", CAST_S32(updateType));

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
    client->setState(State::SWITCH_SERVER);
    Window::close();
}

void LoginDialog::mergeUpdateHosts()
{
    HostsGroup group;

    group.name = mServer->defaultHostName;
    if (group.name.empty())
    {
        // TRANSLATORS: update hosts group default name
        group.name = _("default updates");
    }
    group.hosts = mLoginData->updateHosts;
    mServer->updateHosts.insert(mServer->updateHosts.begin(), group);
}
