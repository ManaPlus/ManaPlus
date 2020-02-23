/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#include "net/ea/loginhandler.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"
#include "settings.h"

#include "utils/cast.h"
#include "utils/dtor.h"

#include "net/logindata.h"

#include "net/ea/loginrecv.h"

#include "debug.h"

namespace Ea
{

LoginHandler::LoginHandler() :
    Net::LoginHandler()
{
    LoginRecv::mVersionResponse = false;
    LoginRecv::mRegistrationEnabled = true;
    LoginRecv::mUpdateHost.clear();
    LoginRecv::mWorlds.clear();
    LoginRecv::mToken.clear();
}

LoginHandler::~LoginHandler()
{
    delete_all(LoginRecv::mWorlds);
}

bool LoginHandler::isRegistrationEnabled() const
{
    return LoginRecv::mRegistrationEnabled;
}

void LoginHandler::getRegistrationDetails() const
{
    // Not supported, so move on
    client->setState(State::REGISTER);
}

void LoginHandler::loginAccount(LoginData *const loginData1) const
{
    if (loginData1 != nullptr)
    {
        loginData1->resetCharacterSlots();
        sendLoginRegister(loginData1->username, loginData1->password, "");
    }
}

void LoginHandler::chooseServer(const unsigned int server,
                                const bool persistentIp) const
{
    if (CAST_SIZE(server) >= LoginRecv::mWorlds.size() ||
        (LoginRecv::mWorlds[server] == nullptr))
    {
        return;
    }

    ServerInfo *const charServer = getCharServer();
    if (charServer != nullptr)
    {
        if (config.getBoolValue("usePersistentIP") || persistentIp)
        {
            charServer->hostname = settings.serverName;
        }
        else
        {
            charServer->hostname = ipToString(
                LoginRecv::mWorlds[server]->address);
        }
        charServer->port = LoginRecv::mWorlds[server]->port;
    }
    charServer->althostname = mServer.althostname;

    client->setState(State::UPDATE);
}

void LoginHandler::registerAccount(const LoginData *const loginData1) const
{
    if (loginData1 == nullptr)
        return;

    std::string username = loginData1->username;
    switch (loginData1->gender)
    {
        case Gender::FEMALE:
            username.append("_F");
            break;
        case Gender::MALE:
            username.append("_M");
            break;
        case Gender::HIDDEN:
            username.append("_O");
            break;
        case Gender::UNSPECIFIED:
        default:
            break;
    }

    sendLoginRegister(username, loginData1->password, loginData1->email);
}

const Worlds &LoginHandler::getWorlds() const
{
    return LoginRecv::mWorlds;
}

void LoginHandler::clearWorlds() const
{
    delete_all(LoginRecv::mWorlds);
    LoginRecv::mWorlds.clear();
}

void LoginHandler::loginOrRegister(LoginData *const data) const
{
    if (data == nullptr)
        return;

    logger->log("Username is %s", data->username.c_str());

    // Send login infos
    if (data->registerLogin)
        registerAccount(data);
    else
        loginAccount(data);

    // Clear the password, avoids auto login when returning to login
    data->password.clear();

    const bool remember = data->remember;
    if (remember)
        serverConfig.setValue("username", data->username);
    else
        serverConfig.setValue("username", "");
    serverConfig.setValue("remember", remember);
}

void LoginHandler::logout() const
{
}

void LoginHandler::changeEmail(const std::string &email A_UNUSED) const
{
}

void LoginHandler::unregisterAccount(const std::string &username A_UNUSED,
                                     const std::string &password
                                     A_UNUSED) const
{
}

const Token &LoginHandler::getToken() const
{
    return LoginRecv::mToken;
}

}  // namespace Ea
