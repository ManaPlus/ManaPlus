/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#include "net/tmwa/battlegroundhandler.h"

#include "debug.h"

extern Net::BattleGroundHandler *battleGroundHandler;

namespace TmwAthena
{

BattleGroundHandler::BattleGroundHandler()
{
    battleGroundHandler = this;
}

void BattleGroundHandler::registerBg(const BattleGroundTypeT &type
                                     A_UNUSED,
                                     const std::string &name A_UNUSED) const
{
}

void BattleGroundHandler::rekoveRequest(const std::string &name A_UNUSED) const
{
}

void BattleGroundHandler::beginAck(const bool result A_UNUSED,
                                   const std::string &bgName A_UNUSED,
                                   const std::string &gameName A_UNUSED) const
{
}

void BattleGroundHandler::checkState(const std::string &name A_UNUSED) const
{
}

}  // namespace TmwAthena
