/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/okdialog.h"
#include "gui/sdlinput.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/passwordfield.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/gettext.h"
#include "utils/paths.h"

#include "debug.h"

std::string LoginDialog::savedPassword("");
std::string LoginDialog::savedPasswordKey("");


const char *UPDATE_TYPE_TEXT[3] =
{
    N_("Normal"),
    N_("Auto Close"),
    N_("Skip"),
};

class UpdateTypeModel : public gcn::ListModel
{
public:
    virtual ~UpdateTypeModel()
    { }

    virtual int getNumberOfElements()
    {
        return 3;
    }

    virtual std::string getElementAt(int i)
    {
        if (i >= getNumberOfElements() || i < 0)
            return _("???");

        return gettext(UPDATE_TYPE_TEXT[i]);
    }
};

class UpdateListModel : public gcn::ListModel
{
    public:
        UpdateListModel(LoginData *const data) :
            gcn::ListModel(),
            mLoginData(data)
        {
        }

        virtual ~UpdateListModel()
        { }

        virtual int getNumberOfElements()
        {
            if (!mLoginData)
                return 0;
            return static_cast<int>(mLoginData->updateHosts.size());
        }

        virtual std::string getElementAt(int i)
        {
            if (!mLoginData || i >= getNumberOfElements() || i < 0)
                return _("???");

            return mLoginData->updateHosts[i];
        }
    protected:
        LoginData *mLoginData;
};

LoginDialog::LoginDialog(LoginData *const data, std::string serverName,
                         std::string *const updateHost):
    Window(_("Login"), false, nullptr, "login.xml"),
    ActionListener(),
    KeyListener(),
    mLoginData(data),
    mUserField(new TextField(mLoginData->username)),
    mPassField(new PasswordField(mLoginData->password)),
    mKeepCheck(new CheckBox(_("Remember username"), mLoginData->remember)),
    mUpdateTypeLabel(new Label(_("Update:"))),
    mUpdateHostLabel(nullptr),
    mUpdateTypeModel(new UpdateTypeModel()),
    mUpdateTypeDropDown(new DropDown(mUpdateTypeModel)),
    mServerButton(new Button(_("Change Server"), "server", this)),
    mLoginButton(new Button(_("Login"), "login", this)),
    mRegisterButton(new Button(_("Register"), "register", this)),
    mCustomUpdateHost(new CheckBox(_("Custom update host"),
        mLoginData->updateType & LoginData::Upd_Custom, this, "customhost")),
    mUpdateHostText(new TextField(serverConfig.getValue(
        "customUpdateHost", ""))),
    mUpdateListModel(nullptr),
    mUpdateHostDropDown(nullptr),
    mUpdateHost(updateHost),
    mServerName(serverName)
{
    gcn::Label *const serverLabel1 = new Label(_("Server:"));
    gcn::Label *const serverLabel2 = new Label(serverName);
    serverLabel2->adjustSize();
    gcn::Label *const userLabel = new Label(_("Name:"));
    gcn::Label *const passLabel = new Label(_("Password:"));
    if (mLoginData && mLoginData->updateHosts.size() > 1)
    {
        mUpdateHostLabel = new Label(strprintf(_("Update host: %s"),
            mLoginData->updateHost.c_str()));
        mUpdateListModel = new UpdateListModel(mLoginData);
        mUpdateHostDropDown = new DropDown(mUpdateListModel,
            this, "updateselect");
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
        setContentSize(300, 250);
    else
        setContentSize(300, 200);

    center();
    setVisible(true);

    if (mUserField->getText().empty())
        mUserField->requestFocus();
    else
        mPassField->requestFocus();

    mLoginButton->setEnabled(canSubmit());
    mRegisterButton->setEnabled(Net::getLoginHandler()
                                ->isRegistrationEnabled());
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
    if (event.getId() == "login" && canSubmit())
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
                mLoginData->updateHost = "";
                *mUpdateHost = "";
            }
        }
        else if (mUpdateHostDropDown)
        {
            const std::string str = mUpdateHostDropDown->getSelectedString();
            serverConfig.setValue("updateHost2", str);
            if (!str.empty() && checkPath(str))
            {
                mLoginData->updateHost = str;
                *mUpdateHost = str;
            }
            else
            {
                mLoginData->updateHost = "";
                *mUpdateHost = "";
            }
        }

        mLoginData->updateType = updateType;
        serverConfig.setValue("updateType", updateType);

        mLoginData->registerLogin = false;

        mRegisterButton->setEnabled(false);
        mServerButton->setEnabled(false);
        mLoginButton->setEnabled(false);

        LoginDialog::savedPassword = mPassField->getText();
        if (mLoginData->remember)
            LoginDialog::savedPasswordKey = mServerName;
        else
            LoginDialog::savedPasswordKey = "-";

        Client::setState(STATE_LOGIN_ATTEMPT);
    }
    else if (event.getId() == "server")
    {
        Client::setState(STATE_SWITCH_SERVER);
    }
    else if (event.getId() == "register")
    {
        mLoginData->username = mUserField->getText();
        mLoginData->password = mPassField->getText();

        Client::setState(STATE_REGISTER_PREP);
    }
    else if (event.getId() == "customhost")
    {
        mUpdateHostText->setVisible(mCustomUpdateHost->isSelected());
    }
    else if (event.getId() == "updateselect")
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
