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

#include "gui/okdialog.h"
#include "gui/sdlinput.h"

#include "gui/widgets/button.h"
#include "gui/widgets/checkbox.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/passwordfield.h"
#include "gui/widgets/textfield.h"

#include "net/logindata.h"
#include "net/loginhandler.h"
#include "net/net.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

static const int MAX_SERVER_LIST_SIZE = 15;
static const int LOGIN_DIALOG_WIDTH = 300;
static const int LOGIN_DIALOG_HEIGHT = 140;
static const int FIELD_WIDTH = LOGIN_DIALOG_WIDTH - 70;

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

LoginDialog::LoginDialog(LoginData *data, std::string serverName,
                         std::string *updateHost):
    Window(_("Login"), false, nullptr, "login.xml"),
    mLoginData(data),
    mUpdateHost(updateHost),
    mServerName(serverName)
{

    gcn::Label *serverLabel1 = new Label(_("Server:"));
    gcn::Label *serverLabel2 = new Label(serverName);
    serverLabel2->adjustSize();
    gcn::Label *userLabel = new Label(_("Name:"));
    gcn::Label *passLabel = new Label(_("Password:"));
    mCustomUpdateHost = new CheckBox(_("Custom update host"),
        mLoginData->updateType & LoginData::Upd_Custom, this, "customhost");

    mUpdateHostText = new TextField(serverConfig.getValue(
        "customUpdateHost", ""));

    mUpdateHostText->adjustSize();

    mUserField = new TextField(mLoginData->username);
    mPassField = new PasswordField(mLoginData->password);

    if (mPassField->getText().empty() && LoginDialog::savedPassword != "")
        mPassField->setText(LoginDialog::savedPassword);

    mKeepCheck = new CheckBox(_("Remember username"), mLoginData->remember);
    mUpdateTypeLabel = new Label(_("Update:"));
    mUpdateTypeModel = new UpdateTypeModel();
    mUpdateTypeDropDown = new DropDown(mUpdateTypeModel);
    mUpdateTypeDropDown->setActionEventId("updatetype");
    mUpdateTypeDropDown->setSelected((mLoginData->updateType
        | LoginData::Upd_Custom) ^ LoginData::Upd_Custom);

    if (!mCustomUpdateHost->isSelected())
        mUpdateHostText->setVisible(false);

    mRegisterButton = new Button(_("Register"), "register", this);
    mServerButton = new Button(_("Change Server"), "server", this);
    mLoginButton = new Button(_("Login"), "login", this);

    mUserField->setActionEventId("login");
    mPassField->setActionEventId("login");

    mUserField->addKeyListener(this);
    mPassField->addKeyListener(this);
    mUserField->addActionListener(this);
    mPassField->addActionListener(this);

    place(0, 0, serverLabel1);
    place(1, 0, serverLabel2, 8);
    place(0, 1, userLabel);
    place(0, 2, passLabel);
    place(1, 1, mUserField, 8);
    place(1, 2, mPassField, 8);
    place(0, 6, mUpdateTypeLabel, 1);
    place(1, 6, mUpdateTypeDropDown, 8);
    place(0, 7, mCustomUpdateHost, 9);
    place(0, 8, mUpdateHostText, 9);
    place(0, 9, mKeepCheck, 9);
    place(0, 10, mRegisterButton).setHAlign(LayoutCell::LEFT);
    place(2, 10, mServerButton);
    place(3, 10, mLoginButton);

    addKeyListener(this);
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
}

void LoginDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "login" && canSubmit())
    {
        mLoginData->username = mUserField->getText();
        mLoginData->password = mPassField->getText();
        mLoginData->remember = mKeepCheck->isSelected();
        int updateType = mUpdateTypeDropDown->getSelected();

        if (mCustomUpdateHost->isSelected())
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
}

void LoginDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    gcn::Key key = keyEvent.getKey();

    if (key.getValue() == Key::ESCAPE)
        action(gcn::ActionEvent(nullptr, mServerButton->getActionEventId()));
    else if (key.getValue() == Key::ENTER)
        action(gcn::ActionEvent(nullptr, mLoginButton->getActionEventId()));
    else
        mLoginButton->setEnabled(canSubmit());
}

bool LoginDialog::canSubmit() const
{
    return !mUserField->getText().empty() &&
           !mPassField->getText().empty() &&
           Client::getState() == STATE_LOGIN;
}
