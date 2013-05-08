/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2013  The ManaPlus Developers
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

#include "gui/logindialog.h"

#include "client.h"
#include "configuration.h"
#include "keydata.h"
#include "keyevent.h"

#include "gui/confirmdialog.h"
#include "gui/okdialog.h"
#include "gui/sdlinput.h"

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

struct OpenUrlListener : public gcn::ActionListener
{
    OpenUrlListener() :
        url()
    {
    }

    A_DELETE_COPY(OpenUrlListener)

    void action(const gcn::ActionEvent &event) override
    {
        if (event.getId() == "yes")
            openBrowser(url);
    }

    std::string url;
} urlListener;

const char *UPDATE_TYPE_TEXT[3] =
{
    // TRANSLATORS: update type
    N_("Normal"),
    // TRANSLATORS: update type
    N_("Auto Close"),
    // TRANSLATORS: update type
    N_("Skip"),
};

class UpdateTypeModel final : public gcn::ListModel
{
    public:
        UpdateTypeModel()
        { }

        A_DELETE_COPY(UpdateTypeModel)

        virtual ~UpdateTypeModel()
        { }

        virtual int getNumberOfElements() override
        {
            return 3;
        }

        virtual std::string getElementAt(int i) override
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";
            return gettext(UPDATE_TYPE_TEXT[i]);
        }
};

class UpdateListModel final : public gcn::ListModel
{
    public:
        explicit UpdateListModel(LoginData *const data) :
            gcn::ListModel(),
            mLoginData(data)
        {
        }

        A_DELETE_COPY(UpdateListModel)

        virtual ~UpdateListModel()
        { }

        virtual int getNumberOfElements() override
        {
            if (!mLoginData)
                return 0;
            return static_cast<int>(mLoginData->updateHosts.size());
        }

        virtual std::string getElementAt(int i) override
        {
            if (!mLoginData || i >= getNumberOfElements() || i < 0)
                return "???";
            return mLoginData->updateHosts[i];
        }

    protected:
        LoginData *mLoginData;
};

LoginDialog::LoginDialog(LoginData *const data, std::string serverName,
                         std::string *const updateHost):
    // TRANSLATORS: login dialog name
    Window(_("Login"), false, nullptr, "login.xml"),
    gcn::ActionListener(),
    gcn::KeyListener(),
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
    delete mUpdateTypeModel;
    mUpdateTypeModel = nullptr;
    delete mUpdateListModel;
    mUpdateListModel = nullptr;
}

void LoginDialog::action(const gcn::ActionEvent &event)
{
    const std::string &eventId = event.getId();
    if (eventId == "login" && canSubmit())
    {
        prepareUpdate();
        mLoginData->registerLogin = false;
        Client::setState(STATE_LOGIN_ATTEMPT);
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
            Client::setState(STATE_REGISTER_PREP);
        }
        else if (!mLoginData->registerUrl.empty())
        {
            const std::string &url = mLoginData->registerUrl;
            urlListener.url = url;
            // TRANSLATORS: question dialog
            ConfirmDialog *const confirmDlg = new ConfirmDialog(
                _("Open register url"), url, false, true);
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

void LoginDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    if (keyEvent.isConsumed())
    {
        mLoginButton->setEnabled(canSubmit());
        return;
    }

    const int actionId = static_cast<KeyEvent*>(
        &keyEvent)->getActionId();
    if (actionId == static_cast<int>(Input::KEY_GUI_CANCEL))
    {
        action(gcn::ActionEvent(nullptr, mServerButton->getActionEventId()));
    }
    else if (actionId == static_cast<int>(Input::KEY_GUI_SELECT)
             || actionId == static_cast<int>(Input::KEY_GUI_SELECT2))
    {
        action(gcn::ActionEvent(nullptr, mLoginButton->getActionEventId()));
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
        Client::getState() == STATE_LOGIN;
}

void LoginDialog::prepareUpdate()
{
    mLoginData->username = mUserField->getText();
    mLoginData->password = mPassField->getText();
    mLoginData->remember = mKeepCheck->isSelected();
    int updateType = mUpdateTypeDropDown->getSelected();

    if (mCustomUpdateHost->isSelected()
        && mUpdateHostText->getText().empty())
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
        std::string str = "";
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
    Client::setState(STATE_SWITCH_SERVER);
    Window::close();
}
