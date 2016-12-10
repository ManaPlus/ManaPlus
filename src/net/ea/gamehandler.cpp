/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "game.h"

#include "net/ea/gamerecv.h"

#include "debug.h"

namespace Ea
{

GameHandler::GameHandler() :
    Net::GameHandler()
{
    GameRecv::mMap.clear();
    GameRecv::mCharID = BeingId_zero;
}

void GameHandler::who() const
{
}

void GameHandler::setMap(const std::string &map)
{
    GameRecv::mMap = map.substr(0, map.rfind('.'));
}

void GameHandler::clear() const
{
    GameRecv::mMap.clear();
    GameRecv::mCharID = BeingId_zero;
}

void GameHandler::initEngines() const
{
    if (!GameRecv::mMap.empty())
        Game::instance()->changeMap(GameRecv::mMap);
}

}  // namespace Ea
