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

#include "net/ea/charserverhandler.h"

#include "client.h"
#include "configuration.h"

#include "gui/charcreatedialog.h"
#include "gui/okdialog.h"

#include "net/ea/loginhandler.h"
#include "net/ea/eaprotocol.h"
#include "net/ea/gamehandler.h"
#include "net/ea/network.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "resources/chardb.h"

#include "debug.h"

extern Net::CharServerHandler *charServerHandler;

namespace Ea
{

extern ServerInfo mapServer;

CharServerHandler::CharServerHandler() :
    Net::CharServerHandler()
{
}

void CharServerHandler::setCharSelectDialog(CharSelectDialog *const window)
{
    mCharSelectDialog = window;
    updateCharSelectDialog();
}

void CharServerHandler::setCharCreateDialog(CharCreateDialog *const window)
{
    mCharCreateDialog = window;

    if (!mCharCreateDialog)
        return;

    StringVect attributes;
    attributes.push_back(_("Strength:"));
    attributes.push_back(_("Agility:"));
    attributes.push_back(_("Vitality:"));
    attributes.push_back(_("Intelligence:"));
    attributes.push_back(_("Dexterity:"));
    attributes.push_back(_("Luck:"));

    const Token &token =
        static_cast<LoginHandler*>(Net::getLoginHandler())->getToken();

    int minStat = CharDB::getMinStat();
    if (!minStat)
        minStat = 1;
    int maxStat = CharDB::getMaxStat();
    if (!maxStat)
        maxStat = 9;
    int sumStat = CharDB::getSumStat();
    if (!sumStat)
        sumStat = 30;

    mCharCreateDialog->setAttributes(attributes, sumStat, minStat, maxStat);
    mCharCreateDialog->setFixedGender(true, token.sex);
}

void CharServerHandler::requestCharacters()
{
    connect();
}

unsigned int CharServerHandler::baseSprite() const
{
    return EA_SPRITE_BASE;
}

unsigned int CharServerHandler::hairSprite() const
{
    return EA_SPRITE_HAIR;
}

unsigned int CharServerHandler::maxSprite() const
{
    return EA_SPRITE_VECTOREND;
}

void CharServerHandler::processCharLoginError(Net::MessageIn &msg) const
{
    switch (msg.readInt8())
    {
        case 0:
            errorMessage = _("Access denied. Most likely, there are "
                             "too many players on this server.");
            break;
        case 1:
            errorMessage = _("Cannot use this ID.");
            break;
        default:
            errorMessage = _("Unknown char-server failure.");
            break;
    }
    Client::setState(STATE_ERROR);
}

void CharServerHandler::processCharCreate(Net::MessageIn &msg,
                                          const bool withColors)
{
    Net::Character *const character = new Net::Character;
    readPlayerData(msg, character, withColors);
    mCharacters.push_back(character);

    updateCharSelectDialog();

    // Close the character create dialog
    if (mCharCreateDialog)
    {
        mCharCreateDialog->scheduleDelete();
        mCharCreateDialog = nullptr;
    }
}

void CharServerHandler::processCharCreateFailed(Net::MessageIn &msg)
{
    switch (msg.readInt8())
    {
        case 1:
        case 0:
        default:
            errorMessage = _("Failed to create character. Most "
                "likely the name is already taken.");
            break;
        case 2:
            errorMessage = _("Wrong name.");
            break;
        case 3:
            errorMessage = _("Incorrect stats.");
            break;
        case 4:
            errorMessage = _("Incorrect hair.");
            break;
        case 5:
            errorMessage = _("Incorrect slot.");
            break;
        case 6:
            errorMessage = _("Incorrect race.");
            break;
    }
    new OkDialog(_("Error"), errorMessage, DIALOG_ERROR);
    if (mCharCreateDialog)
        mCharCreateDialog->unlock();
}

void CharServerHandler::processCharDelete(Net::MessageIn &msg A_UNUSED)
{
    delete mSelectedCharacter;
    mCharacters.remove(mSelectedCharacter);
    mSelectedCharacter = nullptr;
    updateCharSelectDialog();
    unlockCharSelectDialog();
    new OkDialog(_("Info"), _("Character deleted."));
}

void CharServerHandler::processCharDeleteFailed(Net::MessageIn &msg A_UNUSED)
{
    unlockCharSelectDialog();
    new OkDialog(_("Error"), _("Failed to delete character."), DIALOG_ERROR);
}

void CharServerHandler::clear()
{
    delete_all(mCharacters);
    mCharacters.clear();
}

void CharServerHandler::processCharMapInfo(Net::MessageIn &msg,
                                           Network *const network,
                                           ServerInfo &server)
{
//    msg.skip(4); // CharID, must be the same as player_node->charID
    PlayerInfo::setCharId(msg.readInt32());
    GameHandler *const gh = static_cast<GameHandler*>(Net::getGameHandler());
    gh->setMap(msg.readString(16));
    if (config.getBoolValue("usePersistentIP"))
    {
        msg.readInt32();
        server.hostname = Client::getServerName();
    }
    else
    {
        server.hostname = ipToString(msg.readInt32());
    }
    server.port = msg.readInt16();

    // Prevent the selected local player from being deleted
    player_node = mSelectedCharacter->dummy;
    PlayerInfo::setBackend(mSelectedCharacter->data);

    mSelectedCharacter->dummy = nullptr;

    Net::getCharServerHandler()->clear();
    updateCharSelectDialog();

    if (network)
        network->disconnect();
    Client::setState(STATE_CONNECT_GAME);
}

void CharServerHandler::processChangeMapServer(Net::MessageIn &msg,
                                               Network *const network,
                                               ServerInfo &server) const
{
    GameHandler *const gh = static_cast<GameHandler*>(Net::getGameHandler());
    if (!gh || !network)
        return;
    gh->setMap(msg.readString(16));
    const int x = msg.readInt16();
    const int y = msg.readInt16();
    server.hostname = ipToString(msg.readInt32());
    server.port = msg.readInt16();

    network->disconnect();
    Client::setState(STATE_CHANGE_MAP);
    if (player_node)
    {
        player_node->setTileCoords(x, y);
        player_node->setMap(nullptr);
    }
}

} // namespace Ea
