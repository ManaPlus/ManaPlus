/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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
#include "logger.h"
#include "configuration.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "debug.h"

namespace Ea
{

LoginHandler::LoginHandler() :
        mVersionResponse(false),
        mRegistrationEnabled(true)
{
    mToken.clear();
}

LoginHandler::~LoginHandler()
{
    delete_all(mWorlds);
}

bool LoginHandler::isRegistrationEnabled()
{
    return mRegistrationEnabled;
}

void LoginHandler::getRegistrationDetails()
{
    // Not supported, so move on
    Client::setState(STATE_REGISTER);
}

void LoginHandler::loginAccount(LoginData *loginData)
{

    if (loginData)
    {
        // Since we're attempting to use the tAthena protocol,
        // let's reset the character slots to the good value,
        // in case we just logged out a Manaserv server
        // with a different config.
        loginData->resetCharacterSlots();

        sendLoginRegister(loginData->username, loginData->password);
    }
}

void LoginHandler::chooseServer(unsigned int server)
{
    if (server >= mWorlds.size() || !mWorlds[server])
        return;

    ServerInfo *charServer = getCharServer();
    if (charServer)
    {
        if (config.getBoolValue("usePersistentIP"))
            charServer->hostname = Client::getServerName();
        else
            charServer->hostname = ipToString(mWorlds[server]->address);
        charServer->port = mWorlds[server]->port;
    }

    Client::setState(STATE_UPDATE);
}

void LoginHandler::registerAccount(LoginData *loginData)
{
    if (!loginData)
        return;

    std::string username = loginData->username;
    username.append((loginData->gender == GENDER_FEMALE) ? "_F" : "_M");

    sendLoginRegister(username, loginData->password);
}

Worlds LoginHandler::getWorlds() const
{
    return mWorlds;
}

void LoginHandler::clearWorlds()
{
    delete_all(mWorlds);
    mWorlds.clear();
}

void LoginHandler::procecessCharPasswordResponse(Net::MessageIn &msg)
{
    // 0: acc not found, 1: success, 2: password mismatch, 3: pass too short
    int errMsg = msg.readInt8();
    // Successful pass change
    if (errMsg == 1)
    {
        Client::setState(STATE_CHANGEPASSWORD_SUCCESS);
    }
    // pass change failed
    else
    {
        switch (errMsg)
        {
            case 0:
                errorMessage =
                    _("Account was not found. Please re-login.");
                break;
            case 2:
                errorMessage = _("Old password incorrect.");
                break;
            case 3:
                errorMessage = _("New password too short.");
                break;
            default:
                errorMessage = _("Unknown error.");
                break;
        }
        Client::setState(STATE_ACCOUNTCHANGE_ERROR);
    }
}

void LoginHandler::processUpdateHost(Net::MessageIn &msg)
{
    int len;

    len = msg.readInt16() - 4;
    mUpdateHost = msg.readString(len);
    loginData.updateHost = mUpdateHost;

    logger->log("Received update host \"%s\" from login server.",
        mUpdateHost.c_str());
}

void LoginHandler::processLoginData(Net::MessageIn &msg)
{
    // Skip the length word
    msg.skip(2);    // size

    clearWorlds();

    int worldCount = (msg.getLength() - 47) / 32;

    mToken.session_ID1 = msg.readInt32();
    mToken.account_ID = msg.readInt32();
    mToken.session_ID2 = msg.readInt32();
    msg.skip(4);                           // old ip
    loginData.lastLogin = msg.readString(24);
    msg.skip(2);                           // 0 unused bytes

//    msg.skip(30);                           // unknown
    // reserve bits for future usage
    mToken.sex = (msg.readInt8() & 1) ? GENDER_MALE : GENDER_FEMALE;

    for (int i = 0; i < worldCount; i++)
    {
        WorldInfo *world = new WorldInfo;

        world->address = msg.readInt32();
        world->port = msg.readInt16();
        world->name = msg.readString(20);
        world->online_users = msg.readInt16();
        config.setValue("updatehost", mUpdateHost);
        world->updateHost = mUpdateHost;
        msg.skip(2);                        // maintenance
        msg.skip(2);                        // new

        logger->log("Network: Server: %s (%s:%d)", world->name.c_str(),
            ipToString(world->address), world->port);

        mWorlds.push_back(world);
    }
    Client::setState(STATE_WORLD_SELECT);
}

void LoginHandler::processLoginError(Net::MessageIn &msg)
{
    int code = msg.readInt8();
    logger->log("Login::error code: %i", code);

    switch (code)
    {
        case 0:
            errorMessage = _("Unregistered ID.");
            break;
        case 1:
            errorMessage = _("Wrong password.");
            break;
        case 2:
            errorMessage = _("Account expired.");
            break;
        case 3:
            errorMessage = _("Rejected from server.");
            break;
        case 4:
            errorMessage = _("You have been permanently banned from "
                              "the game. Please contact the GM team.");
            break;
        case 5:
            errorMessage = _("Client too old.");
            break;
        case 6:
            errorMessage = strprintf(_("You have been temporarily "
                                        "banned from the game until "
                                        "%s.\nPlease contact the GM "
                                        "team via the forums."),
                                        msg.readString(20).c_str());
            break;
        case 7:
            errorMessage = _("Server overpopulated.");
            break;
        case 9:
            errorMessage = _("This user name is already taken.");
            break;
        case 10:
            errorMessage = _("Wrong name.");
            break;
        case 99:
            errorMessage = _("Username permanently erased.");
            break;
        default:
            errorMessage = _("Unknown error.");
            break;
    }
    Client::setState(STATE_ERROR);
}

void LoginHandler::logout()
{
    // TODO
}

void LoginHandler::changeEmail(const std::string &email A_UNUSED)
{
    // TODO
}

void LoginHandler::unregisterAccount(const std::string &username A_UNUSED,
                                     const std::string &password A_UNUSED)
{
    // TODO
}

} // namespace Ea
