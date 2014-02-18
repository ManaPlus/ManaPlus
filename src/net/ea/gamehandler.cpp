/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "net/ea/gamehandler.h"

#include "client.h"
#include "game.h"
#include "notifications.h"
#include "notifymanager.h"

#include "being/localplayer.h"

#include "gui/windows/okdialog.h"

#include "net/messagein.h"

#include "debug.h"

namespace Ea
{

GameHandler::GameHandler() :
    mMap(),
    mCharID(0)
{
}

void GameHandler::who() const
{
}

void GameHandler::setMap(const std::string &map)
{
    mMap = map.substr(0, map.rfind("."));
}

void GameHandler::processMapLogin(Net::MessageIn &msg) const
{
    unsigned char direction;
    uint16_t x, y;
    msg.readInt32();   // server tick
    msg.readCoordinates(x, y, direction);
    msg.skip(2);      // 0x0505
    logger->log("Protocol: Player start position: (%d, %d),"
                " Direction: %d", x, y, direction);
    // Switch now or we'll have problems
    client->setState(STATE_GAME);
    if (player_node)
        player_node->setTileCoords(x, y);
}

void GameHandler::processWhoAnswer(Net::MessageIn &msg) const
{
    NotifyManager::notify(NotifyManager::ONLINE_USERS, msg.readInt32());
}

void GameHandler::processCharSwitchResponse(Net::MessageIn &msg) const
{
    if (msg.readInt8())
        client->setState(STATE_SWITCH_CHARACTER);
}

void GameHandler::processMapQuitResponse(Net::MessageIn &msg) const
{
    if (msg.readInt8())
        new OkDialog(_("Game"), _("Request to quit denied!"), DIALOG_ERROR);
}

void GameHandler::clear()
{
    mMap.clear();
    mCharID = 0;
}

void GameHandler::initEngines() const
{
    if (!mMap.empty())
        Game::instance()->changeMap(mMap);
}

}  // namespace Ea
